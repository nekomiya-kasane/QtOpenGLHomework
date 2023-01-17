QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++17

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += includes

SOURCES += \
    src/main.cpp \
    src/myglwidget.cpp

HEADERS += \
    includes/GL/eglew.h \
    includes/GL/glew.h \
    includes/GL/glxew.h \
    includes/GL/wglew.h \
    includes/myglwidget.h

win32: LIBS += -lGlU32
win32: LIBS += -lOpenGL32
win32: LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"

