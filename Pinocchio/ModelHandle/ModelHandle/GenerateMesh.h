


#ifndef GENERATEMESH_H
#define GENERATEMESH_H

#include "GenerateTool.h"
#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include "MyVerticeMesh.h"
#include <map>
#include "BVHData.h"
#include "SkeletonNode.h"

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

	//����ģ��
	void OutPutMesh();

	//���ݻ�ȡ
	void process();

	//�������ȱ仯��ģ��ӳ��仯����
	void extendMesh();

	//�����Ƕȱ仯��ģ��ӳ��仯����
	void ChangeFromSkeletonRotation();

	//ת������
	void rotateSkeleton(const int bone, double alpha, double beta);

	//ת��ģ��
	void rotateMesh();

	//�������ɶȸı����������Ծ�ֹǣ����������
	void changeSkeleton(const int bone, double alpha, double beta, double length);

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

	////BVH����
	//BVHData* BvhData;

	//���ı���
	/********************************************/

private:

	//ģ��OBJ�ļ���ַ
	string modelFile;

	//���ı�Ĺ��������
	vector<changeBone> cBones;
	//�ܹ�����+1 = 18
	int countBone;

	//������Ϣ
	SkeletonNode* skeletonNodeInformation;

	//����������淶��energy
	double* deltaEnergy;
	//�������ڴ���淶������������
	double* expectGrowScale;
	//ÿ�����������õ�����λ��
	vector<vector<Vector3>> ExpectGrow;


	//����ģ��
	vector<Vector3> embedding;
	//����+ģ�ͽ������(ԭʼ����)
	PinocchioOutput SMdata;
	//����ģ��(ŷ���任��ĵ�ǰ�̶��Ƕ�ģ��)
	vector<Vector3> CurEmbedding;

	//���涥��ģ��
	Mesh* m;
	//���涥��ģ�͸���������ԭʼģ��(ԭʼ����)
	MyVerticeMesh* copy_m;
	//���ģ��(����angleΪ��׼angle + length��ģ�ͳ�ʼֵȷ��)


	//���Թ���������ģ������
	vector<Vector3> delta_skeleton;
	//���Թ淶����ģ������
	vector<Vector3> delta_Custom;
	//����Energy��ģ�ͱ仯
	vector<Vector3> delta_Energy;
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


