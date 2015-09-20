


#ifndef GENERATEMESH_H
#define GENERATEMESH_H

#include "GenerateTool.h"
#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include "MyVerticeMesh.h"
#include <map>



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


class GenerateMesh
{
public:
	//读取人体模型初始化数据
	GenerateMesh(const string& file);

	//导出模型
	void OutPutMesh();

	//数据获取
	void process();

	//骨骼变化后模型映射变化处理
	void ChangeFromSkeleton();

	//同步改变单一骨骼
	void changeSingleSkeleton(const int bone, const double scale);

	//将骨骼直接转换为所选模板
	void changeSkeletonFromMap();

	//绘制骨骼
	void drawSkeleton();

	//绘制模型
	void drawMesh();

	//清空变化容器
	void clearSkeletonVector();

	//添加变化
	void addSkeletonTran(int numSkele, double scale);

	//清空模型变化
	void clearModelChange();

	//模型向规范化演化
	void MeshGrowCustom();

	//模型期待增长计算
	void CountGrowCustom();

	//模型规范化比例
	void ChangeGrowScale(const int bone, const double scale);

	//总能量计算处理
	void CountingEnergy();

	//寻找点击范围内点
	void FindVertex(double x, double y);

	//更改比例
	/********************************************/

private:

	//模型OBJ文件地址
	string modelFile;

	//待改变的骨骼序号组
	vector<changeBone> cBones;
	//总骨骼数+1 = 18
	int countBone;
	//前续骨骼数组（p[0]无意义 -1）
	int* preIndex;
	//后续骨骼数组（all = 无意义 -1）
	int* aftIndex;


	//各骨骼距离规范化energy
	double* deltaEnergy;
	//各骨骼期待向规范方向增长比例
	double* expectGrowScale;
	//每个骨骼期望该点所在位置
	vector<vector<Vector3>> ExpectGrow;


	//骨骼模型
	vector<Vector3> embedding;
	//骨骼+模型结果数据(原始数据)
	PinocchioOutput SMdata;

	//表面顶点模型
	Mesh* m;
	//表面顶点模型副本，保持原始模型(原始数据)
	Mesh* copy_m;
	//来自骨骼增长的模型增长
	vector<Vector3> delta_skeleton;
	//来自规范化的模型增长
	vector<Vector3> delta_Custom;
	//来自Energy的模型变化
	vector<Vector3> delta_Energy;
	//权重数据
	Attachment* attachment;

	//所选需要平滑点
	map<int, int> verticesToSmooth;
	int Smooth_size;
	//所选突出点(顶点，所属区域（从1开始）)
	map<int, int> verticesToStress;
	int Stress_size;
	//每个区域中 该团顶点对于每个骨骼的距离
	vector<vector<double>> stressArea;

	//模型颜色
	Vector3 color_Mesh;
	//骨骼颜色
	Vector3 color_Skele;
	//变化骨骼颜色
	Vector3 color_Tran;
	//骨骼厚度
	double thin;
	//????
	bool flatShading;
	//????
	Vector3 trans;


};



#endif	//GENERATEMESH_H


