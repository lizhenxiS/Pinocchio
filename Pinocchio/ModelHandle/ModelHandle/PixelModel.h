

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
	bool isInModel(float* pixelPoint, Vector3 vertex, Vector3 vertexNormal);		//判断当前位置体素是否在模型点内

	std::vector<Pixel>* meshPixels;		//模型体素集
	const int cutCountX = 20;			//X轴方向上的切割数量
	const int cutCountY = 30;			//Y轴方向上的切割数量
	const int cutCountZ = 20;			//Z轴方向上的切割数量
	int innerPixelCount = 0;			//模型内体素计数

};

#endif	//__PIXELMODEL_H__


