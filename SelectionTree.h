#pragma once
#include <fstream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include "BpTree.h"
using namespace std;

class SelectionTree {
private:
    ofstream* fout;
    stack<pair<string, string>> history;

public:
    SelectionTree(ofstream* fout);
    ~SelectionTree();

    bool Insert(string key, string value);
    bool Delete();
    bool Print(int dept_no, BpTree* bptree);
};
