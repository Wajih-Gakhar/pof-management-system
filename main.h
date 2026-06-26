#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include <QUuid>

QString hashPassword(const QString& password, const QString& salt, int iterations = 1000);

#endif // MAIN_H
