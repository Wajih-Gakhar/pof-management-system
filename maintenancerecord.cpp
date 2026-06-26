// maintenancerecord.cpp
#include "maintenancerecord.h"

MaintenanceRecord::MaintenanceRecord(const QString& equipmentId, const QString& equipmentName, const QString& maintenanceDate,
                                     const QString& nextMaintenanceDate, const QString& action)
    : equipmentId(equipmentId), equipmentName(equipmentName), maintenanceDate(maintenanceDate),
    nextMaintenanceDate(nextMaintenanceDate), action(action) {
}

MaintenanceRecord::~MaintenanceRecord() = default;

// Getters
QString MaintenanceRecord::getEquipmentId() const {
    return equipmentId;
}

QString MaintenanceRecord::getEquipmentName() const {
    return equipmentName;
}

QString MaintenanceRecord::getMaintenanceDate() const {
    return maintenanceDate;
}

QString MaintenanceRecord::getNextMaintenanceDate() const {
    return nextMaintenanceDate;
}

QString MaintenanceRecord::getAction() const {
    return action;
}

// Setters
void MaintenanceRecord::setEquipmentId(const QString& equipmentId) {
    this->equipmentId = equipmentId;
}

void MaintenanceRecord::setEquipmentName(const QString& equipmentName) {
    this->equipmentName = equipmentName;
}

void MaintenanceRecord::setMaintenanceDate(const QString& maintenanceDate) {
    this->maintenanceDate = maintenanceDate;
}

void MaintenanceRecord::setNextMaintenanceDate(const QString& nextMaintenanceDate) {
    this->nextMaintenanceDate = nextMaintenanceDate;
}

void MaintenanceRecord::setAction(const QString& action) {
    this->action = action;
}
