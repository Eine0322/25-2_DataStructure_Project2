#pragma once
#include "BpTree.h"
#include "SelectionTree.h"

class Manager {
private:
    ifstream fin;
    ofstream fout;
    BpTree* bptree;
    SelectionTree* stree;

public:
    Manager(int bpOrder) {
        bptree = new BpTree(&fout, bpOrder);
        stree = new SelectionTree(&fout);
        fout.open("log.txt");
    }

    ~Manager() {
        if (bptree) delete bptree;
        if (stree) delete stree;
        if (fout.is_open()) fout.close();
        if (fin.is_open()) fin.close();
    }

    void run(const char* command);

    bool Load();
    bool Add_BP(string name, int dept_no, int employee_id, int annual_income);
    bool Search_BP(string name);
    bool Search_BP(string start, string end);
    bool Print_BP();
    bool Add_ST(string key, string value);
    bool Print_ST(int dept_no);
    bool Delete();
    void Exit();
};
