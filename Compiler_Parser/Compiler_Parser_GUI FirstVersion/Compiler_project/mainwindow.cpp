#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QQueue>
#include <QWidget>
#include <QPainter>
#include <queue>
#include <stack>
//#include "Parsing.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <string>
#include <stdio.h>
#include <fstream>
#include <queue>
#include "scanner.h"
#include <vector>
#include <QGraphicsView>
#include <QGraphicsScene>


using namespace std;

QPen NextToMe(Qt::green), belowMe(Qt::red), RectanglePen(Qt::black), EllipsePen(Qt::blue);
QFont heading1Font("Arial", 24, QFont::Bold), buttonFont("Times New Roman", 12, QFont::Bold);

int initialX, initialY, xOffset=20, yOffset=5;
bool firstTime=true;
const int Xlength=75, Ylength=50;

string shapeType[] ={"Rectangle", "Ellipse"}, dummy="dummy";
struct FarouqianNode{
    int typeOfShape;            // 0 for Recangle, 1 for Ellipse
    string title, subTitle;
    int x_coordinate, y_coordinate, length, height, depth, parentX, parentY;

    FarouqianNode (int type, string t, string subT, int x, int y, int l, int h, int d){
        typeOfShape = type;
        title = t;
        subTitle = subT;
        x_coordinate = x;
        y_coordinate = y;
        length = l;
        height = h;
        depth = d;
    }
};
struct FarouqianEdge{
    int x, y, px, py;

    FarouqianEdge(int x1, int y1, int x2, int y2){
        x=x1;
        y=y1;
        px=x2;
        py=y2;
    }
};
vector<FarouqianNode> nodes;
vector<FarouqianEdge> edges;
stack<pair<int, int>>parentsPoints;
int getType(string&s){
    if(s=="ReadStmt" || s=="IfStmt" || s=="AssignStmt" || s=="WriteStmt" || s=="RepeatStmt") return 0;      // rectangle
    return 1;                                                                                               // ellipse
}
string getValue(string&s){
    if (s=="ReadStmt") return "read";
    if (s=="IfStmt") return "if";
    if (s=="AssignStmt") return "assign";
    if (s=="WriteStmt") return "write";
    if (s=="RepeatStmt") return "repeat";
    if (s=="LESSTHAN" || s=="EQUAL" || s=="PLUS" || s=="MINUS" || s=="MULT" || s=="DIV") return "op";
    if (s=="IDENTIFIER") return "id";
    if (s=="NUMBER") return "const";
    return "undefined";

}
class Node
{
public:
    // identifier name, using when node is assign or read
    // to detect which variable is assigned or read
    Token id_op_label;
    Node* neighbour = NULL;

    Node(Token tok) : token(tok) {}
    Node(string nodeType, string nodeValue){
        token.type = nodeType;
        token.value = nodeValue;
    }
    Node(string nodeType){
        token.type = nodeType;
    }

    void addChild(Node* child) {
        children.push_back(child);
    }

    void printTree(int curY=100, int depth = 0) {
        cout<<initialX<<" "<<curY<<" "<<initialX+xOffset<<" "<<curY+yOffset<<endl;
        string t=(!id_op_label.value.empty()? id_op_label.value: token.value);
        if(!t.empty()) t="("+t+")";
        nodes.push_back(FarouqianNode(getType(token.type), getValue(token.type), t, initialX, curY, Xlength, Ylength, depth));
        if(!firstTime) edges.push_back(FarouqianEdge(parentsPoints.top().first+Xlength/2, parentsPoints.top().second+Ylength/2, initialX+Xlength/2, curY+Ylength/2));

        parentsPoints.push({initialX, curY});
        firstTime=false;

        for (int i = 0; i < depth; ++i) {
            cout << "   ";
        }
        cout << token.type;
        if (!token.value.empty()) {
            cout << " (" << token.value << ")";
        }
        if(!id_op_label.value.empty()){
            cout << " {" << id_op_label.value << "}";
        }
        cout << endl;
        int i=0;
        for (Node* child : children) {
            initialX+=(i++?125:0);
            child->printTree(curY+125, depth + 1);
            parentsPoints.pop();
        }
        if(neighbour != NULL){
            initialX+=125;
            neighbour->printTree(curY, depth);
            parentsPoints.pop();
        }
    }

private:
    Token token;
    vector<Node*> children;

};
scanner scan;
class Parser
{
public:
    Parser(){}

