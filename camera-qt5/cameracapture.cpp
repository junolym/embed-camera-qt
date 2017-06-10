#include "cameracapture.h"
#include "ui_cameracapture.h"
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtMultimedia/qcamera.h>
#include <QtWidgets>

CameraCapture::CameraCapture(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraCapture),
    camera(0)
{
    ui->setupUi(this);

    camera = new QCamera;

    viewfinder = new QCameraViewfinder();
    viewfinder->show();
    camera->setViewfinder(viewfinder);
    ui->stackedWidget->addWidget(viewfinder);

    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();

}

CameraCapture::~CameraCapture() {
    delete camera;
}
