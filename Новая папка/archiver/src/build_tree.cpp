#include "archiver.h"
#include "priority_queue.h"

// Haffmann`s Tree Build

void MakeNewVertex(std::shared_ptr<BinaryTrie> left_son, std::shared_ptr<BinaryTrie> right_son, int weight,
                   PriorityQueue<std::pair<int, std::shared_ptr<BinaryTrie>>> &q) {
    auto ptr = std::make_shared<BinaryTrie>();

    ptr->symbol = std::min(left_son->symbol, right_son->symbol);

    ptr->son[1] = left_son;
    ptr->son[0] = right_son;

    q.Push(std::make_pair(weight, ptr));
}

std::shared_ptr<BinaryTrie> BuildTree(std::unordered_map<char16_t, int> weight) {  // get root of our tree
    // fill PriorityQueue with help of starting values
    PriorityQueue<std::pair<int, char16_t>> start_value;
    for (auto [val, w] : weight) {
        start_value.Push({w, static_cast<char16_t>(val)});
    }

    // fill tree with help of PriorityQueue
    PriorityQueue<std::pair<int, std::shared_ptr<BinaryTrie>>> q;

    while (!start_value.Empty()) {
        std::pair<int, char16_t> cur = start_value.Top();
        start_value.Pop();

        auto ptr = std::make_shared<BinaryTrie>();
        ptr->symbol = cur.second;

        q.Push({cur.first, ptr});
    }

    // build tree
    while (q.Size() > 1) {
        auto first = q.Top();
        q.Pop();

        auto second = q.Top();
        q.Pop();

        MakeNewVertex(first.second, second.second, first.first + second.first, q);
    }

    return q.Top().second;
}