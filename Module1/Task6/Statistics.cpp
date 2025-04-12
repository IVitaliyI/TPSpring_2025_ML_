#include <cassert>
#include <iostream>

template <typename T, typename Compare = std::less<T>>
class KStatistics {
   public:
    T operator()(T* data, unsigned left, unsigned right, const unsigned k);

   private:
    int partition(T* data, int left, int right);
    int select(T* data, const int left, const int right) const;
    Compare cmp = Compare();
};

template <typename T, typename Compare>
int KStatistics<T, Compare>::partition(T* data, int left, int right) {
    int pivot_idx = select(data, left, right);
    std::swap(data[pivot_idx], data[right]);
    int i = left, j = left;

    while (j < right) {
        if (cmp(data[right], data[j])) {
            j++;
        } else {
            std::swap(data[i], data[j]);
            i++;
            j++;
        }
    }
    std::swap(data[right], data[i]);
    return i;
}

template <typename T, typename Compare>
int KStatistics<T, Compare>::select(T* data, const int left, const int right) const {
    int mid = left + (right - left) / 2;
    if (cmp(data[mid], data[left])) {
        if (cmp(data[left], data[right])) {
            return left;
        }
        return (cmp(data[right], data[mid])) ? mid : right;
    }
    if (cmp(data[mid], data[right])) {
        return mid;
    }
    return (cmp(data[right], data[left])) ? left : right;
}

template <typename T, typename Compare>
T KStatistics<T, Compare>::operator()(T* data, unsigned left, unsigned right, const unsigned k) {
    assert(k >= 0 && k <= (right - left));
    int pivot_pos = partition(data, left, right);

    while (true) {
        int pivot_pos = partition(data, left, right);
        if (pivot_pos == k) {
            return data[k];
        }
        if (pivot_pos > k) {
            right = pivot_pos - 1;
            continue;
        }
        left = pivot_pos + 1;
    }
}

void run() {
    int n;
    std::cin >> n;

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    std::cout << KStatistics<int>()(arr, 0, n - 1, 0.1 * n) << std::endl;
    std::cout << KStatistics<int>()(arr, 0, n - 1, 0.5 * n) << std::endl;
    std::cout << KStatistics<int>()(arr, 0, n - 1, 0.9 * n) << std::endl;
    delete[] arr;
}

int main() {
    run();
    return 0;
}