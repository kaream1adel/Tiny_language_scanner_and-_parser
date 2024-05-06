// #ifndef SYNTAXTREEWIDGET_H
// #define SYNTAXTREEWIDGET_H

// #include <QGraphicsView>
// #include <QGraphicsScene>
// #include <QGraphicsItem>
// #include <QGraphicsTextItem>
// #include <QPointF>
// #include <QVector>
// #include "parsing.h"

// class SyntaxTreeWidget : public QGraphicsView
// {
// public:
//     SyntaxTreeWidget(QGraphicsView* graphicsView, QWidget* parent = nullptr)
//         : QGraphicsView(parent), graphicsView(graphicsView)
//     {
//         scene = new QGraphicsScene(this);
//         graphicsView->setScene(scene);
//     }

//     void drawTree(Node* root)
//     {
//         scene->clear();
//         drawNode(root, nullptr, width() / 2, 50);
//     }

// private:
//     QGraphicsScene* scene;
//     QGraphicsView* graphicsView;

//     void drawNode(Node* node, QGraphicsItem* parentItem, qreal x, qreal y)
//     {
//         QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::fromStdString(node->getTokenType()), parentItem);
//         textItem->setPos(x - textItem->boundingRect().width() / 2, y);

//         qreal childX = x - 50 * (node->getChildren().size() - 1) / 2;
//         qreal childY = y + 50;

//         for (Node* child : node->getChildren())
//         {
//             drawNode(child, textItem, childX, childY);
//             childX += 50;
//         }
//     }
// };

// #endif // SYNTAXTREEWIDGET_H
