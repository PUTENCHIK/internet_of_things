import numpy as np
import pathlib
import matplotlib.pyplot as plt


path = pathlib.Path(".")
data = np.loadtxt(str(path / "range_calibration" / "data3.txt"))
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
