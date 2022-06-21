
#include <iostream>
#include <stack>

template <class T>
struct DefaultComparator
{
    int operator () (const T &first, const T &second) const
    {
        return first > second;
    }
};

template <class T, class Comparator = DefaultComparator<T> >
class BinaryTree {
    struct Node {
        T key;
        Node *left;
        Node *right;

        explicit Node(const T &key) : key(key), left(nullptr), right(nullptr) {}
    };

public:
    explicit BinaryTree(Comparator comp = Comparator()) : root(nullptr), comp(comp) {}

    ~BinaryTree() = default;

    // Добавление элемента в дерево
    void add(const T &key) {
        Node **tmp = &root;
        while (*tmp) {
            Node& node = **tmp;
            tmp = comp(key, node.key) ? &node.right : &node.left;
        }
        *tmp = new Node(key);
    }
    // Вывод элементов in-order (без рекурсии реально больно)
    void in_order_output() {
        std::stack<Node*> stack_tree;
        Node *children;
        Node *tmp = root;

        while (!stack_tree.empty() || tmp) {
            // двигаемся вниз (в левого ребенка) и добавляем узлы в стек
            if (tmp) {
                stack_tree.push(tmp);
                tmp = tmp->left;
            }
            else {
                // когда достигаем узла, у которого нет левого ребенка, берем последний узел
                children = stack_tree.top();
                // если в узле есть правый ребенок, то переводим указатеь в него
                if (children->right)
                    tmp = children->right;
                // уадляем из стека узел
                stack_tree.pop();
                // выводим
                std::cout << children->key << " ";
            }
        }
    }
    // Удаление дерева
    void clear_tree() {
        std::stack<Node*> stack_tree;
        Node *children;

        while (!stack_tree.empty() || root) {
            // двигаемся вниз (в левого ребенка) и добавляем узлы в стек
            if (root) {
                stack_tree.push(root);
                root = root->left;
            }
            else {
                // когда достигаем узла, у которого нет левого ребенка, берем последний узел
                children = stack_tree.top();
                // если в узле есть правый ребенок, то переводим указатеь в него
                if (children->right)
                    root = children->right;
                // уадляем из стека узел
                stack_tree.pop();
                // удаляем узел
                delete children;
            }
        }
    }

private:
    Node *root;
    Comparator comp;
};

// Реализация
void run(size_t N) {
    // создаем дерево
    BinaryTree<int> bin_tree;
    // добавляем элементы
    int num = 0;
    for (size_t i = 0; i < N; ++i) {
        std::cin >> num;
        bin_tree.add(num);
    }
    // выводим поперечным обходм
    bin_tree.in_order_output();
    // удаляем дерево
    bin_tree.clear_tree();
}

int main() {
    // количество элементов
    size_t N;
    std::cin >> N;
    // реализация
    run(N);

    return 0;
}