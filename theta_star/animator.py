import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

# 创建一个新的 3D 图形
fig = plt.figure(figsize=(10, 10))
ax = fig.add_subplot(111, projection='3d')

# 绘制障碍物方块
obstacles = [
    (10.87, -9.5, 4.27, 11.6, -9.45, 4.97),
    (10.25, -9.5, 4.97, 10.87, -9.45, 5.62),
    (10.87, -8.5, 4.97, 11.6, -8.45, 5.62),
    (10.25, -8.5, 4.27, 10.7, -8.45, 4.97),
    (10.87, -7.4, 4.27, 11.6, -7.35, 4.97),
    (10.25, -7.4, 4.97, 10.87, -7.35, 5.62)
]

# 定义函数绘制立方体
def draw_cube(ax, min_pt, max_pt, color, transparency, edgecolor):
    v = np.array([[min_pt[0], min_pt[1], min_pt[2]],
                  [max_pt[0], min_pt[1], min_pt[2]],
                  [max_pt[0], max_pt[1], min_pt[2]],
                  [min_pt[0], max_pt[1], min_pt[2]],
                  [min_pt[0], min_pt[1], max_pt[2]],
                  [max_pt[0], min_pt[1], max_pt[2]],
                  [max_pt[0], max_pt[1], max_pt[2]],
                  [min_pt[0], max_pt[1], max_pt[2]]])

    faces = [[v[0], v[1], v[5], v[4]],
             [v[7], v[6], v[2], v[3]],
             [v[0], v[3], v[7], v[4]],
             [v[1], v[2], v[6], v[5]],
             [v[0], v[1], v[2], v[3]],
             [v[4], v[5], v[6], v[7]]]

    ax.add_collection3d(Poly3DCollection(faces, facecolors=color, linewidths=1, edgecolors=edgecolor, alpha=transparency))

for obstacle in obstacles:
    min_pt = obstacle[:3]
    max_pt = obstacle[3:]
    draw_cube(ax, min_pt, max_pt, "cyan", 0.5, "black")

# 绘制KIZ方块
KIZS = [
    (10.3, -10.2, 4.32, 11.55, -6.0, 5.57)
]

for KIZ in KIZS:
    min_pt = KIZ[:3]
    max_pt = KIZ[3:]
    draw_cube(ax, min_pt, max_pt, "yellow", 0.05, "yellow")

# 绘制直线段
line_start = np.array([10.5, -10, 5.12])
line_end = np.array([11.1, -9.7, 5.22])

ax.plot([line_start[0], line_end[0]], [line_start[1], line_end[1]], [line_start[2], line_end[2]], color='blue', linewidth=2)

# 设置显示范围
ax.set_xlim(9, 12)
ax.set_ylim(-11, -6)
ax.set_zlim(4, 6)

# 设置坐标轴标签
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# 显示图形
plt.show()





