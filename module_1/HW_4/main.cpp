/*
Решение всех задач данного раздела предполагает использование кучи, реализованной в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.
    В операционной системе Technux есть планировщик процессов.
    Каждый процесс характеризуется:
    приоритетом P
    временем, которое он уже отработал t
    временем, которое необходимо для завершения работы процесса T
    Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и
    кладет обратно в очередь процессов.
    Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
    Требуется посчитать кол-во переключений процессора.
    Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
    Формат выходных данных: Кол-во переключений процессора.
 */
#include <iostream>

#define INITIAL_BUF__SIZE 8


template<typename T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template <class T, class Comparator = DefaultComparator<T>>
class Heap {
public:
    explicit Heap(Comparator comp = Comparator()): buf_size(0), buf_capacity(INITIAL_BUF__SIZE), comp(comp) {
        buf = new T[INITIAL_BUF__SIZE];
    };

    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;

    Heap& operator=(const Heap&) = delete;
    Heap& operator=(Heap&&) = delete;

    ~Heap() {
        delete[] buf;
    };

    [[nodiscard]] const T& top() const {
        return buf[0];
    };
    void pop();
    void push(const T&);

    [[nodiscard]] bool is_empty() const {
        return buf_size == 0;
    };
    [[nodiscard]] size_t get_size() const {
        return buf_size;
    };

private:
    void buf_size_update();
    void sift_up(size_t index);
    void sift_down(size_t index);

    T* buf;
    size_t buf_size;
    size_t buf_capacity;

    Comparator comp;

};

// структура процесса
struct Process {
    int P;
    int t;
    int T;
};

// выбор процесса с минимальным значением для структуры процесса
struct ProcessComparator {
    bool operator()(const Process& l, const Process& r) {
        return (l.P * (l.t + 1) < r.P * (r.t + 1));
    }
};

// изменение кучи
template <class T, class Comparator>
void Heap<T, Comparator>::pop() {
    if (!is_empty()) {
        // кидем последний элемент в начало
        buf[0] = buf[buf_size - 1];
        --buf_size;
        if (!is_empty()) {
            // толкаем его вниз
            sift_down(0);
        }
    }
}

// заполнение буффера
template <class T, class Comparator>
void Heap<T, Comparator>::push(const T& elem) {
    // проверяем не заполнился ли буффер
    if (buf_size == buf_capacity)
        // увеличиваем размер буффера
        buf_size_update();
    // записываем данные в буффер
    if (buf_size < buf_capacity && buf != nullptr) {
        buf[buf_size++] = elem;
    }
    sift_up(buf_size - 1);
}

// проталкивание наверх
template <class T, class Comparator>
void Heap<T, Comparator>::sift_up(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if ( !comp(buf[index], buf[parent]) )
            return;
        std::swap(buf[index], buf[parent]);
        index = parent;
    }
}

// проталкивание вниз
template <class T, class Comparator>
void Heap<T, Comparator>::sift_down(size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t largest = index;
    if (left < get_size() && comp(buf[index], buf[left]))
        largest = left;
    if (right < get_size() && comp(buf[index], buf[right]))
        largest = right;
    if (largest != index) {
        std::swap(buf[index], buf[largest]);
        sift_down(largest);
    }
}

// увеличение размера буфера
template <class T, class Comparator>
void Heap<T, Comparator>::buf_size_update() {
    int new_buf_capacity = buf_capacity * 2;
    T* new_buf = new T[new_buf_capacity];
    for (int i = 0; i < buf_size; ++i)
        new_buf[i] = buf[i];
    delete[] buf;
    buf = new_buf;
    buf_capacity = new_buf_capacity;
}

// подсчет количества переключений
int switch_count(Heap<class Process, class ProcessComparator> &heap) {
    int switch_num = 0;
    while (!heap.is_empty()) {
        // достаем процесс из кучи
        Process top = heap.top();
        // изменяем кучу
        heap.pop();
        // увеличиваем отработанное время процесса
        top.t += top.P;
        // возвращаем в очередь, если еще не завершился
        if (top.t < top.T) {
            heap.push(top);
        }
        ++switch_num;
    }

    return switch_num;
}

void initialization(Heap<class Process, class ProcessComparator> &heap, size_t proc_count) {
    for (int i = 0; i < proc_count; ++i) {
        Process process;
        process.t = 0;
        std::cin >> process.P >> process.T;
        heap.push(process);
    }
}

int main() {
    // количество процессов
    size_t proc_count;
    std::cin >> proc_count;
    // выделение памяти
    Heap<Process, ProcessComparator> heap;
    // инициализация процессов
    initialization(heap, proc_count);
    // подсчет кол-ва переключений процессора
    int result = switch_count(heap);
    // вывод резултата
    std::cout << result;

    return 0;
}

