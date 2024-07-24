import colorsys
import networkx as nx
import matplotlib.pyplot as plt
import sys
import os

INPUT_NAME = sys.argv[1]

def parse_graph_file(file_path):
  with open(file_path, 'r') as file:
    graph = nx.DiGraph()

    n, _, _, _ = file.readline().strip().split()

    file.readline()

    for i in range(int(n)):
      graph.add_node(i)

    for line in file:
      a, b = line.strip().split()
      graph.add_edge(int(a), int(b))
    
    return graph

assert 'SCHED_TC_FOLDER' in os.environ

g = parse_graph_file(f"{os.environ['SCHED_TC_FOLDER']}/{INPUT_NAME}")
n = g.number_of_nodes()

for layer, nodes in enumerate(nx.topological_generations(g)):
  for node in nodes:
    g.nodes[node]["layer"] = layer

color_order = sorted(list(range(n)), key=lambda i: (g.nodes[i]["layer"], i))
color_rank = [color_order.index(i) for i in range(n)]

node_color = [colorsys.hsv_to_rgb(color_rank[i] / n, 0.5, 1) for i in range(n)]

nx.draw_networkx(
  g,
  pos=nx.multipartite_layout(g, subset_key="layer"),
  node_color=node_color,
  with_labels=True
)

plt.show()
