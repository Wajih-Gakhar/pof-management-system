#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

class Employee {
private:
    QString id; // Unique ID
    QString name;
    QString department;
    QString designation;

public:
    Employee(const QString& id, const QString& name, const QString& dept, const QString& desig);
    virtual ~Employee();

    // Getters
    QString getId() const;
    QString getName() const;
    QString getDepartment() const;
    QString getDesignation() const;

    // Setters (for updating)
    void setName(const QString& name);
    void setDepartment(const QString& dept);
    void setDesignation(const QString& desig);
};

#endif // EMPLOYEE_H
