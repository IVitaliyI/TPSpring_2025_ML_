#include <cassert>
#include <iostream>
#include <sstream>

class Stack {
   public:
    Stack() : start(nullptr), nsize(0), tail(-1) {
        start = (int *)malloc(sizeof(int));
        nsize = 1;
    };

    Stack(const Stack &other) = delete;
    Stack &operator=(const Stack &other) = delete;

    ~Stack() { free(start); }

    void push(const int element);
    int pop();
    bool is_empty() { return (tail == -1) ? true : false; }

   private:
    int *start;
    int nsize;
    int tail;

    bool is_space();
    void allocate_more_memory();
    void add_element(const int element);
};

bool Stack::is_space() {
#if 0
    std::cout << tail << " " << nsize << std::endl;
#endif
    assert(tail < (nsize + 1));
    return ((tail + 1) == nsize);
}

void Stack::allocate_more_memory() {
    int *new_start = (int *)realloc(start, sizeof(int) * nsize * 2);
    if (!new_start) {
        return;
    }
    start = new_start;
#if 0
    std::cout << "size of array = " << nsize << std::endl;
#endif
    nsize *= 2;
}

void Stack::add_element(const int element) {
    tail++;
    start[tail] = element;
}

void Stack::push(const int element) {
    if (!is_space()) {
        add_element(element);
    } else {
        allocate_more_memory();
        add_element(element);
    }
}

int Stack::pop() {
    if (is_empty())
        return -1;
    else {
        tail--;
        return start[tail + 1];
    }
}

class Deque {
   public:
    void push_back(const int element);
    void push_front(const int element);
    int pop_back();
    int pop_front();

   private:
    Stack front;
    Stack back;
    void translate(Stack &from, Stack &to);
};

void Deque::push_back(const int element) { back.push(element); }
int Deque::pop_front() {
    if (front.is_empty()) translate(back, front);
    return front.pop();
}

int Deque::pop_back() {
    if (back.is_empty()) translate(front, back);
    return back.pop();
}

void Deque::translate(Stack &from, Stack &to) {
    int i;
    while (!from.is_empty()) {
        to.push(from.pop());
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
            dq.push_back(element);
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
    return;
}

void test_case() {
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "2 -1" << std::endl;
        input << "2 2" << std::endl;
        input << "3 5" << std::endl;
        input << "3 5" << std::endl;
        input << "3 5" << std::endl;
        run(input, output);
        std::cout << "TEST 1: " << output.str();
        assert(output.str() == "NO\n");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "2 -1" << std::endl;
        input << "2 -1" << std::endl;
        input << "3 5" << std::endl;
        input << "3 5" << std::endl;
        input << "3 5" << std::endl;
        run(input, output);
        std::cout << "TEST 2: " << output.str();
        assert(output.str() == "YES\n");
        std::cout << "OK" << std::endl;
    }
}

int main() {
    test_case();
    // run(std::cin, std::cout);
    return 0;
}