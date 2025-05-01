import sys

if len(sys.argv) != 3:
    print("Usage: python3 hex_to_bin.py input_hex.txt output.bin")
    sys.exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, 'r') as infile, open(output_file, 'wb') as outfile:
    hex_data = ''.join([line.strip() for line in infile if line.strip()])
    outfile.write(bytes.fromhex(hex_data))
