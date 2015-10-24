

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

//������ת����תһ���ǶȺ�Ľ��
Vector3 rotateArroundLine(Vector3 point, Vector3 lineStart, Vector3 lineEnd, double angle);

//������λ��
Vector3 normalize(Vector3 vec);

//�������vector3 �Ĳ��
Vector3 getCrossProduct(Vector3 a, Vector3 b);

//��0.05Ϊ�ٽ�ֵ�ж��������Ƿ�������
bool almostEqual(double a, double b);

//�Զ����ٽ�ֵ�ж��������Ƿ�������
bool almostEqual(double a, double b, double threshold);

//���������ʼ���󡢽��������ת�� ȷ����ת�� 3x1 3x1
double transInX(Vector3 origin, Vector3 later);

//���������ʼ���󡢽��������ת�� ȷ����ת�� 4x1 4x1
double transInX(double* origin, double* later);

//���������ʼ���󡢽��������ת�� ȷ����ת�� 4x1 4x1
double transInY(Vector3 origin, Vector3 later);

//���������ʼ���󡢽��������ת�� ȷ����ת�� 4x1 4x1
double transInY(double* origin, double* later);

//���������ʼ���󡢽��������ת�� ȷ����ת�� 4x1 4x1
double transInZ(Vector3 origin, Vector3 later);

//���������ʼ���󡢽��������ת�� ȷ����ת�� 4x1 4x1
double transInZ(double* origin, double* later);

//����ĳ��������ǣ�������к����������±�
vector<int> getAfterSkeletonIndex(SkeletonNode* skeletonInformation, int boneNode);

//����ֱ����ĳ������ת���þ��� 4x1 16x1 4x1
void lineRotate(double* lineVector, double* rotateVector, double* resultVector);

//����ֱ����ĳ������ת���þ��� 3x1 16x1 3x1
Vector3 lineRotate(Vector3 lineVector, double* rotateVector);

//����ֱ�߸�����ת��������ת���	ֱ�����pa 3x1 ֱ��ĩ�˵�pb 3x1 ��ת���� 16x1 ����ת������Ҫ��pa���ֲ��䡿
Vector3 lineRotate(Vector3 pa, Vector3 pb, double* rotateVector);

//��������center��������ת�����ý�� 3x1 3x1 4x1
Vector3 pointRotate(Vector3 center, Vector3 point, double* rotateVector);

//����������ת�����ý����Χ��ԭ�㡢��������ת��
Vector3 pointRotate(Vector3 point, double* rotateVector);

//����X����ת���� 16x1
void rotateInX(double angle, double* result);

//����Y����ת���� 16x1
void rotateInY(double angle, double* result);

//����Z����ת���� 16x1
void rotateInZ(double angle, double* result);

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

void drawConverxHull(vector<Vector3> convertxHull, Vector3 color);

void drawSquare(Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Vector3 color);

string toString(double num);

double toDouble(string str);

#endif	//GENERATETOOL_H

