

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

	std::vector<Pixel>* meshPixels;						//模型体素集
	std::map<int, vector<int>> surfaceIndex;			//表面体素下标集
	std::vector<VoxelAttribute> surfaceVoxelAttribute;	//表面体素所属骨骼[0 - 16]及对应权重[0 - 1]
	const double SURFACETHRESHOLD = 0.005;				//表皮厚度常量
	int surfaceVoxelCount = 0;
	const int cutCountX = 20;							//X轴方向上的切割数量
	const int cutCountY = 30;							//Y轴方向上的切割数量
	const int cutCountZ = 20;							//Z轴方向上的切割数量
	int innerPixelCount = 0;							//模型内体素计数
	Vector3 qualityCenter;								//体素模型重心
	double modelVolum;									//体素模型体积
	float dx = 0.01;									//初始体素大小

};

#endif	//__PIXELMODEL_H__


