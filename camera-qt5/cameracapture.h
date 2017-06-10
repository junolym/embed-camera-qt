#ifndef RECORDER_H
#define RECORDER_H

#include <QtMultimedia/qcamera.h>
#include <QtMultimedia/qmediarecorder.h>
#include <QCameraViewfinder>

namespace Ui {
   class CameraCapture;
}

#include <QtWidgets/QMainWindow>

class CameraCapture : public QMainWindow
{
   Q_OBJECT
public:
   CameraCapture(QWidget *parent = 0);
   ~CameraCapture();

private:
   Ui::CameraCapture *ui;
   QCamera *camera;
   QCameraViewfinder* viewfinder;
};

#endif
