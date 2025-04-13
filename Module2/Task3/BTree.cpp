#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

#define NIL std::nullopt

struct BTreeNode {
    unsigned n_keys;
    std::vector<unsigned> keys;
    std::vector<BTreeNode*> childs;
    bool LEAF;
};

template <typename T>
class BTree {
    private:
        unsigned t = 5;
        BTreeNode *root;
        BTreeNode* create_node(bool leaf = true);
    public:
        BTree();
        std::optional<std::pair<BTreeNode*, unsigned>> BTreeSearch(BTree *btree, T key);
        void BTreeChildSplit(BTreeNode *X, unsigned i);
};

template <typename T>
BTreeNode* BTree<T>::create_node(bool leaf = true) {
    BTreeNode *TreeNode = new BTreeNode();
    TreeNode->LEAF = leaf;
    TreeNode->n_keys = 0;
    TreeNode->keys.reserve(2 * t - 1);
    TreeNode->childs.reserve(2 * t);
    return TreeNode;
}

template <typename T>
BTree<T>::BTree() {
    BTreeNode *TreeNode = create_node(true);
    root = TreeNode;
}

template <typename T>
std::optional<std::pair<BTreeNode*, unsigned>> BTree<T>::BTreeSearch(BTree *btree, T key) {
    unsigned i = 1;
    BTreeNode *pointer = btree->root;
    while ((i <= pointer->n_keys) && (key > pointer->keys[i])) i++;
    if ((i <= pointer->n_keys) && key == pointer->keys[i]) 
        return std::pair<BTreeNode*, unsigned>{pointer, i};
    else if (pointer->LEAF) return NIL;
    return BTreeSearch(pointer->childs[i], key);
}

template <typename T>
void BTree<T>::BTreeChildSplit(BTreeNode* X, unsigned i) {
    unsigned j;
    BTreeNode *Y = X->childs[i];
    BTreeNode *Z = create_node(Y->LEAF);
    Z->n_keys = t - 1;
    for (j = 0; j < Z->n_keys; j++) {
        Z->keys[j] = Y->keys[j + t];
    }

    if (!Y->LEAF) {
        for (j = 0; j < t; j++) {
            Z->childs[j] = Y->childs[j + t];
        }
        Y->n_keys = t - 1;
    }

    for (j = X->n_keys - 1; j >= i; j--) {
        if (j == 0u - 1) break;  // мб в int?
        X->childs[j + 1] = X->childs[j]; 
    }
    X->childs[i + 1] = Z;
    for (j = X->n_keys - 1; j >= i) 
}

int main() {
    return 0;    
}
