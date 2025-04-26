#ifndef TRNG_READER_H
#define TRNG_READER_H

#include <vector>
#include <string>
#include <cstdint>

// HRNG

class TRNGReader {
public:
    // read the TRNG seed file and add them to the blocks
    TRNGReader(const std::string& filename);

    // Returns the next `num_bytes` from blocks
    std::vector<uint8_t> getBytes(size_t num_bytes);

    // Latency to get TRNG bytes
    int latency() const {
        return 10;
    }

private:
    std::vector<uint8_t> blocks;
    static inline size_t index = 0;  // Shared static counter
};

#endif // TRNG_READER_H
