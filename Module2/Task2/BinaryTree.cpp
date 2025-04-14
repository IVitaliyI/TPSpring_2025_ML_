#include <cassert>
#include <deque>
#include <iostream>
#include <vector>
#include <stack>

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
    void in_order(CBinaryNode *node);
    void levelBFS(CBinaryNode *node);
    void visit(CBinaryNode *node);

    CBinaryNode *GetRoot() { return root; }

    void destroy(CBinaryNode *node);

    ~BinaryTree() { destroy(root); }
};

template <typename T, typename Comparator>
void destroy(CBinaryNode *node) {
    if (node == nullptr) return;
    
    std::stack<CBinaryNode*> stack;
    CBinaryNode* pointer;
    stack.push(node);

    while (!stack.empty()) {
        pointer = stack.top();
        if (pointer->left != nullptr) {
            CBinaryNode* left = pointer->left;
            pointer->left = nullptr;
            stack.push(left);
            continue;
        }
        if (pointer->right != nullptr) {
            CBinaryNode* right = pointer->right;
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
    while (!deq.empty()) {
        CBinaryNode *node = deq.front();
        deq.pop_front();
        visit(node);
        if (node->left != nullptr) {
            deq.push_back(node->left);
        }
        if (node->right != nullptr) {
            deq.push_back(node->right);
        }
    }
}

template <typename T, typename Comparator>
void BinaryTree<T, Comparator>::in_order(CBinaryNode *node) {
    if (node == nullptr) {
        return;
    }
    in_order(node->left);
    visit(node);
    in_order(node->right);
}

template <typename T, typename Comparator>
void BinaryTree<T, Comparator>::visit(CBinaryNode *node) {
    std::cout << node->Data << " ";
    return;
}

void run() {
    BinaryTree<int, Comparator> btree;
    unsigned n_elems, i;
    unsigned elem;
    std::cin >> n_elems;
    for (i = 0; i < n_elems; i++) {
        std::cin >> elem;
        btree.insert(elem);
    }
    btree.levelBFS(btree.GetRoot());
}

int main() {
    run();
    return 0;
}