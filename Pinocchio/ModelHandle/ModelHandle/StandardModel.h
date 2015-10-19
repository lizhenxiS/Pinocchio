

#ifndef _STANDARDMODEL_H__
#define _STANDARDMODEL_H__

#include "BVHPoint.h"
#include "mesh.h"
#include "MyVerticeMesh.h"
#include "pinocchioApi.h"
#include "SkeletonNode.h"

#define BONECOUNT 17

/*
模型标架类
模型导入时获取模型初始骨骼长度及mesh信息
经标准化处理后获取的标准姿态模型
*/
class StandardModel
{
public :
	int pointCount;	//骨骼结点数
	int skeletonCount;	//骨骼数
	SkeletonNode* skeletonNodeInformation;	//共用骨骼信息数组
	BVHPoint skeletonData[BONECOUNT];	//每个标架骨骼实际与坐标轴的夹角
	Vector3 skeletonPoints[BONECOUNT + 1];
	MyVerticeMesh* meshVertices;
	PinocchioOutput modelData;
	double rotateVector[BONECOUNT * 3][16];

	void initDefaultSkeleton();	//输入标架角度信息
	void setSkeletonLengthData(const vector<Vector3>& points);	//输入标架骨骼长度信息
	void standardMesh(const vector<Vector3>& points);	//模型标架化
	void rotateSkeleton(const int bone, const vector<Vector3>& points);	//获得某一骨骼向标架转换旋转矩阵
	void rotateMesh(double* rotateVector);	//旋转模型 16x1
	void clearRotateVector();	//清空旋转矩阵
	void refreshMesh(Vector3 pointNode);	//旋转矩阵更新后刷新模型 pointNode为旋转中心点，即主动骨骼起始点
	void outputRotateVector();	//输出旋转矩阵

	StandardModel();
	StandardModel(const PinocchioOutput modelData, const Mesh* m, const vector<Vector3>& points,
		SkeletonNode* skeletonNodeInformation);
};

#endif	//_STANDARDMODEL_H__