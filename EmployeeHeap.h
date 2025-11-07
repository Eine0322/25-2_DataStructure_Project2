#pragma once
#include <vector>
#include "EmployeeData.h"

// simple array-based max-heap for EmployeeData pointers, does not own EmployeeData
class EmployeeHeap {
private:
    EmployeeData** heapArr;  // array of pointers, index starts at 1
    int datanum;             // current number of elements
    int maxCapacity;         // allocated size of array

public:
    EmployeeHeap() {
        datanum = 0;
        maxCapacity = 100;
        heapArr = new EmployeeData*[maxCapacity];
        for (int i = 0; i < maxCapacity; ++i) heapArr[i] = nullptr;
    }

    // destructor must not delete EmployeeData pointers (B+ Tree owns them)
    ~EmployeeHeap() {
        delete[] heapArr; // free array storage only
    }

    // prevent copying to avoid shallow-copy issues
    EmployeeHeap(const EmployeeHeap&) = delete;
    EmployeeHeap& operator=(const EmployeeHeap&) = delete;

    void Insert(EmployeeData* data);
    EmployeeData* Top();
    void Delete(); 
    bool IsEmpty() const;

    int getDataNum() const { return datanum; }

    // safely export current pointers without copying internal array
    std::vector<EmployeeData*> GetAll() const;

private:
    void UpHeap(int index);
    void DownHeap(int index);
    void ResizeArray();
};
