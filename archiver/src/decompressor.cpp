// 1. count of symbols | COUNT_BITS bits
// 2. all alphabet | each consist COUNT_BITS bits
// 3. counts of all sizes | each consist COUNT_BITS bits
// 4. file name
// 5. symbol end of file name
// 6. file
// 7. next file or end of files

#include "archiver.h"
#include "bit_write_and_read.h"

char16_t FindCharInTrie(std::shared_ptr<BinaryTrie> ptr, BitRead &in) {
    if (ptr->son[0] == nullptr || ptr->son[1] == nullptr) {
        return ptr->symbol;
    }

    bool bit = in.GoRead(1);
    return FindCharInTrie(ptr->son[bit], in);
}

void AddToTrie(std::shared_ptr<BinaryTrie> current_vertex, const std::string &binary_string, const char16_t &symbol) {
    for (auto &bit : binary_string) {
        if (current_vertex->son[bit - '0'] == nullptr) {
            auto new_son = std::make_shared<BinaryTrie>();
            current_vertex->son[bit - '0'] = new_son;

            current_vertex = new_son;
            continue;
        }
        current_vertex = current_vertex->son[bit - '0'];
    }
    current_vertex->symbol = symbol;
}

void DeleteTrie(std::shared_ptr<BinaryTrie> ptr) {
    for (size_t bit = 0; bit < 2; ++bit) {
        if (ptr->son[bit] != nullptr) {
            DeleteTrie(ptr->son[bit]);
        }
    }
    ptr.reset();
}

void Decompress(const std::string &archive_name) {
    BitRead in(archive_name);
    while (true) {
        auto root = std::make_shared<BinaryTrie>();

        char16_t size_of_alphabet = in.GoRead(COUNT_BITS);

        std::vector<char16_t> alphabet;
        for (size_t step = 1; step <= size_of_alphabet; ++step) {
            char16_t symbol = in.GoRead(COUNT_BITS);

            alphabet.push_back(symbol);
        }

        std::vector<int> count_of_len;  // counting each of lens
        int count_of_symbols = 0;
        while (count_of_symbols < size_of_alphabet) {
            char16_t current_counting = in.GoRead(COUNT_BITS);

            count_of_symbols += current_counting;
            count_of_len.push_back(current_counting);
        }

        // creating Canon views and build trie
        size_t current_len = 1;
        size_t last_len = 0;
        int current_number = 0;
        for (auto &c : alphabet) {
            while (!count_of_len[current_len - 1]) {
                ++current_len;
            }
            --count_of_len[current_len - 1];

            if (current_len != last_len) {
                size_t diff = current_len - last_len;
                current_number <<= static_cast<int>(diff);
            }

            last_len = current_len;

            AddToTrie(root, ToBitString(current_number, current_len), c);
            ++current_number;
        }

        std::string file_name;
        // decode file name
        while (true) {
            char16_t symbol = FindCharInTrie(root, in);

            if (symbol == FILENAME_END) {
                break;
            }
            file_name += static_cast<char>(symbol);
        }

        // decode our file
        bool have_next = false;
        std::string file;
        while (true) {
            char16_t symbol = FindCharInTrie(root, in);

            if (symbol == ONE_MORE_FILE) {
                have_next = true;
                break;
            }

            if (symbol == ARCHIVE_END) {
                have_next = false;
                break;
            }

            file += static_cast<char>(symbol);
        }

        std::ofstream out(file_name);
        out << file;
        out.close();

        if (!have_next) {
            in.input_.close();
            DeleteTrie(root);
            return;
        }
        DeleteTrie(root);
    }
}
