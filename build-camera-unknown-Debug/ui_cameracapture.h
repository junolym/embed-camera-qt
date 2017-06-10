/********************************************************************************
** Form generated from reading UI file 'cameracapture.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERACAPTURE_H
#define UI_CAMERACAPTURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraCapture
{
public:
    QStackedWidget *stackedWidget;

    void setupUi(QMainWindow *CameraCapture)
    {
        if (CameraCapture->objectName().isEmpty())
            CameraCapture->setObjectName(QStringLiteral("CameraCapture"));
        CameraCapture->resize(800, 480);
        stackedWidget = new QStackedWidget(CameraCapture);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        CameraCapture->setCentralWidget(stackedWidget);

        retranslateUi(CameraCapture);

        QMetaObject::connectSlotsByName(CameraCapture);
    } // setupUi

    void retranslateUi(QMainWindow *CameraCapture)
    {
        Q_UNUSED(CameraCapture);
    } // retranslateUi

};

namespace Ui {
    class CameraCapture: public Ui_CameraCapture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERACAPTURE_H
