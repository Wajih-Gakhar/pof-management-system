#include "facerecognitionwindow.h"
#include "ui_facerecognitionwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QThread>
#include <QDir>

FaceRecognitionWindow::FaceRecognitionWindow(User* user, const QString& targetSystem, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::FaceRecognitionWindow), currentUser(user), targetSystem(targetSystem), attempts(0)
{
    ui->setupUi(this);
    setWindowTitle("POF Management System - Face Recognition");
    resize(400, 300);

    // Personalized greeting
    QString roleName = user->getRoleName();
    QString greeting = QString("Hey %1! It's good to see you.\nFor the system's security, I have to scan your face.\nTap 'Identify Your Identity' to proceed.").arg(roleName);
    ui->greetingLabel->setText(greeting);

    // Load Haar cascade
    QString cascadePath = "D:/Softwares/opencv/install/etc/haarcascades/haarcascade_frontalface_default.xml";
    if (!faceCascade.load(cascadePath.toStdString())) {
        ui->statusLabel->setText("Error: Could not load face cascade.");
        qDebug() << "Failed to load cascade from:" << cascadePath;
        return;
    }

    // Initialize and train face recognizer
    faceRecognizer = cv::face::LBPHFaceRecognizer::create();
    QString imagePath = user->getRole() == UserRole::Admin
                            ? "C:/Users/Icopx/OneDrive/Desktop/Coding/QT/POF_Management_System/build/Release/admin.jpg"
                            : "C:/Users/Icopx/OneDrive/Desktop/Coding/QT/POF_Management_System/build/Release/user.jpg";
    qDebug() << "Attempting to load image:" << imagePath;

    // Check if file exists
    QFileInfo fileInfo(imagePath);
    if (!fileInfo.exists()) {
        ui->statusLabel->setText(QString("Error: Image file does not exist: %1").arg(imagePath));
        qDebug() << "File does not exist:" << imagePath;
        return;
    }
    if (!fileInfo.isReadable()) {
        ui->statusLabel->setText(QString("Error: Cannot read image file: %1").arg(imagePath));
        qDebug() << "File is not readable:" << imagePath;
        return;
    }

    cv::Mat faceImage = cv::imread(imagePath.toStdString(), cv::IMREAD_GRAYSCALE);
    if (faceImage.empty()) {
        ui->statusLabel->setText(QString("Error: Could not load %1.").arg(imagePath));
        qDebug() << "Failed to load image with cv::imread:" << imagePath;
        return;
    }

    std::vector<cv::Mat> images;
    std::vector<int> labels;
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(faceImage, faces, 1.05, 7, 0, cv::Size(50, 50));
    if (!faces.empty()) {
        images.push_back(faceImage(faces[0]));
        labels.push_back(1); // Label 1 for the user
        faceRecognizer->train(images, labels);
    } else {
        ui->statusLabel->setText(QString("Error: No face detected in %1.").arg(imagePath));
        qDebug() << "No face detected in" << imagePath;
        return;
    }
}

FaceRecognitionWindow::~FaceRecognitionWindow()
{
    if (cap.isOpened()) cap.release();
    delete ui;
}

void FaceRecognitionWindow::on_identifyButton_clicked()
{
    if (attempts >= MAX_ATTEMPTS) {
        ui->statusLabel->setText("Too many failed attempts. Please try again later.");
        ui->identifyButton->setEnabled(false);
        return;
    }

    ui->statusLabel->setText("Starting face recognition...");

    // Open webcam
    cap.open(0, cv::CAP_DSHOW);
    if (!cap.isOpened()) {
        ui->statusLabel->setText("Error: Could not open webcam.");
        attempts++;
        ui->statusLabel->setText(QString("Attempts left: %1").arg(MAX_ATTEMPTS - attempts));
        return;
    }

    cv::Mat frame;
    int faceDetectedCount = 0;
    const int threshold = 3;
    bool recognized = false;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            ui->statusLabel->setText("Error: No frame captured.");
            break;
        }

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.05, 7, 0, cv::Size(50, 50));

        if (!faces.empty()) {
            faceDetectedCount++;
            if (faceDetectedCount >= threshold) {
                ui->statusLabel->setText("Face detected!");
                int predictedLabel = -1;
                double confidence = 0.0;
                try {
                    faceRecognizer->predict(gray(faces[0]), predictedLabel, confidence);
                    qDebug() << "Confidence:" << confidence;
                    if (confidence < 50.0 && predictedLabel == 1) {
                        recognized = true;
                        QString roleName = currentUser->getRoleName();
                        QMessageBox::information(this, "Welcome", QString("Welcome %1").arg(roleName));
                        emit faceRecognitionSuccess(currentUser, targetSystem);
                        this->close();
                        break;
                    }
                } catch (const cv::Exception& e) {
                    ui->statusLabel->setText("Error: Face recognition failed.");
                    qDebug() << "Exception:" << e.what();
                    break;
                }
            }
        } else {
            faceDetectedCount = 0;
            ui->statusLabel->setText("No face detected yet...");
        }

        if (faceDetectedCount >= threshold && !recognized) {
            attempts++;
            ui->statusLabel->setText(QString("Face not recognized. Attempts left: %1").arg(MAX_ATTEMPTS - attempts));
            break;
        }

        QThread::msleep(50);
        QCoreApplication::processEvents();
        QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        if (QApplication::sendEvent(this, event) && event->isAccepted()) {
            break;
        }
    }

    cap.release();
}

void FaceRecognitionWindow::on_cancelButton_clicked()
{
    emit faceRecognitionFailed();
    this->close();
}
