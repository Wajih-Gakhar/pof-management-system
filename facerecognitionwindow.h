#ifndef FACERECOGNITIONWINDOW_H
#define FACERECOGNITIONWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>
#include "user.h"

namespace Ui {
class FaceRecognitionWindow;
}

class FaceRecognitionWindow : public QMainWindow
{
    Q_OBJECT

public:
    FaceRecognitionWindow(User* user, const QString& targetSystem, QWidget* parent = nullptr);
    ~FaceRecognitionWindow();

private slots:
    void on_identifyButton_clicked();
    void on_cancelButton_clicked();

signals:
    void faceRecognitionSuccess(User* user, const QString& targetSystem);
    void faceRecognitionFailed();

private:
    Ui::FaceRecognitionWindow* ui;
    User* currentUser;
    QString targetSystem;
    cv::CascadeClassifier faceCascade;
    cv::VideoCapture cap;
    cv::Ptr<cv::face::LBPHFaceRecognizer> faceRecognizer;
    int attempts;
    static const int MAX_ATTEMPTS = 3;
};

#endif // FACERECOGNITIONWINDOW_H
