// SelectionTreeNode.h
#pragma once
#include "EmployeeHeap.h"

class SelectionTreeNode {
private:
    EmployeeData* employeeData;
    SelectionTreeNode* leftChild;
    SelectionTreeNode* rightChild;
    SelectionTreeNode* parent;
    EmployeeHeap* heap; // Each leaf node manages a heap (per department)

public:
    SelectionTreeNode() {
        employeeData = nullptr;
        leftChild = rightChild = parent = nullptr;
        heap = nullptr;
    }

    ~SelectionTreeNode() {
        if (heap) delete heap;
    }

    void HeapInit() {
        if (heap == nullptr)
            heap = new EmployeeHeap();
    }

    // Setters
    void setEmployeeData(EmployeeData* data) { employeeData = data; }
    void setLeftChild(SelectionTreeNode* node) { leftChild = node; }
    void setRightChild(SelectionTreeNode* node) { rightChild = node; }
    void setParent(SelectionTreeNode* node) { parent = node; }

    // Getters
    EmployeeData* getEmployeeData() { return employeeData; }
    SelectionTreeNode* getLeftChild() { return leftChild; }
    SelectionTreeNode* getRightChild() { return rightChild; }
    SelectionTreeNode* getParent() { return parent; }
    EmployeeHeap* getHeap() { return heap; }
};
