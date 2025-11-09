#pragma once
#include "BpTreeNode.h"
#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"

class BpTree {
private:
    BpTreeNode* root;      // Pointer to the root node of the B+ tree
    int order;             // Maximum number of keys per node (tree order)
    ofstream* fout;        // Output file stream for printing results

public:
    // Constructor: initialize tree parameters
    BpTree(ofstream* fout, int order = 3) {
        root = NULL;             // Initialize root to null
        this->order = order;     // Set tree order
        this->fout = fout;       // Assign output stream pointer
    }

    // Destructor: delete all nodes recursively
    ~BpTree() { deleteSubTree(root); }

    // Insert a new EmployeeData object into the B+ tree
    bool Insert(EmployeeData* newData);

    // Check if a data node exceeds its capacity
    bool excessDataNode(BpTreeNode* pDataNode);

    // Check if an index node exceeds its capacity
    bool excessIndexNode(BpTreeNode* pIndexNode);

    // Split a data node when it overflows
    void splitDataNode(BpTreeNode* pDataNode);

    // Split an index node when it overflows
    void splitIndexNode(BpTreeNode* pIndexNode);

    // Search for the data node that should contain a specific key
    BpTreeNode* searchDataNode(string name);

    // Search for an employee record by name and print result
    bool SearchModel(string name, int flag);

    // Search for all employee names within a given range
    vector<string> SearchRange(string start, string end);

    // Print the content of all data nodes starting from the given node
    void Print(BpTreeNode* node);

    // Print the entire B+ tree contents
    bool Print();

    // Return the root node pointer
    BpTreeNode* getRoot() { return root; }

private:
    // Recursively delete all nodes and free allocated data
    void deleteSubTree(BpTreeNode* node);
};
