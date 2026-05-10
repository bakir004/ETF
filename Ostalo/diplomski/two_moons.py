import matplotlib.pyplot as plt
import numpy as np
from matplotlib.collections import LineCollection
from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs, make_moons


def make_two_moons_gaussian_graph():
    x, y = make_moons(n_samples=300, noise=0.12, random_state=42)
    sigma = 0.25
    weight_threshold = 0.25
    squared_distances = np.sum((x[:, None, :] - x[None, :, :]) ** 2, axis=2)
    weights = np.exp(-squared_distances / (2 * sigma**2))
    adjacency_matrix = np.where(weights >= weight_threshold, weights, 0)
    np.fill_diagonal(adjacency_matrix, 0)

    return x, y, adjacency_matrix, sigma, weight_threshold


def spectral_partition(adjacency_matrix, n_clusters):
    """Particionisanje grafa pomocu normalized spectral clustering metode.

    Za koncentricne kruznice obicni/nenormalizovani Laplasijan cesto preferira
    lokalne guste komade/lukove. Normalizovani Laplasijan + row-normalization
    je stabilniji za ovakav dataset.
    """
    degrees = adjacency_matrix.sum(axis=1)
    safe_degrees = np.where(degrees > 1e-12, degrees, 1.0)
    inv_sqrt_degrees = 1.0 / np.sqrt(safe_degrees)

    normalized_adjacency = (
        inv_sqrt_degrees[:, None] * adjacency_matrix * inv_sqrt_degrees[None, :]
    )
    normalized_laplacian = np.eye(adjacency_matrix.shape[0]) - normalized_adjacency
    eigenvalues, eigenvectors = np.linalg.eigh(normalized_laplacian)

    # Ng-Jordan-Weiss: uzmi k najmanjih eigenvektora normalizovanog Laplasijana,
    # zatim normalizuj redove i pusti KMeans.
    embedding = eigenvectors[:, :n_clusters]
    row_norms = np.linalg.norm(embedding, axis=1, keepdims=True)
    embedding = embedding / np.maximum(row_norms, 1e-12)
    partition = KMeans(n_clusters=n_clusters, n_init=30, random_state=42).fit_predict(embedding)

    return partition, eigenvalues, eigenvectors


def save_adjacency_grid(
    adjacency_matrix,
    partition_labels=None,
    output_path="two_moons_adjacency_matrix.png",
    title_suffix="spektralnom particionisanju",
):
    """Nacrtaj matricu adjencije kao crno-bijeli grid piksela.

    Vrijednosti se normalizuju u [0, 1]. Bijelo znaci 0 / nema veze,
    crno znaci jaka veza. Ako su date particije, nacrta se i sortirana verzija
    po particijama koje su dobijene iz eigenvektora, ne po pravim labelama.
    """
    matrix = adjacency_matrix.copy().astype(float)
    max_value = matrix.max()
    if max_value > 0:
        matrix = matrix / max_value

    if partition_labels is not None:
        order = np.lexsort((np.arange(len(partition_labels)), partition_labels))
        sorted_matrix = matrix[order][:, order]

        fig, axes = plt.subplots(1, 2, figsize=(11, 5))
        axes[0].imshow(matrix, cmap="gray_r", vmin=0, vmax=1, interpolation="nearest")
        axes[0].set_title("Matrica adjencije\n(originalni redoslijed)")
        axes[1].imshow(sorted_matrix, cmap="gray_r", vmin=0, vmax=1, interpolation="nearest")
        axes[1].set_title(f"Matrica adjencije\n(sortirano po {title_suffix})")

        for ax in axes:
            ax.set_xlabel("cvor j")
            ax.set_ylabel("cvor i")
    else:
        fig, ax = plt.subplots(figsize=(6, 6))
        ax.imshow(matrix, cmap="gray_r", vmin=0, vmax=1, interpolation="nearest")
        ax.set_title("Matrica adjencije")
        ax.set_xlabel("cvor j")
        ax.set_ylabel("cvor i")

    plt.tight_layout()
    plt.savefig(output_path, dpi=180)
    plt.close()
    print(f"Matrica adjencije je sacuvana kao grid: {output_path}")


def make_three_concentric_circles_gaussian_graph():
    rng = np.random.default_rng(42)
    samples_per_circle = 200
    radii = [0.55, 1.05, 1.55]

    points = []
    labels = []
    for label, radius in enumerate(radii):
        angles = rng.uniform(0, 2 * np.pi, samples_per_circle)
        noisy_radii = radius + rng.normal(0, 0.045, samples_per_circle)
        circle = np.column_stack(
            [noisy_radii * np.cos(angles), noisy_radii * np.sin(angles)]
        )
        points.append(circle)
        labels.extend([label] * samples_per_circle)

    x = np.vstack(points)
    y = np.array(labels)

    # Parametri su podeseni tako da tacke unutar iste kruznice ostanu povezane,
    # ali da se susjedne kruznice ne povezu previse.
    sigma = 0.15
    weight_threshold = 0.2
    squared_distances = np.sum((x[:, None, :] - x[None, :, :]) ** 2, axis=2)
    weights = np.exp(-squared_distances / (2 * sigma**2))
    adjacency_matrix = np.where(weights >= weight_threshold, weights, 0)
    np.fill_diagonal(adjacency_matrix, 0)

    return x, y, adjacency_matrix, sigma, weight_threshold


