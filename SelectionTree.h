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
    ofstream* fout; // output file stream for writing results
    unordered_map<int, EmployeeHeap*> heapMap; // map storing each department's heap (key: dept_no)

public:
    // constructor: initialize SelectionTree with output file stream
    SelectionTree(ofstream* fout);

    // destructor: release allocated memory and clear resources
    ~SelectionTree();

    // insert employee data by department number or name using B+ tree lookup
    bool Insert(string key, string value, BpTree* bptree);

    // delete the employee with the highest salary among all departments
    bool Delete();

    // print all employees in the given department, sorted by salary
    bool Print(int dept_no);
};
