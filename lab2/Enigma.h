#ifndef LAB2_ENIGMA_H
#define LAB2_ENIGMA_H

#include <string>
#include <fstream>
#include "Rotor.h"
#include "Reflector.h"

enum EnigmaType {
    encoder,
    decoder,
};

class Enigma {
public:
    explicit Enigma(EnigmaType type);

    unsigned char encode_symbol(unsigned char symbol);

    ~Enigma();
private:
    static const constexpr size_t ROTORS_NUMBER = 3;
    std::random_device rd;
    std::default_random_engine random_engine;

    std::vector<Rotor> rotors;
    Reflector *reflector;

    void rotate_rotors();
};


#endif //LAB2_ENIGMA_H
