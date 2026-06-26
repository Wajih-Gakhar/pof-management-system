#include "loginwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include "mainwindow.h"
#include "introwindow.h"

LoginWindow::LoginWindow(QWidget* parent) : QMainWindow(parent), loginAttempts(0) {
    setWindowTitle("POF Management System - Login");
    resize(400, 300);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Username");
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);
    showPasswordCheckBox = new QCheckBox("Show Password", this);
    loginButton = new QPushButton("Login", this);
    statusLabel = new QLabel("", this);

    layout->addWidget(new QLabel("POF Management System", this));
    layout->addWidget(usernameField);
    layout->addWidget(passwordField);
    layout->addWidget(showPasswordCheckBox);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);
    layout->addStretch();

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(showPasswordCheckBox, &QCheckBox::checkStateChanged, this, &LoginWindow::togglePasswordVisibility);

    // Add default users for testing
    userManager.addUser("admin", "admin123", UserRole::Admin);
    userManager.addUser("user", "user123", UserRole::Regular);
}

LoginWindow::~LoginWindow() {
    // Empty destructor
}

void LoginWindow::handleLogin() {
    if (loginAttempts >= MAX_ATTEMPTS) {
        statusLabel->setText("Too many failed attempts. Please try again later.");
        loginButton->setEnabled(false);
        return;
    }
    QString username = usernameField->text();
    QString password = passwordField->text();
    User* user = userManager.authenticate(username, password);
    QString targetSystem = property("targetSystem").toString();
    QString expectedRole = property("expectedRole").toString();
    if (user) {
        // Validate role for system-specific login
        if (!targetSystem.isEmpty() && !expectedRole.isEmpty()) {
            QString userRole = user->getRole() == UserRole::Admin ? "Admin" : "Regular";
            if (userRole != expectedRole) {
                loginAttempts++;
                statusLabel->setText(QString("Access denied: Must login as %1. Attempts left: %2")
                                         .arg(expectedRole).arg(MAX_ATTEMPTS - loginAttempts));
                return;
            }
        }
        loginAttempts = 0;
        // Open FaceRecognitionWindow instead of proceeding directly
        FaceRecognitionWindow* faceWindow = new FaceRecognitionWindow(user, targetSystem, this);
        connect(faceWindow, &FaceRecognitionWindow::faceRecognitionSuccess, this, &LoginWindow::handleFaceRecognitionSuccess);
        connect(faceWindow, &FaceRecognitionWindow::faceRecognitionFailed, this, &LoginWindow::handleFaceRecognitionFailed);
        faceWindow->show();
    } else {
        loginAttempts++;
        statusLabel->setText(QString("Invalid username or password. Attempts left: %1").arg(MAX_ATTEMPTS - loginAttempts));
    }
}

void LoginWindow::handleFaceRecognitionSuccess(User* user, const QString& targetSystem) {
    statusLabel->setText("Login successful! Role: " + user->getRoleName());
    if (targetSystem.isEmpty()) {
        // Initial login: show IntroWindow
        IntroWindow* introWindow = new IntroWindow(user, this);
        introWindow->show();
        this->hide();
    } else {
        // System-specific login: emit signal
        emit loginSuccess(user, targetSystem);
        this->close();
    }
}

void LoginWindow::handleFaceRecognitionFailed() {
    statusLabel->setText("Face recognition failed. Please try again.");
}

void LoginWindow::togglePasswordVisibility(Qt::CheckState state) {
    if (state == Qt::Checked) {
        passwordField->setEchoMode(QLineEdit::Normal);
    } else {
        passwordField->setEchoMode(QLineEdit::Password);
    }
}
