#!/usr/bin/env python3
"""
Spektralno klasterisanje MNIST-a koristeći prvih 10 netrivijalnih svojstvenih
vektora normalizovanog Laplasijana, bez biblioteka za spectral clustering.

Evaluacija je permutation-invariant: ne pretpostavlja da klaster 0 znači cifra 0.
Ispisuje purity, NMI i ARI, te najbolju permutacijsku tačnost samo kao dodatnu
mjeru poklapanja grupa sa stvarnim klasama.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

from mnist_spectral_embedding import build_knn_affinity, load_mnist, pca_features


def spectral_cluster_features(W: np.ndarray, n_vecs: int) -> tuple[np.ndarray, np.ndarray]:
    """Uzmi n_vecs netrivijalnih svojstvenih vektora normalizovanog Laplasijana."""
    degrees = W.sum(axis=1)
    if np.any(degrees == 0):
        raise ValueError("Graf ima izolovane čvorove; povećaj --k.")

    inv_sqrt_deg = 1.0 / np.sqrt(degrees)
    S = (inv_sqrt_deg[:, None] * W) * inv_sqrt_deg[None, :]
    L = np.eye(W.shape[0]) - S

    eigvals, eigvecs = np.linalg.eigh(L)
    order = np.argsort(eigvals)
    eigvals = eigvals[order]
    eigvecs = eigvecs[:, order]

    # Preskačemo prvi/trivijalni vektor i uzimamo narednih 10.
    U = eigvecs[:, 1 : n_vecs + 1].copy()

    # Standardni korak u spektralnom klasterisanju: normalizacija redova.
    U /= np.linalg.norm(U, axis=1, keepdims=True) + 1e-12
    return U, eigvals


def kmeans(X: np.ndarray, k: int, n_init: int = 30, max_iter: int = 200, seed: int = 42) -> np.ndarray:
    """Jednostavna implementacija k-means++ bez sklearn-a."""
    rng = np.random.default_rng(seed)
    best_labels = None
    best_inertia = np.inf
    n = X.shape[0]

    for _ in range(n_init):
        centers = np.empty((k, X.shape[1]), dtype=np.float64)
        centers[0] = X[rng.integers(n)]
        closest_d2 = np.sum((X - centers[0]) ** 2, axis=1)

        for c in range(1, k):
            probs = closest_d2 / (closest_d2.sum() + 1e-12)
            centers[c] = X[rng.choice(n, p=probs)]
            d2 = np.sum((X - centers[c]) ** 2, axis=1)
            closest_d2 = np.minimum(closest_d2, d2)

        labels = np.zeros(n, dtype=int)
        for _iter in range(max_iter):
            dists = np.sum((X[:, None, :] - centers[None, :, :]) ** 2, axis=2)
            new_labels = np.argmin(dists, axis=1)
            if np.array_equal(labels, new_labels):
                break
            labels = new_labels

            for c in range(k):
                mask = labels == c
                if np.any(mask):
                    centers[c] = X[mask].mean(axis=0)
                else:
                    centers[c] = X[rng.integers(n)]

        inertia = np.sum((X - centers[labels]) ** 2)
        if inertia < best_inertia:
            best_inertia = inertia
            best_labels = labels.copy()

    return best_labels


def stratified_subsample_digits(
    X: np.ndarray, y: np.ndarray, n: int, seed: int, digits: list[int]
) -> tuple[np.ndarray, np.ndarray]:
    """Uzmi približno isti broj primjera samo za zadane cifre."""
    rng = np.random.default_rng(seed)
    per_digit = n // len(digits)
    rest = n - len(digits) * per_digit
    chosen = []
    for pos, digit in enumerate(digits):
        idx = np.flatnonzero(y == digit)
        take = per_digit + (1 if pos < rest else 0)
        chosen.append(rng.choice(idx, size=take, replace=False))
    chosen = np.concatenate(chosen)
    rng.shuffle(chosen)
    return X[chosen], y[chosen]


def contingency_matrix(y_true: np.ndarray, y_pred: np.ndarray, digits: list[int], n_clusters: int) -> np.ndarray:
    digit_to_row = {digit: i for i, digit in enumerate(digits)}
    C = np.zeros((len(digits), n_clusters), dtype=int)
    for t, p in zip(y_true, y_pred):
        C[digit_to_row[int(t)], int(p)] += 1
    return C


def purity_score(C: np.ndarray) -> float:
    return C.max(axis=0).sum() / C.sum()


def nmi_score(C: np.ndarray) -> float:
    n = C.sum()
    pi = C.sum(axis=1)
    pj = C.sum(axis=0)

    nz = C > 0
    rows, cols = np.nonzero(nz)
    mi = 0.0
    for i, j in zip(rows, cols):
        mi += (C[i, j] / n) * np.log((C[i, j] * n) / (pi[i] * pj[j]))

    h_true = -np.sum((pi[pi > 0] / n) * np.log(pi[pi > 0] / n))
    h_pred = -np.sum((pj[pj > 0] / n) * np.log(pj[pj > 0] / n))
    return mi / np.sqrt(h_true * h_pred + 1e-12)


def comb2(x: np.ndarray | int) -> np.ndarray | float:
    return x * (x - 1) / 2


def ari_score(C: np.ndarray) -> float:
    n = C.sum()
    sum_comb_c = comb2(C).sum()
    sum_comb_rows = comb2(C.sum(axis=1)).sum()
    sum_comb_cols = comb2(C.sum(axis=0)).sum()
    total = comb2(n)
    expected = sum_comb_rows * sum_comb_cols / total
    max_index = 0.5 * (sum_comb_rows + sum_comb_cols)
    return (sum_comb_c - expected) / (max_index - expected + 1e-12)


def best_permutation_accuracy(C: np.ndarray) -> float:
    """Najbolje 1-1 mapiranje klastera u cifre, za kvadratnu matricu."""
    if C.shape[0] != C.shape[1]:
        return float("nan")
    k = C.shape[0]
    # dp[mask] = max pogodaka nakon dodjele prvih popcount(mask) klastera nekim ciframa
    dp = {0: 0}
    for cluster in range(k):
        new_dp = {}
        for mask, val in dp.items():
            for digit in range(k):
                if not (mask & (1 << digit)):
                    new_mask = mask | (1 << digit)
                    new_val = val + C[digit, cluster]
                    if new_val > new_dp.get(new_mask, -1):
                        new_dp[new_mask] = new_val
        dp = new_dp
    return dp[(1 << k) - 1] / C.sum()


def plot_contingency(C: np.ndarray, out: Path, digits: list[int]) -> None:
    # Svakom klasteru dodijelimo cifru koja je najčešća u tom klasteru.
    # Ovo nije supervised treniranje, nego samo interpretacija dobijenih klastera.
    cluster_row = C.argmax(axis=0)
    cluster_digit = [digits[r] for r in cluster_row]
    cluster_purity = C.max(axis=0) / np.maximum(C.sum(axis=0), 1)

    n_rows, n_cols = C.shape
    fig, ax = plt.subplots(figsize=(max(7, 1.1 * n_cols), max(4.5, 0.75 * n_rows)), dpi=160)
    im = ax.imshow(C, cmap="Blues")
    ax.set_title("Kontingencijska matrica: stvarne cifre × klasteri sa dodijeljenom cifrom")
    ax.set_xlabel("Klaster → dodijeljena cifra")
    ax.set_ylabel("Stvarna cifra")
    ax.set_xticks(range(n_cols))
    ax.set_xticklabels(
        [f"K{j}→{cluster_digit[j]}\n{100*cluster_purity[j]:.0f}%" for j in range(n_cols)],
        fontsize=8,
    )
    ax.set_yticks(range(n_rows))
    ax.set_yticklabels(digits)
    fig.colorbar(im, ax=ax, label="broj slika")
    for i in range(n_rows):
        for j in range(n_cols):
            if C[i, j] > 0:
                color = "white" if C[i, j] > 0.55 * C.max() else "black"
                ax.text(j, i, str(C[i, j]), ha="center", va="center", fontsize=8, color=color)
    plt.tight_layout()
    plt.savefig(out)
    print(f"Sačuvana kontingencijska matrica: {out}")
    print("Dodjela klastera ciframa po većini:")
    for j in range(n_cols):
        print(f"  klaster {j} -> cifra {cluster_digit[j]}  ({100*cluster_purity[j]:.1f}% u klasteru)")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, default=1200)
    parser.add_argument("--k", type=int, default=15, help="broj susjeda u grafu")
    parser.add_argument("--clusters", type=int, default=0, help="broj klastera; 0 = broj izabranih cifara")
    parser.add_argument("--eigvecs", type=int, default=0, help="broj netrivijalnih svojstvenih vektora; 0 = broj izabranih cifara")
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--data-dir", type=Path, default=Path("data/mnist"))
    parser.add_argument("--graph", choices=["global", "self_tuning", "cosine"], default="self_tuning")
    parser.add_argument("--pca-dim", type=int, default=50)
    parser.add_argument("--out", type=Path, default=Path("mnist_spectral_clustering_contingency.png"))
    parser.add_argument("--digits", type=str, default="0,1,2,3,4", help="cifre koje koristimo, npr. '0,1,2,3,4'")
    args = parser.parse_args()

    digits = [int(d.strip()) for d in args.digits.split(",") if d.strip() != ""]
    if len(digits) == 0:
        raise ValueError("Moraš zadati bar jednu cifru kroz --digits")
    if len(set(digits)) != len(digits):
        raise ValueError("Cifre u --digits moraju biti različite")
    args.clusters = len(digits) if args.clusters == 0 else args.clusters
    args.eigvecs = len(digits) if args.eigvecs == 0 else args.eigvecs

    X, y = load_mnist(args.data_dir)
    X_sub, y_sub = stratified_subsample_digits(X, y, args.n, args.seed, digits)

    if args.pca_dim > 0:
        print(f"PCA dimenzija: {args.pca_dim}")
        X_graph = pca_features(X_sub, args.pca_dim)
    else:
        X_graph = X_sub

    print(f"Koristim samo cifre: {digits}")
    print(f"Gradim graf: method={args.graph}, k={args.k}, n={args.n}")
    W = build_knn_affinity(X_graph, args.k, args.graph)

    print(f"Uzimam {args.eigvecs} netrivijalnih svojstvenih vektora i klasterišem u {args.clusters} grupa...")
    U, eigvals = spectral_cluster_features(W, args.eigvecs)
    labels = kmeans(U, args.clusters, seed=args.seed)

    C = contingency_matrix(y_sub, labels, digits, args.clusters)
    print("Prvih 12 svojstvenih vrijednosti:", np.round(eigvals[:12], 6))
    print("\nKontingencijska matrica redovi=stvarne cifre, kolone=klasteri:")
    print(C)
    print("\nMjere grupisanosti, nezavisne od imenovanja klastera:")
    print(f"Purity: {purity_score(C):.4f}")
    print(f"NMI:    {nmi_score(C):.4f}")
    print(f"ARI:    {ari_score(C):.4f}")
    perm_acc = best_permutation_accuracy(C)
    if np.isfinite(perm_acc):
        print(f"Najbolja permutacijska tačnost / cluster-label agreement: {perm_acc:.4f}")

    plot_contingency(C, args.out, digits)


if __name__ == "__main__":
    main()
