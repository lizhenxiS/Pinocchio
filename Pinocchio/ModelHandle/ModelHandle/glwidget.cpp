


#include <GL\glew.h>
#include <iostream>
#include <glut.h>
#include <string>
#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include <fstream>
#include "glwidget.h"
#include <QtOpenGL\qgl.h>
#include <QtGui\QMouseEvent>

using namespace std;

#define DEG_TO_RAD 0.017453
#define PI 3.1415926535898
#define GLWIN_WIDTH 600
#define GLWIN_HEIGHT 600


GLdouble modelView[16];
GLdouble projView[16];
GLint viewView[4];

struct MOUSE_POINT
{
	int x;
	int y;
};

struct CAMERA
{
	GLdouble xeye;
	GLdouble yeye;
	GLdouble zeye;
};

struct POLAR
{
	GLdouble r;
	GLdouble alpha;
	GLdouble fy;
};

MOUSE_POINT oldpt;
MOUSE_POINT curpt;
CAMERA camera;
POLAR polar;


void Draw_Circle(MOUSE_POINT s)
{
	glColor3f(0.0, 0.0, 0.5);
	double r = 1.0;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 1000; i++)
	{
		glVertex2f(s.x, s.y);
		glVertex2f(s.x + r*cos((double)i / PI), s.y + r*sin((double)i / PI));
	}
	glEnd();
}


GLWidget::GLWidget(QWidget *parent)
: QGLWidget(parent)
{
	drawSkeleton = true;
	drawMesh = true;
	oldpt.x = -1;
	oldpt.y = -1;
	polar.r = 3.0;
	polar.alpha = 60.0;
	polar.fy = 45.0;
	camera.xeye = 0;
	camera.yeye = 0;
	camera.zeye = 3.0;
	
	initArcBallMatrix();
	setMouseTracking(true);
}

void GLWidget::initArcBallMatrix()
{
	arcBallMatrix[0] = 1;
	arcBallMatrix[1] = 0;
	arcBallMatrix[2] = 0;
	arcBallMatrix[3] = 0;

	arcBallMatrix[4] = 0;
	arcBallMatrix[5] = 1;
	arcBallMatrix[6] = 0;
	arcBallMatrix[7] = 0;

	arcBallMatrix[8] = 0;
	arcBallMatrix[9] = 0;
	arcBallMatrix[10] = 1;
	arcBallMatrix[11] = 0;

	arcBallMatrix[12] = 0;
	arcBallMatrix[13] = 0;
	arcBallMatrix[14] = 0;
	arcBallMatrix[15] = 1;
}

GLWidget::~GLWidget()
{

}

void GLWidget::timerEvent(QTimerEvent *)
{
	updateGL();
}

void GLWidget::mouseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		curpt.x = e->pos().x();
		curpt.y = e->pos().y();
	}
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		oldpt.x = e->pos().x();
		oldpt.y = e->pos().y();
		left_button_down = true;
	}

	if (e->button() == Qt::RightButton)
	{
		//寻找范围内的点
		model->FindVertex(e->pos().x(), height() - e->pos().y());
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		left_button_down = false;
	}
	
}

void GLWidget::setArcBallMatrix(double old_x, double old_y, double new_x, double new_y)
{

}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (left_button_down)
	{
		SetCamera(GLdouble(e->pos().x() - oldpt.x), GLdouble(oldpt.y - e->pos().y()));       /*根据增量重新设定摄像机位置*/

		oldpt.x = e->pos().x();              /*将当前坐标置为旧坐标*/
		oldpt.y = e->pos().y();
	}
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
	if (e->angleDelta().y() > 0)
	{
		angle += 5.0;
		if (angle > 175)
			angle = 175;
	}
	else
	{
		angle -= 5.0;
		if (angle < 5)
			angle = 5.0;
	}
}

//GLfloat light_position[] = { 1.0, -1.0, 1.0, 0.0 };
GLfloat light_position[] = { 0, 0, 3.0, 0.0 };

