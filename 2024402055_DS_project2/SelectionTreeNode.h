#pragma once
#include "EmployeeHeap.h"

class SelectionTreeNode {
private:
    EmployeeData* employeeData;        // pointer to employee information
    SelectionTreeNode* leftChild;      // pointer to left child node
    SelectionTreeNode* rightChild;     // pointer to right child node
    SelectionTreeNode* parent;         // pointer to parent node
    EmployeeHeap* heap;                // heap for managing employees in a department (only used in leaf nodes)

public:
    // constructor: initialize all pointers to nullptr
    SelectionTreeNode() {
        employeeData = nullptr;
        leftChild = rightChild = parent = nullptr;
        heap = nullptr;
    }

    // destructor: delete heap if it exists
    ~SelectionTreeNode() {
        if (heap) delete heap;
    }

    // initialize heap if not already created
    void HeapInit() {
        if (heap == nullptr)
            heap = new EmployeeHeap();
    }

    // setters for each member variable
    void setEmployeeData(EmployeeData* data) { employeeData = data; }
    void setLeftChild(SelectionTreeNode* node) { leftChild = node; }
    void setRightChild(SelectionTreeNode* node) { rightChild = node; }
    void setParent(SelectionTreeNode* node) { parent = node; }

    // getters for each member variable
    EmployeeData* getEmployeeData() { return employeeData; }
    SelectionTreeNode* getLeftChild() { return leftChild; }
    SelectionTreeNode* getRightChild() { return rightChild; }
    SelectionTreeNode* getParent() { return parent; }
    EmployeeHeap* getHeap() { return heap; }
};
