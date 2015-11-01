
#ifndef __VOXELATTRIBUTE_H__
#define __VOXELATTRIBUTE_H__

class VoxelAttribute
{
public :
	int belongBone;			//体素所属骨骼[0-16]
	double belongWeight;	//体素从属权重
	int linkVertex;			//从属顶点下标
	int belongIndex;		//从属顶点附属体素集下标

	VoxelAttribute();
	VoxelAttribute(int belongBone, double belongWeight, int linkVertex, int belongIndex);
};


#endif	//__VOXELATTRIBUTE_H__

