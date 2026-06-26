// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>
#include <QDateEdit>
#include "loginwindow.h"
#include "introwindow.h"
#include <QDateTime>
#include <QDate>
#include <QHeaderView>
#include <QString>

MainWindow::MainWindow(User* user, const QString& system, QWidget* parent) : QMainWindow(parent) {
    currentUser = user;
    setWindowTitle("POF Management System - " + user->getRoleName());
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    tabs = new QTabWidget(this);
    layout->addWidget(tabs);

    // Initialize only the selected system's tab
    if (system == "Employee") {
        setupEmployeeTab();
    } else if (system == "Inventory") {
        setupInventoryTab();
    } else if (system == "Production") {
        setupProductionLogsTab();
    } else if (system == "Maintenance") {
        setupMaintenanceSchedulingTab();
    }
}

MainWindow::~MainWindow() {
    // Empty destructor
}

void MainWindow::setupEmployeeTab() {
    QWidget* employeeTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(employeeTab);

    // Welcome label
    QLabel* welcomeLabel = new QLabel("Welcome, " + currentUser->getUsername() + " (" + currentUser->getRoleName() + ")", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(welcomeLabel);

    // Table to display employees
    QTableWidget* table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Department", "Designation"});
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    // Search controls (visible for all users)
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QComboBox* searchCriterion = new QComboBox(this);
    searchCriterion->addItems({"ID", "Name", "Department", "Designation"});
    QLineEdit* searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Enter search term");
    QPushButton* searchButton = new QPushButton("Search", this);
    QPushButton* clearSearchButton = new QPushButton("Clear Search", this);
    searchLayout->addWidget(new QLabel("Search by:", this));
    searchLayout->addWidget(searchCriterion);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearSearchButton);
    layout->addLayout(searchLayout);

    // Input fields and buttons (visible only for Admins)
    QLineEdit* idField = new QLineEdit(this);
    idField->setPlaceholderText("Employee ID");
    QLabel* idLabel = new QLabel("ID", this);
    QLineEdit* nameField = new QLineEdit(this);
    nameField->setPlaceholderText("Name");
    QLabel* nameLabel = new QLabel("Name", this);
    deptCombo = new QComboBox(this);
    deptCombo->addItems({
        "Weapons Factory",
        "Ammunition Factory",
        "Explosives Factory",
        "Filling Factory",
        "Propellants Factory",
        "Small Arms Ammunition Factory",
        "Medium Artillery Ammunition Factory",
        "Heavy Artillery Ammunition Factory",
        "Tank Ammunition Factory",
        "Machine Gun Factory",
        "Research and Development (R&D)",
        "Quality Control",
        "Procurement",
        "Human Resources",
        "Finance",
        "IT and Systems",
        "Security",
        "Training and Development",
        "Maintenance and Engineering",
        "Logistics and Supply Chain"
    });
    deptCombo->setCurrentIndex(-1);
    QLabel* deptLabel = new QLabel("Department", this);
    desigCombo = new QComboBox(this);
    desigCombo->addItems({
        "General Manager",
        "Deputy General Manager",
        "Assistant General Manager",
        "Director (R&D)",
        "Senior Research Officer",
        "Research Officer",
        "Production Manager",
        "Assistant Production Manager",
        "Quality Control Inspector",
        "Senior Engineer",
        "Engineer",
        "Foreman",
        "Assistant Foreman",
        "Skilled Workman",
        "Technician",
        "Administrative Officer",
        "Accounts Officer",
        "Procurement Officer",
        "HR Officer",
        "IT Specialist",
        "Security Officer",
        "Training Coordinator",
        "Logistics Officer"
    });
    desigCombo->setCurrentIndex(-1);
    QLabel* desigLabel = new QLabel("Designation", this);
    QPushButton* addButton = new QPushButton("Add Employee", this);
    QPushButton* updateButton = new QPushButton("Update Employee", this);
    QPushButton* deleteButton = new QPushButton("Delete Employee", this);
    QPushButton* logoutButton = new QPushButton("Logout", this);

    // Role-based access
    bool isAdmin = currentUser->getRole() == UserRole::Admin;
    idLabel->setVisible(isAdmin);
    idField->setVisible(isAdmin);
    nameLabel->setVisible(isAdmin);
    nameField->setVisible(isAdmin);
    deptLabel->setVisible(isAdmin);
    deptCombo->setVisible(isAdmin);
    desigLabel->setVisible(isAdmin);
    desigCombo->setVisible(isAdmin);
    addButton->setVisible(isAdmin);
    updateButton->setVisible(isAdmin);
    deleteButton->setVisible(isAdmin);
    logoutButton->setVisible(true);

    layout->addWidget(idLabel);
    layout->addWidget(idField);
    layout->addWidget(nameLabel);
    layout->addWidget(nameField);
    layout->addWidget(deptLabel);
    layout->addWidget(deptCombo);
    layout->addWidget(desigLabel);
    layout->addWidget(desigCombo);
    layout->addWidget(addButton);
    layout->addWidget(updateButton);
    layout->addWidget(deleteButton);
    layout->addWidget(logoutButton);
    layout->addStretch();

    // Function to refresh employee table
    auto refreshEmployeeTable = [=](const QVector<Employee*>& employeesToShow) {
        table->setRowCount(0);
        table->setRowCount(employeesToShow.size());
        for (int i = 0; i < employeesToShow.size(); ++i) {
            QTableWidgetItem* idItem = new QTableWidgetItem(employeesToShow[i]->getId());
            QTableWidgetItem* nameItem = new QTableWidgetItem(employeesToShow[i]->getName());
            QTableWidgetItem* deptItem = new QTableWidgetItem(employeesToShow[i]->getDepartment());
            QTableWidgetItem* desigItem = new QTableWidgetItem(employeesToShow[i]->getDesignation());
            if (!idItem->text().isEmpty()) table->setItem(i, 0, idItem);
            if (!nameItem->text().isEmpty()) table->setItem(i, 1, nameItem);
            if (!deptItem->text().isEmpty()) table->setItem(i, 2, deptItem);
            if (!desigItem->text().isEmpty()) table->setItem(i, 3, desigItem);
        }
        table->resizeColumnsToContents();
    };

    // Initial employee table population
    refreshEmployeeTable(employeeManager.getEmployees());

    // Connect search button (accessible to all users)
    connect(searchButton, &QPushButton::clicked, this, [=]() {
        QString criterion = searchCriterion->currentText();
        QString value = searchField->text();
        if (value.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a search term.");
            return;
        }
        QVector<Employee*> results = employeeManager.searchEmployees(criterion, value);
        refreshEmployeeTable(results);
    });

    // Connect clear search button (accessible to all users)
    connect(clearSearchButton, &QPushButton::clicked, this, [=]() {
        searchField->clear();
        refreshEmployeeTable(employeeManager.getEmployees());
    });

    // Connect table selection to populate fields (for Admins only)
    connect(table, &QTableWidget::itemSelectionChanged, this, [=]() {
        if (!isAdmin) return;
        if (table->selectedItems().isEmpty()) return;
        int row = table->currentRow();
        idField->setText(table->item(row, 0) ? table->item(row, 0)->text() : "");
        nameField->setText(table->item(row, 1) ? table->item(row, 1)->text() : "");
        deptCombo->setCurrentText(table->item(row, 2) ? table->item(row, 2)->text() : "");
        desigCombo->setCurrentText(table->item(row, 3) ? table->item(row, 3)->text() : "");
    });

    // Connect buttons (Admin-only)
    if (isAdmin) {
        connect(addButton, &QPushButton::clicked, this, [=]() {
            QString id = idField->text();
            QString name = nameField->text();
            QString dept = deptCombo->currentText();
            QString desig = desigCombo->currentText();
            if (id.isEmpty() || name.isEmpty() || dept.isEmpty() || desig.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required.");
                return;
            }
            if (employeeManager.addEmployee(id, name, dept, desig)) {
                QMessageBox::information(this, "Success", "Employee added.");
                refreshEmployeeTable(employeeManager.getEmployees());
                idField->clear();
                nameField->clear();
                deptCombo->setCurrentIndex(-1);
                desigCombo->setCurrentIndex(-1);
            } else {
                QMessageBox::warning(this, "Error", "Employee ID already exists.");
            }
        });

        connect(updateButton, &QPushButton::clicked, this, [=]() {
            QString id = idField->text();
            QString name = nameField->text();
            QString dept = deptCombo->currentText();
            QString desig = desigCombo->currentText();
            if (id.isEmpty() || name.isEmpty() || dept.isEmpty() || desig.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required.");
                return;
            }
            if (employeeManager.updateEmployee(id, name, dept, desig)) {
                QMessageBox::information(this, "Success", "Employee updated.");
                refreshEmployeeTable(employeeManager.getEmployees());
            } else {
                QMessageBox::warning(this, "Error", "Employee ID not found.");
            }
        });

        connect(deleteButton, &QPushButton::clicked, this, [=]() {
            QString id = idField->text();
            if (id.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please enter an Employee ID.");
                return;
            }
            if (employeeManager.deleteEmployee(id)) {
                QMessageBox::information(this, "Success", "Employee deleted.");
                refreshEmployeeTable(employeeManager.getEmployees());
                idField->clear();
                nameField->clear();
                deptCombo->setCurrentIndex(-1);
                desigCombo->setCurrentIndex(-1);
            } else {
                QMessageBox::warning(this, "Error", "Employee ID not found.");
            }
        });
    }

    // Connect logout button (accessible to all users)
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);

    tabs->addTab(employeeTab, "Employees");
}

