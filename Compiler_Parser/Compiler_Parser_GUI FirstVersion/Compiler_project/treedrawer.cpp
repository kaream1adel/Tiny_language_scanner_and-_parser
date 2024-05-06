// treedrawer.cpp
#include "treedrawer.h"
#include <QPainter>

template <typename T>
TreeDrawer<T>::TreeDrawer(QWidget* parent, Tree<T>* tree)
    : TreeDrawerBase(parent), tree(tree) {
}

template <typename T>
void TreeDrawer<T>::setTree(Tree<T>* newTree) {
    tree = newTree;
    update(); // Request a repaint when the tree changes
}

template <typename T>
void TreeDrawer<T>::paintEvent(QPaintEvent* event) {
    TreeDrawerBase::paintEvent(event); // Call the base class implementation
    if (!tree)
        return;

    QPainter painter(this);

    // Set up initial coordinates and level for the root node
    int x = width() / 2;
    int y = 50;
    int level = 0;

    // Start drawing the tree from the root
    drawTree(tree->getRoot(), painter, x, y, level);
}

template <typename T>
void TreeDrawer<T>::drawTree(TreeNode<T>* node, QPainter& painter, int x, int y, int level) {
    // ... Implement the drawing logic as before
}

// Explicit instantiation for the types you plan to use
template class TreeDrawer<int>;
