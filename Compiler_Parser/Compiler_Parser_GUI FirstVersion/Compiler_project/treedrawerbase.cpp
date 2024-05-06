// treedrawerbase.cpp
#include "treedrawerbase.h"

TreeDrawerBase::TreeDrawerBase(QWidget* parent) : QWidget(parent) {}

TreeDrawerBase::~TreeDrawerBase() {}

void TreeDrawerBase::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    // Implement the common painting logic here
}
