// BpTree.h
#pragma once
#include "BpTreeNode.h"
#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"

// Class representing a B+ Tree
class BpTree {
private:
    BpTreeNode* root;     // Root node of the tree
    int order;            // Tree order (degree)
    ofstream* fout;       // Output file stream for logs

public:
    BpTree(ofstream* fout, int order = 3) {
        root = NULL;
        this->order = order;
        this->fout = fout;
    }

    ~BpTree() {
        deleteSubTree(root);
    }

    // Core functions
    bool Insert(EmployeeData* newData);
    bool excessDataNode(BpTreeNode* pDataNode);
    bool excessIndexNode(BpTreeNode* pIndexNode);
    void splitDataNode(BpTreeNode* pDataNode);
    void splitIndexNode(BpTreeNode* pIndexNode);
    BpTreeNode* searchDataNode(string name);

    // Utility functions
    bool SearchModel(string name, int flag);
    vector<string> SearchRange(string start, string end);
    void Print(BpTreeNode* node);
    bool Print();

    BpTreeNode* getRoot() { return root; }

private:
    void deleteSubTree(BpTreeNode* node);
};
