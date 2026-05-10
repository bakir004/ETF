#!/usr/bin/env python3
"""
Spring/force-directed animacija grafa.

VAZNA NAPOMENA:
Jednostavan neusmjeren graf sa 15 cvorova gdje je svaki stepen 3 ne postoji,
jer bi suma stepeni bila 15 * 3 = 45, a suma stepeni mora biti paran broj.
Zato skripta automatski koristi 16 cvorova i pravi 3-regularan graf.

Pokretanje:
    python spring.py

Izlaz:
    spring_animacija/frames/frame_0000.png ...
    spring_animacija/spring_animacija.mp4   ako postoji ffmpeg
    spring_animacija/spring_animacija.gif   fallback ako nema ffmpeg
    spring_animacija/final.png

Koristi samo standardni Python + Pillow. Numpy/matplotlib nisu potrebni.
"""

from pathlib import Path
from itertools import combinations
import math
import random
import shutil
import subprocess

from PIL import Image, ImageDraw, ImageFont


REQUESTED_N = 12
DEGREE = 3
ITERATIONS = 350
FPS = 30
WIDTH = 900
HEIGHT = 900

# Manja temperatura + damping daju mirniju animaciju bez velikog preskakanja.
INITIAL_TEMPERATURE = 0.08
MIN_TEMPERATURE = 0.001
DAMPING = 0.55

# Tutteov algoritam: vanjska strana je fiksirana kao konveksan trougao,
# a unutrasnji cvorovi se iterativno pomjeraju prema tezistu svojih susjeda.
TUTTE_STEP = 0.18
OUTER_FACE = (0, 1, 2)

OUT_DIR = Path("spring_animacija")
FRAMES_DIR = OUT_DIR / "frames"


def make_cubic_graph(requested_n: int):
    """Planarni 3-connected graf za Tutteovo crtanje.

    Graf nije uzet kao kocka/dodekaedar/3D objekat. Napravljen je kao
    triangulacija: krene se od vanjskog trougla (0, 1, 2), pa se novi cvorovi
    ubacuju u postojece trougaone face i spajaju sa tri vrha te face.
    Takav maksimalno planaran graf je 3-connected.

    Vanjska strana za Tutteov algoritam je OUTER_FACE = (0, 1, 2).
    """
    n = 12
    edges = set()
    faces = [tuple(sorted((0, 1, 2)))]

    def add_edge(u, v):
        edges.add(tuple(sorted((u, v))))

    def face_key(a, b, c):
        return tuple(sorted((a, b, c)))

    add_edge(0, 1)
    add_edge(1, 2)
    add_edge(2, 0)

    insertions = [
        (3, (0, 1, 2)),
        (4, (0, 1, 3)),
        (5, (1, 2, 3)),
        (6, (0, 2, 3)),
        (7, (0, 3, 4)),
        (8, (1, 3, 5)),
        (9, (2, 3, 6)),
        (10, (3, 4, 7)),
        (11, (3, 5, 8)),
    ]

    for new_vertex, face in insertions:
        a, b, c = face
        faces.remove(face_key(a, b, c))
        add_edge(new_vertex, a)
        add_edge(new_vertex, b)
        add_edge(new_vertex, c)
        faces.extend([
            face_key(a, b, new_vertex),
            face_key(b, c, new_vertex),
            face_key(c, a, new_vertex),
        ])

    edges = sorted(edges)
    degrees = [0] * n
    for u, v in edges:
        degrees[u] += 1
        degrees[v] += 1

    assert len(edges) == 3 * n - 6
    return n, edges, degrees


def add(a, b):
    return (a[0] + b[0], a[1] + b[1])


def sub(a, b):
    return (a[0] - b[0], a[1] - b[1])


def mul(a, s):
    return (a[0] * s, a[1] * s)


def norm(a):
    return math.sqrt(a[0] * a[0] + a[1] * a[1])


