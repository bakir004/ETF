import matplotlib.pyplot as plt
import networkx as nx
import streamlit as st


st.set_page_config(page_title="NetworkX graf", layout="wide")
st.title("Interaktivno crtanje grafa pomoću NetworkX-a")

st.sidebar.header("Parametri grafa")

tip_grafa = st.sidebar.selectbox(
    "Tip grafa",
    [
        "Erdos-Renyi slučajni graf",
        "Ciklus",
        "Put",
        "Potpuni graf",
        "Barabasi-Albert graf",
        "Watts-Strogatz small-world graf",
    ],
)

n = st.sidebar.slider("Broj čvorova", min_value=3, max_value=100, value=20, step=1)
seed = st.sidebar.number_input("Seed", min_value=0, max_value=10_000, value=42, step=1)

if tip_grafa == "Erdos-Renyi slučajni graf":
    p = st.sidebar.slider("Vjerovatnoća grane p", 0.0, 1.0, 0.2, 0.01)
    G = nx.erdos_renyi_graph(n, p, seed=seed)
elif tip_grafa == "Ciklus":
    G = nx.cycle_graph(n)
elif tip_grafa == "Put":
    G = nx.path_graph(n)
elif tip_grafa == "Potpuni graf":
    G = nx.complete_graph(n)
elif tip_grafa == "Barabasi-Albert graf":
    m = st.sidebar.slider("Broj novih grana po čvoru m", 1, min(10, n - 1), 2)
    G = nx.barabasi_albert_graph(n, m, seed=seed)
else:
    k = st.sidebar.slider("Broj susjeda k", 2, min(20, n - 1), 4, step=2)
    p_rewire = st.sidebar.slider("Vjerovatnoća prespajanja", 0.0, 1.0, 0.2, 0.01)
    G = nx.watts_strogatz_graph(n, k, p_rewire, seed=seed)

st.sidebar.header("Prikaz")
layout = st.sidebar.selectbox(
    "Layout",
    ["spring", "circular", "kamada_kawai", "shell", "spectral"],
)
show_labels = st.sidebar.checkbox("Prikaži oznake čvorova", value=True)
node_size = st.sidebar.slider("Veličina čvorova", 50, 2000, 500, 50)
edge_width = st.sidebar.slider("Debljina grana", 0.5, 5.0, 1.5, 0.1)

if layout == "spring":
    pos = nx.spring_layout(G, seed=seed)
elif layout == "circular":
    pos = nx.circular_layout(G)
elif layout == "kamada_kawai":
    pos = nx.kamada_kawai_layout(G)
elif layout == "shell":
    pos = nx.shell_layout(G)
else:
    pos = nx.spectral_layout(G)

fig, ax = plt.subplots(figsize=(9, 7))
nx.draw_networkx_nodes(
    G,
    pos,
    ax=ax,
    node_size=node_size,
    node_color="skyblue",
    edgecolors="black",
    linewidths=0.8,
)
nx.draw_networkx_edges(G, pos, ax=ax, width=edge_width, alpha=0.7)

if show_labels:
    nx.draw_networkx_labels(G, pos, ax=ax, font_size=9)

ax.set_title(tip_grafa)
ax.axis("off")
st.pyplot(fig)
plt.close(fig)

col1, col2, col3 = st.columns(3)
col1.metric("Broj čvorova", G.number_of_nodes())
col2.metric("Broj grana", G.number_of_edges())
col3.metric("Broj komponenti", nx.number_connected_components(G))

with st.expander("Osnovne informacije"):
    st.write("Stepeni čvorova:")
    st.dataframe(
        {"čvor": list(dict(G.degree()).keys()), "stepen": list(dict(G.degree()).values())},
        use_container_width=True,
    )

st.caption("Pokretanje: streamlit run streamlit_networkx_graf.py")
