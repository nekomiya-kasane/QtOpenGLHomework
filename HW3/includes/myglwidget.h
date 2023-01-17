#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QtGui>
#include <deque>

#include "ImGuiRenderer.h"
#include "common.h"
#include "imgui.h"
#include "imgui_internal.h"

struct Camera {
  QVector3D position, front, up, right;
  float pitch = -20.0f, yaw = 90.0f, zoom = 45;

  float fov = 1.44f, wheelFov = std::tan(fov - 1.57f), aspect = 1.33f,
        zNear = 0.01f,
        zFar = 100.0f;  // perspective
  float scale = 1.0f;   // orthogonal

  constexpr static float factorRotInit = 50.0f, factorMoveInit = 1.0f,
                         factorFovInit = 0.05f;
  float factorRot = factorRotInit, factorMove = factorMoveInit,
        factorFov = factorFovInit;
};

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  MyGLWidget(QWidget* parent = nullptr);
  ~MyGLWidget();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

 protected:
  const QMatrix4x4& GetViewMatrix();
  const QMatrix4x4& GetProjMatrix();

 private:
  // Time
  QTimer* _timer;

  Time _lastTime, _time;
  Duration _deltaTime;

  // FPS
  const size_t _fpsBufferSize = 1000;
  float _fps[1000];

  // Clear Color
  QVector4D _backgroundColor = {1.0f, 1.0f, 0.0f, 1.0f};

  // Meshes
  struct MeshInstance {
    QOpenGLBuffer *vbo = nullptr, *ebo = nullptr;
    QOpenGLVertexArrayObject* vao = nullptr;

    size_t count = 0;

    inline ~MeshInstance() {
      delete vbo;
      delete vao;
      delete ebo;
    }
  };

  std::map<size_t, MeshInstance> _sphereMeshes;
  int _currentMesh = 10;

  // Shader
  QOpenGLShaderProgram _mainShader;

  // Camera
  Camera _camera;
  QMatrix4x4 _view, _proj;
  bool _isPerspective = true;
};
#endif  // MYGLWIDGET_HF
