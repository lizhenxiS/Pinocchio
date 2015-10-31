


#ifndef GENERATEMESH_H
#define GENERATEMESH_H

#include "GenerateTool.h"
#include "skeleton.h"
#include "attachment.h"
#include "pinocchioApi.h"
#include "MyVerticeMesh.h"
#include <map>
#include "SkeletonNode.h"
#include "PixelModel.h"

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
模型实际绘制类
*/
class GenerateMesh
{
public:
	//读取人体模型初始化数据
	GenerateMesh(const string& file);

	~GenerateMesh();

	//初始化颜色信息
	void initColor();

	//初始化骨骼线条宽度
	void initSkeletonWidth();

	//初始化模型变化量信息
	void initMeshDelta();

	//导出模型
	void OutPutMesh();

	//数据获取
	void process();

	//骨骼长度变化后模型映射变化处理
	void extendMesh(vector<Vector3> oldBoneNodePoint, vector<Vector3> newBoneNodePoint);

	//骨骼角度变化后模型映射变化处理
	void ChangeFromSkeletonRotation();

	//任意自由度改变骨骼，并相对静止牵动关联骨骼
	void changeSkeleton(const int bone, double alpha, double beta);

	//同步改变单一骨骼
	void changeSingleSkeleton(const int boneNode, const double scale);

	//将骨骼直接转换为所选模板
	void changeSkeletonFromMap();

	//绘制骨骼
	void drawSkeleton();

	//绘制骨骼结点
	void drawSkeletonPoint();

	//绘制模型
	void drawMesh();

	//绘制模型体素
	void drawVoxel();

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

	//骨骼相对起始模型旋转角度数组
	double rotateAngle[BONECOUNT][2];

	//初始化记录骨骼旋转角数组
	void initRotateAngleArray();

	//获取模型质心点，用于camera旋转
	Vector3 getModelCenter();

	//计算模型质心
	void updateModelCenter();

	//绘制模型底面盒子
	void drawModelBottomBox();

	//更新模型底面盒子
	void updateModelBottomBox();

	//绘制模型桌底凸包
	void drawModelConvexHull();

	//更新模型着地凸包
	void updateModelConvexHull();

	//更新放缩体素(每次放缩骨骼后必须调用)(同时更新体素模型重心、体积)
	void updateModelVoxelInScale(vector<Vector3> oldBoneNodePoint, vector<Vector3> newBoneNodePoint);

	//绘制模型质心
	void drawModelCenter();

	//从文件中读取法向量信息
	void readVerticeNormal(string filename);

	void drawPixel(Pixel p, Vector3 color);

	//碰撞检测
	void collisionCheck();

	//更改比例
	/********************************************/

private:
	//体素模型
	PixelModel* pixelModel = NULL;

	//模型着地凸包
	vector<Vector3> convexHull;

	//模型底面盒子
	Vector3 modelBottomBox[4];

	//模型最低点
	Vector3 modelMinPoint;

	//模型最高点
	Vector3 modelMaxPoint;

	//模型质心
	Vector3 modelQualityCenter;

	//模型OBJ文件地址
	string modelFile;

	//待改变的骨骼序号组
	vector<changeBone> cBones;
	//总骨骼数+1 = 18
	int bonePointCount;

	//骨骼信息
	SkeletonNode* skeletonNodeInformation = NULL;

	//各骨骼距离规范化energy
	double* deltaEnergy = NULL;
	//各骨骼期待向规范方向增长比例
	double* expectGrowScale = NULL;
	//每个骨骼期望该点所在位置
	vector<vector<Vector3>> ExpectGrow;


	//骨骼模型（实时骨骼数据）
	vector<Vector3> embedding;
	//骨骼+模型结果数据(固定原始数据)
	PinocchioOutput originData;
	//骨骼模型 (旋转后的未缩放模型)
	vector<Vector3> copy_embedding;
	//骨骼初始长度记录数组
	double* boneOriginLen;

	//表面顶点模型 (实时模型数据)
	Mesh* meshVertices = NULL;
	//表面顶点模型 (固定原始数据)
	MyVerticeMesh* originMeshVertices = NULL;
	//表面顶点模型
	MyVerticeMesh* copy_MeshVertices = NULL;
	
	//模型顶点法向量
	vector<Vector3> meshNormals;

	//来自骨骼增长的模型增长
	vector<Vector3> delta_skeleton;
	//来自规范化的模型增长
	vector<Vector3> delta_Custom;
	//来自Energy的模型变化
	vector<Vector3> delta_Energy;
	//来自旋转的临时模型增量
	vector<Vector3> delta_rotate;
	//权重数据
	Attachment* attachment = NULL;

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


