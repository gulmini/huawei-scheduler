from math import sqrt
import networkx as nx 
import matplotlib.pyplot as plt
import colorsys

g = nx.DiGraph()

n = 6

for i in range(n):
  g.add_node(i)

g.add_edge(0, 3)
g.add_edge(0, 4)
g.add_edge(0, 5)

# g.add_edge(1, 3)
# g.add_edge(1, 4)
g.add_edge(1, 5)

g.add_edge(2, 3)
g.add_edge(2, 4)
# g.add_edge(2, 5)

topological_order = list(nx.topological_sort(g))

roots = [node for node in g.nodes if g.in_degree(node) == 0]

def x(node):
  path_lengths = [0]
  for root in roots:
    for path in nx.all_simple_paths(g, root, node):
      path_lengths.append(len(path))
  return max(path_lengths)

base_pos = nx.spring_layout(g, k=100)

color_index = sorted(list(range(n)), key=lambda i: (x(i), i))

node_color = [colorsys.hsv_to_rgb(i / n, 0.5, 1) for i in range(n)]

color_map = [node_color[i] for i in color_index]

nx.draw(
  g,
  pos={
    node: (x(node), base_pos[node][1])
    for node in g.nodes
  },
  with_labels=True,
  arrows=True,
  node_color=color_map
)

plt.show()
