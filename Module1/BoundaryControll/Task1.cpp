#include <iostream>

int binary_search(const int* array, unsigned left, unsigned right,
    const int element) {
    unsigned middle;

    while (left < right) {
        middle = (left + right) / 2;
        if (array[middle] == element) return middle;
        if (array[middle] < element) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return right;
}

int main() {
    int n_array;
    int i;
    std::cin >> n_array;
    int* arr = (int*)malloc(sizeof(int) * n_array);
    for (int i = 0; i < n_array; i++) {
        std::cin >> arr[i];
    }
    int k;
    std::cin >> k;
    std::cout << binary_search(arr, 0, n_array, k);
    free(arr);
    return 0;
}