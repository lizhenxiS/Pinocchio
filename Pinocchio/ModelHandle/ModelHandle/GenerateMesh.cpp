

#include "GenerateMesh.h"
#include "GenerateTool.h"
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <glut.h>
#include "taucs_matrix.h"
#include "taucs_solver.h"
#include <time.h>
#include "SkeletonLinkRotate.h"
#include "GiftWrap.h"
#include "Point2d.h"
#include <sstream>
#include <ctime>
#include <algorithm>
#include <string>

using namespace std;

bool* changedIndex;
bool changeFromMap = false;
bool SmoothState = false;
bool StressState = false;

extern GLdouble modelView[16];
extern GLdouble projView[16];
extern GLint viewView[4];

#define PI 3.1415926

struct CAMERA
{
	GLdouble xeye;
	GLdouble yeye;
	GLdouble zeye;
};

extern CAMERA camera;

//寻找点击范围内的点
//当前块序号从0开始，顶点对应键值从1开始
int area_smooth = 0;
int area_stress = 0;
#define RANGE 5

int counting[100];
int flaging = 0;


//顶点相邻环
vector<vector<int>> VertexNeighbor;

//初始化颜色信息
void GenerateMesh::initColor()
{
	color_Mesh[0] = 0.0;
	color_Mesh[1] = 1.0;
	color_Mesh[2] = 1.0;
	color_Skele[0] = 1.0;
	color_Skele[1] = 1.0;
	color_Skele[2] = 0.0;
	color_Tran[0] = 1.0;
	color_Tran[1] = 0.0;
	color_Tran[2] = 0.0;
}

//初始化骨骼线条宽度信息
void GenerateMesh::initSkeletonWidth()
{
	thin = 5.0;
}

Vector3 getNormal(string str)
{
	istringstream istr(str);
	string str0;
	istr >> str0;
	if (strcmp(str0.c_str(), "vn") == 0)
	{
		double x, y, z;
		istr >> x >> y >> z;
		return Vector3(x, y, z);
	}
	else
		return Vector3(-1, -1, -1);
}

//从文件中读取法向量信息
void GenerateMesh::readVerticeNormal(string filename)
{
	ifstream file(filename);
	if (!file)
	{
		cout << "read normal failed" << endl;
		return;
	}
	while (!file.eof())
	{
		string line;
		getline(file, line);
		if (line[0] == 'v' && line[1] == 'n')
		{
			Vector3 temp = getNormal(line);
			meshNormals.push_back(temp);
		}
	}
	file.close();
}


//读取人体模型初始化数据
GenerateMesh::GenerateMesh(const string& file)
{
	initColor();
	initSkeletonWidth();

	cout << "Begin to generate Mesh :" << endl;

	modelFile = file;
	readVerticeNormal(file);						//从文件中读取法向量信息
	meshVertices = new Mesh(file);					//获取骨骼信息及关联信息及删除重叠点

	int verts = meshVertices->vertices.size();
	flatShading = false;

	modelQualityCenter = Vector3(0, 0, 0);
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		delta_skeleton.push_back(Vector3(0.0, 0.0, 0.0));
		delta_Custom.push_back(Vector3(0.0, 0.0, 0.0));
		delta_Energy.push_back(Vector3(0.0, 0.0, 0.0));
		delta_rotate.push_back(Vector3(0.0, 0.0, 0.0));
		modelQualityCenter += meshVertices->vertices[i].pos;
	}
	modelQualityCenter /= meshVertices->vertices.size();
	process();

	cout << "start to counting pixel model..." << endl;
	pixelModel = new PixelModel(&originData, meshVertices, modelMinPoint, modelMaxPoint);
	cout << "表面体素个数：" << pixelModel->surfaceVoxelCount << endl;
	cout << "finish counting pixel model" << endl;
	collisionCheck();

	int tempSum = 0;
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		tempSum += pixelModel->meshPixels[i].size();
	}
	cout << "实际绘制点数量：" << tempSum << endl;

	if (verts != meshVertices->vertices.size())
		system("pause");

	changedIndex = new bool[embedding.size()];
	for (int i = 0; i < embedding.size(); i++)
	{
		changedIndex[i] = false;
	}
	updateModelCenter();
	Smooth_size = 0;
	Stress_size = 0;
	cout << meshVertices->edges.size() << " ___half" << endl;
	cout << "Generate Mesh Success !" << embedding.size() << endl;
}

