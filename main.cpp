#include "main.h"
#include <QCryptographicHash>
#include <QApplication>
#include "loginwindow.h"

QString hashPassword(const QString& password, const QString& salt, int iterations) {
    QString combined = password + salt; // Fixed the typo here
    QByteArray result = combined.toUtf8();
    for (int i = 0; i < iterations; ++i) {
        result = QCryptographicHash::hash(result, QCryptographicHash::Sha256);
    }
    return QString(result.toHex());
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    LoginWindow loginWindow;
    loginWindow.show();
    return app.exec();
}
