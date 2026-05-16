#!/usr/bin/env python3
"""
SADSE-like eksperiment na MNIST-u.

Ovdje SADSE interpretiramo kao Self-Adaptive Discriminant Spectral Embedding:
  - self-adaptive: lokalne skale sigma_i u kernelu,
  - discriminant: koristi label informacije za within-class i between-class graf.

Napomena: ovo više nije čisto unsupervised embedding, jer koristi stvarne cifre pri
konstrukciji grafa. Zato rezultat pokazuje koliko diskriminativni spektralni graf
može razdvojiti poznate klase, a ne realnu potpuno nenadziranu segmentaciju.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

from mnist_spectral_embedding import load_mnist, pairwise_sq_dists, pca_features
from mnist_spectral_clustering import (
    ari_score,
    best_permutation_accuracy,
    contingency_matrix,
    kmeans,
    nmi_score,
    plot_contingency,
    purity_score,
    stratified_subsample_digits,
)


def adaptive_discriminant_graphs(
    X: np.ndarray,
    y: np.ndarray,
    k_within: int,
    k_between: int,
) -> tuple[np.ndarray, np.ndarray]:
    """Konstruiši within-class i between-class self-tuning grafove."""
    n = X.shape[0]
    D2 = pairwise_sq_dists(X)
    np.fill_diagonal(D2, np.inf)

    # Lokalna skala: udaljenost do k-tog najbližeg susjeda bez obzira na klasu.
    k_scale = min(max(k_within + k_between, 5), n - 1)
    scale_idx = np.argpartition(D2, kth=k_scale, axis=1)[:, :k_scale]
    kth_d2 = np.take_along_axis(D2, scale_idx, axis=1).max(axis=1)
    sigma = np.sqrt(np.maximum(kth_d2, 1e-12))

    Ww = np.zeros((n, n), dtype=np.float64)
    Wb = np.zeros((n, n), dtype=np.float64)

    for i in range(n):
        same = np.flatnonzero(y == y[i])
        same = same[same != i]
        diff = np.flatnonzero(y != y[i])

        if len(same) > 0:
            kw = min(k_within, len(same))
            js = same[np.argpartition(D2[i, same], kth=kw - 1)[:kw]]
            Ww[i, js] = np.exp(-D2[i, js] / (sigma[i] * sigma[js] + 1e-12))

        if len(diff) > 0 and k_between > 0:
            kb = min(k_between, len(diff))
            js = diff[np.argpartition(D2[i, diff], kth=kb - 1)[:kb]]
            Wb[i, js] = np.exp(-D2[i, js] / (sigma[i] * sigma[js] + 1e-12))

    Ww = np.maximum(Ww, Ww.T)
    Wb = np.maximum(Wb, Wb.T)
    np.fill_diagonal(Ww, 0.0)
    np.fill_diagonal(Wb, 0.0)
    return Ww, Wb


def laplacian(W: np.ndarray) -> np.ndarray:
    return np.diag(W.sum(axis=1)) - W


def sadse_embedding(
    X: np.ndarray,
    y: np.ndarray,
    dim: int,
    k_within: int,
    k_between: int,
    reg: float,
) -> tuple[np.ndarray, np.ndarray]:
    """Generalizovani diskriminativni spektralni embedding.

    Maksimiziramo between-class razdvajanje uz ograničenje da within-class
    udaljenosti ostanu male:

        max_Z  tr(Z^T L_b Z)
        s.t.   Z^T (L_w + reg I) Z = I

    Rješavamo preko simetrične transformacije bez scipy-ja.
    """
    Ww, Wb = adaptive_discriminant_graphs(X, y, k_within, k_between)
    Lw = laplacian(Ww)
    Lb = laplacian(Wb)

    B = Lw + reg * np.eye(X.shape[0])
    b_vals, b_vecs = np.linalg.eigh(B)
    b_vals = np.maximum(b_vals, 1e-10)
    B_inv_sqrt = (b_vecs / np.sqrt(b_vals)) @ b_vecs.T

    M = B_inv_sqrt @ Lb @ B_inv_sqrt
    M = 0.5 * (M + M.T)
    vals, vecs = np.linalg.eigh(M)
    order = np.argsort(vals)[::-1]
    vals = vals[order]
    vecs = vecs[:, order]

    Z = B_inv_sqrt @ vecs[:, :dim]
    Z = (Z - Z.mean(axis=0)) / (Z.std(axis=0) + 1e-12)
    return Z, vals


def plot_2d(Z: np.ndarray, y: np.ndarray, out: Path) -> None:
    fig, ax = plt.subplots(figsize=(9, 7), dpi=160)
    sc = ax.scatter(Z[:, 0], Z[:, 1], c=y, cmap="tab10", s=13, alpha=0.85, linewidths=0)
    ax.set_title("SADSE-like MNIST embedding — 2D")
    ax.set_xlabel("SADSE koordinata 1")
    ax.set_ylabel("SADSE koordinata 2")
    cbar = fig.colorbar(sc, ax=ax, ticks=sorted(np.unique(y)))
    cbar.set_label("Cifra")
    plt.tight_layout()
    plt.savefig(out)
    print(f"Sačuvan 2D plot: {out}")


def plot_3d_views(Z: np.ndarray, y: np.ndarray, out: Path) -> None:
    fig = plt.figure(figsize=(13, 11), dpi=160)
    views = [(20, 35), (20, 125), (55, 35), (10, 215)]
    sc = None
    for idx, (elev, azim) in enumerate(views, start=1):
        ax = fig.add_subplot(2, 2, idx, projection="3d")
        sc = ax.scatter(Z[:, 0], Z[:, 1], Z[:, 2], c=y, cmap="tab10", s=8, alpha=0.82, linewidths=0)
        ax.view_init(elev=elev, azim=azim)
        ax.set_title(f"elev={elev}, azim={azim}")
        ax.set_xlabel("c1")
        ax.set_ylabel("c2")
        ax.set_zlabel("c3")
    fig.suptitle("SADSE-like MNIST embedding — 3D, više perspektiva", fontsize=15)
    cbar = fig.colorbar(sc, ax=fig.axes, ticks=sorted(np.unique(y)), shrink=0.65, pad=0.03)
    cbar.set_label("Cifra")
    plt.savefig(out, bbox_inches="tight")
    print(f"Sačuvan 3D plot: {out}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--digits", type=str, default="0,1,2,3,4")
    parser.add_argument("--n", type=int, default=1000)
    parser.add_argument("--pca-dim", type=int, default=50)
    parser.add_argument("--k-within", type=int, default=10)
    parser.add_argument("--k-between", type=int, default=5)
    parser.add_argument("--reg", type=float, default=1e-2)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--data-dir", type=Path, default=Path("data/mnist"))
    parser.add_argument("--out2d", type=Path, default=Path("mnist_sadse_0_4_2d.png"))
    parser.add_argument("--out3d", type=Path, default=Path("mnist_sadse_0_4_3d_views.png"))
    parser.add_argument("--out-cont", type=Path, default=Path("mnist_sadse_0_4_contingency.png"))
    args = parser.parse_args()

    digits = [int(d.strip()) for d in args.digits.split(",") if d.strip()]
    clusters = len(digits)

    X, y = load_mnist(args.data_dir)
    X_sub, y_sub = stratified_subsample_digits(X, y, args.n, args.seed, digits)
    X_graph = pca_features(X_sub, args.pca_dim) if args.pca_dim > 0 else X_sub

    print(f"SADSE-like za cifre {digits}, n={len(X_sub)}, PCA={args.pca_dim}")
    print(f"within k={args.k_within}, between k={args.k_between}, reg={args.reg}")

    Z3, vals = sadse_embedding(X_graph, y_sub, 3, args.k_within, args.k_between, args.reg)
    print("Prvih 10 diskriminativnih svojstvenih vrijednosti:", np.round(vals[:10], 6))
    plot_2d(Z3[:, :2], y_sub, args.out2d)
    plot_3d_views(Z3, y_sub, args.out3d)

    Zc, _ = sadse_embedding(X_graph, y_sub, clusters, args.k_within, args.k_between, args.reg)
    Zc /= np.linalg.norm(Zc, axis=1, keepdims=True) + 1e-12
    labels = kmeans(Zc, clusters, seed=args.seed)
    C = contingency_matrix(y_sub, labels, digits, clusters)

    print("\nKontingencijska matrica redovi=stvarne cifre, kolone=klasteri:")
    print(C)
    print("\nMjere grupisanosti:")
    print(f"Purity: {purity_score(C):.4f}")
    print(f"NMI:    {nmi_score(C):.4f}")
    print(f"ARI:    {ari_score(C):.4f}")
    print(f"Najbolja permutacijska tačnost: {best_permutation_accuracy(C):.4f}")
    plot_contingency(C, args.out_cont, digits)


if __name__ == "__main__":
    main()
