#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// Class representing employee information stored in the B+ tree
class EmployeeData {
private:
    string name;          // Employee name (used as key in B+ tree)
    int dept_no;          // Department number
    int employee_id;      // Unique employee ID
    int annual_income;    // Annual income of the employee

public:
    // Default constructor: initialize with empty/zero values
    EmployeeData() : name(""), dept_no(0), employee_id(0), annual_income(0) {}

    // Parameterized constructor: initialize all member variables
    EmployeeData(string n, int d, int id, int income)
        : name(n), dept_no(d), employee_id(id), annual_income(income) {}

    // Setters — update individual member variables
    void SetName(string n) { name = n; }
    void SetDept(int d) { dept_no = d; }
    void SetID(int id) { employee_id = id; }
    void SetAnnualIncome(int income) { annual_income = income; }

    // Getters — retrieve stored values
    string GetName() const { return name; }
    int GetDept() const { return dept_no; }
    int GetID() const { return employee_id; }
    int GetAnnualIncome() const { return annual_income; }
};
