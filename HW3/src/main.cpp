#include "myglwidget.h"

#include <QApplication>

int main(int argc, char *argv[]) try
{
    QApplication a(argc, argv);

    std::cout << "Current Working Directory: \n" << a.applicationDirPath().toStdString() << std::endl;

    // Use OpenGL 4.5 Core Profile, when available
    QSurfaceFormat glFormat;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        glFormat.setVersion(4, 5);
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(glFormat);

    MyGLWidget w;
    w.setWindowTitle("Homework 3");
    w.resize(1920, 1080);
    w.show();

    return a.exec();
}
catch (std::exception const &eErr)
{
  std::fprintf(stderr, "Top-level Exception (%s):\n", typeid(eErr).name());
  std::fprintf(stderr, "%s\n", eErr.what());
  std::fprintf(stderr, "Bye.\n");
  return 1;
}

