// 1.4 Инвертируйте значение бита в числе N по его номеру K.

#include <iostream>

// Инверитрование
void invert(unsigned int *N, unsigned int k) {
    *N ^= (1<<k);
}

int main() {
    unsigned int N, k;
    // Ввод
    std::cin >> N >> k;
    // Инвертирование
    invert(&N, k);
    // Вывод
    std::cout << N;
    return 0;
}
