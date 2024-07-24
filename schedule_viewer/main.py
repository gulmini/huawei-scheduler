import matplotlib.pyplot as plt
import colorsys
import networkx as nx
import sys
import os

FILE_NUMBER = sys.argv[1]

SCHEDULE_FILE_PATH = f"{os.environ.get("SCHED_OUTPUT_FOLDER")}/{FILE_NUMBER}"
TESTCASE_FILE_PATH = f"{os.environ.get("SCHED_TC_FOLDER")}/{FILE_NUMBER}"
VISUALIZATION_PATH = f"{os.environ.get("SCHED_VIS_FOLDER")}/{FILE_NUMBER}.png"

def parse_schedule(path):
  with open(path, 'r') as file:
    n, gamma, p = map(int, file.readline().strip().split())
    tasks = [tuple(map(int, file.readline().strip().split())) for _ in range(n)]

    return (n, gamma, p, tasks)

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

def last_end_time(tasks):
  return max(task[1] for task in tasks)

n, p, gamma, tasks = parse_schedule(SCHEDULE_FILE_PATH)
last_time = last_end_time(tasks)

g = parse_graph(TESTCASE_FILE_PATH)
g = assign_layer(g)
node_colors = node_colors(g)

ax = plt.gca()

ax.set_xlim([0, last_end_time(tasks) + 1])
ax.set_ylim([0, gamma])

plt.xticks(range(last_time + 1))
plt.yticks(range(gamma))

plt.axvline(x=max(p, last_time), color='red', linestyle='-', linewidth=2)

for i, task in enumerate(tasks):
  start, end, core = task
  ax.add_patch(plt.Rectangle((start, core), end - start, 0.8, color=node_colors[i], alpha=0.5))

ax.grid(True)

plt.show()


