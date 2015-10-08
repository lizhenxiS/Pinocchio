

#include "GenerateTool.h"
#include <GL\glew.h>
#include <iostream>
#include <glut.h>
#include <sstream>

#define PI 3.1415926

//计算直线绕某个轴旋转所得矩阵 4x1 4x4 4x1
void lineRotate(double* lineVector, double* rotateVector, double* resultVector)
{
	resultVector[0] =
		rotateVector[0] * lineVector[0]
		+ rotateVector[1] * lineVector[1]
		+ rotateVector[2] * lineVector[2]
		+ rotateVector[3] * lineVector[3];
	resultVector[1] =
		rotateVector[4] * lineVector[0]
		+ rotateVector[5] * lineVector[1]
		+ rotateVector[6] * lineVector[2]
		+ rotateVector[7] * lineVector[3];
	resultVector[2] =
		rotateVector[8] * lineVector[0]
		+ rotateVector[9] * lineVector[1]
		+ rotateVector[10] * lineVector[2]
		+ rotateVector[11] * lineVector[3];
	resultVector[3] =
		rotateVector[12] * lineVector[0]
		+ rotateVector[13] * lineVector[1]
		+ rotateVector[14] * lineVector[2]
		+ rotateVector[15] * lineVector[3];
	//cout << "linevector " << lineVector[0] << " "
	//	<< lineVector[1] << " "
	//	<< lineVector[2] << " "
	//	<< lineVector[3] << " " << endl;
	//cout << "rotatevector " << rotateVector[0] << " "
	//	<< rotateVector[1] << " "
	//	<< rotateVector[2] << " "
	//	<< rotateVector[3] << " " << endl;
	//cout << "result!!! " << resultVector[0] << " "
	//	<< resultVector[1] << " "
	//	<< resultVector[2] << " "
	//	<< resultVector[3] << " " << endl;
}

//计算X轴旋转矩阵
void rotateInX(double angle, double* result)
{
	result[0] = 1;
	result[1] = 0;
	result[2] = 0;
	result[3] = 0;

	result[4] = 0;
	result[5] = cos(angle);
	result[6] = -sin(angle);
	result[7] = 0;

	result[8] = 0;
	result[9] = sin(angle);
	result[10] = cos(angle);
	result[11] = 0;

	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	result[15] = 1;
};

//计算Y轴旋转矩阵
void rotateInY(double angle, double* result)
{
	result[0] = cos(angle);
	result[1] = 0;
	result[2] = sin(angle);
	result[3] = 0;

	result[4] = 0;
	result[5] = 1;
	result[6] = 0;
	result[7] = 0;

	result[8] = -sin(angle);
	result[9] = 0;
	result[10] = cos(angle);
	result[11] = 0;

	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	result[15] = 1;
}

//计算Z轴旋转矩阵
void rotateInZ(double angle, double* result)
{
	result[0] = cos(angle);
	result[1] = -sin(angle);
	result[2] = 0;
	result[3] = 0;

	result[4] = sin(angle);
	result[5] = cos(angle);
	result[6] = 0;
	result[7] = 0;

	result[8] = 0;
	result[9] = 0;
	result[10] = 1;
	result[11] = 0;

	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	result[15] = 1;
}

//获得空间两点距离
double getDistance(Vector3 bone_a, Vector3 bone_b)
{
	double len;
	len = (bone_b[0] - bone_a[0])*(bone_b[0] - bone_a[0])
		+ (bone_b[1] - bone_a[1])*(bone_b[1] - bone_a[1])
		+ (bone_b[2] - bone_a[2])*(bone_b[2] - bone_a[2]);
	len = sqrt(len);

	return len;
}

//判断两条射线ab, xy是否同向
bool sameDirection(Vector3 a, Vector3 b, Vector3 x, Vector3 y)
{
	Vector3 ab;
	Vector3 xy;
	double ratio_0, ratio_1, ratio_2;

	for (int i = 0; i < 3; i++)
	{
		ab[i] = b[i] - a[i];
		xy[i] = y[i] - x[i];
		ratio_0 = ab[i] / xy[i];
	}

	if (((ratio_0 - ratio_1) <= 0.01 && (ratio_0 - ratio_1) >= 0) || ((ratio_0 - ratio_1) >= -0.01 && (ratio_0 - ratio_1) <= 0))
	{
		if (((ratio_2 - ratio_1) <= 0.01 && (ratio_2 - ratio_1) >= 0) || ((ratio_2 - ratio_1) >= -0.01 && (ratio_2 - ratio_1) <= 0))
			return true;
	}

	return false;
}

