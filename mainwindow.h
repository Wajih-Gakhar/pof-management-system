#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "user.h"
#include "employeemanager.h" // Adjust if filename differs
#include "inventorymanager.h"
#include "productionlogmanager.h"
#include "maintenancemanager.h"
#include <QComboBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(User* user, const QString& system, QWidget* parent = nullptr);
    ~MainWindow();

private:
    User* currentUser;
    EmployeeManager employeeManager;
    InventoryManager inventoryManager;
    ProductionLogManager productionLogManager;
    MaintenanceManager maintenanceManager;
    QTabWidget* tabs;
    QComboBox* deptCombo;
    QComboBox* desigCombo;
    QComboBox* categoryCombo;
    QComboBox* statusCombo;

    void setupEmployeeTab();
    void setupInventoryTab();
    void setupProductionLogsTab();
    void setupMaintenanceSchedulingTab();

private slots:
    void handleLogout();
};

#endif // MAINWINDOW_H