def make_variable_density_gaussian_graph():
    x, y = make_blobs(
        n_samples=[180, 120],
        centers=[[-1.2, 0], [1.4, 0]],
        cluster_std=[0.18, 0.65],
        random_state=42,
    )
    sigma = 0.45
    weight_threshold = 0.15
    squared_distances = np.sum((x[:, None, :] - x[None, :, :]) ** 2, axis=2)
    weights = np.exp(-squared_distances / (2 * sigma**2))
    adjacency_matrix = np.where(weights >= weight_threshold, weights, 0)
    np.fill_diagonal(adjacency_matrix, 0)

    return x, y, adjacency_matrix, sigma, weight_threshold


def main():
    x, y, adjacency_matrix, sigma, weight_threshold = make_two_moons_gaussian_graph()

    spectral_labels, eigenvalues, _ = spectral_partition(adjacency_matrix, n_clusters=2)
    print("Two moons - prve svojstvene vrijednosti:", np.round(eigenvalues[:6], 6))
    save_adjacency_grid(
        adjacency_matrix,
        spectral_labels,
        output_path="two_moons_adjacency_matrix_spectral.png",
        title_suffix="spektralnom particionisanju",
    )

    plt.figure(figsize=(6, 5))
    plt.scatter(
        x[:, 0],
        x[:, 1],
        c=y,
        cmap="coolwarm",
        s=35,
        edgecolors="black",
        linewidths=0.4,
    )
    plt.title("Two moons dataset")
    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")

    output_path = "two_moons.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    edge_indices = np.argwhere(np.triu(adjacency_matrix > 0, k=1))
    edge_weights = adjacency_matrix[edge_indices[:, 0], edge_indices[:, 1]]
    edges = x[edge_indices]

    plt.figure(figsize=(7, 6))
    edge_collection = LineCollection(
        edges,
        colors="gray",
        linewidths=0.2 + 1.8 * edge_weights,
        alpha=0.25,
        zorder=1,
    )
    plt.gca().add_collection(edge_collection)
    plt.scatter(
        x[:, 0],
        x[:, 1],
        c=y,
        cmap="coolwarm",
        s=35,
        edgecolors="black",
        linewidths=0.4,
        zorder=2,
    )
    plt.title(
        f"Two moons Gaussian graf, sigma = {sigma}, prag = {weight_threshold}"
    )
    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")

    output_path = "two_moons_gaussian_graph.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")
    print(f"Broj grana u Gaussian grafu: {len(edge_indices)}")

    # Jos jedan dataset: tri koncentricne kruznice.
    x, y, adjacency_matrix, sigma, weight_threshold = make_three_concentric_circles_gaussian_graph()

    spectral_labels, eigenvalues, _ = spectral_partition(adjacency_matrix, n_clusters=3)
    print("Tri kruznice - prve svojstvene vrijednosti:", np.round(eigenvalues[:8], 6))
    save_adjacency_grid(
        adjacency_matrix,
        spectral_labels,
        output_path="three_concentric_circles_adjacency_matrix_spectral.png",
        title_suffix="spektralnom particionisanju",
    )

    plt.figure(figsize=(6, 6))
    plt.scatter(
        x[:, 0],
        x[:, 1],
        c=y,
        cmap="viridis",
        s=28,
        edgecolors="black",
        linewidths=0.25,
    )
    plt.title("Tri koncentricne kruznice dataset")
    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")

    output_path = "three_concentric_circles.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    edge_indices = np.argwhere(np.triu(adjacency_matrix > 0, k=1))
    edge_weights = adjacency_matrix[edge_indices[:, 0], edge_indices[:, 1]]
    edges = x[edge_indices]

    plt.figure(figsize=(7, 7))
    edge_collection = LineCollection(
        edges,
        colors="gray",
        linewidths=0.15 + 1.4 * edge_weights,
        alpha=0.22,
        zorder=1,
    )
    plt.gca().add_collection(edge_collection)
    plt.scatter(
        x[:, 0],
        x[:, 1],
        c=y,
        cmap="viridis",
        s=28,
        edgecolors="black",
        linewidths=0.25,
        zorder=2,
    )
    plt.title(
        f"Tri koncentricne kruznice Gaussian graf, sigma = {sigma}, prag = {weight_threshold}"
    )
    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")

    output_path = "three_concentric_circles_gaussian_graph.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")
    print(f"Broj grana u Gaussian grafu za kruznice: {len(edge_indices)}")


if __name__ == "__main__":
    main()
