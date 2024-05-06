// tree.cpp
#include "tree.h"

template <typename T>
Tree<T>::Tree() : root(nullptr) {
    // You can implement tree construction logic here
}

template <typename T>
Tree<T>::~Tree() {
    // You can implement tree destruction logic here
}

template <typename T>
TreeNode<T>* Tree<T>::getRoot() const {
    return root;
}

// Explicit instantiation for the types you plan to use
template class Tree<int>; // Example for integer tree
