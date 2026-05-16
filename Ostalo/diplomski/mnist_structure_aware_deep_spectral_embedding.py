#!/usr/bin/env python3
"""
Structure-Aware Deep Spectral Embedding na MNIST podskupu.

Praktična implementacija ideje:
  1. konstruišemo graf sličnosti W iz podataka,
  2. neuronska mreža f_theta(x) daje spektralni embedding z,
  3. treniramo je sa struktur-aware spektralnim ciljem:
       tr(Z^T L Z)  + ortogonalnost/balans
     plus rekonstrukcioni autoencoder loss da embedding ne izgubi strukturu slike.

Ovo je unsupervised u smislu treninga embeddinga: labele se koriste samo za bojenje
plotova i evaluaciju klastera.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import torch
from torch import nn

from mnist_spectral_embedding import build_knn_affinity, load_mnist
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


class DeepSpectralAE(nn.Module):
    def __init__(self, input_dim: int, embed_dim: int):
        super().__init__()
        self.encoder = nn.Sequential(
            nn.Linear(input_dim, 256),
            nn.ReLU(),
            nn.Linear(256, 128),
            nn.ReLU(),
            nn.Linear(128, embed_dim),
        )
        self.decoder = nn.Sequential(
            nn.Linear(embed_dim, 128),
            nn.ReLU(),
            nn.Linear(128, 256),
            nn.ReLU(),
            nn.Linear(256, input_dim),
            nn.Sigmoid(),
        )

    def forward(self, x: torch.Tensor) -> tuple[torch.Tensor, torch.Tensor]:
        z = self.encoder(x)
        recon = self.decoder(z)
        return z, recon


def spectral_losses(Z: torch.Tensor, W: torch.Tensor) -> tuple[torch.Tensor, torch.Tensor, torch.Tensor]:
    degrees = W.sum(dim=1)
    L = torch.diag(degrees) - W
    vol = degrees.sum() + 1e-12

    # Weighted centering: uklanja trivijalni konstantni pravac.
    mean = (degrees[:, None] * Z).sum(dim=0, keepdim=True) / vol
    Zc = Z - mean

    # Spektralni Laplasijan loss: susjedne/slične slike drži blizu.
    lap_loss = torch.trace(Zc.T @ L @ Zc) / vol

    # Bez ovog uslova minimum je kolaps Z=konstanta.
    G = (Zc.T @ (degrees[:, None] * Zc)) / vol
    I = torch.eye(Z.shape[1], device=Z.device)
    orth_loss = torch.mean((G - I) ** 2)

    # Dodatni balans da koordinate imaju nultu sredinu.
    balance_loss = torch.mean(mean**2)
    return lap_loss, orth_loss, balance_loss


def plot_2d(Z: np.ndarray, y: np.ndarray, out: Path) -> None:
    fig, ax = plt.subplots(figsize=(9, 7), dpi=160)
    sc = ax.scatter(Z[:, 0], Z[:, 1], c=y, cmap="tab10", s=13, alpha=0.85, linewidths=0)
    ax.set_title("Structure-Aware Deep Spectral Embedding — 2D")
    ax.set_xlabel("deep spectral coord 1")
    ax.set_ylabel("deep spectral coord 2")
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
    fig.suptitle("Structure-Aware Deep Spectral Embedding — 3D", fontsize=15)
    cbar = fig.colorbar(sc, ax=fig.axes, ticks=sorted(np.unique(y)), shrink=0.65, pad=0.03)
    cbar.set_label("Cifra")
    plt.savefig(out, bbox_inches="tight")
    print(f"Sačuvan 3D plot: {out}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--digits", type=str, default="0,1,2,3,4")
    parser.add_argument("--n", type=int, default=1000)
    parser.add_argument("--k", type=int, default=15)
    parser.add_argument("--embed-dim", type=int, default=5)
    parser.add_argument("--epochs", type=int, default=250)
    parser.add_argument("--lr", type=float, default=1e-3)
    parser.add_argument("--lambda-recon", type=float, default=0.15)
    parser.add_argument("--lambda-orth", type=float, default=25.0)
    parser.add_argument("--lambda-balance", type=float, default=1.0)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--data-dir", type=Path, default=Path("data/mnist"))
    parser.add_argument("--out2d", type=Path, default=Path("mnist_structure_aware_dse_0_4_2d.png"))
    parser.add_argument("--out3d", type=Path, default=Path("mnist_structure_aware_dse_0_4_3d_views.png"))
    parser.add_argument("--out-cont", type=Path, default=Path("mnist_structure_aware_dse_0_4_contingency.png"))
    args = parser.parse_args()

    np.random.seed(args.seed)
    torch.manual_seed(args.seed)
    digits = [int(d.strip()) for d in args.digits.split(",") if d.strip()]

    X, y = load_mnist(args.data_dir)
    X_sub, y_sub = stratified_subsample_digits(X, y, args.n, args.seed, digits)
    print(f"Structure-Aware Deep Spectral Embedding: digits={digits}, n={len(X_sub)}, k={args.k}")

    W_np = build_knn_affinity(X_sub, args.k, method="self_tuning")
    device = torch.device("cpu")
    X_t = torch.tensor(X_sub, dtype=torch.float32, device=device)
    W_t = torch.tensor(W_np, dtype=torch.float32, device=device)

    model = DeepSpectralAE(X_sub.shape[1], args.embed_dim).to(device)
    opt = torch.optim.Adam(model.parameters(), lr=args.lr)
    mse = nn.MSELoss()

    for epoch in range(1, args.epochs + 1):
        opt.zero_grad()
        Z, recon = model(X_t)
        lap, orth, bal = spectral_losses(Z, W_t)
        rec = mse(recon, X_t)
        loss = lap + args.lambda_orth * orth + args.lambda_balance * bal + args.lambda_recon * rec
        loss.backward()
        opt.step()

        if epoch == 1 or epoch % 25 == 0 or epoch == args.epochs:
            print(
                f"epoch {epoch:4d} | loss={loss.item():.5f} "
                f"lap={lap.item():.5f} orth={orth.item():.5f} recon={rec.item():.5f}"
            )

    model.eval()
    with torch.no_grad():
        Z = model.encoder(X_t).cpu().numpy()
    Z = (Z - Z.mean(axis=0)) / (Z.std(axis=0) + 1e-12)

    plot_2d(Z[:, :2], y_sub, args.out2d)
    plot_3d_views(Z[:, :3], y_sub, args.out3d)

    labels = kmeans(Z[:, : len(digits)], len(digits), seed=args.seed)
    C = contingency_matrix(y_sub, labels, digits, len(digits))
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
