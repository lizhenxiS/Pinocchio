

#ifndef __PIXLEMODEL_H__
#define __PIXELMODEL_H__

#include <vector>
#include "pinocchioApi.h"
#include "Pixel.h"
#include <map>

class PixelModel
{
public :
	PixelModel();
	PixelModel(Mesh* mesh, Vector3 meshMinVertex, Vector3 meshMaxVertex);

	std::vector<Pixel>* meshPixels;				//ģ�����ؼ�
	std::map<int, vector<int>> surfaceIndex;	//���������±꼯
	//vector<int, double> surfaceVoxelAttribute;	//����������������[0 - 16]����ӦȨ��[0 - 1]
	const double SURFACETHRESHOLD = 0.005;		//��Ƥ��ȳ���
	int surfaceVoxelCount = 0;
	const int cutCountX = 20;					//X�᷽���ϵ��и�����
	const int cutCountY = 30;					//Y�᷽���ϵ��и�����
	const int cutCountZ = 20;					//Z�᷽���ϵ��и�����
	int innerPixelCount = 0;					//ģ�������ؼ���
	Vector3 qualityCenter;						//����ģ������
	double modelVolum;							//����ģ�����

};

#endif	//__PIXELMODEL_H__