    Parser(queue<Token> tokensQ){
        //		vector<Node*> tokensV;
        while (!tokensQ.empty())
        {
            tokens.push_back(tokensQ.front());
            tokensQ.pop();
        }
        //		tokens = tokensV;
        currentTokenIndex = 0;
    }
    Parser(vector<Token>& tokens) : tokens(tokens), currentTokenIndex(0) {}

    void getTokens(queue<Token>tokensQ){
        //		vector<Node*> tokensV;
        tokens.clear();
        while (!tokensQ.empty())
        {
            tokens.push_back(tokensQ.front());
            tokensQ.pop();
        }
        //		tokens = tokensV;
        currentTokenIndex = 0;
    }

    Node* parse() {
        return program();
    }

    void printVector(){
        for (auto t : tokens){
            cout << t.type << ", " << t.value << "\n";
        }
    }
    vector<Token> tokens;
    size_t currentTokenIndex;

    Token getCurrentToken() {
        if (currentTokenIndex < tokens.size()) {
            return tokens[currentTokenIndex];
        } else {
            Token end;
            end.type = "END_OF_FILE";
            return end;
        }
    }
    Token match(string expectedType) {
        Token currentToken = getCurrentToken();
        if (currentToken.type.compare(expectedType) == 0) {
            currentTokenIndex++;
            return currentToken;
        } else {
            throw std::runtime_error("Syntax error: Expected " + expectedType + ", found " + currentToken.type + "{ "+ currentToken.value + " }" );
            //			exit(EXIT_FAILURE);
        }
    }

    string getTokenTypeString(string type) {
        if(type.compare("SEMICOLON") == 0)
            return ";";
        else if(type.compare("IF") == 0)
            return "if";
        else if(type.compare("THEN") == 0)
            return "then";
        else
            return "UNKNOWN";
    }

    Node* program() {
        Node* stmtSeq = stmtSequence() ;
        if(currentTokenIndex != tokens.size()){
            cerr << "Syntax error: Unexpected token " << getCurrentToken().value << endl;
            throw std::runtime_error("Syntax error: Unexpected token " + getCurrentToken().value + "{ " + getCurrentToken().type + " }");

            //exit(EXIT_FAILURE);
        }
        return stmtSeq;
    }

    Node* stmtSequence() {

        Node* start= statement() ;
        Node* temp= start ;
        while (getCurrentToken().type.compare("SEMICOLON")==0) {
            match("SEMICOLON");
            Node* newtemp =statement();
            temp->neighbour = newtemp;
            temp = newtemp;
        }
        return start ;
    }

    Node* statement() {
        string cur = getCurrentToken().type;
        if (cur.compare("IF") == 0)
            return ifStmt();
        else if(cur.compare("REPEAT") == 0)
            return repeatStmt();
        else if (cur.compare("IDENTIFIER")== 0)
            return assignStmt();

        else if (cur.compare("READ")== 0)
            return readStmt();

        else if (cur.compare("WRITE")== 0)
            return writeStmt();
        else{
            cerr << "Syntax error: Unexpected token " << getCurrentToken().value << endl;
            throw std::runtime_error("Syntax error: Unexpected token " + getCurrentToken().value + "{ " + getCurrentToken().type + " }");

            //exit(EXIT_FAILURE);
        }
    }

    Node* ifStmt() {
        match("IF");
        Token t;
        t.type = "IfStmt";
        Node* ifStmtNode = new Node(t);
        ifStmtNode->addChild(exp());
        match("THEN");
        ifStmtNode->addChild(stmtSequence());
        if (getCurrentToken().type.compare("ELSE") == 0) {
            match("ELSE");
            ifStmtNode->addChild(stmtSequence());
        }
        match("END");
        return ifStmtNode;
    }

    Node* repeatStmt() {
        match("REPEAT");
        Token t;
        t.type = "RepeatStmt";
        Node* repeatStmtNode = new Node(t);
        repeatStmtNode->addChild(stmtSequence());
        match("UNTIL");
        repeatStmtNode->addChild(exp());
        return repeatStmtNode;
    }

    Node* assignStmt() {
        Token id = match("IDENTIFIER");
        match("ASSIGN");
        Token t;
        t.type = "AssignStmt";
        t.value = getCurrentToken().value;
        Node* assignStmtNode = new Node(t);
        //        match("IDENTIFIER");
        assignStmtNode->addChild(exp());
        assignStmtNode->id_op_label = id;
        return assignStmtNode;
    }

