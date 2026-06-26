#include "employee.h"

Employee::Employee(const QString& id, const QString& name, const QString& dept, const QString& desig)
    : id(id), name(name), department(dept), designation(desig) {
}

Employee::~Employee() = default;

// Getters
QString Employee::getId() const {
    return id;
}

QString Employee::getName() const {
    return name;
}

QString Employee::getDepartment() const {
    return department;
}

QString Employee::getDesignation() const {
    return designation;
}

// Setters
void Employee::setName(const QString& name) {
    this->name = name;
}

void Employee::setDepartment(const QString& dept) {
    this->department = dept;
}

void Employee::setDesignation(const QString& desig) {
    this->designation = desig;
}
