# Felipe Brigalante    8941280
# Mateus Rocha Mazzari 8941255

import numpy as np
import matplotlib.pyplot as plt

import re

# make({thread: 1, size: [10, 100], method: ["omp", "pth"], region: "full", x: "size", line: "method"})
def make_plot(arg):
  x_type = arg["x"]
  x_value = arg[x_type]
  fixed_x_type = "method" if(x_type == "region") else "region"
  fixed_x_value = arg[fixed_x_type]
  colors = ['g', 'b', 'r', 'm'] # adicionar mais 1

  bar = []
  for item in data:
    if(item[fixed_x_type] == fixed_x_value and item[x_type] in x_value):
      bar.append((item[x_type], item["time"], item["deviation"]))
  subtitle = []
  fig, ax = plt.subplots()
  groups = len(bar)
  index = np.arange(groups)
  width = 0.35
  if (bar[0]):
    bar.sort(key = lambda x: x[0])
    axis = map(list, zip(*bar))
    errors = [a * b / 100 for a,b in zip(axis[1], axis[2])]
    plt.bar(index, axis[1], width, yerr=errors)
    plt.xticks(index, axis[0])

  box = ax.get_position()
  ax.set_position([box.x0, box.y0 + box.height * 0.1,
                 box.width, box.height * 0.9])
  plt.ylabel("time (s)")
  plt.ylim(ymin=0)
  plt.xlabel(x_type)
  # plt.xlim(xmin=0)

  plt.show()

methods = ["seq", "mpi_1", "mpi_2"] #, "mpi_2", "mpi_4", "mpi_8"]
regions = ["elephant", "full", "seahorse", "triple_spiral"]

data = []
pattern = "([\d\.]+) seconds time elapsed\s+\( \+\-\s+([\d\.]+)\% \)"
regex = re.compile(pattern)

for method in methods:
  for region in regions:
    filename = "results/mandelbrot_{0}/{1}.log".format(method, region)
    textfile = open(filename, 'r')
    filetext = textfile.read()
    textfile.close()
    matches = regex.findall(filetext)
    for match in matches:
      item = {}
      item["method"]    = method
      item["region"]    = region
      item["time"]      = float(match[0].replace(',', '.'))
      item["deviation"] = float(match[1].replace(',', '.'))
      data.append(item)
print data
make_plot({"region": "seahorse", "method": methods, "x": "method"})
