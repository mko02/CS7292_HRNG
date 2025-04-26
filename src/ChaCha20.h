#ifndef CHACHA20_H
#define CHACHA20_H

#include <vector>
#include <array>
#include <cstdint>
#include <stdexcept>

// HRNG

class ChaCha20 {
public:
    ChaCha20(const std::vector<uint8_t>& key, const std::vector<uint8_t>& nonce, uint32_t counter = 0);
    void encrypt(uint8_t* data, size_t length);

    // latency to generate random number
    int latency() const {
        return 5;
    }

private:
    // 256-bit key, a 32-bit counter, a 96-bit nonce
    std::array<uint8_t, 32> key;  // 32 * 8 = 256
    std::array<uint8_t, 12> nonce;  // 12 * 8 = 96
    uint32_t counter;
    uint32_t state[16];
    uint8_t keystream[64];

    static uint32_t rotl(uint32_t x, int n);
    static void quarterRound(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d);

    void initState(uint32_t counter);
    void generateBlock();
};

#endif // CHACHA20_H
