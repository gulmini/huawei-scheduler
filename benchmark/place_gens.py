f = open("benchmark/gens", "r")

lines = f.readlines()

for i, line in enumerate(lines):
  fw = open(f"input/gen/{i+100}.gen", "w")
  fw.write(line)
  fw.close()