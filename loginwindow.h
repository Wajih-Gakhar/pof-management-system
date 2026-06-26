#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVariant>
#include "usermanager.h"
#include "facerecognitionwindow.h"

class LoginWindow : public QMainWindow {
    Q_OBJECT

private:
    UserManager userManager;
    QLineEdit* usernameField;
    QLineEdit* passwordField;
    QPushButton* loginButton;
    QLabel* statusLabel;
    QCheckBox* showPasswordCheckBox;
    int loginAttempts;
    static const int MAX_ATTEMPTS = 3;

public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

private slots:
    void handleLogin();
    void togglePasswordVisibility(Qt::CheckState state);
    void handleFaceRecognitionSuccess(User* user, const QString& targetSystem);
    void handleFaceRecognitionFailed();

signals:
    void loginSuccess(User* user, const QString& targetSystem);
};

#endif // LOGINWINDOW_H
