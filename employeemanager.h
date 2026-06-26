#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include "employee.h"
#include <QVector>
#include <QString>

class EmployeeManager {
private:
    QVector<Employee*> employees;
    const QString employeeFile = "employees.txt";

    void loadEmployees();
    void saveEmployees();

public:
    EmployeeManager();
    ~EmployeeManager();

    bool addEmployee(const QString& id, const QString& name, const QString& dept, const QString& desig);
    bool updateEmployee(const QString& id, const QString& name, const QString& dept, const QString& desig);
    bool deleteEmployee(const QString& id);
    QVector<Employee*> searchEmployees(const QString& criterion, const QString& value) const;
    QVector<Employee*> getEmployees() const { return employees; }
};

#endif // EMPLOYEEMANAGER_H
