#include "glaux.h"
#include "glut.h"
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "legacy_stdio_definitions.lib")
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

AUX_RGBImageRec* Images[10];//图片数组容器
GLuint ImagesIDs[11];  //索引
const char* szFiles[10] = {//各个材质贴图的相对路径
	("../rec/Sun.bmp"),
	("../rec/Mercury.bmp"),
	("../rec/Venus.bmp"),
	("../rec/Earth.bmp"),
	("../rec/Mars.bmp"),
	("../rec/Jupiter.bmp"),
	("../rec/Saturn.bmp"),
	("../rec/Uranus.bmp"),
	("../rec/Neptune.bmp"),
	("../rec/Moon.bmp"), };
GLubyte* pImg; //opengl图片处理类
GLint iWidth, iHeight;//窗口大小

#define PI 3.1415926//圆周率
static float  year = 0, month = 0, day = 0, angle = 30;//初始化时间 以及旋转角

GLint W, H, width, height;//窗口长宽属性
float pox = 2, poy = 3, poz = 8;  //照相机的位置
GLint fovy = 60;//相机的视场角

void Init();//初始化函数
void OnDisplay();//opengl绘图函数
void OnReshape(int, int);//刷新函数
void OnTimer(int);//定时器函数，定时刷新
void DrawCircle(GLdouble);//绘制星球轨道
void gltDrawSphere(GLfloat, GLint, GLint);//绘制星球
void keyDownEvent(unsigned char key, int x, int y);//按键响应函数

int main(int argc, char* argv[]) {//程序主函数
	glutInit(&argc, argv);//opengl初始化函数
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//创建窗口，指定显示模式
	glutInitWindowSize(1000, 800);//窗口大小
	glutInitWindowPosition(100, 100);//窗口位置
	glutCreateWindow("太阳系");//窗口标题
	Init();//调用初始化函数
	glutReshapeFunc(OnReshape);//绑定OpenGL刷新函数
	glutDisplayFunc(&OnDisplay);//绑定OpenGL绘制函数
	glutKeyboardFunc(&keyDownEvent);//绑定OpenGL按键函数
	glutTimerFunc(100, OnTimer, 1);//绑定OpenGL刷新函数
	glutMainLoop();//进去opengl主循环
	return 0;
}

void OnReshape(int w, int h) {
	W = w; H = h;//传入窗口大小
	width = W; height = H;//窗口大小
	glViewport(0, 0, w, h);//设计opengl视口大小
	glMatrixMode(GL_PROJECTION);//将当前矩阵指定为投影矩阵然后把矩阵设为单位矩阵：
	glLoadIdentity();//重置当前指定矩阵为单位矩阵
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1, 20);//用新矩阵替换单位矩阵
	glMatrixMode(GL_MODELVIEW);//对模型视景矩阵操作
	glLoadIdentity();//重置当前指定矩阵为单位矩阵
}

void Init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);//初始化背景颜色为黑色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空窗口绘制
	for (int i = 0; i < 10; i++) {//循环取各个纹理图片
		glGenTextures(1, &ImagesIDs[i]);  //生成纹理
		glBindTexture(GL_TEXTURE_2D, ImagesIDs[i]);   //绑定纹理

		//vs2019下必须进行转化
		WCHAR wfilename[256];//字符格式转化为256大小的数组
		memset(wfilename, 0, sizeof(wfilename));//初始化一串连续的内存，大小为wfilename
		//该函数映射一个字符串到一个宽字符（unicode）的字符串
		MultiByteToWideChar(CP_ACP, 0, szFiles[i], strlen(szFiles[i]) + 1, wfilename, sizeof(wfilename) / sizeof(wfilename[0]));

		Images[i] = auxDIBImageLoad(wfilename);  //加载图片

		iWidth = Images[i]->sizeX;//设置图片宽度
		iHeight = Images[i]->sizeY;//设置图片长度
		pImg = Images[i]->data;
		//装载纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImg);

		//纹理过滤、纹理裁剪
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//纹理环境
		glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//启动2D纹理
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
	}
	glEnable(GL_LIGHTING);                          // 开启光照效果
	glEnable(GL_LIGHT0);
}

void OnTimer(int value) {
	day += angle;//模拟各个星球公转
	glutPostRedisplay();//重新刷新页面绘图
	glutTimerFunc(100, OnTimer, 1);//再次执行下一帧
}

//轨道函数
void DrawCircle(GLdouble R) {
	glPushMatrix();//指定为当前矩阵
	glRotatef(90.0, 1.0, 0.0, 0.0);//初始化旋转量
	glColor3f(1.0, 1.0, 1.0);//初始化颜色
	glBegin(GL_LINE_LOOP);//开始换闭环线
	for (int i = 0; i < 1000; i++) {
		GLdouble angle1 = i * 2 * PI / 1000;
		glVertex2d(R * cos(angle1), R * sin(angle1));//不停计算下一位置的坐标，绘制线条，1000为圆的光滑度
	}
	glEnd();
	glPopMatrix();//结束绘画，刷新回主矩阵
}

