#include <QtGui>
#include "processImage.h"

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);
    ProcessImage process;
    process.setWindowFlags(Qt::Window);
    process.showFullScreen();
    process.show();
    return app.exec();
}