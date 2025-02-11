#include "archiver.h"
#include "BitWriteAndRead.h"

std::string ToBitString(int x, size_t sz) {
    std::string bit;
    while (x) {
        bit += static_cast<char>((x & 1) + '0');
        x >>= 1;
    }

    while (bit.size() < sz) {
        bit += '0';
    }

    std::reverse(bit.begin(), bit.end());
    return bit;
}

void CountOfCodes(std::shared_ptr<Node> ptr, std::string &s, std::vector<Object> &codes) {
    if (!ptr) {
        return;
    }

    if (ptr->zero_son == nullptr || ptr->one_son == nullptr) {
        codes.emplace_back(ptr->ch, s);
        return;
    }

    s += '1';
    CountOfCodes(ptr->one_son, s, codes);
    s.pop_back();

    s += '0';
    CountOfCodes(ptr->zero_son, s, codes);
    s.pop_back();
}

std::unordered_map<char16_t, std::string> CreateCanonView(std::vector<Object> &codes) {
    std::unordered_map<char16_t, std::string> mp_code;
    if (codes.empty()) {
        return mp_code;
    }

    std::sort(codes.begin(), codes.end(), [](const Object &a, const Object &b) {
        if (a.code.size() < b.code.size()) {
            return true;
        }

        if (a.code.size() > b.code.size()) {
            return false;
        }

        if (a.code.size() == b.code.size()) {
            return a.symbol < b.symbol;
        }
        return true;
    });

    int current_number = 0;
    size_t last_size = codes[0].code.size();

    for (auto &it : codes) {
        if (it.code.size() != last_size) {
            size_t diff = it.code.size() - last_size;
            current_number <<= static_cast<int>(diff);
        }

        last_size = it.code.size();

        mp_code[it.symbol] = ToBitString(current_number, it.code.size());
        ++current_number;
    }

    return mp_code;
}

void Compress(const std::string &archive_name, const std::vector<std::string> &file_pathes) {

    size_t count_file_names = file_pathes.size();
    BitPrint out(archive_name);

    for (auto &file_path : file_pathes) {

        std::string file_name = std::filesystem::path(file_path).filename().string();

        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "file is not open!" << std::endl;
            exit(ERROR);
        }

        std::unordered_map<char16_t, int> weight;

        // add file name
        for (auto &it : file_name) {
            ++weight[static_cast<char16_t>(it)];
        }

        // add service symbols
        ++weight[FILENAME_END];
        ++weight[ONE_MORE_FILE];
        ++weight[ARCHIVE_END];

        // add main symbols
        char cur = 0;
        while (file.get(cur)) {
            ++weight[static_cast<char16_t>(cur)];
        }

        file.close();

        std::shared_ptr<Node> root = BuildTree(weight);
        std::string s;
        std::vector<Object> codes;

        CountOfCodes(root, s, codes);
        std::unordered_map<char16_t, std::string> mp = CreateCanonView(codes);

        --count_file_names;
        bool is_last = count_file_names == 0;
        PrintCompressor(mp, codes, out, file_name, is_last, file_path);
    }

    if (out.index_in_bits_ != BLOCK_BITS_SIZE) {
        out.output_ << out.code_;
    }
    out.output_.close();
}