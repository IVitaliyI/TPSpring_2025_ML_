// 2_3. Даны два массива неубывающих целых чисел, упорядоченные по возрастанию.
//  A[0..n-1] и B[0..m-1]. n >> m.
// Найдите их пересечение.
// Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A..
// В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента
// B[i-1]. Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного
// поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный
// поиск.
#include <cassert>
#include <iostream>
#include <sstream>
#include <random>

short binary_search(const int* array, unsigned short left, unsigned short right,
                    const int element) {
    unsigned short middle;

    if (left > right) return -1;

    while (left < right) {
        middle = (left + right) / 2;
        if (array[middle] == element) return middle;
        if (array[middle] < element) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return ((left == right + 1) || (array[left] != element)) ? -1 : left;
}

short exponential_search(const int* array, unsigned short* left, const unsigned short nsize,
                         const int element) {
    
    unsigned short bound = 1;
    unsigned short right = *left;
    while ((right < nsize) && (array[right] < element)) {
        bound = bound << 1;
        *left = right + 1;
        right = *left + bound;
    }
    return binary_search(array, *left, std::min(right, (unsigned short)(nsize - 1)), element);
}

void task_main(const int* array_A, const unsigned short len_A, const int* array_B,
               const unsigned short len_B, std::ostream& output) {
    unsigned short i;
    short buf = -1;
    unsigned short index = 0;
    for (i = 0; i < len_B; i++) {
        if (buf != -1) {
            if (array_B[i] == array_B[i - 1]) {
                output << array_B[i] << " ";
                continue;
            }
        }
        buf = exponential_search(array_A, &index, len_A, array_B[i]);
        if (buf != -1) {
            output << array_A[buf] << " ";
        }
    }
}


void fill_int_array(int* array, unsigned short nsize, std::istream& input) {
    unsigned short i;
    for (i = 0; i < nsize; i++) {
        input >> array[i];
        if (input.fail()) {
            std::cerr << "Elements in array must be integer" << std::endl;
            abort();
        }
    }
}

void test_binary_search(std::istream& input, std::ostream& output) {
    int* array;
    unsigned short len, left, right;
    int element;
    input >> len;
    if (input.fail()) {
        std::cerr << "len must be unsigned short" << std::endl;
        abort();
    }
    array = (int*)malloc(len * sizeof(int));
    if (array == nullptr) {
        std::cerr << "Problems with memory allocation" << std::endl;
        abort();
    }
    fill_int_array(array, len, input);

    input >> left >> right >> element;
    if (input.fail()) {
        std::cerr << "Problems in left, right and element" << std::endl;
        abort();
    }
    output << binary_search(array, left, right, element);
    free(array);
}

void test_case_1() {
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 4 1" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 1: " << output.str();
        assert(output.str() == "0");
        std::cout << " -> OK" << std::endl;

    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 4 2" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 2: " << output.str();
        assert(output.str() == "1");
        std::cout << " -> OK" << std::endl;

    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 4 3" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 3: " << output.str();
        assert(output.str() == "2");
        std::cout << " -> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 4 5" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 4: " << output.str();
        assert(output.str() == "4");
        std::cout << " -> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 4 6" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 5: " << output.str();
        assert(output.str() == "-1");
        std::cout << " -> OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 4 0" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 6: " << output.str();
        assert(output.str() == "-1");
        std::cout << " -> OK" << std::endl;

    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "3 4 5" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 7: " << output.str();
        assert(output.str() == "4");
        std::cout << " -> OK" << std::endl;

    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 1 5" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 8: " << output.str();
        assert(output.str() == "-1");
        std::cout << " -> OK" << std::endl;

    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 0 1" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 9: " << output.str();
        assert(output.str() == "0");
        std::cout << " -> OK" << std::endl;

    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 0 2" << std::endl;
        test_binary_search(input, output);
        std::cout << "Test 10: " << output.str();
        assert(output.str() == "-1");
        std::cout << " -> OK" << std::endl;

    }
}

