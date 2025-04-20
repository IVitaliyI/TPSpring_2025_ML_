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

template <typename T, typename Comparator = std::less<T>>
class BTree {
    private:
        unsigned t = 5;
        BTreeNode *root;
        BTreeNode* create_node(bool leaf = true);
        Comparator cmp = Comparator();
        void BTreeInsertNonFull(BTreeNode *node, unsigned key);
        std::optional<std::pair<BTreeNode*, unsigned>> BTreeSearch(BTreeNode *node, T key);
        void BTreeChildSplit(BTreeNode *X, unsigned i);
    public:
        void BTreeInsert(unsigned key);
        BTree();
        std::optional<std::pair<BTreeNode*, unsigned>> BTreeSearch(T key) {
            return BTreeSearch(this->root, key);
        }

    void PrintNode(BTreeNode *node, int level);
    void Print();

};

template <typename T, typename Comparator>
BTreeNode* BTree<T, Comparator>::create_node(bool leaf) {
    BTreeNode *TreeNode = new BTreeNode();
    TreeNode->LEAF = leaf;
    TreeNode->n_keys = 0;
    TreeNode->keys.resize(2 * t - 1);
    TreeNode->childs.resize(2 * t);
    return TreeNode;
}

template <typename T, typename Comparator>
BTree<T, Comparator>::BTree() {
    BTreeNode *TreeNode = create_node(true);
    root = TreeNode;
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


void test_child_split_clean() {
    BTree<unsigned> tree;

    // Вставляем 9 ключей — это переполнит корень (t = 5)
    for (unsigned i = 1; i <= 15; ++i) {
        tree.BTreeInsert(i);
    }

    std::cout << "Insert completed.\n";

    // Поиск ключа 8 — должен найтись
    auto result = tree.BTreeSearch(8);
    if (result) {
        std::cout << "Found key 8 at index " << result->second << " in node at address " << result->first << "\n";
    } else {
        std::cout << "Key 8 not found.\n";
    }

    // Для контроля выведем корень и его детей
    auto root = tree.BTreeSearch(8)->first;
    std::cout << "Root node keys: ";
    for (unsigned i = 0; i < root->n_keys; ++i)
        std::cout << root->keys[i] << " ";
    std::cout << "\n";

    tree.Print();
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::PrintNode(BTreeNode *node, int level) {
    if (node == nullptr) return;

    // Печатаем ключи текущего узла
    std::cout << std::string(level * 2, ' ');  // отступ для уровня
    std::cout << "[ ";
    for (unsigned i = 0; i < node->n_keys; ++i) {
        std::cout << node->keys[i] << " ";
    }
    std::cout << "]\n";

    // Если узел не является листом, рекурсивно печатаем детей
    if (!node->LEAF) {
        for (unsigned i = 0; i <= node->n_keys; ++i) {
            PrintNode(node->childs[i], level + 1);
        }
    }
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::Print() {
    PrintNode(this->root, 0);  // Начинаем с корня
}


#if 0
void test_child_split() {
        BTree<unsigned> tree;
    
        // Создаём переполненный узел
        BTreeNode* full_node = new BTreeNode();
        full_node->LEAF = true;
        full_node->n_keys = 9; // 2*t - 1 = 9, при t = 5
    
        full_node->keys.resize(2 * 5 - 1); // 9 элементов
        for (unsigned i = 0; i < 9; ++i) {
            full_node->keys[i] = i + 1;  // [1, 2, ..., 9]
        }
    
        // Обёртка — корень, в который добавим full_node как ребёнка
        BTreeNode* root = new BTreeNode();
        root->LEAF = false;
        root->n_keys = 0;
        root->childs.resize(10);
        root->keys.resize(9);

        root->childs[0] = full_node;
    
        // Подменяем корень у дерева
        tree.BTreeChildSplit(root, 0u);

        std::cout << "Split completed.\n";
        std::cout << "Root keys: ";
        for (unsigned i = 0; i < root->n_keys; ++i)
            std::cout << root->keys[i] << " ";
        std::cout << "\n";
    
        std::cout << "Left child keys: ";
        for (unsigned i = 0; i < root->childs[0]->n_keys; ++i)
            std::cout << root->childs[0]->keys[i] << " ";
        std::cout << "\n";
    
        std::cout << "Right child keys: ";
        for (unsigned i = 0; i < root->childs[1]->n_keys; ++i)
            std::cout << root->childs[1]->keys[i] << " ";
        std::cout << "\n";

        
        auto result = tree.BTreeSearch(8);
        if (result) {
            std::cout << "Found key 8 at index " << result->second << " in node with address " << result->first << "\n";
        } else {
            std::cout << "Key 8 not found.\n";
        }
        

}
#endif


int main() {
    test_child_split_clean();
    return 0;    
}
