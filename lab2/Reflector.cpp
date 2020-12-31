#include "Reflector.h"

Reflector::Reflector(std::default_random_engine *re, std::string conf_file_name)
    : state(256), random_engine(re), config_name(std::move(conf_file_name)) {

    std::vector<int> generated_state(256, -1);

    for (auto i = 0u; i < 256; i++) {
        if (generated_state[i] != -1) {
            continue;
        }

        while (true) {
            auto j = i + 1 + (*random_engine)() % (256 - i);
            if (generated_state[j] != -1) {
                continue;
            }

            generated_state[j] = i;
            generated_state[i] = static_cast<int>(j);
            break;
        }
    }

    std::copy(generated_state.begin(), generated_state.end(), state.begin());

    safe_configuration();
}

unsigned char Reflector::reflect(unsigned char symbol) const {
    return state[symbol];
}

void Reflector::load_configuration() {
    std::ifstream file(config_name);

    for (auto &item: state) {
        int tmp;
        file >> tmp;
        item = static_cast<unsigned char>(tmp);
    }

    file.close();
}

void Reflector::safe_configuration() const {
    std::ofstream file(config_name);

    for (auto &item: state) {
        file << int(item) << " ";
    }

    file.close();
}

Reflector::Reflector(std::string conf_file_name)
    : state(256), config_name(std::move(conf_file_name)) {
    load_configuration();
}
