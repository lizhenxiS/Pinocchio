

#ifndef GENERATETOOL_H
#define GENERATETOOL_H


#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"



double getDistance(Vector3 bone_a, Vector3 bone_b);

//�ж���������ab, xy�Ƿ�ͬ��
bool sameDirection(Vector3 a, Vector3 b, Vector3 x, Vector3 y);

//�ռ������� ABC  ��ý�ABC ����ֵ��ΧΪ-180/180
double AngleInSpace(Vector3 pa, Vector3 pb, Vector3 pc);

//�ж�һ��v�Ƿ����߶�ab��
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

#endif	//GENERATETOOL_H

