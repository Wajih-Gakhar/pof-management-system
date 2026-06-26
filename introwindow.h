#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include "usermanager.h"

class IntroWindow : public QMainWindow {
    Q_OBJECT

private:
    User* currentUser;
    UserRole initialRole; // Store initial user's role
    UserManager userManager;
    QPushButton* employeeButton;
    QPushButton* inventoryButton;
    QPushButton* productionButton;
    QPushButton* maintenanceButton;
    QPushButton* logoutButton;
    QLabel* infoLabel;

public:
    IntroWindow(User* user, QWidget* parent = nullptr);
    ~IntroWindow();

private slots:
    void handleLogout();
    void showLoginForSystem(const QString& system);
};

#endif // INTROWINDOW_H
