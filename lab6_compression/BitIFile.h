#ifndef LAB6_COMPRESSION_BITIFILE_H
#define LAB6_COMPRESSION_BITIFILE_H

#include <string>
#include <vector>
#include <fstream>

class BitIFile {
public:
    explicit BitIFile(const std::string &name);

    bool read_bit();
    unsigned char read_byte();
    size_t read_number(unsigned char byte_n);

    void close();
private:
    std::vector<bool> read_bit_array;
    std::ifstream file;
    int extra_bits;
};


#endif //LAB6_COMPRESSION_BITIFILE_H
