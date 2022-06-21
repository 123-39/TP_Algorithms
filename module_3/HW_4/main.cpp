/*
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12]
[ 13 14 15 0 ]
где 0 задает пустую ячейку.
Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.

Формат входных данных
Начальная расстановка.

Формат выходных данных
Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек,
 которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую
 последовательность ходов: L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз.
 Если таких последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация недостижима,
 то выведите в выходной файл одно число −1.
 */

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_map>

static const size_t LINE_SIZE = 4;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

const std::array<uint8_t, FIELD_SIZE> GOAL_STATE = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,10,11,12,13,14,15,0
};

struct FieldStateHash;

class FieldState {
public:
    explicit FieldState(const std::array<uint8_t, FIELD_SIZE> &field)
            : state_array(field) {
        zero_pos = -1;
        for (int i = 0; i < FIELD_SIZE; i++) {
            if (field[i] == 0)
                zero_pos = i;
        }
    }

    bool operator<(const FieldState &other) const {
        return state_array < other.state_array;
    }

    bool operator==(const FieldState &other) const {
        return state_array == other.state_array;
    }

    FieldState(const FieldState &other) = default;
    FieldState(FieldState &&other) = default;

    FieldState &operator=(const FieldState &source) = default;
    FieldState &operator=(FieldState &&source) = default;

    bool is_complete() const {
        return state_array == GOAL_STATE;
    }

    bool is_solvable() const {
        return !((inv_count() + zero_pos / LINE_SIZE + 1) % 2);
    }

    bool is_move_up() const {
        return zero_pos < LINE_SIZE * (LINE_SIZE - 1);
    }

    bool is_move_down() const {
        return zero_pos >= LINE_SIZE;
    }

    bool is_move_left() const {
        return zero_pos % LINE_SIZE != (LINE_SIZE - 1);
    }

    bool is_move_right() const {
        return zero_pos % LINE_SIZE != 0;
    }

    FieldState move_up() const {

        FieldState newState(*this);
        std::swap(newState.state_array[zero_pos], newState.state_array[zero_pos + LINE_SIZE]);
        newState.zero_pos += LINE_SIZE;
        return newState;
    }

    FieldState move_down() const {

        FieldState newState(*this);
        std::swap(newState.state_array[zero_pos], newState.state_array[zero_pos - LINE_SIZE]);
        newState.zero_pos -= LINE_SIZE;
        return newState;
    }

    FieldState move_left() const {

        FieldState newState(*this);
        std::swap(newState.state_array[zero_pos], newState.state_array[zero_pos + 1]);
        ++newState.zero_pos;
        return newState;
    }

    FieldState move_right() const {

        FieldState newState(*this);
        std::swap(newState.state_array[zero_pos], newState.state_array[zero_pos - 1]);
        --newState.zero_pos;
        return newState;
    }

private:
    size_t inv_count() const {
        size_t count = 0;
        for (int i = 0; i < FIELD_SIZE - 1; i++) {
            for (int j = i + 1; j < FIELD_SIZE; j++) {
                if (state_array[i] && state_array[j] && state_array[i] > state_array[j])
                    count++;
            }
        }
        return count;
    }

    std::array<uint8_t, FIELD_SIZE> state_array;
    uint8_t zero_pos;

    friend struct FieldStateHash;
    friend int manhat_dist(const FieldState &state);

    friend std::ostream &operator<<(std::ostream &out, const FieldState &state);
};


struct FieldStateHash {
public:
    size_t operator()(const FieldState &state) const {
        size_t hash = 0;
        for (int i = 1u; i <= 16u; i += 2u) {
            char val = (state.state_array[i] << 4) + state.state_array[i + 1u];
            hash <<= 8;
            hash += val;
        }
        return hash;
    }
};

std::ostream &operator<<(std::ostream &out, const FieldState &state) {
    for (int i = 0; i < LINE_SIZE; i++) {
        for (int j = 0; j < LINE_SIZE; j++) {
            out << static_cast<int>(state.state_array[i * LINE_SIZE + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

int manhat_dist(const FieldState &state) {
    int count = 0;
    for (int i = 0; i < FIELD_SIZE; ++i) {
        if (state.state_array[i] != GOAL_STATE[i])
            ++count;
    }
    return count;
}

std::string solving(const std::array<uint8_t, FIELD_SIZE> &field) {
    FieldState startState(field);

    if (!startState.is_solvable())
        return "-1";

    std::unordered_map<FieldState, char, FieldStateHash> visited;
    std::set<std::pair<int, FieldState>> set;
    set.emplace(manhat_dist(startState), startState);

    visited[startState] = 'S';

    while (true) {
        std::pair<int, FieldState> currState = set.extract(set.begin()).value();

        if (currState.second.is_complete())
            break;

        if (currState.second.is_move_left()) {
            FieldState newState = currState.second.move_left();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'L';
                set.emplace(manhat_dist(newState) + currState.first + 1, newState);
            }
        }
        if (currState.second.is_move_right()) {
            FieldState newState = currState.second.move_right();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'R';
                set.emplace(manhat_dist(newState) + currState.first + 1, newState);
            }
        }
            if (currState.second.is_move_up()) {
            FieldState newState = currState.second.move_up();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'U';
                set.emplace(manhat_dist(newState) + currState.first + 1, newState);
            }
        }
        if (currState.second.is_move_down()) {
            FieldState newState = currState.second.move_down();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'D';
                set.emplace(manhat_dist(newState) + currState.first + 1, newState);
            }
        }
        if (set.size() > 9000u) {
            int averageEvristika = (set.begin()->first + set.rbegin()->first) / 2;
            set.erase(set.upper_bound(std::make_pair(averageEvristika, currState.second)), set.end());
        }
    }

    std::string path;
    FieldState state(GOAL_STATE);

    while (visited[state] != 'S') {
        char move = visited[state];
        switch (move) {
            case 'L': {
                state = state.move_right();
                path += 'L';
                break;
            }
            case 'R': {
                state = state.move_left();
                path += 'R';
                break;
            }
            case 'U': {
                state = state.move_down();
                path += 'U';
                break;
            }
            case 'D': {
                state = state.move_up();
                path += 'D';
                break;
            }
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void init(std::array<uint8_t, FIELD_SIZE> &arr) {
    int val = 0;
    for (int i = 0; i < FIELD_SIZE; ++i) {
        std::cin >> val;
        arr[i] = val;
    }
}

int main() {
    std::array<uint8_t, FIELD_SIZE> arr{};
    init(arr);

    std::string solution = solving(arr);
    std::cout << solution.size() << std::endl;
    std::cout << solution << std::endl;
    return 0;
}
