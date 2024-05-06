// treedrawer.h
#ifndef TREEDRAWER_H
#define TREEDRAWER_H

#include "treedrawerbase.h"
#include "tree.h"

template <typename T>
class TreeDrawer : public TreeDrawerBase {
public:
    TreeDrawer(QWidget* parent = nullptr, Tree<T>* tree = nullptr);
    void setTree(Tree<T>* tree);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Tree<T>* tree;
    void drawTree(TreeNode<T>* node, QPainter& painter, int x, int y, int level);
};

#endif // TREEDRAWER_H
