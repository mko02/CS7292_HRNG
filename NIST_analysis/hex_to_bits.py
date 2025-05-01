import sys

if len(sys.argv) != 3:
    print("Usage: python3 hex_to_bits.py input_hex.txt output_bits.txt")
    sys.exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
    for line in infile:
        hex_str = line.strip()
        if hex_str:
            bits = bin(int(hex_str, 16))[2:].zfill(len(hex_str) * 4)
            outfile.write(bits + '\n')
