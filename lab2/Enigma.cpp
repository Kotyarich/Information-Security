#include "Enigma.h"

Enigma::Enigma(EnigmaType type)
    : random_engine(rd()) {
    for (auto i = 0; i < ROTORS_NUMBER; i++) {
        auto conf_name = "rotor" + std::to_string(i);
        if (type == encoder) {
            rotors.emplace_back(&random_engine, conf_name);
        } else {
            rotors.emplace_back(conf_name);
        }
    }

    auto conf_name = "reflector";
    if (type == encoder) {
        reflector = new Reflector(&random_engine, conf_name);
    } else {
        reflector = new Reflector(conf_name);
    }
}

Enigma::~Enigma() {
    delete reflector;
}

unsigned char Enigma::encode_symbol(unsigned char symbol) {
    auto encoded = symbol;
    for (auto &rotor: rotors) {
        encoded = rotor.encode_symbol(encoded);
    }

    encoded = reflector->reflect(encoded);

    for (auto i = static_cast<int>(ROTORS_NUMBER - 1); i >= 0; i--) {
        encoded = rotors[i].encode_symbol_back(encoded);
    }

    rotate_rotors();

    return encoded;
}

void Enigma::rotate_rotors() {
    for (auto i = 0ul; i < ROTORS_NUMBER; i++) {
        if (rotors[i].get_rotation()) {
            rotors[i].rotate();
            break;
        } else if (i == ROTORS_NUMBER - 1) {
            rotors[0].rotate();
            break;
        } else if (!rotors[i + 1].get_rotation()) {
            rotors[i].rotate();
            break;
        }
    }
}

