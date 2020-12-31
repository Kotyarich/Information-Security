#include "aes.h"

void AES::key_expansion() {
    auto i = 0;

    while (i < Nk) {
        round_keys[i] = {key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]};
        i++;
    }

    while (i < Nb * (Nr + 1)) {
        word temp = round_keys[i - 1];
        if (i % Nk == 0) {
            auto shifted = shift_row(temp, 1);
            auto subbed = sub_word(shifted);
            temp = subbed ^ r_con[i / 4];
        } else if (Nk > 6 and i % Nk == 4) {
            temp = sub_word(temp);
        }

        round_keys[i] = round_keys[i - Nk] ^ temp;
        i++;
    }
}

word AES::sub_word(const word &w) {
    word res;
    for (auto i = 0; i < 4; i++) {
        auto row = w[i] / 0x10;
        auto col = w[i] % 0x10;
        res[i] = s_box[16 * row + col];
    }

    return res;
}

word AES::shift_row(const word &w, int shift) {
    word res = w;
    for (auto i = 0; i < shift; i++) {
        res = {res[1], res[2], res[3], res[0]};
    }

    return res;
}

std::array<unsigned char, 16> AES::encode_block(std::array<unsigned char, 16> block) {
    std::array<unsigned char, 16> state = block;

    add_round_key(state, 0);

    for (auto i = 1; i < Nr; i++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, i);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, Nr);

    return state;
}

void AES::add_round_key(std::array<unsigned char, 16> &state, int round_key_number) {
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            state[4 * i + j] ^= round_keys[round_key_number * 4 + i][j];
        }
    }
}

void AES::sub_bytes(std::array<unsigned char, 16> &state) {
    for (auto i = 0; i < 16; i++) {
        state[i] = s_box[state[i]];
    }
}

void AES::shift_rows(std::array<unsigned char, 16> &state) {
    for (auto i = 0ul; i < 4; i++) {
        word tmp = {state[i], state[4 + i], state[8 + i], state[12 + i]};
        tmp = shift_row(tmp, static_cast<int>(i));
        for (auto j = 0ul; j < 4; j++) {
            state[4 * j + i] = tmp[j];
        }
    }
}

void AES::mix_columns(std::array<unsigned char, 16> &state) {
    for (auto i = 0ul; i < 4; i++) {
        word col;
        for (auto j = 0ul; j < 4; j++) {
            col[j] = {state[j + 4 * i]};
        }

        word a = col;

        col[0] = g_mul(a[0], 2) ^ g_mul(a[3], 1) ^ g_mul(a[2], 1) ^ g_mul(a[1], 3);
        col[1] = g_mul(a[1], 2) ^ g_mul(a[0], 1) ^ g_mul(a[3], 1) ^ g_mul(a[2], 3);
        col[2] = g_mul(a[2], 2) ^ g_mul(a[1], 1) ^ g_mul(a[0], 1) ^ g_mul(a[3], 3);
        col[3] = g_mul(a[3], 2) ^ g_mul(a[2], 1) ^ g_mul(a[1], 1) ^ g_mul(a[0], 3);

        for (auto j = 0ul; j < 4; j++) {
            state[j + 4 * i] = col[j];
        }
    }
}

std::array<unsigned char, 16> AES::decode_block(std::array<unsigned char, 16> block) {
    std::array<unsigned char, 16> state = block;

    add_round_key(state, Nr);

    for (auto i = Nr - 1; i > 0; i--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, i);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, 0);

    return state;
}

void AES::inv_shift_rows(std::array<unsigned char, 16> &state) {
    for (auto i = 0ul; i < 4; i++) {
        word tmp = {state[i], state[4 + i], state[8 + i], state[12 + i]};
        tmp = inv_shift_row(tmp, static_cast<int>(i));
        for (auto j = 0ul; j < 4; j++) {
            state[4 * j + i] = tmp[j];
        }
    }
}

