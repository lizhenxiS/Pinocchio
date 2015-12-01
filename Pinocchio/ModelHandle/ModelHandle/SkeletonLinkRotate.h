

#ifndef _SKELETONLINKROTATE_H__
#define _SKELETONLINKROTATE_H__


#include "mesh.h"
#include "MyVerticeMesh.h"
#include "pinocchioApi.h"
#include "SkeletonNode.h"
#include "SkeletonInfo.h"
#include "OnceRotation.h"

#define BONECOUNT 17
class PixelModel;

/*
����ģ�͹�����ת������ģ����ת����
*/
class SkeletonLinkRotate
{
public :
	int pointCount;	//���������
	int skeletonCount;	//������
	SkeletonNode* skeletonNodeInformation;	//���ù�����Ϣ����
	SkeletonInfo skeletonInfo[BONECOUNT];	//�������ɶ���Ϣ
	Vector3 skeletonPoints[BONECOUNT + 1];	//����λ����Ϣ
	MyVerticeMesh* meshVertices;			//ģ�͵�λ����Ϣ
	PinocchioOutput weightData;				//weight�ȸ�����Ϣ
	OnceRotation linkRotate[BONECOUNT][2];	//������ת����ת����Ϣ

	void initDefaultSkeleton();	//�����ܽǶ���Ϣ
	void rotateSkeleton(const int bone, const vector<Vector3>& points, const double alpha, const double beta);	//��ĳһ��������ת�����ռ���ת����
	void clearRotateVector();	//�����ת����
	void refreshMesh(const MyVerticeMesh* m);			//��ת������º�ˢ��ģ�� pointNodeΪ��ת���ĵ㣬������������ʼ��
	void refreshMesh(const Mesh* m);					//��ת������º�ˢ��ģ�� pointNodeΪ��ת���ĵ㣬������������ʼ��
	void refreshVoxel(int verticeCount, PixelModel* pixelModel);	//��ת������º�ˢ������ pointNodeΪ��ת���ĵ㣬������������ʼ��(ͬʱ��������ģ�����ġ����)

	SkeletonLinkRotate();
	SkeletonLinkRotate(const PinocchioOutput weightData, const vector<Vector3>& points,
		 SkeletonNode* skeletonNodeInformation);
	~SkeletonLinkRotate();
};

#endif	//_SKELETONLINKROTATE_H__