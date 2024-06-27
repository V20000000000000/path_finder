import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np
import matplotlib.animation as animation

# 创建一个新的 3D 图形
fig = plt.figure(figsize=(10, 10))
ax = fig.add_subplot(111, projection='3d')

class InputDataParser:
    def __init__(self):
        self.VertexCubes = []
        self.NeighborCubes = []

    def parseVertexCubes(self, fileName):
        with open(fileName, 'r') as f:
            lines = f.readlines()
            VCubes = []
            for line in lines:
                data = line.split()
                VCubes.append((int(data[0]), (float(data[1]), float(data[2]), float(data[3])), (float(data[4]), float(data[5]), float(data[6]))))
            return VCubes
        
    def parseNeighborCubes(self, fileName):
        with open(fileName, 'r') as f:
            lines = f.readlines()
            NCubes = []
            # 格式為 10777 12052 12068 12070 12086 13361 , 長度可能不同
            for line in lines:
                data = line.split()
                NCubes.append([int(d) for d in data])
            return NCubes

    def parse(self):
        self.VertexCubes = self.parseVertexCubes('VertexLoc.txt')
        self.NeighborCubes = self.parseNeighborCubes('Neighbor.txt')

class Updater:
    def __init__(self, ax):
        self.ax = ax
        self.lines = []
        self.cubes = []
        self.current_cube_index = 0  # 初始为 -1，表示没有立方体被添加

    def add_cube(self, min_pt, max_pt, color, transparency, edgecolor):
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

        poly3d = Poly3DCollection(faces, facecolors=color, linewidths=1, edgecolors=edgecolor, alpha=transparency)
        self.ax.add_collection3d(poly3d)
        self.cubes.append(poly3d)

        # 更新当前立方体的索引
        index = len(self.cubes)

        return index - 1

    def hide_cube(self, index):
        if 0 <= index < len(self.cubes):
            print("hide index:", index)
            self.cubes[index].set_verts([])
            self.cubes[index].set_facecolor((0, 0, 0, 0))

    def add_line(self, start, end, color='blue', linewidth=2):
        line, = self.ax.plot([start[0], end[0]], [start[1], end[1]], [start[2], end[2]], color=color, linewidth=linewidth)
        self.lines.append(line)

        # get the index of the line
        index = len(self.lines)

        return index - 1

    def hide_line(self, index):
        if 0 <= index < len(self.lines):
            self.lines[index].set_data([], []) 
            self.lines[index].set_3d_properties([])

    def update(self, frame):
        current_cube_index = 0
        # 隨時間繪製出parser.NeighborCubes 中的所有立方體
        for i in range(current_cube_index, len(parser.NeighborCubes)):
            if i == frame:
                for j in range(0, len(parser.NeighborCubes[i])):
                    # 繪製新的立方體, 如果已經畫過就跳過
                    if parser.NeighborCubes[i][j] < i:
                        continue

                    self.add_cube(parser.VertexCubes[parser.NeighborCubes[i][j]][1], parser.VertexCubes[parser.NeighborCubes[i][j]][2], 'm', 0.2, 'm')
                current_cube_index = i
                print("current_cube_index:", current_cube_index)
                break
        return self.cubes
                    
           


# 创建输入数据解析器
parser = InputDataParser()
parser.parse()

# 创建更新器
updater = Updater(ax)

# 绘制初始的KIZ方块
KIZS = [
    (10.3, -10.2, 4.32, 11.55, -6.0, 5.57)
]

for KIZ in KIZS:
    min_pt = KIZ[:3]
    max_pt = KIZ[3:]
    updater.add_cube(min_pt, max_pt, "yellow", 0.05, "yellow")

# 绘制初始的障碍物方块
obstacles = [
    (10.87, -9.5, 4.27, 11.6, -9.45, 4.97),
    (10.25, -9.5, 4.97, 10.87, -9.45, 5.62),
    (10.87, -8.5, 4.97, 11.6, -8.45, 5.62),
    (10.25, -8.5, 4.27, 10.7, -8.45, 4.97),
    (10.87, -7.4, 4.27, 11.6, -7.35, 4.97),
    (10.25, -7.4, 4.97, 10.87, -7.35, 5.62)
]

for obstacle in obstacles:
    min_pt = obstacle[:3]
    max_pt = obstacle[3:]
    updater.add_cube(min_pt, max_pt, "cyan", 0.2, "black")  # 将障碍物透明度设置为0.2

# 设置显示范围
ax.set_xlim(9, 12)
ax.set_ylim(-11, -6)
ax.set_zlim(4, 6)

# 设置坐标轴标签
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

print ("len(parser.NeighborCubes):", len(parser.NeighborCubes))

# 创建动画
ani = animation.FuncAnimation(fig, updater.update, frames=len(parser.NeighborCubes), interval=50, blit=True)

# 显示图形
plt.show()










