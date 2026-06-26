// maintenancemanager.cpp
#include "maintenancemanager.h"
#include <QFile>
#include <QTextStream>

MaintenanceManager::MaintenanceManager() {
    loadRecords();
}

MaintenanceManager::~MaintenanceManager() {
    for (MaintenanceRecord* record : records) {
        delete record;
    }
}

void MaintenanceManager::loadRecords() {
    QFile file(recordFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 5) {
            records.push_back(new MaintenanceRecord(parts[0], parts[1], parts[2], parts[3], parts[4]));
        }
    }
    file.close();
}

void MaintenanceManager::saveRecords() {
    QFile file(recordFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (const MaintenanceRecord* record : records) {
        out << record->getEquipmentId() << "," << record->getEquipmentName() << ","
            << record->getMaintenanceDate() << "," << record->getNextMaintenanceDate() << ","
            << record->getAction() << "\n";
    }
    file.close();
}

bool MaintenanceManager::addRecord(const QString& equipmentId, const QString& equipmentName, const QString& maintenanceDate,
                                   const QString& nextMaintenanceDate, const QString& action) {
    records.push_back(new MaintenanceRecord(equipmentId, equipmentName, maintenanceDate, nextMaintenanceDate, action));
    saveRecords();
    return true;
}

bool MaintenanceManager::updateRecord(const QString& oldEquipmentId, const QString& oldMaintenanceDate, const QString& oldAction,
                                      const QString& newEquipmentId, const QString& newEquipmentName, const QString& newMaintenanceDate,
                                      const QString& newNextMaintenanceDate, const QString& newAction) {
    for (int i = 0; i < records.size(); ++i) {
        if (records[i]->getEquipmentId() == oldEquipmentId &&
            records[i]->getMaintenanceDate() == oldMaintenanceDate &&
            records[i]->getAction() == oldAction) {
            records[i]->setEquipmentId(newEquipmentId);
            records[i]->setEquipmentName(newEquipmentName);
            records[i]->setMaintenanceDate(newMaintenanceDate);
            records[i]->setNextMaintenanceDate(newNextMaintenanceDate);
            records[i]->setAction(newAction);
            saveRecords();
            return true;
        }
    }
    return false;
}

bool MaintenanceManager::deleteRecord(const QString& equipmentId, const QString& maintenanceDate, const QString& action) {
    for (int i = 0; i < records.size(); ++i) {
        if (records[i]->getEquipmentId() == equipmentId &&
            records[i]->getMaintenanceDate() == maintenanceDate &&
            records[i]->getAction() == action) {
            delete records[i];
            records.remove(i);
            saveRecords();
            return true;
        }
    }
    return false;
}

QVector<MaintenanceRecord*> MaintenanceManager::searchRecords(const QString& equipmentId) const {
    QVector<MaintenanceRecord*> results;
    QString idLower = equipmentId.toLower();
    for (const MaintenanceRecord* record : records) {
        if (record->getEquipmentId().toLower().contains(idLower)) {
            results.push_back(const_cast<MaintenanceRecord*>(record));
        }
    }
    return results;
}
