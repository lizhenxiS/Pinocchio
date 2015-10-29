
#include "SkeletonLinkRotate.h"
#include "GenerateTool.h"

#define PI 3.1415926

SkeletonLinkRotate::SkeletonLinkRotate()
{

}

SkeletonLinkRotate::SkeletonLinkRotate(const PinocchioOutput modelData,
	const vector<Vector3>& points,
	SkeletonNode* skeletonNodeInformation)
{
	this->skeletonNodeInformation = skeletonNodeInformation;
	this->modelData = modelData;
	initDefaultSkeleton();
	for (int i = 0; i < BONECOUNT + 1; i++)
	{
		skeletonPoints[i] = points[i];
	}
	meshVertices = NULL;
}

//析构函数
SkeletonLinkRotate::~SkeletonLinkRotate()
{
	if (meshVertices != NULL)
	{
		delete meshVertices;
	}
}

/*
将某一骨骼旋转并收集旋转矩阵
bone 当前需要变化的骨骼 points 骨骼数组
*/
void SkeletonLinkRotate::rotateSkeleton(const int bone, const vector<Vector3>& points, const double alpha, const double beta)
{
	clearRotateVector();

	int boneNode = bone + 1;
	int preIndex;
	int prepreIndex;
	if (bone == 0 || bone == 2 || bone == 11 || bone == 14)
	{
		cout << "this skeletons has been dealt innormal there" << endl;
		switch (boneNode)
		{
		case 1:
			preIndex = 0;
			prepreIndex = 3;
			break;
		case 3:
			preIndex = 0;
			prepreIndex = 1;
			break;
		case 12:
			preIndex = 0;
			prepreIndex = 15;
			break;
		case 15:
			preIndex = 0;
			prepreIndex = 12;
			break;
		default:
			break;
		}
	}
	else
	{
		preIndex = skeletonNodeInformation[boneNode].parentIndex;
		prepreIndex = skeletonNodeInformation[preIndex].parentIndex;
	}
	Vector3 prePoint = points[preIndex];
	Vector3 preprePoint = points[prepreIndex];
	Vector3 point = points[boneNode];
	int freeActor = skeletonInfo[bone].freeActor;
	Vector3 localX;		//骨骼的local坐标系，由前一骨骼与当前骨骼叉乘得到
	Vector3 localY;		//骨骼的local坐标系，与当前骨骼方向一致
	Vector3 localZ;		//骨骼的local坐标系，由前二者叉乘获得
	Vector3 boneVector = point - prePoint;	//当前骨骼
	Vector3 preBoneVector = prePoint - preprePoint;	//前一骨骼

	localX = getCrossProduct(preBoneVector, boneVector);
	localY = boneVector;
	localZ = getCrossProduct(localX, localY);

	double angleX = alpha;
	double angleZ = beta;
	Vector3 temp1;

	switch (freeActor)
	{
	case 0: case -1:
		//无法转动的骨骼
		break;
	case 1:
		/*自由度为 1 时，仅考虑绕local X旋转*/
		skeletonPoints[boneNode] = rotateArroundLine(point, prePoint, prePoint + localX, angleX);
		linkRotate[bone][0] = OnceRotation(angleX, prePoint, prePoint + localX);
		break;
	case 2:
		/*自由度为 2 时，先绕local X旋转【注意此时local Y local Z会变化】 再绕local Z旋转*/
		temp1 = rotateArroundLine(point, prePoint, prePoint + localX, angleX);
		linkRotate[bone][0] = OnceRotation(angleX, prePoint, prePoint + localX);
		boneVector = temp1 - prePoint;
		localY = boneVector;
		localZ = getCrossProduct(localX, localY);
		skeletonPoints[boneNode] = rotateArroundLine(temp1, prePoint, prePoint + localZ, angleZ);
		linkRotate[bone][1] = OnceRotation(angleZ, prePoint, prePoint + localZ);
	default:
		break;
	}

	Vector3 temp2;
	/*牵连后续骨骼*/
	vector<int> afterNodes = getAfterSkeletonIndex(skeletonNodeInformation, boneNode);
	for (int i = 0; i < afterNodes.size(); i++)
	{
		int node = afterNodes[i];
		int tempBone = node - 1;

		switch (freeActor)
		{
		case 0: case -1:
			break;
		case 1:
			skeletonPoints[node] = rotateArroundLine(points[node], prePoint, prePoint + localX, angleX);
			linkRotate[tempBone][0] = OnceRotation(angleX, prePoint, prePoint + localX);
		case 2:
			temp2 = rotateArroundLine(points[node], prePoint, prePoint + localX, angleX);
			linkRotate[tempBone][0] = OnceRotation(angleX, prePoint, prePoint + localX);
			skeletonPoints[node] = rotateArroundLine(temp2, prePoint, prePoint + localZ, angleZ);
			linkRotate[tempBone][1] = OnceRotation(angleZ, prePoint, prePoint + localZ);
		default:
			break;
		}
	}
}

