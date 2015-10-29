

#include "GenerateTool.h"
#include <GL\glew.h>
#include <iostream>
#include <glut.h>
#include <sstream>

#define PI 3.1415926

//获得两个vector3 的叉乘
Vector3 getCrossProduct(Vector3 a, Vector3 b)
{
	return Vector3(a[1] * b[2] - a[2] * b[1],
		a[2] * b[0] - a[0] * b[2],
		a[0] * b[1] - a[1] * b[0]);
}

//获得两个vector3 的点乘
double getPointProduct(Vector3 a, Vector3 b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

//将矩阵单位化
Vector3 normalize(Vector3 vec)
{
	double sum = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	double sqrtSum = sqrt(sum);
	return Vector3(vec[0] / sqrtSum, vec[1] / sqrtSum, vec[2] / sqrtSum);
}

//点绕旋转轴旋转一定角度后的结果 3x1 3x1 3x1 角度为0-180
Vector3 rotateArroundLine(Vector3 point, Vector3 lineStart, Vector3 lineEnd, double angle)
{
	if (almostEqual(angle, 0))
		return point;

	double cosAngle = cos(angle * PI / 180);
	double sinAngle = sin(angle * PI / 180);
	Vector3 localPoint = point - lineStart;
	Vector3 lineVector = lineEnd - lineStart;
	lineVector = normalize(lineVector);
	/*
	结果来自 http://www.cnblogs.com/graphics/archive/2012/08/10/2627458.html [未单位化]
	http://blog.csdn.net/tan625747/article/details/5523728
	*/
	double a = lineVector[0];
	double b = lineVector[1];
	double c = lineVector[2];
	double aa = a * a;
	double bb = b * b;
	double cc = c * c;
	double ab = a * b;
	double bc = b * c;
	double ac = a * c;
	double localRotateVector[16] = {
		aa + (1 - aa) * cosAngle, ab * (1 - cosAngle) + c * sinAngle, ac * (1 - cosAngle) - b * sinAngle, 0,
		ab * (1 - cosAngle) - c * sinAngle, bb + (1 - bb) * cosAngle, bc * (1 - cosAngle) + a * sinAngle, 0,
		ac * (1 - cosAngle) + b * sinAngle, bc * (1 - cosAngle) - a * sinAngle, cc + (1 - cc) * cosAngle, 0,
		0, 0, 0, 1
	};
	Vector3 localResult = Vector3(
		localRotateVector[0] * localPoint[0] + localRotateVector[1] * localPoint[1] + localRotateVector[2] * localPoint[2],
		localRotateVector[4] * localPoint[0] + localRotateVector[5] * localPoint[1] + localRotateVector[6] * localPoint[2],
		localRotateVector[8] * localPoint[0] + localRotateVector[9] * localPoint[1] + localRotateVector[10] * localPoint[2]
		);
	Vector3 result = localResult + lineStart;
	//cout << "point       : " << point << endl;
	//cout << "linestart   : " << lineStart << endl;
	//cout << "lineend     : " << lineEnd << endl;
	//cout << "linenormal  : " << lineVector << endl;
	//cout << "localpoint  : " << localPoint << endl;
	//cout << "localresult : " << localResult << endl;
	//cout << "angle       : " << angle << endl;
	//cout << localRotateVector[0] << " , " << localRotateVector[1] << " , " << localRotateVector[2] << " , " << localRotateVector[3] << endl;
	//cout << localRotateVector[4] << " , " << localRotateVector[5] << " , " << localRotateVector[6] << " , " << localRotateVector[7] << endl;
	//cout << localRotateVector[8] << " , " << localRotateVector[9] << " , " << localRotateVector[10] << " , " << localRotateVector[11] << endl;
	//cout << localRotateVector[12] << " , " << localRotateVector[13] << " , " << localRotateVector[14] << " , " << localRotateVector[15] << endl;
	//cout << "result      : " << result << endl;
	return result;
}

//计算某个骨骼点牵连的所有后续骨骼点下标
vector<int> getAfterSkeletonIndex(SkeletonNode* skeletonInformation, int boneNode)
{
	vector<int> result;
	for (int i = 0; i < skeletonInformation[boneNode].childrenIndex.size(); i++)
	{
		int temp = skeletonInformation[boneNode].childrenIndex[i];
		result.push_back(temp);
		if (skeletonInformation[temp].childrenIndex.size() != 0)
		{
			vector<int> childTemp = getAfterSkeletonIndex(skeletonInformation, temp);
			for (int j = 0; j < childTemp.size(); j++)
			{
				result.push_back(childTemp[j]);
			}
		}
	}

	return result;
}

//计算直线绕某个轴旋转所得矩阵 4x1 16x1 4x1
Vector3 lineRotate(Vector3 lineVector, double* rotateVector)
{
	double line[4] = { lineVector[0], lineVector[1], lineVector[2], 0 };
	double result[4];
	lineRotate(line, rotateVector, result);
	return Vector3(result[0], result[1], result[2]);
}

//计算直线绕某个轴旋转所得矩阵 4x1 16x1 4x1
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

//计算直线根据旋转矩阵获得旋转结果	直线起点pa 3x1 直线末端点pb 3x1 旋转矩阵 16x1 【旋转骨骼需要：pa保持不变】
Vector3 lineRotate(Vector3 pa, Vector3 pb, double* rotateVector)
{
	//cout << "before linerotate :" << getDistance(pa, pb) << endl; 
	double lineVector[4] = {
		pb[0] - pa[0],
		pb[1] - pa[1],
		pb[2] - pa[2],
		0
	};
	double resultVector[4];
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

	Vector3 resultPoint;
	resultPoint[0] = pa[0] + resultVector[0];
	resultPoint[1] = pa[1] + resultVector[1];
	resultPoint[2] = pa[2] + resultVector[2];

	//cout << "after linerotate :" << getDistance(pa, resultPoint) << endl;
	return resultPoint;
}

//计算点根据旋转矩阵获得结果【围绕原点、坐标轴旋转】
Vector3 pointRotate(Vector3 point, double* rotateVector)
{
	Vector3 result;
	result[0] = rotateVector[0] * point[0]
		+ rotateVector[1] * point[1]
		+ rotateVector[2] * point[2];
	result[1] = rotateVector[4] * point[0]
		+ rotateVector[5] * point[1]
		+ rotateVector[6] * point[2];
	result[2] = rotateVector[8] * point[0]
		+ rotateVector[9] * point[1]
		+ rotateVector[10] * point[2];
	return result;
}

//计算点根据center点依据旋转矩阵获得结果 3x1 3x1 4x1
Vector3 pointRotate(Vector3 center, Vector3 point, double* rotateVector)
{
	double lineVector[4] = {
		point[0] - center[0],
		point[1] - center[1],
		point[2] - center[2],
		0
	};
	double resultVector[4];
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

	Vector3 resultPoint;
	resultPoint[0] = center[0] + resultVector[0];
	resultPoint[1] = center[1] + resultVector[1];
	resultPoint[2] = center[2] + resultVector[2];

	return resultPoint;
}

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 3x1 3x1
double transInX(Vector3 origin, Vector3 later)
{
	double originV[4] = { origin[0], origin[1], origin[2], 0 };
	double laterV[4] = { later[0], later[1], later[2], 0 };
	return transInX(originV, laterV);
}

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInX(double* origin, double* later)
{
	double angle;

	/*检查输入是否有解*/
	if (!almostEqual(origin[0], later[0]))
		cout << "两矩阵不能绕X轴旋转得到" << endl;

	if (later[0] == 0
		&& later[1] == 0
		&& later[2] == 0)
		return 0;

	double sinangle = (origin[2] * later[1] - origin[1] * later[2]) / (later[1] * later[1] + later[2] * later[2]);
	angle = asin(sinangle);
	return angle;
}

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInY(Vector3 origin, Vector3 later)
{
	double originV[4] = { origin[0], origin[1], origin[2], 0 };
	double laterV[4] = { later[0], later[1], later[2], 0 };
	return transInY(originV, laterV);
}

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInY(double* origin, double* later)
{
	double angle;

	/*检查输入是否有解*/
	if (!almostEqual(origin[1], later[1]))
		cout << "两矩阵不能绕Y轴旋转得到" << endl;

	if (later[0] == 0
		&& later[1] == 0
		&& later[2] == 0)
		return 0;

	double sinangle = (origin[0] * later[2] - origin[2] * later[0]) / (later[0] * later[0] + later[2] * later[2]);
	angle = asin(sinangle);
	return angle;
}

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInZ(Vector3 origin, Vector3 later)
{
	double originV[4] = { origin[0], origin[1], origin[2], 0 };
	double laterV[4] = { later[0], later[1], later[2], 0 };
	return transInZ(originV, laterV);
}

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInZ(double* origin, double* later)
{
	double angle;

	/*检查输入是否有解*/
	if (!almostEqual(origin[2], later[2]))
		cout << "两矩阵不能绕Z轴旋转得到" << endl;

	if (later[0] == 0
		&& later[1] == 0
		&& later[2] == 0)
		return 0;

	double sinangle = (origin[1] * later[0] - origin[0] * later[1]) / (later[0] * later[0] + later[1] * later[1]);
	angle = asin(sinangle);
	return angle;
}

//以0.05为临界值判断两个数是否近似相等
bool almostEqual(double a, double b)
{
	if ((a >= b && a <= b + 0.05)
		|| (a <= b && a >= b - 0.05))
		return true;
	else
		return false;
}

//自定义临界值判断两个数是否近似相等
bool almostEqual(double a, double b, double threshold)
{
	if ((a >= b && a <= b + threshold)
		|| (a <= b && a >= b - threshold))
		return true;
	else
		return false;
}


//计算X轴旋转矩阵 16x1
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

void drawConverxHull(vector<Vector3> convertxHull, Vector3 color)
{
	glColor3d(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	for (int i = 0; i < convertxHull.size(); i++)
	{
		glVertex3d(convertxHull[i][0], convertxHull[i][1], convertxHull[i][2]);
	}
	glEnd();
}

void drawSquare(Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Vector3 color)
{
	glColor3d(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex3d(pa[0], pa[1], pa[2]);
	glVertex3d(pb[0], pb[1], pb[2]);
	glVertex3d(pc[0], pc[1], pc[2]);
	glVertex3d(pd[0], pd[1], pd[2]);
	glEnd();
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
