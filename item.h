// item.h
#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item {
private:
    QString id; // Unique ID
    QString name;
    QString category; // Weapon, Equipment, Spare Part, Raw Material
    int quantity;
    QString status; // Available, In Use, Damaged

public:
    Item(const QString& id, const QString& name, const QString& category, int quantity, const QString& status);
    virtual ~Item();

    // Getters
    QString getId() const;
    QString getName() const;
    QString getCategory() const;
    int getQuantity() const;
    QString getStatus() const;

    // Setters
    void setName(const QString& name);
    void setCategory(const QString& category);
    void setQuantity(int quantity);
    void setStatus(const QString& status);
};

#endif // ITEM_H
