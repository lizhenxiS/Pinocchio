
#ifndef __VOXELATTRIBUTE_H__
#define __VOXELATTRIBUTE_H__

class VoxelAttribute
{
public :
	int belongBone;			//������������[0-16]
	double belongWeight;	//���ش���Ȩ��
	int linkVertex;			//���������±�
	int belongIndex;		//�������㸽�����ؼ��±�

	VoxelAttribute();
	VoxelAttribute(int belongBone, double belongWeight, int linkVertex, int belongIndex);
};


#endif	//__VOXELATTRIBUTE_H__

