#include "archiver.h"
#include "priority_queue.h"

// Haffmann`s Tree Build

void MakeNewVertex(std::shared_ptr<Node> left_son, std::shared_ptr<Node> right_son, int weight,
                   PriorityQueue<std::pair<int, std::shared_ptr<Node>>> &q) {
    auto ptr = std::make_shared<Node>();

    ptr->ch = std::min(left_son->ch, right_son->ch);

    ptr->zero_son = left_son;
    ptr->one_son = right_son;

    q.Push(std::make_pair(weight, ptr));
}

std::shared_ptr<Node> BuildTree(std::unordered_map<char16_t, int> weight) {  // get root of our tree
    // fill PriorityQueue with help of starting values
    PriorityQueue<std::pair<int, char16_t>> start_value;
    for (auto [val, w] : weight) {
        start_value.Push({w, static_cast<char16_t>(val)});
    }

    // fill tree with help of PriorityQueue
    PriorityQueue<std::pair<int, std::shared_ptr<Node>>> q;

    while (!start_value.Empty()) {
        std::pair<int, char16_t> cur = start_value.Top();
        start_value.Pop();

        auto ptr = std::make_shared<Node>();
        ptr->ch = cur.second;

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