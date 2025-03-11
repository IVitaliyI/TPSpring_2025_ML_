#include <cassert>
#include <iostream>
#include <sstream>

class Deque {
public:
    void push_back(const int element);
    void push_front(const int element);
    int pop_back();
    int pop_front();
    void print_deque();
 
    Deque() : nsize{1}, head{-1}, tail{-1} {
        array = (int*)malloc(sizeof(int) * nsize);
        if (array == nullptr) {
            std::cerr << "Erro with memory allocation" << std::endl;
        }
    }

    Deque(const Deque& other) = delete;
    Deque &operator=(const Deque &other) = delete;

    ~Deque() { free(array); }

    private:
    //  void translate(Stack &from, Stack &to);
    int* array;
    int nsize;
    int head;
    int tail;
    bool is_empty();
    bool is_space();
    void allocate_memory();
    void add_element_front(const int element);
    void add_element_back(const int element);
    // void cutoff_memory();
 };

bool Deque::is_empty() {
    return ((tail == -1) && (head == -1)) ? true : false;
}

bool Deque::is_space() {
    if (head == -1 && tail == -1) return true;
    else if (head == -1 && tail != -1) return ((tail + 1) < nsize);
    else if (head != -1 && tail == -1) return (head > 0);
    else return ((tail + 1) % nsize != head);
}

void Deque::allocate_memory() {
    int i, k = 0;
    int *new_start = (int*)malloc(sizeof(int) * nsize * 2);
    if (new_start == nullptr) {
        std::cerr << "Error in memory allocation" << std::endl;
        abort();
    }

    if (head == -1) {
        for (i = 0; i <= tail; i++) {
            new_start[k] = array[i];
            k++; 
            tail = k - 1;
        }
    } else if (tail == -1) {
        for (i = head; i < nsize; i++) {
            new_start[k] = array[i];
            k++; 
            tail = k - 1;
        }
    } else {
        for (i = head; i != tail; i = (i + 1) % nsize) {
            new_start[k] = array[i];
            k++;
        }
        new_start[k] = array[i];
        tail = k;
    }

    free(array);
    array = new_start;
    nsize *= 2;
    head = 0;
}

void Deque::add_element_back(const int element) {
    tail = (tail + 1) % nsize;
    array[tail] = element;
}

void Deque::push_back(const int element) {
    if (is_space()) {
        add_element_back(element);
    } else {
        allocate_memory();
        add_element_back(element);
    }
}

void Deque::add_element_front(const int element) {
    head = (head > 0) ? head - 1 : nsize - 1;
    array[head] = element;
}

void Deque::push_front(const int element) {
    if (is_space()) {
        add_element_front(element);
    } else {
        allocate_memory();
        add_element_front(element);
    }
}

int Deque::pop_back() {
    int tmp = tail;
    if (is_empty()) return -1;
    tail = (tail > 0) ? (tail - 1) : nsize - 1;
    return array[tmp];
}

void Deque::print_deque() {
    int i;
    if (head != -1) {
        for (i = head; i != tail; i = (i + 1) % nsize) {
            std::cout << array[i] << " ";
        }
        std::cout << array[i] << std::endl;
        std::cout << "Array size: " << nsize << std::endl;
    } else if (tail == -1) {
        for (i = head; i < nsize; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << array[i] << std::endl;
        std::cout << "Array size: " << nsize << std::endl; 
    } else {
        for (i = head; i != tail; i = (i + 1) % nsize) {
            std::cout << array[i] << " ";
        }
        std::cout << array[i] << std::endl;
        std::cout << "Array size: " << nsize << std::endl;
    }
}

void run(std::istream &input, std::ostream &output) {
    int count;
    int cmd, element;
    int i;
    bool flag = true;
    Deque dq;
    input >> count;
    if (input.fail()) {
        std::cerr << "Problem with input";
        abort();
    }
    for (i = 0; i < count; i++) {
        if (!flag) break;
        input >> cmd >> element;
        if (cmd == 1) {
            dq.push_front(element);
        // } else if (cmd == 2) {
        //     if (dq.pop_front() != element) flag = false;
        } else if (cmd == 3) {
            dq.push_back(element);
        } else if (cmd == 4) {
            if (dq.pop_back() != element) flag = false;
        } else {
            std::cerr << "Error, this command is not support";
        }
    }
    (flag) ? output << "YES\n" : output << "NO\n";
    dq.print_deque();
    return;
}

void test_case() {
    std::stringstream input, output;
    input << "5" << std::endl;
    input << "3 123" << std::endl;
    input << "4 1234" << std::endl;
    input << "1 15" << std::endl;
    input << "1 27" << std::endl;
    input << "3 12345" << std::endl;
    run(input, output);
    std::cout << output.str() << std::endl;
}

int main() {
    test_case();
    // run(std::cin, std::cout);
    return 0;
}