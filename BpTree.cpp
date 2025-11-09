#include "BpTree.h"
#include <cmath>

bool BpTree::Insert(EmployeeData* newData) {
    BpTreeNode* newnode;

    // If the tree is empty, create the first data node and set it as root
    if (root == NULL) {
        newnode = new BpTreeDataNode();
        root = newnode;
        newnode->insertDataMap(newData->GetName(), newData);
        return true;
    }

    // Find the appropriate data node for insertion
    newnode = searchDataNode(newData->GetName());
    newnode->insertDataMap(newData->GetName(), newData);

    // Check if the node exceeds its capacity and needs to be split
    if (excessDataNode(newnode))
        splitDataNode(newnode);

    return true;
}

// Check if a data node exceeds the maximum number of allowed entries
bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
    return (pDataNode->getDataMap()->size() > order - 1);
}

// Check if an index node exceeds the maximum number of allowed entries
bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
    return (pIndexNode->getIndexMap()->size() > order - 1);
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    int splitIndex = ceil((order - 1) / 2.0) + 1; // Calculate split position
    auto dataMap = pDataNode->getDataMap();

    // Create a new right data node for the split
    BpTreeNode* newRightNode = new BpTreeDataNode();
    auto it = dataMap->begin();
    advance(it, splitIndex - 1); // Move iterator to split position

    // Move data to the right node
    for (auto i = it; i != dataMap->end(); ++i)
        newRightNode->insertDataMap(i->first, i->second);
    dataMap->erase(it, dataMap->end()); // Remove moved elements from original node

    BpTreeNode* parent = pDataNode->getParent();
    if (parent == NULL) {
        // If no parent exists, create a new root index node
        BpTreeNode* newIndex = new BpTreeIndexNode();
        newIndex->insertIndexMap(newRightNode->getDataMap()->begin()->first, newRightNode);
        newIndex->setMostLeftChild(pDataNode);
        pDataNode->setParent(newIndex);
        newRightNode->setParent(newIndex);
        root = newIndex; // Update the tree root
    } else {
        // Insert the split key into the existing parent
        parent->insertIndexMap(newRightNode->getDataMap()->begin()->first, newRightNode);
        newRightNode->setParent(parent);
    }

    // Update linked list pointers between data nodes
    BpTreeNode* nextNode = pDataNode->getNext();
    if (nextNode != NULL) {
        nextNode->setPrev(newRightNode);
        newRightNode->setNext(nextNode);
        newRightNode->setPrev(pDataNode);
        pDataNode->setNext(newRightNode);
    } else {
        pDataNode->setNext(newRightNode);
        newRightNode->setPrev(pDataNode);
    }

    // If the parent index node now exceeds capacity, split it recursively
    if (excessIndexNode(pDataNode->getParent()))
        splitIndexNode(pDataNode->getParent());
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    // Split the index node around the middle key
    auto iterLeft = pIndexNode->getIndexMap()->begin();
    auto iterMid = next(iterLeft);
    auto iterRight = next(iterMid);

    BpTreeNode* newRightNode = new BpTreeIndexNode();
    string splitKey = iterMid->first;

    // Move right-side keys to the new index node
    newRightNode->insertIndexMap(iterRight->first, iterRight->second);
    newRightNode->setMostLeftChild(iterMid->second);
    iterRight->second->setParent(newRightNode);
    iterMid->second->setParent(newRightNode);

    // Remove moved keys from the original node
    pIndexNode->getIndexMap()->erase(iterMid->first);
    pIndexNode->getIndexMap()->erase(iterRight->first);

    BpTreeNode* parent = pIndexNode->getParent();
    if (parent == NULL) {
        // Create a new root if no parent exists
        BpTreeNode* newIndex = new BpTreeIndexNode();
        newIndex->insertIndexMap(splitKey, newRightNode);
        newIndex->setMostLeftChild(pIndexNode);
        pIndexNode->setParent(newIndex);
        newRightNode->setParent(newIndex);
        root = newIndex; // Update tree root
    } else {
        // Insert the split key into the parent
        parent->insertIndexMap(splitKey, newRightNode);
        newRightNode->setParent(parent);
    }

    // Recursively split parent if it exceeds capacity
    if (excessIndexNode(pIndexNode->getParent()))
        splitIndexNode(pIndexNode->getParent());
}

BpTreeNode* BpTree::searchDataNode(string name) {
    BpTreeNode* cur = root;
    // Traverse index nodes until a data node is reached
    while (cur != NULL && cur->isIndexNode()) {
        map<string, BpTreeNode*>* indexMap = cur->getIndexMap();
        auto it = indexMap->upper_bound(name);
        if (it == indexMap->begin())
            cur = cur->getMostLeftChild();
        else {
            it--;
            cur = it->second;
        }
    }
    return cur; // Return the located data node
}

bool BpTree::SearchModel(string name, int flag) {
    if (root == NULL)
        return false;

    // Find the data node containing the target key
    BpTreeNode* target = searchDataNode(name);
    auto it = target->getDataMap()->find(name);

    // If the key exists, print the data in the selected format
    if (it != target->getDataMap()->end()) {
        EmployeeData* e = it->second;
        if (flag == 1)
            *fout << e->GetName() << "/" << e->GetDept() << "/" << e->GetID() << "/" << e->GetAnnualIncome() << endl;
        else {
            *fout << "========SEARCH_BP========" << endl;
            *fout << e->GetName() << "/" << e->GetDept() << "/" << e->GetID() << "/" << e->GetAnnualIncome() << endl;
            *fout << "=========================" << endl;
        }
        return true;
    }
    return false;
}

vector<string> BpTree::SearchRange(string start, string end) {
    vector<string> result;
    if (root == NULL) return result;

    // Start searching from the node containing the lower bound key
    BpTreeNode* node = searchDataNode(start);
    while (node) {
        for (auto& kv : *node->getDataMap()) {
            string key = kv.first;
            // Include all keys within the given range [start, end]
            if (key >= start && key <= end)
                result.push_back(key);
        }
        node = node->getNext(); // Move to next data node
    }
    return result;
}

void BpTree::Print(BpTreeNode* node) {
    // Traverse to the leftmost data node
    while (node->isIndexNode())
        node = node->getMostLeftChild();

    // Sequentially print all data nodes
    while (node) {
        for (auto& kv : *node->getDataMap()) {
            EmployeeData* e = kv.second;
            *fout << e->GetName() << "/" << e->GetDept() << "/" << e->GetID() << "/" << e->GetAnnualIncome() << endl;
        }
        node = node->getNext();
    }
}

bool BpTree::Print() {
    // Print the entire B+ tree contents in order
    if (root == NULL) return false;
    *fout << "========PRINT_BP========" << endl;
    Print(root);
    *fout << "========================" << endl;
    return true;
}

// Recursively delete all nodes and free allocated EmployeeData objects
void BpTree::deleteSubTree(BpTreeNode* node) {
    if (node == nullptr) return;

    if (node->isIndexNode()) {
        // Recursively delete all child nodes of index node
        for (auto& pair : *(node->getIndexMap()))
            deleteSubTree(pair.second);
        node->getIndexMap()->clear();
    }
    else if (node->isDataNode()) {
        // Delete all EmployeeData pointers in data node
        auto dataMap = node->getDataMap();
        for (auto& kv : *dataMap) {
            if (kv.second) delete kv.second; // Free dynamically allocated object
        }
        dataMap->clear();
    }
    delete node; // Delete the node itself
}
