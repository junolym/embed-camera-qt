TEMPLATE = app
TARGET = camera-qt5

QMAKE_LFLAGS += -L/usr/lib/x86_64-linux-gnu/

QT += multimedia multimediawidgets core gui widgets

CONFIG += mobility
MOBILITY += multimedia

HEADERS = cameracapture.h \
          cameracapture.h

SOURCES = main.cpp \
          cameracapture.cpp

FORMS += cameracapture.ui \

OTHER_FILES +=

