// https://xilinx.github.io/Vitis_Libraries/security/2019.2/guide_L1/internals/chacha20.html
// https://loup-vaillant.fr/tutorials/chacha20-design

#include "ChaCha20.h"
#include <cstring>

// HRNG

ChaCha20::ChaCha20(const std::vector<uint8_t>& keyInput, const std::vector<uint8_t>& nonceInput, uint32_t counter)
{
    // check for input to ensure size is valid
    if (keyInput.size() != 32)
        throw std::invalid_argument("ChaCha20 key must be 32 bytes");
    if (nonceInput.size() != 12)
        throw std::invalid_argument("ChaCha20 nonce must be 12 bytes");

    this->counter = counter;
    std::copy(keyInput.begin(), keyInput.end(), key.begin());
    std::copy(nonceInput.begin(), nonceInput.end(), nonce.begin());
}

// In the encryption, a new 512-bit key is generated and is used for doing XOR with 512-bit plain text, then output a cipher block in each iteration
void ChaCha20::encrypt(uint8_t* plaintext, size_t length) {
    size_t offset = 0;
    while (offset < length) {

        // generate a new keystream block and updating counter after
        initState(counter++);
        generateBlock();

        size_t blockSize = std::min(length - offset, size_t(64));
        for (size_t i = 0; i < blockSize; ++i) {
            plaintext[offset + i] ^= keystream[i];
        }

        offset += blockSize;
    }
}

// Rotate left function
uint32_t ChaCha20::rotl(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

// quarter round scrambling
void ChaCha20::quarterRound(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
    a += b; d ^= a; d = rotl(d, 16);
    c += d; b ^= c; b = rotl(b, 12);
    a += b; d ^= a; d = rotl(d, 8);
    c += d; b ^= c; b = rotl(b, 7);
}

// initialize the state using the current counter, key, and nonce
// build up the block of 512 bits
// 4 * 4 bytes for the constants, 8 * 4 bytes for the key, 1 * 4 bytes for the counter, and 3 * 4 bytes for the nonce 
void ChaCha20::initState(uint32_t counter) {
    const char* constants = "expand 32-byte k";
    for (int i = 0; i < 4; ++i) {
        state[i] = ((uint32_t)constants[4 * i]) |
                   ((uint32_t)constants[4 * i + 1] << 8) |
                   ((uint32_t)constants[4 * i + 2] << 16) |
                   ((uint32_t)constants[4 * i + 3] << 24);
    }

    for (int i = 0; i < 8; ++i) {
        state[4 + i] = ((uint32_t)key[4 * i]) |
                       ((uint32_t)key[4 * i + 1] << 8) |
                       ((uint32_t)key[4 * i + 2] << 16) |
                       ((uint32_t)key[4 * i + 3] << 24);
    }

    state[12] = counter;

    for (int i = 0; i < 3; ++i) {
        state[13 + i] = ((uint32_t)nonce[4 * i]) |
                        ((uint32_t)nonce[4 * i + 1] << 8) |
                        ((uint32_t)nonce[4 * i + 2] << 16) |
                        ((uint32_t)nonce[4 * i + 3] << 24);
    }
}

// generate keystream by performing the quarter round operation

void ChaCha20::generateBlock() {

    // Copy the state to a temporary array
    uint32_t temp[16];
    std::memcpy(temp, state, sizeof(state));

    // 20 rounds of quarter rounds scrambling, the 20 in the name ChaCha20
    // 10 column + 10 diagonal rounds
    for (int i = 0; i < 10; ++i) {
        quarterRound(temp[0], temp[4], temp[8], temp[12]);
        quarterRound(temp[1], temp[5], temp[9], temp[13]);
        quarterRound(temp[2], temp[6], temp[10], temp[14]);
        quarterRound(temp[3], temp[7], temp[11], temp[15]);

        quarterRound(temp[0], temp[5], temp[10], temp[15]);
        quarterRound(temp[1], temp[6], temp[11], temp[12]);
        quarterRound(temp[2], temp[7], temp[8], temp[13]);
        quarterRound(temp[3], temp[4], temp[9], temp[14]);
    }

    // Add the unscrambled block to the scrambled block (https://loup-vaillant.fr/tutorials/chacha20-design)
    for (int i = 0; i < 16; ++i)
        temp[i] += state[i];

    // get keystream 
    for (int i = 0; i < 16; ++i) {
        keystream[4 * i + 0] = temp[i] & 0xff;
        keystream[4 * i + 1] = (temp[i] >> 8) & 0xff;
        keystream[4 * i + 2] = (temp[i] >> 16) & 0xff;
        keystream[4 * i + 3] = (temp[i] >> 24) & 0xff;
    }
}
