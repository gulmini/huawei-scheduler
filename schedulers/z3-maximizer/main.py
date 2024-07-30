from z3 import *

FILE_NUMBER = sys.argv[1]

TESTCASE_FILE_PATH = f"{os.environ.get("SCHED_TC_FOLDER")}/{FILE_NUMBER}.tc"
SCHEDULE_FILE_PATH = f"{os.environ.get("SCHED_OUTPUT_FOLDER")}/z3-maximizer/{FILE_NUMBER}.sched"

file = open(TESTCASE_FILE_PATH, "r")
n, m, p, gamma = map(int, file.readline().strip().split())
omega = list(map(int, file.readline().strip().split()))
edges = []

for i in range(m):
  edges.append(tuple(map(int, file.readline().strip().split())))

file.close()

s_variables = [Int(f"s{i}") for i in range(n)]
c_variables = [Int(f"c{i}") for i in range(n)]
b_i_j_1_variables = [Bool(f"b{i}_{j}_1") for i in range(n) for j in range(n)]
b_i_j_2_variables = [Bool(f"b{i}_{j}_2") for i in range(n) for j in range(n)]
b_i_j_3_variables = [Bool(f"b{i}_{j}_3") for i in range(n) for j in range(n)]
d_i_j_k_variables = [Bool(f"d{i}_{j}_{k}") for i in range(n) for j in range(n) for k in range(gamma)]

T = Int("T")

opt = Optimize()

# processo aggiunto a solo un core
for i in range(n):
  opt.add(c_variables[i] >= 0)
  opt.add(c_variables[i] < gamma)

M = 1_000_000

for i in range(n):
  s_i = s_variables[i]
  omega_i = omega[i]
  c_i = c_variables[i]

  # Non-negativita'
  opt.add(s_i >= 0)

  # finisce in tempo
  opt.add(s_i + omega_i <= T)

  for j in range(i + 1, n):
    s_j = s_variables[j]
    omega_j = omega[j]
    c_j = c_variables[j]
    b_i_j_1 = b_i_j_1_variables[i * n + j]
    b_i_j_2 = b_i_j_2_variables[i * n + j]
    b_i_j_3 = b_i_j_3_variables[i * n + j]
    d_i_j_k = [d_i_j_k_variables[i * n * gamma + j * gamma + k] for k in range(gamma)]
      
    # non sovrapposizione
    opt.add(s_i + omega_i <= s_j + M * (1 - If(b_i_j_1, 1, 0)))
    opt.add(s_j + omega_j <= s_i + M * (1 - If(b_i_j_2, 1, 0)))
    opt.add(b_i_j_1 + b_i_j_2 >= 1)

    # for k in range(gamma):
    #   opt.add(d_i_j_k[k] >= c_i - k + 1 - M * (1 - If(b_i_j_3, 1, 0)))
    #   opt.add(d_i_j_k[k] >= k - c_i + 1 - M * (1 - If(b_i_j_3, 1, 0)))
    #   opt.add(d_i_j_k[k] <= M * (1 - If(b_i_j_3, 1, 0)))
    #   opt.add(d_i_j_k[k] <= c_i - k)
    #   opt.add(d_i_j_k[k] <= k - c_i)

    # opt.add(
    #   Or(
    #     c_i != c_j,
    #     s_i + omega_i <= s_j,
    #     s_j + omega_j <= s_i
    #   )
    # )

# vincolo di precedenza
for i, j in edges:
  opt.add(s_variables[i] + omega[i] <= s_variables[j])

# return opt
opt.minimize(T)

if opt.check() == sat:
  model = opt.model()

  file = open(SCHEDULE_FILE_PATH, "w")

  file.write(f"{n} {p} {gamma}\n")  

  for s_i, c_i, w_i in zip(s_variables, c_variables, omega):
    s_i = model[s_i].as_long()
    c_i = model[c_i].as_long()
    file.write(f"{s_i} {s_i + w_i} {c_i}\n")

else:
  print("No solution found.")