void MainWindow::setupInventoryTab() {
    QWidget* inventoryTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(inventoryTab);

    // Table to display inventory items
    QTableWidget* table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Name", "Category", "Quantity", "Status"});
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    // Search controls (visible for all users)
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QComboBox* searchCriterion = new QComboBox(this);
    searchCriterion->addItems({"Name", "Category"});
    QLineEdit* searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Enter search term");
    QPushButton* searchButton = new QPushButton("Search", this);
    QPushButton* clearSearchButton = new QPushButton("Clear Search", this);
    searchLayout->addWidget(new QLabel("Search by:", this));
    searchLayout->addWidget(searchCriterion);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearSearchButton);
    layout->addLayout(searchLayout);

    // Input fields and buttons (visible only for Admins)
    QLineEdit* idField = new QLineEdit(this);
    idField->setPlaceholderText("Item ID");
    QLabel* idLabel = new QLabel("ID", this);
    QLineEdit* nameField = new QLineEdit(this);
    nameField->setPlaceholderText("Item Name");
    QLabel* nameLabel = new QLabel("Name", this);
    categoryCombo = new QComboBox(this);
    categoryCombo->addItems({"Weapon", "Equipment", "Spare Part", "Raw Material"});
    categoryCombo->setCurrentIndex(-1);
    QLabel* categoryLabel = new QLabel("Category", this);
    QSpinBox* quantityField = new QSpinBox(this);
    quantityField->setRange(0, 1000000);
    quantityField->setValue(0);
    QLabel* quantityLabel = new QLabel("Quantity", this);
    statusCombo = new QComboBox(this);
    statusCombo->addItems({"Available", "In Use", "Damaged"});
    statusCombo->setCurrentIndex(-1);
    QLabel* statusLabel = new QLabel("Status", this);
    QPushButton* addButton = new QPushButton("Add Item", this);
    QPushButton* updateButton = new QPushButton("Update Item", this);
    QPushButton* deleteButton = new QPushButton("Delete Item", this);
    QPushButton* logoutButton = new QPushButton("Logout", this);

    // Role-based access
    bool isAdmin = currentUser->getRole() == UserRole::Admin;
    idLabel->setVisible(isAdmin);
    idField->setVisible(isAdmin);
    nameLabel->setVisible(isAdmin);
    nameField->setVisible(isAdmin);
    categoryLabel->setVisible(isAdmin);
    categoryCombo->setVisible(isAdmin);
    quantityLabel->setVisible(isAdmin);
    quantityField->setVisible(isAdmin);
    statusLabel->setVisible(isAdmin);
    statusCombo->setVisible(isAdmin);
    addButton->setVisible(isAdmin);
    updateButton->setVisible(isAdmin);
    deleteButton->setVisible(isAdmin);
    logoutButton->setVisible(true);

    // Add labels and fields to layout
    layout->addWidget(idLabel);
    layout->addWidget(idField);
    layout->addWidget(nameLabel);
    layout->addWidget(nameField);
    layout->addWidget(categoryLabel);
    layout->addWidget(categoryCombo);
    layout->addWidget(quantityLabel);
    layout->addWidget(quantityField);
    layout->addWidget(statusLabel);
    layout->addWidget(statusCombo);
    layout->addWidget(addButton);
    layout->addWidget(updateButton);
    layout->addWidget(deleteButton);
    layout->addWidget(logoutButton);
    layout->addStretch();

    // Function to refresh inventory table
    auto refreshInventoryTable = [=](const QVector<Item*>& itemsToShow) {
        table->setRowCount(0);
        table->setRowCount(itemsToShow.size());
        for (int i = 0; i < itemsToShow.size(); ++i) {
            QTableWidgetItem* idItem = new QTableWidgetItem(itemsToShow[i]->getId());
            QTableWidgetItem* nameItem = new QTableWidgetItem(itemsToShow[i]->getName());
            QTableWidgetItem* categoryItem = new QTableWidgetItem(itemsToShow[i]->getCategory());
            QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(itemsToShow[i]->getQuantity()));
            QTableWidgetItem* statusItem = new QTableWidgetItem(itemsToShow[i]->getStatus());
            if (!idItem->text().isEmpty()) table->setItem(i, 0, idItem);
            if (!nameItem->text().isEmpty()) table->setItem(i, 1, nameItem);
            if (!categoryItem->text().isEmpty()) table->setItem(i, 2, categoryItem);
            if (quantityItem->text().toInt() >= 0) table->setItem(i, 3, quantityItem);
            if (!statusItem->text().isEmpty()) table->setItem(i, 4, statusItem);
        }
        table->resizeColumnsToContents();
    };

    // Function to refresh production logs table
    auto refreshProductionLogsTable = [=](const QVector<ProductionLog*>& logsToShow) {
        QTableWidget* productionTable = tabs->findChild<QTableWidget*>("productionLogsTable");
        if (productionTable) {
            productionTable->setRowCount(0);
            productionTable->setRowCount(logsToShow.size());
            for (int i = 0; i < logsToShow.size(); ++i) {
                QTableWidgetItem* dateItem = new QTableWidgetItem(logsToShow[i]->getDate());
                QTableWidgetItem* itemNameItem = new QTableWidgetItem(logsToShow[i]->getItemName());
                QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(logsToShow[i]->getQuantity()));
                if (!dateItem->text().isEmpty()) productionTable->setItem(i, 0, dateItem);
                if (!itemNameItem->text().isEmpty()) productionTable->setItem(i, 1, itemNameItem);
                if (quantityItem->text().toInt() >= 0) productionTable->setItem(i, 2, quantityItem);
            }
            productionTable->resizeColumnsToContents();
        }
    };

    // Function to update item names in Production Logs tab's QComboBox
    auto updateProductionItemNames = [=]() {
        QComboBox* itemNameCombo = tabs->findChild<QComboBox*>("productionItemNameCombo");
        if (itemNameCombo) {
            itemNameCombo->clear();
            QVector<Item*> items = inventoryManager.getItems();
            for (const Item* item : items) {
                itemNameCombo->addItem(item->getName());
            }
            itemNameCombo->setCurrentIndex(-1);
        }
    };

    // Function to update equipment IDs and names in Maintenance Scheduling tab
    auto updateMaintenanceEquipment = [=]() {
        QComboBox* equipmentIdCombo = tabs->findChild<QComboBox*>("maintenanceEquipmentIdCombo");
        QComboBox* equipmentNameCombo = tabs->findChild<QComboBox*>("maintenanceEquipmentNameCombo");
        if (equipmentIdCombo && equipmentNameCombo) {
            equipmentIdCombo->clear();
            equipmentNameCombo->clear();
            QVector<Item*> items = inventoryManager.getItems();
            for (const Item* item : items) {
                equipmentIdCombo->addItem(item->getId());
            }
            equipmentIdCombo->setCurrentIndex(-1);
            equipmentNameCombo->setEnabled(false); // Will be populated based on selected ID
        }
    };

    // Initial table population
    refreshInventoryTable(inventoryManager.getItems());
    updateProductionItemNames();
    updateMaintenanceEquipment();

    // Connect search button (accessible to all users)
    connect(searchButton, &QPushButton::clicked, this, [=]() {
        QString criterion = searchCriterion->currentText();
        QString value = searchField->text();
        if (value.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a search term.");
            return;
        }
        QVector<Item*> results = inventoryManager.searchItems(criterion, value);
        refreshInventoryTable(results);
    });

    // Connect clear search button (accessible to all users)
    connect(clearSearchButton, &QPushButton::clicked, this, [=]() {
        searchField->clear();
        refreshInventoryTable(inventoryManager.getItems());
    });

    // Connect table selection to populate fields (for Admins only)
    connect(table, &QTableWidget::itemSelectionChanged, this, [=]() {
        if (!isAdmin) return;
        if (table->selectedItems().isEmpty()) return;
        int row = table->currentRow();
        idField->setText(table->item(row, 0) ? table->item(row, 0)->text() : "");
        nameField->setText(table->item(row, 1) ? table->item(row, 1)->text() : "");
        categoryCombo->setCurrentText(table->item(row, 2) ? table->item(row, 2)->text() : "");
        quantityField->setValue(table->item(row, 3) ? table->item(row, 3)->text().toInt() : 0);
        statusCombo->setCurrentText(table->item(row, 4) ? table->item(row, 4)->text() : "");
    });

    // Connect buttons (Admin-only)
    if (isAdmin) {
        connect(addButton, &QPushButton::clicked, this, [=]() {
            QString id = idField->text();
            QString name = nameField->text();
            QString category = categoryCombo->currentText();
            int quantity = quantityField->value();
            QString status = statusCombo->currentText();
            if (id.isEmpty() || name.isEmpty() || category.isEmpty() || status.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required.");
                return;
            }
            if (quantity < 0) {
                QMessageBox::warning(this, "Error", "Quantity cannot be negative.");
                return;
            }
            if (inventoryManager.addItem(id, name, category, quantity, status)) {
                QMessageBox::information(this, "Success", "Item added.");
                refreshInventoryTable(inventoryManager.getItems());

                // Add production log for the new item
                QString currentDate = QDateTime::currentDateTime().toString("dd-MM-yyyy");
                productionLogManager.addLog(currentDate, name, quantity);
                refreshProductionLogsTable(productionLogManager.getLogs());

                // Update item names in Production Logs tab
                updateProductionItemNames();

                // Update equipment IDs and names in Maintenance Scheduling tab
                updateMaintenanceEquipment();

                idField->clear();
                nameField->clear();
                categoryCombo->setCurrentIndex(-1);
                quantityField->setValue(0);
                statusCombo->setCurrentIndex(-1);
            } else {
                QMessageBox::warning(this, "Error", "Item ID already exists.");
            }
        });

        connect(updateButton, &QPushButton::clicked, this, [=]() {
            QString id = idField->text();
            QString name = nameField->text();
            QString category = categoryCombo->currentText();
            int quantity = quantityField->value();
            QString status = statusCombo->currentText();
            if (id.isEmpty() || name.isEmpty() || category.isEmpty() || status.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required.");
                return;
            }
            if (quantity < 0) {
                QMessageBox::warning(this, "Error", "Quantity cannot be negative.");
                return;
            }
            if (inventoryManager.updateItem(id, name, category, quantity, status)) {
                QMessageBox::information(this, "Success", "Item updated.");
                refreshInventoryTable(inventoryManager.getItems());
                updateProductionItemNames();
                updateMaintenanceEquipment();
            } else {
                QMessageBox::warning(this, "Error", "Item ID not found.");
            }
        });

        connect(deleteButton, &QPushButton::clicked, this, [=]() {
            QString id = idField->text();
            if (id.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please enter an Item ID.");
                return;
            }
            if (inventoryManager.deleteItem(id)) {
                QMessageBox::information(this, "Success", "Item deleted.");
                refreshInventoryTable(inventoryManager.getItems());
                updateProductionItemNames();
                updateMaintenanceEquipment();
                idField->clear();
                nameField->clear();
                categoryCombo->setCurrentIndex(-1);
                quantityField->setValue(0);
                statusCombo->setCurrentIndex(-1);
            } else {
                QMessageBox::warning(this, "Error", "Item ID not found.");
            }
        });
    }

    // Connect logout button (accessible to all users)
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);

    tabs->addTab(inventoryTab, "Inventory");
}

