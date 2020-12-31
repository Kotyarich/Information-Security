#ifndef LAB2_ROTOR_H
#define LAB2_ROTOR_H

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

class Rotor {
public:
    Rotor(std::default_random_engine *re, std::string conf_file_name);
    explicit Rotor(std::string conf_file_name);

    unsigned char encode_symbol(unsigned char symbol) const;
    unsigned char encode_symbol_back(unsigned char symbol) const;

    void rotate();
    size_t get_rotation() const;
private:
    std::vector<unsigned char> state;
    std::default_random_engine *random_engine;
    size_t rotation_number;
    std::string config_name;

    void load_state();
    void save_state() const;
};


#endif //LAB2_ROTOR_H
