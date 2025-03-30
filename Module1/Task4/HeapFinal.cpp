#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>

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

    T& operator[](const unsigned index) const { return data[index]; }

    unsigned size() const { return sz; }

    unsigned capacity() const { return cap; }

    bool empty() const { return sz == 0; }

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

template <typename T, typename Compare = std::less<T>>
class MinHeap {
   public:
    MinHeap();
    ~MinHeap();
    explicit MinHeap(const DynamicArray<T>& array);
    void insert(T element);
    T extract();
    T peek();
    bool empty();

   private:
    DynamicArray<T> data;
    Compare cmp;
    void build_heap();
    void siftDown(int i);
    void siftUp(int i);
};

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap() : cmp(Compare()){};

template <typename T, typename Compare>
MinHeap<T, Compare>::~MinHeap() = default;

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap(const DynamicArray<T>& array) : cmp(Compare()) {
    data = array;
    build_heap();
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::insert(T element) {
    data.push_back(element);
    siftUp(data.size() - 1);
}

template <typename T, typename Compare>
T MinHeap<T, Compare>::extract() {
    if (data.empty()) {
        throw std::out_of_range("Heap is empty.");
    }
    T buf = data[0];
    data[0] = data.pop();
    if (data.size() > 1) {
        siftDown(0);
    }
    return buf;
}

template <typename T, typename Compare>
T MinHeap<T, Compare>::peek() {
    if (data.empty()) {
        throw std::out_of_range("Heap is empty.");
    }
    return data[0];
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::build_heap() {
    if (data.empty()) {
        throw std::out_of_range("Array is empty.");
    }
    int n = data.size();
    int i;
    for (i = n / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::siftDown(int i) {
    int size = data.size();
    int left, right, smallest;

    while (true) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;

        if (left < size && cmp(data[left], data[smallest])) {
            smallest = left;
        }
        if (right < size && cmp(data[right], data[smallest])) {
            smallest = right;
        }
        if (smallest == i) return;

        std::swap(data[i], data[smallest]);
        i = smallest;
    }
    return;
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::siftUp(int i) {
    int parent;
    while (i > 0) {
        parent = (i - 1) / 2;
        if (this->cmp(data[i], data[parent])) {
            std::swap(data[i], data[parent]);
            i = parent;
        } else
            break;
    }
    return;
}

template <typename T, typename Compare>
bool MinHeap<T, Compare>::empty() {
    return data.empty();
}

struct MyComparator {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        return a.first < b.first;
    }
};

struct InputData {
    int* array = nullptr;
    int size = 0;
    int index = 0;
};

class MainTask {
   public:
    void read_instream(std::istream& input);
    void merge_arrays(std::ostream& output);
    ~MainTask();

   private:
    MinHeap<std::pair<int, int>, MyComparator> heap;
    InputData* data;
    int size_data;
};

void MainTask::read_instream(std::istream& input) {
    int n_array, n_elems, elem;
    input >> n_array;
    data = new InputData[n_array];
    size_data = n_array;

    for (int i = 0; i < n_array; i++) {
        input >> n_elems;
        data[i].array = new int[n_elems];
        data[i].size = n_elems;

        for (int j = 0; j < n_elems; j++) {
            input >> elem;
            data[i].array[j] = elem;
        }
    }
}

void MainTask::merge_arrays(std::ostream& output) {
    for (int i = 0; i < size_data; i++) {
        if (data[i].size > 0) {
            heap.insert({data[i].array[0], i});
            data[i].index = 1;
        }
    }

    while (!heap.empty()) {
        auto [value, array_idx] = heap.extract();
        output << value << " ";

        if (data[array_idx].index < data[array_idx].size) {
            heap.insert({data[array_idx].array[data[array_idx].index], array_idx});
            data[array_idx].index++;
        }
    }
    output << std::endl;
}

MainTask::~MainTask() {
    for (int i = 0; i < size_data; i++) {
        delete[] data[i].array;
    }
    delete[] data;
}

void run(std::istream& input, std::ostream& output) {
    MainTask run;
    run.read_instream(input);
    run.merge_arrays(output);
}

void test_case() {
    // Test 1: Simple case with 3 arrays of different sizes
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "1" << std::endl;
        input << "6" << std::endl;
        input << "2" << std::endl;
        input << "50 90" << std::endl;
        input << "3" << std::endl;
        input << "1 10 70" << std::endl;
        std::cout << "Test 1" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 2: Edge case with no elements
    {
        std::stringstream input, output;
        input << "1" << std::endl;
        input << "0" << std::endl;  // Empty array
        std::cout << "Test 2" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 3: Arrays with duplicate values
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "3" << std::endl;
        input << "1 1 1" << std::endl;
        input << "2" << std::endl;
        input << "1 1" << std::endl;
        std::cout << "Test 3" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 4: Single large array
    {
        std::stringstream input, output;
        input << "1" << std::endl;
        input << "5" << std::endl;
        input << "10 20 30 40 50" << std::endl;
        std::cout << "Test 4" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 5: Arrays with very large numbers
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "3" << std::endl;
        input << "1000 10000 100000" << std::endl;
        input << "3" << std::endl;
        input << "500 20000 250000" << std::endl;
        std::cout << "Test 5" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 6: All arrays have the same size and values
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "1 2 3" << std::endl;
        input << "3" << std::endl;
        input << "1 2 3" << std::endl;
        input << "3" << std::endl;
        input << "1 2 3" << std::endl;
        std::cout << "Test 6" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 7: One array with multiple elements and the rest empty
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0" << std::endl;  // Empty array
        input << "0" << std::endl;  // Empty array
        std::cout << "Test 7" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 8: Reverse order
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "3" << std::endl;
        input << "3 4 5" << std::endl;
        input << "3" << std::endl;
        input << "6 7 8" << std::endl;
        std::cout << "Test 8" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 9: Arrays with some negative numbers
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "-3 -2 -1" << std::endl;
        input << "2" << std::endl;
        input << "-6 -5" << std::endl;
        input << "4" << std::endl;
        input << "-7 -4 3 9" << std::endl;
        std::cout << "Test 9" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }

    // Test 10: Test with increasing order in each array
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "1 2 3" << std::endl;
        input << "2" << std::endl;
        input << "4 5" << std::endl;
        input << "4" << std::endl;
        input << "6 7 8 9" << std::endl;
        std::cout << "Test 10" << std::endl;
        run(input, output);
        std::cout << output.str() << std::endl;
    }
}

void test_min_heap() {
    // Test 1: Creating an empty heap and checking if it's empty
    {
        MinHeap<int> heap;
        assert(heap.empty() == true);
        std::cout << "Test 1 Passed: Empty heap initialized successfully." << std::endl;
    }

    // Test 2: Inserting elements and checking the minimum
    {
        MinHeap<int> heap;
        heap.insert(10);
        heap.insert(5);
        heap.insert(15);
        heap.insert(3);
        assert(heap.peek() == 3);  // Min element should be 3
        std::cout << "Test 2 Passed: Insert operation and peek are correct." << std::endl;
    }

    // Test 3: Extracting elements while maintaining min-heap property
    {
        MinHeap<int> heap;
        heap.insert(10);
        heap.insert(5);
        heap.insert(15);
        heap.insert(3);
        assert(heap.extract() == 3);  // Min element should be extracted first
        assert(heap.extract() == 5);
        assert(heap.extract() == 10);
        assert(heap.extract() == 15);
        assert(heap.empty() == true);
        std::cout << "Test 3 Passed: Extract operation maintains heap property." << std::endl;
    }

    // Test 4: Peeking and extracting from an empty heap (Edge Case)
    {
        MinHeap<int> heap;
        try {
            heap.peek();
            assert(false);  // Should not reach here
        } catch (...) {
            std::cout << "Test 4 Passed: Peek on empty heap throws an exception." << std::endl;
        }

        try {
            heap.extract();
            assert(false);  // Should not reach here
        } catch (...) {
            std::cout << "Test 4 Passed: Extract on empty heap throws an exception." << std::endl;
        }
    }
    {
        const int SIZE = 1000;
        MinHeap<int> heap;
        std::vector<int> elements(SIZE);

        // Generate values in reverse order to make it challenging for heap
        for (int i = 0; i < SIZE; i++) {
            elements[i] = SIZE - i;
        }

        // Insert all elements into the heap
        for (int num : elements) {
            heap.insert(num);
        }

        // Ensure peek() gives the smallest element
        assert(heap.peek() == 1);
        std::cout << "Test Large Heap - Peek Passed" << std::endl;

        // Extract elements and ensure they are sorted in ascending order
        for (int i = 1; i <= SIZE; i++) {
            assert(heap.extract() == i);
        }

        // Heap should be empty after all extractions
        assert(heap.empty() == true);
        std::cout << "Test Large Heap - Extraction Passed" << std::endl;
    }
}

int main() {
    run(std::cin, std::cout);
#if 0
    // test_case();
    // test_min_heap();
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
    
    assert(MinHeap1.peek() == 5);
    
    assert(MinHeap1.extract() == 5);
    assert(MinHeap1.extract() == 10);
    assert(MinHeap1.extract() == 20);
    assert(MinHeap1.extract() == 25);
    assert(MinHeap1.extract() == 30);
    assert(MinHeap1.extract() == 40);
    
    try {
        MinHeap1.extract();
        assert(false);
    } catch (const std::out_of_range &e) {
        std::cout << "MinHeap is empty: " << e.what() << std::endl;
    }
    
    MinHeap<int> MinHeap2(arr);
    
    assert(MinHeap2.extract() == 5);
    assert(MinHeap2.extract() == 10);
    assert(MinHeap2.extract() == 20);
    assert(MinHeap2.extract() == 25);
    assert(MinHeap2.extract() == 30);
    assert(MinHeap2.extract() == 40);

    std::cout << "All MinHeap tests passed successfully!" << std::endl;
#endif
    return 0;
}