void MainWindow::setupProductionLogsTab() {
    QWidget* productionLogsTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(productionLogsTab);

    // Table to display production logs
    QTableWidget* table = new QTableWidget(this);
    table->setObjectName("productionLogsTable");
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Date", "Item Name", "Quantity"});
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    // Search controls (visible for all users)
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QComboBox* searchCriterion = new QComboBox(this);
    searchCriterion->addItems({"Item Name", "Date"});
    QLineEdit* searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Enter search term");
    QPushButton* searchButton = new QPushButton("Search", this);
    QPushButton* clearSearchButton = new QPushButton("Clear Search", this);
    searchLayout->addWidget(new QLabel("Search by:", this));
    searchLayout->addWidget(searchCriterion);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearSearchButton);
    layout->addLayout(searchLayout);

    // Input fields and buttons (visible only for Admins)
    QDateEdit* dateField = new QDateEdit(this);
    dateField->setDisplayFormat("dd-MM-yyyy");
    dateField->setCalendarPopup(true);
    dateField->setDate(QDate::currentDate());
    QLabel* dateLabel = new QLabel("Date", this);
    QComboBox* itemNameCombo = new QComboBox(this);
    itemNameCombo->setObjectName("productionItemNameCombo");
    // Populate with item names from inventory
    QVector<Item*> items = inventoryManager.getItems();
    for (const Item* item : items) {
        itemNameCombo->addItem(item->getName());
    }
    itemNameCombo->setCurrentIndex(-1);
    QLabel* itemNameLabel = new QLabel("Item Name", this);
    QSpinBox* quantityField = new QSpinBox(this);
    quantityField->setRange(0, 1000000);
    quantityField->setValue(0);
    QLabel* quantityLabel = new QLabel("Quantity", this);
    QPushButton* addButton = new QPushButton("Add Log", this);
    QPushButton* updateButton = new QPushButton("Update Log", this);
    QPushButton* deleteButton = new QPushButton("Delete Log", this);
    QPushButton* logoutButton = new QPushButton("Logout", this);

    // Role-based access
    bool isAdmin = currentUser->getRole() == UserRole::Admin;
    dateLabel->setVisible(isAdmin);
    dateField->setVisible(isAdmin);
    itemNameLabel->setVisible(isAdmin);
    itemNameCombo->setVisible(isAdmin);
    quantityLabel->setVisible(isAdmin);
    quantityField->setVisible(isAdmin);
    addButton->setVisible(isAdmin);
    updateButton->setVisible(isAdmin);
    deleteButton->setVisible(isAdmin);
    logoutButton->setVisible(true);

    // Add labels and fields to layout
    layout->addWidget(dateLabel);
    layout->addWidget(dateField);
    layout->addWidget(itemNameLabel);
    layout->addWidget(itemNameCombo);
    layout->addWidget(quantityLabel);
    layout->addWidget(quantityField);
    layout->addWidget(addButton);
    layout->addWidget(updateButton);
    layout->addWidget(deleteButton);
    layout->addWidget(logoutButton);
    layout->addStretch();

    // Function to refresh production logs table
    auto refreshProductionLogsTable = [=](const QVector<ProductionLog*>& logsToShow) {
        table->setRowCount(0);
        table->setRowCount(logsToShow.size());
        for (int i = 0; i < logsToShow.size(); ++i) {
            QTableWidgetItem* dateItem = new QTableWidgetItem(logsToShow[i]->getDate());
            QTableWidgetItem* itemNameItem = new QTableWidgetItem(logsToShow[i]->getItemName());
            QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(logsToShow[i]->getQuantity()));
            if (!dateItem->text().isEmpty()) table->setItem(i, 0, dateItem);
            if (!itemNameItem->text().isEmpty()) table->setItem(i, 1, itemNameItem);
            if (quantityItem->text().toInt() >= 0) table->setItem(i, 2, quantityItem);
        }
        table->resizeColumnsToContents();
    };

    // Initial production logs table population
    refreshProductionLogsTable(productionLogManager.getLogs());

    // Connect search button (accessible to all users)
    connect(searchButton, &QPushButton::clicked, this, [=]() {
        QString criterion = searchCriterion->currentText();
        QString value = searchField->text();
        if (value.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a search term.");
            return;
        }
        QVector<ProductionLog*> results = productionLogManager.searchLogs(criterion, value);
        refreshProductionLogsTable(results);
    });

    // Connect clear search button (accessible to all users)
    connect(clearSearchButton, &QPushButton::clicked, this, [=]() {
        searchField->clear();
        refreshProductionLogsTable(productionLogManager.getLogs());
    });

    // Connect table selection to populate fields (for Admins only)
    connect(table, &QTableWidget::itemSelectionChanged, this, [=]() {
        if (!isAdmin) return;
        if (table->selectedItems().isEmpty()) return;
        int row = table->currentRow();
        dateField->setDate(QDate::fromString(table->item(row, 0)->text(), "dd-MM-yyyy"));
        itemNameCombo->setCurrentText(table->item(row, 1)->text());
        quantityField->setValue(table->item(row, 2)->text().toInt());
    });

    // Connect buttons (Admin-only)
    if (isAdmin) {
        connect(addButton, &QPushButton::clicked, this, [=]() {
            QString date = dateField->date().toString("dd-MM-yyyy");
            QString itemName = itemNameCombo->currentText();
            int quantity = quantityField->value();
            if (itemName.isEmpty() || quantity < 0) {
                QMessageBox::warning(this, "Error", "Please select an Item Name and ensure Quantity is not negative.");
                return;
            }
            if (productionLogManager.addLog(date, itemName, quantity)) {
                QMessageBox::information(this, "Success", "Production log added.");
                refreshProductionLogsTable(productionLogManager.getLogs());
                itemNameCombo->setCurrentIndex(-1);
                quantityField->setValue(0);
            }
        });

        connect(updateButton, &QPushButton::clicked, this, [=]() {
            if (table->selectedItems().isEmpty()) {
                QMessageBox::warning(this, "Error", "Please select a log to update.");
                return;
            }
            int row = table->currentRow();
            QString oldDate = table->item(row, 0)->text();
            QString oldItemName = table->item(row, 1)->text();
            QString newDate = dateField->date().toString("dd-MM-yyyy");
            QString newItemName = itemNameCombo->currentText();
            int newQuantity = quantityField->value();
            if (newItemName.isEmpty() || newQuantity < 0) {
                QMessageBox::warning(this, "Error", "Please select an Item Name and ensure Quantity is not negative.");
                return;
            }
            if (productionLogManager.updateLog(oldDate, oldItemName, newDate, newItemName, newQuantity)) {
                QMessageBox::information(this, "Success", "Production log updated.");
                refreshProductionLogsTable(productionLogManager.getLogs());
                itemNameCombo->setCurrentIndex(-1);
                quantityField->setValue(0);
            } else {
                QMessageBox::warning(this, "Error", "Log not found or update failed.");
            }
        });

        connect(deleteButton, &QPushButton::clicked, this, [=]() {
            if (table->selectedItems().isEmpty()) {
                QMessageBox::warning(this, "Error", "Please select a log to delete.");
                return;
            }
            int row = table->currentRow();
            QString date = table->item(row, 0)->text();
            QString itemName = table->item(row, 1)->text();
            if (productionLogManager.deleteLog(date, itemName)) {
                QMessageBox::information(this, "Success", "Production log deleted.");
                refreshProductionLogsTable(productionLogManager.getLogs());
                itemNameCombo->setCurrentIndex(-1);
                quantityField->setValue(0);
            } else {
                QMessageBox::warning(this, "Error", "Log not found or deletion failed.");
            }
        });
    }

    // Connect logout button (accessible to all users)
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);

    tabs->addTab(productionLogsTab, "Production Logs");
}

