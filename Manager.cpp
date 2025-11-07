// Manager.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Manager.h"
#include <sstream>
#include <cstdlib>
#include <cctype>
using namespace std;

Manager::Manager(int bpOrder) {
    bp = new BpTree(&flog, bpOrder);
    st = new SelectionTree(&flog);
}

Manager::~Manager() {
    for (EmployeeData* data : empList)
        delete data;
    empList.clear();

    if (bp) delete bp;
    if (st) delete st;
}

void Manager::run(const char* command_txt) {
    fcmd.open(command_txt);
    flog.open("log.txt", ios::out);

    if (!fcmd.is_open()) {
        flog << "========ERROR========" << endl;
        flog << 800 << endl;
        flog << "=====================" << endl;
        return;
    }

    string cmd;
    while (fcmd >> cmd) {
        if (cmd == "LOAD") {
            string tmp;
            getline(fcmd, tmp);
            if (!tmp.empty() && tmp.find_first_not_of(" \t\r") != string::npos)
                printErrorCode(100);
            else if (!LOAD()) printErrorCode(100);
            else {
                flog << "========LOAD========" << endl;
                flog << "Success" << endl;
                flog << "====================" << endl;
            }
        }
        else if (cmd == "ADD_BP") {
            string name; int dept, id, sal;
            if (!(fcmd >> name >> dept >> id >> sal)) {
                printErrorCode(200);
                fcmd.clear(); fcmd.ignore(256, '\n');
                continue;
            }
            if (!ADD_BP(name, dept, id, sal))
                printErrorCode(200);
            else {
                flog << "========ADD_BP========" << endl;
                flog << name << "/" << dept << "/" << id << "/" << sal << endl;
                flog << "======================" << endl;
            }
        }
        else if (cmd == "SEARCH_BP") {
            string rest; getline(fcmd, rest);
            stringstream ss(rest);
            vector<string> arg;
            string t;
            while (ss >> t) arg.push_back(t);

            if (arg.size() == 1) {
                if (!SEARCH_BP(arg[0])) printErrorCode(300);
            }
            else if (arg.size() == 2) {
                if (!SEARCH_BP(arg[0], arg[1])) printErrorCode(300);
            }
            else printErrorCode(300);
        }
        else if (cmd == "PRINT_BP") {
            string tmp; getline(fcmd, tmp);
            if (!tmp.empty() && tmp.find_first_not_of(" \t\r") != string::npos)
                printErrorCode(400);
            else if (!PRINT_BP()) printErrorCode(400);
        }
        else if (cmd == "ADD_ST") {
            string type, val;
            if (!(fcmd >> type >> val)) {
                printErrorCode(500);
                fcmd.clear(); fcmd.ignore(256, '\n');
                continue;
            }
            if (!ADD_ST(type, val))
                printErrorCode(500);
            else {
                flog << "========ADD_ST========" << endl;
                flog << "Success" << endl;
                flog << "======================" << endl;
            }
        }
        else if (cmd == "PRINT_ST") {
            int dept;
            if (!(fcmd >> dept)) {
                printErrorCode(600);
                fcmd.clear(); fcmd.ignore(256, '\n');
                continue;
            }
            if (!PRINT_ST(dept))
                printErrorCode(600);
        }
        else if (cmd == "DELETE") {
            string tmp; getline(fcmd, tmp);
            if (!tmp.empty() && tmp.find_first_not_of(" \t\r") != string::npos)
                printErrorCode(700);
            else if (!DELETE_ST()) printErrorCode(700);
            else {
                flog << "========DELETE========" << endl;
                flog << "Success" << endl;
                flog << "======================" << endl;
            }
        }
        else if (cmd == "EXIT") {
            flog << "========EXIT========" << endl;
            flog << "Success" << endl;
            flog << "====================" << endl;
            break;
        }
        else {
            printErrorCode(800);
            string skip; getline(fcmd, skip);
        }
    }

    fcmd.close();
    flog.close();
}

