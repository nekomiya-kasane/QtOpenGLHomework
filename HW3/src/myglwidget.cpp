#include <QtImGui.h>
#include <imgui.h>

#include <direct.h>

#include "myglwidget.h"

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
    _timer = new QTimer(this); // 实例化一个定时器
    _timer->start(1); // 时间间隔设置为16ms，可以根据需要调整
    connect(_timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图

    for (size_t i = 0; i < _fpsBufferSize; ++i) {
        _fps[i] = 0;
    }

//    char buf[128];
//    _getcwd(buf, 128);
//    std::cout << "Current Working Directory: \n" << buf << std::endl;
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
    delete _timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
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
    if (!_mainShader.addShader(vertShader) || !_mainShader.addShader(fragShader)) {
        std::cout << "Failed to add shader." << std::endl;
    }
    if (!_mainShader.link()) {
        std::cout << "Failed to link." << std::endl;
    }

    OGL_CHECKPOINT_DEBUG();
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    // Time
    _lastTime = _time;
    _time     = std::chrono::high_resolution_clock::now();
    _deltaTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(_time - _lastTime).count() / 1000.0f;

    // Imgui
    QtImGui::newFrame();
    auto fps = static_cast<double>(ImGui::GetIO().Framerate);
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0 / fps, fps);
    memcpy(_fps, &(_fps[0]) + 1, (_fpsBufferSize - 1) * sizeof(float));
    _fps[_fpsBufferSize - 1] = static_cast<float>(fps);
    ImGui::PlotLines("FPS", _fps, static_cast<int>(_fpsBufferSize));
    ImGui::ColorEdit4("clear color", &_backgroundColor[0]);

    {
        // Update background
        const auto& c = _backgroundColor;
        glClearColor(c[0], c[1], c[2], c[3]);
    }

    ImGui::Render();
    QtImGui::render();
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

