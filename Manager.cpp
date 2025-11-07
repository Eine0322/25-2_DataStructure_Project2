#include "Manager.h"
#include <sstream>

Manager::Manager(int order) { // takes B+ tree order
    fout.open("log.txt");
    bptree = new BpTree(&fout, order);
    stree = new SelectionTree(&fout);
}

Manager::~Manager() {
    if (bptree) delete bptree;
    if (stree) delete stree;
    fout.close();
}

// run command file
void Manager::run(const char* commandFile) {
    fin.open(commandFile);
    if (!fin.is_open()) {
        fout << "Fail to open command file" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "LOAD") Load();
        else if (cmd == "ADD_BP") {
            string name; int dept, id, income;
            ss >> name >> dept >> id >> income;
            Add_BP(name, dept, id, income);
        }
        else if (cmd == "SEARCH_BP") {
            string a, b; ss >> a >> b;
            Search_BP(a, b);
        }
        else if (cmd == "PRINT_BP") Print_BP();
        else if (cmd == "ADD_ST") {
            string type, val; ss >> type >> val;
            Add_ST(type, val);
        }
        else if (cmd == "PRINT_ST") {
            int dept; ss >> dept;
            Print_ST(dept);
        }
        else if (cmd == "DELETE") Delete();
        else if (cmd == "EXIT") {
            fout << "========EXIT========" << endl;
            fout << "Success" << endl;
            fout << "====================" << endl;
            break;
        }
    }
    fin.close();
}

// load employee data from file
bool Manager::Load() {
    fout << "========LOAD========" << endl;
    ifstream data("employee.txt");
    if (!data.is_open()) {
        fout << "Fail" << endl;
        fout << "====================" << endl;
        return false;
    }

    string name;
    int dept, id, income;
    while (data >> name >> dept >> id >> income) {
        EmployeeData* emp = new EmployeeData(name, dept, id, income);
        bptree->Insert(emp);
    }

    fout << "Success" << endl;
    fout << "====================" << endl;
    data.close();
    return true;
}

// add new employee into B+ tree
bool Manager::Add_BP(string name, int dept, int id, int income) {
    fout << "========ADD_BP========" << endl;
    EmployeeData* emp = new EmployeeData(name, dept, id, income);
    bptree->Insert(emp);
    fout << name << "/" << dept << "/" << id << "/" << income << endl;
    fout << "======================" << endl;
    return true;
}

// search by name or range
bool Manager::Search_BP(string arg1, string arg2) {
    fout << "========SEARCH_BP========" << endl;
    if (arg2.empty()) {
        bptree->SearchModel(arg1, 1);
    } else {
        auto range = bptree->SearchRange(arg1, arg2);
        for (auto& name : range)
            bptree->SearchModel(name, 1);
    }
    fout << "=========================" << endl;
    return true;
}

// print all employees in B+ tree
bool Manager::Print_BP() {
    return bptree->Print();
}

// add data into selection tree
bool Manager::Add_ST(string type, string value) {
    return stree->Insert(type, value, bptree);
}

// print selection tree for department
bool Manager::Print_ST(int dept_no) {
    return stree->Print(dept_no);
}

// delete top employee (highest salary)
bool Manager::Delete() {
    return stree->Delete();
}