int pixelcolorr = 0;
int pixelcolorg = 0;
int pixelcolorb = 0;
void GenerateMesh::drawPixel(Pixel p, Vector3 color)
{
	glColor3d(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	glVertex3dv(p.box[0]);
	glVertex3dv(p.box[3]);
	glVertex3dv(p.box[2]);
	glVertex3dv(p.box[1]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3dv(p.box[1]);
	glVertex3dv(p.box[2]);
	glVertex3dv(p.box[6]);
	glVertex3dv(p.box[5]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3dv(p.box[3]);
	glVertex3dv(p.box[7]);
	glVertex3dv(p.box[6]);
	glVertex3dv(p.box[2]);
	glEnd();

	glColor3b(100, 100, 100);
	glBegin(GL_POLYGON);
	glVertex3dv(p.box[0]);
	glVertex3dv(p.box[4]);
	glVertex3dv(p.box[7]);
	glVertex3dv(p.box[3]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3dv(p.box[0]);
	glVertex3dv(p.box[1]);
	glVertex3dv(p.box[5]);
	glVertex3dv(p.box[4]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3dv(p.box[7]);
	glVertex3dv(p.box[4]);
	glVertex3dv(p.box[5]);
	glVertex3dv(p.box[6]);
	glEnd();
}

//析构函数
GenerateMesh::~GenerateMesh()
{
	if (skeletonNodeInformation != NULL)
	{
		delete skeletonNodeInformation;
	}
	if (deltaEnergy != NULL)
	{
		delete deltaEnergy;
	}
	if (expectGrowScale != NULL)
	{
		delete expectGrowScale;
	}
	if (meshVertices != NULL)
	{
		delete meshVertices;
	}
	if (copy_MeshVertices != NULL)
	{
		delete copy_MeshVertices;
	}
	if (attachment != NULL)
	{
		delete attachment;
	}
	if (originMeshVertices != NULL)
	{
		delete originMeshVertices;
	}
	if (pixelModel != NULL)
	{
		delete pixelModel;
	}
}

void PutStringToInt(string s, int *num)
{
	int index = 0;
	int head, end;
	int temp, ii;


	while (s[index] < '0' || s[index] > '9' )
		index++;

	head = index;
	while (s[index] >= '0' && s[index] <= '9')
	{
		index++;
	}
	end = index;
	temp = end - head;

	num[0] = 0;
	ii = 0;
	while (temp != 0)
	{
		num[0] += (s[head + ii] - '0') * pow(10, temp - 1);
		ii++;
		temp--;
	}


	while (s[index] != ' ')
		index++;
	while (s[index] == ' ')
		index++;

	head = index;
	while (s[index] >= '0' && s[index] <= '9')
	{
		index++;
	}
	end = index;
	temp = end - head;

	num[1] = 0;
	ii = 0;
	while (temp != 0)
	{
		num[1] += (s[head + ii] - '0') * pow(10, temp - 1);
		ii++;
		temp--;
	}

	while (s[index] != ' ')
		index++;
	while (s[index] == ' ')
		index++;

	head = index;
	while (s[index] >= '0' && s[index] <= '9')
	{
		index++;
	}
	end = index;
	temp = end - head;

	num[2] = 0;
	ii = 0;
	while (temp != 0)
	{
		num[2] += (s[head + ii] - '0') * pow(10, temp - 1);
		ii++;
		temp--;
	}


}

//导出模型
void GenerateMesh::OutPutMesh()
{
	fstream out_obj;
	out_obj.open("output.obj", ios::out);

	if (!out_obj.is_open())
	{
		cout << "Error: canot open " << endl;
		return;
	}

	int small_x_index = 0;
	int small_y_index = 0;
	float small_y = 1;
	float small_x = 1;

	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		out_obj << "v " << meshVertices->vertices[i].pos[0] << " " << meshVertices->vertices[i].pos[1] << " " << meshVertices->vertices[i].pos[2] << endl;


		if (meshVertices->vertices[i].pos[0] < small_x)
		{
			small_x_index = i;
			small_x = meshVertices->vertices[i].pos[0];
		}
		if (meshVertices->vertices[i].pos[1] < small_y)
		{
			small_y_index = i;
			small_y = meshVertices->vertices[i].pos[1];
		}

	}

	cout << "SMALL_X " << small_x_index << " " << small_x << endl;
	cout << "SMALL_Y " << small_y_index << " " << small_y << endl;


	fstream re_obj(modelFile);
	string temp;
	getline(re_obj, temp);
	int num[3];
	while (!re_obj.eof())
	{
		if (temp[0] == 'f')
		{
			temp[0] = ' ';
			PutStringToInt(temp, num);
			out_obj << "f " << num[0] << " " << num[1] << " " << num[2] << endl;
		}

		getline(re_obj, temp);
	}

	out_obj.close();
	re_obj.close();
	cout << "Obj file output Success !" << endl;
}

/*
数据获取
计算模型点法向量
骨骼适配输入模型
*/
void GenerateMesh::process()
{
	//传入参数的信息存储
	ArgData a;

	meshVertices->normalizeBoundingBox();
	//copy_m->normalizeBoundingBox();

	meshVertices->computeVertexNormals();
	//copy_m->computeVertexNormals();

	originMeshVertices = new MyVerticeMesh(*meshVertices);
	copy_MeshVertices = new MyVerticeMesh(*meshVertices);

	//初始化骨架
	Skeleton given = HumanSkeleton();
	given.scale(a.skelScale * 0.7);

	//system("time");
	

	if (!a.noFit) { //do everything
		//组合骨骼人体数据
		originData = autorig(given, *meshVertices);
	}

	//system("time");
	

	bonePointCount = originData.embedding.size();
	boneOriginLen = new double[bonePointCount - 1];
	skeletonNodeInformation = new SkeletonNode[bonePointCount];
	
	//获取骨骼初始长度及节点前后关系
	for (int y = 1; y < originData.embedding.size(); y++)
	{
		//y为末端点，given.fPrev()[y]为前续端点
		int parentIndex = given.fPrev()[y];
		skeletonNodeInformation[y].parentIndex = parentIndex;
		skeletonNodeInformation[given.fPrev()[y]].childrenIndex.push_back(y);

		double curLen = getDistance(originData.embedding[parentIndex], originData.embedding[y]);
		boneOriginLen[y - 1] = curLen;
	}

	//骨骼模型获取
	for (int y = 0; y < originData.embedding.size(); y++)
	{
		embedding.push_back(originData.embedding[y]);
		copy_embedding.push_back(originData.embedding[y]);

		if (y == 14)
		cout << y << " " << originData.embedding[y] << endl;
	}

	//各骨骼距离规范化energy
	deltaEnergy = new double[bonePointCount];

	//除去 0 下标
	deltaEnergy[0] = -1;

	//各骨骼期待向规范方向增长比例
	expectGrowScale = new double[bonePointCount];
	for (int i = 1; i < bonePointCount; i++)
	{
		expectGrowScale[i] = 0;
	}

	vector<Vector3> tempInitbone;
	Vector3 temppoint;
	temppoint[0] = 0.0;
	temppoint[1] = 0.0;
	temppoint[2] = 0.0;

	for (int j = 0; j < bonePointCount; j++)
	{
		tempInitbone.push_back(temppoint);
	}

	modelMinPoint = Vector3(10000, 10000, 10000);
	modelMaxPoint = Vector3(-10000, -10000, -10000);
	for (int i = 0; i < copy_MeshVertices->vertices.size(); i++)
	{
		ExpectGrow.push_back(tempInitbone);
		Vector3 tempPoint = meshVertices->vertices[i].pos;
		modelMinPoint[0] = modelMinPoint[0] < tempPoint[0] ? modelMinPoint[0] : tempPoint[0];
		modelMinPoint[1] = modelMinPoint[1] < tempPoint[1] ? modelMinPoint[1] : tempPoint[1];
		modelMinPoint[2] = modelMinPoint[2] < tempPoint[2] ? modelMinPoint[2] : tempPoint[2];
		modelMaxPoint[0] = modelMaxPoint[0] > tempPoint[0] ? modelMaxPoint[0] : tempPoint[0];
		modelMaxPoint[1] = modelMaxPoint[1] > tempPoint[1] ? modelMaxPoint[1] : tempPoint[1];
		modelMaxPoint[2] = modelMaxPoint[2] > tempPoint[2] ? modelMaxPoint[2] : tempPoint[2];
	}

	CountGrowCustom();
	initRotateAngleArray();
}

#define HEAD 3

//模型规范化比例
void GenerateMesh::ChangeGrowScale(const int bone, const double scale)
{
	expectGrowScale[bone] = scale;
	MeshGrowCustom();
}

//更新模型因骨骼变化而变化的期待模型各点位置
void GenerateMesh::CountGrowCustom()
{
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		//每个骨骼期望该点所在位置
		Vector3 temp;

		for (int j = 1; j < bonePointCount; j++)
		{
			//头部期望的点增长
			if (j == HEAD)
			{
				Vector3 m_delta;
				m_delta[0] = meshVertices->vertices[i].pos[0] + delta_skeleton[i][0];
				m_delta[1] = meshVertices->vertices[i].pos[1] + delta_skeleton[i][1];
				m_delta[2] = meshVertices->vertices[i].pos[2] + delta_skeleton[i][2];

				for (int k = 0; k < 3; k++)
				{
					temp[k] = embedding[HEAD][k] + ((m_delta[k] - embedding[HEAD][k]) * getDistance(embedding[HEAD], embedding[skeletonNodeInformation[HEAD].parentIndex])) / getDistance(m_delta, embedding[HEAD]) - m_delta[k];
				}
			}
			else
				//普通臂骨-----------假设规范化臂骨长宽比 3：2  __________________
			{
				Vector3 m_delta;
				m_delta[0] = meshVertices->vertices[i].pos[0] + delta_skeleton[i][0];
				m_delta[1] = meshVertices->vertices[i].pos[1] + delta_skeleton[i][1];
				m_delta[2] = meshVertices->vertices[i].pos[2] + delta_skeleton[i][2];

				//骨骼上“投影点”
				Vector3 projVertice = proj(embedding[skeletonNodeInformation[j].parentIndex], embedding[j], m_delta);
				double d = getDistance(embedding[skeletonNodeInformation[j].parentIndex], embedding[j]) * 2 / 3;

				//当投影落在骨骼外时，将近端点作为投影点
				if (!isInLine(projVertice, embedding[skeletonNodeInformation[j].parentIndex], embedding[j]))
				{
					if (getDistance(m_delta, embedding[j]) < getDistance(m_delta, embedding[skeletonNodeInformation[j].parentIndex]))
					{
						projVertice = embedding[j];
					}
					else
						projVertice = embedding[skeletonNodeInformation[j].parentIndex];
				}

				for (int k = 0; k < 3; k++)
				{
					temp[k] = projVertice[k] + ((m_delta[k] - projVertice[k]) * (d / 2)) / getDistance(m_delta, projVertice) - m_delta[k];
				}
			}

			for (int k = 0; k < 3; k++)
			{
				ExpectGrow[i][j][k] = temp[k];
			}
		}
	}
}

//模型向规范化演化
void GenerateMesh::MeshGrowCustom()
{
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		//现对每个期望点与实际点作差后加权处理，得到期望和
		Vector3 tempExpect;
		for (int k = 0; k < 3; k++)
		{
			tempExpect[k] = 0;
			for (int j = 1; j < bonePointCount; j++)
			{
				tempExpect[k] += originData.attachment->getWeights(i)[j - 1] * ExpectGrow[i][j][k] * expectGrowScale[j];
			}

			delta_Custom[i][k] = tempExpect[k];
		}
	}
}

vector<Vector3> cg_girl;
void dis_CG()
{
	Vector3 temp;

	temp[0] = 0.03;
	temp[1] = 0.37;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = -0.02;
	temp[1] = 0.03;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = 0.;
	temp[1] = 0.;
	temp[2] = -0.05;
	cg_girl.push_back(temp);
	temp[0] = 0.04;
	temp[1] = 0.88;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = -0.13;
	temp[1] = 0.02;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = -0.13;
	temp[1] = -0.23;
	temp[2] = 0.03;
	cg_girl.push_back(temp);
	temp[0] = -0.12;
	temp[1] = -0.43;
	temp[2] = 0.03;
	cg_girl.push_back(temp);
	temp[0] = -0.1;
	temp[1] = -0.47;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = 0.13;
	temp[1] = 0.02;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = 0.13;
	temp[1] = -0.23;
	temp[2] = 0.03;
	cg_girl.push_back(temp);
	temp[0] = 0.12;
	temp[1] = -0.43;
	temp[2] = 0.03;
	cg_girl.push_back(temp);
	temp[0] = 0.1;
	temp[1] = -0.47;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = -0.12;
	temp[1] = 0.37;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = -0.29;
	temp[1] = 0.16;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = -0.21;
	temp[1] = 0.;
	temp[2] = -0.03;
	cg_girl.push_back(temp);
	temp[0] = 0.18;
	temp[1] = 0.37;
	temp[2] = 0;
	cg_girl.push_back(temp);
	temp[0] = 0.27;
	temp[1] = 0.15;
	temp[2] = -0.02;
	cg_girl.push_back(temp);
	temp[0] = 0.28;
	temp[1] = -0.07;
	temp[2] = 0.02;
	cg_girl.push_back(temp);
}


//骨骼角度变化后模型映射变化处理
void GenerateMesh::ChangeFromSkeletonRotation()
{

}

//将骨骼直接转换为所选模板
void GenerateMesh::changeSkeletonFromMap()
{
	dis_CG();

	for (int i = 1; i < bonePointCount; i++)
	{
		double scale;
		scale = getDistance(cg_girl[i], cg_girl[skeletonNodeInformation[i].parentIndex]) / getDistance(originData.embedding[i], originData.embedding[skeletonNodeInformation[i].parentIndex]);
		
		changeSingleSkeleton(i, scale);

	}

	updateModelCenter();
}

//初始化记录骨骼旋转角数组
void GenerateMesh::initRotateAngleArray()
{
	Vector3 boneNode;
	Vector3 preBoneNode;
	Vector3 prepreBoneNode;
	int preIndex;
	int prepreIndex;
	double alpha;
	double beta;
	for (int i = 0; i < BONECOUNT; i++)
	{
		if (i == 0 || i == 2 || i == 11 || i == 14)
		{
			switch (i)
			{
			case 0:
				preIndex = 0;
				prepreIndex = 3;
				break;
			case 2:
				preIndex = 0;
				prepreIndex = 1;
				break;
			case 11:
				preIndex = 0;
				prepreIndex = 15;
				break;
			case 14:
				preIndex = 0;
				prepreIndex = 12;
				break;
			default:
				break;
			}
			boneNode = embedding[i + 1];
			preBoneNode = embedding[preIndex];
			prepreBoneNode = embedding[prepreIndex];
			alpha = AngleInSpace(boneNode, preBoneNode, prepreBoneNode);
			beta = 0;
		}
		else
		{
			boneNode = embedding[i + 1];
			preIndex = skeletonNodeInformation[i + 1].parentIndex;
			prepreIndex = skeletonNodeInformation[preIndex].parentIndex;
			preBoneNode = embedding[preIndex];
			prepreBoneNode = embedding[prepreIndex];
			alpha = AngleInSpace(boneNode, preBoneNode, prepreBoneNode);
			beta = 0;
		}
		rotateAngle[i][0] = alpha;
		rotateAngle[i][1] = beta;
	}
}

//任意自由度改变骨骼，并相对静止牵动关联骨骼
//bone 为骨骼序数，0 - 16
void GenerateMesh::changeSkeleton(const int bone, double alpha, double beta)
{
	if (bone == 0 || bone == 2 || bone == 11 || bone == 14)
	{
		//cout << "this skeletons can not be changed now" << endl;
		cout << "this skeletons will be deal innormal in SkeletonLinkRotate" << endl;
		//return;
	}
	double tempAlpha = alpha - rotateAngle[bone][0];
	double tempBeta = beta - rotateAngle[bone][1];
	rotateAngle[bone][0] = alpha;
	rotateAngle[bone][1] = beta;

	//将实际骨骼旋转
	SkeletonLinkRotate temp(originData, embedding, skeletonNodeInformation);
	temp.rotateSkeleton(bone, embedding, tempAlpha, tempBeta);
	for (int j = 0; j < bonePointCount; j++)
	{
		embedding[j] = temp.skeletonPoints[j];
	}
	temp.refreshMesh(meshVertices);
	for (int i = 0; i < temp.meshVertices->vertices.size(); i++)
	{
		meshVertices->vertices[i].pos = temp.meshVertices->vertices[i].pos;
	}

	//变化完模型后继续变化体素
	temp.refreshVoxel(meshVertices->vertices.size(), pixelModel);
	collisionCheck();

	updateModelCenter();
	updateModelBottomBox();
	updateModelConvexHull();
}

//同步改变单一骨骼
void GenerateMesh::changeSingleSkeleton(const int boneNode, const double scale)
{

	vector<Vector3> oldBoneNodePoint;
	vector<Vector3> newBoneNodePoint;

	for (int i = 0; i < embedding.size(); i++)
	{
		oldBoneNodePoint.push_back(embedding[i]);
	}

	if (((scale - 1) < 0.05) && ((scale - 1) > 0.05))
	{
		changedIndex[boneNode] = false;
	}
	else
		changedIndex[boneNode] = true;

	vector<int> concernNodes = getAfterSkeletonIndex(skeletonNodeInformation, boneNode);

	//当前骨骼（不一定为初始骨骼）变化前位置记录
	double Temp_embed_X = embedding[boneNode][0];
	double Temp_embed_Y = embedding[boneNode][1];
	double Temp_embed_Z = embedding[boneNode][2];

	double curLen = getDistance(embedding[boneNode], embedding[skeletonNodeInformation[boneNode].parentIndex]);
	Vector3 curUnitVector = (embedding[boneNode] - embedding[skeletonNodeInformation[boneNode].parentIndex]) / curLen;
	embedding[boneNode][0] =
		scale * boneOriginLen[boneNode - 1] * curUnitVector[0]
		+ embedding[skeletonNodeInformation[boneNode].parentIndex][0];
	embedding[boneNode][1] =
		scale * boneOriginLen[boneNode - 1] * curUnitVector[1]
		+ embedding[skeletonNodeInformation[boneNode].parentIndex][1];
	embedding[boneNode][2] =
		scale * boneOriginLen[boneNode - 1] * curUnitVector[2]
		+ embedding[skeletonNodeInformation[boneNode].parentIndex][2];

	for (int i = 0; i < concernNodes.size(); i++)
	{
		int tempIndex = concernNodes[i];

		//此处对于每个被动关联骨骼都只处理一个端点，因为一个端点即代表该骨骼，剩余端点会由后续骨骼处理
		embedding[tempIndex][0] = embedding[tempIndex][0] + embedding[boneNode][0] - Temp_embed_X;
		embedding[tempIndex][1] = embedding[tempIndex][1] + embedding[boneNode][1] - Temp_embed_Y;
		embedding[tempIndex][2] = embedding[tempIndex][2] + embedding[boneNode][2] - Temp_embed_Z;
	}


	for (int i = 0; i < embedding.size(); i++)
	{
		newBoneNodePoint.push_back(embedding[i]);
	}

	extendMesh(oldBoneNodePoint, newBoneNodePoint);
	updateModelVoxelInScale(oldBoneNodePoint, newBoneNodePoint);
	collisionCheck();

	CountGrowCustom();
	updateModelCenter();
	updateModelBottomBox();
	updateModelConvexHull();
}

//骨骼长度变化后模型映射变化处理
void GenerateMesh::extendMesh(vector<Vector3> oldBoneNodePoint, vector<Vector3> newBoneNodePoint)
{
	//顶点临时替代
	Vector3 newMeshVertice;
	//顶点数
	int VerticesSize = meshVertices->vertices.size();

	int weightSize;
	Vector3 projTemp;
	double RjTemp;
	Vector3 SBjTemp;
	double EBjViTemp;
	double weight;


	for (int i = 0; i < VerticesSize; i++)
	{
		newMeshVertice = Vector3(0, 0, 0);

		weightSize = originData.attachment->getWeights(i).size();

		/*?????????????????*/ //weightSize=17  以及 j 取值不确定 bone_a bone_b 选取
		//假定 bone_a 为起点  bone_b 为前点
		for (int j = 0; j < weightSize; j++)
		{
			//第j+1根骨骼
			int startNode = skeletonNodeInformation[j + 1].parentIndex;
			int endNode = j + 1;
			Vector3 oldStartPoint = oldBoneNodePoint[startNode];
			Vector3 oldEndPoint = oldBoneNodePoint[endNode];
			Vector3 newStartPoint = newBoneNodePoint[startNode];
			Vector3 newEndPoint = newBoneNodePoint[endNode];
			Vector3 oldMeshVertice = meshVertices->vertices[i].pos;

			projTemp = proj(oldStartPoint, oldEndPoint, oldMeshVertice);
			RjTemp = Rj(oldStartPoint, oldEndPoint, newStartPoint, newEndPoint);
			SBjTemp = SBj(oldStartPoint, oldEndPoint, newStartPoint, newEndPoint);
			EBjViTemp = EBjVi(oldStartPoint, oldEndPoint, oldMeshVertice);
			weight = (0.5 + originData.attachment->getWeights(i)[j] * 10000.0) / 10000.0;

			newMeshVertice += weight*(embedding[startNode] + (oldMeshVertice - oldStartPoint)
				+ (oldMeshVertice - projTemp)*(RjTemp - 1)
				+ EBjViTemp * SBjTemp);

		}

		meshVertices->vertices[i].pos = newMeshVertice;
	}
}

//更新放缩体素(每次放缩骨骼后必须调用)(同时更新体素模型重心、体积)
void GenerateMesh::updateModelVoxelInScale(vector<Vector3> oldBoneNodePoint, vector<Vector3> newBoneNodePoint)
{
	//初始化体素模型重心、体积
	pixelModel->qualityCenter = Vector3(0, 0, 0);
	pixelModel->modelVolum = 0;

	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		for (int j = 0; j < pixelModel->meshPixels[i].size(); j++)
		{
			Vector3 newCenter = Vector3(0, 0, 0);
			Vector3 newBox[8] = { Vector3(0, 0, 0), Vector3(0, 0, 0),
				Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0),
				Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0) };
			Vector3 oldCenter = Vector3(pixelModel->meshPixels[i][j].pos[0],
				pixelModel->meshPixels[i][j].pos[1], pixelModel->meshPixels[i][j].pos[2]);
			Vector3 oldBox[8];
			for (int t = 0; t < 8; t++)
			{
				oldBox[t] = Vector3(pixelModel->meshPixels[i][j].box[t][0],
					pixelModel->meshPixels[i][j].box[t][1], pixelModel->meshPixels[i][j].box[t][2]);
			}
			int weightSize;
			Vector3 projTemp;
			double RjTemp;
			Vector3 SBjTemp;
			double EBjViTemp;
			double weight;
			for (int k = 0; k < BONECOUNT; k++)
			{
				//第k+1根骨骼
				int startNode = skeletonNodeInformation[k + 1].parentIndex;
				int endNode = k + 1;
				Vector3 oldStartPoint = oldBoneNodePoint[startNode];
				Vector3 oldEndPoint = oldBoneNodePoint[endNode];
				Vector3 newStartPoint = newBoneNodePoint[startNode];
				Vector3 newEndPoint = newBoneNodePoint[endNode];

				//center 部分
				projTemp = proj(oldStartPoint, oldEndPoint, oldCenter);
				RjTemp = Rj(oldStartPoint, oldEndPoint, newStartPoint, newEndPoint);
				SBjTemp = SBj(oldStartPoint, oldEndPoint, newStartPoint, newEndPoint);
				EBjViTemp = EBjVi(oldStartPoint, oldEndPoint, oldCenter);
				weight = (0.5 + originData.attachment->getWeights(i)[k] * 10000.0) / 10000.0;
				newCenter += weight*(embedding[startNode] + (oldCenter - oldStartPoint)
					+ (oldCenter - projTemp)*(RjTemp - 1)
					+ EBjViTemp * SBjTemp);

				//box部分
				for (int t = 0; t < 8; t++)
				{
					projTemp = proj(oldStartPoint, oldEndPoint, oldBox[t]);
					RjTemp = Rj(oldStartPoint, oldEndPoint, newStartPoint, newEndPoint);
					SBjTemp = SBj(oldStartPoint, oldEndPoint, newStartPoint, newEndPoint);
					EBjViTemp = EBjVi(oldStartPoint, oldEndPoint, oldBox[t]);
					weight = (0.5 + originData.attachment->getWeights(i)[k] * 10000.0) / 10000.0;
					newBox[t] += weight*(embedding[startNode] + (oldBox[t] - oldStartPoint)
						+ (oldBox[t] - projTemp)*(RjTemp - 1)
						+ EBjViTemp * SBjTemp);
				}
			}

			pixelModel->meshPixels[i][j].pos[0] = newCenter[0];
			pixelModel->meshPixels[i][j].pos[1] = newCenter[1];
			pixelModel->meshPixels[i][j].pos[2] = newCenter[2];
			for (int t = 0; t < 8; t++)
			{
				pixelModel->meshPixels[i][j].box[t][0] = newBox[t][0];
				pixelModel->meshPixels[i][j].box[t][1] = newBox[t][1];
				pixelModel->meshPixels[i][j].box[t][2] = newBox[t][2];
			}
			pixelModel->meshPixels[i][j].updateLWH();
			pixelModel->modelVolum += pixelModel->meshPixels[i][j].getVolume();
		}
	}
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		for (int j = 0; j < pixelModel->meshPixels[i].size(); j++)
		{
			pixelModel->qualityCenter += (pixelModel->meshPixels[i][j].getVolume() / pixelModel->modelVolum)
				* Vector3(pixelModel->meshPixels[i][j].pos[0], pixelModel->meshPixels[i][j].pos[1], pixelModel->meshPixels[i][j].pos[2]);
		}
	}
}

