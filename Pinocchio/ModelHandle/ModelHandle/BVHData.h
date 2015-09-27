
#ifndef _BVHDATA_H__
#define _BVHDATA_H__

#include "BVHPoint.h"
#include "vector.h"

class BVHData
{
private :
	int pointCount;	//骨骼结点数
	int skeletonCount;	//骨骼数

	/*将骨骼末端点与骨骼连在一起考虑*/
	BVHPoint* data;

public:

	BVHData();

	BVHData(vector<Vector3>& points, int* preIndex);

	void printData();
};

#endif	//_BVHDATA_H__
