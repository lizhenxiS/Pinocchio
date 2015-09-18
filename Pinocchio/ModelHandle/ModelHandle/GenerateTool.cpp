

#include "GenerateTool.h"
#include <GL\glew.h>
#include <iostream>
#include <glut.h>

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
