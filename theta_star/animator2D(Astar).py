import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
import heapq

# Define the grid size and obstacles
GRID_SIZE = 20
OBSTACLES = [(5, 5), (5, 6), (5, 7), (5, 8), (5, 9), (5, 10), (5, 11), (5, 12)]
START = (0, 0)
GOAL = (19, 19)

# Heuristic function: Euclidean distance
def heuristic(a, b):
    return ((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2) ** 0.5

# A* search algorithm
def astar_search(start, goal, grid_size, obstacles):
    open_set = []
    heapq.heappush(open_set, (0, start))
    came_from = {}
    g_score = {start: 0}
    f_score = {start: heuristic(start, goal)}

    open_set_hash = {start}
    search_steps = []  # To store the search steps for animation

    while open_set:
        current = heapq.heappop(open_set)[1]
        open_set_hash.remove(current)

        if current == goal:
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(start)
            path.reverse()
            return path, search_steps

        neighbors = [(current[0] + dx, current[1] + dy) for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]]
        neighbors = [(x, y) for x, y in neighbors if 0 <= x < grid_size and 0 <= y < grid_size and (x, y) not in obstacles]

        for neighbor in neighbors:
            tentative_g_score = g_score[current] + 1
            if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g_score
                f_score[neighbor] = tentative_g_score + heuristic(neighbor, goal)
                if neighbor not in open_set_hash:
                    heapq.heappush(open_set, (f_score[neighbor], neighbor))
                    open_set_hash.add(neighbor)
                    search_steps.append((current, neighbor))  # Add current and neighbor to search steps

    return None, search_steps

# Create a new 2D figure
fig, ax = plt.subplots(figsize=(10, 10))

# Create the grid
grid = np.zeros((GRID_SIZE, GRID_SIZE))

# Mark obstacles
for obs in OBSTACLES:
    grid[obs] = 1

# Plot the grid
ax.set_xlim(0, GRID_SIZE)
ax.set_ylim(0, GRID_SIZE)
ax.set_xticks(np.arange(0, GRID_SIZE+1, 1))
ax.set_yticks(np.arange(0, GRID_SIZE+1, 1))
ax.grid(True)

# Plot obstacles
for obs in OBSTACLES:
    ax.add_patch(plt.Rectangle(obs, 1, 1, color='black'))

# Initialize variables for animation
path, search_steps = astar_search(START, GOAL, GRID_SIZE, OBSTACLES)
path_set = set(path) if path else set()
search_index = 0
current_index = 0

def update(frame):
    global search_index, current_index
    if search_index < len(search_steps):
        current, neighbor = search_steps[search_index]
        ax.add_patch(plt.Rectangle(neighbor, 1, 1, color='green', alpha=0.3))
        ax.plot([current[0] + 0.5, neighbor[0] + 0.5], [current[1] + 0.5, neighbor[1] + 0.5], color='green', alpha=0.3)
        search_index += 1
    if current_index < len(path):
        node = path[current_index]
        ax.add_patch(plt.Rectangle(node, 1, 1, color='blue', alpha=0.5))

        # 畫A*線段
        if(current_index > 0):
            current = path[current_index - 1]
            neighbor = path[current_index]
            ax.plot([current[0] + 0.5, neighbor[0] + 0.5], [current[1] + 0.5, neighbor[1] + 0.5], color='blue', alpha=0.5)
        
        # 在A* 線段轉折的地方加上紅色點
        if(current_index > 1):
            current = path[current_index - 2]
            neighbor = path[current_index - 1]
            next = path[current_index]
            if(current[0] == neighbor[0] and neighbor[0] == next[0] or current[1] == neighbor[1] and neighbor[1] == next[1]):
                pass
            else:
                ax.add_patch(plt.Circle((neighbor[0] + 0.5, neighbor[1] + 0.5), 0.1, color='red'))
        current_index += 1
    return []

# Create animation
ani = animation.FuncAnimation(fig, update, frames=range(max(len(search_steps), len(path))), interval=200, repeat=False)

# title
plt.title("A* Algorithm")

# Show the plot
plt.show()
