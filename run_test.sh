#!/bin/bash

CONFIG_FILE="configs/TLDRAM-config.cfg"
MODE="--mode=dram"
STATS_DIR="stats"

traces=("gcc" "leela" "linpack" "mcf")

for trace in "${traces[@]}"; do
    TRACE_PATH="6290/reversed/${trace}_reversed.trace"
    STATS_FILE="${STATS_DIR}/${trace}_hrng_off.stats"

    # remove "CSPRNG.txt", we only want one copy
    if [ -f CSPRNG.txt ]; then
        echo "Removing CSPRNG.txt..."
        rm CSPRNG.txt
    fi
    
    echo "Running ${trace} trace..."
    ./ramulator "$CONFIG_FILE" $MODE --stats "$STATS_FILE" "$TRACE_PATH"
done