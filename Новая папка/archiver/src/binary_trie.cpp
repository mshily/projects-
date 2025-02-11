#include "archiver.h"

BinaryTrie::BinaryTrie() {
    son[0] = son[1] = nullptr;
    symbol = ' ';
}