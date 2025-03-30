#include <iostream>
#include <functional>
#include <assert.h>

template <typename T>
class DynamicArray {
public:
    DynamicArray() : data{nullptr}, sz{0}, cap{0} {}
    ~DynamicArray() {delete[] data;}
    DynamicArray(const DynamicArray &other) = delete;

    DynamicArray &operator=(const DynamicArray &other) {
        unsigned i;
        for (i = 0; i < other.size(); i++) {
            this->push_back(other[i]);
        }
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

    T& operator[](const unsigned index) const {
        return data[index];
    }

    unsigned size() const {
        return sz;
    }

    unsigned capacity() const {
        return cap;
    }

    bool empty() const {return sz == 0;}

    // Not need
    void insert(T element, int index) {
        if (check_space()) {
            allocate_memory(index);
        } else {
            shift_index(data, index);
        }
        data[index] = element;
        sz++;
    }

    void swap_elements(const unsigned left_index, const unsigned right_index) {
        std::swap(data[left_index], data[right_index]);
    }

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

    bool check_space() {
        return sz == cap;
    }

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

    void shift_index(T* new_data, const unsigned index) {
        int i;
        for (i = sz - 1; i >= 0; --i) {
            if (i >= index) {
                new_data[i + 1] = data[i];
            } else {
                new_data[i] = data[i];
            }
        }
    }
    
    void allocate_memory(const unsigned index) {
        unsigned i;
        unsigned new_cap = (cap == 0) ? 1 : 2 * cap;
        
        T* new_data = new T[new_cap];
        shift_index(new_data, index);
        delete[] data;

        data = new_data;
        cap = new_cap; 
    }
};


template <typename T, typename Compare = std::less<T>>
class MinHeap
{
private:
    DynamicArray<T> MinHeapData;
    Compare cmp;

    void build_MinHeap();
    void siftDown(int i);
    void siftUp(int i);
public:
    MinHeap(/* args */);
    explicit MinHeap(const DynamicArray<T>& array);
    ~MinHeap();

    void insert(T element);
    T extract_min();
    T peek_min();
};

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap() : cmp(Compare()) {}

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap(const DynamicArray<T>& array) {
    MinHeapData = array;
    build_MinHeap();
}

template <typename T, typename Compare>
T MinHeap<T, Compare>::extract_min() {
    if (MinHeapData.empty()) {
        throw std::out_of_range("MinHeap is empty!");
    }

    T buf = MinHeapData[0];
    MinHeapData[0] = MinHeapData.pop();
    if (!MinHeapData.empty()) {
        siftDown(0);
    }
    return buf;
}

template <typename T, typename Compare>
T MinHeap<T, Compare>::peek_min() {
    if (MinHeapData.empty()) {
        throw std::out_of_range("MinHeap is empty");
    }
    return MinHeapData[0];
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::insert(T element) {
    MinHeapData.push_back(element);
    siftUp(MinHeapData.size() - 1);
}

template <typename T, typename Compare>
MinHeap<T, Compare>::~MinHeap() = default;

template <typename T, typename Compare>
void MinHeap<T, Compare>::build_MinHeap() {
    int n = MinHeapData.size();
    int i;
    for (i = n / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::siftDown(int i) {
    unsigned size = MinHeapData.size();
    unsigned left, right, largest;

    while (true) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        largest = i;

        if (left < size && !(this->cmp(MinHeapData[largest], MinHeapData[left]))) {
            largest = left;
        }
        if (right < size && !this->cmp(MinHeapData[largest], MinHeapData[right])) {
            largest = right;
        }
        if (largest == i) return;

        std::swap(MinHeapData[i], MinHeapData[largest]);
        i = largest;
    }
    return;
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::siftUp(int i) {
    unsigned parent;
    while (i > 0) {
        parent = (i - 1) / 2;
        if (this->cmp(MinHeapData[i], MinHeapData[parent])) {
            std::swap(MinHeapData[i], MinHeapData[parent]);
            i = parent;
        } else {
            break;
        }
    }
    return;
}

struct MyComparator {
    bool operator()(const DynamicArray<int> &a, const DynamicArray<int> &b) const {
        return std::less<int>{}(a[0], b[0]);
    }
};

template <typename T>
class MainTask {
public:
    MainTask() : data{nullptr} {}
    ~MainTask() {delete[] data;}
    MainTask(const MainTask<T> &other) = delete;

    void generate_data(std::istream& input);
    void merge_data(std::ostream& output);

private:
    MinHeap<DynamicArray<T>, MyComparator> data;
};

template <typename T>
void MainTask<T>::merge_data(std::ostream &output) {
    
}

template <typename T>
void MainTask<T>::generate_data(std::istream& input) {
    int n_array;
    int n_elem;
    int i, j;
    T elem;
    input >> n_array;
        
    for (i = 0; i < n_array; i++) {
        DynamicArray<T> darr;
        input >> n_elem;
        for (j = 0; j < n_elem; j++) {
            input >> elem;
            darr.push_back(elem);
        }
        data.insert(darr);
        delete darr;
    }
};

int main() {

#if 0
    DynamicArray<int> tt;
    tt.push_back(5);
    tt.push_back(5);
    tt.push_back(3);
    tt.push_back(5);
    tt.push_back(2);
    tt.push_back(1);
    tt.print_array();
    std::cout << tt.capacity() << " " << tt.size() << " " << tt.pop() << std::endl;
    std::cout << tt.capacity() << " " << tt.size() << " " << tt.pop() << std::endl;
    tt.print_array();
    tt.insert(5, 2);
    tt.print_array();
#endif
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(5);
    arr.push_back(30);
    arr.push_back(25);
    arr.push_back(40);
    
    MinHeap<int> MinHeap1;
    MinHeap1.insert(10);
    MinHeap1.insert(20);
    MinHeap1.insert(5);
    MinHeap1.insert(30);
    MinHeap1.insert(25);
    MinHeap1.insert(40);
    
    assert(MinHeap1.peek_min() == 5);
    
    assert(MinHeap1.extract_min() == 5);
    assert(MinHeap1.extract_min() == 10);
    assert(MinHeap1.extract_min() == 20);
    assert(MinHeap1.extract_min() == 25);
    assert(MinHeap1.extract_min() == 30);
    assert(MinHeap1.extract_min() == 40);
    
    try {
        MinHeap1.extract_min();
        assert(false);
    } catch (const std::out_of_range &e) {
        std::cout << "MinHeap is empty: " << e.what() << std::endl;
    }
    
    MinHeap<int> MinHeap2(arr);
    
    assert(MinHeap2.extract_min() == 5);
    assert(MinHeap2.extract_min() == 10);
    assert(MinHeap2.extract_min() == 20);
    assert(MinHeap2.extract_min() == 25);
    assert(MinHeap2.extract_min() == 30);
    assert(MinHeap2.extract_min() == 40);

    std::cout << "All MinHeap tests passed successfully!" << std::endl;
    return 0;

}