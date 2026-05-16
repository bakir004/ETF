#!/usr/bin/env python3
"""MNIST spectral embedding koristeći sklearn.manifold.SpectralEmbedding."""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from sklearn.manifold import SpectralEmbedding
from sklearn.neighbors import kneighbors_graph

from mnist_spectral_embedding import load_mnist, pca_features
from mnist_spectral_clustering import stratified_subsample_digits


def plot_2d(Z: np.ndarray, y: np.ndarray, out: Path) -> None:
    fig, ax = plt.subplots(figsize=(9, 7), dpi=160)
    sc = ax.scatter(Z[:, 0], Z[:, 1], c=y, cmap="tab10", s=13, alpha=0.85, linewidths=0)
    ax.set_title("MNIST SpectralEmbedding / sklearn — 2D")
    ax.set_xlabel("spectral coordinate 1")
    ax.set_ylabel("spectral coordinate 2")
    cbar = fig.colorbar(sc, ax=ax, ticks=sorted(np.unique(y)))
    cbar.set_label("Cifra")
    plt.tight_layout()
    plt.savefig(out)
    print(f"Sačuvan 2D plot: {out}")


def plot_3d_views(Z: np.ndarray, y: np.ndarray, out: Path) -> None:
    fig = plt.figure(figsize=(13, 11), dpi=160)
    views = [
        (20, 35, "pogled 1: elev=20, azim=35"),
        (20, 125, "pogled 2: elev=20, azim=125"),
        (55, 35, "pogled 3: elev=55, azim=35"),
        (10, 215, "pogled 4: elev=10, azim=215"),
    ]
    sc = None
    for i, (elev, azim, title) in enumerate(views, start=1):
        ax = fig.add_subplot(2, 2, i, projection="3d")
        sc = ax.scatter(Z[:, 0], Z[:, 1], Z[:, 2], c=y, cmap="tab10", s=8, alpha=0.82, linewidths=0)
        ax.view_init(elev=elev, azim=azim)
        ax.set_title(title)
        ax.set_xlabel("coord 1")
        ax.set_ylabel("coord 2")
        ax.set_zlabel("coord 3")
    fig.suptitle("MNIST SpectralEmbedding / sklearn — 3D, više perspektiva", fontsize=15)
    cbar = fig.colorbar(sc, ax=fig.axes, ticks=sorted(np.unique(y)), shrink=0.65, pad=0.03)
    cbar.set_label("Cifra")
    plt.savefig(out, bbox_inches="tight")
    print(f"Sačuvan 3D plot: {out}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--digits", type=str, default="0,1,2,3,4")
    parser.add_argument("--n", type=int, default=1000)
    parser.add_argument("--k", type=int, default=15, help="broj susjeda za sklearn nearest_neighbors affinity")
    parser.add_argument("--pca-dim", type=int, default=50, help="0 = bez PCA")
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--data-dir", type=Path, default=Path("data/mnist"))
    parser.add_argument("--out2d", type=Path, default=Path("mnist_sklearn_spectral_0_4_2d.png"))
    parser.add_argument("--out3d", type=Path, default=Path("mnist_sklearn_spectral_0_4_3d_views.png"))
    args = parser.parse_args()

    digits = [int(d.strip()) for d in args.digits.split(",") if d.strip()]
    X, y = load_mnist(args.data_dir)
    X_sub, y_sub = stratified_subsample_digits(X, y, args.n, args.seed, digits)

    if args.pca_dim > 0:
        print(f"PCA dimenzija: {args.pca_dim}")
        X_emb_input = pca_features(X_sub, args.pca_dim)
    else:
        X_emb_input = X_sub

    print(f"Sklearn SpectralEmbedding za cifre {digits}, n={len(X_sub)}, k={args.k}")

    emb2 = SpectralEmbedding(
        n_components=2,
        affinity="nearest_neighbors",
        n_neighbors=args.k,
        random_state=args.seed,
    )
    Z2 = emb2.fit_transform(X_emb_input)
    plot_2d(Z2, y_sub, args.out2d)

    emb3 = SpectralEmbedding(
        n_components=3,
        affinity="nearest_neighbors",
        n_neighbors=args.k,
        random_state=args.seed,
    )
    Z3 = emb3.fit_transform(X_emb_input)
    plot_3d_views(Z3, y_sub, args.out3d)

    # Dodatno: sačuvaj graf sličnosti koji sklearn koristi, ako hoćemo kasnije poređenje.
    A = kneighbors_graph(X_emb_input, args.k, mode="connectivity", include_self=False)
    print(f"Sklearn nearest-neighbor graf: shape={A.shape}, broj usmjerenih grana={A.nnz}")


if __name__ == "__main__":
    main()
