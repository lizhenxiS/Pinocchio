

#ifndef __PIXLEMODEL_H__
#define __PIXELMODEL_H__

#include <vector>
#include "pinocchioApi.h"
#include "Pixel.h"

class PixelModel
{
public :
	PixelModel();
	PixelModel(Mesh* mesh, Vector3 meshMinVertex, Vector3 meshMaxVertex);
	bool isInModel(float* pixelPoint, Vector3 vertex, Vector3 vertexNormal);		//�жϵ�ǰλ�������Ƿ���ģ�͵���

	std::vector<Pixel>* meshPixels;		//ģ�����ؼ�
	const int cutCountX = 20;			//X�᷽���ϵ��и�����
	const int cutCountY = 30;			//Y�᷽���ϵ��и�����
	const int cutCountZ = 20;			//Z�᷽���ϵ��и�����
	int innerPixelCount = 0;			//ģ�������ؼ���

};

#endif	//__PIXELMODEL_H__


