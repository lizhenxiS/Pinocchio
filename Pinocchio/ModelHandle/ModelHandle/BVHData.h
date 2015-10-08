
#ifndef _BVHDATA_H__
#define _BVHDATA_H__

#include "BVHPoint.h"
#include "vector.h"

class BVHData
{
public :
	int pointCount;	//骨骼结点数
	int skeletonCount;	//骨骼数

	/*将骨骼末端点与骨骼连在一起考虑*/
	BVHPoint* data;

	/*默认骨架模型*/
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
