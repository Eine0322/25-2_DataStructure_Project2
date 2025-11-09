#pragma once
#include <vector>
#include "EmployeeData.h"

// simple array-based max-heap for EmployeeData pointers, does not own EmployeeData
class EmployeeHeap {
private:
    EmployeeData** heapArr;  // array of pointers, index starts at 1
    int datanum;             // current number of elements in the heap
    int maxCapacity;         // current allocated capacity of the array

public:
    EmployeeHeap() {
        datanum = 0; // initialize element count
        maxCapacity = 100; // default initial capacity
        heapArr = new EmployeeData*[maxCapacity]; // allocate array
        for (int i = 0; i < maxCapacity; ++i) heapArr[i] = nullptr; // initialize to null
    }

    // destructor must not delete EmployeeData pointers (B+ Tree owns them)
    ~EmployeeHeap() {
        delete[] heapArr; // free only the internal array, not the data itself
    }

    // prevent copying to avoid shallow-copy issues and double deletion
    EmployeeHeap(const EmployeeHeap&) = delete;
    EmployeeHeap& operator=(const EmployeeHeap&) = delete;

    // insert a new EmployeeData pointer into heap
    void Insert(EmployeeData* data);

    // return top element (max income), nullptr if empty
    EmployeeData* Top();

    // remove top element without deleting data
    void Delete(); 

    // check if heap is empty
    bool IsEmpty() const;

    // return number of elements currently stored
    int getDataNum() const { return datanum; }

    // return all EmployeeData pointers (unsorted, for inspection or processing)
    std::vector<EmployeeData*> GetAll() const;

private:
    // adjust upward to maintain max-heap property after insertion
    void UpHeap(int index);

    // adjust downward to maintain max-heap property after deletion
    void DownHeap(int index);

    // expand internal array when capacity is exceeded
    void ResizeArray();
};
