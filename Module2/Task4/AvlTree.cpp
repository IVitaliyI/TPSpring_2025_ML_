#include <iostream>

#define WORK true

template <typename T, typename Comparator = std::less<T>>
class AvlTree {
    struct Node {
        Node(const T &data) : data(data), left(nullptr), right(nullptr), height(1), index(1){};

        T data;
        Node *left;
        Node *right;
        size_t height;
        size_t index;
    };

   public:
    AvlTree() : root(nullptr) {}
    ~AvlTree() { destroyTree(root); }

    void Add(const T &data) { root = addInternal(root, data); }

    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp) {
            if (tmp->data == data)
                return true;
            else if (tmp->data < data)
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }
    void Delete(const T &data) { root = deleteInternal(root, data); }

    void DeleteIndex(size_t k) { root = deleteIndexInternal(root, k); }

    unsigned AddIndex(const T &data) {
        auto [new_root, idx] = addInternalWithIndex(root, data, 0);
        root = new_root;
        return idx;
    }

    void Print() const { printInternal(root, "", true); }

   private:
    Node *root;
    Comparator cmp = Comparator();

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node *deleteInternal(Node *node, const T &data) {
        if (!node) {
            return nullptr;
        }
        if (cmp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else if (cmp(data, node->data))
            node->left = deleteInternal(node->left, data);
        else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right) return left;

            Node *min = nullptr;
            right = findAndRemoveMin(right, min);
            min->right = right;
            min->left = left;

            return doBalance(min);
        }
        return doBalance(node);
    }

    Node *deleteIndexInternal(Node *node, const size_t k) {
        if (!node) return nullptr;

        size_t left_index = getIndex(node->left);

        if (k < left_index) {
            node->left = deleteIndexInternal(node->left, k);
        } else if (k > left_index) {
            node->right = deleteIndexInternal(node->right, k - left_index - 1);
        } else {
            node = deleteInternal(node, node->data);
        }
        if (!node) return nullptr;
        return doBalance(node);
    }

    Node *findAndRemoveMin(Node *node, Node *&min) {
        if (!node->left) {
            min = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, min);
        return doBalance(node);
    }

    Node *addInternal(Node *node, const T &data) {
        if (!node) return new Node(data);
        if (!cmp(data, node->data))
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);

        return doBalance(node);
    }

    std::pair<Node *, unsigned> addInternalWithIndex(Node *node, const T &data, unsigned stat) {
        if (!node) return {new Node(data), stat};

        if (!cmp(data, node->data)) {
            auto [new_right, idx] =
                addInternalWithIndex(node->right, data, stat + getIndex(node->left) + ((cmp(node->data, data) ? 1 : 0)));
            node->right = new_right;
            node = doBalance(node);
            return {node, idx};
        } else {
            auto [new_left, idx] = addInternalWithIndex(node->left, data, stat);
            node->left = new_left;
            node = doBalance(node);
            return {node, idx};
        }
    }

    size_t getHeight(Node *node) { return node ? node->height : 0; }

    size_t getIndex(Node *node) { return node ? node->index : 0; }

    void fixHeight(Node *node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void fixIndex(Node *node) { node->index = getIndex(node->left) + getIndex(node->right) + 1; }

    int getBalance(Node *node) { return getHeight(node->right) - getHeight(node->left); }

    Node *rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixIndex(node);
        fixHeight(tmp);
        fixIndex(tmp);
        return tmp;
    }

    Node *rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixIndex(node);
        fixHeight(tmp);
        fixIndex(tmp);
        return tmp;
    }

    Node *doBalance(Node *node) {
        fixHeight(node);
        fixIndex(node);
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0) node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0) node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    void printInternal(Node *node, const std::string &prefix, bool isTail) const {
        if (node) {
            std::cout << prefix << (isTail ? "\\-- " : "|-- ") << node->data
                      << " (h=" << node->height << ")" << " (i=" << node->index << ")" << std::endl;
            printInternal(node->right, prefix + (isTail ? "    " : "|   "), false);
            printInternal(node->left, prefix + (isTail ? "    " : "|   "), true);
        }
    }
};

void run(std::istream &input, std::ostream &output) {
    unsigned command_count, i;
    unsigned command_type, element;
    input >> command_count;
    AvlTree<unsigned, std::greater<unsigned>> tree;

#if WORK
    if (input.fail()) {
        std::cerr << "Problem with command count input" << std::endl;
        abort();
    }
#endif

    for (i = 0; i < command_count; i++) {
        input >> command_type >> element;
        #if WORK
        if (input.fail()) {
            std::cerr << "Problem with input" << std::endl;
            abort();
        }
        #endif

        switch (command_type) {
            case 1:
                std::cout << tree.AddIndex(element) << std::endl;
                break;
            case 2:
                tree.DeleteIndex(element);
                break;
        }
    }
    // tree.Print();
}

int main(int argc, const char *argv[]) {
    run(std::cin, std::cout);
    return 0;
}
