#ifndef LAB2_REFLECTOR_H
#define LAB2_REFLECTOR_H

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

class Reflector {
public:
    Reflector(std::default_random_engine *re, std::string conf_file_name);
    explicit Reflector(std::string conf_file_name);

    unsigned char reflect(unsigned char symbol) const;
    void load_configuration();
private:
    std::vector<unsigned char> state;
    std::default_random_engine *random_engine;
    std::string config_name;

    void safe_configuration() const;
};


#endif //LAB2_REFLECTOR_H
