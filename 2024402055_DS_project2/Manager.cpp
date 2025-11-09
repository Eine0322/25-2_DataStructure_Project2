#include "Manager.h"
#include <sstream>

Manager::Manager(int order) { // takes B+ tree order
    fout.open("log.txt"); // open log file for output
    bptree = new BpTree(&fout, order); // create B+ tree with given order
    stree = new SelectionTree(&fout); // create selection tree
}

Manager::~Manager() {
    if (bptree) delete bptree; // release B+ tree memory
    if (stree) delete stree;   // release selection tree memory
    fout.close(); // close log file
}

// run command file
void Manager::run(const char* commandFile) {
    fin.open(commandFile); // open input command file
    if (!fin.is_open()) {
        fout << "Fail to open command file" << endl;
        return; // stop if command file not found
    }

    string line;
    while (getline(fin, line)) { // read file line by line
        if (line.empty()) continue; // skip empty lines
        stringstream ss(line);
        string cmd;
        ss >> cmd; // extract command

        // handle commands one by one
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
            // exit command prints success message and terminates
            fout << "========EXIT========" << endl;
            fout << "Success" << endl;
            fout << "====================" << endl;
            break;
        }
    }
    fin.close(); // close command file
}

// load employee data from file
bool Manager::Load() {
    fout << "========LOAD========" << endl;
    ifstream data("employee.txt"); // open employee data file
    if (!data.is_open()) { // fail if file not found
        fout << "Fail" << endl;
        fout << "====================" << endl;
        return false;
    }

    string name;
    int dept, id, income;
    // read each employee record and insert into B+ tree
    while (data >> name >> dept >> id >> income) {
        EmployeeData* emp = new EmployeeData(name, dept, id, income);
        bptree->Insert(emp);
    }

    fout << "Success" << endl;
    fout << "====================" << endl;
    data.close(); // close file after loading
    return true;
}

// add new employee into B+ tree
bool Manager::Add_BP(string name, int dept, int id, int income) {
    fout << "========ADD_BP========" << endl;
    EmployeeData* emp = new EmployeeData(name, dept, id, income); // create new employee
    bptree->Insert(emp); // insert into B+ tree
    fout << name << "/" << dept << "/" << id << "/" << income << endl; // log inserted data
    fout << "======================" << endl;
    return true;
}

// search by name or range
bool Manager::Search_BP(string arg1, string arg2) {
    fout << "========SEARCH_BP========" << endl;
    if (arg2.empty()) { // single search by name
        bptree->SearchModel(arg1, 1);
    } else { // range search between arg1 and arg2
        auto range = bptree->SearchRange(arg1, arg2);
        for (auto& name : range)
            bptree->SearchModel(name, 1);
    }
    fout << "=========================" << endl;
    return true;
}

// print all employees in B+ tree
bool Manager::Print_BP() {
    return bptree->Print(); // call print function of B+ tree
}

// add data into selection tree
bool Manager::Add_ST(string type, string value) {
    return stree->Insert(type, value, bptree); // insert node using B+ tree reference
}

// print selection tree for department
bool Manager::Print_ST(int dept_no) {
    return stree->Print(dept_no); // print all employees in the given department
}

// delete top employee (highest salary)
bool Manager::Delete() {
    return stree->Delete(); // delete node with maximum salary from selection tree
}
