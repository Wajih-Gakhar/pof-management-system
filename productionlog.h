// productionlog.h
#ifndef PRODUCTIONLOG_H
#define PRODUCTIONLOG_H

#include <QString>

class ProductionLog {
private:
    QString date;      // YYYY-MM-DD format
    QString itemName;
    int quantity;

public:
    ProductionLog(const QString& date, const QString& itemName, int quantity); // Ensure this constructor exists
    virtual ~ProductionLog();

    // Getters
    QString getDate() const;
    QString getItemName() const;
    int getQuantity() const;

    // Setters
    void setDate(const QString& date);
    void setItemName(const QString& itemName);
    void setQuantity(int quantity);
};

#endif // PRODUCTIONLOG_H
