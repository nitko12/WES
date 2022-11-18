from scipy.interpolate import RegularGridInterpolator
import matplotlib.pyplot as plt

from pprint import pprint
import numpy as np

arr = np.array([
    np.array([10, 13, 18, 20, 25, 25, 25, 25, 25, 25, 25]),
    np.array([10, 13, 18, 21, 28, 28, 28, 28, 28, 28, 28]),
    np.array([10, 13, 19, 22, 30, 30, 30, 30, 30, 30, 30]),
    np.array([10, 13, 19, 23, 32, 32, 32, 32, 32, 32, 32]),
    np.array([10, 13, 19, 24, 33, 33, 33, 33, 33, 33, 33]),
    np.array([10, 13, 20, 24, 35, 35, 35, 35, 35, 35, 35]),
    np.array([10, 14, 20, 25, 36, 36, 36, 36, 36, 36, 36]),
    np.array([10, 14, 21, 28, 38, 38, 38, 38, 38, 38, 38]),
    np.array([10, 15, 23, 30, 40, 40, 40, 40, 40, 40, 40]),
    np.array([10, 18, 25, 34, 38, 42, 42, 42, 42, 42, 42]),
])

x_index = [1000, 1400, 1800, 2500, 3000, 4000, 6000, 8000, 10000, 12000, 14000]
y_index = [15, 25, 30, 40, 50, 60, 70, 80, 90, 100]


def find_lb(array, value):
    for i in range(len(array)):
        if array[i] > value:
            return i - 1
    return len(array) - 1

# interpolate arr


interp = RegularGridInterpolator(
    (np.arange(10), np.arange(11)),
    arr, method='linear')

x = 1200
y = 35

x_lb = find_lb(x_index, x)
x_ub = x_lb + 1

y_lb = find_lb(y_index, y)
y_ub = y_lb + 1

print(x_lb, x_ub, y_lb, y_ub)

x_interp = (x - x_index[x_lb]) / (x_index[x_ub] - x_index[x_lb])
y_interp = (y - y_index[y_lb]) / (y_index[y_ub] - y_index[y_lb])

print(x_interp, y_interp)

print(interp([x_interp, y_interp]))


fig = plt.figure(figsize=(12, 12))
ax = fig.add_subplot(projection='3d')

for x in np.linspace(1000, 14000, 10):
    for y in np.linspace(15, 100, 10):
        x_lb = find_lb(x_index, x)
        x_ub = min(x_lb + 1, len(x_index) - 1)

        y_lb = find_lb(y_index, y)
        y_ub = min(y_lb + 1, len(y_index) - 1)

        print(x_lb, x_ub, y_lb, y_ub)

        x_interp = (x - x_index[x_lb]) / (x_index[x_ub] - x_index[x_lb])
        y_interp = (y - y_index[y_lb]) / (y_index[y_ub] - y_index[y_lb])

        print(x_lb, x_ub, y_lb, y_ub)

        ax.scatter(x, y, interp([x_interp, y_interp]))


plt.show()
