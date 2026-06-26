// maintenancerecord.h
#ifndef MAINTENANCERECORD_H
#define MAINTENANCERECORD_H

#include <QString>

class MaintenanceRecord {
private:
    QString equipmentId;
    QString equipmentName;
    QString maintenanceDate;    // YYYY-MM-DD format
    QString nextMaintenanceDate; // YYYY-MM-DD format
    QString action;

public:
    MaintenanceRecord(const QString& equipmentId, const QString& equipmentName, const QString& maintenanceDate,
                      const QString& nextMaintenanceDate, const QString& action);
    virtual ~MaintenanceRecord();

    // Getters
    QString getEquipmentId() const;
    QString getEquipmentName() const;
    QString getMaintenanceDate() const;
    QString getNextMaintenanceDate() const;
    QString getAction() const;

    // Setters
    void setEquipmentId(const QString& equipmentId);
    void setEquipmentName(const QString& equipmentName);
    void setMaintenanceDate(const QString& maintenanceDate);
    void setNextMaintenanceDate(const QString& nextMaintenanceDate);
    void setAction(const QString& action);
};

#endif // MAINTENANCERECORD_H
