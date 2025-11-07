// BpTree.cpp
#include "BpTree.h"
#include <cmath>

bool BpTree::Insert(EmployeeData* newData) {
    BpTreeNode* newnode;

    if (root == NULL) {
        newnode = new BpTreeDataNode();
        root = newnode;
        newnode->insertDataMap(newData->GetName(), newData);
        return true;
    }

    newnode = searchDataNode(newData->GetName());
    newnode->insertDataMap(newData->GetName(), newData);

    if (excessDataNode(newnode))
        splitDataNode(newnode);

    return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
    return (pDataNode->getDataMap()->size() > order - 1);
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
    return (pIndexNode->getIndexMap()->size() > order - 1);
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    int splitIndex = ceil((order - 1) / 2.0) + 1;
    auto dataMap = pDataNode->getDataMap();

    BpTreeNode* newRightNode = new BpTreeDataNode();
    auto it = dataMap->begin();
    advance(it, splitIndex - 1);

    for (auto i = it; i != dataMap->end(); ++i)
        newRightNode->insertDataMap(i->first, i->second);
    dataMap->erase(it, dataMap->end());

    BpTreeNode* parent = pDataNode->getParent();
    if (parent == NULL) {
        BpTreeNode* newIndex = new BpTreeIndexNode();
        newIndex->insertIndexMap(newRightNode->getDataMap()->begin()->first, newRightNode);
        newIndex->setMostLeftChild(pDataNode);
        pDataNode->setParent(newIndex);
        newRightNode->setParent(newIndex);
        root = newIndex;
    } else {
        parent->insertIndexMap(newRightNode->getDataMap()->begin()->first, newRightNode);
        newRightNode->setParent(parent);
    }

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

    if (excessIndexNode(pDataNode->getParent()))
        splitIndexNode(pDataNode->getParent());
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    auto iterLeft = pIndexNode->getIndexMap()->begin();
    auto iterMid = next(iterLeft);
    auto iterRight = next(iterMid);

    BpTreeNode* newRightNode = new BpTreeIndexNode();
    string splitKey = iterMid->first;

    newRightNode->insertIndexMap(iterRight->first, iterRight->second);
    newRightNode->setMostLeftChild(iterMid->second);
    iterRight->second->setParent(newRightNode);
    iterMid->second->setParent(newRightNode);

    pIndexNode->getIndexMap()->erase(iterMid->first);
    pIndexNode->getIndexMap()->erase(iterRight->first);

    BpTreeNode* parent = pIndexNode->getParent();
    if (parent == NULL) {
        BpTreeNode* newIndex = new BpTreeIndexNode();
        newIndex->insertIndexMap(splitKey, newRightNode);
        newIndex->setMostLeftChild(pIndexNode);
        pIndexNode->setParent(newIndex);
        newRightNode->setParent(newIndex);
        root = newIndex;
    } else {
        parent->insertIndexMap(splitKey, newRightNode);
        newRightNode->setParent(parent);
    }

    if (excessIndexNode(pIndexNode->getParent()))
        splitIndexNode(pIndexNode->getParent());
}

BpTreeNode* BpTree::searchDataNode(string name) {
    BpTreeNode* cur = root;
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
    return cur;
}

bool BpTree::SearchModel(string name, int flag) {
    if (root == NULL)
        return false;

    BpTreeNode* target = searchDataNode(name);
    auto it = target->getDataMap()->find(name);

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

    BpTreeNode* node = searchDataNode(start);
    while (node) {
        for (auto& kv : *node->getDataMap()) {
            string key = kv.first;
            if (key >= start && key <= end)
                result.push_back(key);
        }
        node = node->getNext();
    }
    return result;
}

void BpTree::Print(BpTreeNode* node) {
    while (node->isIndexNode())
        node = node->getMostLeftChild();

    while (node) {
        for (auto& kv : *node->getDataMap()) {
            EmployeeData* e = kv.second;
            *fout << e->GetName() << "/" << e->GetDept() << "/" << e->GetID() << "/" << e->GetAnnualIncome() << endl;
        }
        node = node->getNext();
    }
}

bool BpTree::Print() {
    if (root == NULL) return false;
    *fout << "========PRINT_BP========" << endl;
    Print(root);
    *fout << "========================" << endl;
    return true;
}

// delete all nodes and free EmployeeData pointers in data nodes
void BpTree::deleteSubTree(BpTreeNode* node) {
    if (node == nullptr) return;

    if (node->isIndexNode()) {
        // recursively delete child nodes
        for (auto& pair : *(node->getIndexMap()))
            deleteSubTree(pair.second);
        node->getIndexMap()->clear();
    }
    else if (node->isDataNode()) {
        // delete all EmployeeData pointers stored in this data node
        auto dataMap = node->getDataMap();
        for (auto& kv : *dataMap) {
            if (kv.second) delete kv.second; // free employee object
        }
        dataMap->clear();
    }
    delete node; // delete node itself
}
