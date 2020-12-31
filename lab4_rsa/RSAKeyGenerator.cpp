#include "RSAKeyGenerator.h"


RSAKeyGenerator::RSAKeyGenerator(): gen(std::random_device()()), distribution(100, 200), e_distribution(5, 20){}

std::pair<Key, Key> RSAKeyGenerator::generate_keys() {
    unsigned int p = random_prime();
    unsigned int q = random_prime();
    unsigned int n = p * q;
    unsigned int phi = (p - 1) * (q - 1);
    unsigned int e = generate_e(phi);
    unsigned int d = generate_d(e, phi);

    return std::make_pair(std::make_pair(e, n), std::make_pair(d, n));
}

unsigned int RSAKeyGenerator::random_prime() {
    int prime = distribution(gen);
    while (!is_prime(prime)) {
        prime = distribution(gen);
    }
    return static_cast<unsigned int>(prime);
}

bool RSAKeyGenerator::is_prime(int num) {
    for (auto &el: primes) {
        if (num % el == 0) {
            return false;
        }
    }
    return true;
}

unsigned int RSAKeyGenerator::generate_e(unsigned int phi) {
    auto e = static_cast<unsigned int>(e_distribution(gen));
    while (gcd(e, phi) != 1) {
        e = static_cast<unsigned int>(e_distribution(gen));
    }
    return e;
}

unsigned int RSAKeyGenerator::gcd(unsigned int a, unsigned int b) {
    for (unsigned int c; b; ) {
        c = a % b;
        a = b;
        b = c;
    }
    return (a < 0) ? -a : a;
}

unsigned int RSAKeyGenerator::generate_d(unsigned int e, unsigned int phi) {
    auto res = gcdx(e, phi);
    return res[1] % phi;
}

std::vector<unsigned int> RSAKeyGenerator::gcdx(unsigned int a, unsigned int b) {
    if (!b) {
        return {a, 1, 0};
    }
    auto res = gcdx(b, a % b);
    unsigned x = res[2];
    unsigned y = res[1] - (a / b) * res[2];

    return {res[0], x, y};
}