//绘制骨骼shunj 
void GenerateMesh::drawSkeleton()
{
	for (int i = 1; i < bonePointCount; i++)
	{
		if (changedIndex[i])
			drawLine(embedding[i], embedding[skeletonNodeInformation[i].parentIndex], color_Tran, thin);
		else
			drawLine(embedding[i], embedding[skeletonNodeInformation[i].parentIndex], color_Skele, thin);
	}

	updateModelBottomBox();
	updateModelConvexHull();
	drawModelBottomBox();
	drawModelConvexHull();
	drawModelCenter();
}

//绘制模型体素
void GenerateMesh::drawVoxel()
{
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		int tempCount = pixelModel->meshPixels[i].size();
		map<int, vector<int>>::iterator ite = pixelModel->surfaceIndex.find(i);

		for (int j = 0; j < tempCount; j++)
		{
			//表面体素
			if (ite != pixelModel->surfaceIndex.end()
				&& std::find(ite->second.begin(), ite->second.end(), j) != ite->second.end())
			{
				//碰撞区域
				if (collisionArea.find(i) != collisionArea.end()
					&& std::find(collisionArea[i].begin(), collisionArea[i].end(), j) != collisionArea[i].end())
				{
					drawPixel(pixelModel->meshPixels[i][j], Vector3(1, 0, 0));
				}
				else
				{
					drawPixel(pixelModel->meshPixels[i][j], Vector3(0, 0, 1));
				}
			}
			else
			{
				drawPixel(pixelModel->meshPixels[i][j], Vector3(0.6, 0.6, 0));
			}
		}
	}
}


