

#include "GenerateMesh.h"
#include "GenerateTool.h"
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <glut.h>
#include "taucs_matrix.h"
#include "taucs_solver.h"
#include <time.h>

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

//Ѱ�ҵ����Χ�ڵĵ�
//��ǰ����Ŵ�0��ʼ�������Ӧ��ֵ��1��ʼ
int area_smooth = 0;
int area_stress = 0;
#define RANGE 5

int counting[100];
int flaging = 0;


//�������ڻ�
vector<vector<int>> VertexNeighbor;

//��ȡ����ģ�ͳ�ʼ������
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
	this->BvhData = new BVHData(embedding, preIndex);
	this->BvhData->printData();
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

//����ģ��
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

//���ݻ�ȡ
void GenerateMesh::process()
{
	//�����������Ϣ�洢
	ArgData a;

	m->normalizeBoundingBox();
	//copy_m->normalizeBoundingBox();

	m->computeVertexNormals();
	//copy_m->computeVertexNormals();

	copy_m = new MyVerticeMesh(*m);

	//��ʼ���Ǽ�
	Skeleton given = HumanSkeleton();
	given.scale(a.skelScale * 0.7);

	//system("time");
	

	if (!a.noFit) { //do everything
		//��Ϲ�����������
		SMdata = autorig(given, *m);
	}

	//system("time");
	

	countBone = SMdata.embedding.size();

	preIndex = new int[countBone];
	aftIndex = new int[countBone];

	//ǰ��������ʼ��
	preIndex[0] = -1;
	for (int i = 0; i < SMdata.embedding.size(); i++)
	{
		aftIndex[i] = -1;
	}

	for (int y = 1; y < SMdata.embedding.size(); y++)
	{
		//yΪ�м�ǣ�given.fPrev()[y]Ϊǰ����
		preIndex[y] = given.fPrev()[y];
		//preIndex[y]Ϊ�м�ǣ�yΪ������
		aftIndex[preIndex[y]] = y;
	}

	//����ģ�ͻ�ȡ
	for (int y = 0; y < SMdata.embedding.size(); y++)
	{
		embedding.push_back(SMdata.embedding[y]);
		CurEmbedding.push_back(SMdata.embedding[y]);
	}

	//����������淶��energy
	deltaEnergy = new double[countBone];

	//��ȥ 0 �±�
	deltaEnergy[0] = -1;

	//�������ڴ���淶������������
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

}

#define HEAD 3

//ģ�͹淶������
void GenerateMesh::ChangeGrowScale(const int bone, const double scale)
{
	expectGrowScale[bone] = scale;
	MeshGrowCustom();
}