    Node* readStmt() {
        match("READ");
        Token t;
        t.type = "ReadStmt";
        Node* readStmtNode = new Node(t);
        //		Token t2;
        //		t2.type = "Identifier";
        //		t2.value = getCurrentToken().value;
        //		readStmtNode->addChild(new Node(getCurrentToken()));
        Token id = match("IDENTIFIER");
        readStmtNode->id_op_label = id;
        return readStmtNode;
    }

    Node* writeStmt() {
        match("WRITE");
        Token t;
        t.type = "WriteStmt";
        Node* writeStmtNode = new Node(t);
        writeStmtNode->addChild(exp());
        return writeStmtNode;
    }

    Node* exp() {

        Node* left =simpleExp(); //0

        if(!getCurrentToken().type.compare("LESSTHAN") || !getCurrentToken().type.compare("EQUAL")) {
            Node* operand = new Node(getCurrentToken());

            match(getCurrentToken().type);

            Node* right = simpleExp(); //x

            operand->addChild(left) ;
            operand->addChild(right);
            left = operand;

        }
        return left ;
    }

    Node* simpleExp() {
        Node* base = term();
        Node* left = base ;
        while (!getCurrentToken().type.compare("PLUS") || !getCurrentToken().type.compare("MINUS")) {
            Node* addopnode = new Node(getCurrentToken());
            match(getCurrentToken().type);

            addopnode->addChild(left);
            Node* right = term() ;
            addopnode->addChild(right);
            left = addopnode ;
        }
        return left;
    }

    Node* term() {
        Node* base = factor();
        Node* left = base ;
        while (!getCurrentToken().type.compare("MULT") || !getCurrentToken().type.compare("DIV")) {
            Node* mulopnode = new Node(getCurrentToken());
            match(getCurrentToken().type);

            mulopnode->addChild(left);
            Node* right = factor() ;
            mulopnode->addChild(right);
            left=mulopnode ;
        }
        return left;
    }

    Node* factor() {
        Node* factorNode = nullptr;
        if (!getCurrentToken().type.compare("OPENBRACKET")) {
            match("OPENBRACKET");
            factorNode = exp();
            match("CLOSEDBRACKET");
        } else if (!getCurrentToken().type.compare("NUMBER") || !getCurrentToken().type.compare("IDENTIFIER")) {
            factorNode = new Node(getCurrentToken());
            match(getCurrentToken().type);
        } else {
            throw std::runtime_error("Syntax error: Unexpected token " + getCurrentToken().value + "{ " + getCurrentToken().type + " }");
            //			exit(EXIT_FAILURE);
        }
        return factorNode;
    }

};
Parser p;
Node* tree;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nodes.push_back(FarouqianNode(getType(dummy), getValue(dummy), dummy, initialX-50, initialY-50, 1, 1, 0));

    ui->label->setFont(heading1Font);
    ui->label_2->setFont(heading1Font);
    ui->label_3->setFont(heading1Font);
    ui->tabWidget->setStyleSheet("QTabBar::tab:first { min-width: 200px; }QTabBar::tab:second { min-width: 200px; }QTabBar::tab:third { min-width: 200px; }");
    ui->clearBtn->setFont(buttonFont);
    ui->scannerBtn->setFont(buttonFont);
    ui->pushButton->setFont(buttonFont);
    ui->uploadFile->setFont(buttonFont);


    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onParseButtonClicked);
    connect(ui->scannerBtn, &QPushButton::clicked, this, &MainWindow::on_scannerBtn_clicked);
    connect(ui->clearBtn, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(ui->uploadFile, &QPushButton::clicked, this, &MainWindow::openFile);
}

void MainWindow::onParseButtonClicked() {
    // Set the flag to indicate the "Parse" button has been clicked.
    if(ScannerBtnClicked){
        // Call the text processing slot immediately after the button is clicked.
        parseButtonClicked = true;
        processAndDrawSyntaxtree();
    }
    else QMessageBox::critical(0, "Error", "Please, scan your code first!");
}

void MainWindow :: paintEvent(QPaintEvent *event){

}



