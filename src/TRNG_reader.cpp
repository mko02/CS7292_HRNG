#include "TRNG_reader.h"
#include <fstream>
#include <vector>

// function that takes in a file name and output a list of seeds
std::vector<uint64_t> read_TRNG_seed(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<uint64_t> seeds;
    std::string line;

    while (std::getline(file, line)) {
        seeds.push_back(std::stoull(line));
    }

    return seeds;
}