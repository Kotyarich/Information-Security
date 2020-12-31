#include <iostream>
#include <fstream>
#include <vector>
#include "sign.h"

int main(int argc, char *argv[]) {
    OpenSSL_add_all_algorithms();
    EVP_PKEY *skey = nullptr, *vkey = nullptr;
    byte *sig = nullptr;
    size_t slen = 0;

    if (argv[1][0] == 's') {
        make_keys(&skey, &vkey);
        write_keys(skey, vkey);

        std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(static_cast<unsigned long>(size));
        file.read(reinterpret_cast<char *>(buffer.data()), size);

        sign(buffer.data(), buffer.size(), &sig, &slen, skey);
        write_sign(sig, slen, argv[3]);
        printf("Created signature\n");
        print_labeled("Signature", sig, slen);
    } else {
        read_keys(&skey, &vkey);
        slen = read_sign(&sig, argv[3]);

        std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(static_cast<unsigned long>(size));
        file.read(reinterpret_cast<char *>(buffer.data()), size);

        auto rc = verify(buffer.data(), buffer.size(), sig, slen, vkey);
        if (rc == 0) {
            printf("Verified signature\n");
        } else {
            printf("Failed to verify signature, return code %d\n", rc);
        }
    }

    OPENSSL_free(sig);
    EVP_PKEY_free(skey);
    EVP_PKEY_free(vkey);

    return 0;
}
