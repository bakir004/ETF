import numpy as np
import matplotlib.pyplot as plt
import networkx as nx
from matplotlib.collections import LineCollection
from sklearn.cluster import DBSCAN
from sklearn.decomposition import PCA

from two_moons import make_two_moons_gaussian_graph, make_variable_density_gaussian_graph


def print_matrix(title, matrix):
    print(f"\n{title}:")
    print(np.array2string(matrix, precision=2, suppress_small=True))


def add_plot_jitter(points, scale=0.03):
    offsets = np.zeros_like(points)

    for i in range(len(points)):
        angle = 2 * np.pi * i / len(points)
        offsets[i] = scale * np.array([np.cos(angle), np.sin(angle)])

    return points + offsets


def find_cut_edges(adjacency_matrix, partition):
    cut_edges = []

    for i in range(adjacency_matrix.shape[0]):
        for j in range(i + 1, adjacency_matrix.shape[1]):
            if adjacency_matrix[i, j] != 0 and partition[i] != partition[j]:
                cut_edges.append((i, j))

    return cut_edges


def find_ratiocut(adjacency_matrix, fiedler_vector):
    best_partition = None
    best_cut_edges = None
    best_cut_size = None
    best_ratiocut = None

    for threshold in np.unique(fiedler_vector)[:-1]:
        partition = fiedler_vector <= threshold

        if partition.all() or not partition.any():
            continue

        cut_edges = find_cut_edges(adjacency_matrix, partition)
        cut_size = len(cut_edges)
        size_a = partition.sum()
        size_b = (~partition).sum()
        ratiocut = cut_size * (1 / size_a + 1 / size_b)

        if best_ratiocut is None or ratiocut < best_ratiocut:
            best_partition = partition
            best_cut_edges = cut_edges
            best_cut_size = cut_size
            best_ratiocut = ratiocut

    return best_partition, best_cut_edges, best_cut_size, best_ratiocut


def plot_weighted_point_graph(
    points,
    adjacency_matrix,
    node_values,
    output_path,
    title,
    cmap="coolwarm",
):
    edge_indices = np.argwhere(np.triu(adjacency_matrix > 0, k=1))
    edge_weights = adjacency_matrix[edge_indices[:, 0], edge_indices[:, 1]]
    edges = points[edge_indices]

    plt.figure(figsize=(7, 6))

    if len(edges) > 0:
        edge_collection = LineCollection(
            edges,
            colors="gray",
            linewidths=0.2 + 1.8 * edge_weights,
            alpha=0.2,
            zorder=1,
        )
        plt.gca().add_collection(edge_collection)

    plt.scatter(
        points[:, 0],
        points[:, 1],
        c=node_values,
        cmap=cmap,
        s=35,
        edgecolors="black",
        linewidths=0.3,
        zorder=2,
    )
    plt.title(title)
    plt.xlabel("x1")
    plt.ylabel("x2")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")


def plot_clustering_comparison(points, clusterings, output_path):
    fig, axes = plt.subplots(1, len(clusterings), figsize=(5 * len(clusterings), 4.5))

    for ax, (title, clustering) in zip(axes, clusterings):
        scatter = ax.scatter(
            points[:, 0],
            points[:, 1],
            c=clustering,
            cmap="coolwarm",
            s=35,
            edgecolors="black",
            linewidths=0.3,
        )
        ax.set_title(title)
        ax.set_xlabel("x1")
        ax.set_ylabel("x2")
        ax.grid(True, alpha=0.3)
        ax.axis("equal")
        fig.colorbar(scatter, ax=ax, fraction=0.046, pad=0.04)

    fig.tight_layout()
    fig.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")


def compare_clustering_methods(
    title,
    output_prefix,
    points,
    true_labels,
    adjacency_matrix,
    dbscan_eps,
):
    degree_matrix = np.diag(adjacency_matrix.sum(axis=1))
    laplacian_matrix = degree_matrix - adjacency_matrix
    eigenvalues, eigenvectors = np.linalg.eigh(laplacian_matrix)
    fiedler_partition = eigenvectors[:, 1] <= 0
    pca_projection = PCA(n_components=1).fit_transform(points).ravel()
    pca_partition = pca_projection <= np.median(pca_projection)
    dbscan_labels = DBSCAN(eps=dbscan_eps, min_samples=5).fit_predict(points)

    np.save(f"{output_prefix}_eigenvalues.npy", eigenvalues)
    np.save(f"{output_prefix}_eigenvectors.npy", eigenvectors)

    print(f"\n=== {title} ===")
    print(f"Broj tacaka = {len(points)}")
    print(f"Broj grana = {np.count_nonzero(np.triu(adjacency_matrix > 0, k=1))}")
    print_matrix("Prvih 20 svojstvenih vrijednosti Laplasijana", eigenvalues[:20])
    print("DBSCAN klasteri =", sorted(set(dbscan_labels.tolist())))
    print("DBSCAN broj sumova/noise tacaka =", int(np.sum(dbscan_labels == -1)))

    plot_weighted_point_graph(
        points,
        adjacency_matrix,
        true_labels,
        f"{output_prefix}_gaussian_graf.png",
        f"{title}: Gaussian graf",
    )
    plot_clustering_comparison(
        points,
        [
            ("Spektralno: znak Fiedlerovog vektora", fiedler_partition),
            ("PCA: split po PC1 medijani", pca_partition),
            (f"DBSCAN: eps = {dbscan_eps}, min_samples = 5", dbscan_labels),
        ],
        f"{output_prefix}_poredjenje_klasterisanja.png",
    )


