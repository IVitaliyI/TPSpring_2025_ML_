#include <cassert>
#include <iostream>
#include <vector>


struct Comparator {
    bool operator()(const int left, const int right) {
        return left < right;
    }
};

struct CBinaryNode {
    int Data;
    CBinaryNode *left;
    CBinaryNode *right;
    CBinaryNode *parent;
};

template <typename T, typename Comparator = std::less<T>>
class BinaryTree {
    private:
        Comparator cmp = Comparator();
        CBinaryNode *root = nullptr;
    public:
        bool insert(const T& object);
};

template <typename T, typename Comparator>
bool BinaryTree<T, Comparator>::insert(const T& object) {
    if (root == nullptr) {
        root = {object, nullptr, nullptr, nullptr};
    } else {
        CBinaryNode *lr;
        while (lr != nullptr)
    }
}

int main() {
    return 0;
}