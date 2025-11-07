#include "EmployeeHeap.h"
#include <algorithm>

// insert pointer into heap, heap does not take ownership of EmployeeData
void EmployeeHeap::Insert(EmployeeData* data) {
    if (datanum + 1 >= maxCapacity) ResizeArray();
    heapArr[++datanum] = data;
    UpHeap(datanum);
}

// return top pointer, or nullptr if empty
EmployeeData* EmployeeHeap::Top() {
    if (datanum == 0) return nullptr;
    return heapArr[1];
}

// remove top element, do not delete EmployeeData pointer
void EmployeeHeap::Delete() {
    if (datanum == 0) return;
    heapArr[1] = heapArr[datanum];
    heapArr[datanum] = nullptr;
    --datanum;
    if (datanum > 0) DownHeap(1);
}

// check empty
bool EmployeeHeap::IsEmpty() const {
    return datanum == 0;
}

// export pointers in arbitrary order (not sorted)
std::vector<EmployeeData*> EmployeeHeap::GetAll() const {
    std::vector<EmployeeData*> out;
    out.reserve(datanum);
    for (int i = 1; i <= datanum; ++i) {
        if (heapArr[i] != nullptr) out.push_back(heapArr[i]);
    }
    return out;
}

// internal up-heap to maintain max-heap by annual income
void EmployeeHeap::UpHeap(int index) {
    if (index <= 1) return;
    int parent = index / 2;
    if (heapArr[parent]->GetAnnualIncome() < heapArr[index]->GetAnnualIncome()) {
        std::swap(heapArr[parent], heapArr[index]);
        UpHeap(parent);
    }
}

// internal down-heap to maintain max-heap by annual income
void EmployeeHeap::DownHeap(int index) {
    int left = index * 2;
    int right = index * 2 + 1;
    int largest = index;
    if (left <= datanum && heapArr[left]->GetAnnualIncome() > heapArr[largest]->GetAnnualIncome())
        largest = left;
    if (right <= datanum && heapArr[right]->GetAnnualIncome() > heapArr[largest]->GetAnnualIncome())
        largest = right;
    if (largest != index) {
        std::swap(heapArr[index], heapArr[largest]);
        DownHeap(largest);
    }
}

// resize internal array
void EmployeeHeap::ResizeArray() {
    int newCap = maxCapacity * 2;
    EmployeeData** newArr = new EmployeeData*[newCap];
    for (int i = 0; i <= datanum; ++i) newArr[i] = heapArr[i];
    for (int i = datanum + 1; i < newCap; ++i) newArr[i] = nullptr;
    delete[] heapArr;
    heapArr = newArr;
    maxCapacity = newCap;
}