#define MODELBOTTOMSCALE 0.005

//更新模型底面盒子
void GenerateMesh::updateModelBottomBox()
{
	modelBottomBox[0] = Vector3(modelMinPoint[0], modelMinPoint[1], modelMinPoint[2]);
	modelBottomBox[1] = Vector3(modelMaxPoint[0], modelMinPoint[1], modelMinPoint[2]);
	modelBottomBox[2] = Vector3(modelMinPoint[0], modelMinPoint[1], modelMaxPoint[2]);
	modelBottomBox[3] = Vector3(modelMaxPoint[0], modelMinPoint[1], modelMaxPoint[2]);
}

//更新模型着地面
void GenerateMesh::updateModelConvexHull()
{
	convexHull.clear();
	double modelHeight = modelMaxPoint[1] - modelMinPoint[1];
	double bottomY = modelMinPoint[1] + MODELBOTTOMSCALE * modelHeight;
	vector<Vector3> bottomPoints;
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		if (meshVertices->vertices[i].pos[1] < bottomY)
			bottomPoints.push_back(meshVertices->vertices[i].pos);
	}

	GiftWrap giftWrap = GiftWrap(bottomPoints);
	vector<Point2d> convexPoints = giftWrap.getConvexHull();
	for (int i = 0; i < convexPoints.size(); i++)
	{
		convexHull.push_back(Vector3(convexPoints[i].x, bottomY, convexPoints[i].y));
	}
}

