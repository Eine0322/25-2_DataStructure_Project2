#pragma once
#include <fstream>
#include <string>
#include "BpTree.h"
#include "SelectionTree.h"
using namespace std;

class Manager {
private:
    ifstream fin;
    ofstream fout;

    BpTree* bptree;
    SelectionTree* stree;

public:
    Manager();
    ~Manager();

    void run(const char* commandFile);

private:
    bool Load();
    bool Add_BP(string name, int dept, int id, int income);
    bool Search_BP(string arg1, string arg2 = "");
    bool Print_BP();
    bool Add_ST(string type, string value);
    bool Print_ST(int dept_no);
    bool Delete();
};
