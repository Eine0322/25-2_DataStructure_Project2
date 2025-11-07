#include "SelectionTree.h"
#include <algorithm>
using namespace std;

// Rebuild winners recursively (used to refresh the tree after insertion/deletion)
void SelectionTree::rebuildWinners(SelectionTreeNode* node) {
    if (!node) return;
    if (!node->getLeftChild() && !node->getRightChild()) return;

    rebuildWinners(node->getLeftChild());
    rebuildWinners(node->getRightChild());

    SelectionTreeNode* left = node->getLeftChild();
    SelectionTreeNode* right = node->getRightChild();
    EmployeeData* L = (left ? left->getEmployeeData() : nullptr);
    EmployeeData* R = (right ? right->getEmployeeData() : nullptr);

    if (!L && !R) node->setEmployeeData(nullptr);
    else if (!R) node->setEmployeeData(L);
    else if (!L) node->setEmployeeData(R);
    else node->setEmployeeData(
        (L->GetAnnualIncome() >= R->GetAnnualIncome()) ? L : R
    );
}

// Initialize the Selection Tree structure
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

// Insert an employee into the corresponding department heap
bool SelectionTree::Insert(EmployeeData* newData) {
    if (root == nullptr) setTree();
    if (!newData) return false;

    int dept = newData->GetDept();
    int idx = (dept / 100) - 1;
    if (idx < 0 || idx >= 8 || run[idx] == nullptr) return false;

    run[idx]->getHeap()->Insert(newData);
    run[idx]->setEmployeeData(run[idx]->getHeap()->Top());

    // Propagate winner upward to parent nodes
    SelectionTreeNode* cur = run[idx];
    while (cur && cur->getParent() != nullptr) {
        SelectionTreeNode* parent = cur->getParent();
        SelectionTreeNode* left = parent->getLeftChild();
        SelectionTreeNode* right = parent->getRightChild();

        EmployeeData* L = (left ? left->getEmployeeData() : nullptr);
        EmployeeData* R = (right ? right->getEmployeeData() : nullptr);

        if (!L && !R) parent->setEmployeeData(nullptr);
        else if (!R) parent->setEmployeeData(L);
        else if (!L) parent->setEmployeeData(R);
        else parent->setEmployeeData(
            (L->GetAnnualIncome() >= R->GetAnnualIncome()) ? L : R
        );

        cur = parent;
    }

    // Rebuild the entire tree to update root correctly
    rebuildWinners(root);

    // Ensure root winner is valid (in case root->employeeData is null)
    if (root) {
        SelectionTreeNode* L = root->getLeftChild();
        SelectionTreeNode* R = root->getRightChild();
        EmployeeData* leftData = (L ? L->getEmployeeData() : nullptr);
        EmployeeData* rightData = (R ? R->getEmployeeData() : nullptr);
        if (!leftData && !rightData) root->setEmployeeData(nullptr);
        else if (!rightData) root->setEmployeeData(leftData);
        else if (!leftData) root->setEmployeeData(rightData);
        else root->setEmployeeData(
            (leftData->GetAnnualIncome() >= rightData->GetAnnualIncome()) ? leftData : rightData
        );
    }

    return true;
}

// Delete the employee with the highest salary (root winner)
bool SelectionTree::Delete() {
    if (root == nullptr) return false;
    EmployeeData* top = root->getEmployeeData();
    if (top == nullptr) return false;

    int dept = top->GetDept();
    int idx = (dept / 100) - 1;
    if (idx < 0 || idx >= 8) return false;
    if (run[idx] == nullptr) return false;

    EmployeeHeap* heap = run[idx]->getHeap();
    if (heap == nullptr || heap->IsEmpty()) return false;

    // Delete top employee from heap
    heap->Delete();

    // Update leaf's employeeData
    if (heap->IsEmpty())
        run[idx]->setEmployeeData(nullptr);
    else
        run[idx]->setEmployeeData(heap->Top());

    // Propagate winner updates up to the root
    SelectionTreeNode* cur = run[idx];
    while (cur && cur->getParent() != nullptr) {
        SelectionTreeNode* parent = cur->getParent();
        SelectionTreeNode* left = parent->getLeftChild();
        SelectionTreeNode* right = parent->getRightChild();

        EmployeeData* L = (left ? left->getEmployeeData() : nullptr);
        EmployeeData* R = (right ? right->getEmployeeData() : nullptr);

        if (!L && !R) parent->setEmployeeData(nullptr);
        else if (!R) parent->setEmployeeData(L);
        else if (!L) parent->setEmployeeData(R);
        else parent->setEmployeeData(
            (L->GetAnnualIncome() >= R->GetAnnualIncome()) ? L : R
        );

        cur = parent;
    }

    // Rebuild the tree to ensure root is up to date
    rebuildWinners(root);

    // Ensure root winner is valid
    if (root) {
        SelectionTreeNode* L = root->getLeftChild();
        SelectionTreeNode* R = root->getRightChild();
        EmployeeData* leftData = (L ? L->getEmployeeData() : nullptr);
        EmployeeData* rightData = (R ? R->getEmployeeData() : nullptr);
        if (!leftData && !rightData) root->setEmployeeData(nullptr);
        else if (!rightData) root->setEmployeeData(leftData);
        else if (!leftData) root->setEmployeeData(rightData);
        else root->setEmployeeData(
            (leftData->GetAnnualIncome() >= rightData->GetAnnualIncome()) ? leftData : rightData
        );
    }

    return true;
}

// Print all employees in a given department (sorted by salary descending)
bool SelectionTree::printEmployeeData(int dept_no) {
    if (dept_no % 100 != 0 || dept_no < 100 || dept_no > 800)
        return false;

    int idx = (dept_no / 100) - 1;
    if (idx < 0 || idx >= 8) return false;
    if (run[idx] == nullptr) return false;

    EmployeeHeap* heap = run[idx]->getHeap();
    if (heap == nullptr || heap->IsEmpty())
        return false;

    // Copy heap contents (so original is preserved)
    vector<EmployeeData*> temp;
    EmployeeHeap copy = *heap;
    while (!copy.IsEmpty()) {
        temp.push_back(copy.Top());
        copy.Delete();
    }

    // Sort by salary descending
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

// Constructor
SelectionTree::SelectionTree(ofstream* fout) {
    this->fout = fout;
    this->root = nullptr;
    for (int i = 0; i < 8; i++) run[i] = nullptr;
}

// Destructor
SelectionTree::~SelectionTree() {
    for (int i = 0; i < 8; i++) {
        if (run[i] != nullptr)
            delete run[i];
    }
    if (root != nullptr) delete root;
}
