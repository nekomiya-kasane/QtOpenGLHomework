#include "myglwidget.h"

#include <QtImGui.h>
#include <direct.h>
#include <imgui.h>

#include "mesh.h"

const QMatrix4x4 kIndetity = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  _timer = new QTimer(this);  // 实例化一个定时器
  _timer->start(1);  // 时间间隔设置为16ms，可以根据需要调整
  connect(
      _timer, SIGNAL(timeout()), this,
      SLOT(
          update()));  // 连接update()函数，每16ms触发一次update()函数进行重新绘图

  for (size_t i = 0; i < _fpsBufferSize; ++i) {
    _fps[i] = 0;
  }
}

/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget() { delete _timer; }

/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  QtImGui::initialize(this);

  // Prepare Env

  glViewport(0, 0, width(), height());

  const auto& c = _backgroundColor;
  glClearColor(c[0], c[1], c[2], c[3]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Prepare Camera
  _camera.front[0] =
      std::cos(ToRadian(_camera.yaw)) * std::cos(ToRadian(_camera.pitch));
  _camera.front[1] = std::sin(ToRadian(_camera.pitch));
  _camera.front[2] =
      std::sin(ToRadian(_camera.yaw)) * std::cos(ToRadian(_camera.pitch));
  _camera.front = _camera.front.normalized();

  auto ya = QVector3D{0, 1, 0};
  _camera.right = QVector3D::crossProduct(_camera.front, ya).normalized();
  _camera.up =
      QVector3D::crossProduct(_camera.right, _camera.front).normalized();

  // Prepare Shaders

  auto vertShader = new QOpenGLShader(QOpenGLShader::Vertex);
  vertShader->compileSourceFile("../shaders/default.vert");
  std::cout << vertShader->log().toStdString() << std::endl;

  auto fragShader = new QOpenGLShader(QOpenGLShader::Fragment);
  fragShader->compileSourceFile("../shaders/default.frag");
  std::cout << fragShader->log().toStdString() << std::endl;

  if (!_mainShader.create()) {
    std::cout << "Failed to create shader." << std::endl;
  }
  if (!_mainShader.addShader(vertShader) ||
      !_mainShader.addShader(fragShader)) {
    std::cout << "Failed to add shader." << std::endl;
  }
  if (!_mainShader.link()) {
    std::cout << "Failed to link." << std::endl;
  }

  std::cout << _mainShader.log().toStdString() << std::endl;

  _mainShader.bind();
  _mainShader.setUniformValue("view", GetViewMatrix());
  _mainShader.setUniformValue("proj", GetProjMatrix());
  _mainShader.setUniformValue("model", kIndetity);

  OGL_CHECKPOINT_DEBUG();

  // Prepare Meshes
  for (unsigned int i = 10; i <= 110; i += 20) {
    Mesh mesh;
    CreateSphere(i, i, mesh);

    auto* vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    auto* buf = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    auto* indbuf = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);

    OGL_CHECKPOINT_DEBUG();

    buf->create();
    indbuf->create();
    buf->bind();
    indbuf->bind();
    buf->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    indbuf->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    OGL_CHECKPOINT_DEBUG();
    buf->allocate(static_cast<int>(mesh.data.size() * sizeof(float)));
    indbuf->allocate(
        static_cast<int>(mesh.indices.size() * sizeof(unsigned int)));
    buf->write(0, mesh.data.data(),
               static_cast<int>(mesh.data.size() * sizeof(float)));
    indbuf->write(0, mesh.indices.data(),
                  static_cast<int>(mesh.indices.size() * sizeof(unsigned int)));

    _sphereMeshes[i].vao = vao;
    _sphereMeshes[i].vbo = buf;
    _sphereMeshes[i].ebo = indbuf;
    _sphereMeshes[i].count = mesh.indices.size() / 3;

    OGL_CHECKPOINT_DEBUG();

    _mainShader.enableAttributeArray(0);
    _mainShader.setAttributeBuffer(0, GL_FLOAT, 0, 3,
                                   static_cast<int>(11 * sizeof(float)));
    _mainShader.enableAttributeArray(1);
    _mainShader.setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 3,
                                   static_cast<int>(11 * sizeof(float)));
    _mainShader.enableAttributeArray(2);
    _mainShader.setAttributeBuffer(2, GL_FLOAT, sizeof(float) * 6, 2,
                                   static_cast<int>(11 * sizeof(float)));
    _mainShader.enableAttributeArray(3);
    _mainShader.setAttributeBuffer(3, GL_FLOAT, sizeof(float) * 8, 3,
                                   static_cast<int>(11 * sizeof(float)));

    OGL_CHECKPOINT_DEBUG();

    buf->release();
    indbuf->release();

    vao->release();

    OGL_CHECKPOINT_DEBUG();
  }

  _mainShader.release();

  OGL_CHECKPOINT_DEBUG();
}

