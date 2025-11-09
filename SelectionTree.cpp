#include "SelectionTree.h"
#include <algorithm>

// constructor stores output stream pointer
SelectionTree::SelectionTree(ofstream* fout) {
    this->fout = fout; // store output stream for logging
}

// destructor deletes only heaps, heaps don't own EmployeeData
SelectionTree::~SelectionTree() {
    for (auto& kv : heapMap) delete kv.second; // free all EmployeeHeap objects
}

// insert employees into department heap by dept or name
bool SelectionTree::Insert(string key, string value, BpTree* bptree) {
    *fout << "========ADD_ST========" << endl;
    int dept_no = 0; // department number
    vector<EmployeeData*> targets; // list of employees to insert

    if (key == "dept_no") { // insert all employees in a given department
        dept_no = stoi(value);
        BpTreeNode* node = bptree->getRoot();
        if (!node) { // fail if B+ tree is empty
            *fout << "Fail\n======================" << endl;
            return false;
        }
        // move to the leftmost data node
        while (node->isIndexNode()) node = node->getMostLeftChild();
        // traverse all data nodes to collect employees of the department
        while (node) {
            for (auto& kv : *node->getDataMap()) {
                EmployeeData* emp = kv.second;
                if (emp->GetDept() == dept_no) targets.push_back(emp);
            }
            node = node->getNext();
        }
    } else if (key == "name") { // insert a single employee by name
        BpTreeNode* node = bptree->searchDataNode(value);
        if (!node) {
            *fout << "Fail\n======================" << endl;
            return false;
        }
        auto it = node->getDataMap()->find(value);
        if (it != node->getDataMap()->end()) { // found matching employee
            dept_no = it->second->GetDept();
            targets.push_back(it->second);
        }
    }

    if (targets.empty()) { // fail if no matching data
        *fout << "Fail\n======================" << endl;
        return false;
    }

    // create new heap for department if not exists
    if (heapMap.find(dept_no) == heapMap.end()) heapMap[dept_no] = new EmployeeHeap();

    // insert all selected employees into the department heap
    for (auto emp : targets) heapMap[dept_no]->Insert(emp);

    *fout << "Success\n======================" << endl;
    return true;
}

// delete the highest salary employee among all department heaps
bool SelectionTree::Delete() {
    *fout << "========DELETE========" << endl;

    EmployeeData* top = nullptr; // pointer to top employee
    int topDept = -1; // department with highest salary employee

    // find the top employee across all heaps
    for (auto& kv : heapMap) {
        EmployeeHeap* heap = kv.second;
        if (!heap || heap->IsEmpty()) continue; // skip empty heaps
        EmployeeData* candidate = heap->Top();
        if (!top || candidate->GetAnnualIncome() > top->GetAnnualIncome()) {
            top = candidate;
            topDept = kv.first;
        }
    }

    // fail if no employee exists
    if (!top) {
        *fout << "Fail" << endl;
        *fout << "======================" << endl;
        return false;
    }

    // remove the top employee from corresponding heap
    EmployeeHeap* targetHeap = heapMap[topDept];
    if (targetHeap && !targetHeap->IsEmpty()) {
        targetHeap->Delete();
        *fout << "Success" << endl;
    } else {
        *fout << "Fail" << endl;
    }

    *fout << "======================" << endl;
    return true;
}

// print employees of a department sorted by income descending
bool SelectionTree::Print(int dept_no) {
    *fout << "========PRINT_ST========" << endl;

    // check if heap exists and is not empty
    if (heapMap.find(dept_no) == heapMap.end() || heapMap[dept_no]->IsEmpty()) {
        *fout << "Fail\n========================" << endl;
        return false;
    }

    // get all employees in department (unsorted)
    vector<EmployeeData*> list = heapMap[dept_no]->GetAll();

    // sort by income in descending order
    sort(list.begin(), list.end(), [](EmployeeData* a, EmployeeData* b) {
        return a->GetAnnualIncome() > b->GetAnnualIncome();
    });

    // print each employee’s information
    for (auto emp : list) {
        *fout << emp->GetName() << "/" << emp->GetDept() << "/"
              << emp->GetID() << "/" << emp->GetAnnualIncome() << endl;
    }

    *fout << "========================" << endl;
    return true;
}
