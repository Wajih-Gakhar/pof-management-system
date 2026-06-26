// productionlogmanager.cpp
#include "productionlogmanager.h"
#include <QFile>
#include <QTextStream>

ProductionLogManager::ProductionLogManager() {
    loadLogs();
}

ProductionLogManager::~ProductionLogManager() {
    for (ProductionLog* log : logs) {
        delete log;
    }
}

void ProductionLogManager::loadLogs() {
    QFile file(logFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 3) {
            bool ok;
            int quantity = parts[2].toInt(&ok);
            if (ok) {
                logs.push_back(new ProductionLog(parts[0], parts[1], quantity));
            }
        }
    }
    file.close();
}

void ProductionLogManager::saveLogs() {
    QFile file(logFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (const ProductionLog* log : logs) {
        out << log->getDate() << "," << log->getItemName() << "," << log->getQuantity() << "\n";
    }
    file.close();
}

bool ProductionLogManager::addLog(const QString& date, const QString& itemName, int quantity) {
    logs.push_back(new ProductionLog(date, itemName, quantity));
    saveLogs();
    return true;
}

bool ProductionLogManager::updateLog(const QString& oldDate, const QString& oldItemName, const QString& newDate, const QString& newItemName, int newQuantity) {
    for (int i = 0; i < logs.size(); ++i) {
        if (logs[i]->getDate() == oldDate && logs[i]->getItemName() == oldItemName) {
            logs[i]->setDate(newDate);
            logs[i]->setItemName(newItemName);
            logs[i]->setQuantity(newQuantity);
            saveLogs();
            return true;
        }
    }
    return false;
}

bool ProductionLogManager::deleteLog(const QString& date, const QString& itemName) {
    for (int i = 0; i < logs.size(); ++i) {
        if (logs[i]->getDate() == date && logs[i]->getItemName() == itemName) {
            delete logs[i];
            logs.remove(i);
            saveLogs();
            return true;
        }
    }
    return false;
}

QVector<ProductionLog*> ProductionLogManager::searchLogs(const QString& criterion, const QString& value) const {
    QVector<ProductionLog*> results;
    QString valLower = value.toLower();
    for (const ProductionLog* log : logs) {
        if (criterion == "Item Name" && log->getItemName().toLower().contains(valLower)) {
            results.push_back(const_cast<ProductionLog*>(log));
        } else if (criterion == "Date" && log->getDate().contains(value)) {
            results.push_back(const_cast<ProductionLog*>(log));
        }
    }
    return results;
}
