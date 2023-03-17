#include <memory>
#include <concepts>
#include <stack>

template<typename T>
concept LessThanOperator = requires (T a, T b) {
    a < b;
};

template<typename T>
class Node {
public:
    Node(const T& data) : _data{data} {};
    void set_left(const T& data) {
        _left = std::make_unique<Node<T>>(data);
    }

    void set_right(const T& data) {
        _right = std::make_unique<Node<T>>(data);
    }

    std::unique_ptr<Node<T>>& left() { return _left; }
    std::unique_ptr<Node<T>>& right() { return _right; }
    T& data() { return _data; }

    const std::unique_ptr<Node<T>>& left() const { return _left; }
    const std::unique_ptr<Node<T>>& right() const { return _right; }
    const T& data() const { return _data; }
private:
    T _data;
    std::unique_ptr<Node<T>> _left{nullptr};
    std::unique_ptr<Node<T>> _right{nullptr};
};

template<LessThanOperator T>
class BTree {
public:
    BTree() = default;

    void add(const T& data) {
        if (_head == nullptr) {
            _head = std::make_unique<Node<T>>(data);
            return;
        }

        _add_helper(_head, data);
    }

    const std::unique_ptr<Node<T>>& get_head() const {
        return _head;
    }

private:
    std::unique_ptr<Node<T>> _head{nullptr};

    void _add_helper(std::unique_ptr<Node<T>>& node, const T& data) {
        if (data < node->data()) {
            std::unique_ptr<Node<T>>& left = node->left();
            if (left == nullptr) {
                left = std::make_unique<Node<T>>(data);
                return;
            }
            _add_helper(left, data);
        } else {
            std::unique_ptr<Node<T>>& right = node->right();
            if (right == nullptr) {
                right = std::make_unique<Node<T>>(data);
                return;
            }
            _add_helper(right, data);
        }
    }

};

template<LessThanOperator T>
class DFSIterator {
public:
    DFSIterator(const BTree<T>& btree) {
        _populate_stack(btree.get_head());
    }

    void iterate(std::invocable<const Node<T>*> auto&& lambda) {
        const Node<T>* node = next();
        while (node) {
            lambda(node);
            node = next();
        }
    }

private:
    std::stack<const Node<T>*> _stack{};

    void _populate_stack(const std::unique_ptr<Node<T>>& node) {
        if (node == nullptr) {
            return;
        }

        _populate_stack(node->right());
        _populate_stack(node->left());
        _stack.push(node.get());
    }

    const Node<T>* next() {
        if (_stack.empty()) {
            return nullptr;
        }

        const Node<T>* node = _stack.top();
        _stack.pop();
        return node;
    }
};