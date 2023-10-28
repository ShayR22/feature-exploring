#include "btree.hpp"
#include <stdio.h>

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
    btree.add(5);
    btree.add(3);
    btree.add(1);
    btree.add(4);
    btree.add(7);

    // my_dfs(btree.get_head());
    // printf("\n\n\n");
    auto print_node = [](const Node<int>* node) {
        printf("%d\n", node->data());
    };

    auto dfs_iter = DFSIterator(btree);
    dfs_iter.iterate(print_node);
}