// SelectionTree.cpp (수정됨 - PRINT_ST 내림차순 정렬)
#include "SelectionTree.h"
#include <algorithm>

// Initialize the Selection Tree
void SelectionTree::setTree() {
    if (root != nullptr) return;

    SelectionTreeNode* leaf[8];
    for (int i = 0; i < 8; i++) {
        leaf[i] = new SelectionTreeNode();
        leaf[i]->HeapInit();
        run[i] = leaf[i];
    }

    // Level 1
    SelectionTreeNode* level1[4];
    for (int i = 0; i < 4; i++) {
        level1[i] = new SelectionTreeNode();
        level1[i]->setLeftChild(leaf[i * 2]);
        level1[i]->setRightChild(leaf[i * 2 + 1]);
        leaf[i * 2]->setParent(level1[i]);
        leaf[i * 2 + 1]->setParent(level1[i]);
    }

    // Level 2
    SelectionTreeNode* level2[2];
    for (int i = 0; i < 2; i++) {
        level2[i] = new SelectionTreeNode();
        level2[i]->setLeftChild(level1[i * 2]);
        level2[i]->setRightChild(level1[i * 2 + 1]);
        level1[i * 2]->setParent(level2[i]);
        level1[i * 2 + 1]->setParent(level2[i]);
    }

    // Root
    root = new SelectionTreeNode();
    root->setLeftChild(level2[0]);
    root->setRightChild(level2[1]);
    level2[0]->setParent(root);
    level2[1]->setParent(root);
}

// Insert employee into tree
bool SelectionTree::Insert(EmployeeData* newData) {
    if (root == nullptr) setTree();

    int dept = newData->GetDept();
    int idx = (dept / 100) - 1;
    if (idx < 0 || idx >= 8) return false;

    run[idx]->getHeap()->Insert(newData);
    run[idx]->setEmployeeData(run[idx]->getHeap()->Top());

    // Propagate winner upward
    SelectionTreeNode* cur = run[idx];
    while (cur->getParent() != nullptr) {
        SelectionTreeNode* parent = cur->getParent();
        SelectionTreeNode* left = parent->getLeftChild();
        SelectionTreeNode* right = parent->getRightChild();

        EmployeeData* L = left->getEmployeeData();
        EmployeeData* R = right->getEmployeeData();

        if (L == nullptr && R == nullptr) parent->setEmployeeData(nullptr);
        else if (R == nullptr) parent->setEmployeeData(L);
        else if (L == nullptr) parent->setEmployeeData(R);
        else {
            if (L->GetAnnualIncome() >= R->GetAnnualIncome())
                parent->setEmployeeData(L);
            else
                parent->setEmployeeData(R);
        }
        cur = parent;
    }
    return true;
}

// Delete highest salary employee (root)
bool SelectionTree::Delete() {
    if (root == nullptr || root->getEmployeeData() == nullptr)
        return false;

    EmployeeData* top = root->getEmployeeData();
    int dept = top->GetDept();
    int idx = (dept / 100) - 1;
    if (idx < 0 || idx >= 8) return false;

    EmployeeHeap* heap = run[idx]->getHeap();
    heap->Delete();

    if (heap->IsEmpty()) run[idx]->setEmployeeData(nullptr);
    else run[idx]->setEmployeeData(heap->Top());

    // Propagate updates upward
    SelectionTreeNode* cur = run[idx];
    while (cur->getParent() != nullptr) {
        SelectionTreeNode* parent = cur->getParent();
        SelectionTreeNode* left = parent->getLeftChild();
        SelectionTreeNode* right = parent->getRightChild();

        EmployeeData* L = left->getEmployeeData();
        EmployeeData* R = right->getEmployeeData();

        if (L == nullptr && R == nullptr) parent->setEmployeeData(nullptr);
        else if (R == nullptr) parent->setEmployeeData(L);
        else if (L == nullptr) parent->setEmployeeData(R);
        else {
            if (L->GetAnnualIncome() >= R->GetAnnualIncome())
                parent->setEmployeeData(L);
            else
                parent->setEmployeeData(R);
        }
        cur = parent;
    }
    return true;
}

// Print all employees in a department (by salary DESCENDING)
bool SelectionTree::printEmployeeData(int dept_no) {
    if (dept_no % 100 != 0 || dept_no < 100 || dept_no > 800)
        return false;

    int idx = (dept_no / 100) - 1;
    if (run[idx] == nullptr) return false;

    EmployeeHeap* heap = run[idx]->getHeap();
    if (heap == nullptr || heap->IsEmpty())
        return false;

    // Copy all heap data into vector (no modification to original)
    vector<EmployeeData*> temp;
    EmployeeHeap copy = *heap;
    while (!copy.IsEmpty()) {
        temp.push_back(copy.Top());
        copy.Delete();
    }

    // Sort by AnnualIncome descending
    sort(temp.begin(), temp.end(),
         [](EmployeeData* a, EmployeeData* b) {
             return a->GetAnnualIncome() > b->GetAnnualIncome();
         });

    *fout << "========PRINT_ST========" << endl;
    for (auto emp : temp)
        *fout << emp->GetName() << "/" << emp->GetDept() << "/" 
              << emp->GetID() << "/" << emp->GetAnnualIncome() << endl;
    *fout << "========================" << endl;

    return true;
}

// Constructor / Destructor
SelectionTree::SelectionTree(ofstream* fout) {
    this->fout = fout;
    this->root = nullptr;
    for (int i = 0; i < 8; i++) run[i] = nullptr;
}

SelectionTree::~SelectionTree() {
    for (int i = 0; i < 8; i++) {
        if (run[i] != nullptr)
            delete run[i];
    }
    if (root != nullptr) delete root;
}
