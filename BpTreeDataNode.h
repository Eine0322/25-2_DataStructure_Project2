// BpTreeDataNode.h
#pragma once
#include "BpTreeNode.h"

// Class representing a Data Node in the B+ Tree
class BpTreeDataNode : public BpTreeNode {
private:
    map<string, EmployeeData*> mapData;
    BpTreeNode* pNext;
    BpTreeNode* pPrev;

public:
    BpTreeDataNode() {
        pNext = NULL;
        pPrev = NULL;
    }
    ~BpTreeDataNode() {}

    bool isDataNode() const override { return true; }

    void setNext(BpTreeNode* pN) { pNext = pN; }
    void setPrev(BpTreeNode* pN) { pPrev = pN; }

    BpTreeNode* getNext() { return pNext; }
    BpTreeNode* getPrev() { return pPrev; }

    void insertDataMap(string n, EmployeeData* pN) {
        mapData.insert(map<string, EmployeeData*>::value_type(n, pN));
    }

    void deleteMap(string n) { mapData.erase(n); }

    map<string, EmployeeData*>* getDataMap() { return &mapData; }
};
