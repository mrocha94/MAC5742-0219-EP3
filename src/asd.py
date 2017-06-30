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

  bars = {}
  for x in fixed_x_value:
    bars[x] = []
  for item in data:
    if(item[fixed_x_type] in fixed_x_value and item[x_type] in x_value):
      bars[item[fixed_x_type]].append((item[x_type], item["time"], item["deviation"]))
  subtitle = []
  fig, ax = plt.subplots()
  groups = len(bars[fixed_x_value[0]])
  fig.set_size_inches(10, 5)
  index = np.arange(groups)
  width = 0.17
  tab = 0
  print bars
  for fixed, bar in sorted(bars.iteritems()):
    if (bar[0]):
      bar.sort(key = lambda x: x[0])
      axis = map(list, zip(*bar))
      errors = [a * b / 100 for a,b in zip(axis[1], axis[2])]
      plt.bar(index + tab * width, axis[1], width, yerr=errors)
      plt.xticks(index + len(bars) / 2.0 * width, axis[0])
      subtitle.append(fixed)
      tab += 1

  box = ax.get_position()
  ax.set_position([box.x0, box.y0 + box.height * 0.05,
                 box.width, box.height * 0.95])
  ax.legend(subtitle, loc='upper center', bbox_to_anchor=(0.5, 1.12), ncol=5)
  plt.ylabel("time (s)")
  plt.ylim(ymin=0)
  plt.xlabel(x_type)
  # plt.xlim(xmin=0)

  plt.show()

methods = ["seq", "mpi_1", "mpi_2", "mpi_4", "mpi_8"]
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

# make_plot({"region": regions, "method": ["seq"], "x": "method"})
make_plot({"region": regions, "method": ["mpi_1", "mpi_2", "mpi_4", "mpi_8"], "x": "region"})
make_plot({"region": regions, "method": methods, "x": "region"})




# make_plot({"region": regions, "method": "mpi_1", "x": "region"})
# make_plot({"region": regions, "method": ["seq", "mpi_1", "mpi_2", "mpi_4"], "x": "region"})
