#ifndef mainwindow
#define mainwindow

#include <QMainWindow>
#include <string>
#include <queue>
#include <QQueue>
#include <QString>
// #include "syntaxtreewidget.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPointF>
#include <QVector>

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <queue>
#include "scanner.h"

#include <vector>





using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{


public:
    QString inputText;
    QList <QGraphicsItem*> itemList;
    std::queue<Token> scannerTokens;
    class Token {
    public:
        std::string type;
        std::string value;
    };

    MainWindow(QWidget *parent = nullptr);
    bool parseButtonClicked, ScannerBtnClicked=false;


    void processAndDrawSyntaxtree();
    void clearAll();
    void openFile();

    ~MainWindow();

    std::queue<MainWindow::Token> get_tokens( std::string& tiny);
    Ui::MainWindow *ui;
    // SyntaxTreeWidget* syntaxTreeWidget;

    virtual void paintEvent(QPaintEvent *event);


public slots:
    void onParseButtonClicked();


private slots:
    void on_scannerBtn_clicked();

    // void on_pushButton_2_clicked();

    // void on_clearBtn_clicked();

    // void on_pushButton_3_clicked();

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;

};


#endif // MAINWINDOW_H
