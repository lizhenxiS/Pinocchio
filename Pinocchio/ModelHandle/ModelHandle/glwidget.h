


#ifndef WIW_H
#define WIW_H

#include <QtOpenGL\QGLWidget>
#include <QtOpenGL\qglfunctions.h>
#include <QtCore\qbasictimer.h>
#include "GenerateMesh.h"


class GLWidget : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT

public :
	GLWidget(QWidget *parent = 0);
	~GLWidget();

protected :
	void timerEvent(QTimerEvent *e);
	void mouseEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void SetCamera(GLdouble x, GLdouble y);

	void initArcBallMatrix();
	void setArcBallMatrix(double old_x, double old_y, double new_x, double new_y);
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();


public :
	void inputFileName(const string& file);
	GenerateMesh* model;
	bool drawSkeleton;
	bool drawMesh;
	bool drawCoord;
	bool drawVoxel;
	int rotateX = 0;
	int rotateY = 0;
	int rotateZ = 0;
	float angle;

private :
	QBasicTimer timer;
	double y_rotate;
	string fileName;
	GLdouble arcBallMatrix[16];
	bool hasModel = false;
	bool left_button_down = false;

};


#endif


