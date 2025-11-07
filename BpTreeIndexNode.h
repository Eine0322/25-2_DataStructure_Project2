// BpTreeIndexNode.h
#pragma once
#include "BpTreeNode.h"

// Class representing an Index Node in the B+ Tree
class BpTreeIndexNode : public BpTreeNode {
private:
    map<string, BpTreeNode*> mapIndex;

public:
    BpTreeIndexNode() {}
    ~BpTreeIndexNode() {}

    bool isIndexNode() const override { return true; }

    void insertIndexMap(string n, BpTreeNode* pN) {
        mapIndex.insert(map<string, BpTreeNode*>::value_type(n, pN));
    }

    void deleteMap(string n) { mapIndex.erase(n); }

    map<string, BpTreeNode*>* getIndexMap() { return &mapIndex; }
};