void run(std::istream& input, std::ostream& output) {
   
    int* array_A;
    int* array_B;
    unsigned short len_A, len_B;
    input >> len_A >> len_B;
    array_A = (int*)malloc(len_A * sizeof(int));
    if (array_A == nullptr) {
        std::cerr << "Error with memory allocation" << std::endl;
    }
    array_B = (int*)malloc(len_B * sizeof(int));
    if (array_B == nullptr) {
        std::cerr << "Error with memory allocation" << std::endl;
    }
    fill_int_array(array_A, len_A, input);
    fill_int_array(array_B, len_B, input);
    task_main(array_A, len_A, array_B, len_B, output);
    free(array_A);
    free(array_B);
}

void test_main() {
#if 1    
    std::cout << "--------Binary search tests--------" << std::endl;
    test_case_1();
    std::cout << "-----------------------------------" << std::endl;
#endif 
    std::cout << "--------Task tests-----------------" << std::endl;

    {
        std::stringstream input, output;
        input << "5 3" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "0 0 2" << std::endl;
        run(input, output);
        std::cout << "Test 1: " << output.str() << " -> ";
        assert(output.str() == "2 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5 3" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "1 2 3" << std::endl;
        run(input, output);
        std::cout << "Test 2: " << output.str() << " -> ";
        assert(output.str() == "1 2 3 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5 3" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "5 5 5" << std::endl;
        run(input, output);
        std::cout << "Test 3: " << output.str() << " -> ";
        assert(output.str() == "5 5 5 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5 0" << std::endl;
        input << "1 2 3 4 5" << std::endl;
        input << "" << std::endl;
        run(input, output);
        std::cout << "Test 4: " << output.str() << " -> ";
        assert(output.str() == "");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5 4" << std::endl;
        input << "1 2 2 3 4" << std::endl;
        input << "2 3 3 5" << std::endl;
        run(input, output);
        std::cout << "Test 5: " << output.str() << " -> ";
        assert(output.str() == "2 3 3 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "3 3" << std::endl;
        input << "1 2 3" << std::endl;
        input << "1 2 3" << std::endl;
        run(input, output);
        std::cout << "Test 6: " << output.str() << " -> ";
        assert(output.str() == "1 2 3 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5 5" << std::endl;
        input << "5 5 5 5 5" << std::endl;
        input << "5 5 5 5 5" << std::endl;
        run(input, output);
        std::cout << "Test 7: " << output.str() << " -> ";
        assert(output.str() == "5 5 5 5 5 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "10 5" << std::endl;
        input << "1 2 3 4 5 6 7 8 9 10" << std::endl;
        input << "-1 1 2 2 5" << std::endl;
        run(input, output);
        std::cout << "Test 8: " << output.str() << " -> ";
        assert(output.str() == "1 2 2 5 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "10 5" << std::endl;
        input << "1 1 1 1 1 1 1 1 1 1" << std::endl;
        input << "-1 1 2 2 5" << std::endl;
        run(input, output);
        std::cout << "Test 9: " << output.str() << " -> ";
        assert(output.str() == "1 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "10 5" << std::endl;
        input << "1 1 1 1 1 2 2 2 2 2" << std::endl;
        input << "2 2 2 2 2 " << std::endl;
        run(input, output);
        std::cout << "Test 9: " << output.str() << " -> ";
        assert(output.str() == "2 2 2 2 2 ");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        int i;
        input << "10000 100" << std::endl;
        for (i = 0; i < 10000; i++)
            input << 3 * i * i + 5 * i << " ";
        for (i = 0; i < 100; i++)
            input << i << " ";
        run(input, output);
        std::cout << "Test 9: " << output.str() << " -> ";
        // assert(output.str() == "2 2 2 2 2 ");
        std::cout << "OK" << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
}

int main() {
    test_main();
    // run(std::cin, std::cout);
    return 0;
}