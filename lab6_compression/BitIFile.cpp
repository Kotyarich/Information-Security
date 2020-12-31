#include "BitIFile.h"

BitIFile::BitIFile(const std::string &name) {
    file.open(name, std::fstream::binary);
    extra_bits = read_byte();
}

void BitIFile::close() {
    file.close();
}

unsigned char BitIFile::read_byte() {
    if (file.eof()) {
        throw std::logic_error("eof");
    }
    return static_cast<unsigned char>(file.get());
}

size_t BitIFile::read_number(unsigned char byte_n) {
    size_t number = 0;
    for (auto i = 0; i < byte_n; i++) {
        number <<= 8;
        auto byte = read_byte();
        number |= byte;
    }
    return number;
}

bool BitIFile::read_bit() {
    if (read_bit_array.empty()) {
        auto byte = read_byte();
        // check the end of the file
        file.get();

        int end_bit_n = 0;
        if (file.eof()) {
            end_bit_n += extra_bits;
        } else {
            file.unget();
        }

        auto result_bit = static_cast<bool>((byte >> 7) & 1);
        for (auto i = 6; i >= end_bit_n; i--) {
            auto bit = static_cast<bool>((byte >> i) & 1);
            read_bit_array.push_back(bit);
        }

        return result_bit;
    }

    bool bit = read_bit_array[0];
    read_bit_array.erase(read_bit_array.begin());

    return bit;
}