//绘制星球
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks) {
	GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;//斯皮尔曼相关系数
	GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;//计算时间消耗度
	GLfloat ds = 1.0f / (GLfloat)iSlices;//位置量
	GLfloat dt = 1.0f / (GLfloat)iStacks;//位置量
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;
	GLint i, j;

	GLfloat rho=0.0f ;//初始化各个斯皮尔曼相关系数
	GLfloat srho = 0.0f;
	GLfloat crho = 0.0f;
	GLfloat srhodrho = 0.0f;
	GLfloat crhodrho = 0.0f;

	for (i = 0; i < iStacks; i++) {
		rho = (GLfloat)i * drho;
		srho = (GLfloat)(sin(rho));
		crho = (GLfloat)(cos(rho));
		srhodrho = (GLfloat)(sin(rho + drho));
		crhodrho = (GLfloat)(cos(rho + drho));

		glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for (j = 0; j <= iSlices; j++) {
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));

			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;

			glTexCoord2f(s, t);
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);

			x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
			s += ds;
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);
		}
	}
	
	glEnd();

	t -= dt;
}

void keyDownEvent(unsigned char key, int x, int y)
{
	switch (key)//按键判断
	{
	case 'a'://x轴增加
	{
		pox+=0.2;
		break;
	}
	case 'd'://x轴减小
	{
		pox-=0.2;
		break;
	}
	case 'w'://z轴增加
	{
		poz -=0.2;
		break;
	}
	case 's'://z轴增加
	{
		poz +=0.2;
		break;
	}
	case 'q'://y轴增减
	{
		poy +=0.2;
		break;
	}
	case 'e'://y轴减小
	{
		poy -=0.2;
		break;
	}
	default:
		break;
	}
}

void OnDisplay() {

	glColor3f(1.0, 0.0, 0.0);//初始化颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空画布

	year = day / 365;//当前年的时间
	month = day / 30;//当前月的时间

	glMatrixMode(GL_PROJECTION);//指定当前矩阵绘制矩阵
	glLoadIdentity(); //重置当前指定矩阵为单位矩阵
	gluPerspective(fovy, (GLfloat)W / (GLfloat)H, 2, 60.0);//矩阵变换

	glMatrixMode(GL_MODELVIEW);//指定当前矩阵绘制矩阵
	glLoadIdentity();//重置当前指定矩阵为单位矩阵
	gluLookAt(pox, poy, poz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//相机位置

	//太阳光
	GLfloat sun_mat_ambient[4] = { 1.0,1.0,1.0,0.0 };
	GLfloat sun_mat_diffuse[4] = { 1.0,1.0,0.5,1.0 };
	GLfloat sun_mat_specular[4] = { 1.0,1.0,1.0,1.0 };
	GLfloat sun_mat_shininess[] = { 10.0 };
	GLfloat sun_mat_emission[4] = { 0.1,0.1,0.1,1.0 };
	GLfloat mat_ambient[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat mat_diffuse[4] = { 1.0,1.0,1.0,1.0 };
	GLfloat mat_specular[4] = { 0.5,0.5,0.5,1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	GLfloat light_position[] = { -10.0,10.0,0.0, 1.0 };//光源位置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);     // 创建光源
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sun_mat_ambient);   // 材质设定
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sun_mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_mat_emission);

	//绘制太阳
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);            // 创建光源
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sun_mat_ambient);   // 材质设定
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sun_mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_mat_emission);
	glBindTexture(GL_TEXTURE_2D, ImagesIDs[0]);//添加纹理
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);  //太阳自转
	glRotatef(90.0, -1.0, 0.0, 0.0);//设置当前时间的旋转量
	gltDrawSphere(1.0, 50, 50);//画星球
	glPopMatrix();//结束绘画，刷新回主矩阵
	

	//绘制地月系统
	glPushMatrix();
	//绘制地球
	DrawCircle(4.5);//绘制轨道
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);   // 材质设定
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);  //地球公转
	glTranslatef(4.5, 0.0, 0.0);  //向x轴右移
	glRotatef(30.0, -1.0, 0.0, 0.0);//让地球倾斜
	glPushMatrix();//刷新矩阵
	glBindTexture(GL_TEXTURE_2D, ImagesIDs[3]);//材质贴图
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);//当前时间的公转角度
	glRotatef(90.0, -1.0, 0.0, 0.0);//自转
	gltDrawSphere(0.4, 20, 20);//画星球
	glPopMatrix();//结束绘画，刷新回主矩阵

	//绘制月亮
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);   // 材质设定
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	DrawCircle(1.0);//绘制轨道
	glBindTexture(GL_TEXTURE_2D, ImagesIDs[9]);
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0); //月球绕地球转
	glTranslatef(1.0, 0.0, 0.0);////向x轴右移
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);//当前时间的公转角
	glRotatef(90.0, -1.0, 0.0, 0.0);//自转
	gltDrawSphere(0.15, 10, 10);//画星球
	glPopMatrix();
	glPopMatrix();//结束绘画，刷新回主矩阵

	

	glutSwapBuffers();//结束绘画，刷新缓存
}