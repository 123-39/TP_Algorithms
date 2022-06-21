/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 Хранимые строки непустые и состоят из строчных латинских букв.
 Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 когда коэффициент заполнения таблицы достигает 3/4.
 Структура данных должна поддерживать операции добавления строки в множество,
 удаления строки из множества и проверки принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.

Требования: В таблице запрещено хранение указателей на описатель элемента.
 */

#include <iostream>
#include <string>

template <class T>
size_t HashHorner(const T& s, size_t table_size, const size_t key) {
    size_t hash_result = 0;
    for (char i : s)
        hash_result = (key * hash_result + i) % table_size;
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

template <class T>
struct Hash_1 {
    size_t operator()(const T& s, size_t table_size) const {
        return HashHorner(s, table_size, table_size - 1);
    }
};

template <class T>
struct Hash_2 {
    size_t operator()(const T& s, size_t table_size) const {
        return HashHorner(s, table_size, table_size + 1);
    }
};

template <class T, class Hash1 = Hash_1<T>, class Hash2 = Hash_2<T>>
class HashTable {
    static constexpr size_t default_table_size = 8;
    static constexpr double rehash_coef_size = 0.75;
    struct Node {
        T key;
        bool state;
        explicit Node(T key_) : key(std::move(key_)), state(true) {}
    };

public:
    explicit HashTable(Hash1 hash1 = Hash1(), Hash2 hash2 = Hash2()) : hash_1(hash1), hash_2(hash2) {
        buff_size = default_table_size;
        size = 0;
        full_size = 0;
        arr = new Node*[buff_size];
        for (int i = 0; i < buff_size; ++i)
            arr[i] = nullptr;
    }
    ~HashTable() {
        delete[] arr;
    }
    // Вставка
    bool insert(const T& key) {
        if (size >= rehash_coef_size * buff_size)
            grow();
        else if (full_size > 2 * size)
            rehash();
        // двойное хеширование
        size_t h1 = hash_1(key, buff_size);
        size_t h2 = hash_2(key, buff_size);

        size_t i = 0;
        // первый подходящий (удаленный) ключ
        int del_key = -1;
        while (arr[h1] != nullptr && i < buff_size) {
            // проверяем, существует ли уже такой ключ
            // и не удален ли он
            if (arr[h1]->key == key && arr[h1]->state)
                return false;
            // ищем удаленное место для нового ключа
            if (!arr[h1]->state && del_key == -1)
                del_key = int(h1);
            h1 = (h1 + h2) % buff_size;
            ++i;
        }
        // cоздаем новое поле по ключу
        if (del_key == -1) {
            arr[h1] = new Node(key);
            ++full_size;
        }
        // записываем новый ключ в удаленное поле
        else {
            arr[del_key]->key = key;
            arr[del_key]->state = true;
        }
        ++size;
        return true;
    }
    // Удаление
    bool erase(const T& key) {
        // двойное хеширование
        size_t h1 = hash_1(key, buff_size);
        size_t h2 = hash_2(key, buff_size);

        size_t i = 0;
        while (arr[h1] != nullptr && i < buff_size) {
            // меняем флаг найденного элемента на удаленный
            if (arr[h1]->key == key && arr[h1]->state) {
                arr[h1]->state = false;
                --size;
                return true;
            }
            h1 = (h1 + h2) % buff_size;
            ++i;
        }
        return false;
    }
    // Поиск
    bool find(const T& key) {
        // Двойное хеширование
        size_t h1 = hash_1(key, buff_size);
        size_t h2 = hash_2(key, buff_size);

        size_t i = 0;
        while (arr[h1] != nullptr && i < buff_size) {
            // нашли ключ и он не удален
            if (arr[h1]->key == key && arr[h1]->state)
                return true;
            h1 = (h1 + h2) % buff_size;
            ++i;
        }
        return false;
    }
private:
    // Увеличение массива в два раза
    void grow() {
        buff_size *= 2;
        full_size = size = 0;
        // инициализация нового масссива
        Node** new_arr = new Node * [buff_size];
        for (int i = 0; i < buff_size; ++i)
            new_arr[i] = nullptr;
        // добавляем элементы в новыя массив
        std::swap(arr, new_arr);
        for (int i = 0; i < (buff_size / 2); ++i) {
            if (new_arr[i] && new_arr[i]->state)
                insert(new_arr[i]->key);
        }
        // удаляем предыдующий массив
        delete[] new_arr;
    }
    // Перехеширование (удаление пустых элементов)
    void rehash() {
        full_size = size = 0;
        // инициализация
        Node** new_arr = new Node * [buff_size];
        for (int i = 0; i < buff_size; ++i)
            new_arr[i] = nullptr;
        // добавляем неудаленные элементы в новыя массив
        std::swap(arr, new_arr);
        for (int i = 0; i < buff_size; ++i) {
            if (new_arr[i] && new_arr[i]->state)
                insert(new_arr[i]->key);
        }
        // удаляем предыдующий массив
        delete[] new_arr;
    }

    // размер массива без пустых элементов
    size_t size;
    // размер массива
    size_t full_size;
    // размер буффера
    size_t buff_size;

    Node** arr;

    Hash1 hash_1;
    Hash2 hash_2;
};


int main() {
    HashTable<std::string> htable;
    std::string key;
    char operation = '\0';
    while (std::cin >> operation >> key) {
        switch (operation) {
            case '+':
                std::cout << (htable.insert(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (htable.erase(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (htable.find(key) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                return 1;
        }
    }

    return 0;
}