def jacobi_eigen_symmetric(matrix, max_iter=5000, tol=1e-12):
    """Eigen-dekompozicija male realne simetricne matrice Jacobi metodom.

    Vraca (eigenvalues, eigenvectors), gdje je eigenvectors[i][k] i-ta
    komponenta k-tog eigenvektora. Ovo je dovoljno za male grafove i ne trazi
    numpy/scipy.
    """
    n = len(matrix)
    a = [row[:] for row in matrix]
    v = [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]

    for _ in range(max_iter):
        # Nadji najveci vandijagonalni element.
        p, q = 0, 1
        max_off = abs(a[p][q])
        for i in range(n):
            for j in range(i + 1, n):
                if abs(a[i][j]) > max_off:
                    max_off = abs(a[i][j])
                    p, q = i, j

        if max_off < tol:
            break

        if abs(a[p][p] - a[q][q]) < 1e-15:
            angle = math.pi / 4
        else:
            angle = 0.5 * math.atan2(2 * a[p][q], a[q][q] - a[p][p])

        c = math.cos(angle)
        s = math.sin(angle)

        app = a[p][p]
        aqq = a[q][q]
        apq = a[p][q]

        a[p][p] = c * c * app - 2 * s * c * apq + s * s * aqq
        a[q][q] = s * s * app + 2 * s * c * apq + c * c * aqq
        a[p][q] = 0.0
        a[q][p] = 0.0

        for r in range(n):
            if r != p and r != q:
                arp = a[r][p]
                arq = a[r][q]
                a[r][p] = c * arp - s * arq
                a[p][r] = a[r][p]
                a[r][q] = s * arp + c * arq
                a[q][r] = a[r][q]

        for r in range(n):
            vrp = v[r][p]
            vrq = v[r][q]
            v[r][p] = c * vrp - s * vrq
            v[r][q] = s * vrp + c * vrq

    eigenvalues = [a[i][i] for i in range(n)]
    return eigenvalues, v


def spectral_initial_positions(n, edges):
    """Napravi 2D layout iz eigenvektora Laplasove matrice grafa."""
    adjacency = [[0.0 for _ in range(n)] for _ in range(n)]
    degree = [0 for _ in range(n)]

    for u, v in edges:
        adjacency[u][v] = 1.0
        adjacency[v][u] = 1.0
        degree[u] += 1
        degree[v] += 1

    laplacian = [[0.0 for _ in range(n)] for _ in range(n)]
    for i in range(n):
        laplacian[i][i] = float(degree[i])
        for j in range(n):
            if i != j and adjacency[i][j]:
                laplacian[i][j] = -1.0

    eigenvalues, eigenvectors = jacobi_eigen_symmetric(laplacian)
    order = sorted(range(n), key=lambda k: eigenvalues[k])

    # order[0] je eigenvalue 0 sa konstantnim eigenvektorom.
    x_col = order[1]
    y_col = order[2]
    points = [(eigenvectors[i][x_col], eigenvectors[i][y_col]) for i in range(n)]

    print("Spektralne eigenvalues Laplasijana:")
    print([round(eigenvalues[k], 6) for k in order[:6]])
    print("Koristim eigenvektore za eigenvalues:", round(eigenvalues[x_col], 6), round(eigenvalues[y_col], 6))
    return points


def outer_triangle_from_spectral(points):
    """Napravi lijep jednakostranican vanjski trougao.

    Velicina je relativna pocetnom spektralnom layoutu, ali oblik je namjerno
    jednakostranican da vanjska strana ne bude uska ili spljostena.
    """
    cx = sum(x for x, _ in points) / len(points)
    cy = sum(y for _, y in points) / len(points)

    xs = [x for x, _ in points]
    ys = [y for _, y in points]
    width = max(xs) - min(xs)
    height = max(ys) - min(ys)
    size = max(width, height, 1e-9)

    radius = 0.75 * size
    angles = [-math.pi / 2, math.pi / 6, 5 * math.pi / 6]
    return [(cx + radius * math.cos(a), cy + radius * math.sin(a)) for a in angles]


