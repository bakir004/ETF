#!/usr/bin/env python3
"""
Spektralno embedanje MNIST skupa u 2D bez biblioteka za spectral embedding.

Koristi se samo:
  - numpy za linearnu algebru
  - matplotlib za plot

Postupak:
  1. ručno preuzmi i učitaj MNIST IDX fajlove,
  2. napravi k-NN graf nad poduzorkom slika,
  3. formiraj normalizovani Laplasijan L = I - D^{-1/2} W D^{-1/2},
  4. izračunaj svojstvene vektore i uzmi 2 netrivijalna vektora kao koordinate.
"""

from __future__ import annotations

import argparse
import gzip
import struct
import urllib.request
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


MNIST_URLS = {
    "train_images": "https://storage.googleapis.com/cvdf-datasets/mnist/train-images-idx3-ubyte.gz",
    "train_labels": "https://storage.googleapis.com/cvdf-datasets/mnist/train-labels-idx1-ubyte.gz",
}


def download_if_needed(url: str, path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists():
        return
    print(f"Preuzimam {url} -> {path}")
    urllib.request.urlretrieve(url, path)


def load_mnist(data_dir: Path) -> tuple[np.ndarray, np.ndarray]:
    image_gz = data_dir / "train-images-idx3-ubyte.gz"
    label_gz = data_dir / "train-labels-idx1-ubyte.gz"
    download_if_needed(MNIST_URLS["train_images"], image_gz)
    download_if_needed(MNIST_URLS["train_labels"], label_gz)

    with gzip.open(image_gz, "rb") as f:
        magic, n, rows, cols = struct.unpack(">IIII", f.read(16))
        if magic != 2051:
            raise ValueError("Neispravan MNIST image fajl")
        images = np.frombuffer(f.read(), dtype=np.uint8).reshape(n, rows * cols)

    with gzip.open(label_gz, "rb") as f:
        magic, n_labels = struct.unpack(">II", f.read(8))
        if magic != 2049:
            raise ValueError("Neispravan MNIST label fajl")
        labels = np.frombuffer(f.read(), dtype=np.uint8)

    if len(images) != len(labels):
        raise ValueError("Broj slika i labela se ne poklapa")
    return images.astype(np.float64) / 255.0, labels


def stratified_subsample(X: np.ndarray, y: np.ndarray, n: int, seed: int) -> tuple[np.ndarray, np.ndarray]:
    """Uzmi približno isti broj primjera za svaku cifru."""
    rng = np.random.default_rng(seed)
    per_digit = n // 10
    rest = n - 10 * per_digit
    chosen = []
    for digit in range(10):
        idx = np.flatnonzero(y == digit)
        take = per_digit + (1 if digit < rest else 0)
        chosen.append(rng.choice(idx, size=take, replace=False))
    chosen = np.concatenate(chosen)
    rng.shuffle(chosen)
    return X[chosen], y[chosen]


def pairwise_sq_dists(X: np.ndarray) -> np.ndarray:
    """Matrica kvadriranih Euklidskih udaljenosti."""
    norms = np.sum(X * X, axis=1)
    D2 = norms[:, None] + norms[None, :] - 2.0 * (X @ X.T)
    np.maximum(D2, 0.0, out=D2)
    return D2


def pca_features(X: np.ndarray, n_components: int) -> np.ndarray:
    """PCA reprezentacija slika; uklanja dio šuma iz sirovih piksela."""
    Xc = X - X.mean(axis=0, keepdims=True)
    _, _, Vt = np.linalg.svd(Xc, full_matrices=False)
    Z = Xc @ Vt[:n_components].T
    Z = (Z - Z.mean(axis=0, keepdims=True)) / (Z.std(axis=0, keepdims=True) + 1e-12)
    return Z


def build_knn_affinity(X: np.ndarray, k: int, method: str = "self_tuning") -> np.ndarray:
    """Simetrična k-NN matrica težina W.

    method='global': jedan globalni sigma u Gaussovom kernelu.
    method='self_tuning': lokalna skala sigma_i za svaku sliku
    (Zelnik-Manor/Perona), često bolja za MNIST jer gustoće klasa nisu iste.
    method='cosine': kosinusna sličnost na k najbližih po kosinusnoj udaljenosti.
    """
    n = X.shape[0]

    if method == "cosine":
        Xn = X / (np.linalg.norm(X, axis=1, keepdims=True) + 1e-12)
        sim = Xn @ Xn.T
        D = 1.0 - sim
        np.fill_diagonal(D, np.inf)
        knn_idx = np.argpartition(D, kth=k, axis=1)[:, :k]
        W = np.zeros((n, n), dtype=np.float64)
        rows = np.arange(n)[:, None]
        W[rows, knn_idx] = np.maximum(sim[rows, knn_idx], 0.0) ** 2
    else:
        D2 = pairwise_sq_dists(X)
        np.fill_diagonal(D2, np.inf)
        knn_idx = np.argpartition(D2, kth=k, axis=1)[:, :k]
        rows = np.arange(n)[:, None]

        if method == "global":
            kth_d2 = np.take_along_axis(D2, knn_idx, axis=1).max(axis=1)
            sigma2 = float(np.median(kth_d2))
            if sigma2 <= 0 or not np.isfinite(sigma2):
                sigma2 = 1.0
            weights = np.exp(-D2[rows, knn_idx] / sigma2)
        elif method == "self_tuning":
            # sigma_i je udaljenost od i-te slike do njenog k-tog susjeda.
            # Kernel: exp(-||x_i-x_j||^2 / (sigma_i sigma_j)).
            kth_d2 = np.take_along_axis(D2, knn_idx, axis=1).max(axis=1)
            sigma = np.sqrt(np.maximum(kth_d2, 1e-12))
            weights = np.exp(-D2[rows, knn_idx] / (sigma[:, None] * sigma[knn_idx] + 1e-12))
        else:
            raise ValueError(f"Nepoznata metoda grafa: {method}")

        W = np.zeros((n, n), dtype=np.float64)
        W[rows, knn_idx] = weights

    # union k-NN graf: poveži ako je bar jedan smjer među k najbližih
    W = np.maximum(W, W.T)
    np.fill_diagonal(W, 0.0)
    return W


def spectral_embedding(W: np.ndarray, dim: int = 2) -> tuple[np.ndarray, np.ndarray]:
    """Vrati dimenzionalni embedding i svojstvene vrijednosti normalizovanog Laplasijana."""
    degrees = W.sum(axis=1)
    if np.any(degrees == 0):
        raise ValueError("Graf ima izolovane čvorove; povećaj --k.")

    inv_sqrt_deg = 1.0 / np.sqrt(degrees)
    S = (inv_sqrt_deg[:, None] * W) * inv_sqrt_deg[None, :]
    L = np.eye(W.shape[0]) - S

    # eigh računa sve svojstvene parove simetrične matrice; ne koristi spectral embedding biblioteku
    eigvals, eigvecs = np.linalg.eigh(L)
    order = np.argsort(eigvals)
    eigvals = eigvals[order]
    eigvecs = eigvecs[:, order]

    # prvi vektor je trivijalan za povezani graf; narednih dim daju koordinate
    Z = eigvecs[:, 1:dim + 1].copy()
    Z = (Z - Z.mean(axis=0)) / Z.std(axis=0)
    return Z, eigvals


def plot_embedding(Z: np.ndarray, y: np.ndarray, out_path: Path) -> None:
    if Z.shape[1] == 3:
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
            sc = ax.scatter(Z[:, 0], Z[:, 1], Z[:, 2], c=y, cmap="tab10", s=8, alpha=0.8, linewidths=0)
            ax.view_init(elev=elev, azim=azim)
            ax.set_title(title)
            ax.set_xlabel("v2")
            ax.set_ylabel("v3")
            ax.set_zlabel("v4")
        fig.suptitle("MNIST spektralno embedanje u 3D — više perspektiva", fontsize=15)
        cbar = fig.colorbar(sc, ax=fig.axes, ticks=range(10), shrink=0.65, pad=0.03)
    else:
        fig = plt.figure(figsize=(9, 7), dpi=160)
        ax = fig.add_subplot(111)
        sc = ax.scatter(Z[:, 0], Z[:, 1], c=y, cmap="tab10", s=12, alpha=0.85, linewidths=0)
        ax.set_title("MNIST spektralno embedanje u 2D\n(2 netrivijalna svojstvena vektora Laplasijana)")
        ax.set_xlabel("svojstveni vektor 2")
        ax.set_ylabel("svojstveni vektor 3")
        cbar = fig.colorbar(sc, ax=ax, ticks=range(10))

    cbar.set_label("Cifra")
    plt.savefig(out_path, bbox_inches="tight")
    print(f"Sačuvan plot: {out_path}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, default=1000, help="broj MNIST slika za spektralni graf")
    parser.add_argument("--k", type=int, default=12, help="broj najbližih susjeda u grafu")
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--data-dir", type=Path, default=Path("data/mnist"))
    parser.add_argument("--out", type=Path, default=Path("mnist_spectral_embedding.png"))
    parser.add_argument("--graph", choices=["global", "self_tuning", "cosine"], default="self_tuning")
    parser.add_argument("--pca-dim", type=int, default=50, help="broj PCA komponenti prije grafa; 0 = sirovi pikseli")
    parser.add_argument("--dim", type=int, choices=[2, 3], default=2, help="dimenzija spektralnog plota")
    args = parser.parse_args()

    X, y = load_mnist(args.data_dir)
    X_sub, y_sub = stratified_subsample(X, y, args.n, args.seed)

    print(f"MNIST poduzorak: {X_sub.shape[0]} slika, dimenzija {X_sub.shape[1]}")
    if args.pca_dim > 0:
        print(f"Računam PCA reprezentaciju dimenzije {args.pca_dim}...")
        X_graph = pca_features(X_sub, args.pca_dim)
    else:
        X_graph = X_sub

    print(f"Gradim k-NN graf metodom '{args.graph}'...")
    W = build_knn_affinity(X_graph, args.k, args.graph)

    print("Računam svojstvene vektore normalizovanog Laplasijana...")
    Z, eigvals = spectral_embedding(W, args.dim)
    print("Prvih 10 svojstvenih vrijednosti:", np.round(eigvals[:10], 6))

    plot_embedding(Z, y_sub, args.out)


if __name__ == "__main__":
    main()
