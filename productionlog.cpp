// productionlog.cpp
#include "productionlog.h"

ProductionLog::ProductionLog(const QString& date, const QString& itemName, int quantity)
    : date(date), itemName(itemName), quantity(quantity) {
}

ProductionLog::~ProductionLog() = default;

// Getters
QString ProductionLog::getDate() const {
    return date;
}

QString ProductionLog::getItemName() const {
    return itemName;
}

int ProductionLog::getQuantity() const {
    return quantity;
}

// Setters
void ProductionLog::setDate(const QString& date) {
    this->date = date;
}

void ProductionLog::setItemName(const QString& itemName) {
    this->itemName = itemName;
}

void ProductionLog::setQuantity(int quantity) {
    this->quantity = quantity;
}
