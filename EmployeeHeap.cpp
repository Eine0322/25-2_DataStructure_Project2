// EmployeeHeap.cpp
#include "EmployeeHeap.h"

// Function to insert new employee data into the heap
void EmployeeHeap::Insert(EmployeeData* data) {
    if (datanum + 1 >= maxCapacity) {
        ResizeArray();
    }

    heapArr[++datanum] = data;
    UpHeap(datanum);
}

// Function to get the top element (highest salary)
EmployeeData* EmployeeHeap::Top() {
    if (IsEmpty()) return nullptr;
    return heapArr[1];
}

// Function to delete the top element
void EmployeeHeap::Delete() {
    if (IsEmpty()) return;
    heapArr[1] = heapArr[datanum];
    heapArr[datanum] = nullptr;
    datanum--;
    DownHeap(1);
}

// Function to check if heap is empty
bool EmployeeHeap::IsEmpty() {
    return datanum == 0;
}

// Function to adjust upward after insertion
void EmployeeHeap::UpHeap(int index) {
    if (index <= 1) return;
    int parent = index / 2;

    if (heapArr[parent]->GetAnnualIncome() < heapArr[index]->GetAnnualIncome()) {
        swap(heapArr[parent], heapArr[index]);
        UpHeap(parent);
    }
}

// Function to adjust downward after deletion
void EmployeeHeap::DownHeap(int index) {
    int left = index * 2;
    int right = index * 2 + 1;
    int largest = index;

    if (left <= datanum && heapArr[left]->GetAnnualIncome() > heapArr[largest]->GetAnnualIncome())
        largest = left;
    if (right <= datanum && heapArr[right]->GetAnnualIncome() > heapArr[largest]->GetAnnualIncome())
        largest = right;

    if (largest != index) {
        swap(heapArr[index], heapArr[largest]);
        DownHeap(largest);
    }
}

// Function to resize heap array
void EmployeeHeap::ResizeArray() {
    int newCapacity = maxCapacity * 2;
    EmployeeData** newArr = new EmployeeData*[newCapacity];

    for (int i = 0; i <= datanum; i++)
        newArr[i] = heapArr[i];

    delete[] heapArr;
    heapArr = newArr;
    maxCapacity = newCapacity;
}
