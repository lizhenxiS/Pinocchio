

#include "PixelModel.h"
#include "flann\flann.h"
#include "GenerateTool.h"
#include "SDFGen_vec.h"
#include "SDFGen_array3.h"
#include "SDFGen_makelevelset3.h"

using namespace std;

PixelModel::PixelModel()
{

}

PixelModel::PixelModel(Mesh* mesh, Vector3 meshMinVertex, Vector3 meshMaxVertex)
{
	double initLength = (meshMaxVertex[0] - meshMinVertex[0]) / cutCountX;
	double initWidth = (meshMaxVertex[1] - meshMinVertex[1]) / cutCountY;
	double initHeight = (meshMaxVertex[2] - meshMinVertex[2]) / cutCountZ;


	int verticeCount = mesh->vertices.size();
	//float* vertice = new float[verticeCount * 3];		//FLANN需要以数组形式传参
	meshPixels = new vector<Pixel>[verticeCount];
	//for (int i = 0; i < verticeCount; i++)
	//{
	//	vertice[i * 3 + 0] = mesh->vertices[i].pos[0];
	//	vertice[i * 3 + 1] = mesh->vertices[i].pos[1];
	//	vertice[i * 3 + 2] = mesh->vertices[i].pos[2];
	//}

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
	float dx = 0.01;	//体素大小
	int padding = 1;
	minBox -= padding*dx*unit;
	maxBox += padding*dx*unit;
	Vec3ui sizes = Vec3ui((maxBox - minBox) / dx);
	Array3f phi_grid;
	Array3i closeF = make_level_set3(faceList, vertList, minBox, dx, sizes[0], sizes[1], sizes[2], phi_grid);
	cout << "参与处理体素点数：" << sizes[0] * sizes[1] * sizes[2] << endl;
	for (int k = 0; k < sizes[2]; k++)
	{
		for (int j = 0; j < sizes[1]; j++)
		{
			for (int i = 0; i < sizes[0]; i++)
			{
				if (phi_grid(i, j, k) > 0)
					continue;
				int linkFace = closeF(i, j, k);
				int faceVertice[3] = { mesh->edges[linkFace].vertex, mesh->edges[linkFace].prev, mesh->edges[linkFace].twin };
				float pos[3] = { minBox[0] + i * dx, minBox[1] + j * dx, minBox[2] + k * dx };
				meshPixels[faceVertice[0]].push_back(Pixel(pos, dx, dx, dx, faceVertice[0]));
			}
		}
	}


	//struct FLANNParameters p;
	//p = DEFAULT_FLANN_PARAMETERS;
	//p.algorithm = FLANN_INDEX_KDTREE;
	//p.trees = 8;
	//p.log_level = FLANN_LOG_INFO;
	//p.checks = 64;
	//cout << "总体素数：" << cutCountX * cutCountY * cutCountZ * mesh->vertices.size() << endl;
	//cout << "min     : " << meshMinVertex << endl;
	//cout << "max     : " << meshMaxVertex << endl;
	//for (int i = 0; i < cutCountX; i++)
	//{
	//	if (i == 2)
	//		break;
	//	for (int j = 0; j < cutCountY; j++)
	//	{
	//		if (j == 2)
	//			break;
	//		for (int k = 0; k < cutCountZ; k++)
	//		{
	//			if (k == 2)
	//				break;
	//			float curPixel[3] = { meshMinVertex[0] + i * initLength,
	//				meshMinVertex[1] + j * initWidth, meshMinVertex[2] + j * initHeight };
	//			/*
	//			FLANN 算法获取模型中与当前点最近邻的点
	//			*/
	//			float speedup;
	//			flann_index_t index_id = flann_build_index(vertice, verticeCount, 1, &speedup, &p);
	//			int result;		//距离当前点最近的点
	//			float dists[3];
	//			flann_find_nearest_neighbors_index(index_id, curPixel, 1, &result, dists, 1, &p);
	//			flann_free_index(index_id, &p);
	//			if (!isInModel(curPixel, mesh->vertices[result].pos, mesh->vertices[result].normal))
	//				continue;
	//			meshPixels[result].push_back(Pixel(curPixel, initLength, initWidth, initHeight, result));
	//			innerPixelCount++;
	//			return;
	//		}
	//	}
	//}

	cout << "顶点数 ： " << verticeCount << endl;
	int pointSum = 0;
	for (int i = 0; i < verticeCount; i++)
	{
		pointSum += meshPixels[i].size();
	}
	cout << "模型体素数：" << pointSum << endl;
}

//判断当前位置体素是否在模型点内
bool PixelModel::isInModel(float* pixelPoint, Vector3 vertex, Vector3 vertexNormal)
{
	Vector3 lineVector = Vector3(pixelPoint[0] - vertex[0],
		pixelPoint[1] - vertex[1], pixelPoint[2] - vertex[2]);
	double pointProduct = getPointProduct(lineVector, vertexNormal);
	if (pointProduct < 0)
		return true;
	return false;
}




