#include <cassert>
#include <iostream>
#include <sstream>
#include <optional>
#include <vector>
#include <deque>

#define NIL std::nullopt

struct BTreeNode {
    unsigned n_keys;
    std::vector<unsigned> keys;
    std::vector<BTreeNode*> childs;
    bool LEAF;
};

template <typename T, typename Comparator = std::less<T>>
class BTree {
    private:
        unsigned t;
        BTreeNode *root;
        BTreeNode* create_node(bool leaf = true);
        Comparator cmp = Comparator();
        void BTreeInsertNonFull(BTreeNode *node, unsigned key);
        std::optional<std::pair<BTreeNode*, unsigned>> BTreeSearch(BTreeNode *node, T key);
        void BTreeChildSplit(BTreeNode *X, unsigned i);
    public:
        void BTreeInsert(unsigned key);
        BTree(unsigned t);
        std::optional<std::pair<BTreeNode*, unsigned>> BTreeSearch(T key) {
            return BTreeSearch(this->root, key);
        }

    void Print();
    void visit(BTreeNode *node);

};

template <typename T, typename Comparator>
BTreeNode* BTree<T, Comparator>::create_node(bool leaf) {
    BTreeNode *TreeNode = new BTreeNode();
    TreeNode->LEAF = leaf;
    TreeNode->n_keys = 0;
    TreeNode->keys.resize(2 * t - 1);
    TreeNode->childs.resize(2 * t, nullptr);
    return TreeNode;
}

template <typename T, typename Comparator>
BTree<T, Comparator>::BTree(unsigned t) {
    BTreeNode *TreeNode = create_node(true);
    root = TreeNode;
    this->t = t;
}

template <typename T, typename Comparator>
std::optional<std::pair<BTreeNode*, unsigned>> BTree<T, Comparator>::BTreeSearch(BTreeNode *node, T key) {
    unsigned i = 0;
    while (i < node->n_keys && cmp(node->keys[i], key)) i++;
    if (i < node->n_keys && !cmp(key, node->keys[i]) && !cmp(node->keys[i], key))
        return std::make_pair(node, i);
    else if (node->LEAF)
        return NIL;
    else
        return BTreeSearch(node->childs[i], key);
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::BTreeChildSplit(BTreeNode* X, unsigned i) {
    unsigned j;
    int k;
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
    }

    Y->n_keys = t - 1;

    for (k = static_cast<int>(X->n_keys) - 1; k >= static_cast<int>(i); k--) {
        X->childs[k + 1] = X->childs[k]; 
    }

    X->childs[i + 1] = Z;
    for (k = static_cast<int>(X->n_keys) - 1; k >= static_cast<int>(i); k--) {
        X->keys[k + 1] = X->keys[k]; 
    }
    X->keys[i] = Y->keys[t - 1];
    X->n_keys++;
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::BTreeInsert(unsigned key) {
    BTreeNode *root_base = this->root;
    if (root->n_keys == 2 * t - 1) {
        BTreeNode *new_root = create_node(false);
        new_root->childs[0] = root_base;
        this->root = new_root;
        // new_root->n_keys = 1;
        BTreeChildSplit(new_root, 0);
        BTreeInsertNonFull(new_root, key);
    } else {
        BTreeInsertNonFull(root, key);
    }
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::BTreeInsertNonFull(BTreeNode *node, unsigned key) {
    int j = node->n_keys - 1;
    if (node->LEAF) {
        while (j >= 0 && cmp(key, node->keys[j])) {
            node->keys[j + 1] = node->keys[j];
            j--;
        }
        node->keys[j + 1] = key;
        node->n_keys++;
    } else {
        while (j >= 0 && cmp(key, node->keys[j])) {
            j--;
        }
        j++;
        BTreeNode *child = node->childs[j];
        if (child->n_keys == 2 * t - 1) {
            BTreeChildSplit(node, j);
            if (cmp(node->keys[j], key)) {
                j++;
            }
        }
        BTreeInsertNonFull(node->childs[j], key);
    }
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::Print() {
    if (root == nullptr) return;

    std::deque<std::pair<BTreeNode*, unsigned>> deq;
    deq.emplace_back(root, 0);
    unsigned current_level = 0;

    while (!deq.empty()) {
        auto [node, level] = deq.front();
        deq.pop_front();

        if (level != current_level) {
            std::cout << std::endl;
            current_level = level;
        }

        for (unsigned i = 0; i < node->n_keys; ++i) {
            std::cout << node->keys[i] << " ";
        }

        if (!node->LEAF) {
            for (unsigned i = 
                0; i <= node->n_keys; ++i) {
                if (node->childs[i])
                    deq.emplace_back(node->childs[i], level + 1);
            }
        }
    }

    std::cout << std::endl;
}


template <typename T, typename Comparator>
void BTree<T, Comparator>::visit(BTreeNode *node) {
    unsigned i;
    for (i = 0; i < node->n_keys; i++) {
        std::cout << node->keys[i] << " ";
    }
}

void run(std::istream &input, std::ostream &output) {
    unsigned count, i;
    int element;
    input >> count;
    // if (input.fail()) std::cerr << "Problem with input" << std::endl;
    BTree<int> Btree(count);
    std::vector<int> elements;
    while (input >> element) {
        Btree.BTreeInsert(element);
    }
    Btree.Print();
}

void test_case() {
    {
        std::stringstream input, output;
        input << 2 << std::endl;
        input << "0 1 2 3 4 5 6 7 8 9";
        run(input, std::cout);
    }
}

int main() {
    // test_child_split_clean();
    run(std::cin, std::cout);
    // test_case();
    return 0;    
}
