import re

# Sample log data
log_data = """
07-13 15:02:20.791 I/PathfindingMain( 1701): x: 11.230000000000013 y: -6.719999999999974 z: 4.899999999999999
07-13 15:02:20.791 I/PathfindingMain( 1701): x: 11.280000000000014 y: -7.4199999999999715 z: 5.099999999999998
07-13 15:02:20.791 I/PathfindingMain( 1701): x: 11.330000000000014 y: -8.519999999999976 z: 4.849999999999999
07-13 15:02:20.791 I/PathfindingMain( 1701): x: 11.380000000000015 y: -9.819999999999995 z: 5.299999999999997
"""


# Function to parse log data and extract coordinates
def extract_coordinates(log):
    pattern = r"x:\s([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)\s+y:\s([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)\s+z:\s([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)"
    matches = re.findall(pattern, log)
    coordinates = [(float(x), float(y), float(z)) for x, y, z in matches]
    return coordinates


# Function to create line segments from coordinates
def create_line_segments(coords):
    segments = []
    for i in range(len(coords) - 1):
        segments.append((coords[i], coords[i + 1]))
    return segments


# Main script
coordinates = extract_coordinates(log_data)
line_segments = create_line_segments(coordinates)

# Print line segments in x1, y1, z1, x2, y2, z2 format
for segment in line_segments:
    print(
        f"{segment[0][0]},{segment[0][1]},{segment[0][2]},{segment[1][0]},{segment[1][1]},{segment[1][2]}"
    )