#include "SelectionTree.h"

bool SelectionTree::Insert(string key, string value) {
    history.push(make_pair(key, value));
    return true;
}

bool SelectionTree::Delete() {
    if (history.empty()) {
        return false;
    }
    
    history.pop();
    return true;
}

bool SelectionTree::Print(int dept_no, BpTree* bptree) {
    if (history.empty()) {
        return false;
    }

    vector<pair<string, string>> items;
    stack<pair<string, string>> temp = history;
    
    while (!temp.empty()) {
        items.push_back(temp.top());
        temp.pop();
    }
    
    reverse(items.begin(), items.end());

    map<string, vector<EmployeeData*>> filtered;
    
    for (const auto& item : items) {
        string key = item.first;
        string value = item.second;
        
        if (key == "dept_no") {
            int filter_dept = stoi(value);
            
            BpTreeNode* node = bptree->getRoot();
            if (node == nullptr) continue;
            
            while (node->isIndexNode()) {
                node = node->getMostLeftChild();
            }
            
            while (node) {
                for (auto& kv : *node->getDataMap()) {
                    EmployeeData* e = kv.second;
                    if (e->GetDept() == filter_dept) {
                        filtered[e->GetName()].push_back(e);
                    }
                }
                node = node->getNext();
            }
        }
        else if (key == "name") {
            BpTreeNode* node = bptree->getRoot();
            if (node == nullptr) continue;
            
            while (node->isIndexNode()) {
                node = node->getMostLeftChild();
            }
            
            while (node) {
                auto it = node->getDataMap()->find(value);
                if (it != node->getDataMap()->end()) {
                    EmployeeData* e = it->second;
                    if (filtered.empty() || filtered.find(e->GetName()) != filtered.end()) {
                        filtered[e->GetName()].push_back(e);
                    }
                    break;
                }
                node = node->getNext();
            }
        }
    }
    
    if (filtered.empty()) {
        return false;
    }
    
    vector<EmployeeData*> results;
    for (auto& kv : filtered) {
        for (auto e : kv.second) {
            results.push_back(e);
        }
    }
    
    sort(results.begin(), results.end(), [](EmployeeData* a, EmployeeData* b) {
        return a->GetName() < b->GetName();
    });
    
    for (auto e : results) {
        *fout << e->GetName() << "/" << e->GetDept() << "/" 
              << e->GetID() << "/" << e->GetAnnualIncome() << endl;
    }
    
    return true;
}