/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL() {
  // Time
  _lastTime = _time;
  _time = std::chrono::high_resolution_clock::now();
  _deltaTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(_time - _lastTime)
          .count() /
      1000.0f;

  // Imgui
  QtImGui::newFrame();
  auto fps = static_cast<double>(ImGui::GetIO().Framerate);
  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0 / fps, fps);
  memcpy(_fps, &(_fps[0]) + 1, (_fpsBufferSize - 1) * sizeof(float));
  _fps[_fpsBufferSize - 1] = static_cast<float>(fps);
  ImGui::PlotLines("FPS", _fps, static_cast<int>(_fpsBufferSize));
  ImGui::ColorEdit4("clear color", &_backgroundColor[0]);
  ImGui::DragInt("Tessellation of Sphere", &_currentMeshWrap, 0.2f, 10, 110,
                 "%d");
  ImGui::ColorEdit3("Material - Ambient", &_material.ambient[0]);
  ImGui::ColorEdit3("Material - Diffuse", &_material.diffuse[0]);
  ImGui::ColorEdit3("Material - Specular", &_material.specular[0]);
  ImGui::DragFloat("Material - Shiness", &_material.shiness);
  _currentMesh = (static_cast<int>(_currentMeshWrap) - 10) / 20 * 20 + 10;

  // Update Camera
  if (_status._mouseButtonStatus[Qt::MouseButton::LeftButton].held &&
      _status._moved) {
    // rotate
    float fov_x = _camera.aspect * _camera.fov,
          rot_x = _status._mouseMoveCoord[0] / 2 * fov_x;
    float rot_y = _status._mouseMoveCoord[1] / 2 * _camera.fov;

    _camera.yaw -= rot_x * _camera.factorRot;
    _camera.pitch += rot_y * _camera.factorRot;

    if (_camera.pitch > 89.0f) _camera.pitch = 89.0f;
    if (_camera.pitch < -89.0f) _camera.pitch = -89.0f;

    _status.ResetMove();
  }

  if (_status._wheeled) {
    _camera.wheelFov += _status._mouseWheeling[1] * _camera.factorFov;
    _camera.wheelFov = _camera.wheelFov < -10  ? -10
                       : _camera.wheelFov > 10 ? 10
                                               : _camera.wheelFov;
    _camera.fov = std::atan(_camera.wheelFov) + 3.1415926f / 2;

    _status.ResetWheel();

    std::cout << _camera.wheelFov << "," << _camera.fov << std::endl;
  }

  _camera.front[0] =
      std::cos(ToRadian(_camera.yaw)) * std::cos(ToRadian(_camera.pitch));
  _camera.front[1] = std::sin(ToRadian(_camera.pitch));
  _camera.front[2] =
      std::sin(ToRadian(_camera.yaw)) * std::cos(ToRadian(_camera.pitch));
  _camera.front = _camera.front.normalized();

  auto ya = QVector3D{0, 1, 0};
  _camera.right = QVector3D::crossProduct(_camera.front, ya).normalized();
  _camera.up =
      QVector3D::crossProduct(_camera.right, _camera.front).normalized();

  // Update Model
  {
    // Update background
    const auto& c = _backgroundColor;
    glClearColor(c[0], c[1], c[2], c[3]);

    // Update Transformations
    _mainShader.bind();
    _mainShader.setUniformValue("view", GetViewMatrix());
    _mainShader.setUniformValue("proj", GetProjMatrix());
    _mainShader.setUniformValue("model", kIndetity);

    // Update Material
    _mainShader.setUniformValue("material.ambient", _material.ambient);
    _mainShader.setUniformValue("material.diffuse", _material.diffuse);
    _mainShader.setUniformValue("material.specular", _material.specular);
    _mainShader.setUniformValue("material.shiness", _material.shiness);

    // Update Lights
    // - Directional Light

    // Draw
    auto& instance = _sphereMeshes.at(static_cast<size_t>(_currentMesh));
    instance.vao->bind();
    instance.vbo->bind();
    instance.ebo->bind();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(instance.count) * 3,
                   GL_UNSIGNED_INT, nullptr);

    instance.ebo->release();
    instance.vbo->release();
    instance.vao->release();
  }

  ImGui::Render();
  QtImGui::render();
}

