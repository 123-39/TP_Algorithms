/*
 Дано множество целых чисел из [0..10^9] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
1) 10%  перцентиль
2) медиана
3) 90%  перцентиль

 Требования: к дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

 Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

 Реализуйте стратегию выбора опорного элемента “медиана трёх”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
 */

#include <iostream>
#include <cmath>

template<typename T>
bool Comparator(const T& l, const T& r){
    return l > r;
}

// выбора опорного элемента “медиана трёх”
template<typename T>
size_t median_of_3(T *arr, size_t left, size_t right, bool (*comp)(const T&, const T&)){
    size_t center = (left + right) / 2;
    if (comp(arr[left], arr[center])){
        if (comp(arr[center], arr[--right]))
            return center;
        else
            return comp(arr[left], arr[right]) ? right : left;
    }
    else if (comp(arr[center], arr[--right]))
        return (comp(arr[left], arr[right])) ? left : right;
    else
        return center;
}

// возврат индекса пивота после разделения.
template<typename T>
size_t partition(T* arr, size_t left, size_t right, bool (*comp)(const T&, const T&)) {
    // считаем индекс пивота
    size_t pivot_id = median_of_3(arr, left, right, comp);
    // считаем пивот
    T pivot = arr[pivot_id];
    // смена с первым элементом (вроде так при реализации от начала к концу)
    std::swap(arr[pivot_id], arr[left]);

    std::size_t i = right;
    std::size_t j = right;

    while (j != left) {
        if (comp(arr[j], pivot)) {
            std::swap(arr[i--], arr[j]);
        }
        j--;
    }
    // меняем опорный с итеративным
    std::swap(arr[left], arr[i]);
    return i;
}

// Возврат k-й порядковой статистики
template<typename T>
T kth_elem(T *arr, size_t end, size_t k, bool (*cmp)(const T&, const T&)) {
    if (end != -1) {
        size_t start = 0;
        if (end == 0) {
            return arr[0];
        }

        size_t pivot_idx = partition(arr, start, end, cmp);
        while (k != pivot_idx) {
            if (k < pivot_idx) {
                end = pivot_idx - 1;
            }
            if (pivot_idx < k) {
                start = pivot_idx + 1;
            }
            pivot_idx = partition(arr, start, end, cmp);
        }

        return arr[pivot_idx];
    }
}

void result(int *arr, size_t N) {
    size_t percentil_10 = N / 10;
    size_t percentil_50 = N / 2;
    size_t percentil_90 = 9 * N / 10;
    // подсчет k-й порядковой статистики
    std::cout << kth_elem(arr, N - 1, percentil_10, Comparator) << std::endl;
    std::cout << kth_elem(arr, N - 1, percentil_50, Comparator) << std::endl;
    std::cout << kth_elem(arr, N - 1, percentil_90, Comparator) << std::endl;
}

int main() {
    // количество чисел
    size_t N = 0;
    std::cin >> N;
    // выделение памяти для массива чисел
    auto arr = new int[N];
    // инициализация массива
    for(size_t i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }
    // результат
    result(arr, N);
    // удаление/очистка
    delete[] arr;
    arr = nullptr;

    return 0;
}
