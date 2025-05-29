#include <cassert>
#include <deque>
#include <iostream>
#include <stack>
#include <vector>

struct Comparator {
    bool operator()(const int left, const int right) { return left < right; }
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
    void insert(const T &object);
    void levelBFS(CBinaryNode *node);
    bool visit(CBinaryNode *node, T *element);

    bool allNodeSame(CBinaryNode *root);
    bool checkSame(CBinaryNode *node, T value);


    CBinaryNode *GetRoot() { return root; }

    void destroy(CBinaryNode *node);

    ~BinaryTree() { destroy(root); }
};

template <typename T, typename Comparator>
void BinaryTree<T, Comparator>::destroy(CBinaryNode *node) {
    if (node == nullptr) return;

    std::stack<CBinaryNode *> stack;
    CBinaryNode *pointer;
    stack.push(node);

    while (!stack.empty()) {
        pointer = stack.top();
        if (pointer->left != nullptr) {
            CBinaryNode *left = pointer->left;
            pointer->left = nullptr;
            stack.push(left);
            continue;
        }
        if (pointer->right != nullptr) {
            CBinaryNode *right = pointer->right;
            pointer->right = nullptr;
            stack.push(right);
            continue;
        }
        stack.pop();
        delete pointer;
    }
}

template <typename T, typename Comparator>
void BinaryTree<T, Comparator>::insert(const T &object) {
    if (root == nullptr) {
        root = new CBinaryNode{object, nullptr, nullptr, nullptr};
    } else {
        CBinaryNode *pointer = root;
        while (pointer != nullptr) {
            if (cmp(object, pointer->Data)) {
                if (pointer->left == nullptr) break;
                pointer = pointer->left;
            } else {
                if (pointer->right == nullptr) break;
                pointer = pointer->right;
            }
        }
        CBinaryNode *data_object = new CBinaryNode{object, nullptr, nullptr, pointer};

        if (cmp(object, pointer->Data)) {
            pointer->left = data_object;
        } else {
            pointer->right = data_object;
        }
    }
}

template <typename T, typename Comparator>
void BinaryTree<T, Comparator>::levelBFS(CBinaryNode *node) {
    if (node == nullptr) return;
    std::deque<CBinaryNode *> deq;
    deq.push_back(node);
    T *element = nullptr;
    while (!deq.empty()) {
        CBinaryNode *node = deq.front();
        deq.pop_front();
        if (visit(node, element)) {
            std::cout << 0 << std::endl;
            return;
        };
        if (node->left != nullptr) {
            deq.push_back(node->left);
        }
        if (node->right != nullptr) {
            deq.push_back(node->right);
        }
    }
    std::cout << 1 << std::endl;

}

template <typename T, typename Comparator>
bool BinaryTree<T, Comparator>::visit(CBinaryNode *node, T *element) {
    if (element == nullptr) {
        element = &node->Data;
    }
    if (node->Data == *element) return true;
    return false;
}

template <typename T, typename Comparator>
bool BinaryTree<T, Comparator>::allNodeSame(CBinaryNode *root) {
    if (root == nullptr) return true;
    return checkSame(root, root->Data);
}

template <typename T, typename Comparator>
bool BinaryTree<T, Comparator>::checkSame(CBinaryNode *node, T value) {
    if (node == nullptr) {
        return true;
    }
    if (node->Data != value) return false;
    return checkSame(node->left, value) && checkSame(node->right, value);
}

void run() {
    BinaryTree<int, Comparator> btree;
    unsigned i;
    unsigned elem;
    while (std::cin >> elem) {
        btree.insert(elem);
    }
    std::cout << btree.allNodeSame(btree.GetRoot()) << std::endl;
}

int main() {
    run();
    return 0;
}