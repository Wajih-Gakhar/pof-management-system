// employeemanager.cpp
#include "employeemanager.h"
#include <QFile>
#include <QTextStream>

EmployeeManager::EmployeeManager() {
    loadEmployees();
}

EmployeeManager::~EmployeeManager() {
    for (Employee* emp : employees) {
        delete emp;
    }
}

void EmployeeManager::loadEmployees() {
    QFile file(employeeFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 4) {
            employees.push_back(new Employee(parts[0], parts[1], parts[2], parts[3]));
        }
    }
    file.close();
}

void EmployeeManager::saveEmployees() {
    QFile file(employeeFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (const Employee* emp : employees) {
        out << emp->getId() << "," << emp->getName() << ","
            << emp->getDepartment() << "," << emp->getDesignation() << "\n";
    }
    file.close();
}

bool EmployeeManager::addEmployee(const QString& id, const QString& name, const QString& dept, const QString& desig) {
    for (const Employee* emp : employees) {
        if (emp->getId() == id) {
            return false; // ID already exists
        }
    }
    employees.push_back(new Employee(id, name, dept, desig));
    saveEmployees();
    return true;
}

bool EmployeeManager::updateEmployee(const QString& id, const QString& name, const QString& dept, const QString& desig) {
    for (Employee* emp : employees) {
        if (emp->getId() == id) {
            emp->setName(name);
            emp->setDepartment(dept);
            emp->setDesignation(desig);
            saveEmployees();
            return true;
        }
    }
    return false;
}

bool EmployeeManager::deleteEmployee(const QString& id) {
    for (int i = 0; i < employees.size(); ++i) {
        if (employees[i]->getId() == id) {
            delete employees[i];
            employees.remove(i);
            saveEmployees();
            return true;
        }
    }
    return false;
}

QVector<Employee*> EmployeeManager::searchEmployees(const QString& criterion, const QString& value) const {
    QVector<Employee*> results;
    QString valLower = value.toLower();
    for (const Employee* emp : employees) {
        if (criterion == "ID" && emp->getId().toLower().contains(valLower)) {
            results.push_back(const_cast<Employee*>(emp));
        } else if (criterion == "Name" && emp->getName().toLower().contains(valLower)) {
            results.push_back(const_cast<Employee*>(emp));
        } else if (criterion == "Department" && emp->getDepartment().toLower().contains(valLower)) {
            results.push_back(const_cast<Employee*>(emp));
        } else if (criterion == "Designation" && emp->getDesignation().toLower().contains(valLower)) { // Added Designation
            results.push_back(const_cast<Employee*>(emp));
        }
    }
    return results;
}
