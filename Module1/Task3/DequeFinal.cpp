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
        array = (int *)malloc(sizeof(int) * nsize);
        if (array == nullptr) {
            std::cerr << "Error with memory allocation" << std::endl;
        }
    }

    Deque(const Deque &other) = delete;
    Deque &operator=(const Deque &other) = delete;

    ~Deque() { free(array); }

   private:
    //  void translate(Stack &from, Stack &to);
    int *array;
    unsigned nsize;
    int head;
    int tail;
    bool is_empty();
    bool is_full();
    void allocate_memory();
    void add_element_front(const int element);
    void add_element_back(const int element);
    // void cutoff_memory();
};

bool Deque::is_empty() { return head == -1; }

bool Deque::is_full() { return head == (tail + 1) % nsize; }

void Deque::allocate_memory() {
    int i, k = 0;
    int newsize = nsize * 2;
    int *new_start = (int *)malloc(sizeof(int) * newsize);
    if (new_start == nullptr) {
        std::cerr << "Error in memory allocation" << std::endl;
        abort();
    }

    for (i = head; i != tail; i = (i + 1) % nsize) {
        new_start[k] = array[i];
        k++;
    }
    new_start[k] = array[i];

    free(array);
    array = new_start;
    nsize = newsize;
    head = 0;
    tail = k;
}

void Deque::add_element_back(const int element) {
    if (is_empty()) {
        head = tail = 0;
    } else {
        tail = (tail + 1) % nsize;
    }
    array[tail] = element;
}

void Deque::add_element_front(const int element) {
    if (is_empty()) {
        head = tail = 0;
    } else {
        head = (head > 0) ? (head - 1) : (nsize - 1);
    }
    array[head] = element;
}

void Deque::push_front(const int element) {
    if (is_full()) allocate_memory();
    add_element_front(element);
}

void Deque::push_back(const int element) {
    if (is_full()) allocate_memory();
    add_element_back(element);
}

int Deque::pop_back() {
    int tmp = tail;
    if (is_empty()) return -1;
    if (head == tail) {
        head = tail = -1;
    } else {
        tail = (tail > 0) ? (tail - 1) : (nsize - 1);
    }
    return array[tmp];
}

int Deque::pop_front() {
    int tmp = head;
    if (is_empty()) return -1;
    if (head == tail) {
        head = tail = -1;
    } else {
        head = (head + 1) % nsize;
    }
    return array[tmp];
}

void Deque::print_deque() {
    int i;
    for (i = head; i != tail; i = (i + 1) % nsize) {
        std::cout << array[i] << ' ';
    }
    std::cout << array[i] << ' ' << std::endl;
    std::cout << nsize << std::endl;
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
        } else if (cmd == 2) {
            if (dq.pop_front() != element) flag = false;
        } else if (cmd == 3) {
            dq.push_back(element);
        } else if (cmd == 4) {
            if (dq.pop_back() != element) flag = false;
        } else {
            std::cerr << "Error, this command is not support";
        }
    }
    (flag) ? output << "YES\n" : output << "NO\n";
    // dq.print_deque();
    return;
}

void test_case() {
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "1 44" << std::endl;
        input << "3 50" << std::endl;
        input << "2 44" << std::endl;
        std::cout << "Test 1: " << std::endl;
        run(input, output);
        std::cout << output.str();
    }
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "2 -1" << std::endl;
        input << "1 10" << std::endl;
        std::cout << "Test 2: " << std::endl;
        run(input, output);
        std::cout << output.str();
    }
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "3 44" << std::endl;
        input << "4 66" << std::endl;
        std::cout << "Test 3: " << std::endl;
        run(input, output);
        std::cout << output.str();
    }
}

int main() {
    // test_case();
    run(std::cin, std::cout);
    return 0;
}
