/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне 0..2^32 -1

Требования:
B-дерево должно быть реализовано в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
*/

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

template <class T>
struct DefaultComparator {
    int operator () (const T &first, const T &second) const {
        return first > second;
    }
};

template <class Key, class Comparator = DefaultComparator<Key> >
class BTree
{
public:
    struct Node
    {
        bool leaf;
        std::vector<Key> keys;
        std::vector<Node*> children;

        explicit Node(bool leaf) : leaf(leaf) {
        }

        ~Node() {
            for (Node* child: children)
                delete child;
        }
    };

    explicit BTree(size_t min_degree, Comparator comp = Comparator()) :
    root(nullptr), t(min_degree), comp(comp) {
        assert(min_degree >= 2);
    }

    ~BTree() {
        if (root)
            delete root;
    }
    // Вставка элемента
    void insert(const Key &key) {
        // проверка на пустоту
        if (!root)
            root = new Node(true);
        // если узел переполнен, то разбиваем и спускаемся
        if (is_node_full(root)) {
            // поднимаем наверх и формируем новый корень
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            // разбиваем узел
            split(root, 0);
        }
        // добавляем элемент в непереполненный узел
        insert_non_full(root, key);
    }

    // Вывод результатов
    void res_output() {
        // инициализируем очередь
        std::queue<Node *> q;
        // и заполняем нашим деревом
        q.push(root);
        int prev_num_elem_in_node = 1;
        int num_elem_in_node;
        while (!q.empty()) {
            num_elem_in_node = prev_num_elem_in_node;
            prev_num_elem_in_node = 0;
            for (int j = 0; j < num_elem_in_node; j++) {
                // достаем корень (верхний узел)
                Node *print_node = q.front();
                // и удаляем из очереди
                q.pop();
                // выводим элементы узла
                for (int i = 0; i < print_node->keys.size(); i++) {
                    std::cout << print_node->keys[i] << " ";
                }
                for (int i = 0; i < print_node->children.size(); i++) {
                    // заполняем очередь дочерним узлом
                    q.push(print_node->children[i]);
                    // и считаем количество элементов в узле
                    prev_num_elem_in_node++;
                }
            }
            std::cout << std::endl;
        }
    }


private:
    // Проверка узла на переполнение
    bool is_node_full(Node *node) {
        return node->keys.size() == 2 * t - 1;
    }

    // Разбиение узла
    void split(Node *node, size_t index) {
        node->children.resize(node->children.size() + 1);
        node->keys.resize(node->keys.size() + 1);
        // записываем данные старого дочернего узла в новывй
        for (int i = node->children.size() - 1; i > index; i--)
            node->children[i] = node->children[i - 1];
        for (int i = node->keys.size() - 1; i > index; i--)
            node->keys[i] = node->keys[i - 1];

        // части разбитого узла
        Node *left_node = node->children[index];
        Node *right_node = new Node(left_node->leaf);
        // отправляем элемент в корень
        node->keys[index] = left_node->keys[t - 1];

        // формируем правый узел-ребенок
        node->children[index + 1] = right_node;
        for (int i = t; i < 2 * t - 1; i++)
            right_node->keys.push_back(left_node->keys[i]);
        //и левый
        left_node->keys.resize(t - 1);
        if (!left_node->leaf) {
            for (int i = t; i < 2 * t; i++)
                right_node->children.push_back(left_node->children[i]);
            left_node->children.resize(t);
        }
    };

    // Добавление элемента
    void insert_non_full(Node *node, const Key &key) {
        int pos = node->keys.size() - 1;
        if (node->leaf) {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && key < node->keys[pos]) {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            // добавляем элемент
            node->keys[pos + 1] = key;
        }
        else {
            while (pos >= 0 && key < node->keys[pos])
                pos--;

            if (is_node_full(node->children[pos + 1])) {
                split(node, pos + 1);
                if (key > node->keys[pos + 1])
                    pos++;
            }
            insert_non_full(node->children[pos + 1], key);
        }
    }

    Node *root;
    Comparator comp;
    size_t t;

};

void run(size_t t) {
    // инициализация дерева
    BTree<int> b_tree(t);
    // добавление элементов
    int elem;
    while (std::cin >> elem)
        b_tree.insert(elem);
//    b_tree.insert(0);
//    b_tree.insert(1);
//    b_tree.insert(2);
//    b_tree.insert(3);
//    b_tree.insert(4);
//    b_tree.insert(5);
//    b_tree.insert(6);
//    b_tree.insert(7);
//    b_tree.insert(8);
//    b_tree.insert(9);
    // вывод результатов
    b_tree.res_output();
}

int main() {
    // минимальный порядок дерева
    size_t t;
    std::cin >> t;
    // реализация
    run(t);

    return 0;
}
