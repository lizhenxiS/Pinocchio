

#ifndef _STANDARDMODEL_H__
#define _STANDARDMODEL_H__

#include "BVHPoint.h"
#include "mesh.h"
#include "MyVerticeMesh.h"
#include "pinocchioApi.h"
#include "SkeletonNode.h"

#define BONECOUNT 17

/*
ģ�ͱ����
ģ�͵���ʱ��ȡģ�ͳ�ʼ�������ȼ�mesh��Ϣ
����׼��������ȡ�ı�׼��̬ģ��
*/
class StandardModel
{
public :
	int pointCount;	//���������
	int skeletonCount;	//������
	SkeletonNode* skeletonNodeInformation;	//���ù�����Ϣ����
	BVHPoint skeletonData[BONECOUNT];	//ÿ����ܹ���ʵ����������ļн�
	Vector3 skeletonPoints[BONECOUNT + 1];
	MyVerticeMesh* meshVertices;
	PinocchioOutput modelData;
	double rotateVector[BONECOUNT * 3][16];

	void initDefaultSkeleton();	//�����ܽǶ���Ϣ
	void setSkeletonLengthData(const vector<Vector3>& points);	//�����ܹ���������Ϣ
	void standardMesh(const vector<Vector3>& points);	//ģ�ͱ�ܻ�
	void rotateSkeleton(const int bone, const vector<Vector3>& points);	//���ĳһ��������ת����ת����
	void rotateMesh(double* rotateVector);	//��תģ�� 16x1
	void clearRotateVector();	//�����ת����
	void refreshMesh(Vector3 pointNode);	//��ת������º�ˢ��ģ�� pointNodeΪ��ת���ĵ㣬������������ʼ��
	void outputRotateVector();	//�����ת����

	StandardModel();
	StandardModel(const PinocchioOutput modelData, const Mesh* m, const vector<Vector3>& points,
		SkeletonNode* skeletonNodeInformation);
};

#endif	//_STANDARDMODEL_H__