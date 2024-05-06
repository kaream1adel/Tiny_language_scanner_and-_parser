#ifndef NODES_H
#define NODES_H

class Node
{
public:
    // identifier name, using when node is assign or read
    // to detect which variable is assigned or read
   // Token id_op_label;

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

    void printTree(int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }
        cout << token.type;
        if (!token.value.empty()) {
            cout << " (" << token.value << ")";
        }
        if(!id_op_label.value.empty()){
            cout << " {" << id_op_label.value << "}";
        }
        cout << endl;

        for (Node* child : children) {
            child->printTree(depth + 1);
        }
    }

private:
    Token token;
    vector<Node*> children;

};

#endif // NODES_H
