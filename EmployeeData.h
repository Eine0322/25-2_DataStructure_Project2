// EmployeeData.h 
#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class EmployeeData {
private:
    string name;
    int dept_no;
    int employee_id;
    int annual_income;

public:
    EmployeeData() : name(""), dept_no(0), employee_id(0), annual_income(0) {}

    EmployeeData(string n, int d, int id, int income) 
        : name(n), dept_no(d), employee_id(id), annual_income(income) {}

    void SetName(string n) { name = n; }
    void SetDept(int d) { dept_no = d; }
    void SetID(int id) { employee_id = id; }
    void SetAnnualIncome(int income) { annual_income = income; }

    string GetName() const { return name; }
    int GetDept() const { return dept_no; }
    int GetID() const { return employee_id; }
    int GetAnnualIncome() const { return annual_income; }
};
