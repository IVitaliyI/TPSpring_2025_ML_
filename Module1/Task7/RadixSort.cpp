#include <cassert>
#include <iostream>

template <typename T>
class MSDBinarySort {
   public:
    void operator()(T* data, const int size);

   private:
    void sort(T* data, int left, int right, int radix_idx);
};

template <typename T>
void MSDBinarySort<T>::operator()(T* data, int size) {
    if (size <= 1) return;

    int max_bit = sizeof(T) * 8 - 1;
    sort(data, 0, size - 1, max_bit);
}

template <typename T>
void MSDBinarySort<T>::sort(T* data, int left, int right, int radix_idx) {
    if ((left >= right) || (radix_idx < 0)) return;

    int i = left, j = right;
    while (i <= j) {
        while ((i <= j) && ((data[i] >> radix_idx) & 1u) == 0) i++;
        while ((i <= j) && ((data[j] >> radix_idx) & 1u) == 1) j--;
        if (i < j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }

    sort(data, left, j, radix_idx - 1);
    sort(data, i, right, radix_idx - 1);
}

void run() {
    int n;
    unsigned i;
    std::cin >> n;
    unsigned long long* data = new unsigned long long[n];
    for (i = 0; i < n; i++) {
        std::cin >> data[i];
    }
    MSDBinarySort<unsigned long long>()(data, n);
    for (i = 0; i < n; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    delete[] data;
}

int main() {
    run();
    return 0;
}