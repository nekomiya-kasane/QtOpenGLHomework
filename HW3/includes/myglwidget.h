#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <deque>

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiRenderer.h"

#include "common.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    // Time
    QTimer *_timer;

    Time _lastTime, _time;
    Duration _deltaTime;

    // FPS
    const size_t _fpsBufferSize = 1000;
    float _fps[1000];

    // Clear Color
    QVector4D _backgroundColor = {1.0f, 1.0f, 0.0f, 1.0f};

    // Shader
    QOpenGLShaderProgram _mainShader;
};
#endif // MYGLWIDGET_HF