//判断一点v是否在线段ab内
bool isInLine(Vector3 v, Vector3 a, Vector3 b)
{
	if (sameDirection(a, v, a, b))
	{
		if (getDistance(a, v) <= getDistance(a, b))
			return true;
	}

	return false;
}

//refers to projection of a vertex to its nearest point on the bone Bj
Vector3 proj(Vector3 bone_a, Vector3 bone_b, Vector3 v)
{
	Vector3 projection;
	double factor;

	factor = ((v[0] - bone_a[0])*(bone_b[0] - bone_a[0])
		+ (v[1] - bone_a[1])*(bone_b[1] - bone_a[1])
		+ (v[2] - bone_a[2])*(bone_b[2] - bone_a[2])) / ((bone_b[0] - bone_a[0])*(bone_b[0] - bone_a[0])
		+ (bone_b[1] - bone_a[1])*(bone_b[1] - bone_a[1])
		+ (bone_b[2] - bone_a[2])*(bone_b[2] - bone_a[2]));

	projection[0] = bone_a[0] + factor*(bone_b[0] - bone_a[0]);
	projection[1] = bone_a[1] + factor*(bone_b[1] - bone_a[1]);
	projection[2] = bone_a[2] + factor*(bone_b[2] - bone_a[2]);

	return projection;
}

/*??????????????????*/
//the radio of the deformed shoulder length to the original value for all bones excluding the head bone
double Rj(Vector3 bone_a, Vector3 bone_b, Vector3 bone_a_, Vector3 bone_b_)
{
	return (getDistance(bone_a_, bone_b_) / getDistance(bone_a, bone_b));
}

//the stretch vector at the bone BJ
Vector3 SBj(Vector3 bone_a, Vector3 bone_b, Vector3 bone_a_, Vector3 bone_b_)
{
	Vector3 result;
	double distance = (getDistance(bone_b_, bone_a_) / getDistance(bone_b, bone_a)) - 1;

	for (int i = 0; i < 3; i++)
	{
		result[i] = distance * (bone_b[i] - bone_a[i]);
	}

	return result;
}

static int awcount = 0;

//空间中三点 ABC  获得角ABC 返回值范围为-180/180
double AngleInSpace(Vector3 pa, Vector3 pb, Vector3 pc)
{
	double lenAB = getDistance(pa, pb) * 10000;
	double lenBC = getDistance(pb, pc) * 10000;
	double lenAC = getDistance(pa, pc) * 10000;

	//余弦定理
	double cosTheta = (lenAB * lenAB + lenBC * lenBC - lenAC * lenAC) / (2 * lenAB * lenBC);
	double result = acos(cosTheta) * 180 / PI;

	//if (awcount == 0)
	//{
	//	cout << lenAB << ","
	//		<< lenBC << ","
	//		<< lenAC << ","
	//		<< cosTheta << ","
	//		<< result << endl;
	//	awcount++;
	//}

	return result;
}


//joint weight
double EBjVi(Vector3 bone_a, Vector3 bone_b, Vector3 v)
{
	return (getDistance(proj(bone_a, bone_b, v), bone_a) / getDistance(bone_b, bone_a));
}

void drawLine(Vector3& lina, Vector3& linb, Vector3& color, double thick)
{
	glColor3f(color[0], color[1], color[2]);
	glLineWidth(thick);
	glBegin(GL_LINES);
	glVertex3f(lina[0], lina[1], lina[2]);
	glVertex3f(linb[0], linb[1], linb[2]);
	glEnd();
}

void drawPoint(Vector3 point, double radius, Vector3 color)
{
	glColor3d(color[0], color[1], color[2]);
	glTranslated(point[0], point[1], point[2]);
	glutSolidSphere(radius, 3, 3);
	glTranslated(-point[0], -point[1], -point[2]);
}

string toString(double num)
{
	stringstream ss;
	ss << num;
	string result;
	ss >> result;

	return result;
}

double toDouble(string str)
{
	stringstream ss;
	ss << str;
	double result;
	ss >> result;

	return result;
}
