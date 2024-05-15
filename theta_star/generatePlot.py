import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 讀取output.txt文件
with open("output.txt", "r") as file:
    lines = file.readlines()

# 解析每行中的坐標
points = {}
for line in lines:
    parts = line.strip().split("(")
    if parts[0].startswith("Obstacle"):
        continue
    else:
        name = parts[0].strip()
        coords = parts[1].split(")")[0].split(",")
        x = float(coords[0].split("=")[1].strip())
        y = float(coords[1].split("=")[1].strip())
        z = float(coords[2].split("=")[1].strip())
        points[name] = (x, y, z)

# 提取每個點的坐標
x = [point[0] for point in points.values()]
y = [point[1] for point in points.values()]
z = [point[2] for point in points.values()]

# 定義障礙物的位置
obstacles = [
    ((10.87, -9.5, 4.27), (11.6, -9.45, 4.97)),
    ((10.25, -9.5, 4.97), (10.87, -9.45, 5.62)),
    ((10.87, -8.5, 4.97), (11.6, -8.45, 5.62)),
    ((10.25, -8.5, 4.27), (10.7, -8.45, 4.97)),
    ((10.87, -7.40, 4.27), (11.6, -7.35, 4.97)),
    ((10.25, -7.40, 4.97), (10.87, -7.35, 5.62))
]

# 建立3D圖形
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# 繪製障礙物
for obstacle in obstacles:
    x_vals = [obstacle[0][0], obstacle[1][0], obstacle[1][0], obstacle[0][0], obstacle[0][0]]
    y_vals = [obstacle[0][1], obstacle[0][1], obstacle[1][1], obstacle[1][1], obstacle[0][1]]
    z_vals = [obstacle[0][2], obstacle[0][2], obstacle[1][2], obstacle[1][2], obstacle[0][2]]
    ax.plot(x_vals, y_vals, z_vals, c='gray')

# 繪製點
ax.scatter(x, y, z, c='r', marker='o')

# 添加點的標籤
for name, point in points.items():
    ax.text(point[0], point[1], point[2], name)

# 連接點
for i in range(len(x)-1):
    ax.plot([x[i], x[i+1]], [y[i], y[i+1]], [z[i], z[i+1]], c='b')

# 設置坐標軸標籤
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

# 顯示圖形
plt.show()

