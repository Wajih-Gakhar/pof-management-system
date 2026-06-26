#include "introwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include "loginwindow.h"
#include "mainwindow.h"

IntroWindow::IntroWindow(User* user, QWidget* parent) : QMainWindow(parent), currentUser(user), initialRole(user->getRole()) {
    setWindowTitle("POF Management System - Welcome");
    resize(600, 400);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    infoLabel = new QLabel(
        "<h2>Welcome to Pakistan Ordnance Factories Management System</h2>"
        "<p>Pakistan Ordnance Factories (POF) is the premier defense manufacturing complex in Pakistan, "
        "producing a wide range of conventional arms and ammunition to international standards. "
        "This system facilitates efficient management of key operations, including:</p>"
        "<ul>"
        "<li>Employee Management: Manage workforce details, roles, and departments.</li>"
        "<li>Inventory Management: Track weapons, equipment, spare parts, and raw materials.</li>"
        "<li>Production Logs: Monitor production activities and quantities.</li>"
        "<li>Maintenance Scheduling: Schedule and track equipment maintenance.</li>"
        "</ul>"
        "<p>Select a system below to proceed:</p>", this);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    employeeButton = new QPushButton("Employee Management", this);
    inventoryButton = new QPushButton("Inventory Management", this);
    productionButton = new QPushButton("Production Logs", this);
    maintenanceButton = new QPushButton("Maintenance Scheduling", this);
    logoutButton = new QPushButton("Logout", this);

    QHBoxLayout* buttonLayout1 = new QHBoxLayout();
    buttonLayout1->addWidget(employeeButton);
    buttonLayout1->addWidget(inventoryButton);
    layout->addLayout(buttonLayout1);

    QHBoxLayout* buttonLayout2 = new QHBoxLayout();
    buttonLayout2->addWidget(productionButton);
    buttonLayout2->addWidget(maintenanceButton);
    layout->addLayout(buttonLayout2);

    layout->addWidget(logoutButton);
    layout->addStretch();

    // Connect system buttons to login window with specific system
    connect(employeeButton, &QPushButton::clicked, this, [=]() { showLoginForSystem("Employee"); });
    connect(inventoryButton, &QPushButton::clicked, this, [=]() { showLoginForSystem("Inventory"); });
    connect(productionButton, &QPushButton::clicked, this, [=]() { showLoginForSystem("Production"); });
    connect(maintenanceButton, &QPushButton::clicked, this, [=]() { showLoginForSystem("Maintenance"); });
    connect(logoutButton, &QPushButton::clicked, this, &IntroWindow::handleLogout);
}

IntroWindow::~IntroWindow() {
    // Empty destructor
}

void IntroWindow::showLoginForSystem(const QString& system) {
    LoginWindow* loginWindow = new LoginWindow(this);
    loginWindow->setProperty("targetSystem", system);
    loginWindow->setProperty("expectedRole", initialRole == UserRole::Admin ? "Admin" : "Regular"); // Pass expected role
    // Connect loginSuccess signal for this LoginWindow instance
    connect(loginWindow, &LoginWindow::loginSuccess, this, [=](User* user, const QString& system) {
        MainWindow* mainWindow = new MainWindow(user, system, this); // Pass system to MainWindow
        mainWindow->show();
        this->hide();
    });
    loginWindow->show();
    this->hide();
}

void IntroWindow::handleLogout() {
    LoginWindow* loginWindow = new LoginWindow();
    loginWindow->show();
    this->close();
}
