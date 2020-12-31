#include <iostream>
#include "Huffman.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "c to compress\nd to decompress\ninput filename\noutput filename\n";
        exit(1);
    }

    Huffman huffman;
    if (argv[1][0] == 'c') {
        huffman.compress(argv[2], argv[3]);
    } else {
        huffman.decompress(argv[2], argv[3]);
    }

    return 0;
}