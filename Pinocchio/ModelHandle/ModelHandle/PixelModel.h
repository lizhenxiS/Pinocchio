

#ifndef __PIXLEMODEL_H__
#define __PIXELMODEL_H__

#include <vector>
#include "vector.h"
#include "Pixel.h"
#include <map>
#include "VoxelAttribute.h"

class Mesh;
class PinocchioOutput;


class PixelModel
{
public :
	PixelModel();
	PixelModel(const PinocchioOutput* originData, Mesh* mesh, Vector3 meshMinVertex, Vector3 meshMaxVertex);

	std::vector<Pixel>* meshPixels;						//ģ�����ؼ�
	std::map<int, vector<int>> surfaceIndex;			//���������±꼯
	std::vector<VoxelAttribute> surfaceVoxelAttribute;	//����������������[0 - 16]����ӦȨ��[0 - 1]
	const double SURFACETHRESHOLD = 0.005;				//��Ƥ��ȳ���
	int surfaceVoxelCount = 0;
	const int cutCountX = 20;							//X�᷽���ϵ��и�����
	const int cutCountY = 30;							//Y�᷽���ϵ��и�����
	const int cutCountZ = 20;							//Z�᷽���ϵ��и�����
	int innerPixelCount = 0;							//ģ�������ؼ���
	Vector3 qualityCenter;								//����ģ������
	double modelVolum;									//����ģ�����
	float dx = 0.01;									//��ʼ���ش�С

};

#endif	//__PIXELMODEL_H__


