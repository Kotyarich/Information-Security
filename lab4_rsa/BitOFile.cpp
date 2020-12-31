#include "BitOFile.h"

BitOFile::BitOFile(const std::string &name) {
    file.open(name, std::fstream::binary);
    file.put(0);
}

void BitOFile::write_bit(bool bit) {
    write_bit_array.push_back(bit);
    if (write_bit_array.size() >= 8) {
        flush();
    }
}

void BitOFile::flush() {
    unsigned char byte = 0;

    for (auto i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= write_bit_array[i];
    }
    write_bit_array.erase(write_bit_array.begin(), write_bit_array.begin() + 8);

    file.put(byte);
}

void BitOFile::write_byte(unsigned char byte) {
    if (write_bit_array.empty()) {
        file.put(byte);
    } else {
        for (auto i = 0; i < 8; i++) {
            bool bit = static_cast<bool>(byte & 1);
            write_bit_array.push_back(bit);
            byte >>= 1;
        }
        flush();
    }
}

void BitOFile::close() {
    if (!write_bit_array.empty()) {
        auto extra_bits = static_cast<unsigned char>(8 - write_bit_array.size());
        for (auto i = 0; i < extra_bits; i++) {
            write_bit_array.push_back(false);
        }
        flush();

        file.seekp(0);
        file.put(extra_bits);
    }

    file.close();
}

void BitOFile::write_number(size_t number, unsigned char byte) {
    std::vector<bool> bits;
    while (number > 0) {
        bool bit = static_cast<bool>(number & 1);
        bits.push_back(bit);
        number >>= 1;
    }

    auto zeroes = byte * 8 - bits.size();
    for (auto i = 0; i < zeroes; i++) {
        write_bit(false);
    }

    for (int i = static_cast<int>(bits.size() - 1); i >= 0; i--) {
        write_bit(bits[i]);
    }
}

bool BitOFile::eof() {
    return file.eof();
}
