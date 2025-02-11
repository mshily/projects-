#pragma once

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <memory>

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;
const int COUNT_BITS = 9;
const int BLOCK_BITS_SIZE = 7;
const int ERROR = 111;

struct BinaryTrie {
    std::shared_ptr<BinaryTrie> son[2];
    char16_t symbol;

    BinaryTrie();
    ~BinaryTrie() = default;
};
std::shared_ptr<BinaryTrie> BuildTree(std::unordered_map<char16_t, int> weight);

struct Reader {
    Reader(int argc, char **argv);

    bool is_compress = false;
    bool is_decompress = false;
    bool is_log = false;

    std::string archive_name;
    std::vector<std::string> file_pathes;
};

struct SymbolCodes {
    char16_t symbol;
    std::string code;
    SymbolCodes(char16_t symbol_c, std::string code_c) {
        symbol = symbol_c;
        code = code_c;
    }
};
std::unordered_map<char16_t, std::string> CreateCanonView(std::vector<SymbolCodes> &codes);
void Compress(const std::string &archive_name, const std::vector<std::string> &file_pathes);

std::string ToBitString(int x, size_t sz);
void Decompress(const std::string &archive_name);

struct Compare {
    bool operator()(std::pair<int, std::shared_ptr<BinaryTrie>> a, std::pair<int, std::shared_ptr<BinaryTrie>> b);
    bool operator()(std::pair<int, char16_t> a, std::pair<int, char16_t> b);
};

template <typename T>
struct PriorityQueue {
private:
    std::vector<T> heap_;
    Compare comp_;
    size_t Parent(size_t index);
    size_t LeftSon(size_t index);
    size_t RightSon(size_t index);
    void GetUp(size_t index);
    void GetDown(size_t index);

public:
    PriorityQueue();
    bool Empty();
    size_t Size();
    const T &Top();
    void Push(const T &value);
    void Pop();
    ~PriorityQueue();
};

struct BitPrint {
    std::ofstream output_;
    char code_;
    int index_in_bits_;

    void IsNextBlock();
    void NewBlock();

    explicit BitPrint(const std::string &archive_name);
    void Print(const char16_t &symbol, const int &count);
    ~BitPrint() = default;
};

struct BitRead {
    std::ifstream input_;
    int index_in_bits_;
    char ch_;

    explicit BitRead(const std::string &archive_name);
    ~BitRead() = default;

    void IsCreateNewBlock();
    bool ReadOneBit();

    char16_t GoRead(const int &count);
};

void PrintCompressor(std::unordered_map<char16_t, std::string> &mp, const std::vector<SymbolCodes> &codes,
                     BitPrint &out, const std::string &file_name, bool is_last_file, const std::string &file_path);