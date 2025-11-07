// BpTree.h
#pragma once
#include "BpTreeNode.h"
#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"

class BpTree {
private:
    BpTreeNode* root;
    int order;
    ofstream* fout;

public:
    BpTree(ofstream* fout, int order = 3) {
        root = NULL;
        this->order = order;
        this->fout = fout;
    }
    ~BpTree() { deleteSubTree(root); }

    bool Insert(EmployeeData* newData);
    bool excessDataNode(BpTreeNode* pDataNode);
    bool excessIndexNode(BpTreeNode* pIndexNode);
    void splitDataNode(BpTreeNode* pDataNode);
    void splitIndexNode(BpTreeNode* pIndexNode);
    BpTreeNode* searchDataNode(string name);

    bool SearchModel(string name, int flag);
    vector<string> SearchRange(string start, string end);
    void Print(BpTreeNode* node);
    bool Print();

    BpTreeNode* getRoot() { return root; }

private:
    void deleteSubTree(BpTreeNode* node);
};