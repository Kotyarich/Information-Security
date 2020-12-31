#include <iostream>
#include <fstream>
#include <sstream>
#include "aes.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        std::cout << "flag d or e" << std::endl
                  << "path to encoding file" << std::endl
                  << "name for encoded file" << std::endl
                  << "key" << std::endl;
        return 0;
    }

    auto flag = argv[1];
    auto path = argv[2];
    auto output_name = argv[3];
    auto key = argv[4];

    AES aes(key);

    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto data = buffer.str();

    std::string result;
    if (flag[0] == 'e') {
        result = aes.encode(data);
    } else {
        result = aes.decode(data);
    }

    std::ofstream output(output_name);
    output << result;

    return 0;
}