//����ģ��������仯���仯���ڴ�ģ�͸���λ��
void GenerateMesh::CountGrowCustom()
{
	for (int i = 0; i < m->vertices.size(); i++)
	{
		//ÿ�����������õ�����λ��
		Vector3 temp;

		for (int j = 1; j < countBone; j++)
		{
			//ͷ�������ĵ�����
			if (j == HEAD)
			{
				Vector3 m_delta;
				m_delta[0] = m->vertices[i].pos[0] + delta_skeleton[i][0];
				m_delta[1] = m->vertices[i].pos[1] + delta_skeleton[i][1];
				m_delta[2] = m->vertices[i].pos[2] + delta_skeleton[i][2];

				for (int k = 0; k < 3; k++)
				{
					temp[k] = embedding[HEAD][k] + ((m_delta[k] - embedding[HEAD][k]) * getDistance(embedding[HEAD], embedding[preIndex[HEAD]])) / getDistance(m_delta, embedding[HEAD]) - m_delta[k];
				}
			}
			else
				//��ͨ�۹�-----------����淶���۹ǳ���� 3��2  __________________
			{
				Vector3 m_delta;
				m_delta[0] = m->vertices[i].pos[0] + delta_skeleton[i][0];
				m_delta[1] = m->vertices[i].pos[1] + delta_skeleton[i][1];
				m_delta[2] = m->vertices[i].pos[2] + delta_skeleton[i][2];

				//�����ϡ�ͶӰ�㡱
				Vector3 projVertice = proj(embedding[preIndex[j]], embedding[j], m_delta);
				double d = getDistance(embedding[preIndex[j]], embedding[j]) * 2 / 3;

				//��ͶӰ���ڹ�����ʱ�������˵���ΪͶӰ��
				if (!isInLine(projVertice, embedding[preIndex[j]], embedding[j]))
				{
					if (getDistance(m_delta, embedding[j]) < getDistance(m_delta, embedding[preIndex[j]]))
					{
						projVertice = embedding[j];
					}
					else
						projVertice = embedding[preIndex[j]];
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

//ģ����淶���ݻ�
void GenerateMesh::MeshGrowCustom()
{
	for (int i = 0; i < m->vertices.size(); i++)
	{
		//�ֶ�ÿ����������ʵ�ʵ�������Ȩ�����õ�������
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


//�����仯��ģ��ӳ��仯����
void GenerateMesh::ChangeFromSkeleton()
{
	////BVHData ����
	//delete BvhData;
	//BvhData = new BVHData(embedding, preIndex);

	//������ʱ���
	Vector3 temp;
	//������
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

		/*?????????????????*/ //weightSize=17  �Լ� j ȡֵ��ȷ�� bone_a bone_b ѡȡ
		//�ٶ� bone_a Ϊ���  bone_b Ϊǰ��
		for (int j = 0; j < weightSize; j++)
		{
				//��j+1������
				projTemp = proj(SMdata.embedding[preIndex[j + 1]], SMdata.embedding[j + 1], copy_m->vertices[i].pos);
				RjTemp = Rj(SMdata.embedding[preIndex[j + 1]], SMdata.embedding[j + 1], embedding[preIndex[j + 1]], embedding[j + 1]);
				SBjTemp = SBj(SMdata.embedding[preIndex[j + 1]], SMdata.embedding[j + 1], embedding[preIndex[j + 1]], embedding[j + 1]);
				EBjViTemp = EBjVi(SMdata.embedding[preIndex[j + 1]], SMdata.embedding[j + 1], copy_m->vertices[i].pos);
				weight = (0.5 + SMdata.attachment->getWeights(i)[j] * 10000.0) / 10000.0;

				for (int k = 0; k < 3; k++)
				{
					temp[k] += weight*(embedding[preIndex[j + 1]][k] + (copy_m->vertices[i].pos[k] - SMdata.embedding[preIndex[j + 1]][k])
						+ (copy_m->vertices[i].pos[k] - projTemp[k])*(RjTemp - 1)
						+ EBjViTemp * SBjTemp[k]);
				}
		}

		delta_skeleton[i][0] = temp[0] - copy_m->vertices[i].pos[0];
		delta_skeleton[i][1] = temp[1] - copy_m->vertices[i].pos[1];
		delta_skeleton[i][2] = temp[2] - copy_m->vertices[i].pos[2];
	}
}


//������ֱ��ת��Ϊ��ѡģ��
void GenerateMesh::changeSkeletonFromMap()
{
	dis_CG();

	for (int i = 1; i < countBone; i++)
	{
		double scale;
		scale = getDistance(cg_girl[i], cg_girl[preIndex[i]]) / getDistance(SMdata.embedding[i], SMdata.embedding[preIndex[i]]);
		
		changeSingleSkeleton(i, scale);

	}
}

//�������ɶȸı����������Ծ�ֹǣ����������
//bone Ϊ������������1��ʼ
void GenerateMesh::changeSkeleton(const int bone, double alpha, double beta, double length)
{
	BvhData->data[bone].angle_alpha = alpha;
	BvhData->data[bone].angle_beta = beta;
	//�Ƕ�ת��Ϊ����
	alpha = alpha * PI / 180;
	beta = beta * PI / 180;

	//temp �ı�Ŀ�����ĩ�ˣ���øı����� �������������øı���������
	int preBone = preIndex[bone];
	//Vector3 oldPos = embedding[bone];	/*�˴���ȥ������Ĭ��λ�ô���ı�ǰ��ԭʼλ�ý��м���*/
	Vector3 prePos = embedding[preBone];
	changedIndex[bone] = true;

	int* beMoved = new int[countBone - 2];	//���������Ƿ�ǣ����¼����
	for (int i = 0; i < (countBone - 2); i++)
	{
		beMoved[i] = 0;
	}
	int afterList = aftIndex[bone];
	int beMoved_index = 0;
	while (afterList != -1)
	{
		beMoved[beMoved_index] = afterList;
		afterList = aftIndex[afterList];
		beMoved_index++;
	}

	RelayAxes reAxes = this->BvhData->defaultSkeleton[bone - 1].defaultAxes;
	Vector3 targetPos;
	double skeVector[4];
	double tempResult1[4];
	double tempResult2[4];
	double tempInX[16];
	double tempInY[16];
	double tempInZ[16];

	switch (reAxes)
	{
	case Axes_X:
		skeVector[0] = length;
		skeVector[1] = 0;
		skeVector[2] = 0;
		skeVector[3] = 0;
		rotateInZ(beta, tempInZ);
		rotateInY(alpha, tempInY);
		lineRotate(skeVector, tempInZ, tempResult1);
		lineRotate(tempResult1, tempInY, tempResult2);
		targetPos[0] = prePos[0] + tempResult2[0];
		targetPos[1] = prePos[1] + tempResult2[1];
		targetPos[2] = prePos[2] + tempResult2[2];
		break;
	case Axes_Y:
		skeVector[0] = 0;
		skeVector[1] = length;
		skeVector[2] = 0;
		skeVector[3] = 0;
		rotateInX(beta, tempInX);
		rotateInZ(alpha, tempInZ);
		lineRotate(skeVector, tempInX, tempResult1);
		lineRotate(tempResult1, tempInZ, tempResult2);
		targetPos[0] = prePos[0] + tempResult2[0];
		targetPos[1] = prePos[1] + tempResult2[1];
		targetPos[2] = prePos[2] + tempResult2[2];
		break;
	case Axes_Z:
		skeVector[0] = 0;
		skeVector[1] = 0;
		skeVector[2] = length;
		skeVector[3] = 0;
		rotateInX(alpha, tempInX);
		rotateInY(beta, tempInY);
		lineRotate(skeVector, tempInY, tempResult1);
		lineRotate(tempResult1, tempInX, tempResult2);
		targetPos[0] = prePos[0] + tempResult2[0];
		targetPos[1] = prePos[1] + tempResult2[1];
		targetPos[2] = prePos[2] + tempResult2[2];
		break;
	default:
		break;
	}

	Vector3 oldPos = embedding[bone];
	Vector3 transformation = targetPos - oldPos;

	embedding[bone] = targetPos;
	for (int i = 0; i < beMoved_index; i++)
	{
		int tempIndex = beMoved[i];

		//�˴�����ÿ����������������ֻ����һ���˵㣬��Ϊһ���˵㼴����ù�����ʣ��˵���ɺ�����������
		embedding[tempIndex][0] = embedding[tempIndex][0] + transformation[0];
		embedding[tempIndex][1] = embedding[tempIndex][1] + transformation[1];
		embedding[tempIndex][2] = embedding[tempIndex][2] + transformation[2];
	}

	cout << "\nCUR CHANGE SKELETON " << bone << endl
		<< "PRE POINT " << preBone << endl
		<< "NEW ANGLES " << BvhData->data[bone].angle_alpha << " " << BvhData->data[bone].angle_beta << endl
		<< "OLD POS " << oldPos << endl
		<< "NEW POS " << targetPos << endl;

	ChangeFromSkeleton();

	CountGrowCustom();
}

//ͬ���ı䵥һ����
void GenerateMesh::changeSingleSkeleton(const int bone, const double scale)
{
	if (((scale - 1) < 0.05) && ((scale - 1) > 0.05))
	{
		changedIndex[bone] = false;
	}
	else
		changedIndex[bone] = true;

	int* beMoved = new int[countBone - 2];	//���������Ƿ�ǣ����¼����
	for (int i = 0; i < (countBone - 2); i++)
	{
		beMoved[i] = 0;
	}
	int afterList = aftIndex[bone];
	int beMoved_index = 0;
	while (afterList != -1)
	{
		beMoved[beMoved_index] = afterList;
		afterList = aftIndex[afterList];
		beMoved_index++;
	}

	//��ǰ��������һ��Ϊ��ʼ�������仯ǰλ�ü�¼
	double Temp_embed_X = embedding[bone][0];
	double Temp_embed_Y = embedding[bone][1];
	double Temp_embed_Z = embedding[bone][2];

	embedding[bone][0] =
		scale * (SMdata.embedding[bone][0] - SMdata.embedding[preIndex[bone]][0])
		+ embedding[preIndex[bone]][0];
	embedding[bone][1] =
		scale * (SMdata.embedding[bone][1] - SMdata.embedding[preIndex[bone]][1])
		+ embedding[preIndex[bone]][1];
	embedding[bone][2] =
		scale * (SMdata.embedding[bone][2] - SMdata.embedding[preIndex[bone]][2])
		+ embedding[preIndex[bone]][2];



	for (int i = 0; i < beMoved_index; i++)
	{
		int tempIndex = beMoved[i];

		//�˴�����ÿ����������������ֻ����һ���˵㣬��Ϊһ���˵㼴����ù�����ʣ��˵���ɺ�����������
		embedding[tempIndex][0] = embedding[tempIndex][0] + embedding[bone][0] - Temp_embed_X;
		embedding[tempIndex][1] = embedding[tempIndex][1] + embedding[bone][1] - Temp_embed_Y;
		embedding[tempIndex][2] = embedding[tempIndex][2] + embedding[bone][2] - Temp_embed_Z;
	}

	ChangeFromSkeleton();

	CountGrowCustom();
}


//���ƹ���
void GenerateMesh::drawSkeleton()
{
	for (int i = 1; i < countBone; i++)
	{
		if (changedIndex[i])
			drawLine(embedding[i], embedding[preIndex[i]], color_Tran, thin);
		else
			drawLine(embedding[i], embedding[preIndex[i]], color_Skele, thin);
	}
}

//���ƹ������
void GenerateMesh::drawSkeletonPoint()
{
	for (int i = 0; i < countBone; i++)
	{
		drawPoint(embedding[i], thin * 2, Vector3(0, 0, 1));
	}
}

//����ģ��
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

//��ӱ任
void GenerateMesh::addSkeletonTran(int numSkele, double scale)
{
	changeBone addOne;
	addOne.numOfBone = numSkele;
	addOne.scale = scale;
	cBones.push_back(addOne);
}

//��ձ任
void GenerateMesh::clearSkeletonVector()
{
	cBones.clear();
}

//���ģ�ͱ仯 
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

	//Ѱ�ҵ����Χ�ڵĵ�
	//��ǰ����Ŵ�0��ʼ�������Ӧ��ֵ��1��ʼ
	area_smooth = 0;
	area_stress = 0;
	flaging = 0;

	cout << "Model has been cleared !" << endl;

}


//Ѱ�ҵ����Χ�ڵĵ�
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

				//��������ͶӰ��
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
							//��ǵ�ÿһ������ÿһ�������ϵ�ͶӰ������������ֵ���бȽ�
							double temp = getDistance(proj(SMdata.embedding[preIndex[j + 1]], SMdata.embedding[j + 1], copy_m->vertices[i].pos), copy_m->vertices[i].pos);
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
//���������㴦��
void GenerateMesh::CountingEnergy()
{
	int vertices_size = m->vertices.size();
	int row = vertices_size * (2 + countBone - 2 + 1) + Stress_size * (countBone - 1);	//countBone-2Ϊ��ȥͷ���Ĺ�����
	int col = vertices_size;



	vector<Vector3> temp_vertices;

	fstream innn;
	innn.open("aaaa111.txt", ios::out);

	if (!innn)
	{
		system("pause");
	}

	cout << "Begin to count Energy :" << endl;

	//����������ֿ�����
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
		//������J��(���϶���)��ÿһ�У�i��
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
				//�Ƿ��ڵ�ǰ���Ի���
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
		//���¶���������
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

		//��һ����
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

		//�ڶ�����
		for (i = 0; i < vertices_size; i++)
		{
			b[vertices_size * 2 - 1 - i] = sqrt(WEIGHT_FH * SMdata.attachment->getWeights(i)[HEAD - 1]) * (copy_m->vertices[i].pos[k] + ExpectGrow[i][HEAD][k]);
		}
		cout << "@2 success" << endl;

		//system("pause");

		//��������
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

		////�������ض�bûӰ��

		//��������
		//���¶������
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
					double d_vi = getDistance(proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos), copy_m->vertices[stress_it->first].pos);
					double d_min = stressArea[verticesToStress[stress_it->first] - 1][2 * bone];
					double d_max = stressArea[verticesToStress[stress_it->first] - 1][2 * bone + 1];

					double temp_vi_Bi = embedding[preIndex[bone + 1]][k]
						+ (proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k] - SMdata.embedding[preIndex[bone + 1]][k]) / (SMdata.embedding[bone + 1][k] - SMdata.embedding[preIndex[bone + 1]][k])
						* (embedding[bone + 1][k] - embedding[preIndex[bone + 1]][k])
						+ getDistance(embedding[preIndex[bone + 1]], embedding[bone + 1]) / getDistance(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1])
						* (SE_STRESS * (d_vi - d_min) / (d_max - d_min) + 1)
						* (copy_m->vertices[stress_it->first].pos[k] - proj(SMdata.embedding[preIndex[bone + 1]], SMdata.embedding[bone + 1], copy_m->vertices[stress_it->first].pos)[k]);

					//if (SMdata.attachment->getWeights(stress_it->first)[bone] > 0.5 && temp_vi_Bi <= 0)
					//{
					//	//�����Ǽ�����������µ�����������0
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


		////��������
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
		//	//��䵱ǰ�е�ÿһ��
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

		//			//�Ƿ��ڵ�ǰ���Ի���
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