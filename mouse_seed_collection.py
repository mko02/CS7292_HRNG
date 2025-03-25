import numpy as np
from pynput import mouse

# Citation: A True Random Number Generator Based on Mouse Movement and Chaotic Cryptography

GRID_SIZE = 64
BLOCK_SIZE = 4
MIN_POINTS = 256
MAX_POINTS = 1024
FILENAME = "mouse_seed.txt"

# Function to capture mouse movement
def get_mouse_movement():
    points = []

    def on_move(x, y):
        if len(points) >= MAX_POINTS:
            return False  # Stop listening
        points.append((x, y))

    with mouse.Listener(on_move=on_move) as mouse_listener:
        mouse_listener.join()

    return points

# Normalize the points to a 64x64 grid
def normalize_points(points):
    x_coord, y_coord = zip(*points)
    min_x, min_y = min(x_coord), min(y_coord)
    max_x, max_y = max(x_coord), max(y_coord)

    normalized_coordinates = []
    for x, y in points:
        nx = int((x - min_x) / (max_x - min_x + 1e-9) * (GRID_SIZE - 1))
        ny = int((y - min_y) / (max_y - min_y + 1e-9) * (GRID_SIZE - 1))
        normalized_coordinates.append((nx, ny))
    return normalized_coordinates

def create_mouse_movement_image(points):
    img = np.zeros((GRID_SIZE, GRID_SIZE), dtype=np.uint8)
    for x, y in points:
        img[y, x] = 1
    return img

# Following the chaotic permutation algorithm from the paper
def chaotic_permute(img, rounds=9):
    flat = img.flatten()
    size = flat.size
    x = 0.31  # initial value stated in the paper

    for _ in range(rounds):
        indices = []
        for _ in range(size):
            x = 4 * x * (1 - x)
            idx = int(x * size) % size
            indices.append(idx)
        flat = flat[indices]
    
    return flat.reshape(GRID_SIZE, GRID_SIZE)

'''
Extraction Algorithm:
1. Divide the image into 256 blocks of 4x4 pixels
2. Count the number of points with black colour inside a block. If the count is odd, the block is assigned a value of 1, otherwise the block is assigned 0.
3. Scan those 256 blocks from top to down and from left to right, concatenating the value assigned to each block to compose the 256-bit random number
'''
def extract_256_bits(img):
    bits = []
    for y in range(0, GRID_SIZE, BLOCK_SIZE):
        for x in range(0, GRID_SIZE, BLOCK_SIZE):
            block = img[y:y+BLOCK_SIZE, x:x+BLOCK_SIZE]
            count = np.sum(block)
            bits.append(1 if count % 2 == 1 else 0)

    return bits[:256] # ensures 256 bits
   

def main():

    try:
        while True:
            points = get_mouse_movement()

            if len(points) < MIN_POINTS:
                print("Not enough points collected.\n")
                continue

            norm_points = normalize_points(points)
            image = create_mouse_movement_image(norm_points)
            encrypted_image = chaotic_permute(image, rounds=9)
            random_bits = extract_256_bits(encrypted_image)

            bit_str = ''.join(str(b) for b in random_bits)
            bit_number = int(bit_str, 2)
            hex_str = hex(bit_number)

            print(f"Random 256-bit number:\n{hex_str}\n")

            with open(FILENAME, "a") as file:
                file.write(hex_str + "\n")

    except KeyboardInterrupt:
        print("\nRandom numbers saved to", FILENAME)

if __name__ == "__main__":
    main()
