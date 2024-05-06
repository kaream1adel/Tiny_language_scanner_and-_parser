#ifndef TREETYPES_H
#define TREETYPES_H

#include <QVector>

template <typename T>
struct TreeNode {
    T data;
    QVector<TreeNode*> children;

    TreeNode(const T& value) : data(value) {}
};

#endif // TREETYPES_H
