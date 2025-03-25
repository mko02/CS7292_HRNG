import numpy as np
import cv2
from pynput import mouse
import time
import random

GRID_SIZE = 64
BLOCK_SIZE = 4
MIN_POINTS = 256
MAX_POINTS = 1024
FILENAME = "mouse_seed.txt"

def capture_mouse_movement():
    points = []

    def on_move(x, y):
        if len(points) >= MAX_POINTS:
            return False  # Stop listening
        points.append((x, y))

    print("Move your mouse randomly...")
    with mouse.Listener(on_move=on_move) as listener:
        listener.join()

    return points

def normalize_points(points):
    xs, ys = zip(*points)
    min_x, min_y = min(xs), min(ys)
    max_x, max_y = max(xs), max(ys)

    normalized = []
    for x, y in points:
        nx = int((x - min_x) / (max_x - min_x + 1e-9) * (GRID_SIZE - 1))
        ny = int((y - min_y) / (max_y - min_y + 1e-9) * (GRID_SIZE - 1))
        normalized.append((nx, ny))
    return normalized

def create_image(points):
    img = np.zeros((GRID_SIZE, GRID_SIZE), dtype=np.uint8)
    for x, y in points:
        img[y, x] = 1
    return img

def chaotic_permute(img, rounds=9):
    flat = img.flatten()
    size = flat.size
    seed = int(np.sum(flat)) % size
    x = 0.31
    for r in range(rounds):
        indices = []
        for _ in range(size):
            x = 4 * x * (1 - x)
            idx = int(x * size) % size
            indices.append(idx)
        flat = flat[indices]
    return flat.reshape(GRID_SIZE, GRID_SIZE)

def extract_256_bits(img):
    bits = []
    for y in range(0, GRID_SIZE, BLOCK_SIZE):
        for x in range(0, GRID_SIZE, BLOCK_SIZE):
            block = img[y:y+BLOCK_SIZE, x:x+BLOCK_SIZE]
            count = np.sum(block)
            bits.append(1 if count % 2 == 1 else 0)
    return bits[:256]

def main():
    print("Press Ctrl+C to stop...\n")

    try:
        while True:
            points = capture_mouse_movement()

            if len(points) < MIN_POINTS:
                print("Not enough points collected. Skipping...\n")
                continue

            norm_points = normalize_points(points)
            image = create_image(norm_points)
            encrypted_image = chaotic_permute(image, rounds=9)
            random_bits = extract_256_bits(encrypted_image)

            bit_str = ''.join(str(b) for b in random_bits)
            hex_str = f'{int(bit_str, 2):064x}'

            print(f"Random 256-bit number:\n{hex_str}\n")

            with open(FILENAME, "a") as file:
                file.write(hex_str + "\n")

    except KeyboardInterrupt:
        print("\nStopped by user. Random numbers saved to", FILENAME)

if __name__ == "__main__":
    main()
