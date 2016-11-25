import matplotlib.pyplot as plt
import numpy as np

N = 8  # range for x, y
M = 8  # range for u, v
S = 4  # pixel size (for visualization)
A = N * M * S

g = np.zeros([A, A])
h = np.zeros([A, A])
dct = np.zeros([A, A])


def alpha(w):
    if w == 0:
        return np.sqrt(1 / N)
    return np.sqrt(2 / N)

for i in range(A):
    for j in range(A):
        x = int(i / M) % N
        y = int(j / M) % N
        u = int(i / M / S)
        v = int(j / M / S)
        g[i, j] = np.cos(2 * np.pi * (u * x + v * y) / N)
        h[i, j] = -np.sin(2 * np.pi * (u * x + v * y) / N)
        dct[i, j] = alpha(u) * alpha(v) * np.cos((2 * x + 1) * u * np.pi / 2 / N) * np.cos((2 * y + 1) * v * np.pi / 2 / N)


def draw(a, pos, label=''):
    ticks = np.arange(0, A, 1)
    plt.subplot(pos)
    plt.title(label)
    plt.grid(True)
    plt.xticks(ticks)
    plt.yticks(ticks)
    plt.imshow(a, extent=[0, M, M, 0])


plt.gray()
draw(g, 221, 'DFT Real')
draw(h, 222, 'DFT Imaginary')
draw(dct, 223, 'DCT')
plt.show()
