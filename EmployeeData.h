// EmployeeData.h
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// Class representing an employee's data
class EmployeeData {
private:
    string name;       // employee name (unique, lowercase)
    int dept_no;       // department code (100~800)
    int id;            // employee ID
    int annual_income; // annual income

public:
    EmployeeData() {
        name = "";
        dept_no = 0;
        id = 0;
        annual_income = 0;
    }
    ~EmployeeData() {}

    // Set functions
    void SetName(string n) { name = n; }
    void SetDept(int d) { dept_no = d; }
    void SetID(int i) { id = i; }
    void SetAnnualIncome(int income) { annual_income = income; }

    // Get functions
    string GetName() { return name; }
    int GetDept() { return dept_no; }
    int GetID() { return id; }
    int GetAnnualIncome() { return annual_income; }
};
