#include "EmployeeHeap.h"
#include <algorithm>

// insert pointer into heap, heap does not take ownership of EmployeeData
void EmployeeHeap::Insert(EmployeeData* data) {
    if (datanum + 1 >= maxCapacity) ResizeArray(); // resize if array is full
    heapArr[++datanum] = data; // insert data at the end
    UpHeap(datanum); // adjust heap upward to maintain max-heap property
}

// return top pointer, or nullptr if empty
EmployeeData* EmployeeHeap::Top() {
    if (datanum == 0) return nullptr; // return null if heap is empty
    return heapArr[1]; // return the top element (max income)
}

// remove top element, do not delete EmployeeData pointer
void EmployeeHeap::Delete() {
    if (datanum == 0) return; // do nothing if heap is empty
    heapArr[1] = heapArr[datanum]; // move last element to root
    heapArr[datanum] = nullptr; // clear the last position
    --datanum; // decrease heap size
    if (datanum > 0) DownHeap(1); // adjust heap downward to maintain order
}

// check empty
bool EmployeeHeap::IsEmpty() const {
    return datanum == 0; // true if no data in heap
}

// export pointers in arbitrary order (not sorted)
std::vector<EmployeeData*> EmployeeHeap::GetAll() const {
    std::vector<EmployeeData*> out;
    out.reserve(datanum); // reserve space for efficiency
    for (int i = 1; i <= datanum; ++i) {
        if (heapArr[i] != nullptr) out.push_back(heapArr[i]); // add valid pointers
    }
    return out; // return all heap elements
}

// internal up-heap to maintain max-heap by annual income
void EmployeeHeap::UpHeap(int index) {
    if (index <= 1) return; // stop if at root
    int parent = index / 2;
    if (heapArr[parent]->GetAnnualIncome() < heapArr[index]->GetAnnualIncome()) {
        std::swap(heapArr[parent], heapArr[index]); // swap with parent if income is greater
        UpHeap(parent); // continue upward
    }
}

// internal down-heap to maintain max-heap by annual income
void EmployeeHeap::DownHeap(int index) {
    int left = index * 2;
    int right = index * 2 + 1;
    int largest = index;

    // find child with larger income
    if (left <= datanum && heapArr[left]->GetAnnualIncome() > heapArr[largest]->GetAnnualIncome())
        largest = left;
    if (right <= datanum && heapArr[right]->GetAnnualIncome() > heapArr[largest]->GetAnnualIncome())
        largest = right;

    // if parent is smaller, swap and continue down
    if (largest != index) {
        std::swap(heapArr[index], heapArr[largest]);
        DownHeap(largest);
    }
}

// resize internal array
void EmployeeHeap::ResizeArray() {
    int newCap = maxCapacity * 2; // double the capacity
    EmployeeData** newArr = new EmployeeData*[newCap]; // allocate new array

    // copy old data to new array
    for (int i = 0; i <= datanum; ++i) newArr[i] = heapArr[i];

    // initialize remaining positions to nullptr
    for (int i = datanum + 1; i < newCap; ++i) newArr[i] = nullptr;

    delete[] heapArr; // free old array
    heapArr = newArr; // update pointer
    maxCapacity = newCap; // update capacity
}
