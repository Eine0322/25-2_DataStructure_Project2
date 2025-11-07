// BpTreeNode.h
#pragma once
#include "EmployeeData.h"

class BpTreeNode {
private:
    BpTreeNode* pParent;
    BpTreeNode* pMostLeftChild;

public:
    BpTreeNode() {
        pParent = NULL;
        pMostLeftChild = NULL;
    }
    virtual ~BpTreeNode() {}

    virtual bool isIndexNode() const { return false; }
    virtual bool isDataNode() const { return false; }

    void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; }
    void setParent(BpTreeNode* pN) { pParent = pN; }

    BpTreeNode* getParent() { return pParent; }
    BpTreeNode* getMostLeftChild() { return pMostLeftChild; }

    virtual void setNext(BpTreeNode* pN) {}
    virtual void setPrev(BpTreeNode* pN) {}
    virtual BpTreeNode* getNext() { return NULL; }
    virtual BpTreeNode* getPrev() { return NULL; }

    virtual void insertDataMap(string n, EmployeeData* pN) {}
    virtual void insertIndexMap(string n, BpTreeNode* pN) {}
    virtual void deleteMap(string n) {}

    virtual map<string, BpTreeNode*>* getIndexMap() {
        static map<string, BpTreeNode*> m;
        return &m;
    }
    virtual map<string, EmployeeData*>* getDataMap() {
        static map<string, EmployeeData*> m;
        return &m;
    }
};