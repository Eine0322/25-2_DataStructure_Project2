// Manager.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Manager.h"
#include <sstream>
#include <cstdlib>
#include <cctype>
using namespace std;

static inline void splitTokens(const string &line, vector<string> &out) {
    out.clear();
    string token;
    stringstream ss(line);
    while (ss >> token) out.push_back(token);
}

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

    string line;
    while (std::getline(fcmd, line)) {
        // trim leading/trailing spaces (simple)
        auto l = line.find_first_not_of(" \t\r");
        if (l == string::npos) continue; // blank line
        line = line.substr(l);
        // tokenize by whitespace (handles tabs too)
        vector<string> tokens;
        splitTokens(line, tokens);
        if (tokens.empty()) continue;

        string cmd = tokens[0];

        if (cmd == "LOAD") {
            // LOAD should have no extra args
            if (tokens.size() != 1) {
                printErrorCode(100);
                continue;
            }
            if (!LOAD()) printErrorCode(100);
            else {
                flog << "========LOAD========" << endl;
                flog << "Success" << endl;
                flog << "====================" << endl;
            }
        }
        else if (cmd == "ADD_BP") {
            if (tokens.size() != 5) { printErrorCode(200); continue; }
            string name = tokens[1];
            int dept = stoi(tokens[2]);
            int id = stoi(tokens[3]);
            int sal = stoi(tokens[4]);
            if (!ADD_BP(name, dept, id, sal)) printErrorCode(200);
            else {
                flog << "========ADD_BP========" << endl;
                flog << name << "/" << dept << "/" << id << "/" << sal << endl;
                flog << "======================" << endl;
            }
        }
        else if (cmd == "SEARCH_BP") {
            if (tokens.size() == 2) {
                if (!SEARCH_BP(tokens[1])) printErrorCode(300);
            } else if (tokens.size() == 3) {
                if (!SEARCH_BP(tokens[1], tokens[2])) printErrorCode(300);
            } else {
                printErrorCode(300);
            }
        }
        else if (cmd == "PRINT_BP") {
            if (tokens.size() != 1) { printErrorCode(400); continue; }
            if (!PRINT_BP()) printErrorCode(400);
        }
        else if (cmd == "ADD_ST") {
            if (tokens.size() != 3) { printErrorCode(500); continue; }
            string type = tokens[1];
            string val = tokens[2];
            if (!ADD_ST(type, val)) printErrorCode(500);
            else {
                flog << "========ADD_ST========" << endl;
                flog << "Success" << endl;
                flog << "======================" << endl;
            }
        }
        else if (cmd == "PRINT_ST") {
            if (tokens.size() != 2) { printErrorCode(600); continue; }
            int dept = stoi(tokens[1]);
            if (!PRINT_ST(dept)) printErrorCode(600);
        }
        else if (cmd == "DELETE") {
            if (tokens.size() != 1) { printErrorCode(700); continue; }
            if (!DELETE_ST()) printErrorCode(700);
            else {
                flog << "========DELETE========" << endl;
                flog << "Success" << endl;
                flog << "======================" << endl;
            }
        }
        else if (cmd == "EXIT") {
            if (tokens.size() != 1) { printErrorCode(800); continue; }
            flog << "========EXIT========" << endl;
            flog << "Success" << endl;
            flog << "====================" << endl;
            break;
        }
        else {
            printErrorCode(800);
            continue;
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
