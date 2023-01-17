QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++17

include(third_party/qtimgui/qtimgui.pri)

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += includes

SOURCES += \
    src/checkpoint.cpp \
    src/light.cpp \
    src/main.cpp \
    src/mesh.cpp \
    src/myglwidget.cpp \

HEADERS += \
    includes/GL/eglew.h \
    includes/GL/glew.h \
    includes/GL/glxew.h \
    includes/GL/wglew.h \
    includes/common.h \
    includes/events.h \
    includes/light.h \
    includes/mesh.h \
    includes/myglwidget.h

win32: LIBS += -lGlU32
win32: LIBS += -lOpenGL32
win32: LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"

DISTFILES += \
    shaders/default.frag \
    shaders/default.vert

Release:DESTDIR = $$PWD/release
Release:OBJECTS_DIR = $$PWD/release/.obj
Release:MOC_DIR = $$PWD/release/.moc
Release:RCC_DIR = $$PWD/release/.rcc
Release:UI_DIR = $$PWD/release/.ui

Debug:DESTDIR = $$PWD/debug
Debug:OBJECTS_DIR = $$PWD/debug/.obj
Debug:MOC_DIR = $$PWD/debug/.moc
Debug:RCC_DIR = $$PWD/debug/.rcc
Debug:UI_DIR = $$PWD/debug/.ui
