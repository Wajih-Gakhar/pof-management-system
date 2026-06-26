// item.cpp
#include "item.h"

Item::Item(const QString& id, const QString& name, const QString& category, int quantity, const QString& status)
    : id(id), name(name), category(category), quantity(quantity), status(status) {
}

Item::~Item() = default;

// Getters
QString Item::getId() const {
    return id;
}

QString Item::getName() const {
    return name;
}

QString Item::getCategory() const {
    return category;
}

int Item::getQuantity() const {
    return quantity;
}

QString Item::getStatus() const {
    return status;
}

// Setters
void Item::setName(const QString& name) {
    this->name = name;
}

void Item::setCategory(const QString& category) {
    this->category = category;
}

void Item::setQuantity(int quantity) {
    this->quantity = quantity;
}

void Item::setStatus(const QString& status) {
    this->status = status;
}
