#include "Manager.h"

void Manager::run(const char* command) {
    fin.open(command);
    if (!fin) {
        fout << "[ERROR] command file open error!" << endl;
        return;
    }

    string cmd;
    while (!fin.eof()) {
        fin >> cmd;
        if (cmd == "LOAD") {
            Load();
        }
        else if (cmd == "ADD_BP") {
            string name;
            int dept_no, employee_id, annual_income;
            fin >> name >> dept_no >> employee_id >> annual_income;
            Add_BP(name, dept_no, employee_id, annual_income);
        }
        else if (cmd == "SEARCH_BP") {
            string start, end;
            fin >> start;
            if (fin.peek() == '\n' || fin.peek() == '\r') {
                Search_BP(start);
            } else {
                fin >> end;
                Search_BP(start, end);
            }
        }
        else if (cmd == "PRINT_BP") {
            Print_BP();
        }
        else if (cmd == "ADD_ST") {
            string key, value;
            fin >> key >> value;
            Add_ST(key, value);
        }
        else if (cmd == "PRINT_ST") {
            int dept_no;
            fin >> dept_no;
            Print_ST(dept_no);
        }
        else if (cmd == "DELETE") {
            Delete();
        }
        else if (cmd == "EXIT") {
            Exit();
            break;
        }
    }
    fin.close();
}

bool Manager::Load() {
    fout << "========LOAD========" << endl;
    
    ifstream dataFile("employee_data.txt");
    if (!dataFile.is_open()) {
        fout << "Fail" << endl;
        fout << "====================" << endl;
        return false;
    }

    string name;
    int dept_no, employee_id, annual_income;
    
    while (dataFile >> name >> dept_no >> employee_id >> annual_income) {
        EmployeeData* newData = new EmployeeData(name, dept_no, employee_id, annual_income);
        bptree->Insert(newData);
    }
    
    dataFile.close();
    fout << "Success" << endl;
    fout << "====================" << endl;
    return true;
}

bool Manager::Add_BP(string name, int dept_no, int employee_id, int annual_income) {
    fout << "========ADD_BP========" << endl;
    
    EmployeeData* newData = new EmployeeData(name, dept_no, employee_id, annual_income);
    bool result = bptree->Insert(newData);
    
    if (result) {
        fout << name << "/" << dept_no << "/" << employee_id << "/" << annual_income << endl;
    } else {
        fout << "Fail" << endl;
        delete newData;
    }
    
    fout << "======================" << endl;
    return result;
}

bool Manager::Search_BP(string name) {
    return bptree->SearchModel(name, 0);
}

bool Manager::Search_BP(string start, string end) {
    fout << "========SEARCH_BP========" << endl;
    
    vector<string> results = bptree->SearchRange(start, end);
    
    if (results.empty()) {
        fout << "Fail" << endl;
        fout << "=========================" << endl;
        return false;
    }
    
    for (const string& name : results) {
        bptree->SearchModel(name, 1);
    }
    
    fout << "=========================" << endl;
    return true;
}

bool Manager::Print_BP() {
    return bptree->Print();
}

bool Manager::Add_ST(string key, string value) {
    fout << "========ADD_ST========" << endl;
    
    bool result = stree->Insert(key, value);
    
    if (result) {
        fout << "Success" << endl;
    } else {
        fout << "Fail" << endl;
    }
    
    fout << "======================" << endl;
    return result;
}

bool Manager::Print_ST(int dept_no) {
    fout << "========PRINT_ST========" << endl;
    
    bool result = stree->Print(dept_no, bptree);
    
    if (!result) {
        fout << "Fail" << endl;
    }
    
    fout << "========================" << endl;
    return result;
}

bool Manager::Delete() {
    fout << "========DELETE========" << endl;
    
    bool result = stree->Delete();
    
    if (result) {
        fout << "Success" << endl;
    } else {
        fout << "Fail" << endl;
    }
    
    fout << "======================" << endl;
    return result;
}

void Manager::Exit() {
    // Cleanup is handled by destructors
}
