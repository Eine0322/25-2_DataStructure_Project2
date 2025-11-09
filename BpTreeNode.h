#pragma once
#include "EmployeeData.h"

// Base class representing a generic node in the B+ tree
class BpTreeNode {
private:
    BpTreeNode* pParent;          // Pointer to the parent node
    BpTreeNode* pMostLeftChild;   // Pointer to the leftmost child (used in index nodes)

public:
    // Constructor: initialize parent and child pointers
    BpTreeNode() {
        pParent = NULL;
        pMostLeftChild = NULL;
    }

    // Virtual destructor for safe polymorphic deletion
    virtual ~BpTreeNode() {}

    // Identify node type — overridden in derived classes
    virtual bool isIndexNode() const { return false; }
    virtual bool isDataNode() const { return false; }

    // Setters for parent and leftmost child pointers
    void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; }
    void setParent(BpTreeNode* pN) { pParent = pN; }

    // Getters for parent and leftmost child pointers
    BpTreeNode* getParent() { return pParent; }
    BpTreeNode* getMostLeftChild() { return pMostLeftChild; }

    // Virtual linked list methods (implemented in data nodes)
    virtual void setNext(BpTreeNode* pN) {}
    virtual void setPrev(BpTreeNode* pN) {}
    virtual BpTreeNode* getNext() { return NULL; }
    virtual BpTreeNode* getPrev() { return NULL; }

    // Virtual insertion and deletion methods for polymorphic access
    virtual void insertDataMap(string n, EmployeeData* pN) {}
    virtual void insertIndexMap(string n, BpTreeNode* pN) {}
    virtual void deleteMap(string n) {}

    // Virtual getters returning maps for each node type
    virtual map<string, BpTreeNode*>* getIndexMap() {
        static map<string, BpTreeNode*> m;
        return &m;
    }
    virtual map<string, EmployeeData*>* getDataMap() {
        static map<string, EmployeeData*> m;
        return &m;
    }
};