/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  _camera.aspect = static_cast<float>(width) / height;

  update();
}

/*###################################################
##  函数: GetViewMatrix/GetProjMatrix
##  函数描述： Get view and projection matrices from
##          Camera parameters.
##  参数描述： 无
#####################################################*/
const QMatrix4x4& MyGLWidget::GetViewMatrix() {
  _view.setToIdentity();
  _view.lookAt(_camera.position, _camera.position + _camera.front, _camera.up);
  return _view;
}

const QMatrix4x4& MyGLWidget::GetProjMatrix() {
  _proj.setToIdentity();
  if (_isPerspective) {
    _proj.perspective(ToDegree(_camera.fov), _camera.aspect, _camera.zNear,
                      _camera.zFar);
  } else {
    _proj.ortho(-_camera.aspect * _camera.scale, _camera.aspect * _camera.scale,
                -1.0f * _camera.scale, 1.0f * _camera.scale, _camera.zNear,
                _camera.zFar);
  }
  return _proj;
}

void MyGLWidget::mousePressEvent(QMouseEvent* event) {
  auto& io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
    _status._mouseButtonStatus[event->button()] =
        MouseButtonStatus(event->button(), event->type());
  }
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  auto& io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
    _status._mouseButtonStatus[event->button()] =
        MouseButtonStatus(event->button(), event->type());
  }
}
void MyGLWidget::mouseMoveEvent(QMouseEvent* event) {
  auto& io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
    QVector2D newPos = {static_cast<float>(event->x()),
                        static_cast<float>(event->y())};

    _status._mouseMove = newPos - _status._mousePos;
    _status._mousePos = newPos;

    _status._mouseCoord[0] = _status._mousePos[0] / width();
    _status._mouseCoord[1] = _status._mousePos[1] / height();

    _status._mouseMoveCoord[0] = _status._mouseMove[0] / width();
    _status._mouseMoveCoord[1] = _status._mouseMove[1] / height();

    _status._mouseNormalCoord[0] =
        static_cast<float>(_status._mousePos[0] - 0.5f * width()) /
        static_cast<float>(0.5f * width());
    _status._mouseNormalCoord[1] =
        static_cast<float>(_status._mousePos[1] - 0.5f * height()) /
        static_cast<float>(0.5f * height());

    for (auto& status : _status._mouseButtonStatus) {
      if (status.second.down) {
        status.second.held = true;
      }
    }

    _status._moved = true;

    std::cout << event->x() << ", " << event->y() << std::endl;
  }
}

#if QT_CONFIG(wheelevent)
void MyGLWidget::wheelEvent(QWheelEvent* event) {
  _status._mouseWheeling[0] = static_cast<float>(event->x());
  _status._mouseWheeling[1] = static_cast<float>(event->y());
  _status._wheeled = true;
}
#endif

void MyGLWidget::Status::ResetMove() {
  _mouseCoord[0] = 0;
  _mouseCoord[1] = 0;

  _mouseMoveCoord[0] = 0;
  _mouseMoveCoord[1] = 0;

  _moved = false;
}

void MyGLWidget::Status::ResetWheel() { _wheeled = false; }

// void MyGLWidget::keyPressEvent(QKeyEvent* event);
// void MyGLWidget::keyReleaseEvent(QKeyEvent* event);
