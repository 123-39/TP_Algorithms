/*
4_2. Порядковые статистики. Дано число N и N строк.
 Каждая строка содержит команду добавления или удаления натуральных чисел,
 а также запрос на получение k-ой порядковой статистики.
 Команда добавления числа A задается положительным числом A,
 команда удаления числа A задается отрицательным числом “-A”.
 Запрос на получение k-ой порядковой статистики задается числом k.
Требования: скорость выполнения запроса - O(log n).
 */
#include <iostream>


template <class T>
struct DefaultComparator
{
    int operator () (const T& l, const T& r) const
    {
        if ( l < r ) return -1;
        if ( l > r ) return 1;
        return 0;
    }
};

template <class Key, class Comparator = DefaultComparator<Key> >
class AVLTree {
    struct Node {
        Key key;
        size_t count;

        // высота дерева
        uint8_t height;

        // указатель на левого и правого ребенка
        Node *left;
        Node *right;

        // конструктор
        explicit Node(const Key& key) :
            key(key),
            count(1),
            height(1),
            left(nullptr),
            right(nullptr)
            {
            }

        // деструктор
        ~Node()
        {
            delete left;
            delete right;
        }
    };
public:
    AVLTree(Comparator comp = Comparator()) :
    comp(comp), root(nullptr) {
    }

    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator =(const AVLTree&) = delete;
    AVLTree& operator =(AVLTree&&) = delete;

    ~AVLTree() {
        delete root;
    };

    void insert(const Key& key) {
        root = insert_aux(key, root);
    };

    void erase(const Key& key) {
        root = erase_aux(key, root);
    };

    int kth_statistic(const int k)
    {
        return kth_statistic_aux(k, root);
    }

private:
    int kth_statistic_aux(const int k, Node* node)
    {
        int i = k - nodes_count(node->left);
        if (i < 0)
            return kth_statistic_aux(k, node->left);
        else if (i > 0)
            return kth_statistic_aux(i - 1, node->right);
        return node->key;
    }

    int nodes_count(const Node *node) const
    {
        if (!node)
            return 0;
        return node->count;
    }

    // вставка элемента
    Node* insert_aux(const Key& key, Node* node) {
        // если дерево (поддервево) пустое, создаем его с данными ключом и значением
        if (!node) {
            return new Node(key);
        }
        // определем куда будем вставлять элемент
        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { // <
            // вставляем влево
            node->left = insert_aux(key, node->left);
        }
        else if (cmp_res == 1) { // >
            // вставляем вправо
            node->right = insert_aux(key, node->right);
        }
        // приводим AVL дерево к нормальной форме
        return balance(node);
    }


    // удаление элемента
    Node* erase_aux(const Key& key, Node* node) {
        // проверка на пустоту (присуотсвует ли удаляемая вершина)
        if (!node) {
            return nullptr;
        }
        // ищем вершину, которую требуется удалить
        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { // <
            // опускаемся влево
            node->left = erase_aux(key, node->left);
        }
        else if (cmp_res == 1) { // >
            // опускаемся вправо
            node->right = erase_aux(key, node->right);
        }
        // нашли вершину, которую нужно удалить
        else {
            // запоминаем детей
            Node* left = node->left;
            Node* right = node->right;

            node->right = nullptr;
            node->left = nullptr;

            // удаляем вершину
            delete node;
            // ставим на место старой вершины левого ребенка
            if (!right) {
                return left;
            }
            // ищем минимум правого ребенка и ставим на место старой вершины
            Node* min_node;
            right = find_and_remove_min(right, min_node);
            min_node->left = left;
            min_node->right = right;

            return balance(min_node);
        }
        // приводим AVL дерево к нормальной форме
        return balance(node);
    }

    Node* find_and_remove_min(Node* node, Node *&min_node)
    {
        if (node->left == nullptr)
        {
            min_node = node;
            return node->right;
        }
        node->left = find_and_remove_min(node->left, min_node);
        return balance(node);
    }

    // Чтобы каждый раз не писать if
    uint8_t height(Node* node) {
        return node ? node->height : 0;
    }

    //
    void fix_height(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
        node->count = 1 + nodes_count(node->left) + nodes_count(node->right);
    }

    // Проверям нужно ли балансировать (если 2 или -2)
    int bfactor(Node* node) {
        return height(node->right) - height(node->left);
    }

    // Малое правое и левое вращение
    Node* rotate_left(Node* node) {
        Node* top = node->right;
        node->right = top->left;
        top->left = node;

        fix_height(node);
        fix_height(top);
        return top;
    };
    Node* rotate_right(Node* node) {
        Node* top = node->left;
        node->left = top->right;
        top->right = node;

        fix_height(node);
        fix_height(top);
        return top;
    };

    // Приводим высоту дерева к адкватной форме (разрыв не больше 2)
    Node* balance(Node* node) {
        fix_height(node);
        int bf = bfactor(node);
        if (bf == 2) {
            if (bfactor(node->right) < 0) { // большой поворот
                node->right = rotate_right(node->right);
            }
            // малый
            return rotate_left(node);
        }
        else if (bf == -2) {
            if (bfactor(node->left) > 0) { // большой поворот
                node->left = rotate_left(node->left);
            }
            // малый
            return rotate_right(node);
        }
        return node;
    }

    Node* root;
    Comparator comp;
};

// Реализация
void run(int* res, size_t N) {
    AVLTree<int> avl_tree;
    for (int i = 0; i < N; ++i) {
        int command = 0;
        int k_statistic = 0;
        std::cin >> command >> k_statistic;
        if (command >= 0)
            avl_tree.insert(command);
        else
            avl_tree.erase(-command);
        res[i] = avl_tree.kth_statistic(k_statistic);
    }
}
// Результаты
void result_output(int* res, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        std::cout << res[i] << "\n";
    }
}



int main() {
    // количество строк
    size_t N;
    std::cin >> N;
    // результаты
    int *res = new int[N];
    // реализация
    run(res, N);
    // вывод результатов
    result_output(res, N);
    // очистка памяти
    delete[] res;
    res = nullptr;

    return 0;
}
