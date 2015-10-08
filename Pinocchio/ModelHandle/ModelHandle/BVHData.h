
#ifndef _BVHDATA_H__
#define _BVHDATA_H__

#include "BVHPoint.h"
#include "vector.h"

class BVHData
{
public :
	int pointCount;	//���������
	int skeletonCount;	//������

	/*������ĩ�˵����������һ����*/
	BVHPoint* data;

	/*Ĭ�ϹǼ�ģ��*/
	BVHPoint defaultSkeleton[17];
	//RelayAxes defaultRelayAxes[17];

private :
	void initDefaultSkeleton();
	//void initDefaultRelayAxes();

public:

	BVHData();

	BVHData(vector<Vector3>& points, int* preIndex);

	void printData();
};

#endif	//_BVHDATA_H__
