#ifndef LAB6_COMPRESSION_BITOFILE_H
#define LAB6_COMPRESSION_BITOFILE_H

#include <fstream>
#include <vector>
#include <string>

class BitOFile {
public:
    explicit BitOFile(const std::string &name);

    void write_bit(bool bit);
    void write_byte(unsigned char byte);
    void write_number(size_t number, unsigned char byte);

    bool eof();

    void close();
private:
    std::vector<bool> write_bit_array;
    std::ofstream file;

    void flush();
};


#endif //LAB6_COMPRESSION_BITOFILE_H
