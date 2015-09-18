


#ifndef WIW_H
#define WIW_H

#include <QtOpenGL\QGLWidget>
#include <QtOpenGL\qglfunctions.h>
#include <QtCore\qbasictimer.h>
#include "GenerateMesh.h"


class W : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT

public :
	W(QWidget *parent = 0);
	~W();

protected :
	void timerEvent(QTimerEvent *e);
	void mouseEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void SetCamera(GLdouble x, GLdouble y);

	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

public :
	void inputFileName(const string& file);
	GenerateMesh* model;

private :
	QBasicTimer timer;
	double y_rotate;
	string fileName;

};


#endif


