

#ifndef _SKELETONLINKROTATE_H__
#define _SKELETONLINKROTATE_H__


#include "mesh.h"
#include "MyVerticeMesh.h"
#include "pinocchioApi.h"
#include "SkeletonNode.h"
#include "SkeletonInfo.h"
#include "OnceRotation.h"

#define BONECOUNT 17
class PixelModel;

/*
处理模型骨骼旋转及带动模型旋转问题
*/
class SkeletonLinkRotate
{
public :
	int pointCount;	//骨骼结点数
	int skeletonCount;	//骨骼数
	SkeletonNode* skeletonNodeInformation;	//共用骨骼信息数组
	SkeletonInfo skeletonInfo[BONECOUNT];	//骨骼自由度信息
	Vector3 skeletonPoints[BONECOUNT + 1];	//骨骼位置信息
	MyVerticeMesh* meshVertices;			//模型点位置信息
	PinocchioOutput weightData;				//weight等各种信息
	OnceRotation linkRotate[BONECOUNT][2];	//骨骼旋转轴旋转角信息

	void initDefaultSkeleton();	//输入标架角度信息
	void rotateSkeleton(const int bone, const vector<Vector3>& points, const double alpha, const double beta);	//将某一骨骼向标架转换并收集旋转矩阵
	void clearRotateVector();	//清空旋转矩阵
	void refreshMesh(const MyVerticeMesh* m);			//旋转矩阵更新后刷新模型 pointNode为旋转中心点，即主动骨骼起始点
	void refreshMesh(const Mesh* m);					//旋转矩阵更新后刷新模型 pointNode为旋转中心点，即主动骨骼起始点
	void refreshVoxel(int verticeCount, PixelModel* pixelModel);	//旋转矩阵更新后刷新体素 pointNode为旋转中心点，即主动骨骼起始点(同时更新体素模型重心、体积)

	SkeletonLinkRotate();
	SkeletonLinkRotate(const PinocchioOutput weightData, const vector<Vector3>& points,
		 SkeletonNode* skeletonNodeInformation);
	~SkeletonLinkRotate();
};

#endif	//_SKELETONLINKROTATE_H__