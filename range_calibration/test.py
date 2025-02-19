import matplotlib.pyplot as plt
import numpy as np


data = np.loadtxt("data0.txt", dtype="uint16")
print(data.shape)

z = 3
coeff = np.polyfit(data[:, 0], data[:, 1], z)

x = np.linspace(data[:, 0].min(), data[:, 0].max(), data.shape[0])
y = np.zeros_like(x)
for i, c in enumerate(coeff):
    y += c * x**(z-i)

print(f"Shapes: {x.shape} {y.shape}")
plt.plot(x, y, c='r')
plt.scatter(data[:, 0], data[:, 1], s=3, c='b')
plt.show()
