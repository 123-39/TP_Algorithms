// 1.4 Инвертируйте значение бита в числе N по его номеру K.

#include <iostream>

void invert(unsigned int *N, unsigned int k) {
    *N ^= (1<<k);
}

int main() {
    unsigned int N, k;
    std::cout << "Input N: ";
    std::cin >> N;
    std::cout << "Input k: ";
    std::cin >> k;
    invert(&N, k);
    std::cout << N;
    return 0;
}

