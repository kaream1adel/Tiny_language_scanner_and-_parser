// treedrawerbase.h
#ifndef TREEDRAWERBASE_H
#define TREEDRAWERBASE_H

#include <QWidget>

class TreeDrawerBase : public QWidget {
    Q_OBJECT

public:
    TreeDrawerBase(QWidget* parent = nullptr);
    virtual ~TreeDrawerBase();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TREEDRAWERBASE_H
