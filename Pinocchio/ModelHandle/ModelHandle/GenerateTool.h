

#ifndef GENERATETOOL_H
#define GENERATETOOL_H


#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include <iostream>
#include <conio.h>
#include "SkeletonNode.h"

#define SELFDEBUG cout << "enter"<< endl; getch();cout << "pass" << endl;

double getDistance(Vector3 bone_a, Vector3 bone_b);

//点绕旋转轴旋转一定角度后的结果
Vector3 rotateArroundLine(Vector3 point, Vector3 lineStart, Vector3 lineEnd, double angle);

//将矩阵单位化
Vector3 normalize(Vector3 vec);

//获得两个vector3 的叉乘
Vector3 getCrossProduct(Vector3 a, Vector3 b);

//以0.05为临界值判断两个数是否近似相等
bool almostEqual(double a, double b);

//自定义临界值判断两个数是否近似相等
bool almostEqual(double a, double b, double threshold);

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 3x1 3x1
double transInX(Vector3 origin, Vector3 later);

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInX(double* origin, double* later);

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInY(Vector3 origin, Vector3 later);

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInY(double* origin, double* later);

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInZ(Vector3 origin, Vector3 later);

//反向根据起始矩阵、结果矩阵、旋转轴 确定旋转角 4x1 4x1
double transInZ(double* origin, double* later);

//计算某个骨骼点牵连的所有后续骨骼点下标
vector<int> getAfterSkeletonIndex(SkeletonNode* skeletonInformation, int boneNode);

//计算直线绕某个轴旋转所得矩阵 4x1 16x1 4x1
void lineRotate(double* lineVector, double* rotateVector, double* resultVector);

//计算直线绕某个轴旋转所得矩阵 3x1 16x1 3x1
Vector3 lineRotate(Vector3 lineVector, double* rotateVector);

//计算直线根据旋转矩阵获得旋转结果	直线起点pa 3x1 直线末端点pb 3x1 旋转矩阵 16x1 【旋转骨骼需要：pa保持不变】
Vector3 lineRotate(Vector3 pa, Vector3 pb, double* rotateVector);

//计算点根据center点依据旋转矩阵获得结果 3x1 3x1 4x1
Vector3 pointRotate(Vector3 center, Vector3 point, double* rotateVector);

//计算点根据旋转矩阵获得结果【围绕原点、坐标轴旋转】
Vector3 pointRotate(Vector3 point, double* rotateVector);

//计算X轴旋转矩阵 16x1
void rotateInX(double angle, double* result);

//计算Y轴旋转矩阵 16x1
void rotateInY(double angle, double* result);

//计算Z轴旋转矩阵 16x1
void rotateInZ(double angle, double* result);

//判断两条射线ab, xy是否同向
bool sameDirection(Vector3 a, Vector3 b, Vector3 x, Vector3 y);

//空间中三点 ABC  获得角ABC 返回值范围为-180/180
double AngleInSpace(Vector3 pa, Vector3 pb, Vector3 pc);

//判断一点v是否在线段ab内
bool isInLine(Vector3 v, Vector3 a, Vector3 b);

//refers to projection of a vertex to its nearest point on the bone Bj
Vector3 proj(Vector3 bone_a, Vector3 bone_b, Vector3 v);

/*??????????????????*/
//the radio of the deformed shoulder length to the original value for all bones excluding the head bone
double Rj(Vector3 bone_a, Vector3 bone_b, Vector3 bone_a_, Vector3 bone_b_);

//the stretch vector at the bone BJ
Vector3 SBj(Vector3 bone_a, Vector3 bone_b, Vector3 bone_a_, Vector3 bone_b_);

//joint weight
double EBjVi(Vector3 bone_a, Vector3 bone_b, Vector3 v);

void drawLine(Vector3& lina, Vector3& linb, Vector3& color, double thick);

void drawPoint(Vector3 point, double radius, Vector3 color);

void drawConverxHull(vector<Vector3> convertxHull, Vector3 color);

void drawSquare(Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Vector3 color);

string toString(double num);

double toDouble(string str);

#endif	//GENERATETOOL_H

