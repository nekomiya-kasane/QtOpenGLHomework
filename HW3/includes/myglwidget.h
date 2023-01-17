#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QtGui>
#include <deque>

#include "ImGuiRenderer.h"
#include "common.h"
#include "events.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "light.h"

struct Camera {
  QVector3D position = {-0.6f, 2.0f, -5.5f}, front, up, right;
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
  MyGLWidget(QWidget *parent = nullptr);
  ~MyGLWidget();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
  virtual void wheelEvent(QWheelEvent *event) override;
#endif
  //  virtual void keyPressEvent(QKeyEvent *event) override;
  //  virtual void keyReleaseEvent(QKeyEvent *event) override;

 protected:
  const QMatrix4x4 &GetViewMatrix();
  const QMatrix4x4 &GetProjMatrix();

 private:
  // Time
  QTimer *_timer;

  Time _lastTime, _time;
  Duration _deltaTime;

  // FPS
  const size_t _fpsBufferSize = 1000;
  float _fps[1000];

  // Clear Color
  QVector4D _backgroundColor = {0.5f, 0.5f, 0.5f, 1.0f};

  // Meshes
  struct MeshInstance {
    QOpenGLBuffer *vbo = nullptr, *ebo = nullptr;
    QOpenGLVertexArrayObject *vao = nullptr;

    size_t count = 0;

    inline ~MeshInstance() {
      delete vbo;
      delete vao;
      delete ebo;
    }
  };

  std::map<size_t, MeshInstance> _sphereMeshes;
  int _currentMesh = 10;
  int _currentMeshWrap = 10;

  // Shader
  QOpenGLShaderProgram _mainShader;

  // Camera
  Camera _camera;
  QMatrix4x4 _view, _proj;
  bool _isPerspective = true;

  // Events
  struct Status {
    std::map<int, KeyStatus> _keyStatus;
    std::map<Qt::MouseButton, MouseButtonStatus> _mouseButtonStatus;
    QVector2D _mousePos, _mouseCoord, _mouseNormalCoord, _mouseMove,
        _mouseMoveCoord, _mouseWheeling;
    bool _moved = false, _wheeled = false, _windowSizeChanged = true;

    void ResetMove();
    void ResetWheel();
  } _status;

  // Material
  //  Ns 5.60558655
  //  Ka 1.000000 1.000000 1.000000
  //  Kd 0.800000 0.176183 0.709585
  //  Ks 0.764249 0.764249 0.764249
  //  Ke 0.000000 0.000000 0.000000
  //  Ni 1.450000
  //  d 1.000000
  //  illum 2
  struct Material {
    QVector3D ambient = {1.0f, 1.0f, 1.0f};
    QVector3D diffuse = {0.8f, 0.176183f, 0.709585f};
    QVector3D specular = {0.764249f, 0.764249f, 0.764249f};
    float shiness = 5.60558655f;
  } _material;

  // Lights
  enum LightingModel {
    LAMBERT = 0x0100u,
    HALF_LAMBERT = 0x0200u,
    PHONG = 0x0400u,
    BLINN_PHONG = 0x1000u,

    SHOW_NORM = 0x2000u,
    SHOW_COLOR = 0x4000u
  } _lightingModel = PHONG;
  int _guiLightingModel = 3;

  DirectionalLight _dirLight;
  SpotLight _spotLight;
  PointLight _pointLight;
};
#endif  // MYGLWIDGET_HF
