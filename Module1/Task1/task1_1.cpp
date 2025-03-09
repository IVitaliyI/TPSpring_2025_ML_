// 1.2 Вернуть значение бита в числе N по его номеру K.
// Формат входных данных. Число N, номер бита K
#include <iostream>

int main() {
    unsigned int N, K;
    bool result;
    std::cin >> N >> K;

    if (std::cin.fail()) {
        std::cerr << "Invalid input\n";
        return 1;
    }

    result = (N >> K) & 1u;
    std::cout << result << "\n";
    return 0;
}