/*
3_1. Реализовать очередь с динамическим зацикленным буфером.
Требования: Очередь должна быть реализована в виде класса.
*/

#include <iostream>
#define BUF_CAPACITY 8

class Queue
{
public:
    Queue() {
        buffer_capacity = BUF_CAPACITY;
        buf = new int[BUF_CAPACITY];
        size = 0;
        head = -1;
        tail = -1;
    }

    ~Queue()
    {
        delete[] buf;
    }
    // добавляем элемент в хвост
    void push(int var) {
        if (is_full()) {
            int* new_buf = new int[buffer_capacity * 2];
            for (int i = 0; i < size; ++i) {
                new_buf[head + i] = buf[(head + i) % buffer_capacity];
            }
            tail = head + size - 1;
            buffer_capacity *= 2;

            delete[] buf;
            buf = new_buf;
        }

        if (is_empty())
            head = 0;

        tail = (tail + 1) % buffer_capacity;
        buf[tail] = var;
        size++;
    }
    // берем елемент с головы
    int pop() {
        if (is_empty())
            return -1;

        int tmp = buf[head];
        if (head == tail) {
            head = -1;
            tail = -1;
        }
        else
            head = (head + 1) % buffer_capacity;
        size--;

        return tmp;
    }

private:
    // провкра на пустоту
    [[nodiscard]] bool is_empty() const {
        return (head == -1 && tail == -1);
    }

    [[nodiscard]] bool is_full() const {
        return (size == buffer_capacity);
    }

    int* buf;
    int size, buffer_capacity, head, tail;
};

// реализация
bool run (size_t num_command) {
    Queue q;
    bool flag = true;
    for (int i = 0; i < num_command; ++i)
    {
        int command = 0;
        int command_value = -1;
        std::cin >> command >> command_value;

        if (command == 3)
            q.push(command_value);

        if (command == 2)
            if (q.pop() != command_value)
                flag = false;
    }
    return flag;
}

int main() {
    // количество команд
    size_t num_command = 0;
    std::cin >> num_command;
    // прогоняем команды
    bool flag = run(num_command);
    // результат
    std::cout << (flag ? "YES" : "NO");

    return 0;
}