//绘制模型着地凸包
void GenerateMesh::drawModelConvexHull()
{
	drawConverxHull(convexHull, Vector3(1, 0, 0));
}

//绘制模型底面
void GenerateMesh::drawModelBottomBox()
{
	drawSquare(modelBottomBox[0], modelBottomBox[1], modelBottomBox[3], modelBottomBox[2], Vector3(0, 1, 0));
}

//绘制模型质心
void GenerateMesh::drawModelCenter()
{
	double modelHeight = modelMaxPoint[1] - modelMinPoint[1];
	double bottomY = modelMinPoint[1] + MODELBOTTOMSCALE * modelHeight;
	/*代表质心的正方形*/
	double sideLen = 0.02;
	Vector3 pa = Vector3(modelQualityCenter[0] - sideLen / 2, bottomY + 0.01, modelQualityCenter[2] - sideLen / 2);
	Vector3 pb = Vector3(modelQualityCenter[0] + sideLen / 2, bottomY + 0.01, modelQualityCenter[2] - sideLen / 2);
	Vector3 pd = Vector3(modelQualityCenter[0] - sideLen / 2, bottomY + 0.01, modelQualityCenter[2] + sideLen / 2);
	Vector3 pc = Vector3(modelQualityCenter[0] + sideLen / 2, bottomY + 0.01, modelQualityCenter[2] + sideLen / 2);
	drawSquare(pa, pb, pc, pd, Vector3(0, 0, 1));
}

//绘制骨骼结点
void GenerateMesh::drawSkeletonPoint()
{
	for (int i = 0; i < bonePointCount; i++)
	{
		drawPoint(embedding[i], thin * 2, Vector3(0, 0, 1));
	}
}


//计算模型质心
void GenerateMesh::updateModelCenter()
{
	//更新模型边框
	modelMinPoint = Vector3(10000, 10000, 10000);
	modelMaxPoint = Vector3(-10000, -10000, -10000);
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		Vector3 tempPoint = meshVertices->vertices[i].pos;
		modelMinPoint[0] = modelMinPoint[0] < tempPoint[0] ? modelMinPoint[0] : tempPoint[0];
		modelMinPoint[1] = modelMinPoint[1] < tempPoint[1] ? modelMinPoint[1] : tempPoint[1];
		modelMinPoint[2] = modelMinPoint[2] < tempPoint[2] ? modelMinPoint[2] : tempPoint[2];
		modelMaxPoint[0] = modelMaxPoint[0] > tempPoint[0] ? modelMaxPoint[0] : tempPoint[0];
		modelMaxPoint[1] = modelMaxPoint[1] > tempPoint[1] ? modelMaxPoint[1] : tempPoint[1];
		modelMaxPoint[2] = modelMaxPoint[2] > tempPoint[2] ? modelMaxPoint[2] : tempPoint[2];
	}

	modelQualityCenter = pixelModel->qualityCenter;

	cout << "当前模型重心为：" << modelQualityCenter << endl;
}

//碰撞检测
void GenerateMesh::collisionCheck()
{
	collisionArea.clear();
	//bool isCrashed = false;
	//TODO 根据表面体素属性，检测碰撞
	map<string, VoxelAttribute> spaceAttribute;		//记录空间占用情况：占有坐标与其对应体素
	for (int i = 0; i < pixelModel->surfaceVoxelCount; i++)
	{
		//当前表面体素各顶点公用属性
		int linkVertex = pixelModel->surfaceVoxelAttribute[i].linkVertex;
		int belongIndex = pixelModel->surfaceVoxelAttribute[i].belongIndex;
		int belongBone = pixelModel->surfaceVoxelAttribute[i].belongBone;
		double belongWeight = pixelModel->surfaceVoxelAttribute[i].belongWeight;

		for (int t = 0; t < 8; t++)
		{
			//当前体素的一个顶点的局部单位坐标 dx为单位长度
			int localX = (pixelModel->meshPixels[linkVertex][belongIndex].box[t][0] - modelMinPoint[0]) / pixelModel->dx;
			int localY = (pixelModel->meshPixels[linkVertex][belongIndex].box[t][1] - modelMinPoint[1]) / pixelModel->dx;
			int localZ = (pixelModel->meshPixels[linkVertex][belongIndex].box[t][2] - modelMinPoint[2]) / pixelModel->dx;
			string localIndex;	//当前顶点即将占据的空间坐标
			stringstream ss;
			ss << "[" << localX << "," << localY << "," << localZ << "]";
			ss >> localIndex;

			map<string, VoxelAttribute>::iterator ite = spaceAttribute.find(localIndex);
			if (ite != spaceAttribute.end())
			{
				//空间已有体素信息
				int originBelongBone = spaceAttribute[localIndex].belongBone;
				double originBelongWeight = spaceAttribute[localIndex].belongWeight;
				int originLinkVertex = spaceAttribute[localIndex].linkVertex;
				int originBelongIndex = spaceAttribute[localIndex].belongIndex;
				if (belongBone != originBelongBone)
				{
					if (belongWeight > 0.95 || originBelongWeight > 0.95)
					{
						//cout << "\n-----------发生碰撞！！" << endl;
						//cout << "belongBone   : " << belongBone << " : " << originBelongBone << endl;
						//cout << "belongWeight : " << belongWeight << " : " << originBelongWeight << endl;
						//cout << "mapIndex     : " << localIndex << " : " << ite->first << endl;
						//cout << endl;
						//isCrashed = true;
						map<int, vector<int>>::iterator collisionIte = collisionArea.find(linkVertex);
						if (collisionIte != collisionArea.end())
						{
							collisionArea[linkVertex].push_back(belongIndex);
						}
						else
						{
							vector<int> tempAdd;
							tempAdd.push_back(belongIndex);
							collisionArea[linkVertex] = tempAdd;
						}
						break;
					}
				}
			}
			else
			{
				spaceAttribute[localIndex] = pixelModel->surfaceVoxelAttribute[i];
			}
			//if (isCrashed)
			//	break;
		}
	}
}

//绘制模型
void GenerateMesh::drawMesh()
{
	Vector3 normal;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < (int)meshVertices->edges.size(); ++i) {
		int v = meshVertices->edges[i].vertex;
		const Vector3 &p = meshVertices->vertices[v].pos;

		if (!flatShading) {
			normal = meshVertices->vertices[v].normal;
			glNormal3d(normal[0], normal[1], normal[2]);
		}
		else if (i % 3 == 0) {
			const Vector3 &p2 = meshVertices->vertices[meshVertices->edges[i + 1].vertex].pos;
			const Vector3 &p3 = meshVertices->vertices[meshVertices->edges[i + 2].vertex].pos;

			normal = ((p2 - p) % (p3 - p)).normalize();
			glNormal3d(normal[0], normal[1], normal[2]);
		}

		//glColor3d(m_color[v][0], m_color[v][1], m_color[v][2]);
		if (verticesToStress[v])
			glColor3d(1.0, 0.0, 0.0);
		else
			if (verticesToSmooth[v])
				glColor3d(0.0, 1.0, 0.0);
			else
				glColor3d(0.5, 0.5, 0.5);
		glVertex3d(p[0] + trans[0], p[1] + trans[1], p[2] + trans[2]);
	}
	glEnd();
}

//添加变换
void GenerateMesh::addSkeletonTran(int numSkele, double scale)
{
	changeBone addOne;
	addOne.numOfBone = numSkele;
	addOne.scale = scale;
	cBones.push_back(addOne);
}

//清空变换
void GenerateMesh::clearSkeletonVector()
{
	cBones.clear();
}

