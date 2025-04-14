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

class Box {
public:
    int x, y, z, index;
    Box(int x, int y, int z, int index) : x{x}, y{y}, z{z}, index{index} {};
    Box() : x(0), y(0), z(0), index(-1) {}

    bool operator<(const Box& other) const {
        return (x < other.x || y < other.y || z < other.z);
    }
};

void insertion_sort(DynamicArray<Box>& boxes) {
    for (unsigned i = 1; i < boxes.size(); ++i) {
        Box current = boxes[i];
        int j = i - 1;
        while (j >= 0 && !(boxes[j] < current)) {
            boxes[j + 1] = boxes[j];
            --j;
        }
        boxes[j + 1] = current;
    }
}

void run() {
    int n;
    std::cin >> n;
    
    DynamicArray<Box> boxes;
    for (int i = 0; i < n; ++i) {
        int x, y, z;
        std::cin >> x >> y >> z;
        boxes.push_back(Box(x, y, z, i));
    }
    insertion_sort(boxes);
    for (unsigned i = 0; i < boxes.size(); ++i) {
        std::cout << boxes[i].index << " ";
    }
    std::cout << std::endl;
}

int main() {
    run();
    return 0;
}