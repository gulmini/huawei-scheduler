import numpy as np
import csv
import matplotlib.pyplot as plt

n = 100

execution_times = [0] * n

iterations = 4

for iteration in range(iterations):

  with open(f'output/benchmark/bin_packing_{iteration + 1}.bench', 'r') as file:
    reader = csv.reader(file)
    for i, row in enumerate(reader):
      if i == n:
        break
      execution_times[i] += 100000 if row[1] == '-1' else float(row[1])

x_steps = [30, 40, 50, 60, 70]
y_steps = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

# 100 => (0, 0)
# 119 => (0, 19)
# 120 => (1, 0)


x = np.array(x_steps)
y = np.array(y_steps)

z = np.array([
  [execution_times[y + 20 * x] / iterations for x in range(5)]
  for y in range(10)
])

ax = plt.gca()

ax.set_xticks(x_steps)
ax.set_yticks(y_steps)

plt.contourf(x, y, z, 20, cmap='RdYlGn_r')
plt.show()