bool Manager::LOAD() {
    ifstream fin("employee.txt");
    if (!fin.is_open() || bp->getRoot() != nullptr) return false;

    string name; int dept, id, sal;
    while (fin >> name >> dept >> id >> sal) {
        EmployeeData* emp = new EmployeeData();
        emp->SetName(name);
        emp->SetDept(dept);
        emp->SetID(id);
        emp->SetAnnualIncome(sal);
        empList.push_back(emp);
        bp->Insert(emp);
    }
    fin.close();
    return true;
}

bool Manager::ADD_BP(string name, int dept, int id, int sal) {
    if (dept < 100 || dept > 800 || dept % 100 != 0) return false;

    BpTreeNode* node = bp->searchDataNode(name);
    if (node) {
        auto it = node->getDataMap()->find(name);
        if (it != node->getDataMap()->end()) {
            it->second->SetDept(dept);
            it->second->SetID(id);
            it->second->SetAnnualIncome(sal);
            return true;
        }
    }

    EmployeeData* emp = new EmployeeData();
    emp->SetName(name);
    emp->SetDept(dept);
    emp->SetID(id);
    emp->SetAnnualIncome(sal);
    empList.push_back(emp);
    return bp->Insert(emp);
}

bool Manager::SEARCH_BP(string name) {
    if (bp->getRoot() == nullptr) return false;
    BpTreeNode* node = bp->searchDataNode(name);
    if (!node) return false;
    auto it = node->getDataMap()->find(name);
    if (it == node->getDataMap()->end()) return false;

    flog << "========SEARCH_BP========" << endl;
    EmployeeData* emp = it->second;
    flog << emp->GetName() << "/" << emp->GetDept() << "/" << emp->GetID() << "/" << emp->GetAnnualIncome() << endl;
    flog << "=========================" << endl;
    return true;
}

bool Manager::SEARCH_BP(string start, string end) {
    if (bp->getRoot() == nullptr) return false;

    vector<string> names = bp->SearchRange(start, end);
    if (names.empty()) return false;

    flog << "========SEARCH_BP========" << endl;
    for (auto& name : names) {
        BpTreeNode* node = bp->searchDataNode(name);
        if (!node) continue;
        auto it = node->getDataMap()->find(name);
        if (it != node->getDataMap()->end()) {
            EmployeeData* emp = it->second;
            flog << emp->GetName() << "/" << emp->GetDept() << "/" << emp->GetID() << "/" << emp->GetAnnualIncome() << endl;
        }
    }
    flog << "=========================" << endl;
    return true;
}

bool Manager::PRINT_BP() {
    if (bp->getRoot() == nullptr) return false;
    flog << "========PRINT_BP========" << endl;
    bp->Print(bp->getRoot());
    flog << "========================" << endl;
    return true;
}

bool Manager::ADD_ST(string type, string value) {
    if (bp->getRoot() == nullptr) return false;
    if (type == "dept_no") {
        int dept = stoi(value);
        if (dept < 100 || dept > 800 || dept % 100 != 0) return false;

        BpTreeNode* node = bp->getRoot();
        while (node->isIndexNode()) node = node->getMostLeftChild();

        bool found = false;
        while (node) {
            for (auto it : *node->getDataMap()) {
                EmployeeData* emp = it.second;
                if (emp->GetDept() == dept) {
                    st->Insert(emp);
                    found = true;
                }
            }
            node = node->getNext();
        }
        return found;
    }
    else if (type == "name") {
        string name = value;
        BpTreeNode* node = bp->searchDataNode(name);
        if (!node) return false;
        auto it = node->getDataMap()->find(name);
        if (it == node->getDataMap()->end()) return false;
        return st->Insert(it->second);
    }
    else return false;
}

bool Manager::PRINT_ST(int dept) {
    return st->printEmployeeData(dept);
}

bool Manager::DELETE_ST() {
    return st->Delete();
}

void Manager::printErrorCode(int n) {
    flog << "========ERROR========" << endl;
    flog << n << endl;
    flog << "=====================" << endl;
}
