// inventorymanager.cpp
#include "inventorymanager.h"
#include <QFile>
#include <QTextStream>

InventoryManager::InventoryManager() {
    loadItems();
}

InventoryManager::~InventoryManager() {
    for (Item* item : items) {
        delete item;
    }
}

void InventoryManager::loadItems() {
    QFile file(inventoryFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 5) {
            bool ok;
            int quantity = parts[3].toInt(&ok);
            if (ok) {
                items.push_back(new Item(parts[0], parts[1], parts[2], quantity, parts[4]));
            }
        }
    }
    file.close();
}

void InventoryManager::saveItems() {
    QFile file(inventoryFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (const Item* item : items) {
        out << item->getId() << "," << item->getName() << ","
            << item->getCategory() << "," << item->getQuantity() << ","
            << item->getStatus() << "\n";
    }
    file.close();
}

bool InventoryManager::addItem(const QString& id, const QString& name, const QString& category, int quantity, const QString& status) {
    for (const Item* item : items) {
        if (item->getId() == id) {
            return false; // ID already exists
        }
    }
    items.push_back(new Item(id, name, category, quantity, status));
    saveItems();
    return true;
}

bool InventoryManager::updateItem(const QString& id, const QString& name, const QString& category, int quantity, const QString& status) {
    for (Item* item : items) {
        if (item->getId() == id) {
            item->setName(name);
            item->setCategory(category);
            item->setQuantity(quantity);
            item->setStatus(status);
            saveItems();
            return true;
        }
    }
    return false;
}

bool InventoryManager::deleteItem(const QString& id) {
    for (int i = 0; i < items.size(); ++i) {
        if (items[i]->getId() == id) {
            delete items[i];
            items.remove(i);
            saveItems();
            return true;
        }
    }
    return false;
}

QVector<Item*> InventoryManager::searchItems(const QString& criterion, const QString& value) const {
    QVector<Item*> results;
    QString valLower = value.toLower();
    for (const Item* item : items) {
        if (criterion == "Name" && item->getName().toLower().contains(valLower)) {
            results.push_back(const_cast<Item*>(item));
        } else if (criterion == "Category" && item->getCategory().toLower().contains(valLower)) {
            results.push_back(const_cast<Item*>(item));
        }
    }
    return results;
}