//清空模型变化 
void GenerateMesh::clearModelChange()
{

	for (int i = 0; i < bonePointCount; i++)
	{
		changedIndex[i] = false;
	}

	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		meshVertices->vertices[i].pos[0] = originMeshVertices->vertices[i].pos[0];
		meshVertices->vertices[i].pos[1] = originMeshVertices->vertices[i].pos[1];
		meshVertices->vertices[i].pos[2] = originMeshVertices->vertices[i].pos[2];


		delta_skeleton[i][0] = 0.0;
		delta_skeleton[i][1] = 0.0;
		delta_skeleton[i][2] = 0.0;
		delta_Custom[i][0] = 0.0;
		delta_Custom[i][1] = 0.0;
		delta_Custom[i][2] = 0.0;
		delta_Energy[i][0] = 0.0;
		delta_Energy[i][1] = 0.0;
		delta_Energy[i][2] = 0.0;
	}

	for (int i = 0; i < embedding.size(); i++)
	{
		if (i == 14)
		cout << i << " " << embedding[i] << endl;
	}
	embedding.clear();
	for (int i = 0; i < originData.embedding.size(); i++)
	{
		embedding.push_back(originData.embedding[i]);
	}
	for (int i = 0; i < embedding.size(); i++)
	{
		if (i == 14)
		cout << i << " " << embedding[i] << endl;
	}

	verticesToSmooth.clear();
	Smooth_size = 0;
	verticesToStress.clear();
	Stress_size = 0;
	stressArea.clear();

	//寻找点击范围内的点
	//当前块序号从0开始，顶点对应键值从1开始
	area_smooth = 0;
	area_stress = 0;
	flaging = 0;

	cout << "Model has been cleared !" << endl;
	updateModelCenter();
	initRotateAngleArray();
}

//获取模型质心点，用于camera旋转
Vector3 GenerateMesh::getModelCenter()
{
	//int verticeCount = meshVertices->vertices.size();
	//Vector3 sum = Vector3(0, 0, 0);
	//for (int i = 0; i < verticeCount; i++)
	//{
	//	sum += meshVertices->vertices[i].pos;
	//}
	//Vector3 result = Vector3(sum[0] / verticeCount, sum[1] / verticeCount, sum[2] / verticeCount);
	//return result;
	return modelQualityCenter;
}


//寻找点击范围内的点
vector<int> temp_stress_find;

void GenerateMesh::FindVertex(double x, double y)
{
	GLdouble v_x, v_y, v_z;
	cout << "****************" << endl;

	if (!StressState && !SmoothState)
		return;

	if (StressState)
	{
		area_stress++;
	}
	if (SmoothState)
	{
		area_smooth++;
	}

	double *temp_distance = new double[2*(bonePointCount - 1)];
	int index = 0;
	for (int i = 0; i < bonePointCount - 1; i++)
		temp_distance[2*i] = 9999;
	for (int i = 0; i < bonePointCount - 1; i++)
		temp_distance[2*i+1] = 0;

	//cout << VertexNeighbor.size() << endl;
	//cout << VertexNeighbor[0].size() << endl;
	//system("pause");

	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		gluProject(meshVertices->vertices[i].pos[0], meshVertices->vertices[i].pos[1], meshVertices->vertices[i].pos[2], modelView, projView, viewView, &v_x, &v_y, &v_z);

		if ((v_x >= x - 5) && (v_x <= x + 5))
			if ((v_y >= y - 5) && (v_y <= y + 5))
			{
				counting[flaging++] = i;

				//仅留正面投影点
				if ((meshVertices->vertices[i].normal[0] * camera.xeye + meshVertices->vertices[i].normal[1] * camera.yeye + meshVertices->vertices[i].normal[2] * camera.zeye) > 0)
				{
					cout << i << " : " << v_x << " , " << v_y << " , " << v_z << endl;
					//cout << VertexNeighbor.size() << endl;
					//cout << VertexNeighbor[0].size() << endl;
					//system("pause");
					if (SmoothState)
					{
						verticesToSmooth[i] = area_smooth;
						Smooth_size++;
					}	
					if (StressState)
					{
						for (int j = 0; j < bonePointCount - 1; j++)
						{
							//标记的每一个点在每一个骨骼上的投影距离与已有最值进行比较
							double temp = getDistance(proj(originData.embedding[skeletonNodeInformation[j + 1].parentIndex], originData.embedding[j + 1], copy_MeshVertices->vertices[i].pos), copy_MeshVertices->vertices[i].pos);
							if (temp > temp_distance[2 * j + 1])
								temp_distance[2 * j + 1] = temp;
							if (temp < temp_distance[2 * j])
								temp_distance[2 * j] = temp;

							if (temp < 0.00005)
							{
								cout << "distance lower than 0 ? "<< temp << endl;
								system("pause");
							}
						}
						verticesToStress[i] = area_stress;
						Stress_size++;

						temp_stress_find.push_back(i);
					}
				}
			}
	}

	//cout << VertexNeighbor.size() << endl;
	//for (int i = 0; i < VertexNeighbor.size(); i++)
	//	cout << VertexNeighbor[i].size() << endl;
	//system("pause");

	if (StressState)
	{
		stressArea.resize(area_stress);
		for (int j = 0; j < bonePointCount - 1; j++)
		{
			stressArea[area_stress-1].push_back(temp_distance[2*j]);
			stressArea[area_stress-1].push_back(temp_distance[2*j+1]);
			if (temp_distance[2 * j] < 0.00005)
			{
				cout << "stressArea distance lower than 0 ?" << endl;
				system("pause");
			}
		}
	}


	cout << "****************" << endl;
	cout << "_________ " << x << " , " << y << endl;
	cout << "****************" << endl;

}


#define WEIGHT_P 1
#define WEIGHT_FH 0.3
#define WEIGHT_FL 0.2
#define WEIGHT_S 10
#define WEIGHT_A 1

//#define WEIGHT_P 2
//#define WEIGHT_FH 1.3
//#define WEIGHT_FL 0.2
//#define WEIGHT_S 5
//#define WEIGHT_A 7

#define SMOOTH_A 0.5
#define SMOOTH_B 0.2

