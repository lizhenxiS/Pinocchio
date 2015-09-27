
#ifndef _BVHDATA_H__
#define _BVHDATA_H__

#include "BVHPoint.h"
#include "vector.h"

class BVHData
{
private :
	int pointCount;	//���������
	int skeletonCount;	//������

	/*������ĩ�˵����������һ����*/
	BVHPoint* data;

public:

	BVHData();

	BVHData(vector<Vector3>& points, int* preIndex);

	void printData();
};

#endif	//_BVHDATA_H__