def spring_layout_steps(n, edges, iterations=ITERATIONS, seed=7):
    """Tutte/spring iteracije; vraca listu pozicija kroz vrijeme.

    Pocetna slika je spektralna. Zatim vanjsku stranu fiksiramo kao konveksan
    trougao, a svaki unutrasnji cvor ponavljano pomjeramo prema prosjeku
    susjeda. To je Tutteov barycentric/spring embedding: za planaran
    3-connected graf daje crtanje bez presijecanja grana.
    """
    random.seed(seed)

    spectral_pos = spectral_initial_positions(n, edges)
    outer_targets = outer_triangle_from_spectral(spectral_pos)

    # Unutrasnji cvorovi krecu bas iz spektralnog layouta. Samo tri cvora
    # vanjske strane fiksiramo u konveksan trougao slicne velicine kao pocetna
    # spektralna slika.
    pos = spectral_pos[:]

    outer = set(OUTER_FACE)
    for vertex, target in zip(OUTER_FACE, outer_targets):
        pos[vertex] = target

    adjacency = [[] for _ in range(n)]
    for u, v in edges:
        adjacency[u].append(v)
        adjacency[v].append(u)

    history = [pos[:]]

    for _ in range(iterations):
        new_pos = pos[:]
        for i in range(n):
            if i in outer:
                continue
            avg_x = sum(pos[j][0] for j in adjacency[i]) / len(adjacency[i])
            avg_y = sum(pos[j][1] for j in adjacency[i]) / len(adjacency[i])

            # Relaksacija: ne skacemo odmah u teziste, nego animiramo put.
            new_pos[i] = (
                pos[i][0] + TUTTE_STEP * (avg_x - pos[i][0]),
                pos[i][1] + TUTTE_STEP * (avg_y - pos[i][1]),
            )

        pos = new_pos
        history.append(pos[:])

    return history


def canvas_transform_from_history(history):
    """Izracunaj fiksan centar/skalu za cijelu animaciju.

    Ako svaki frame posebno normalizujemo, kamera stalno zumira i to izgleda
    kao dodatna oscilacija. Fiksna skala pravi mnogo mirniji video.
    """
    all_points = [p for frame in history for p in frame]
    cx = sum(p[0] for p in all_points) / len(all_points)
    cy = sum(p[1] for p in all_points) / len(all_points)
    max_abs = max(max(abs(x - cx), abs(y - cy)) for x, y in all_points)
    if max_abs < 1e-9:
        max_abs = 1.0
    return cx, cy, max_abs


def normalize_for_canvas(points, transform):
    """Pretvori pozicije u piksele koristeci fiksnu skalu."""
    cx, cy, max_abs = transform
    # Manji margin = veci crtez u kadru, ali cvorovi su ispod smanjeni
    # da se grane i struktura bolje vide.
    margin = 55
    scale = min(WIDTH, HEIGHT) / 2 - margin

    pixels = []
    for x, y in points:
        px = WIDTH / 2 + ((x - cx) / max_abs) * scale
        py = HEIGHT / 2 - ((y - cy) / max_abs) * scale
        pixels.append((px, py))
    return pixels


def load_font(size):
    try:
        return ImageFont.truetype("DejaVuSans.ttf", size)
    except OSError:
        return ImageFont.load_default()


def draw_frame(n, edges, points, frame_idx, total_frames, path, transform):
    img = Image.new("RGB", (WIDTH, HEIGHT), "white")
    draw = ImageDraw.Draw(img)
    font = load_font(15)
    small_font = load_font(24)

    p = normalize_for_canvas(points, transform)

    title = f"Tutte spring embedding - iteracija {frame_idx}/{total_frames - 1}"
    draw.text((30, 25), title, fill=(30, 30, 30), font=small_font)

    # Grane
    for u, v in edges:
        draw.line([p[u], p[v]], fill=(120, 120, 120), width=4)

    # Cvorovi
    r = 8
    for i, (x, y) in enumerate(p):
        bbox = [x - r, y - r, x + r, y + r]
        draw.ellipse(bbox, fill=(125, 183, 255), outline=(31, 78, 121), width=3)

        label = str(i)
        label_box = draw.textbbox((0, 0), label, font=font)
        tw = label_box[2] - label_box[0]
        th = label_box[3] - label_box[1]
        draw.text((x - tw / 2, y - th / 2 - 2), label, fill=(0, 0, 0), font=font)

    img.save(path)


