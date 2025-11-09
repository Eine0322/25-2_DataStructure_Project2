#pragma once
#include "BpTreeNode.h"

// Class representing a data node in the B+ tree
class BpTreeDataNode : public BpTreeNode {
private:
    map<string, EmployeeData*> mapData;  // Map storing key-value pairs (name → EmployeeData)
    BpTreeNode* pNext;                   // Pointer to the next data node (right link)
    BpTreeNode* pPrev;                   // Pointer to the previous data node (left link)

public:
    // Constructor: initialize linked list pointers
    BpTreeDataNode() {
        pNext = NULL;
        pPrev = NULL;
    }

    // Destructor
    ~BpTreeDataNode() {}

    // Identify this node as a data node
    bool isDataNode() const override { return true; }

    // Set the next data node in the linked list
    void setNext(BpTreeNode* pN) { pNext = pN; }

    // Set the previous data node in the linked list
    void setPrev(BpTreeNode* pN) { pPrev = pN; }

    // Get the next data node
    BpTreeNode* getNext() { return pNext; }

    // Get the previous data node
    BpTreeNode* getPrev() { return pPrev; }

    // Insert a key–value pair into the data map
    void insertDataMap(string n, EmployeeData* pN) {
        mapData.insert({n, pN});
    }

    // Return a pointer to the internal data map
    map<string, EmployeeData*>* getDataMap() { return &mapData; }
};
