#ifndef USER_H
#define USER_H

#include <QString>

enum class UserRole { Admin, Regular };

class User {
protected:
    QString username;
    QString hashedPassword;
    QString salt;
    UserRole role;

public:
    User(const QString& uname, const QString& hpass, const QString& s, UserRole r)
        : username(uname), hashedPassword(hpass), salt(s), role(r) {}
    virtual ~User() = default;

    QString getUsername() const { return username; }
    QString getHashedPassword() const { return hashedPassword; }
    QString getSalt() const { return salt; }
    UserRole getRole() const { return role; }
    virtual QString getRoleName() const = 0;
};

class Admin : public User {
public:
    Admin(const QString& uname, const QString& hpass, const QString& s)
        : User(uname, hpass, s, UserRole::Admin) {}
    QString getRoleName() const override { return "Admin"; }
};

class RegularUser : public User {
public:
    RegularUser(const QString& uname, const QString& hpass, const QString& s)
        : User(uname, hpass, s, UserRole::Regular) {}
    QString getRoleName() const override { return "Regular User"; }
};

#endif // USER_H
