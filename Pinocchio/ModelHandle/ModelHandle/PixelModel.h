

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

	std::vector<Pixel>* meshPixels;				//模型体素集
	std::map<int, vector<int>> surfaceIndex;	//表面体素下标集
	//vector<int, double> surfaceVoxelAttribute;	//表面体素所属骨骼[0 - 16]及对应权重[0 - 1]
	const double SURFACETHRESHOLD = 0.005;		//表皮厚度常量
	int surfaceVoxelCount = 0;
	const int cutCountX = 20;					//X轴方向上的切割数量
	const int cutCountY = 30;					//Y轴方向上的切割数量
	const int cutCountZ = 20;					//Z轴方向上的切割数量
	int innerPixelCount = 0;					//模型内体素计数
	Vector3 qualityCenter;						//体素模型重心
	double modelVolum;							//体素模型体积

};

#endif	//__PIXELMODEL_H__


