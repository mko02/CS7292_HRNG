#include "TRNGReader.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

// HRNG

TRNGReader::TRNGReader(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open TRNG seed file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // one byte is two hex digits
        for (size_t i = 0; i + 1 < line.size(); i += 2) {
            std::string hexString = line.substr(i, 2);

            // Convert hex string to byte
            uint8_t byte = static_cast<uint8_t>(std::stoul(hexString, nullptr, 16));

            // add to block of bytes
            blocks.push_back(byte);
        }
    }
}

// Returns next `num_bytes` of blocks
std::vector<uint8_t> TRNGReader::getBytes(size_t num_bytes) {
    if (index + num_bytes > blocks.size()) {
        throw std::runtime_error("TRNG blocks exhausted");
    }

    // extract num_bytes starting from index
    std::vector<uint8_t> result(blocks.begin() + index, blocks.begin() + index + num_bytes);

    // update index
    index += num_bytes;
    return result;
}
