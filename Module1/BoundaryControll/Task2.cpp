#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class DynamicArray {
   public:
    DynamicArray() : data{nullptr}, sz{0}, cap{0} {}
    ~DynamicArray() { delete[] data; }
    DynamicArray(const DynamicArray& other) = delete;

    DynamicArray& operator=(const DynamicArray& other) {
        unsigned i;
        if (this == &other) return *this;
        T* new_data = new T[other.cap];
        for (i = 0; i < other.sz; i++) {
            new_data[i] = other.data[i];
        }
        delete[] data;
        data = new_data;
        sz = other.sz;
        cap = other.cap;
        return *this;
    }

    void push_back(const T& value) {
        if (check_space()) {
            allocate_memory();
        }
        data[sz++] = value;
    }

    T pop() {
        if (!empty()) {
            return data[--sz];
        }
        throw std::out_of_range("Pop from empty array");
    }

    T peek_last() {
        if (!empty()) {
            return data[sz - 1];
        } 
        throw std::out_of_range("Peek from empty array");
    }

    T& operator[](const unsigned index) const { return data[index]; }

    unsigned size() const { return sz; }

    unsigned capacity() const { return cap; }

    bool empty() const { return sz == 0; }

    void print_array() {
        unsigned i;
        for (i = 0; i < size(); i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

   private:
    T* data;
    unsigned sz;
    unsigned cap;

    bool check_space() { return sz == cap; }

    void allocate_memory() {
        unsigned i;
        unsigned new_cap = (cap == 0) ? 1 : 2 * cap;

        T* new_data = new T[new_cap];

        for (i = 0; i < sz; i++) {
            new_data[i] = data[i];
        }
        delete[] data;

        data = new_data;
        cap = new_cap;
    }
};

void run(std::string first, std::string second) {
    DynamicArray<char> stack;
    int i, k = 0;
    for (i = 0; i < first.size(); i++) {
        stack.push_back(first[i]);

        while (!stack.empty() && stack.peek_last() == second[k]) {
            stack.pop();
            k++;
        }
    }

    if (k == second.size()) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
}

int main() {
    std::string word1, word2;
    std::cin >> word1 >> word2;
    run(word1, word2);
    return 0;
}