def analyze_two_moons_graph():
    points, labels, adjacency_matrix, sigma, weight_threshold = (
        make_two_moons_gaussian_graph()
    )
    degree_matrix = np.diag(adjacency_matrix.sum(axis=1))
    laplacian_matrix = degree_matrix - adjacency_matrix

    eigenvalues, eigenvectors = np.linalg.eigh(laplacian_matrix)
    fiedler_vector = eigenvectors[:, 1]
    fiedler_partition = fiedler_vector <= 0
    fiedler_cut_edges = find_cut_edges(adjacency_matrix, fiedler_partition)
    fiedler_cut_size = len(fiedler_cut_edges)
    ratiocut_partition, ratiocut_edges, ratiocut_size, ratiocut = find_ratiocut(
        adjacency_matrix,
        fiedler_vector,
    )
    pca_projection = PCA(n_components=1).fit_transform(points).ravel()
    pca_partition = pca_projection <= np.median(pca_projection)
    dbscan_labels = DBSCAN(eps=0.15, min_samples=4).fit_predict(points)
    embedding = eigenvectors[:, 1:3]
    orthogonality_check = eigenvectors.T @ eigenvectors
    identity_matrix = np.eye(eigenvectors.shape[1])
    are_orthonormal = np.allclose(orthogonality_check, identity_matrix)

    np.save("two_moons_eigenvalues.npy", eigenvalues)
    np.save("two_moons_eigenvectors.npy", eigenvectors)

    print("\n=== Two Moons Gaussian graf ===")
    print(f"Broj tacaka = {len(points)}")
    print(f"sigma = {sigma}")
    print(f"prag = {weight_threshold}")
    print(f"Broj grana = {np.count_nonzero(np.triu(adjacency_matrix > 0, k=1))}")
    print_matrix("Prvih 20 svojstvenih vrijednosti Laplasijana", eigenvalues[:20])
    print_matrix("Fiedlerov vektor", fiedler_vector)
    print_matrix("Spektralni embedding u 2D", embedding)
    print("\nJesu li svojstveni vektori ortonormirani?")
    print(are_orthonormal)
    print("\nRez pomocu znaka Fiedlerovog vektora:")
    print("A =", np.where(fiedler_partition)[0].tolist())
    print("B =", np.where(~fiedler_partition)[0].tolist())
    print("Velicina reza =", fiedler_cut_size)
    print("\nRatioCut pomocu Fiedlerovog vektora:")
    print("A =", np.where(ratiocut_partition)[0].tolist())
    print("B =", np.where(~ratiocut_partition)[0].tolist())
    print("Velicina reza =", ratiocut_size)
    print("RatioCut =", round(ratiocut, 2))
    print("\nPCA split po medijani prve glavne komponente:")
    print("A =", np.where(pca_partition)[0].tolist())
    print("B =", np.where(~pca_partition)[0].tolist())
    print("\nDBSCAN:")
    print("Klasteri =", sorted(set(dbscan_labels.tolist())))
    print("Broj sumova/noise tacaka =", int(np.sum(dbscan_labels == -1)))
    print("Svojstvene vrijednosti su sacuvane u fajl: two_moons_eigenvalues.npy")
    print("Svojstveni vektori su sacuvani u fajl: two_moons_eigenvectors.npy")

    plot_weighted_point_graph(
        points,
        adjacency_matrix,
        labels,
        "two_moons_laplasijan_graf.png",
        "Two Moons Gaussian graf",
    )

    plt.figure(figsize=(6, 5))
    plt.scatter(embedding[:, 0], embedding[:, 1], c=labels, cmap="coolwarm", s=35)
    plt.title("Two Moons 2D spektralni embedding")
    plt.xlabel("Drugi svojstveni vektor")
    plt.ylabel("Treci svojstveni vektor")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")
    output_path = "two_moons_spektralni_embedding.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    fig, axes = plt.subplots(2, 2, figsize=(10, 8))
    first_four_eigenvectors = eigenvectors[:, :4]

    for index, ax in enumerate(axes.flat):
        values = first_four_eigenvectors[:, index]
        max_abs_value = np.max(np.abs(values))
        edge_indices = np.argwhere(np.triu(adjacency_matrix > 0, k=1))
        edges = points[edge_indices]

        if len(edges) > 0:
            edge_collection = LineCollection(
                edges,
                colors="gray",
                linewidths=0.25,
                alpha=0.12,
                zorder=1,
            )
            ax.add_collection(edge_collection)

        nodes = ax.scatter(
            points[:, 0],
            points[:, 1],
            c=values,
            cmap="coolwarm",
            vmin=-max_abs_value,
            vmax=max_abs_value,
            s=35,
            edgecolors="black",
            linewidths=0.3,
            zorder=2,
        )
        ax.set_title(
            f"Svojstveni vektor {index + 1}, lambda = {eigenvalues[index]:.2f}"
        )
        ax.set_xlabel("x1")
        ax.set_ylabel("x2")
        ax.grid(True, alpha=0.3)
        ax.axis("equal")
        fig.colorbar(nodes, ax=ax, fraction=0.046, pad=0.04)

    output_path = "two_moons_prva_4_svojstvena_vektora.png"
    fig.tight_layout()
    fig.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    graph_after_fiedler_cut = adjacency_matrix.copy()

    for i, j in fiedler_cut_edges:
        graph_after_fiedler_cut[i, j] = 0
        graph_after_fiedler_cut[j, i] = 0

    plot_weighted_point_graph(
        points,
        graph_after_fiedler_cut,
        fiedler_partition,
        "two_moons_graf_poslije_fiedler_cuta.png",
        f"Two Moons Fiedlerov rez, |cut| = {fiedler_cut_size}",
    )

    graph_after_ratiocut = adjacency_matrix.copy()

    for i, j in ratiocut_edges:
        graph_after_ratiocut[i, j] = 0
        graph_after_ratiocut[j, i] = 0

    plot_weighted_point_graph(
        points,
        graph_after_ratiocut,
        ratiocut_partition,
        "two_moons_graf_poslije_ratiocut.png",
        f"Two Moons RatioCut, RatioCut = {ratiocut:.2f}",
    )

    plot_clustering_comparison(
        points,
        [
            ("Spektralno: znak Fiedlerovog vektora", fiedler_partition),
            ("PCA: split po PC1 medijani", pca_partition),
            ("DBSCAN: eps = 0.2, min_samples = 5", dbscan_labels),
        ],
        "two_moons_poredjenje_klasterisanja.png",
    )


