#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>



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
    QTimer *timer;
    // 你可以将你的shader或者vbo声明为私有变量
    // GLuint vertexShader;
    // GLuint fragmentShader;
};
#endif // MYGLWIDGET_H
