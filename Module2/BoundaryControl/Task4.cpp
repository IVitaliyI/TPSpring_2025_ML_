#include <iostream>

#define WORK true

template <typename T, typename Comparator = std::less<T>>
class AvlTree {
    struct Node {
        Node(const T &data) : data(data), left(nullptr), right(nullptr), height(1) {};

        T data;
        Node *left;
        Node *right;
        unsigned height;
    };

   public:
    AvlTree() : root(nullptr) {}
    ~AvlTree() { destroyTree(root); }

    void Add(const T &data) { root = addInternal(root, data); }

    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp) {
            if (!cmp(tmp->data, data) && !cmp(data, tmp->data))
                return true;
            else if (cmp(tmp->data, data))
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }
    
    void Delete(const T &data) { root = deleteInternal(root, data); }

    void Print() const { printInternal(root, "", true); }

    int next(int x) {
        Node* res = next(root, x);
        return res ? res->data : -1;
    }

    int prev(int x) {
        Node* res = prev(root, x);
        return res ? res->data : -1;
    }

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
            Node *newRight = findAndRemoveMin(right, min);
            
            min->left  = left;
            min->right = newRight;
            fixHeight(min);
            return doBalance(min);

        }
        return doBalance(node);
    }

    Node *findAndRemoveMin(Node *node, Node *&min) {
        if (!node->left) {
            min = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, min);
        fixHeight(node);
        return doBalance(node);
    }

    Node *addInternal(Node *node, const T &data) {
        if (!node)
            return new Node(data);
    
        if (!cmp(data, node->data) && !cmp(node->data, data)) {
            return node;
        }
        else if (cmp(data, node->data)) {
            node->left  = addInternal(node->left,  data);
        }
        else {
            node->right = addInternal(node->right, data);
        }
    
        return doBalance(node);
    }
    

    size_t getHeight(Node *node) { return node ? node->height : 0; }


    void fixHeight(Node *node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    int getBalance(Node *node) { return getHeight(node->right) - getHeight(node->left); }

    Node *rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node *rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node *doBalance(Node *node) {
        fixHeight(node);
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


    Node* prev(Node *node, T value) {
        Node *result = nullptr;
        while (node) {
            if (cmp(node->data, value)) {
                result = node; 
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return result;
    }

    Node* next(Node *node, T value) {
        Node *result = nullptr;
        while (node) {
            if (cmp(value, node->data)) {
                result = node;  
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return result;
    }
};


void run() {
    std::string command;
    int element;
    AvlTree<int> tree;

    while (std::cin >> command >> element) {
        if (command == "insert") {
            tree.Add(element);
        }
        else if (command == "delete") {
            tree.Delete(element);
        }
        else if (command == "exists") {
            std::cout << (tree.Has(element) ? "true\n" : "false\n");
        }
        else if (command == "next") {
            int res = tree.next(element);
            std::cout << (res == -1 ? "none\n" : std::to_string(res) + "\n");
        }
        else if (command == "prev") {
            int res = tree.prev(element);
            std::cout << (res == -1 ? "none\n" : std::to_string(res) + "\n");
        }
    }
}


int main(int argc, const char *argv[]) {
    run();
    return 0;
}