def main():
    # Matrica susjedstva neusmjerenog grafa.
    adjacency_matrix = np.array(
        [
            [0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1],
            [1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
            [1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0],
            [1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0],
            [1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0],
            [1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1],
            [0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1],
            [0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1],
            [0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1],
            [0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0],
        ],
        dtype=float,
    )

    degree_matrix = np.diag(adjacency_matrix.sum(axis=1))
    laplacian_matrix = degree_matrix - adjacency_matrix

    eigenvalues, eigenvectors = np.linalg.eigh(laplacian_matrix)
    fiedler_vector = eigenvectors[:, 1]
    fiedler_partition = fiedler_vector <= 0
    fiedler_cut_edges = find_cut_edges(adjacency_matrix, fiedler_partition)
    fiedler_cut_size = len(fiedler_cut_edges)
    ratiocut_partition, ratiocut_edges, ratiocut_size, ratiocut = find_ratiocut(
        adjacency_matrix,
        fiedler_vector,
    )
    embedding = eigenvectors[:, 1:3]
    plot_embedding = add_plot_jitter(embedding)
    orthogonality_check = eigenvectors.T @ eigenvectors
    identity_matrix = np.eye(eigenvectors.shape[1])
    are_orthonormal = np.allclose(orthogonality_check, identity_matrix)

    print_matrix("Matrica susjedstva", adjacency_matrix)
    print_matrix("Laplasijan grafa", laplacian_matrix)
    print_matrix("Svojstvene vrijednosti Laplasijana", eigenvalues)
    print_matrix("Svojstveni vektori Laplasijana", eigenvectors)
    print_matrix("Fiedlerov vektor", fiedler_vector)
    print_matrix("Spektralni embedding u 2D", embedding)
    print_matrix("Provjera ortonormalnosti V.T @ V", orthogonality_check)
    print("\nJesu li svojstveni vektori ortonormirani?")
    print(are_orthonormal)
    print("\nRez pomocu znaka Fiedlerovog vektora:")
    print("A =", np.where(fiedler_partition)[0].tolist())
    print("B =", np.where(~fiedler_partition)[0].tolist())
    print("Presjecene grane =", fiedler_cut_edges)
    print("Velicina reza =", fiedler_cut_size)
    print("\nRatioCut pomocu Fiedlerovog vektora:")
    print("A =", np.where(ratiocut_partition)[0].tolist())
    print("B =", np.where(~ratiocut_partition)[0].tolist())
    print("Presjecene grane =", ratiocut_edges)
    print("Velicina reza =", ratiocut_size)
    print("RatioCut =", round(ratiocut, 2))

    plt.figure(figsize=(6, 5))

    for i in range(adjacency_matrix.shape[0]):
        for j in range(i + 1, adjacency_matrix.shape[1]):
            if adjacency_matrix[i, j] != 0:
                plt.plot(
                    [plot_embedding[i, 0], plot_embedding[j, 0]],
                    [plot_embedding[i, 1], plot_embedding[j, 1]],
                    color="gray",
                    linewidth=1,
                    zorder=1,
                )

    plt.scatter(plot_embedding[:, 0], plot_embedding[:, 1], s=120, zorder=2)

    for i, (x, y) in enumerate(plot_embedding):
        plt.text(x, y, str(i), ha="center", va="center", color="white")

    plt.title("2D spektralni embedding grafa")
    plt.xlabel("Drugi svojstveni vektor")
    plt.ylabel("Treći svojstveni vektor")
    plt.grid(True, alpha=0.3)
    plt.axis("equal")
    output_path = "spektralni_embedding.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"\nPlot je sacuvan u fajl: {output_path}")

    graph = nx.from_numpy_array(adjacency_matrix)
    positions = nx.spring_layout(graph, seed=42)

    plt.figure(figsize=(6, 5))
    nx.draw_networkx(
        graph,
        pos=positions,
        node_color="tab:blue",
        node_size=800,
        font_color="white",
        edge_color="gray",
    )
    plt.title("Graf nacrtan pomocu spring layouta")
    plt.axis("off")

    output_path = "spring_layout.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    fig, axes = plt.subplots(2, 2, figsize=(10, 8))
    first_four_eigenvectors = eigenvectors[:, :4]

    for index, ax in enumerate(axes.flat):
        values = first_four_eigenvectors[:, index]
        max_abs_value = np.max(np.abs(values))

        nx.draw_networkx_edges(graph, pos=positions, edge_color="gray", ax=ax)
        nodes = nx.draw_networkx_nodes(
            graph,
            pos=positions,
            node_color=values,
            cmap="coolwarm",
            vmin=-max_abs_value,
            vmax=max_abs_value,
            node_size=800,
            edgecolors="black",
            ax=ax,
        )
        nx.draw_networkx_labels(graph, pos=positions, font_color="white", ax=ax)

        ax.set_title(
            f"Svojstveni vektor {index + 1}, lambda = {eigenvalues[index]:.2f}"
        )
        ax.axis("off")
        fig.colorbar(nodes, ax=ax, fraction=0.046, pad=0.04)

    output_path = "prva_4_svojstvena_vektora.png"
    fig.tight_layout()
    fig.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    node_colors = [
        "tab:orange" if fiedler_partition[node] else "tab:blue"
        for node in graph
    ]
    graph_after_cut = graph.copy()
    graph_after_cut.remove_edges_from(fiedler_cut_edges)

    plt.figure(figsize=(6, 5))
    nx.draw_networkx(
        graph_after_cut,
        pos=positions,
        node_color=node_colors,
        node_size=800,
        font_color="white",
        edge_color="gray",
    )
    plt.title(f"Graf poslije Fiedlerovog reza, |cut| = {fiedler_cut_size}")
    plt.axis("off")

    output_path = "graf_poslije_fiedler_cuta.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    node_colors = [
        "tab:orange" if ratiocut_partition[node] else "tab:blue"
        for node in graph
    ]
    graph_after_cut = graph.copy()
    graph_after_cut.remove_edges_from(ratiocut_edges)

    plt.figure(figsize=(6, 5))
    nx.draw_networkx(
        graph_after_cut,
        pos=positions,
        node_color=node_colors,
        node_size=800,
        font_color="white",
        edge_color="gray",
    )
    plt.title(f"Graf poslije RatioCut reza, RatioCut = {ratiocut:.2f}")
    plt.axis("off")

    output_path = "graf_poslije_ratiocut.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Plot je sacuvan u fajl: {output_path}")

    analyze_two_moons_graph()

    points, labels, adjacency_matrix, sigma, weight_threshold = (
        make_variable_density_gaussian_graph()
    )
    print(f"\nVariable density Gaussian parametri: sigma = {sigma}, prag = {weight_threshold}")
    compare_clustering_methods(
        "Razlicita gustoca clustera",
        "variable_density",
        points,
        labels,
        adjacency_matrix,
        dbscan_eps=0.35,
    )


if __name__ == "__main__":
    main()
