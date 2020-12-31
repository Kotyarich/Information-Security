#include "Huffman.h"

void Huffman::compress(std::string inp_file, std::string otp_file) {
    auto priorities = get_priorities(inp_file);
    auto tree = get_h_tree(priorities);

    HTable table;
    build_h_table(tree, table, "");

    std::ifstream file(inp_file, std::ios::binary);
    BitOFile output(otp_file);

    auto table_len = table.size() - 1;
    output.write_byte(static_cast<unsigned char>(table_len));

    size_t max = *std::max_element(priorities.begin(), priorities.end());
    auto bits_per_freq = std::ceil(std::log2(static_cast<double>(max)));
    auto bytes_per_freq = std::ceil(bits_per_freq / 8);
    output.write_byte(static_cast<unsigned char>(bytes_per_freq));

    for (auto &elem: table) {
        output.write_byte(elem.first);
        output.write_number(priorities[elem.first], static_cast<unsigned char>(bytes_per_freq));
    }

    while (true) {
        auto byte = static_cast<unsigned char>(file.get());
        if (file.eof()) {
            break;
        }
        auto code = table[byte];
        for (auto &ch: code) {
            output.write_bit(ch == '1');
        }
    }

    output.close();
}

// Считаем частоту для каждого символа в файле
std::vector<size_t> Huffman::get_priorities(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<size_t> priorities(256, 0);

    while (!file.eof()) {
        auto byte = static_cast<unsigned char>(file.get());
        priorities[byte]++;
        // TODO bytes 10 and 255 in the end of file, wtf
    }

    file.close();
    return priorities;
}

// Построение дерева Хаффмана
TreeNode *Huffman::get_h_tree(const std::vector<size_t> &priorities) {
    // Для получения канонического кода сравниваем сначала по частоте, потом по символу
    auto cmp = [](TreeNode *a, TreeNode *b) {
        if (a->get_priority() < b->get_priority()) {
            return true;
        } else if (a->get_priority() == b->get_priority()) {
            return a->get_byte() > b->get_byte();
        }
        return false;
    };

    std::set<TreeNode *, decltype(cmp)> queue(cmp);
    for (auto i = 0; i < priorities.size(); i++) {
        if (priorities[i] == 0) {
            continue;
        }

        auto node = new TreeNode(static_cast<unsigned char>(i), priorities[i]);
        queue.insert(node);
    }

    while (queue.size() > 1) {
        auto node1 = *queue.begin();
        queue.erase(queue.begin());
        auto node2 = *queue.begin();
        queue.erase(queue.begin());

        auto new_node = new TreeNode(0, node1->get_priority() + node2->get_priority());
        new_node->add_child(node2);
        new_node->add_child(node1);

        queue.insert(new_node);
    }

    return *queue.begin();
}

// Построение таблицы Хаффмана на основе дерева
void Huffman::build_h_table(const TreeNode *h_tree, HTable &table, std::string str) {
    if (!h_tree) {
        return;
    }

    if (!h_tree->get_left() && !h_tree->get_right()) {
        table[h_tree->get_byte()] = str;
        return;
    }

    build_h_table(h_tree->get_left(), table, str + "0");
    build_h_table(h_tree->get_right(), table, str + "1");
}

void Huffman::decompress(std::string inp_file, std::string otp_file) {
    BitIFile file(inp_file);
    auto tree = read_h_tree(file);
    HTable table;
    build_h_table(tree, table, "");

    std::ofstream output(otp_file, std::fstream::binary);
    try {
        std::string code;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (true) {
            auto bit = file.read_bit();
            code += bit ? "1" : "0";
            for (auto &item: table) {
                if (code == item.second) {
                    output.put(item.first);
                    code = "";
                    continue;
                }
            }
        }
#pragma clang diagnostic pop
    } catch (std::exception &) {}


    file.close();
    output.close();
}

TreeNode *Huffman::read_h_tree(BitIFile &file) {
    int symbols_n = file.read_byte() + 1;
    auto bytes_per_freq = file.read_byte();
    std::vector<size_t> priorities(256, 0);

    for (auto i = 0; i < symbols_n; i++) {
        auto byte = file.read_byte();
        auto freq = file.read_number(bytes_per_freq);
        priorities[byte] = freq;
    }

    auto tree = get_h_tree(priorities);

    return tree;
}
