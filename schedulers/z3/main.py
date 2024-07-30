from z3 import *

FILE_NUMBER = sys.argv[1]

TESTCASE_FILE_PATH = f"{os.environ.get("SCHED_TC_FOLDER")}/{FILE_NUMBER}.tc"
SCHEDULE_FILE_PATH = f"{os.environ.get("SCHED_OUTPUT_FOLDER")}/z3/{FILE_NUMBER}.sched"

file = open(TESTCASE_FILE_PATH, "r")
n, m, p, gamma = map(int, file.readline().strip().split())
omega = list(map(int, file.readline().strip().split()))
edges = []

for i in range(m):
  edges.append(tuple(map(int, file.readline().strip().split())))

file.close()

s_variables = [Int(f"s{i}") for i in range(n)]
c_variables = [Int(f"c{i}") for i in range(n)]

def get_solver(p):
  solver = Solver()

  # processo aggiunto a solo un core
  for i in range(n):
    solver.add(c_variables[i] >= 0)
    solver.add(c_variables[i] < gamma)

  for i in range(n):
    s_i = s_variables[i]
    omega_i = omega[i]
    c_i = c_variables[i]

    # Non-negativita'
    solver.add(s_i >= 0)

    # finisce in tempo
    solver.add(s_i + omega_i <= p)

    for j in range(i + 1, n):
      s_j = s_variables[j]
      omega_j = omega[j]
      c_j = c_variables[j]
        
      # non sovrapposizione
      solver.add(
        Or(
          c_i != c_j,
          s_i + omega_i <= s_j,
          s_j + omega_j <= s_i
        )
      )

  # vincolo di precedenza
  for i, j in edges:
    solver.add(s_variables[i] + omega[i] <= s_variables[j])

  return solver


def binary_search():
  left = 0
  right = p

  while left < right:
    mid = (left + right) // 2

    if get_solver(mid).check() == sat:
      right = mid
    else:
      left = mid + 1

  return left


max_p = binary_search()
solver = get_solver(max_p)


if solver.check() == sat:
  model = solver.model()

  file = open(SCHEDULE_FILE_PATH, "w")

  file.write(f"{n} {p} {gamma}\n")

  for s_i, c_i, w_i in zip(s_variables, c_variables, omega):
    s_i = model[s_i].as_long()
    c_i = model[c_i].as_long()
    file.write(f"{s_i} {s_i + w_i} {c_i}\n")

else:
  print("No solution found.")


