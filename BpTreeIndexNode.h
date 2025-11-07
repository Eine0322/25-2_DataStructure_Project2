// BpTreeIndexNode.h
#pragma once
#include "BpTreeNode.h"

class BpTreeIndexNode : public BpTreeNode {
private:
    map<string, BpTreeNode*> mapIndex;

public:
    BpTreeIndexNode() {}
    ~BpTreeIndexNode() {}

    bool isIndexNode() const override { return true; }

    void insertIndexMap(string n, BpTreeNode* pN) {
        mapIndex.insert({n, pN});
    }

    map<string, BpTreeNode*>* getIndexMap() { return &mapIndex; }
};