def create_video_or_gif():
    mp4_path = OUT_DIR / "spring_animacija.mp4"
    gif_path = OUT_DIR / "spring_animacija.gif"

    if shutil.which("ffmpeg"):
        cmd = [
            "ffmpeg",
            "-y",
            "-framerate", str(FPS),
            "-i", str(FRAMES_DIR / "frame_%04d.png"),
            "-c:v", "libx264",
            "-pix_fmt", "yuv420p",
            str(mp4_path),
        ]
        subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        print(f"Snimljena animacija: {mp4_path}")
        return

    # Fallback GIF ako nema ffmpeg.
    frame_paths = sorted(FRAMES_DIR.glob("frame_*.png"))
    frames = [Image.open(p) for p in frame_paths]
    frames[0].save(
        gif_path,
        save_all=True,
        append_images=frames[1:],
        duration=int(1000 / FPS),
        loop=0,
    )
    print(f"ffmpeg nije pronadjen, snimljen GIF: {gif_path}")


def save_animation(n, edges, history):
    OUT_DIR.mkdir(exist_ok=True)
    FRAMES_DIR.mkdir(exist_ok=True)

    # Obrisi stare frameove da ffmpeg ne pokupi visak.
    for old in FRAMES_DIR.glob("frame_*.png"):
        old.unlink()

    total = len(history)
    transform = canvas_transform_from_history(history)
    for i, points in enumerate(history):
        frame_path = FRAMES_DIR / f"frame_{i:04d}.png"
        draw_frame(n, edges, points, i, total, frame_path, transform)

    final_png = OUT_DIR / "final.png"
    draw_frame(n, edges, history[-1], total - 1, total, final_png, transform)
    print(f"Snimljeno {total} frameova u: {FRAMES_DIR}")
    print(f"Snimljena finalna slika: {final_png}")

    create_video_or_gif()


def is_connected_after_removing(n, edges, removed):
    removed = set(removed)
    remaining = [v for v in range(n) if v not in removed]
    if not remaining:
        return True

    adjacency = [[] for _ in range(n)]
    for u, v in edges:
        if u not in removed and v not in removed:
            adjacency[u].append(v)
            adjacency[v].append(u)

    seen = {remaining[0]}
    stack = [remaining[0]]
    while stack:
        u = stack.pop()
        for v in adjacency[u]:
            if v not in seen:
                seen.add(v)
                stack.append(v)

    return len(seen) == len(remaining)


def verify_3_connected(n, edges):
    """Provjeri definiciju: nema 1-cut ni 2-cut, a postoji 3-cut."""
    for k in (1, 2):
        for removed in combinations(range(n), k):
            if not is_connected_after_removing(n, edges, removed):
                return False, f"Nije 3-connected; cut od {k} cvorova: {removed}"

    for removed in combinations(range(n), 3):
        if not is_connected_after_removing(n, edges, removed):
            return True, f"3-connected; primjer 3-cut skupa: {removed}"

    return True, "Najmanje 3-connected; nisam nasao 3-cut u brute-force provjeri."


def main():
    n, edges, degrees = make_cubic_graph(REQUESTED_N)
    print("Dataset: planarna triangulacija, 3-connected; pocetak je spektralni layout")
    print(f"Broj cvorova: {n}")
    print(f"Broj grana: {len(edges)}")
    print(f"Stepeni: {degrees}")
    ok, msg = verify_3_connected(n, edges)
    print("Provjera 3-connected:", msg)
    print(f"Grane: {edges}")

    history = spring_layout_steps(n, edges)
    save_animation(n, edges, history)


if __name__ == "__main__":
    main()
