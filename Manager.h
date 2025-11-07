// Manager.h
#pragma once
#include "BpTree.h"
#include "SelectionTree.h"

class Manager {
private:
    BpTree* bp;             // B+ Tree for employee info
    SelectionTree* st;      // Selection Tree for department heaps
    vector<EmployeeData*> empList; // Loaded employees
    ofstream flog;
    ifstream fcmd;

public:
    Manager(int bpOrder);
    ~Manager();

    void run(const char* command_txt);

private:
    bool LOAD();
    bool ADD_BP(string name, int dept, int id, int sal);
    bool SEARCH_BP(string name);
    bool SEARCH_BP(string start, string end);
    bool PRINT_BP();

    bool ADD_ST(string type, string value);
    bool PRINT_ST(int dept);
    bool DELETE_ST();

    void printErrorCode(int n);
};