word AES::inv_shift_row(const word &w, int shift) {
    word res = w;
    for (auto i = 0; i < shift; i++) {
        res = {res[3], res[0], res[1], res[2]};
    }

    return res;
}

void AES::inv_sub_bytes(std::array<unsigned char, 16> &state) {
    for (auto i = 0; i < 16; i++) {
        auto row = state[i] / 0x10;
        auto col = state[i] % 0x10;
        state[i] = inv_s_box[16 * row + col];
    }
}

void AES::inv_mix_columns(std::array<unsigned char, 16> &state) {
    for (auto i = 0ul; i < 4; i++) {
        word col;
        for (auto j = 0ul; j < 4; j++) {
            col[j] = {state[j + 4 * i]};
        }

        state[4 * i] = g_mul(col[0], 14) ^ g_mul(col[3], 9) ^ g_mul(col[2], 13) ^ g_mul(col[1], 11);
        state[4 * i + 1] = g_mul(col[1], 14) ^ g_mul(col[0], 9) ^ g_mul(col[3], 13) ^ g_mul(col[2], 11);
        state[4 * i + 2] = g_mul(col[2], 14) ^ g_mul(col[1], 9) ^ g_mul(col[0], 13) ^ g_mul(col[3], 11);
        state[4 * i + 3] = g_mul(col[3], 14) ^ g_mul(col[2], 9) ^ g_mul(col[1], 13) ^ g_mul(col[0], 11);
    }
}

unsigned char AES::g_mul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for (counter = 0; counter < 8; counter++) {
        if ((b & 1) == 1) {
            p ^= a;
        }
        hi_bit_set = static_cast<unsigned char>(a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

AES::AES(std::string key) {
    for (auto i = 0u; i < 4 * Nk; i++) {
        this->key[i] = static_cast<unsigned char>(key[i]);
    }

    key_expansion();
}

std::string AES::encode(std::string data) {
    std::string encoded;
    unsigned char zeroes = 0;

    auto i = 0ul;
    while (i < data.length()) {
        std::array<unsigned char, 16> block{};

        unsigned char j = 0;
        while (j < 16 && i + j < data.length()) {
            block[j] = static_cast<unsigned char>(data[i + j]);
            j++;
        }
        i += 16;

        if (j < 16) {
            zeroes = static_cast<unsigned char>(16 - j);
            for (; j < 16; j++) {
                block[j] = 0;
            }
        }

        auto encoded_block = encode_block(block);
        for (auto k = 0; k < 16; k++) {
            encoded += encoded_block[k];
        }

    }

    std::array<unsigned char, 16> zeroes_block{};
    for (auto j = 0; j < 15; j++) {
        zeroes_block[j] = 0;
    }
    zeroes_block[15] = zeroes;
    auto encoded_block = encode_block(zeroes_block);
    for (auto k = 0; k < 16; k++) {
        encoded += encoded_block[k];
    }

    return encoded;
}

std::string AES::decode(std::string data) {
    std::string decoded;

    auto i = 0ul;
    while (i < data.length()) {
        std::array<unsigned char, 16> block{};

        unsigned char j = 0;
        while (j < 16 && i + j < data.length()) {
            block[j] = static_cast<unsigned char>(data[i + j]);
            j++;
        }
        i += 16;

        auto decoded_block = decode_block(block);
        for (auto k = 0; k < 16; k++) {
            decoded += (char) decoded_block[k];
        }
    }

    auto zeroes = decoded[decoded.length() - 1] + 16;
    if (zeroes > 0) {
        decoded = decoded.substr(0, decoded.size() - zeroes);
    }

    return decoded;
}

word operator^(const word &a, const word &b) {
    unsigned char a0 = a[0] ^ b[0];
    unsigned char a1 = a[1] ^ b[1];
    unsigned char a2 = a[2] ^ b[2];
    unsigned char a3 = a[3] ^ b[3];

    return {a0, a1, a2, a3};
}