/*
Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции.
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).

На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка (Li и Ri).
Найти длину окрашенной части числовой прямой.
*/


#include <iostream>
#include <cstring>

struct Point {
    int val;
    int start_end;
};

template<typename T>
struct DefaultComparator{
    bool operator() (const T& l, const T& r){
        return l < r;
    }
};

bool ComparatorLine(const Point &l, const Point &r) {
    return l.val < r.val;
}

// слияние
template<typename T, typename Comparator = DefaultComparator<T>>
void merge(T *arr_1, size_t arr_1_size, T *arr_2, size_t arr_2_size, T *merge_arr, Comparator comp = Comparator()){
    size_t i = 0;
    size_t i_1 = 0;
    size_t i_2 = 0;
    // объединяем, пока один из массивов не закончится
    while (i_1 < arr_1_size && i_2 < arr_2_size)
        merge_arr[i++] = comp(arr_1[i_1], arr_2[i_2]) ? arr_1[i_1++] : arr_2[i_2++];
    // дописываем остаток
    while(i_1 < arr_1_size)
        merge_arr[i++] = arr_1[i_1++];
    while(i_2 < arr_2_size)
        merge_arr[i++] = arr_2[i_2++];
}

// сортировка слиянием (делаем рекурсию, так как глубина явно меньше 1000)
template<typename T, typename Comparator = DefaultComparator<T>>
void mergeSort(T *arr, size_t num_points, Comparator comp = Comparator()){
    if (num_points <= 1)
        return;
    size_t first_len = num_points / 2;
    size_t second_len = num_points - first_len;
    mergeSort(arr, first_len, comp);
    mergeSort(arr + first_len, second_len, comp);
    T *new_arr = new T[num_points];
    merge(arr, first_len, arr + first_len, second_len, new_arr, comp);
    memcpy(arr, new_arr, sizeof(T) * num_points);
    delete[] new_arr;
}

// считаем длину  окрашенной части числовой прямой
int calculate_lenght(Point *arr, size_t num_points){
    int length = 0;
    int previous = 0;
    // начинаем подсчет с последней правой точки
    int start_point = arr[0].val;
    for (size_t i = 0; i < num_points; ++i){
        // если точка праввая, то считаем разность
        if (arr[i].start_end){
            if (--previous == 0) {
                length += arr[i].val - start_point;
            }
        }
        // устанавливаем новую точку как начальную
        else if (previous++ == 0) {
            start_point = arr[i].val;
        }
    }
    return length;
}

// инициализация значений точек
void initialization(Point *points, size_t N) {
    int i = 0;
    while(i < 2 * N){
        // начальная точка
        std::cin >> points[i].val;
        points[i++].start_end = false;
        // конечная точка
        std::cin >> points[i].val;
        points[i++].start_end = true;
    }
}

int main() {
    // количество отрезков
    size_t N = 0;
    std::cin >> N;
    // массив точек
    Point *points = new Point[2 * N];
    // инициализация точек
    initialization(points, N);
    // сортировка слиянием
    mergeSort(points, 2 * N, ComparatorLine);
    // вычисление расстояния
    std::cout << calculate_lenght(points, 2 * N);
    // удаление/очистка
    delete[] points;
    points = nullptr;

    return 0;
}