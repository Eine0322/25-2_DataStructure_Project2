#include "SelectionTree.h"

SelectionTree::SelectionTree(ofstream* fout) {
    this->fout = fout;
}

SelectionTree::~SelectionTree() {}

bool SelectionTree::Insert(string key, string value) {
    history.push({key, value});
    *fout << "========ADD_ST========" << endl;
    *fout << "Success" << endl;
    *fout << "======================" << endl;
    return true;
}

bool SelectionTree::Delete() {
    *fout << "========DELETE========" << endl;
    if (history.empty()) {
        *fout << "Fail" << endl;
        *fout << "======================" << endl;
        return false;
    }
    history.pop();
    *fout << "Success" << endl;
    *fout << "======================" << endl;
    return true;
}

bool SelectionTree::Print(int dept_no, BpTree* bptree) {
    *fout << "========PRINT_ST========" << endl;

    if (history.empty() || !bptree->getRoot()) {
        *fout << "Fail" << endl;
        *fout << "========================" << endl;
        return false;
    }

    vector<EmployeeData*> result;

    // B+ Tree 전체 탐색
    BpTreeNode* node = bptree->getRoot();
    while (node->isIndexNode())
        node = node->getMostLeftChild();

    while (node) {
        for (auto& kv : *node->getDataMap()) {
            EmployeeData* emp = kv.second;
            if (emp->GetDept() == dept_no)
                result.push_back(emp);
        }
        node = node->getNext();
    }

    if (result.empty()) {
        *fout << "Fail" << endl;
        *fout << "========================" << endl;
        return false;
    }

    // ✅ 연봉 내림차순 정렬
    sort(result.begin(), result.end(), [](EmployeeData* a, EmployeeData* b) {
        return a->GetAnnualIncome() > b->GetAnnualIncome();
    });

    for (auto emp : result) {
        *fout << emp->GetName() << "/" << emp->GetDept() << "/"
              << emp->GetID() << "/" << emp->GetAnnualIncome() << endl;
    }

    *fout << "========================" << endl;
    return true;
}
