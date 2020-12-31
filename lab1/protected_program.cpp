#include <fstream>
#include <iostream>
#include "key_generator.h"

int main() {
    auto key = getLicenseKey();
    auto f = std::ifstream("key", std::ofstream::in);

    std::string user_key;
    f >> user_key;
    f.close();

    if (!user_key.empty() && key == std::stoull(user_key)) {
        std::cout << "Hello world!" << std::endl;
    } else {
        std::cout << "Your key is incorrect" << std::endl;
    }

    return 0;
}