/*
旋转矩阵更新后刷新模型
*/
void SkeletonLinkRotate::refreshMesh(const MyVerticeMesh* m)
{
	if (meshVertices != NULL)
	{
		delete meshVertices;
	}
	meshVertices = new MyVerticeMesh(*m);

	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		Vector3 tempPoint = meshVertices->vertices[i].pos;
		Vector3 resultPoint = Vector3(0, 0, 0);
		for (int j = 0; j < BONECOUNT; j++)
		{
			Vector3 temp1 = linkRotate[j][0].pointRotate(tempPoint);
			Vector3 temp2 = linkRotate[j][1].pointRotate(temp1);
			resultPoint += modelData.attachment->getWeights(i)[j] * temp2;
		}
		meshVertices->vertices[i].pos = resultPoint;
	}
}

/*
旋转矩阵更新后刷新模型
*/
void SkeletonLinkRotate::refreshMesh(const Mesh* m)
{
	if (meshVertices != NULL)
	{
		delete meshVertices;
	}
	meshVertices = new MyVerticeMesh(*m);

	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		Vector3 tempPoint = meshVertices->vertices[i].pos;
		Vector3 resultPoint = Vector3(0, 0, 0);
		for (int j = 0; j < BONECOUNT; j++)
		{
			Vector3 temp1 = linkRotate[j][0].pointRotate(tempPoint);
			Vector3 temp2 = linkRotate[j][1].pointRotate(temp1);
			resultPoint += modelData.attachment->getWeights(i)[j] * temp2;
		}
		meshVertices->vertices[i].pos = resultPoint;
	}
}

/*
清空旋转矩阵
*/
void SkeletonLinkRotate::clearRotateVector()
{
	for (int i = 0; i < BONECOUNT; i++)
	{
		linkRotate[i][0] = OnceRotation(0, Vector3(0, 0, 0), Vector3(0, 0, 0));
		linkRotate[i][1] = OnceRotation(0, Vector3(0, 0, 0), Vector3(0, 0, 0));
	}
}


/*
为skeletonData赋予标架骨骼依赖坐标轴信息
*/
void SkeletonLinkRotate::initDefaultSkeleton()
{
	skeletonInfo[0] = SkeletonInfo(0);
	skeletonInfo[1] = SkeletonInfo(0);
	skeletonInfo[2] = SkeletonInfo(2);
	skeletonInfo[3] = SkeletonInfo(0);
	skeletonInfo[4] = SkeletonInfo(2);
	skeletonInfo[5] = SkeletonInfo(1);
	skeletonInfo[6] = SkeletonInfo(2);
	skeletonInfo[7] = SkeletonInfo(0);
	skeletonInfo[8] = SkeletonInfo(2);
	skeletonInfo[9] = SkeletonInfo(1);
	skeletonInfo[10] = SkeletonInfo(2);
	skeletonInfo[11] = SkeletonInfo(1);
	skeletonInfo[12] = SkeletonInfo(2);
	skeletonInfo[13] = SkeletonInfo(1);
	skeletonInfo[14] = SkeletonInfo(1);
	skeletonInfo[15] = SkeletonInfo(2);
	skeletonInfo[16] = SkeletonInfo(1);
}