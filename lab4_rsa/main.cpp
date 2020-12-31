#include <fstream>
#include "BitOFile.h"
#include "BitIFile.h"
#include "RSAKeyGenerator.h"
#include "RSA.h"

unsigned char bytes_per_int(unsigned int n) {
    auto bits_per_freq = std::ceil(std::log2(static_cast<double>(n)));
    auto bytes_per_freq = std::ceil(bits_per_freq / 8);
    return static_cast<unsigned char>(bytes_per_freq);
}


int main(int argc, char **argv) {
    if (argv[1][0] == 'g') {
        RSAKeyGenerator generator;
        auto keys = generator.generate_keys();
        std::ofstream file("public.key");
        file << keys.first.first << " " << keys.first.second;
        std::ofstream private_file("private.key");
        private_file << keys.second.first << " " << keys.second.second;
    } else if (argv[1][0] == 'e') {
        std::ifstream inp(argv[2], std::fstream::binary);
        BitOFile otp(argv[3]);
        std::ifstream key("public.key");
        unsigned int e, n;
        key >> e >> n;
        auto bytes = bytes_per_int(n);

        RSA rsa;
        rsa.set_open_key(e, n);
        while (!inp.eof()) {
            auto symbol = inp.get();
            if (symbol < 0) {
                break;
            }
            auto encoded = rsa.encode(static_cast<unsigned char>(symbol));
            otp.write_number(encoded, bytes);
        }
        inp.close();
        otp.close();
    } else {
        BitIFile inp(argv[2]);
        std::ofstream otp(argv[3], std::fstream::binary);
        std::ifstream key("private.key");
        unsigned int d, n;
        key >> d >> n;
        auto bytes = bytes_per_int(n);

        RSA rsa;
        rsa.set_private_key(d, n);
        while (!inp.eof()) {
            auto symbol = inp.read_number(bytes);
            auto decoded = rsa.decode(symbol);
            otp.put(decoded);
        }

        inp.close();
        otp.close();
    }

    return 0;
}