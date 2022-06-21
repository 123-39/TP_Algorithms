/*
 2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
 Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
 равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
 Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)).
 Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска
 размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
 Формат входных данных: в первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.
 */

#include <iostream>
#include <cmath>

// Инициализируем массив
void create_arr(int *arr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
}

// Бинарный поиск
size_t bin_Search(const int *A, size_t low_bound, size_t up_bound, int element) {
    size_t mid = (low_bound + up_bound) / 2;
    while (A[mid] != element) {
        if (mid == low_bound) {
            return up_bound;
        }
        if (A[mid] > element) {
            up_bound = mid;
        }
        else {
            low_bound = mid;
        }
        mid = (low_bound + up_bound) / 2;
    }
    return mid;
}

// Экспоненциальный поиск (ищем нужный интервал)
size_t exp_search(int element, const int *A, size_t size_A) {
    if (A[0] >= element)
        return 0;
    if (A[1] >= element) {
        return 1;
    }
    if (A[size_A - 1] < element) {
        return size_A;
    }
    size_t i = 2;
    while (i < size_A && A[i] <= element)
        i *= 2;
    return bin_Search(A, i/2, i > (size_A - 1) ? size_A - 1 : i, element);
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    // Выделяем память для массивов
    int *A = new int[n];
    int *B = new int[m];
    // Инициализируем массивы
    create_arr(A, n);
    create_arr(B, m);
    // Вывдоим результат
    for (size_t i = 0; i < m; ++i) {
        std::cout << exp_search(B[i], A, n) << ' ';
    }
    // Очищаем память
    delete [] A;
    A = nullptr;
    delete [] B;
    B = nullptr;

    return 0;
}
