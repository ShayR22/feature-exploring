#include "btree.hpp"
#include <array>
#include <cstdio>

void my_dfs(const std::unique_ptr<Node<int>>& node) {
    if (node == nullptr) {
        return;
    }
    printf("%d\n", node->data());
    my_dfs(node->left());
    my_dfs(node->right());
}

int main() {
    BTree<int> btree{};
    static constexpr auto NUM_RANDOM_NUMBERS = 5;
    static constexpr std::array<int, NUM_RANDOM_NUMBERS> RANDOM_VALUES = {
        5, 3, 1, 4, 7
    };

    for (const auto& val: RANDOM_VALUES) {
        btree.add(val);
    }

    auto print_node = [](const Node<int>* node) {
        printf("%d\n", node->data());
    };

    auto dfs_iter = DFSIterator(btree);
    dfs_iter.iterate(print_node);
}