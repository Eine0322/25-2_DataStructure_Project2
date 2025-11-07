// SelectionTree.h
#pragma once
#include "SelectionTreeNode.h"

class SelectionTree {
private:
    ofstream* fout;
    SelectionTreeNode* root;
    SelectionTreeNode* run[8]; // Department nodes (100~800)

public:
    SelectionTree(ofstream* fout);
    ~SelectionTree();

    void setTree();
    bool Insert(EmployeeData* newData);
    bool Delete();
    bool printEmployeeData(int dept_no);

private:
    EmployeeData* getWinner(SelectionTreeNode* node);
};
