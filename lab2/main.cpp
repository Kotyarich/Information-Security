#include <fstream>
#include <iostream>
#include "Enigma.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "-e to encode\n-d to decode\ninput filename\noutput filename\n";
        exit(1);
    }

    EnigmaType type = std::string(argv[1]) == "-d"? decoder: encoder;
    Enigma enigma(type);

    std::ifstream file(argv[2]);
    std::ofstream encoded_file(argv[3]);

    char symbol;
    file.read(&symbol, 1);
    while (!file.eof()) {
        auto encoded = enigma.encode_symbol(static_cast<unsigned char>(symbol));
        encoded_file.write(reinterpret_cast<const char *>(&encoded), 1);

        file.read(&symbol, 1);
    }

    return 0;
}