void MainWindow::clearAll(){
    ui->textEdit->clear();
    ui->scannerTxt->clear();

    ui->textEdit->setFocus();

    if(!(nodes.size()==1 && nodes[0].title == "undefined")){
        edges.clear();
        nodes.clear();



        ui->graphicsView->clearMask();

        QGraphicsItem* item;

        if(item->scene() == nullptr){
            qDebug() << "Scene is empty";
            QMessageBox::information(nullptr, "Done", "ALL tabs are cleared");
            return;
        }

        scene->clear();

        view->viewport()->update();

        nodes.push_back(FarouqianNode(getType(dummy), getValue(dummy), dummy, initialX-50, initialY-50, 1, 1, 0));      // as a flag to enter this if condition


        /*
        //          didn't work too (segmentation error in scene->items())
        QList <QGraphicsItem*> itemList2 = scene->items();

        while(!itemList2.isEmpty()){
            delete itemList2.first();
            itemList2 = scene->items();
        }

        //    while( ! Items.empty() ) {
        //       scene->removeItem( Items.first() );
        //       delete Items.first();
        //       Items.removeFirst();
        //    }
        view->viewport()->update();
        */

        /*
        //          the scene isn't cleared by this method too

        QGraphicsScene* scene2 = new QGraphicsScene();

        // Switch Scene to the new Scene
        scene = scene2;
        */
    }
}

void MainWindow::processAndDrawSyntaxtree() {
    // Check if the user stopped typing and the "Parse" button has been clicked.
    // Get the input text from QTextEdit.
    QString inputText = ui->textEdit->toPlainText();
    string inputStr = inputText.toStdString();

    // now the inputSTR contain the user input from gui as a string
     scannerTokens = scan.get_tokens(inputStr);
    // now the scannerTokens contain the output of the scanner which is a queue of tokens

    //todo : create the parse tree then draw it

     try {

         initialX = 100, initialY = 100, firstTime=true;
         // Call your function
         // Parser p = Parser(scannerTokens);           done down in the scannerOnClickFunc
         //	p.parse();

         nodes.clear();
         edges.clear();

         //nodes.push_back(FarouqianNode(getType(dummy), getValue(dummy), dummy, initialX-50, initialY-50, 1, 1, 0));
         // Node* tree = p.parse();                 down in the onClick scan butn
         cout << "print Tree\n";
         //	n.printTree();
         //	p.printVector();


         //     preprocessing of edges and nodes positions
         tree->printTree();
         cout << "END print Tree\n";

         //     create your view and scene
         view = new QGraphicsView(this);
         // setCentralWidget(view);
         scene = new QGraphicsScene(this);
         view->setScene(scene);
         // // view->setRenderHint(QPainter::Antialiasing); // Optional: Enable antialiasing for smoother rendering
         view->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align the scene to the top-left corner

         ui->graphicsView->setScene(scene);

         for(auto edge: edges){
             /*
             // this code only prints on the whole screen not the QGraphicsView as I want
             QLineF l(edge.x, edge.y, edge.px,edge.py);
             QPainter painter(this);
             painter.setPen(NextToMe);
             painter.drawLine(l);
             */


             QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(edge.x, edge.y, edge.px,edge.py));
             scene->addItem(line);
             itemList.append(line);

            //                     هنحتاج نعمل حاجة زي كده لو عايزين نفرق بين لون الneighbours and children
             // if(edge.isFriend)
             //     parsetree->addEdgeFriend(edge.x1, edge.y1, edge.x2, edge.y2);
             // else if(!edge.isFriend)
             //     parsetree->addEdgeChild(edge.x1, edge.y1, edge.x2, edge.y2);
         }
         for(auto&FNode: nodes){
             if(FNode.typeOfShape && FNode.title !="undefined"){                                // 1 -> ellipse
                 QGraphicsEllipseItem* ell = new QGraphicsEllipseItem(FNode.x_coordinate, FNode.y_coordinate, FNode.length, FNode.height);
                 ell->setPen(EllipsePen);
                 ell->setBrush(Qt::white);
                 string txt=(FNode.title+"\n  "+FNode.subTitle);
                 QGraphicsTextItem* txtItem = new QGraphicsTextItem();
                 txtItem->setDefaultTextColor(Qt::black);
                 txtItem->setPlainText(QString::fromStdString(txt));
                 txtItem->setPos(FNode.x_coordinate+20, FNode.y_coordinate+5);
                 scene->addItem(ell);
                 scene->addItem(txtItem);
                 itemList.append(txtItem);
             }
             else if(FNode.title!="undefined"){                                                 // 0 -> rectangle
                 QGraphicsRectItem* rect = new QGraphicsRectItem(FNode.x_coordinate, FNode.y_coordinate, FNode.length, FNode.height);
                 rect->setPen(RectanglePen);
                 rect->setBrush(Qt::white);
                 string txt=(FNode.title+"\n  "+FNode.subTitle);
                 QGraphicsTextItem* txtItem = new QGraphicsTextItem();
                 txtItem->setDefaultTextColor(Qt::black);
                 txtItem->setPlainText(QString::fromStdString(txt));
                 txtItem->setPos(FNode.x_coordinate+20, FNode.y_coordinate+5);
                 scene->addItem(rect);
                 scene->addItem(txtItem);
                 itemList.append(txtItem);
             }
             else {
                 QGraphicsLineItem* line = new QGraphicsLineItem( QLineF(0,0,10,10) );
                 scene->addItem(line);
                 itemList.append(line);
             }
             // for(auto node: nodes){
             //     if (node.Rect)
             //         parsetree->addRectangleNode(node.x, node.y, QString::fromStdString(node.text));
             //     if(!node.Rect)
             //         parsetree->addEllipseNode(node.x, node.y, QString::fromStdString(node.text));
             //     qDebug() << QString::fromStdString(node.text) << "\n";
             // }
         }

     } catch (const exception& e) {
         // Handle the exception gracefully
         QMessageBox::critical(0, "error", e.what());
         cerr << "Error: " << e.what() << endl;
     }



     // Parser parser= Parser(scannerTokens);

     // cout<<"\nHere are the values of the parser's tokens which must be the same as the scanner one\n";

     // for(auto&x:parser.tokens) cout<<x.type<<": "<<x.value<<endl;

    // Node* syntaxTreeRoot = parser.parse();

    // syntaxTreeRoot->printTree();
    /*
    view = new QGraphicsView(this);
    setCentralWidget(view);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing); // Optional: Enable antialiasing for smoother rendering
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align the scene to the top-left corner

    ui->graphicsView->setScene(scene);

    QGraphicsLineItem* line = new QGraphicsLineItem( QLineF(10,10,50,25) );
    scene->addItem(line);




    ellipse = scene->addEllipse(10, 10, 100, 100, EllipsePen);
    ellipse = scene->addEllipse(10, 50, 100, 100, EllipsePen);
    rectangle = scene->addRect(30, 30, 50, 50, RectanglePen);
    */


    //Display the syntax tree in the custom widget.
    // if (syntaxTreeRoot != nullptr) {
    //     syntaxTreeWidget->drawTree(syntaxTreeRoot);
    // } else {
    //     QMessageBox::warning(this, "Parsing Error", "Parsing failed. Unable to draw the tree.");
    // }

    // Reset the flag for the "Parse" button click.

    QMessageBox::information(0, "Succeeded", "Parsed Successfuly!");
    nodes.push_back(FarouqianNode(getType(dummy), getValue(dummy), dummy, initialX-50, initialY-50, 1, 1, 0));

    parseButtonClicked = false;
}


