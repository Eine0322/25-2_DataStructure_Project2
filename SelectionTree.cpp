#include "SelectionTree.h"
#include <algorithm>

// constructor stores output stream pointer
SelectionTree::SelectionTree(ofstream* fout) {
    this->fout = fout;
}

// destructor deletes only heaps, heaps don't own EmployeeData
SelectionTree::~SelectionTree() {
    for (auto& kv : heapMap) delete kv.second;
}

// insert employees into department heap by dept or name
bool SelectionTree::Insert(string key, string value, BpTree* bptree) {
    *fout << "========ADD_ST========" << endl;
    int dept_no = 0;
    vector<EmployeeData*> targets;

    if (key == "dept_no") {
        dept_no = stoi(value);
        BpTreeNode* node = bptree->getRoot();
        if (!node) {
            *fout << "Fail\n======================" << endl;
            return false;
        }
        while (node->isIndexNode()) node = node->getMostLeftChild();
        while (node) {
            for (auto& kv : *node->getDataMap()) {
                EmployeeData* emp = kv.second;
                if (emp->GetDept() == dept_no) targets.push_back(emp);
            }
            node = node->getNext();
        }
    } else if (key == "name") {
        BpTreeNode* node = bptree->searchDataNode(value);
        if (!node) {
            *fout << "Fail\n======================" << endl;
            return false;
        }
        auto it = node->getDataMap()->find(value);
        if (it != node->getDataMap()->end()) {
            dept_no = it->second->GetDept();
            targets.push_back(it->second);
        }
    }

    if (targets.empty()) {
        *fout << "Fail\n======================" << endl;
        return false;
    }

    if (heapMap.find(dept_no) == heapMap.end()) heapMap[dept_no] = new EmployeeHeap();

    for (auto emp : targets) heapMap[dept_no]->Insert(emp);

    *fout << "Success\n======================" << endl;
    return true;
}

// delete the highest salary employee among all department heaps
bool SelectionTree::Delete() {
    *fout << "========DELETE========" << endl;

    EmployeeData* top = nullptr;
    int topDept = -1;

    for (auto& kv : heapMap) {
        EmployeeHeap* heap = kv.second;
        if (!heap || heap->IsEmpty()) continue;
        EmployeeData* candidate = heap->Top();
        if (!top || candidate->GetAnnualIncome() > top->GetAnnualIncome()) {
            top = candidate;
            topDept = kv.first;
        }
    }

    if (!top) {
        *fout << "Fail" << endl;
        *fout << "======================" << endl;
        return false;
    }

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

    if (heapMap.find(dept_no) == heapMap.end() || heapMap[dept_no]->IsEmpty()) {
        *fout << "Fail\n========================" << endl;
        return false;
    }

    // get pointers directly, no heap copy
    vector<EmployeeData*> list = heapMap[dept_no]->GetAll();

    // sort by income descending
    sort(list.begin(), list.end(), [](EmployeeData* a, EmployeeData* b) {
        return a->GetAnnualIncome() > b->GetAnnualIncome();
    });

    for (auto emp : list) {
        *fout << emp->GetName() << "/" << emp->GetDept() << "/"
              << emp->GetID() << "/" << emp->GetAnnualIncome() << endl;
    }

    *fout << "========================" << endl;
    return true;
}
