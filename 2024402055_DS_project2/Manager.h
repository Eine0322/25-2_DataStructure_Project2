#pragma once
#include <fstream>
#include <string>
#include "BpTree.h"
#include "SelectionTree.h"
using namespace std;

// Manager class controls all operations for B+ tree and Selection tree
class Manager {
private:
    ifstream fin;   // input file stream for command file
    ofstream fout;  // output file stream for log file

    BpTree* bptree;       // pointer to B+ tree instance
    SelectionTree* stree; // pointer to Selection tree instance

public:
    // constructor: initializes data structures with given B+ tree order
    Manager(int order = 3);   

    // destructor: releases resources and closes files
    ~Manager();

    // run program using commands from a text file
    void run(const char* commandFile);

private:
    // load employee data from employee.txt
    bool Load();

    // add a new employee record into B+ tree
    bool Add_BP(string name, int dept, int id, int income);

    // search employee by name or by range
    bool Search_BP(string arg1, string arg2 = "");

    // print all employees in B+ tree
    bool Print_BP();

    // add data into selection tree
    bool Add_ST(string type, string value);

    // print selection tree for a specific department
    bool Print_ST(int dept_no);

    // delete top employee (highest income) from selection tree
    bool Delete();
};
