#include "archiver.h"
// block is 8 bits

// bit blocks
inline void BitPrint::NewBlock() {
    code_ = 0;
    index_in_bits_ = BLOCK_BITS_SIZE;
}

inline void BitPrint::IsNextBlock() {
    if (index_in_bits_ < 0) {
        output_ << code_;
        NewBlock();
    }
}

inline void BitRead::IsCreateNewBlock() {
    if (index_in_bits_ < 0) {
        input_.get(ch_);
        index_in_bits_ = BLOCK_BITS_SIZE;
    }
}

// prints
inline void BitPrint::Print(const char16_t& symbol, const int& count) {
    int bit = count;
    while (bit-- > 0) {
        bool val = ((1 << bit)) & symbol;
        if (val) {
            int pw = 1 << index_in_bits_;
            code_ = static_cast<char>(static_cast<int>(code_) | pw);
        }
        --index_in_bits_;
        IsNextBlock();
    }
}

inline bool BitRead::ReadOneBit() {
    IsCreateNewBlock();

    int k = 1 << index_in_bits_;
    k &= ch_;
    --index_in_bits_;
    return k;
}

inline char16_t BitRead::GoRead(const int& count) {
    char16_t res = 0;
    int bit = count;
    while (bit--) {
        res |= (ReadOneBit() << bit);
    }
    return res;
}

// init
inline BitPrint::BitPrint(const std::string& archive_name) {
    NewBlock();
    output_.open(archive_name);
    if (!output_.is_open()) {
        std::cerr << "output is not open!" << std::endl;
        exit(ERROR);
    }
}

inline BitRead::BitRead(const std::string& archive_name) {
    index_in_bits_ = -1;
    ch_ = 0;
    input_.open(archive_name);
    if (!input_.is_open()) {
        std::cerr << "input is not open!" << std::endl;
        exit(ERROR);
    }
}