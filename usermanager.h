#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include "main.h" // Include for hashPassword
#include <QVector>
#include <QString>

class UserManager {
private:
    QVector<User*> users;
    const QString userFile = "users.txt";

    void loadUsers();
    void saveUsers();

public:
    UserManager();
    ~UserManager();

    User* authenticate(const QString& uname, const QString& pass);
    bool addUser(const QString& uname, const QString& pass, UserRole role);
};

#endif // USERMANAGER_H
