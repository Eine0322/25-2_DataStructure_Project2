#pragma once
#include "BpTreeNode.h"

// Class representing an index node in the B+ tree
class BpTreeIndexNode : public BpTreeNode {
private:
    map<string, BpTreeNode*> mapIndex;  // Map storing key–child node pairs

public:
    // Constructor
    BpTreeIndexNode() {}

    // Destructor
    ~BpTreeIndexNode() {}

    // Identify this node as an index node
    bool isIndexNode() const override { return true; }

    // Insert a key–child pair into the index map
    void insertIndexMap(string n, BpTreeNode* pN) {
        mapIndex.insert({n, pN});
    }

    // Return a pointer to the internal index map
    map<string, BpTreeNode*>* getIndexMap() { return &mapIndex; }
};
