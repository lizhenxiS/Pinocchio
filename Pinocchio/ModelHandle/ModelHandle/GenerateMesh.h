


#ifndef GENERATEMESH_H
#define GENERATEMESH_H

#include "GenerateTool.h"
#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include "MyVerticeMesh.h"
#include <map>
#include "SkeletonNode.h"

#define BONECOUNT 17

struct ArgData
{
	ArgData() :
		stopAtMesh(false), stopAfterCircles(false), skelScale(1.), noFit(false),
		skeleton(HumanSkeleton())
	{
	}

	bool stopAtMesh;
	bool stopAfterCircles;
	string filename;
	string motionname;
	Quaternion<> meshTransform;
	double skelScale;
	bool noFit;
	Skeleton skeleton;
	string skeletonname;
};

struct changeBone
{
	int numOfBone;
	double scale;
};


/*
ģ��ʵ�ʻ�����
*/
class GenerateMesh
{
public:
	//��ȡ����ģ�ͳ�ʼ������
	GenerateMesh(const string& file);

	~GenerateMesh();

	//����ģ��
	void OutPutMesh();

	//���ݻ�ȡ
	void process();

	//�������ȱ仯��ģ��ӳ��仯����
	void extendMesh(vector<Vector3> oldBonePoint, vector<Vector3> newBonePoint);

	//�����Ƕȱ仯��ģ��ӳ��仯����
	void ChangeFromSkeletonRotation();

	//�������ɶȸı����������Ծ�ֹǣ����������
	void changeSkeleton(const int bone, double alpha, double beta);

	//ͬ���ı䵥һ����
	void changeSingleSkeleton(const int boneNode, const double scale);

	//������ֱ��ת��Ϊ��ѡģ��
	void changeSkeletonFromMap();

	//���ƹ���
	void drawSkeleton();

	//���ƹ������
	void drawSkeletonPoint();

	//����ģ��
	void drawMesh();

	//��ձ仯����
	void clearSkeletonVector();

	//��ӱ仯
	void addSkeletonTran(int numSkele, double scale);

	//���ģ�ͱ仯
	void clearModelChange();

	//ģ����淶���ݻ�
	void MeshGrowCustom();

	//ģ���ڴ���������
	void CountGrowCustom();

	//ģ�͹淶������
	void ChangeGrowScale(const int bone, const double scale);

	//���������㴦��
	void CountingEnergy();

	//Ѱ�ҵ����Χ�ڵ�
	void FindVertex(double x, double y);

	//���������ʼģ����ת�Ƕ�����
	double rotateAngle[BONECOUNT][2];

	//��ʼ����¼������ת������
	void initRotateAngleArray();

	//��ȡģ�����ĵ㣬����camera��ת
	Vector3 getModelCenter();

	//����ģ������
	void updateModelCenter();

	//����ģ�͵������
	void drawModelBottomBox();

	//����ģ�͵������
	void updateModelBottomBox();

	//����ģ������͹��
	void drawModelConvexHull();

	//����ģ���ŵ�͹��
	void updateModelConvexHull();

	//����ģ������
	void drawModelCenter();

	//���ı���
	/********************************************/

private:

	//ģ���ŵ�͹��
	vector<Vector3> convexHull;

	//ģ�͵������
	Vector3 modelBottomBox[4];

	//ģ����͵�
	Vector3 modelMinPoint;

	//ģ����ߵ�
	Vector3 modelMaxPoint;

	//ģ������
	Vector3 modelQualityCenter;

	//ģ��OBJ�ļ���ַ
	string modelFile;

	//���ı�Ĺ��������
	vector<changeBone> cBones;
	//�ܹ�����+1 = 18
	int bonePointCount;

	//������Ϣ
	SkeletonNode* skeletonNodeInformation;

	//����������淶��energy
	double* deltaEnergy;
	//�������ڴ���淶������������
	double* expectGrowScale;
	//ÿ�����������õ�����λ��
	vector<vector<Vector3>> ExpectGrow;


	//����ģ�ͣ�ʵʱ�������ݣ�
	vector<Vector3> embedding;
	//����+ģ�ͽ������(�̶�ԭʼ����)
	PinocchioOutput originData;
	//����ģ�� (��ת���δ����ģ��)
	vector<Vector3> copy_embedding;
	//������ʼ���ȼ�¼����
	double* boneOriginLen;

	//���涥��ģ�� (ʵʱģ������)
	Mesh* meshVertices;
	//���涥��ģ�� (�̶�ԭʼ����)
	MyVerticeMesh* originMeshVertices;
	//���涥��ģ��
	MyVerticeMesh* copy_MeshVertices;
	


	//���Թ���������ģ������
	vector<Vector3> delta_skeleton;
	//���Թ淶����ģ������
	vector<Vector3> delta_Custom;
	//����Energy��ģ�ͱ仯
	vector<Vector3> delta_Energy;
	//������ת����ʱģ������
	vector<Vector3> delta_rotate;
	//Ȩ������
	Attachment* attachment;

	//��ѡ��Ҫƽ����
	map<int, int> verticesToSmooth;
	int Smooth_size;
	//��ѡͻ����(���㣬�������򣨴�1��ʼ��)
	map<int, int> verticesToStress;
	int Stress_size;
	//ÿ�������� ���Ŷ������ÿ�������ľ���
	vector<vector<double>> stressArea;

	//ģ����ɫ
	Vector3 color_Mesh;
	//������ɫ
	Vector3 color_Skele;
	//�仯������ɫ
	Vector3 color_Tran;
	//�������
	double thin;
	//????
	bool flatShading;
	//????
	Vector3 trans;


};



#endif	//GENERATEMESH_H


