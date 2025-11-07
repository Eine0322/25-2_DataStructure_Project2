#pragma once
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "BpTree.h"
#include "EmployeeHeap.h"
using namespace std;

class SelectionTree {
private:
    ofstream* fout;
    unordered_map<int, EmployeeHeap*> heapMap; // map from dept_no to heap pointer

public:
    SelectionTree(ofstream* fout);
    ~SelectionTree();

    // insert by "dept_no" or "name", bptree is used for lookups
    bool Insert(string key, string value, BpTree* bptree);
    bool Delete();                // remove global highest salary employee
    bool Print(int dept_no);      // print employees in dept sorted by salary
};
