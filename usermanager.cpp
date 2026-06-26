#include "usermanager.h"
#include <QFile>
#include <QTextStream>
#include <QUuid>

UserManager::UserManager() {
    loadUsers();
}

UserManager::~UserManager() {
    for (User* user : users) {
        delete user;
    }
}

void UserManager::loadUsers() {
    QFile file(userFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 4) {
            QString uname = parts[0];
            QString hpass = parts[1];
            QString salt = parts[2];
            UserRole role = parts[3] == "Admin" ? UserRole::Admin : UserRole::Regular;
            if (role == UserRole::Admin) {
                users.push_back(new Admin(uname, hpass, salt));
            } else {
                users.push_back(new RegularUser(uname, hpass, salt));
            }
        }
    }
    file.close();
}

void UserManager::saveUsers() {
    QFile file(userFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (const User* user : users) {
        out << user->getUsername() << "," << user->getHashedPassword() << ","
            << user->getSalt() << "," << user->getRoleName() << "\n";
    }
    file.close();
}

User* UserManager::authenticate(const QString& uname, const QString& pass) {
    for (User* user : users) {
        if (user->getUsername() == uname) {
            QString hashedPass = hashPassword(pass, user->getSalt());
            if (user->getHashedPassword() == hashedPass) {
                return user;
            }
        }
    }
    return nullptr;
}

bool UserManager::addUser(const QString& uname, const QString& pass, UserRole role) {
    for (const User* user : users) {
        if (user->getUsername() == uname) {
            return false;
        }
    }
    QString salt = QUuid::createUuid().toString();
    QString hashedPass = hashPassword(pass, salt);
    if (role == UserRole::Admin) {
        users.push_back(new Admin(uname, hashedPass, salt));
    } else {
        users.push_back(new RegularUser(uname, hashedPass, salt));
    }
    saveUsers();
    return true;
}
