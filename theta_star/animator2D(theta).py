import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
import heapq

# Define the grid size and obstacles
GRID_SIZE = 20
OBSTACLES = [(8, 5), (8, 6), (8, 7), (8, 8), (8, 9), (8, 10), (8, 11), (8, 12), (8, 13), (8, 14)]
START = (0, 0)
GOAL = (19, 19)

# Heuristic function: Euclidean distance
def heuristic(a, b):
    return ((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2) ** 0.5

# Function to check if a line between two points is obstructed
def line_of_sight(start, end, obstacles):
    x0, y0 = start
    x1, y1 = end

    dx = x1 - x0
    dy = y1 - y0

    if dx == 0:  # Vertical line
        step = 1 if y1 > y0 else -1
        for y in range(y0, y1, step):
            for obstacle in obstacles:
                if obstacle[0] - 0.5 <= x0 <= obstacle[0] + 0.5 and obstacle[1] - 0.5 <= y <= obstacle[1] + 0.5:
                    return False
        return True

    if dy == 0:  # Horizontal line
        step = 1 if x1 > x0 else -1
        for x in range(x0, x1, step):
            for obstacle in obstacles:
                if obstacle[0] - 0.5 <= x <= obstacle[0] + 0.5 and obstacle[1] - 0.5 <= y0 <= obstacle[1] + 0.5:
                    return False
        return True

    # General case: Diagonal line
    m = dy / dx
    step = 0.1 if abs(dx) > abs(dy) else 0.1 / abs(m)
    for x in np.arange(x0, x1, step):
        y = m * (x - x0) + y0
        for obstacle in obstacles:
            if obstacle[0] - 0.5 <= x <= obstacle[0] + 0.5 and obstacle[1] - 0.5 <= y <= obstacle[1] + 0.5:
                return False
    return True


# Theta* search algorithm
def theta_star_search(start, goal, grid_size, obstacles):
    open_set = []
    heapq.heappush(open_set, (0, start))
    came_from = {start: None}
    g_score = {start: 0}
    f_score = {start: heuristic(start, goal)}

    open_set_hash = {start}
    search_steps = []  # To store the search steps for animation

    while open_set:
        current = heapq.heappop(open_set)[1]
        open_set_hash.remove(current)

        if current == goal:
            path = []
            while current is not None:
                path.append(current)
                current = came_from[current]
            path.reverse()
            return path, search_steps

        neighbors = [(current[0] + dx, current[1] + dy) for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]]
        neighbors = [(x, y) for x, y in neighbors if 0 <= x < grid_size and 0 <= y < grid_size and (x, y) not in obstacles]

        for neighbor in neighbors:
            if came_from[current] and line_of_sight(came_from[current], neighbor, obstacles):
                tentative_g_score = g_score[came_from[current]] + heuristic(came_from[current], neighbor)
                if tentative_g_score < g_score.get(neighbor, float('inf')):
                    came_from[neighbor] = came_from[current]
                    g_score[neighbor] = tentative_g_score
                    f_score[neighbor] = tentative_g_score + heuristic(neighbor, goal)
                    if neighbor not in open_set_hash:
                        heapq.heappush(open_set, (f_score[neighbor], neighbor))
                        open_set_hash.add(neighbor)
                        search_steps.append((came_from[current], neighbor, True))  # True for line of sight success
            else:
                tentative_g_score = g_score[current] + 1
                if tentative_g_score < g_score.get(neighbor, float('inf')):
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g_score
                    f_score[neighbor] = tentative_g_score + heuristic(neighbor, goal)
                    if neighbor not in open_set_hash:
                        heapq.heappush(open_set, (f_score[neighbor], neighbor))
                        open_set_hash.add(neighbor)
                        if came_from[current]:
                            search_steps.append((came_from[current], neighbor, False))  # False for line of sight failure

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
path, search_steps = theta_star_search(START, GOAL, GRID_SIZE, OBSTACLES)
path_set = set(path) if path else set()
search_index = 0
current_index = 0
search_completed = False

def update(frame):
    if frame == 5:
            ax.add_patch(plt.Rectangle(START, 1, 1, color='m', alpha=0.5))
            ax.text(START[0] + 0.5, START[1] + 0.5, 'Start', ha='center', va='center', color='black', fontsize=12)
        
    if frame == 7:
        ax.add_patch(plt.Rectangle(GOAL, 1, 1, color='m', alpha=0.5))
        ax.text(GOAL[0] + 0.5, GOAL[1] + 0.5, 'Goal', ha='center', va='center', color='black', fontsize=12)

    if (frame > 20):
        global search_index, current_index, search_completed
        if search_index < len(search_steps):
            prev, neighbor, success = search_steps[search_index]
            ax.add_patch(plt.Rectangle(neighbor, 1, 1, color='green', alpha=0.3))
            if success:
                ax.plot([prev[0] + 0.5, neighbor[0] + 0.5], [prev[1] + 0.5, neighbor[1] + 0.5], color='green', alpha=0.5)
            else:
                ax.plot([prev[0] + 0.5, neighbor[0] + 0.5], [prev[1] + 0.5, neighbor[1] + 0.5], color='red', alpha=0.5, linestyle='-')
            search_index += 1
        elif not search_completed:
            search_completed = True
            current_index = 0

        if search_completed and current_index < len(path):
            node = path[current_index]
            ax.add_patch(plt.Rectangle(node, 1, 1, color='blue', alpha=0.5))

            # Draw Theta* line segments
            if current_index > 0:
                current = path[current_index - 1]
                neighbor = path[current_index]
                ax.plot([current[0] + 0.5, neighbor[0] + 0.5], [current[1] + 0.5, neighbor[1] + 0.5], color='blue', alpha=0.8, linestyle='-', linewidth=5)
            
            # Draw red points at turns
            if current_index > 1:
                current = path[current_index - 2]
                neighbor = path[current_index - 1]
                next = path[current_index]
                if not (current[0] == neighbor[0] == next[0] or current[1] == neighbor[1] == next[1]):
                    ax.add_patch(plt.Circle((neighbor[0] + 0.5, neighbor[1] + 0.5), 0.1, color='red'))
            current_index += 1
        return []

# Create animation
ani = animation.FuncAnimation(fig, update, frames=range(len(search_steps) + len(path) + 25), interval=100, repeat=True)

# Set the title
plt.title('Theta* Search Algorithm')

# Show the plot
plt.show()



