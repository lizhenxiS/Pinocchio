

#include "GenerateMesh.h"
#include "GenerateTool.h"
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <glut.h>
#include "taucs_matrix.h"
#include "taucs_solver.h"
#include <time.h>
#include "StandardModel.h"

using namespace std;

bool* changedIndex;
bool changeFromMap = false;
bool SmoothState = false;
bool StressState = false;

fstream*  view;
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

//读取人体模型初始化数据
GenerateMesh::GenerateMesh(const string& file)
{
	cout << "Begin to generate Mesh :" << endl;

	modelFile = file;
	m = new Mesh(file);
	//copy_m = new Mesh(file);

	int verts = m->vertices.size();

	view = new fstream("aaaaaview.txt", ios::out);
	if (!view)
		system("pause");

	color_Mesh[0] = 0.0;
	color_Mesh[1] = 1.0;
	color_Mesh[2] = 1.0;
	color_Skele[0] = 1.0;
	color_Skele[1] = 1.0;
	color_Skele[2] = 0.0;
	color_Tran[0] = 1.0;
	color_Tran[1] = 0.0;
	color_Tran[2] = 0.0;
	thin = 5.0;
	flatShading = false;

	for (int i = 0; i < m->vertices.size(); i++)
	{
		delta_skeleton.push_back(Vector3(0.0, 0.0, 0.0));
		delta_Custom.push_back(Vector3(0.0, 0.0, 0.0));
		delta_Energy.push_back(Vector3(0.0, 0.0, 0.0));
	}

	process();

	if (verts != m->vertices.size())
		system("pause");


	changedIndex = new bool[embedding.size()];
	for (int i = 0; i < embedding.size(); i++)
	{
		changedIndex[i] = false;
	}


	Smooth_size = 0;
	Stress_size = 0;
	cout << m->edges.size() << " ___half" << endl;
	cout << "Generate Mesh Success !" << embedding.size() << endl;
	//this->BvhData = new BVHData(embedding, preIndex);
	//this->BvhData->printData();
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

	for (int i = 0; i < m->vertices.size(); i++)
	{
		out_obj << "v " << m->vertices[i].pos[0] << " " << m->vertices[i].pos[1] << " " << m->vertices[i].pos[2] << endl;


		if (m->vertices[i].pos[0] < small_x)
		{
			small_x_index = i;
			small_x = m->vertices[i].pos[0];
		}
		if (m->vertices[i].pos[1] < small_y)
		{
			small_y_index = i;
			small_y = m->vertices[i].pos[1];
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

//数据获取
void GenerateMesh::process()
{
	//传入参数的信息存储
	ArgData a;

	m->normalizeBoundingBox();
	//copy_m->normalizeBoundingBox();

	m->computeVertexNormals();
	//copy_m->computeVertexNormals();

	copy_m = new MyVerticeMesh(*m);

	//初始化骨架
	Skeleton given = HumanSkeleton();
	given.scale(a.skelScale * 0.7);

	//system("time");
	

	if (!a.noFit) { //do everything
		//组合骨骼人体数据
		SMdata = autorig(given, *m);
	}

	//system("time");
	

	countBone = SMdata.embedding.size();

	skeletonNodeInformation = new SkeletonNode[countBone];

	for (int y = 1; y < SMdata.embedding.size(); y++)
	{
		//y为末端点，given.fPrev()[y]为前续端点
		skeletonNodeInformation[y].parentIndex = given.fPrev()[y];
		skeletonNodeInformation[given.fPrev()[y]].childrenIndex.push_back(y);
	}

	//SELFDEBUG;

	//for (int i = 0; i < countBone; i++)
	//{
	//	vector<int> tempp = getAfterSkeletonIndex(skeletonNodeInformation ,i);
	//	for (int j = 0; j < tempp.size(); j++)
	//	{
	//		cout << tempp[j] << " ";
	//	}
	//	cout << endl;
	//}

	//SELFDEBUG;

	//骨骼模型获取
	for (int y = 0; y < SMdata.embedding.size(); y++)
	{
		embedding.push_back(SMdata.embedding[y]);
		CurEmbedding.push_back(SMdata.embedding[y]);
	}

	//各骨骼距离规范化energy
	deltaEnergy = new double[countBone];

	//除去 0 下标
	deltaEnergy[0] = -1;

	//各骨骼期待向规范方向增长比例
	expectGrowScale = new double[countBone];
	for (int i = 1; i < countBone; i++)
	{
		expectGrowScale[i] = 0;
	}

	vector<Vector3> tempInitbone;
	Vector3 temppoint;
	temppoint[0] = 0.0;
	temppoint[1] = 0.0;
	temppoint[2] = 0.0;

	for (int j = 0; j < countBone; j++)
	{
		tempInitbone.push_back(temppoint);
	}
	for (int i = 0; i < copy_m->vertices.size(); i++)
	{
		ExpectGrow.push_back(tempInitbone);
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
	for (int i = 0; i < m->vertices.size(); i++)
	{
		//每个骨骼期望该点所在位置
		Vector3 temp;

		for (int j = 1; j < countBone; j++)
		{
			//头部期望的点增长
			if (j == HEAD)
			{
				Vector3 m_delta;
				m_delta[0] = m->vertices[i].pos[0] + delta_skeleton[i][0];
				m_delta[1] = m->vertices[i].pos[1] + delta_skeleton[i][1];
				m_delta[2] = m->vertices[i].pos[2] + delta_skeleton[i][2];

				for (int k = 0; k < 3; k++)
				{
					temp[k] = embedding[HEAD][k] + ((m_delta[k] - embedding[HEAD][k]) * getDistance(embedding[HEAD], embedding[skeletonNodeInformation[HEAD].parentIndex])) / getDistance(m_delta, embedding[HEAD]) - m_delta[k];
				}
			}
			else
				//普通臂骨-----------假设规范化臂骨长宽比 3：2  __________________
			{
				Vector3 m_delta;
				m_delta[0] = m->vertices[i].pos[0] + delta_skeleton[i][0];
				m_delta[1] = m->vertices[i].pos[1] + delta_skeleton[i][1];
				m_delta[2] = m->vertices[i].pos[2] + delta_skeleton[i][2];

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
	for (int i = 0; i < m->vertices.size(); i++)
	{
		//现对每个期望点与实际点作差后加权处理，得到期望和
		Vector3 tempExpect;
		for (int k = 0; k < 3; k++)
		{
			tempExpect[k] = 0;
			for (int j = 1; j < countBone; j++)
			{
				tempExpect[k] += SMdata.attachment->getWeights(i)[j-1] * ExpectGrow[i][j][k] * expectGrowScale[j];
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

//骨骼长度变化后模型映射变化处理
void GenerateMesh::extendMesh()
{
	////BVHData 更新
	//delete BvhData;
	//BvhData = new BVHData(embedding, preIndex);

	//顶点临时替代
	Vector3 temp;
	//顶点数
	int VerticesSize = m->vertices.size();

	int weightSize;
	Vector3 projTemp;
	double RjTemp;
	Vector3 SBjTemp;
	double EBjViTemp;
	double weight;


	for (int i = 0; i < VerticesSize; i++)
	{
		temp[0] = 0.0;
		temp[1] = 0.0;
		temp[2] = 0.0;
		weightSize = SMdata.attachment->getWeights(i).size();

		/*?????????????????*/ //weightSize=17  以及 j 取值不确定 bone_a bone_b 选取
		//假定 bone_a 为起点  bone_b 为前点
		for (int j = 0; j < weightSize; j++)
		{
			//第j+1根骨骼
			projTemp = proj(CurEmbedding[skeletonNodeInformation[j + 1].parentIndex], CurEmbedding[j + 1], copy_m->vertices[i].pos);
			RjTemp = Rj(CurEmbedding[skeletonNodeInformation[j + 1].parentIndex], CurEmbedding[j + 1], embedding[skeletonNodeInformation[j + 1].parentIndex], embedding[j + 1]);
			SBjTemp = SBj(CurEmbedding[skeletonNodeInformation[j + 1].parentIndex], CurEmbedding[j + 1], embedding[skeletonNodeInformation[j + 1].parentIndex], embedding[j + 1]);
			EBjViTemp = EBjVi(CurEmbedding[skeletonNodeInformation[j + 1].parentIndex], CurEmbedding[j + 1], copy_m->vertices[i].pos);
				weight = (0.5 + SMdata.attachment->getWeights(i)[j] * 10000.0) / 10000.0;

				for (int k = 0; k < 3; k++)
				{
					temp[k] += weight*(embedding[skeletonNodeInformation[j + 1].parentIndex][k] + (copy_m->vertices[i].pos[k] - CurEmbedding[skeletonNodeInformation[j + 1].parentIndex][k])
						+ (copy_m->vertices[i].pos[k] - projTemp[k])*(RjTemp - 1)
						+ EBjViTemp * SBjTemp[k]);
				}
		}

		delta_skeleton[i][0] = temp[0] - copy_m->vertices[i].pos[0];
		delta_skeleton[i][1] = temp[1] - copy_m->vertices[i].pos[1];
		delta_skeleton[i][2] = temp[2] - copy_m->vertices[i].pos[2];
	}
}


//转动骨骼
void GenerateMesh::rotateSkeleton(const int bone, double alpha, double beta)
{
	
}

//转动模型
void GenerateMesh::rotateMesh()
{
	for (int i = 0; i < countBone - 1; i++)
	{

	}
}

//将骨骼直接转换为所选模板
void GenerateMesh::changeSkeletonFromMap()
{
	dis_CG();

	for (int i = 1; i < countBone; i++)
	{
		double scale;
		scale = getDistance(cg_girl[i], cg_girl[skeletonNodeInformation[i].parentIndex]) / getDistance(SMdata.embedding[i], SMdata.embedding[skeletonNodeInformation[i].parentIndex]);
		
		changeSingleSkeleton(i, scale);

	}
}

//初始化记录骨骼旋转角数组
void GenerateMesh::initRotateAngleArray()
{
	for (int i = 0; i < BONECOUNT; i++)
	{
		rotateAngle[i][0] = 0;
		rotateAngle[i][1] = 0;
	}
}

//任意自由度改变骨骼，并相对静止牵动关联骨骼
//bone 为骨骼序数，从1开始
void GenerateMesh::changeSkeleton(const int bone, double alpha, double beta)
{
	//cout << "\n show standard..." << endl;
	StandardModel temp(SMdata, m, embedding, skeletonNodeInformation);

	double tempAlpha = alpha - rotateAngle[bone][0];
	double tempBeta = beta - rotateAngle[bone][1];
	rotateAngle[bone][0] = alpha;
	rotateAngle[bone][1] = beta;

	temp.rotateSkeleton(bone, embedding, tempAlpha, tempBeta);
	for (int j = 0; j < countBone; j++)
	{
		embedding[j] = temp.skeletonPoints[j];
	}
	temp.refreshMesh();
	//for (int i = 0; i < countBone - 1; i++)
	//{
	//	if (i != 12)
	//		continue;
	//	temp.rotateSkeleton(i, embedding);
	//	for (int j = 0; j < countBone; j++)
	//	{
	//		embedding[j] = temp.skeletonPoints[j];
	//	}
	//	temp.refreshMesh(embedding[i]);
	//}
	for (int i = 0; i < temp.meshVertices->vertices.size(); i++)
	{
		//cout << "old point:" << copy_m->vertices[i].pos;
		//cout << "   new pos: " << temp.meshVertices->vertices[i].pos << endl;
		copy_m->vertices[i].pos = temp.meshVertices->vertices[i].pos;
	}
}


//同步改变单一骨骼
void GenerateMesh::changeSingleSkeleton(const int boneNode, const double scale)
{
	if (((scale - 1) < 0.05) && ((scale - 1) > 0.05))
	{
		changedIndex[boneNode] = false;
	}
	else
		changedIndex[boneNode] = true;

	vector<int> concernNodes = getAfterSkeletonIndex(skeletonNodeInformation, boneNode);

	//int* beMoved = new int[countBone - 2];	//后续骨骼是否被牵动记录数组
	//for (int i = 0; i < (countBone - 2); i++)
	//{
	//	beMoved[i] = 0;
	//}
	//int afterList = aftIndex[boneNode];
	//int beMoved_index = 0;
	//while (afterList != -1)
	//{
	//	beMoved[beMoved_index] = afterList;
	//	afterList = aftIndex[afterList];
	//	beMoved_index++;
	//}


	//当前骨骼（不一定为初始骨骼）变化前位置记录
	double Temp_embed_X = embedding[boneNode][0];
	double Temp_embed_Y = embedding[boneNode][1];
	double Temp_embed_Z = embedding[boneNode][2];

	embedding[boneNode][0] =
		scale * (CurEmbedding[boneNode][0] - CurEmbedding[skeletonNodeInformation[boneNode].parentIndex][0])
		+ embedding[skeletonNodeInformation[boneNode].parentIndex][0];
	embedding[boneNode][1] =
		scale * (CurEmbedding[boneNode][1] - CurEmbedding[skeletonNodeInformation[boneNode].parentIndex][1])
		+ embedding[skeletonNodeInformation[boneNode].parentIndex][1];
	embedding[boneNode][2] =
		scale * (CurEmbedding[boneNode][2] - CurEmbedding[skeletonNodeInformation[boneNode].parentIndex][2])
		+ embedding[skeletonNodeInformation[boneNode].parentIndex][2];



	for (int i = 0; i < concernNodes.size(); i++)
	{
		int tempIndex = concernNodes[i];

		//此处对于每个被动关联骨骼都只处理一个端点，因为一个端点即代表该骨骼，剩余端点会由后续骨骼处理
		embedding[tempIndex][0] = embedding[tempIndex][0] + embedding[boneNode][0] - Temp_embed_X;
		embedding[tempIndex][1] = embedding[tempIndex][1] + embedding[boneNode][1] - Temp_embed_Y;
		embedding[tempIndex][2] = embedding[tempIndex][2] + embedding[boneNode][2] - Temp_embed_Z;
	}

	extendMesh();

	CountGrowCustom();
}


//绘制骨骼
void GenerateMesh::drawSkeleton()
{
	for (int i = 1; i < countBone; i++)
	{
		if (changedIndex[i])
			drawLine(embedding[i], embedding[skeletonNodeInformation[i].parentIndex], color_Tran, thin);
		else
			drawLine(embedding[i], embedding[skeletonNodeInformation[i].parentIndex], color_Skele, thin);
	}
}

//绘制骨骼结点
void GenerateMesh::drawSkeletonPoint()
{
	for (int i = 0; i < countBone; i++)
	{
		drawPoint(embedding[i], thin * 2, Vector3(0, 0, 1));
	}
}

//绘制模型
void GenerateMesh::drawMesh()
{
	for (int i = 0; i < m->vertices.size(); i++)
	{
		for (int k = 0; k < 3; k++)
			m->vertices[i].pos[k] = copy_m->vertices[i].pos[k] + delta_skeleton[i][k] + delta_Custom[i][k]; /*+ delta_Energy[i][k];*/
			/*m->vertices[i].pos[k] = copy_m->vertices[i].pos[k] + delta_Energy[i][k];*/
	}

	Vector3 normal;

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < (int)m->edges.size(); ++i) {
		int v = m->edges[i].vertex;
		const Vector3 &p = m->vertices[v].pos;

		if (!flatShading) {
			normal = m->vertices[v].normal;
			glNormal3d(normal[0], normal[1], normal[2]);
		}
		else if (i % 3 == 0) {
			const Vector3 &p2 = m->vertices[m->edges[i + 1].vertex].pos;
			const Vector3 &p3 = m->vertices[m->edges[i + 2].vertex].pos;

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

	for (int i = 0; i < countBone; i++)
	{
		changedIndex[i] = false;
	}

	for (int i = 0; i < m->vertices.size(); i++)
	{
		m->vertices[i].pos[0] = copy_m->vertices[i].pos[0];
		m->vertices[i].pos[1] = copy_m->vertices[i].pos[1];
		m->vertices[i].pos[2] = copy_m->vertices[i].pos[2];


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

	embedding.clear();
	for (int i = 0; i < SMdata.embedding.size(); i++)
	{
		embedding.push_back(SMdata.embedding[i]);
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

	double *temp_distance = new double[2*(countBone - 1)];
	int index = 0;
	for (int i = 0; i < countBone - 1; i++)
		temp_distance[2*i] = 9999;
	for (int i = 0; i < countBone - 1; i++)
		temp_distance[2*i+1] = 0;

	//cout << VertexNeighbor.size() << endl;
	//cout << VertexNeighbor[0].size() << endl;
	//system("pause");

	for (int i = 0; i < m->vertices.size(); i++)
	{
		gluProject(m->vertices[i].pos[0], m->vertices[i].pos[1], m->vertices[i].pos[2], modelView, projView, viewView, &v_x, &v_y, &v_z);

		if ((v_x >= x - 5) && (v_x <= x + 5))
			if ((v_y >= y - 5) && (v_y <= y + 5))
			{
				counting[flaging++] = i;

				//仅留正面投影点
				if ((m->vertices[i].normal[0] * camera.xeye + m->vertices[i].normal[1] * camera.yeye + m->vertices[i].normal[2] * camera.zeye) > 0)
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
						for (int j = 0; j < countBone - 1; j++)
						{
							//标记的每一个点在每一个骨骼上的投影距离与已有最值进行比较
							double temp = getDistance(proj(SMdata.embedding[skeletonNodeInformation[j + 1].parentIndex], SMdata.embedding[j + 1], copy_m->vertices[i].pos), copy_m->vertices[i].pos);
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
		for (int j = 0; j < countBone - 1; j++)
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
	int vertices_size = m->vertices.size();
	int row = vertices_size * (2 + countBone - 2 + 1) + Stress_size * (countBone - 1);	//countBone-2为除去头部的骨骼数
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
			temp = sqrt(WEIGHT_FH * SMdata.attachment->getWeights(j)[HEAD - 1]);
			M_x.set_coef(vertices_size * 2 - 1 - j, j, temp);
			//cout << "@2 success" << endl;

			///////3
			double skeleton_weight = 0;
			for (int t = 1; t < countBone; t++)
			{
				if (t == HEAD)
					continue;
				skeleton_weight = WEIGHT_FL * SMdata.attachment->getWeights(j)[t - 1];
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
				M_x.set_coef(vertices_size * (2 + countBone - 2 + 1) - 1 - j, j, temp);
			}
			else
			{
				temp = sqrt(WEIGHT_S * SMOOTH_B);
				M_x.set_coef(vertices_size * (2 + countBone - 2 + 1) - 1 - j, j, temp);
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
					M_x.set_coef(vertices_size * (2 + countBone - 2 + 1) - 1 - j, i, temp);
				}
			}
		}
		cout << "@4 success" << endl;

		/////////5
		//自下而上填充矩阵
		if (Stress_size != 0)
		{
			double temp;
			for (int b = 0; b < countBone - 1; b++)
			{
				int line = 0;
				for (map<int, int>::iterator stress_it = verticesToStress.begin(); stress_it != verticesToStress.end(); stress_it++)
				{
					if (!stress_it->second)
						continue;

					line++;
					//system("pause");
					temp = sqrt(WEIGHT_A * SMdata.attachment->getWeights(stress_it->first)[b]);
					M_x.set_coef((vertices_size * (2 + countBone - 2 + 1) - 1) + b * Stress_size + line, stress_it->first, temp);
					//system("pause");
				}
			}
			cout << "@5 success" << endl;
		}


		cout << "Calculate b..." << endl;
		std::vector<double> b(row);
		int i;

		//第一因素
		if (delta_skeleton.size() != vertices_size || copy_m->vertices.size() != vertices_size)
		{
			cout << "BUG ALARM !" << endl;
			system("pause");
		}
		for (i = 0; i < vertices_size; i++)
		{
			b[vertices_size - 1 - i] = sqrt(WEIGHT_P) * (copy_m->vertices[i].pos[k] + delta_skeleton[i][k]);
		}
		cout << "@1 success" << endl;

		//第二因素
		for (i = 0; i < vertices_size; i++)
		{
			b[vertices_size * 2 - 1 - i] = sqrt(WEIGHT_FH * SMdata.attachment->getWeights(i)[HEAD - 1]) * (copy_m->vertices[i].pos[k] + ExpectGrow[i][HEAD][k]);
		}
		cout << "@2 success" << endl;

		//system("pause");

		//第三因素
		for (i = 0; i < vertices_size; i++)
		{
			double skeleton_b = 0;
			for (int j = 1; j < countBone; j++)
			{
				if (j == HEAD)
					continue;
				skeleton_b = sqrt(WEIGHT_FL * SMdata.attachment->getWeights(i)[j - 1]) * (copy_m->vertices[i].pos[k] + ExpectGrow[i][j][k]);
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
			for (int bone = 0; bone < countBone - 1; bone++)
			{
				int line = 0;
				for (map<int, int>::iterator stress_it = verticesToStress.begin(); stress_it != verticesToStress.end(); stress_it++)
				{
					if (!stress_it->second)
						continue;

					line++;
					double d_vi = getDistance(proj(SMdata.embedding[skeletonNodeInformation[bone + 1].parentIndex], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos), copy_m->vertices[stress_it->first].pos);
					double d_min = stressArea[verticesToStress[stress_it->first] - 1][2 * bone];
					double d_max = stressArea[verticesToStress[stress_it->first] - 1][2 * bone + 1];

					double temp_vi_Bi = embedding[skeletonNodeInformation[bone + 1].parentIndex][k]
						+ (proj(SMdata.embedding[skeletonNodeInformation[bone + 1].parentIndex], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k] - SMdata.embedding[skeletonNodeInformation[bone + 1].parentIndex][k]) / (SMdata.embedding[bone + 1][k] - SMdata.embedding[skeletonNodeInformation[bone + 1].parentIndex][k])
						* (embedding[bone + 1][k] - embedding[skeletonNodeInformation[bone + 1].parentIndex][k])
						+ getDistance(embedding[skeletonNodeInformation[bone + 1].parentIndex], embedding[bone + 1]) / getDistance(SMdata.embedding[skeletonNodeInformation[bone + 1].parentIndex], SMdata.embedding[bone + 1])
						* (SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1)
						* (copy_m->vertices[stress_it->first].pos[k] - proj(SMdata.embedding[skeletonNodeInformation[bone + 1].parentIndex], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k]);

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
					b[(vertices_size * (2 + countBone - 2 + 1) - 1) + bone * Stress_size + line] = sqrt(WEIGHT_A * SMdata.attachment->getWeights(stress_it->first)[bone]) * temp_vi_Bi;
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

		//		for (int j = 1; j < countBone; j++)
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
		//			for (int t = 1; t < countBone; t++)
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
		//	for (int j = 1; j < countBone; j++)
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

		//		for (int j = 1; j < countBone; j++)
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

		for (int i = 0; i < m->vertices.size(); i++)
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


	for (int i = 0; i < m->vertices.size(); i++)
	{
		delta_Energy[i][0] = temp_vertices[i][0] - copy_m->vertices[i].pos[0];
		delta_Energy[i][1] = temp_vertices[i][1] - copy_m->vertices[i].pos[1];
		delta_Energy[i][2] = temp_vertices[i][2] - copy_m->vertices[i].pos[2];

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