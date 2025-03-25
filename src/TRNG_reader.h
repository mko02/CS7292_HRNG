#ifndef RAMULATOR_TRNG_READER_H
#include <vector>

std::vector<uint64_t> read_TRNG_seed(const std::string& filename);

#endif //RAMULATOR_TRNG_READER_H