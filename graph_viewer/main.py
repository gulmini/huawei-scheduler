import colorsys
import networkx as nx
import matplotlib.pyplot as plt
import sys
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--display', action='store_true')
options = parser.parse_known_args()

FILE_NUMBER = f"{sys.argv[1]:03}"

TESTCASE_FILE_PATH = f"{os.environ.get("SCHED_TC_FOLDER")}/{FILE_NUMBER}.tc"

if len(sys.argv) > 2:
  VISUALIZATION_PATH = sys.argv[2]
else:
  VISUALIZATION_PATH = f"{os.environ.get("SCHED_GRAPH_VIS_FOLDER")}/{FILE_NUMBER}.png"
  os.makedirs(f"{os.environ.get("SCHED_GRAPH_VIS_FOLDER")}", exist_ok=True)


def parse_graph(file_path):
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


def assign_layer(g):
  for layer, nodes in enumerate(nx.topological_generations(g)):
    for node in nodes:
      g.nodes[node]["layer"] = layer
  return g

def node_colors(g):
  color_order = sorted(list(range(n)), key=lambda i: (g.nodes[i]["layer"], i))
  color_rank = [color_order.index(i) for i in range(n)]
  return [colorsys.hsv_to_rgb(color_rank[i] / n, 0.5, 1) for i in range(n)]

g = parse_graph(TESTCASE_FILE_PATH)
n = g.number_of_nodes()

g = assign_layer(g)
node_color = node_colors(g)

nx.draw_networkx(
  g,
  pos=nx.multipartite_layout(g, subset_key="layer"),
  node_color=node_color,
  with_labels=True
)

fig = plt.gcf()

if options[0].display:
  plt.show()
else:
  fig.savefig(VISUALIZATION_PATH)
