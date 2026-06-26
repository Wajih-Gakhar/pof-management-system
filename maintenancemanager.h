// maintenancemanager.h
#ifndef MAINTENANCEMANAGER_H
#define MAINTENANCEMANAGER_H

#include "maintenancerecord.h"
#include <QVector>
#include <QString>

class MaintenanceManager {
private:
    QVector<MaintenanceRecord*> records;
    const QString recordFile = "maintenance_records.txt";

    void loadRecords();
    void saveRecords();

public:
    MaintenanceManager();
    ~MaintenanceManager();

    bool addRecord(const QString& equipmentId, const QString& equipmentName, const QString& maintenanceDate,
                   const QString& nextMaintenanceDate, const QString& action);
    bool updateRecord(const QString& oldEquipmentId, const QString& oldMaintenanceDate, const QString& oldAction,
                      const QString& newEquipmentId, const QString& newEquipmentName, const QString& newMaintenanceDate,
                      const QString& newNextMaintenanceDate, const QString& newAction);
    bool deleteRecord(const QString& equipmentId, const QString& maintenanceDate, const QString& action);
    QVector<MaintenanceRecord*> searchRecords(const QString& equipmentId) const;
    QVector<MaintenanceRecord*> getRecords() const { return records; }
};

#endif // MAINTENANCEMANAGER_H
