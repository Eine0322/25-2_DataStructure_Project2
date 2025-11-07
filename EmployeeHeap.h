// EmployeeHeap.h
#pragma once
#include "EmployeeData.h"

class EmployeeHeap {
private:
    EmployeeData** heapArr;  // Array-based heap
    int datanum;             // Number of data
    int maxCapacity;         // Max array size

public:
    EmployeeHeap() {
        datanum = 0;
        maxCapacity = 100;
        heapArr = new EmployeeData*[maxCapacity];
        for (int i = 0; i < maxCapacity; i++)
            heapArr[i] = nullptr;
    }

    ~EmployeeHeap() {
        for (int i = 1; i <= datanum; i++) {
            if (heapArr[i] != nullptr)
                delete heapArr[i];
        }
        delete[] heapArr;
    }

    void Insert(EmployeeData* data);
    EmployeeData* Top();
    void Delete();
    bool IsEmpty();

    int getDataNum() { return datanum; }

private:
    void UpHeap(int index);
    void DownHeap(int index);
    void ResizeArray();
};
