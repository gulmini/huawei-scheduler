import matplotlib.pyplot as plt
import sys

FILE_PATH = sys.argv[1]

def parse_schedule(path):
  with open(path, 'r') as file:
    n, gamma, p = map(int, file.readline().strip().split())
    tasks = [tuple(map(int, file.readline().strip().split())) for _ in range(n)]

    return (n, gamma, p, tasks)

n, gamma, p, tasks = parse_schedule(FILE_PATH)

ax = plt.gca()
ax.set_xlim([0, gamma])
ax.set_ylim([0, p + 1])

plt.xticks(range(gamma))
plt.yticks(range(p + 1))

for i, task in enumerate(tasks):
  start, end, core = task
  ax.add_patch(plt.Rectangle((start, core), end - start, 0.8, color='blue', alpha=0.5))

ax.grid(True)

plt.show()


