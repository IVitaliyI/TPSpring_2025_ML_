#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>

template <typename T, typename Compare = std::less<T>>
class MergeSort {
   public:
    void operator()(T* data, unsigned size);

   private:
    Compare cmp = Compare();
    void Merge(T* first, unsigned first_len, T* second, unsigned second_len, T* result);
};

template <typename T, typename Compare>
void MergeSort<T, Compare>::operator()(T* data, unsigned size) {
    int first_len, second_len;
    if (size <= 1) {
        return;
    }
    first_len = size / 2;
    second_len = size - first_len;
    MergeSort<T, Compare>()(data, first_len);
    MergeSort<T, Compare>()(data + first_len, second_len);
    T* sorting = new T[size];
    Merge(data, first_len, data + first_len, second_len, sorting);
    std::copy(sorting, sorting + size, data);
    delete[] sorting;
}

template <typename T, typename Compare>
void MergeSort<T, Compare>::Merge(T* first, unsigned first_len, T* second, unsigned second_len,
                                  T* result) {
    unsigned i = 0, j = 0, k = 0;
    while (i < first_len && j < second_len) {
        if (cmp(first[i], second[j])) {
            result[k++] = first[i++];
        } else {
            result[k++] = second[j++];
        }
    }
    while (i < first_len) {
        result[k++] = first[i++];
    }
    while (j < second_len) {
        result[k++] = second[j++];
    }
}

struct Comparator {
    bool operator()(const std::pair<unsigned, unsigned> left,
                    const std::pair<unsigned, unsigned> right) const {
        if (left.second == right.second) {
            return std::less<unsigned>{}(left.first, right.first);
        } else {
            return std::less<unsigned>{}(left.second, right.second);
        }
    }
};

class MainTask {
   private:
    std::pair<unsigned, unsigned>* data;
    unsigned size;

   public:
    void read_data(std::istream& input);
    void run(std::ostream& output);
    ~MainTask();
};

void MainTask::read_data(std::istream& input) {
    input >> size;
    data = new std::pair<unsigned, unsigned>[size];
    if (input.fail()) {
        std::cerr << "Problems with input" << std::endl;
        abort();
    }
    for (unsigned i = 0; i < size; i++) {
        input >> data[i].first >> data[i].second;
        if (input.fail()) {
            std::cerr << "Problems with input" << std::endl;
            abort();
        }
    }
}

void MainTask::run(std::ostream& output) {
    MergeSort<std::pair<unsigned, unsigned>, Comparator>()(data, size);
#if flag    
    for (unsigned i = 0; i < size; i++) {
        std::cout << data[i].first << " " << data[i].second << std::endl; 
    }
    std::cout << std::endl;
#endif
    unsigned count = 0;
    int last_ad = -2, second_last_ad = -2;

    for (unsigned i = 0; i < size; i++) {
        int a = data[i].first;
        int b = data[i].second;

        if (second_last_ad >= a) continue; 
        if (last_ad >= a) {
            second_last_ad = last_ad;
            last_ad = b;
            count++;
        } else {
            second_last_ad = b - 1;
            last_ad = b;
            count += 2;
        }
    }

    output << count << std::endl;
}

void test_case() {
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 10" << std::endl;
        input << "10 12" << std::endl;
        input << "1 10" << std::endl;
        input << "1 10" << std::endl;
        input << "23 24" << std::endl;
        std::cout << "Test 0" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "5\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "1" << std::endl;
        input << "1 5" << std::endl;
        std::cout << "Test 1" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "2\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 5" << std::endl;
        input << "1 5" << std::endl;
        input << "1 5" << std::endl;
        input << "1 5" << std::endl;
        input << "1 5" << std::endl;
        std::cout << "Test 2" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "2\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "4" << std::endl;
        input << "1 10" << std::endl;
        input << "2 5" << std::endl;
        input << "6 8" << std::endl;
        input << "7 10" << std::endl;
        std::cout << "Test 3" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "4\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 3" << std::endl;
        input << "1 4" << std::endl;
        input << "1 5" << std::endl;
        input << "1 6" << std::endl;
        input << "1 7" << std::endl;
        std::cout << "Test 5" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "2\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 3" << std::endl;
        input << "2 4" << std::endl;
        input << "3 5" << std::endl;
        input << "4 6" << std::endl;
        input << "5 7" << std::endl;
        std::cout << "Test 6" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "4\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "100000 100010" << std::endl;
        input << "100005 100015" << std::endl;
        input << "100010 100020" << std::endl;
        input << "100020 100030" << std::endl;
        input << "100030 100040" << std::endl;
        std::cout << "Test 7" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "5\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "100010 100020" << std::endl;
        input << "100030 100040" << std::endl;
        input << "100020 100030" << std::endl;
        input << "100000 100010" << std::endl;
        input << "100005 100015" << std::endl;
        std::cout << "Test 8" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "5\n");
        std::cout << "-> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "2 4" << std::endl;
        input << "1 3" << std::endl;
        input << "5 7" << std::endl;
        input << "4 6" << std::endl;
        input << "3 5" << std::endl;
        std::cout << "Test 9" << std::endl;
        MainTask task;
        task.read_data(input);
        task.run(output);
        assert(output.str() == "4\n");
        std::cout << "-> OK" << std::endl;
    }
}

MainTask::~MainTask() { delete[] data; }



int main() {

#define flag true
#if flag
    MainTask task;
    task.read_data(std::cin);
    task.run(std::cout);
#endif 

#if !flag   
    test_case();
#endif
    return 0;
}