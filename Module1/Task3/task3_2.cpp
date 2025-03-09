#include <cassert>
#include <iostream>

class Stack {
public:
    Stack() : start(nullptr), nsize(0), tail(-1) {
        start = (int*)malloc(sizeof(int));
        nsize = 1;
    };

    Stack(const Stack &other) = delete;
    Stack& operator=(const Stack &other) = delete;

    ~Stack() {
        free(start);
    }

    void push(const int element);
    int pop();
private:
    int* start;
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
    int* new_start = (int*)realloc(start, sizeof(int) * nsize * 2);
    if (!new_start) {
        return;
    }
    start = new_start;
    std::cout << "size of array = " << nsize << std::endl;
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
    if (tail == -1) return -1;
    else {
        tail--;
        return start[tail + 1];
    }
}

int main() {
    return 0;
}