// productionlogmanager.h
#ifndef PRODUCTIONLOGMANAGER_H
#define PRODUCTIONLOGMANAGER_H

#include "productionlog.h"
#include <QVector>
#include <QString>

class ProductionLogManager {
private:
    QVector<ProductionLog*> logs;
    const QString logFile = "production_logs.txt";

    void loadLogs();
    void saveLogs();

public:
    ProductionLogManager();
    ~ProductionLogManager();

    bool addLog(const QString& date, const QString& itemName, int quantity);
    bool updateLog(const QString& oldDate, const QString& oldItemName, const QString& newDate, const QString& newItemName, int newQuantity);
    bool deleteLog(const QString& date, const QString& itemName);
    QVector<ProductionLog*> searchLogs(const QString& criterion, const QString& value) const;
    QVector<ProductionLog*> getLogs() const { return logs; }
};

#endif // PRODUCTIONLOGMANAGER_H
