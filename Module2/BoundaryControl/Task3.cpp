#include <string>
#include <iostream>
#include <unordered_set>

struct Node {
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node &other) const {
        return product_name == other.product_name && color == other.color && size == other.size;
    }
    Node(std::string str, int col, int sz) : product_name{str}, color{col}, size{sz} {};
};

namespace std {
    template<>
        struct hash<Node> {
            std::size_t operator()(const Node &node) const noexcept {
                return std::hash<std::string>{}(node.product_name);
            }
        };
}

void run() {
    char command;
    std::string str;
    int color, size;
    std::unordered_set<Node> unique;
    
    while (std::cin >> command >> str >> color >> size) {
        Node buf{str, color, size};
        switch (command) {
            case '+':
                if (unique.contains(buf)) {
                    std::cout << "FAIL" << std::endl;
                } else {
                    unique.insert(buf);
                    std::cout << "OK" << std::endl;
                }
                break;
            case '-':
                if (!unique.contains(buf)) {
                    std::cout << "FAIL" << std::endl;
                } else {
                    unique.erase(buf);
                    std::cout << "OK" << std::endl;
                }
                break;
            case '?':
                if (unique.contains(buf)) {
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "FAIL" << std::endl;
                }
                break;
            default:
                break;
        }
    }
}

int main() {
    run();
    return 0;
}