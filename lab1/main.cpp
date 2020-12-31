#include <iostream>
#include <string>
#include <fstream>
#include "key_generator.h"

int main() {
    auto key = getLicenseKey();
    auto f = std::ofstream("key", std::ofstream::out);
    f << key;
    f.close();

    return 0;
}
