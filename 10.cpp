#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <typename T>
struct Node
{
    T info;
    Node* left;
    Node* right;
    long long label = 0;
    long long sumLabels = 0;
    long long leaves = 1;
    long long maxpathnum = 0;
    long long allNum = 0;

    Node(T i = T(), Node* l = nullptr, Node* r = nullptr) : info(i), left(l), right(r) {}
};

template <typename T>
class Tree
{
private:
    Node<T>* first;
    long long maxlen = 0;
    long long maxcol = 0;
public:
    Tree() :first(nullptr) {}
    Tree(const std::vector<T>& args) {
        long long c = args.size();
        while (c) {
            Add(args[args.size() - c]);
            c--;
        }
    }

    void Add(const T& i) {
        Node<T>** temp = &first;
        while (*temp) {
            if ((*temp)->info > i) temp = &(*temp)->left;
            else if ((*temp)->info < i)temp = &(*temp)->right;
            else
                return;
        }
        *temp = new Node<T>(i);
    }
    //прямой левый
    void Print1(ofstream& outputFile)
    {
        Print1(first, outputFile);
    }
    void Print1(Node<T>* a, ofstream& outputFile) const {
        if (a) {
            outputFile << a->info << '\n';
            if (a->left)Print1(a->left, outputFile);
            if (a->right)Print1(a->right, outputFile);
        }
    }
    //внутренний левый
    void Print2(ofstream& outputFile) {
        Print2(first, outputFile);
    }
    void Print2(Node<T>* a, ofstream& outputFile) const {
        if (a) {
            if (a->left)Print2(a->left, outputFile);
            outputFile << a->info << '\n';
            if (a->right)Print2(a->right, outputFile);
        }
    }

    Node<T>* Find(const T& i) const {
        Node<T>* temp = first;
        while (temp) {
            if (temp->info == i) return temp;
            if (temp->info > i) temp = temp->left;
            else temp = temp->right;
        }
        return nullptr;
    }

    bool Contains(const T& i) const {
        return Find(i);
    }
    void Algorithm() {
        UpdateLabels(first);
        maxPathNumWhereRoot(first);
        DescBranches(first);
        DelMaxMax(first, nullptr);
    }
    void DelMaxMax(Node<T>* node, Node<T>* parent) {
        if (node == nullptr) {
            return;
        }
        DelMaxMax(node->left, node);
        DelMaxMax(node->right, node);
        if (node->allNum == maxcol) {
            Delete(parent, node, node->info);
        }
    }
    void DescBranches(Node<T>* node) {
        if (!node)
            return;
        //учесть для детей если родитель корень
        if (node->left)
            node->left->allNum = node->maxpathnum;
        if (node->right)
            node->right->allNum = node->maxpathnum;
        if (node->left || node->right) {
            //если ветка разделяется на две "равных" то кол-во разных
            //путей делится пропорционально кол-ву листев в каждом поддереве
            if (node->left && node->right && node->left->label == node->right->label) {
                node->left->allNum += node->allNum * node->left->leaves / node->leaves;
                node->right->allNum += node->allNum * node->right->leaves / node->leaves;
            }
            //если одно большее поддерево то через него пройдет столько же
            else if ((node->left && !node->right) || (node->left && node->right && node->left->label > node->right->label))
                node->left->allNum += node->allNum;
            else if ((node->right && !node->left) || (node->left && node->right && node->left->label < node->right->label))
                node->right->allNum += node->allNum;
        }
        //если сам был корнем
        node->allNum += node->maxpathnum;
        maxcol = max(maxcol, node->allNum);
        DescBranches(node->left);
        DescBranches(node->right);
    }
    //если нода корень одного из больших полупутей
    void maxPathNumWhereRoot(Node<T>* node) {
        if (!node)
            return;
        if (node->sumLabels == maxlen) {
            //C n 1 = C n n-1 = n
            node->maxpathnum = (node->left? node->left->leaves: 1) * (node->right? node->right->leaves: 1);
        }
        maxPathNumWhereRoot(node->left);
        maxPathNumWhereRoot(node->right);
    }

    void UpdateLabels(Node<T>* node){
        if (!node) return;
        else if (!node->left && !node->right) {
            node->sumLabels = 0;
            node->label = 0;
        }
        else {
            long long leftLabel = -1;
            long long rightLabel = -1;
            if (node->left) {
                UpdateLabels(node->left);
                leftLabel = node->left->label;
            }
            if (node->right) {
                UpdateLabels(node->right);
                rightLabel = node->right->label;
            }
            node->label = max(leftLabel, rightLabel) + 1;
            if (leftLabel!=-1 && rightLabel!=-1) {
                node->sumLabels = leftLabel + rightLabel + 2;
                if(leftLabel == rightLabel)
                    node->leaves = node->left->leaves + node->right->leaves;
                else
                    node->leaves = leftLabel > rightLabel? node->left->leaves: node->right->leaves;
            }
            else {
                //-1
                node->sumLabels = max(leftLabel, rightLabel) + 1;
                node->leaves = node->left? node->left->leaves: node->right->leaves;
            }
        }
        maxlen = max(node->sumLabels, maxlen);
    }
    //правое удаление
    void Delete(T key) {
        Node <T>* parent = nullptr;
        Node<T>* temp = first;
        while (temp) {
            if (temp->info == key)
                break;
            if (temp->info > key) { parent = temp; temp = temp->left; }
            else { parent = temp; temp = temp->right; }
        }
        if (!temp)
            return;
        Delete(parent, temp, key);
    }

    void Delete(Node<T>* parent, Node<T>* temp, T key) {
        bool isleft;
        if (parent)
            isleft = (parent->left && parent->left->info == key);
        if (!temp->left && !temp->right) {
            if (parent)
                isleft ? parent->left = nullptr : parent->right = nullptr;
            else
                first = nullptr;
            delete temp;
        }
        else if ((temp->left && !temp->right)||(!temp->left && temp->right)) {
            if (parent) {
                if (temp->left)
                    isleft ? parent->left = temp->left : parent->right = temp->left;
                else
                    isleft ? parent->left = temp->right: parent->right = temp->right;
            }
            else
                first = temp->left ? temp->left : temp->right;
            delete temp;
        }
        else {
            auto predNextNode = temp;
            auto nextNode = temp->right;
            while (nextNode->left) {
                predNextNode = nextNode;
                nextNode = nextNode->left;
            }
            temp->info = nextNode->info;
            Delete(predNextNode, nextNode, nextNode->info);
        }
    }
};
int main() {
    ifstream inputFile("in.txt");
    auto tree = Tree<long long>();
    long long s;
    while (inputFile >> s) {
        tree.Add(s);
    }
    inputFile.close();
    ofstream outputFile("out.txt");
    tree.Algorithm();
    tree.Print1(outputFile);
    outputFile.close();
    return 0;
}