void GLWidget::SetCamera(GLdouble x, GLdouble y)
{
	GLfloat alpha, fy;                  /*和它的名字一样，不过是单位是弧度*/
	if ((polar.fy + y)>5.0f && (polar.fy + y)<175.0f)
	{     /*定义竖直偏角只能在5°到175°之间*/
		polar.alpha += x;                  /*根据鼠标移动的方向设置新的球坐标*/
		polar.fy += y;

		if (polar.alpha>360.0f) polar.alpha -= 360.0f;
		if (polar.alpha<0.0f) polar.alpha += 360.0f;      /*将水平偏角锁定在0°到360°之间*/

		alpha = polar.alpha*DEG_TO_RAD;
		fy = polar.fy*DEG_TO_RAD;               /*角度转弧度*/
		camera.xeye = polar.r * sin(fy) * cos(alpha);       /*极坐标转直角坐标*/
		camera.zeye = polar.r * sin(fy) * sin(alpha);
		camera.yeye = polar.r * cos(fy);                 /*注意：竖直方向的是y轴*/

		light_position[0] = camera.xeye;
		light_position[1] = camera.yeye;
		light_position[2] = camera.zeye;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);	//光源位置

		paintGL();                     /*每次改变相机位置时重绘*/
		/*当然，我画的是静态的，若景物是动态的，你必须将重绘放在空闲回调函数中*/
	}
}


void GLWidget::inputFileName(const string& file)
{
	fileName = file;
	initializeGL();
}

void GLWidget::initializeGL()
{

	if (fileName.size() == 0)
		return;

	if (hasModel)
	{
		delete model;
		hasModel = false;
	}

	initializeGLFunctions();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	angle = 60.0;

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat white_light[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);	//光源位置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);		//光的散射强度
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);		//光的镜面强度

	//GLfloat secondPos[4] = { -light_position[0],
	//	-light_position[1], -light_position[2], -light_position[3] };
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position);	//光源位置
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);		//光的散射强度
	//glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);		//光的镜面强度

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	model = new GenerateMesh(fileName);
	hasModel = true;

	glShadeModel(GL_SMOOTH);
	timer.start(12, this);
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

double g_projMatrix[1000][16];
double g_modelviewMatrix[1000][16];
int g_matrixCount = 0;

bool isDiff(double* m1, double* m2)
{
	bool bret = false;
	for (int i = 0; i < 16; ++i)
	{
		if (fabs(m1[i] - m2[i]) > 1e-6)
		{
			bret = true;
			break;
		}
	}
	return bret;
}

void DrawCoordinary()
{
	glLineWidth(5.0);
	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(10, 0, 0);
	glEnd();
	glColor3d(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 10, 0);
	glEnd();
	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 10);
	glEnd();
}

void GLWidget::paintGL()
{
	if (fileName.size() == 0)
		return;

	if (drawMesh)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, 1.0, 1.0, 20.0);
	
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/**********使相机围绕物体中心旋转，好像有问题？？**********/
	Vector3 center = model->getModelCenter();
	//glTranslated(center[0], center[1], center[2]);
	gluLookAt(camera.xeye, camera.yeye, camera.zeye, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(camera.xeye, camera.yeye, camera.zeye, center[0], center[1], center[2], 0.0, 1.0, 0.0);
	//gluLookAt(0, 0, 3.0, 0, 0, 0, 0, 1, 0);
	//glTranslated(-center[0], -center[1], -center[2]);
	/********************/
	
	/*初始（0，0）为屏幕中间，现移动到左下*/
	glTranslated(-0.5, -0.5, 0.0);

	glRotated((GLdouble)rotateX, 1.0, 0.0, 0.0);
	glRotated((GLdouble)rotateY, 0.0, 1.0, 0.0);
	glRotated((GLdouble)rotateZ, 0.0, 0.0, 1.0);
	

	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projView);
	glGetIntegerv(GL_VIEWPORT, viewView);

	if (drawSkeleton)
	{
		model->drawSkeleton();
		//model->drawSkeletonPoint();
	}
	if (drawMesh)
	{
		model->drawMesh();
	}
	if (drawCoord)
	{
		DrawCoordinary();
	}
	if (drawVoxel)
	{
		model->drawVoxel();
	}
				
	//Draw_Circle(curpt);
	

	glFlush();
}






