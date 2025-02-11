#include "archiver.h"

bool Compare::operator()(std::pair<int, std::shared_ptr<Node>> a, std::pair<int, std::shared_ptr<Node>> b) {
    if (a.first > b.first) {
        return true;
    }

    if (a.first < b.first) {
        return false;
    }

    return a.second->ch > b.second->ch;
}

bool Compare::operator()(std::pair<int, char16_t> a, std::pair<int, char16_t> b) {
    if (a.first > b.first) {
        return true;
    }

    if (a.first < b.first) {
        return false;
    }

    return a.second > b.second;
}

template <typename T>
size_t PriorityQueue<T>::Parent(size_t id) {
    return (id - 1) / 2;
}

template <typename T>
size_t PriorityQueue<T>::LeftSon(size_t id) {
    return 2 * id + 1;
}

template <typename T>
size_t PriorityQueue<T>::RightSon(size_t id) {
    return 2 * id + 2;
}

template <typename T>
void PriorityQueue<T>::GetUp(size_t id) {
    while (id > 0) {
        size_t p = Parent(id);
        if (comp_(heap_[p], heap_[id])) {
            std::swap(heap_[p], heap_[id]);
            id = p;
        } else {
            break;
        }
    }
}

template <typename T>
void PriorityQueue<T>::GetDown(size_t id) {
    size_t size = heap_.size();
    while (LeftSon(id) < size) {
        size_t son = LeftSon(id);
        if (RightSon(id) < size && comp_(heap_[son], heap_[RightSon(id)])) {
            son = RightSon(id);
        }

        if (comp_(heap_[id], heap_[son])) {
            std::swap(heap_[id], heap_[son]);
            id = son;
        } else {
            break;
        }
    }
}

template <typename T>
bool PriorityQueue<T>::Empty() {
    return heap_.empty();
}

template <typename T>
size_t PriorityQueue<T>::Size() {
    return heap_.size();
}

template <typename T>
const T &PriorityQueue<T>::Top() {
    return heap_[0];
}

template <typename T>
void PriorityQueue<T>::Push(const T &value) {
    heap_.push_back(value);
    GetUp(heap_.size() - 1);
}

template <typename T>
void PriorityQueue<T>::Pop() {
    heap_[0] = heap_.back();
    heap_.pop_back();
    if (!heap_.empty()) {
        GetDown(0);
    }
}

template <typename T>
PriorityQueue<T>::PriorityQueue() : heap_(), comp_(Compare()) {
}

template <typename T>
PriorityQueue<T>::~PriorityQueue() = default;