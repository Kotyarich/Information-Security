#include <iostream>
#include "Rotor.h"

Rotor::Rotor(std::default_random_engine *re, std::string conf_file_name)
    : state(256), random_engine(re), rotation_number(0), config_name(std::move(conf_file_name)) {
    for (auto i = 0u; i < 256; i++) {
        state[i] = static_cast<unsigned char>(i);
    }

    std::shuffle(state.begin(), state.end(), *random_engine);

    save_state();
}

void Rotor::rotate() {
    std::rotate(state.begin(), state.begin() + 1, state.end());
    rotation_number = (rotation_number + 1) % 256;
}

unsigned char Rotor::encode_symbol(unsigned char symbol) const {
    auto encoded = state[symbol];
    return encoded;
}

unsigned char Rotor::encode_symbol_back(unsigned char symbol) const {
    auto pos = std::find(state.begin(), state.end(), symbol) - state.begin();
    return static_cast<unsigned char>(pos);
}

size_t Rotor::get_rotation() const {
    return rotation_number;
}

void Rotor::load_state() {
    std::ifstream file(config_name);

    for (auto &item: state) {
        int tmp;
        file >> tmp;
        item = static_cast<unsigned char>(tmp);
    }

    file.close();
}

void Rotor::save_state() const {
    std::ofstream file(config_name);

    for (auto &item: state) {
        file << int(item) << " ";
    }

    file.close();
}

Rotor::Rotor(std::string conf_file_name)
    : state(256), rotation_number(0), config_name(std::move(conf_file_name)) {
    load_state();
}
