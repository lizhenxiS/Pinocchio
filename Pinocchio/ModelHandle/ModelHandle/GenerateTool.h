

#ifndef GENERATETOOL_H
#define GENERATETOOL_H


#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include <iostream>


double getDistance(Vector3 bone_a, Vector3 bone_b);

//计算直线绕某个轴旋转所得矩阵 4x1 4x4 4x1
void lineRotate(double* lineVector, double* rotateVector, double* resultVector);

//计算X轴旋转矩阵
void rotateInX(double angle, double* result);

//计算X轴旋转矩阵
void rotateInY(double angle, double* result);

//计算X轴旋转矩阵
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

string toString(double num);

double toDouble(string str);

#endif	//GENERATETOOL_H