/*---------------------------------------------------------*/

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scannerBtn_clicked()
{
    cerr<<"I'm here2"<<endl;
    try {
        QString inputText = ui->textEdit->toPlainText();
        string inputStr = inputText.toStdString();

        // now the inputSTR contain the user input from gui as a string
        scannerTokens = scan.get_tokens(inputStr);

        p.getTokens(scannerTokens);

        tree = p.parse();

        // now the scannerTokens contain the output of the scanner which is a queue of tokens

        ScannerBtnClicked=true;
        string scannerResultStr="";
        auto tempTokens=scan.get_tokens(inputStr);
        while(!tempTokens.empty()){
            string temp=tempTokens.front().value+","+tempTokens.front().type+"\n";
            scannerResultStr+=temp;
            tempTokens.pop();
        }
        ui->scannerTxt->setText(QString::fromStdString(scannerResultStr));

        QMessageBox::information(0, "Succeeded", "Scanned Successfuly!");

        // // to clear the syntax tree
        // QGraphicsItem* item;

        // if(item->scene() == nullptr){
        //     scene->clear();
        //     view->viewport()->update();
        // }

    } catch (const exception& e) {
        // Handle the exception gracefully
        ScannerBtnClicked=false;
        ui->scannerTxt->clear();
        ui->textEdit->setFocus();
        QMessageBox::critical(0, "Error", e.what());
    }
}

void MainWindow::openFile(){

    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Open File"), "C://",
        "All Files (*.*);;Text Files (*.txt)");

    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "info", file.errorString());
        return;
    }

    ui->textEdit->clear();

    QTextStream in(&file);

    ui->textEdit->setText(in.readAll());
}

