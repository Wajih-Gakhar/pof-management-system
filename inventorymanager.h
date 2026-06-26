// inventorymanager.h
#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "item.h"
#include <QVector>
#include <QString>

class InventoryManager {
private:
    QVector<Item*> items;
    const QString inventoryFile = "inventory.txt";

    void loadItems();
    void saveItems();

public:
    InventoryManager();
    ~InventoryManager();

    bool addItem(const QString& id, const QString& name, const QString& category, int quantity, const QString& status);
    bool updateItem(const QString& id, const QString& name, const QString& category, int quantity, const QString& status);
    bool deleteItem(const QString& id);
    QVector<Item*> searchItems(const QString& criterion, const QString& value) const;
    QVector<Item*> getItems() const { return items; }
};

#endif // INVENTORYMANAGER_H
