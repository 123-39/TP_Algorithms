/*
LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6.
Отсортировать массив методом поразрядной сортировки LSD по байтам.
*/

#include <iostream>

#define MAX_SIZE 256 // максимальный диапазон данных
#define SORT_STEP 8 // сортировка по байтам

// побайтовый сдвиг при максимальном диапазоне данных
int nth_byte_convert(long long a, int i) {
    return (int)((a >> (8 * i)) & (MAX_SIZE - 1));
}

void results(long long *arr, size_t N) {
    for(size_t i = 0; i < N; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void count_sort(long long* arr, int size, int counters_size) {
    // вспомогательный массив
    auto* temp_arr = new long long[size];
    // массив счетчиков
    int* counters = new int[counters_size];
    // сортируем по байтам
    for (int j = 0; j < SORT_STEP; ++j) {
        // заполняем массив счетиков нулями
        std::fill_n(counters, counters_size, 0);
        // считаем количество вхождений каждого элемента исходного массива
        for (int i = 0; i < size; ++i) {
            counters[nth_byte_convert(arr[i], j)]++;
        }
        // считаем диапазоны групп (массивы окончания групп)
        for (int i = 0; i < counters_size - 1; ++i) {
            counters[i + 1] += counters[i];
        }
        // заполняем вспомогательный (отсортированный массив)
        for (int arr_idx = size - 1; arr_idx >= 0; --arr_idx) {
            int temp_idx = --counters[nth_byte_convert(arr[arr_idx], j)];
            temp_arr[temp_idx] = arr[arr_idx];
        }
        // копируем в исходный массив
        std::copy(temp_arr, temp_arr + size, arr);
    }

    delete [] temp_arr;
    temp_arr = nullptr;
    delete [] counters;
    counters = nullptr;
}



int main() {
    // размер массива
    int N;
    std::cin >> N;
    // выделяем память
    auto *arr = new long long[N];
    // инициализируем массив
    for (int i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }
    // сортировка
    count_sort(arr, N, MAX_SIZE);
    // вывод резултата
    results(arr, N);
    // удаление/очистка
    delete[] arr;
    arr = nullptr;

    return 0;
}
