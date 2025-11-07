#pragma once
#include "SelectionTreeNode.h"

class SelectionTree {
private:
    ofstream* fout;
    SelectionTreeNode* root;
    SelectionTreeNode* run[8]; // Department leaf nodes (100 ~ 800)

public:
    SelectionTree(ofstream* fout);
    ~SelectionTree();

    void setTree();                          // Build the selection tree
    bool Insert(EmployeeData* newData);      // Insert employee into tree
    bool Delete();                           // Delete top (highest salary) employee
    bool printEmployeeData(int dept_no);     // Print employees in a department (salary descending)

private:
    void rebuildWinners(SelectionTreeNode* node); // <== ADD THIS FUNCTION
};
