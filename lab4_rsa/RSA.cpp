#include "RSA.h"

unsigned int RSA::binpow(unsigned int a, unsigned int n, unsigned int m) {
    unsigned int res = 1;
    while (n) {
        if (n & 1) {
            res *= a;
            res %= m;
        }
        a *= (a % m);
        a %= m;
        n >>= 1;
    }
    return res % m;
}

unsigned int RSA::encode(unsigned char symbol) {
    auto encoded = binpow(symbol, e, n);
    return encoded;
}

void RSA::set_open_key(unsigned int e, unsigned int n) {
    this->e = e;
    this->n = n;
}

unsigned char RSA::decode(unsigned int symbol) {
    auto decoded = binpow(symbol, d, n);
    return static_cast<unsigned char>(decoded);
}

void RSA::set_private_key(unsigned int d, unsigned int n) {
    this->d = d;
    this->n = n;
}