#define SE_STRESS 0.5
//总能量计算处理
void GenerateMesh::CountingEnergy() 
{
	int vertices_size = meshVertices->vertices.size();
	int row = vertices_size * (2 + bonePointCount - 2 + 1) + Stress_size * (bonePointCount - 1);	//bonePointCount-2为除去头部的骨骼数
	int col = vertices_size;



	vector<Vector3> temp_vertices;

	fstream innn;
	innn.open("aaaa111.txt", ios::out);

	if (!innn)
	{
		system("pause");
	}

	cout << "Begin to count Energy :" << endl;

	//将三个坐标分开考虑
	for (int k = 0; k < 3; k++)
	{
		cout << "Counting " << k << " @@@ " << endl;

		cout << "Calculate x ..." << endl;
		TaucsMatrix M_x(row, col, false);

		for (int j = 0; j < col; j++)
		{
			///////////1
			double temp = 0;
			temp = sqrt(WEIGHT_P);
			M_x.set_coef(vertices_size - 1 - j, j, temp);
			//cout << "@1 success" << endl;

			/////////2
			temp = sqrt(WEIGHT_FH * originData.attachment->getWeights(j)[HEAD - 1]);
			M_x.set_coef(vertices_size * 2 - 1 - j, j, temp);
			//cout << "@2 success" << endl;

			///////3
			double skeleton_weight = 0;
			for (int t = 1; t < bonePointCount; t++)
			{
				if (t == HEAD)
					continue;
				skeleton_weight = WEIGHT_FL * originData.attachment->getWeights(j)[t - 1];
				temp = sqrt(skeleton_weight);
				if (t > HEAD)
				{
					int tempRow = vertices_size * (2 + t - 1) - 1 - j;
					M_x.set_coef(tempRow, j, temp);
				}
				else
					M_x.set_coef(vertices_size * (2 + t) - 1 - j, j, temp);
			}
			//cout << "@3 success" << endl;
			
			/////////4
			if (verticesToSmooth.count(j))
			{
				temp = sqrt(WEIGHT_S * SMOOTH_A);
				M_x.set_coef(vertices_size * (2 + bonePointCount - 2 + 1) - 1 - j, j, temp);
			}
			else
			{
				temp = sqrt(WEIGHT_S * SMOOTH_B);
				M_x.set_coef(vertices_size * (2 + bonePointCount - 2 + 1) - 1 - j, j, temp);
			}
			
		}
		cout << "@123 success" << endl;


		/////////4
		//遍历第J行(自上而下)的每一列（i）
		for (int j = 0; j < vertices_size; j++)
		{
			double temp = 0;

			for (int i = 0; i < vertices_size; i++)
			{
				if (i == j)
					continue;

				bool isVertexNeighbor = false;

				if (VertexNeighbor.size() == 0)
				{
					cout << "no vertex input to VertexNeighbor vector ?" << endl;
					system("pause");
				}
				if (VertexNeighbor.size() < j + 1)
				{
					cout << "VertexNeighbor vector cannot include vertex " << j << " ? !" << endl;
					cout << "VertexNeighbor 's size : " << VertexNeighbor.size() << "   j : " << j << endl;
					system("pause");
				}
				if (VertexNeighbor[j].size() == 0)
				{
					cout << j <<" vertex hasn't VertexNeighbor vector ?" << endl;
					system("pause");
				}
				for (int t = 0; t < VertexNeighbor[j].size(); t++)
				{
					if (VertexNeighbor[j][t] == i)
					{
						isVertexNeighbor = true;
						break;
					}
				}
				//是否处于当前点旁环上
				if (isVertexNeighbor)
				{
					if (verticesToSmooth.count(i))
						temp = -(sqrt(WEIGHT_S * SMOOTH_A) / VertexNeighbor[j].size());
					else
						temp = -(sqrt(WEIGHT_S * SMOOTH_B) / VertexNeighbor[j].size());
					//system("pause");
					M_x.set_coef(vertices_size * (2 + bonePointCount - 2 + 1) - 1 - j, i, temp);
				}
			}
		}
		cout << "@4 success" << endl;

		/////////5
		//自下而上填充矩阵
		if (Stress_size != 0)
		{
			double temp;
			for (int b = 0; b < bonePointCount - 1; b++)
			{
				int line = 0;
				for (map<int, int>::iterator stress_it = verticesToStress.begin(); stress_it != verticesToStress.end(); stress_it++)
				{
					if (!stress_it->second)
						continue;

					line++;
					//system("pause");
					temp = sqrt(WEIGHT_A * originData.attachment->getWeights(stress_it->first)[b]);
					M_x.set_coef((vertices_size * (2 + bonePointCount - 2 + 1) - 1) + b * Stress_size + line, stress_it->first, temp);
					//system("pause");
				}
			}
			cout << "@5 success" << endl;
		}


		cout << "Calculate b..." << endl;
		std::vector<double> b(row);
		int i;

		//第一因素
		if (delta_skeleton.size() != vertices_size || copy_MeshVertices->vertices.size() != vertices_size)
		{
			cout << "BUG ALARM !" << endl;
			system("pause");
		}
		for (i = 0; i < vertices_size; i++)
		{
			b[vertices_size - 1 - i] = sqrt(WEIGHT_P) * (copy_MeshVertices->vertices[i].pos[k] + delta_skeleton[i][k]);
		}
		cout << "@1 success" << endl;

		//第二因素
		for (i = 0; i < vertices_size; i++)
		{
			b[vertices_size * 2 - 1 - i] = sqrt(WEIGHT_FH * originData.attachment->getWeights(i)[HEAD - 1]) * (copy_MeshVertices->vertices[i].pos[k] + ExpectGrow[i][HEAD][k]);
		}
		cout << "@2 success" << endl;

		//system("pause");

		//第三因素
		for (i = 0; i < vertices_size; i++)
		{
			double skeleton_b = 0;
			for (int j = 1; j < bonePointCount; j++)
			{
				if (j == HEAD)
					continue;
				skeleton_b = sqrt(WEIGHT_FL * originData.attachment->getWeights(i)[j - 1]) * (copy_MeshVertices->vertices[i].pos[k] + ExpectGrow[i][j][k]);
				if (j > HEAD)
				{
					int tempRow = vertices_size * (2 + j - 1) - 1 - i;
					b[tempRow] = skeleton_b;
				}
				else
					b[vertices_size * (2 + j) - 1 - i] = skeleton_b;
			}
		}
		cout << "@3 success" << endl;

		////第四因素对b没影响

		//第五因素
		//从下而上填充
		if (Stress_size != 0)
		{
			for (int bone = 0; bone < bonePointCount - 1; bone++)
			{
				int line = 0;
				for (map<int, int>::iterator stress_it = verticesToStress.begin(); stress_it != verticesToStress.end(); stress_it++)
				{
					if (!stress_it->second)
						continue;

					line++;
					double d_vi = getDistance(
						proj(originData.embedding[skeletonNodeInformation[bone + 1].parentIndex], originData.embedding[bone + 1], copy_MeshVertices->vertices[stress_it->first].pos),
						copy_MeshVertices->vertices[stress_it->first].pos);
					double d_min = stressArea[verticesToStress[stress_it->first] - 1][2 * bone];
					double d_max = stressArea[verticesToStress[stress_it->first] - 1][2 * bone + 1];

					double temp_vi_Bi = embedding[skeletonNodeInformation[bone + 1].parentIndex][k]
						+ (proj(originData.embedding[skeletonNodeInformation[bone + 1].parentIndex], originData.embedding[bone + 1], copy_MeshVertices->vertices[stress_it->first].pos)[k] - originData.embedding[skeletonNodeInformation[bone + 1].parentIndex][k]) / (originData.embedding[bone + 1][k] - originData.embedding[skeletonNodeInformation[bone + 1].parentIndex][k])
						* (embedding[bone + 1][k] - embedding[skeletonNodeInformation[bone + 1].parentIndex][k])
						+ getDistance(embedding[skeletonNodeInformation[bone + 1].parentIndex], embedding[bone + 1]) / getDistance(originData.embedding[skeletonNodeInformation[bone + 1].parentIndex], originData.embedding[bone + 1])
						* (SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1)
						* (copy_MeshVertices->vertices[stress_it->first].pos[k] - proj(originData.embedding[skeletonNodeInformation[bone + 1].parentIndex], originData.embedding[bone + 1], copy_MeshVertices->vertices[stress_it->first].pos)[k]);

					//if (SMdata.attachment->getWeights(stress_it->first)[bone] > 0.5 && temp_vi_Bi <= 0)
					//{
					//	//这里是假设正常情况下点的坐标均大于0
					//	cout << stress_it->first << " : " << copy_m->vertices[stress_it->first].pos[k] + delta_skeleton[stress_it->first][k] << " : " << temp_vi_Bi << " temp_vi_Bi should not lower than 0 !" << endl;
					//	system("pause");
					//	cout << "checking ..." << endl;
					//	cout << "aj'               : " << embedding[preIndex[bone + 1]][k] << endl;
					//	cout << "t_bj_vi           : " << (proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k] - SMdata.embedding[preIndex[bone + 1]][k]) / (SMdata.embedding[bone + 1][k] - SMdata.embedding[preIndex[bone + 1]][k]) << endl;
					//	cout << "(bj' - aj')       : " << (embedding[bone + 1][k] - embedding[preIndex[bone + 1]][k]) << endl;
					//	cout << "RJ                : " << getDistance(embedding[preIndex[bone + 1]], embedding[bone + 1]) / getDistance(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1]) << endl;
					//	cout << "(SE * rj + 1)     : " << (SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1) << endl;
					//	if ((SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1) < 0)
					//	{
					//		cout << "    d_vi          : " << d_vi << endl;
					//		cout << "    d_min         : " << d_min << endl;
					//		cout << "    d_max         : " << d_max << endl;
					//	}
					//	cout << "(vj - proj_bj_vi) : " << (copy_m->vertices[stress_it->first].pos[k] - proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k]) << endl;
					//	if ((copy_m->vertices[stress_it->first].pos[k] - proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k]) < 0)
					//	{
					//		cout << "    vj            : " << copy_m->vertices[stress_it->first].pos[k] << endl;
					//		cout << "    proj_bj_vi    : " << proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k] << endl;
					//	}
					//	system("pause");
					//}
					b[(vertices_size * (2 + bonePointCount - 2 + 1) - 1) + bone * Stress_size + line] = sqrt(WEIGHT_A * originData.attachment->getWeights(stress_it->first)[bone]) * temp_vi_Bi;
				}
			}
		}
		cout << "@5 success" << endl;


		////第五因素
		//for (i = 4 * m->vertices.size(); i < 5 * m->vertices.size(); i++)
		//{
		//	if (verticesToStress[i - 4 * vertices_size])
		//	{
		//		double temp_vertice_vi_Bj_total = 0;

		//		for (int j = 1; j < bonePointCount; j++)
		//		{
		//			double d_vi = getDistance(proj(embedding[preIndex[j]], embedding[j], copy_m->vertices[i - 4 * vertices_size].pos), copy_m->vertices[i - 4 * vertices_size].pos);
		//			double d_min = stressArea[verticesToStress[i - 4 * vertices_size] - 1][2 * (j - 1)];
		//			double d_max = stressArea[verticesToStress[i - 4 * vertices_size] - 1][2 * (j - 1) + 1];

		//			temp_vertice_vi_Bj_total += WEIGHT_A * SMdata.attachment->getWeights(i - 4 * vertices_size)[j - 1]
		//				* (embedding[preIndex[j]][k]
		//				+ (proj(SMdata.embedding[preIndex[j]], SMdata.embedding[j], copy_m->vertices[i - 4 * vertices_size].pos)[k] - SMdata.embedding[preIndex[j]][k]) / (SMdata.embedding[j][k] - SMdata.embedding[preIndex[j]][k])
		//				* (embedding[j][k] - embedding[preIndex[j]][k])
		//				+ getDistance(embedding[preIndex[j]], embedding[j]) / getDistance(SMdata.embedding[preIndex[j]], SMdata.embedding[j])
		//				* (SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1)
		//				* (copy_m->vertices[i - 4 * vertices_size].pos[k] - proj(SMdata.embedding[preIndex[j]], SMdata.embedding[j], copy_m->vertices[i - 4 * vertices_size].pos)[k]));
		//		}

		//		b[i] = temp_vertice_vi_Bj_total;
		//	}
		//	else
		//		b[i] = 0;
		//}



		//for (int i = 0; i < row; i++)
		//{
		//	//填充当前行的每一列
		//	for (int j = 0; j < col; j++)
		//	{
		//		double temp = 0;
		//		if (j == i)
		//		{
		//			double skeleton_weight = 0;
		//			for (int t = 1; t < bonePointCount; t++)
		//			{
		//				if (t == HEAD)
		//					continue;
		//				skeleton_weight += SMdata.attachment->getWeights(i)[t - 1];
		//			}
		//			temp = WEIGHT_P + WEIGHT_FH * SMdata.attachment->getWeights(i)[HEAD - 1]
		//				+ WEIGHT_FL * skeleton_weight;;
		//			if (verticesToSmooth.count(i))
		//				temp += WEIGHT_S * SMOOTH_A;
		//			else
		//				temp += WEIGHT_S * SMOOTH_B;
		//			if (verticesToStress[i])
		//			{
		//				skeleton_weight += SMdata.attachment->getWeights(i)[HEAD - 1];
		//			}
		//			temp += WEIGHT_A * skeleton_weight;

		//			if (temp*temp > 0)
		//				M_x.set_coef(i, j, temp);
		//		}
		//		else
		//		{
		//			bool isNeighbor = false;
		//			for (int t = 0; t < neighbor[i].size(); t++)
		//			{
		//				if (neighbor[i][t] == j)
		//				{
		//					isNeighbor = true;
		//					break;
		//				}
		//			}

		//			//是否处于当前点旁环上
		//			if (isNeighbor)
		//			{
		//				if (verticesToSmooth.count(i))
		//					temp = -(WEIGHT_S * SMOOTH_A / neighbor[i].size());
		//				else
		//					temp = -(WEIGHT_S * SMOOTH_B / neighbor[i].size());
		//			}

		//			if (temp*temp > 0)
		//				M_x.set_coef(i, j, temp);
		//		}
		//	}
		//}

		//cout << "Calculate b..." << endl;
		//std::vector<double> b(row);
		//double temp;
		//for (int i = 0; i < row; i++)
		//{
		//	temp = 0;
		//	double skeleton_b = 0;
		//	for (int j = 1; j < bonePointCount; j++)
		//	{
		//		if (j == HEAD)
		//			continue;
		//		skeleton_b += SMdata.attachment->getWeights(i)[j - 1] * ExpectGrow[i][j][k];
		//	}
		//	temp += (WEIGHT_P * (copy_m->vertices[i].pos[k] + delta_skeleton[i][k]) + WEIGHT_FH * SMdata.attachment->getWeights(i)[HEAD - 1] * ExpectGrow[i][HEAD][k]
		//		+ WEIGHT_FL * skeleton_b);
		//	
		//	if (verticesToStress[i])
		//	{
		//		double temp_vertice_vi_Bj_total = 0;

		//		for (int j = 1; j < bonePointCount; j++)
		//		{
		//			double d_vi = getDistance(proj(embedding[preIndex[j]], embedding[j], copy_m->vertices[i].pos), copy_m->vertices[i].pos);
		//			double d_min = stressArea[verticesToStress[i]-1][2 * (j - 1)];
		//			double d_max = stressArea[verticesToStress[i]-1][2 * (j - 1) + 1];

		//			temp_vertice_vi_Bj_total += WEIGHT_A * SMdata.attachment->getWeights(i)[j - 1]
		//				* (embedding[preIndex[j]][k]
		//				+ (proj(SMdata.embedding[preIndex[j]], SMdata.embedding[j], copy_m->vertices[i].pos)[k] - SMdata.embedding[preIndex[j]][k]) / (SMdata.embedding[j][k] - SMdata.embedding[preIndex[j]][k])
		//				* (embedding[j][k] - embedding[preIndex[j]][k])
		//				+ getDistance(embedding[preIndex[j]], embedding[j]) / getDistance(SMdata.embedding[preIndex[j]], SMdata.embedding[j])
		//				* (SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1)
		//				* (copy_m->vertices[i].pos[k] - proj(SMdata.embedding[preIndex[j]], SMdata.embedding[j], copy_m->vertices[i].pos)[k]));
		//		}

		//		temp += temp_vertice_vi_Bj_total;
		//	}

		//	b[i] = temp;
		//}

		cout << "Begin to calculate !" << endl;
		std::vector<double> x;
		bool success = TaucsSolver::solve_linear_least_square(M_x, b, x);
		if (success)
		{
			std::cout << "linear least square " << k << "   Success !" << std::endl;
		}
		else
		{
			std::cout << "linear least square " << k << "   Error !" << std::endl;
			system("pause");
		}

		for (int i = 0; i < meshVertices->vertices.size(); i++)
		{
			int temp_size;
			if (k == 0)
			{
				temp_size = temp_vertices.size();
				temp_vertices.resize(temp_size + 1);
				temp_vertices[temp_size][k] = x[i];
			}
			else
			{
				temp_vertices[i][k] = x[i];
			}
		}
	}

	fstream outinging;
	outinging.open("123456789123.txt", ios::out);


	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		delta_Energy[i][0] = temp_vertices[i][0] - copy_MeshVertices->vertices[i].pos[0];
		delta_Energy[i][1] = temp_vertices[i][1] - copy_MeshVertices->vertices[i].pos[1];
		delta_Energy[i][2] = temp_vertices[i][2] - copy_MeshVertices->vertices[i].pos[2];

		for (int tt = 0; tt < flaging; tt++)
		{
			if (i == counting[tt])
			{
				outinging << counting[tt] << " : " << temp_vertices[i][0] << " , " << temp_vertices[i][1] << " , " << temp_vertices[i][2] << endl;
				break;
			}
		}
		//if (j == 789 || j == 3725 || j == 523 || j == 1603 || j == 1604 || j == 3084 || j == 9341 || j == 11723)
		//{
		//	innn << "  @ " << i << "  " << temp_vertices[i][0] << " " << temp_vertices[i][1] << " " << temp_vertices[i][2] << endl;
		//}
	}


	//innn.close();

	fstream ofile;
	ofile.open("0011.txt", ios::out);
	if (!ofile)
	{
		cout << "ofile cannot open" << endl;
		system("pause");
	}

	for (int i = 0; i < temp_stress_find.size(); i++)
	{
		ofile << "x: " << delta_Energy[i][0] << "  y: " << delta_Energy[i][1] << "  z: " << delta_Energy[i][2] << endl;
	}

	ofile.close();



	//for (int k = 0; k < 3; k++)
	//{
	//	for (int i = 0; i < m->vertices.size(); i++)
	//	{
	//		m->vertices[i].pos[k] = temp_vertices[i][k];

	//		for (int tt = 0; tt < flaging; tt++)
	//		{
	//			if (i == counting[tt])
	//			{
	//				outinging << counting[tt] << " : " << temp_vertices[i][0] << " , " << temp_vertices[i][1] << " , " << temp_vertices[i][2] << endl;
	//				break;
	//			}
	//		}
	//	}
	//}

	outinging.close();

	cout << "Energy smooth Success !" << endl;
}