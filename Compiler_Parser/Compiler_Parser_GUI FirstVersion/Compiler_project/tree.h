// tree.h
#ifndef TREE_H
#define TREE_H

#include "treetypes.h"

    template <typename T>
    class Tree {
public:
    Tree();
    ~Tree();

    TreeNode<T>* getRoot() const;

private:
    TreeNode<T>* root;

    // You may need additional functions for tree construction and destruction
};

#endif // TREE_H
