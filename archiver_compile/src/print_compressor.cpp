#include "archiver.h"
#include "BitWriteAndRead.h"

size_t FindMaxCodeSize(const std::vector<Object>& codes) {
    size_t mx = 0;
    for (const auto& code : codes) {
        mx = std::max(mx, code.code.size());
    }
    return mx;
}
int FromBitToInt1(const std::string& s) {
    int res = 0;
    size_t by = s.size() - 1;
    for (char i : s) {
        res |= (1 << by) * (i - '0');
        --by;
    }
    return res;
}
void SizeCounting(const std::vector<Object>& codes, std::vector<int>& count_of_sizes) {
    for (const auto& code : codes) {
        ++count_of_sizes[code.code.size()];
    }
}

void PrintCompressor(std::unordered_map<char16_t, std::string>& math_of_codes, const std::vector<Object>& codes,
                     BitPrint& out, const std::string& file_name, bool is_last_file, const std::string& file_path) {

    size_t max_symbol_code_size = FindMaxCodeSize(codes);

    std::vector<int> count_of_sizes(max_symbol_code_size + 1, 0);
    SizeCounting(codes, count_of_sizes);

    // Print
    out.Print(math_of_codes.size(), COUNT_BITS);  // count of symbols
    for (const auto& symbol : codes) {            // all symbols
        out.Print(symbol.symbol, COUNT_BITS);
    }

    for (size_t i = 0; i < max_symbol_code_size; ++i) {  // counts of sizes
        out.Print(count_of_sizes[i + 1], COUNT_BITS);
    }

    for (char it : file_name) {  // file names
        out.Print(FromBitToInt1(math_of_codes[it]), static_cast<int>(math_of_codes[it].size()));
    }
    // FILENAME_END
    out.Print(FromBitToInt1(math_of_codes[FILENAME_END]), static_cast<int>(math_of_codes[FILENAME_END].size()));

    std::ifstream in(file_path);  // contents of file
    char ch = 0;
    while (in.get(ch)) {
        out.Print(FromBitToInt1(math_of_codes[ch]), static_cast<int>(math_of_codes[ch].size()));
    }
    in.close();

    if (!is_last_file) {  // next string symbol
        out.Print(FromBitToInt1(math_of_codes[ONE_MORE_FILE]), static_cast<int>(math_of_codes[ONE_MORE_FILE].size()));
    } else {
        out.Print(FromBitToInt1(math_of_codes[ARCHIVE_END]), static_cast<int>(math_of_codes[ARCHIVE_END].size()));
    }
}