void MainWindow::setupMaintenanceSchedulingTab() {
    QWidget* maintenanceTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(maintenanceTab);

    // Table to display maintenance records
    QTableWidget* table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Equipment ID", "Equipment Name", "Maintenance Date", "Next Maintenance Date", "Action", "Status"});
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    // Search controls (visible for all users)
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLineEdit* searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Enter Equipment ID");
    QPushButton* searchButton = new QPushButton("Search", this);
    QPushButton* clearSearchButton = new QPushButton("Clear Search", this);
    searchLayout->addWidget(new QLabel("Search by Equipment ID:", this));
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearSearchButton);
    layout->addLayout(searchLayout);

    // Input fields and buttons (visible only for Admins)
    QComboBox* equipmentIdCombo = new QComboBox(this);
    equipmentIdCombo->setObjectName("maintenanceEquipmentIdCombo");
    // Populate with item IDs from inventory
    QVector<Item*> items = inventoryManager.getItems();
    for (const Item* item : items) {
        equipmentIdCombo->addItem(item->getId());
    }
    equipmentIdCombo->setCurrentIndex(-1);
    QLabel* equipmentIdLabel = new QLabel("Equipment ID", this);
    QComboBox* equipmentNameCombo = new QComboBox(this);
    equipmentNameCombo->setObjectName("maintenanceEquipmentNameCombo");
    equipmentNameCombo->setEnabled(false); // Will be populated based on selected ID
    QLabel* equipmentNameLabel = new QLabel("Equipment Name", this);
    QDateEdit* maintenanceDateField = new QDateEdit(this);
    maintenanceDateField->setDisplayFormat("dd-MM-yyyy");
    maintenanceDateField->setCalendarPopup(true);
    maintenanceDateField->setDate(QDate::currentDate());
    QLabel* maintenanceDateLabel = new QLabel("Maintenance Date", this);
    QDateEdit* nextMaintenanceDateField = new QDateEdit(this);
    nextMaintenanceDateField->setDisplayFormat("dd-MM-yyyy");
    nextMaintenanceDateField->setCalendarPopup(true);
    nextMaintenanceDateField->setDate(QDate::currentDate());
    QLabel* nextMaintenanceDateLabel = new QLabel("Next Maintenance Date", this);
    QComboBox* actionCombo = new QComboBox(this);
    actionCombo->addItems({
        "Oil Change",
        "Calibration",
        "Parts Replacement",
        "Inspection",
        "Lubrication",
        "Cleaning",
        "Software Update",
        "Alignment",
        "Filter Replacement",
        "Electrical Check"
    });
    actionCombo->setCurrentIndex(-1);
    QLabel* actionLabel = new QLabel("Action Performed", this);
    QPushButton* addButton = new QPushButton("Add Record", this);
    QPushButton* updateButton = new QPushButton("Update Record", this);
    QPushButton* deleteButton = new QPushButton("Delete Record", this);
    QPushButton* logoutButton = new QPushButton("Logout", this);

    // Role-based access
    bool isAdmin = currentUser->getRole() == UserRole::Admin;
    equipmentIdLabel->setVisible(isAdmin);
    equipmentIdCombo->setVisible(isAdmin);
    equipmentNameLabel->setVisible(isAdmin);
    equipmentNameCombo->setVisible(isAdmin);
    maintenanceDateLabel->setVisible(isAdmin);
    maintenanceDateField->setVisible(isAdmin);
    nextMaintenanceDateLabel->setVisible(isAdmin);
    nextMaintenanceDateField->setVisible(isAdmin);
    actionLabel->setVisible(isAdmin);
    actionCombo->setVisible(isAdmin);
    addButton->setVisible(isAdmin);
    updateButton->setVisible(isAdmin);
    deleteButton->setVisible(isAdmin);
    logoutButton->setVisible(true);

    // Add labels and fields to layout
    layout->addWidget(equipmentIdLabel);
    layout->addWidget(equipmentIdCombo);
    layout->addWidget(equipmentNameLabel);
    layout->addWidget(equipmentNameCombo);
    layout->addWidget(maintenanceDateLabel);
    layout->addWidget(maintenanceDateField);
    layout->addWidget(nextMaintenanceDateLabel);
    layout->addWidget(nextMaintenanceDateField);
    layout->addWidget(actionLabel);
    layout->addWidget(actionCombo);
    layout->addWidget(addButton);
    layout->addWidget(updateButton);
    layout->addWidget(deleteButton);
    layout->addWidget(logoutButton);
    layout->addStretch();

    // Function to refresh maintenance records table
    auto refreshMaintenanceTable = [=](const QVector<MaintenanceRecord*>& recordsToShow) {
        table->setRowCount(0);
        table->setRowCount(recordsToShow.size());
        QDate currentDate = QDate::currentDate();
        for (int i = 0; i < recordsToShow.size(); ++i) {
            QTableWidgetItem* idItem = new QTableWidgetItem(recordsToShow[i]->getEquipmentId());
            QTableWidgetItem* nameItem = new QTableWidgetItem(recordsToShow[i]->getEquipmentName());
            QTableWidgetItem* maintenanceDateItem = new QTableWidgetItem(recordsToShow[i]->getMaintenanceDate());
            QTableWidgetItem* nextMaintenanceDateItem = new QTableWidgetItem(recordsToShow[i]->getNextMaintenanceDate());
            QTableWidgetItem* actionItem = new QTableWidgetItem(recordsToShow[i]->getAction());
            QString status;
            QDate nextDate = QDate::fromString(recordsToShow[i]->getNextMaintenanceDate(), "dd-MM-yyyy");
            if (nextDate < currentDate) {
                status = "Overdue";
                nextMaintenanceDateItem->setBackground(Qt::red);
            } else {
                status = "Scheduled";
                nextMaintenanceDateItem->setBackground(Qt::green);
            }
            QTableWidgetItem* statusItem = new QTableWidgetItem(status);
            table->setItem(i, 0, idItem);
            table->setItem(i, 1, nameItem);
            table->setItem(i, 2, maintenanceDateItem);
            table->setItem(i, 3, nextMaintenanceDateItem);
            table->setItem(i, 4, actionItem);
            table->setItem(i, 5, statusItem);
        }
        table->resizeColumnsToContents();
    };

    // Initial maintenance records table population
    refreshMaintenanceTable(maintenanceManager.getRecords());

    // Connect equipmentIdCombo to update equipmentNameCombo and filter table
    connect(equipmentIdCombo, &QComboBox::currentTextChanged, this, [=](const QString& equipmentId) {
        equipmentNameCombo->clear();
        if (equipmentId.isEmpty()) {
            equipmentNameCombo->setEnabled(false);
            refreshMaintenanceTable(maintenanceManager.getRecords());
            return;
        }
        equipmentNameCombo->setEnabled(true);
        QVector<Item*> inventoryItems = inventoryManager.getItems();
        for (const Item* item : inventoryItems) {
            if (item->getId() == equipmentId) {
                equipmentNameCombo->addItem(item->getName());
                break;
            }
        }
        equipmentNameCombo->setCurrentIndex(0);
        // Filter table to show records for the selected Equipment ID
        QVector<MaintenanceRecord*> filteredRecords = maintenanceManager.searchRecords(equipmentId);
        refreshMaintenanceTable(filteredRecords);
    });

    // Connect search button (accessible to all users)
    connect(searchButton, &QPushButton::clicked, this, [=]() {
        QString equipmentId = searchField->text();
        if (equipmentId.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter an Equipment ID to search.");
            return;
        }
        QVector<MaintenanceRecord*> results = maintenanceManager.searchRecords(equipmentId);
        refreshMaintenanceTable(results);
    });

    // Connect clear search button (accessible to all users)
    connect(clearSearchButton, &QPushButton::clicked, this, [=]() {
        searchField->clear();
        equipmentIdCombo->setCurrentIndex(-1);
        refreshMaintenanceTable(maintenanceManager.getRecords());
    });

    // Connect table selection to populate fields (for Admins only)
    connect(table, &QTableWidget::itemSelectionChanged, this, [=]() {
        if (!isAdmin) return;
        if (table->selectedItems().isEmpty()) return;
        int row = table->currentRow();
        equipmentIdCombo->setCurrentText(table->item(row, 0)->text());
        equipmentNameCombo->setCurrentText(table->item(row, 1)->text());
        maintenanceDateField->setDate(QDate::fromString(table->item(row, 2)->text(), "dd-MM-yyyy"));
        nextMaintenanceDateField->setDate(QDate::fromString(table->item(row, 3)->text(), "dd-MM-yyyy"));
        actionCombo->setCurrentText(table->item(row, 4)->text());
    });

    // Connect buttons (Admin-only)
    if (isAdmin) {
        connect(addButton, &QPushButton::clicked, this, [=]() {
            QString equipmentId = equipmentIdCombo->currentText();
            QString equipmentName = equipmentNameCombo->currentText();
            QString maintenanceDate = maintenanceDateField->date().toString("dd-MM-yyyy");
            QString nextMaintenanceDate = nextMaintenanceDateField->date().toString("dd-MM-yyyy");
            QString action = actionCombo->currentText();
            if (equipmentId.isEmpty() || equipmentName.isEmpty() || action.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required.");
                return;
            }
            if (maintenanceDateField->date() > nextMaintenanceDateField->date()) {
                QMessageBox::warning(this, "Error", "Next Maintenance Date must be after Maintenance Date.");
                return;
            }
            if (maintenanceManager.addRecord(equipmentId, equipmentName, maintenanceDate, nextMaintenanceDate, action)) {
                QMessageBox::information(this, "Success", "Maintenance record added.");
                // Refresh table with filtered records for the current Equipment ID
                QVector<MaintenanceRecord*> filteredRecords = maintenanceManager.searchRecords(equipmentId);
                refreshMaintenanceTable(filteredRecords);
                equipmentIdCombo->setCurrentIndex(-1);
                equipmentNameCombo->clear();
                equipmentNameCombo->setEnabled(false);
                maintenanceDateField->setDate(QDate::currentDate());
                nextMaintenanceDateField->setDate(QDate::currentDate());
                actionCombo->setCurrentIndex(-1);
            }
        });

        connect(updateButton, &QPushButton::clicked, this, [=]() {
            if (table->selectedItems().isEmpty()) {
                QMessageBox::warning(this, "Error", "Please select a record to update.");
                return;
            }
            int row = table->currentRow();
            QString oldEquipmentId = table->item(row, 0)->text();
            QString oldMaintenanceDate = table->item(row, 2)->text();
            QString oldAction = table->item(row, 4)->text();
            QString newEquipmentId = equipmentIdCombo->currentText();
            QString newEquipmentName = equipmentNameCombo->currentText();
            QString newMaintenanceDate = maintenanceDateField->date().toString("dd-MM-yyyy");
            QString newNextMaintenanceDate = nextMaintenanceDateField->date().toString("dd-MM-yyyy");
            QString newAction = actionCombo->currentText();
            if (newEquipmentId.isEmpty() || newEquipmentName.isEmpty() || newAction.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required.");
                return;
            }
            if (maintenanceDateField->date() > nextMaintenanceDateField->date()) {
                QMessageBox::warning(this, "Error", "Next Maintenance Date must be after Maintenance Date.");
                return;
            }
            if (maintenanceManager.updateRecord(oldEquipmentId, oldMaintenanceDate, oldAction,
                                                newEquipmentId, newEquipmentName, newMaintenanceDate,
                                                newNextMaintenanceDate, newAction)) {
                QMessageBox::information(this, "Success", "Maintenance record updated.");
                // Refresh table with filtered records for the current Equipment ID
                QVector<MaintenanceRecord*> filteredRecords = maintenanceManager.searchRecords(newEquipmentId);
                refreshMaintenanceTable(filteredRecords);
                equipmentIdCombo->setCurrentIndex(-1);
                equipmentNameCombo->clear();
                equipmentNameCombo->setEnabled(false);
                maintenanceDateField->setDate(QDate::currentDate());
                nextMaintenanceDateField->setDate(QDate::currentDate());
                actionCombo->setCurrentIndex(-1);
            } else {
                QMessageBox::warning(this, "Error", "Record not found or update failed.");
            }
        });

        connect(deleteButton, &QPushButton::clicked, this, [=]() {
            if (table->selectedItems().isEmpty()) {
                QMessageBox::warning(this, "Error", "Please select a record to delete.");
                return;
            }
            int row = table->currentRow();
            QString equipmentId = table->item(row, 0)->text();
            QString maintenanceDate = table->item(row, 2)->text();
            QString action = table->item(row, 4)->text();
            if (maintenanceManager.deleteRecord(equipmentId, maintenanceDate, action)) {
                QMessageBox::information(this, "Success", "Maintenance record deleted.");
                // Refresh table with filtered records for the current Equipment ID
                QVector<MaintenanceRecord*> filteredRecords = maintenanceManager.searchRecords(equipmentIdCombo->currentText());
                refreshMaintenanceTable(filteredRecords.isEmpty() ? maintenanceManager.getRecords() : filteredRecords);
                equipmentIdCombo->setCurrentIndex(-1);
                equipmentNameCombo->clear();
                equipmentNameCombo->setEnabled(false);
                maintenanceDateField->setDate(QDate::currentDate());
                nextMaintenanceDateField->setDate(QDate::currentDate());
                actionCombo->setCurrentIndex(-1);
            } else {
                QMessageBox::warning(this, "Error", "Record not found or deletion failed.");
            }
        });
    }

    // Connect logout button (accessible to all users)
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);

    tabs->addTab(maintenanceTab, "Maintenance Scheduling");
}

void MainWindow::handleLogout() {
    IntroWindow* introWindow = new IntroWindow(currentUser);
    introWindow->show();
    this->close();
}
