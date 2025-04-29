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
    // assuming that we always have a TRNG value ready
    // we can simply load the value, which would take 1 cycle
    // This is similar to the RDRAND instruction in Intel, where if the value is ready, it takes one read cycle
    // https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
    int latency() const {
        return 1;
    }

private:
    std::vector<uint8_t> blocks;
    static inline size_t index = 0;  // Shared static counter
};

#endif // TRNG_READER_H
