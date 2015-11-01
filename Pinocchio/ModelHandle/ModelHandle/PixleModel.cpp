

#include "PixelModel.h"
#include "flann\flann.h"
#include "GenerateTool.h"
#include "SDFGen_vec.h"
#include "SDFGen_array3.h"
#include "SDFGen_makelevelset3.h"
#include "mesh.h"
#include "pinocchioApi.h"

using namespace std;

PixelModel::PixelModel()
{

}

PixelModel::PixelModel(const PinocchioOutput* originData, Mesh* mesh, Vector3 meshMinVertex, Vector3 meshMaxVertex)
{
	double initLength = (meshMaxVertex[0] - meshMinVertex[0]) / cutCountX;
	double initWidth = (meshMaxVertex[1] - meshMinVertex[1]) / cutCountY;
	double initHeight = (meshMaxVertex[2] - meshMinVertex[2]) / cutCountZ;


	int verticeCount = mesh->vertices.size();
	meshPixels = new vector<Pixel>[verticeCount];

	vector<Vec3ui> faceList;
	for (int i = 0; i < mesh->edges.size(); i+=3)
	{
		faceList.push_back(Vec3ui(mesh->edges[i].vertex, mesh->edges[i + 1].vertex, mesh->edges[i + 2].vertex));
	}
	cout << "tri face size :" << faceList.size() << endl;
	vector<Vec3f> vertList;
	for (int i = 0; i < mesh->vertices.size(); i++)
	{ 
		vertList.push_back(Vec3f(mesh->vertices[i].pos[0], mesh->vertices[i].pos[1], mesh->vertices[i].pos[2]));
	}
	Vec3f minBox = Vec3f(meshMinVertex[0], meshMinVertex[1], meshMinVertex[2]);
	Vec3f maxBox = Vec3f(meshMaxVertex[0], meshMaxVertex[1], meshMaxVertex[2]);
	Vec3f unit(1, 1, 1);
	int padding = 1;
	minBox -= padding*dx*unit;
	maxBox += padding*dx*unit;
	Vec3ui sizes = Vec3ui((maxBox - minBox) / dx);
	Array3f phi_grid;
	Array3i closeF = make_level_set3(faceList, vertList, minBox, dx, sizes[0], sizes[1], sizes[2], phi_grid);
	cout << "参与处理体素点数：" << sizes[0] * sizes[1] * sizes[2] << endl;
	modelVolum = 0;
	for (int k = 0; k < sizes[2]; k++)
	{
		for (int j = 0; j < sizes[1]; j++)
		{
			for (int i = 0; i < sizes[0]; i++)
			{
				//判断体素点与当前面的signed distance
				if (phi_grid(i, j, k) > 0)
					continue;
				int linkFace = closeF(i, j, k);
				//TODO 此处获得的linkFace应该为faceList中的坐标 从而进一步获得点
				int faceVertice[3] = { faceList[linkFace][0], faceList[linkFace][1], faceList[linkFace][2] };
				float pos[3] = { minBox[0] + i * dx, minBox[1] + j * dx, minBox[2] + k * dx };
				meshPixels[faceVertice[0]].push_back(Pixel(pos, dx, dx, dx, faceVertice[0]));
				//判断是否为表面体素
				if (phi_grid(i, j, k) > -SURFACETHRESHOLD)
				{
					surfaceVoxelCount++;
					map<int, vector<int>>::iterator ite = surfaceIndex.find(faceVertice[0]);
					if (ite == surfaceIndex.end())
					{
						vector<int> tempAdd;
						tempAdd.push_back(meshPixels[faceVertice[0]].size() - 1);
						surfaceIndex.insert(pair<int, vector<int>>(faceVertice[0], tempAdd));
					}
					else
					{
						ite->second.push_back(meshPixels[faceVertice[0]].size() - 1);
					}
					//TODO 计算表面体素属性
					double maxWeight = 0;
					int linkIndex;
					for (int bone = 0; bone < originData->embedding.size() - 1; bone++)
					{
						if (originData->attachment->getWeights(faceVertice[0])[bone] > maxWeight)
						{
							linkIndex = bone;
							maxWeight = originData->attachment->getWeights(faceVertice[0])[bone];
						}
					}
					surfaceVoxelAttribute.push_back(VoxelAttribute(linkIndex, maxWeight, faceVertice[0], meshPixels[faceVertice[0]].size() - 1));
				}

				modelVolum += dx * dx * dx;
			}
		}
	}
	qualityCenter = Vector3(0, 0, 0);
	for (int i = 0; i < verticeCount; i++)
	{
		for (int j = 0; j < meshPixels[i].size(); j++)
		{
			qualityCenter += (meshPixels[i][j].getVolume() / modelVolum)
				* Vector3(meshPixels[i][j].pos[0], meshPixels[i][j].pos[1], meshPixels[i][j].pos[2]);
		}
	}

	cout << "顶点数 ： " << verticeCount << endl;
	int pointSum = 0;
	for (int i = 0; i < verticeCount; i++)
	{
		pointSum += meshPixels[i].size();
	}
	cout << "模型体素数：" << pointSum << endl;
}





