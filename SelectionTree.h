// SelectionTree.h
#pragma once
#include "BpTree.h"
#include <stack>
#include <utility>

class SelectionTree {
private:
    ofstream* fout;
    stack<pair<string, string>> history;  // Store recent additions for DELETE

public:
    SelectionTree(ofstream* fout) {
        this->fout = fout;
    }
    ~SelectionTree() {}

    bool Insert(string key, string value);
    bool Delete();
    bool Print(int dept_no, BpTree* bptree);
};
