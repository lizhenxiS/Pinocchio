
#include "StandardModel.h"
#include "GenerateTool.h"

#define PI 3.1415926

StandardModel::StandardModel()
{

}

StandardModel::StandardModel(const PinocchioOutput modelData, 
	const Mesh* m, const vector<Vector3>& points,
	SkeletonNode* skeletonNodeInformation)
{
	this->skeletonNodeInformation = skeletonNodeInformation;
	this->modelData = modelData;
	initDefaultSkeleton();
	for (int i = 0; i < BONECOUNT + 1; i++)
	{
		skeletonPoints[i] = points[i];
	}
	setSkeletonLengthData(points);
	meshVertices = new MyVerticeMesh(*m);
	//standardMesh(points);
}

/*
获得某一骨骼向标架转换旋转矩阵
*/
void StandardModel::rotateSkeleton(const int bone, const vector<Vector3>& points)
{
	clearRotateVector();

	double alphaVector[16];
	double betaVector[16];
	double gammaVector[16];
	Vector3 shadowPoint;	//对应投影面投影点
	Vector3 localPoint;	//局部坐标
	double beta;	//模型向标架骨骼旋转的第二个角度
	double alpha;	//模型向标架骨骼旋转的第一个角度
	double gamma;	//为骨骼向标架旋转造成的自身旋转计算的反自身旋转角
	int boneNode = bone + 1;
	int preIndex = skeletonNodeInformation[boneNode].parentIndex;
	int prepreIndex = skeletonNodeInformation[preIndex].parentIndex;
	Vector3 prePoint = points[preIndex];
	Vector3 preprePoint;
	if (preIndex == 0)
	{
		preprePoint = Vector3(0, 1, 0);
	}
	else 
	{
		preprePoint = points[prepreIndex];
	}
	Vector3 point = points[boneNode];
	localPoint = point - prePoint;
	Vector3 preSkeletonVector = prePoint - preprePoint;
	Vector3 skeletonVector = point - prePoint;
	Vector3 temp1;
	Vector3 temp2;
	Vector3 crossProduct;	//当前骨骼与前一骨骼的叉乘【用于处理骨骼自身旋转情况】
	crossProduct = getCrossProduct(preSkeletonVector, skeletonVector);
	switch (skeletonData[bone].defaultAxes)
	{
	case Axes_X:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = localPoint[1];
		shadowPoint[2] = 0;
		//cout << "skeleton ： " << bone + 1 << " relay Axes_X changed" << endl;
		//绕Y轴旋转
		if (localPoint[2] > 0)
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInY(alpha, alphaVector);
		//绕Z轴旋转
		if (localPoint[1] > 0)
		{
			beta = -AngleInSpace(Vector3(1, 0, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		else
		{
			beta = AngleInSpace(Vector3(1, 0, 0), Vector3(0, 0, 0), shadowPoint * PI / 180);
		}
		rotateInZ(beta, betaVector);
		temp1 = lineRotate(crossProduct, betaVector);
		temp2 = lineRotate(temp1, alphaVector);
		gamma = transInX(crossProduct, temp2);
		rotateInX(gamma, gammaVector);
		break;
	case OP_Axes_X:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = localPoint[1];
		shadowPoint[2] = 0;
		//cout << "skeleton ： " << bone + 1 << " relay OP_Axes_X changed" << endl;
		//绕Y轴旋转
		if (localPoint[2] > 0)
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInY(alpha, alphaVector);
		//绕Z轴旋转
		if (localPoint[1] > 0)
		{
			beta = AngleInSpace(Vector3(-1, 0, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		else
		{
			beta = -AngleInSpace(Vector3(-1, 0, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		rotateInZ(beta, betaVector);
		temp1 = lineRotate(crossProduct, betaVector);
		temp2 = lineRotate(temp1, alphaVector);
		gamma = transInX(crossProduct, temp2);
		rotateInX(gamma, gammaVector);
		break;
	case Axes_Y:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = localPoint[1];
		shadowPoint[2] = 0;
		//cout << "skeleton ： " << bone + 1 << " relay Axes_Y changed" << endl;
		//绕X轴旋转
		if (localPoint[2] > 0)
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInX(alpha, alphaVector);
		//绕Z轴旋转
		if (localPoint[0] > 0)
		{
			beta = AngleInSpace(Vector3(0, 1, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		else
		{
			beta = -AngleInSpace(Vector3(0, 1, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		rotateInZ(beta, betaVector);
		temp1 = lineRotate(crossProduct, betaVector);
		temp2 = lineRotate(temp1, alphaVector);
		gamma = transInY(crossProduct, temp2);
		rotateInY(gamma, gammaVector);
		break;
	case OP_Axes_Y:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = localPoint[1];
		shadowPoint[2] = 0;
		//cout << "skeleton ： " << bone + 1 << " relay OP_Axes_Y changed" << endl;
		//绕local X轴旋转
		if (localPoint[2] > 0)
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		//alpha = -35 * PI / 180;	//debug
		rotateInX(alpha, alphaVector);
		//绕local Z轴旋转
		if (localPoint[0] > 0)
		{
			beta = -AngleInSpace(Vector3(0, -1, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		else
		{
			beta = AngleInSpace(Vector3(0, -1, 0), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		//beta = 90 * PI / 180;	//debug
		rotateInZ(beta, betaVector);
		temp1 = lineRotate(crossProduct, betaVector);
		temp2 = lineRotate(temp1, alphaVector);
		gamma = transInY(crossProduct, temp2);
		rotateInY(gamma, gammaVector);
		break;
	case Axes_Z:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = 0;
		shadowPoint[2] = localPoint[2];
		//cout << "skeleton ： " << bone + 1 << " relay Axes_Z changed" << endl;
		//绕X轴旋转
		if (localPoint[1] > 0)
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInX(alpha, alphaVector);
		//绕Y轴旋转
		if (localPoint[0] > 0)
		{
			beta = -AngleInSpace(Vector3(0, 0, 1), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		else
		{
			beta = AngleInSpace(Vector3(0, 0, 1), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		rotateInY(beta, betaVector);
		temp1 = lineRotate(crossProduct, betaVector);
		temp2 = lineRotate(temp1, alphaVector);
		gamma = transInZ(crossProduct, temp2);
		rotateInZ(gamma, gammaVector);
		break;
	case OP_Axes_Z:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = 0;
		shadowPoint[2] = localPoint[2];
		//cout << "skeleton ： " << bone + 1 << " relay OP_Axes_Z changed" << endl;
		//绕X轴旋转
		if (localPoint[1] > 0)
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInX(alpha, alphaVector);
		//绕Y轴旋转
		if (localPoint[0] > 0)
		{
			beta = AngleInSpace(Vector3(0, 0, -1), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		else
		{
			beta = -AngleInSpace(Vector3(0, 0, -1), Vector3(0, 0, 0), shadowPoint) * PI / 180;
		}
		rotateInY(beta, betaVector);
		temp1 = lineRotate(crossProduct, betaVector);
		temp2 = lineRotate(temp1, alphaVector);
		gamma = transInZ(crossProduct, temp2);
		rotateInZ(gamma, gammaVector);
		break;
	default:
		break;
	}

	skeletonPoints[boneNode] = lineRotate(prePoint, point, betaVector);
	temp1 = Vector3(skeletonPoints[boneNode][0], skeletonPoints[boneNode][1], skeletonPoints[boneNode][2]);
	skeletonPoints[boneNode] = lineRotate(prePoint, temp1, alphaVector);
	for (int j = 0; j < 16; j++)
	{
		rotateVector[(boneNode - 1) * 3][j] = alphaVector[j];
		rotateVector[(boneNode - 1) * 3 + 1][j] = betaVector[j];
		rotateVector[(boneNode - 1) * 3 + 2][j] = gammaVector[j];
	}

	Vector3 transVector = skeletonPoints[boneNode] - point;
	//cout << "transform : " << transVector << endl;
	/*牵连后续骨骼*/
	vector<int> afterNodes = getAfterSkeletonIndex(skeletonNodeInformation, boneNode);
	for (int i = 0; i < afterNodes.size(); i++)
	{
		int node = afterNodes[i];
		//cout << "node : " << node << " is being changed" << endl;
		for (int j = 0; j < 16; j++)
		{
			rotateVector[(node - 1) * 3][j] = alphaVector[j];
			rotateVector[(node - 1) * 3 + 1][j] = betaVector[j];
		}
		skeletonPoints[node] += transVector;
		int preIndex = skeletonNodeInformation[node].parentIndex;
		int prepreIndex = skeletonNodeInformation[preIndex].parentIndex;
		Vector3 prePointnew = skeletonPoints[preIndex];
		Vector3 prePointold = points[preIndex];
		Vector3 preprePointnew = skeletonPoints[prepreIndex];
		Vector3 preprePointold = points[prepreIndex];
		Vector3 skeletonVectornew = skeletonPoints[node] - prePointnew;
		Vector3 skeletonVectorold = prePointold - points[node];
		Vector3 preSkeletonVectornew = preprePointnew - prePointnew;
		Vector3 preSkeletonVectorold = preprePointold - prePointold;
		Vector3 crossProductold = getCrossProduct(preSkeletonVectorold, skeletonVectorold);
		Vector3 crossProductnew = getCrossProduct(preSkeletonVectornew, skeletonVectornew);
		double tempGammaVector[16];
		double tempGamma;
		RelayAxes relay = skeletonData[node - 1].defaultAxes;
		switch (relay)
		{
		case Axes_X:
			tempGamma = transInX(crossProductold, crossProductnew);
			rotateInX(tempGamma, tempGammaVector);
			break;
		case Axes_Y:
			tempGamma = transInY(crossProductold, crossProductnew);
			rotateInY(tempGamma, tempGammaVector);
			break;
		case Axes_Z:
			tempGamma = transInZ(crossProductold, crossProductnew);
			rotateInZ(tempGamma, tempGammaVector);
			break;
		case OP_Axes_X:
			tempGamma = transInX(crossProductold, crossProductnew);
			rotateInX(tempGamma, tempGammaVector);
			break;
		case OP_Axes_Y:
			tempGamma = transInY(crossProductold, crossProductnew);
			rotateInY(tempGamma, tempGammaVector);
			break;
		case OP_Axes_Z:
			tempGamma = transInZ(crossProductold, crossProductnew);
			rotateInZ(tempGamma, tempGammaVector);
			break;
		default:
			break;
		}
		for (int j = 0; j < 16; j++)
		{
			rotateVector[(node - 1) * 3 + 2][j] = tempGammaVector[j];
		}
	}

}

/*
由初始模型反向求解向标架的旋转矩阵
通过旋转矩阵再将已有模型转换为标架模型

先初始化旋转矩阵数组
通过逐个考虑实际模型与标架的旋转变化，将对应旋转矩阵赋值并对后续骨骼对应矩阵进行处理
对于该次情况，将标架模型进行一次刷新

所以该方法将刷新模型BONECOUNT次
*/
void StandardModel::standardMesh(const vector<Vector3>& points)
{
	////double alphaVector[16];
	////double betaVector[16];
	////double gammaVector[16];
	//for (int i = 0; i < BONECOUNT; i++)
	//{
	//	//if (i != 5)
	//	//	continue;
	//	clearRotateVector();
	//	getSkeletonRotateVector(i, points);

	//	for (int i = 0; i < points.size(); i++)
	//	{
	//		points[i] = skeletonPoints[i];
	//	}

	//	//for (int j = 0; j < 16; j++)
	//	//{
	//	//	rotateVector[i * 3][j] = alphaVector[j];
	//	//	rotateVector[i * 3 + 1][j] = betaVector[j];
	//	//	rotateVector[i * 3 + 2][j] = gammaVector[j];
	//	//}
	//	//vector<int> afterNodes = getAfterSkeletonIndex(skeletonNodeInformation, i + 1);

	//	//for (int k = 0; k < afterNodes.size(); k++)
	//	//{
	//	//	int tempIndex = afterNodes[k] - 1;
	//	//	//对于后续传导骨骼，旋转矩阵相同
	//	//	for (int j = 0; j < 16; j++)
	//	//	{
	//	//		rotateVector[tempIndex * 3][j] = alphaVector[j];
	//	//		rotateVector[tempIndex * 3 + 1][j] = betaVector[j];
	//	//		rotateVector[tempIndex * 3 + 2][j] = gammaVector[j];
	//	//	}
	//	//	//cout << afterNodes[k] << " ";
	//	//}
	//	//cout << endl;
	//	//refreshMesh();
	//	//SELFDEBUG;
	//}

}

/*
输出旋转矩阵
*/
void StandardModel::outputRotateVector()
{
	cout << "OUTPUT ROTATE VECTOR..." << endl;
	for (int i = 0; i < BONECOUNT * 3; i++)
	{
		cout << "[" << i << "]" << endl;
		int j = 0;
		for (; j < 4; j++)
		{
			cout << rotateVector[i][j] << " , ";
		}
		cout << endl;
		
		for (; j < 8; j++)
		{
			cout << rotateVector[i][j] << " , ";
		}
		cout << endl;

		for (; j < 12; j++)
		{
			cout << rotateVector[i][j] << " , ";
		}
		cout << endl;

		for (; j < 16; j++)
		{
			cout << rotateVector[i][j] << " , ";
		}
		cout << endl << endl;
	}
}

/*
旋转矩阵更新后刷新模型
*/
void StandardModel::refreshMesh(Vector3 pointNode)
{
	//outputRotateVector();
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{
		Vector3 tempPoint = meshVertices->vertices[i].pos;
		Vector3 resultPoint = Vector3(0, 0, 0);
		for (int j = 0; j < BONECOUNT; j++)
		{
			//outputRotateVector();
			//Vector3 temp1 = pointRotate(skeletonPoints[j + 1], tempPoint, rotateVector[j * 3 + 1]);
			//Vector3 temp2 = pointRotate(skeletonPoints[j + 1], temp1, rotateVector[j * 3]);
			//Vector3 temp3 = pointRotate(temp2, rotateVector[3 * j + 2]);
			Vector3 temp1 = pointRotate(pointNode, tempPoint, rotateVector[j * 3 + 1]);
			Vector3 temp2 = pointRotate(pointNode, temp1, rotateVector[j * 3]);
			Vector3 temp3 = pointRotate(temp2, rotateVector[3 * j + 2]);
			resultPoint += modelData.attachment->getWeights(i)[j] * temp3;
			//cout << tempPoint << endl;
			//cout << temp1 << endl;
			//cout << temp2 << endl;
			//SELFDEBUG;
		}
		meshVertices->vertices[i].pos = resultPoint;
	}
}

/*
清空旋转矩阵
*/
void StandardModel::clearRotateVector()
{
	for (int i = 0; i < BONECOUNT * 3; i++)
	{
		rotateVector[i][0] = 1;
		rotateVector[i][1] = 0;
		rotateVector[i][2] = 0;
		rotateVector[i][3] = 0;

		rotateVector[i][4] = 0;
		rotateVector[i][5] = 1;
		rotateVector[i][6] = 0;
		rotateVector[i][7] = 0;

		rotateVector[i][8] = 0;
		rotateVector[i][9] = 0;
		rotateVector[i][10] = 1;
		rotateVector[i][11] = 0;

		rotateVector[i][12] = 0;
		rotateVector[i][13] = 0;
		rotateVector[i][14] = 0;
		rotateVector[i][15] = 1;
	}
}

/*
旋转模型 16x1
*/
void StandardModel::rotateMesh(double* rotateVector)
{
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{

	}
}

/*
为skeletonData中每个骨骼赋予当前模型实际长度信息
*/
void StandardModel::setSkeletonLengthData(const vector<Vector3>& points)
{
	for (int i = 0; i < BONECOUNT; i++)
	{
		skeletonData[i].length = getDistance(points[i + 1], points[skeletonNodeInformation[i + 1].parentIndex]);
	}
}

/*
为skeletonData赋予标架骨骼依赖坐标轴信息
*/
void StandardModel::initDefaultSkeleton()
{
	skeletonData[0] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[1] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[2] = BVHPoint(Axes_Y, 0, 0, 0);
	skeletonData[3] = BVHPoint(OP_Axes_X, 0, 0, 0);
	skeletonData[4] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[5] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[6] = BVHPoint(Axes_Z, 0, 0, 0);
	skeletonData[7] = BVHPoint(Axes_X, 0, 0, 0);
	skeletonData[8] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[9] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[10] = BVHPoint(Axes_Z, 0, 0, 0);
	skeletonData[11] = BVHPoint(OP_Axes_X, 0, 0, 0);
	skeletonData[12] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[13] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[14] = BVHPoint(Axes_X, 0, 0, 0);
	skeletonData[15] = BVHPoint(OP_Axes_Y, 0, 0, 0);
	skeletonData[16] = BVHPoint(OP_Axes_Y, 0, 0, 0);
}