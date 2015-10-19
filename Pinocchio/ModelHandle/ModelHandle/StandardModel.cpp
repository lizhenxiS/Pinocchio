
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
���ĳһ��������ת����ת����
*/
void StandardModel::rotateSkeleton(const int bone, const vector<Vector3>& points)
{
	clearRotateVector();

	double alphaVector[16];
	double betaVector[16];
	double gammaVector[16];
	Vector3 shadowPoint;	//��ӦͶӰ��ͶӰ��
	Vector3 localPoint;	//�ֲ�����
	double beta;	//ģ�����ܹ�����ת�ĵڶ����Ƕ�
	double alpha;	//ģ�����ܹ�����ת�ĵ�һ���Ƕ�
	double gamma;	//Ϊ����������ת��ɵ�������ת����ķ�������ת��
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
	Vector3 crossProduct;	//��ǰ������ǰһ�����Ĳ�ˡ����ڴ������������ת�����
	crossProduct = getCrossProduct(preSkeletonVector, skeletonVector);
	switch (skeletonData[bone].defaultAxes)
	{
	case Axes_X:
		shadowPoint[0] = localPoint[0];
		shadowPoint[1] = localPoint[1];
		shadowPoint[2] = 0;
		//cout << "skeleton �� " << bone + 1 << " relay Axes_X changed" << endl;
		//��Y����ת
		if (localPoint[2] > 0)
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInY(alpha, alphaVector);
		//��Z����ת
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
		//cout << "skeleton �� " << bone + 1 << " relay OP_Axes_X changed" << endl;
		//��Y����ת
		if (localPoint[2] > 0)
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInY(alpha, alphaVector);
		//��Z����ת
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
		//cout << "skeleton �� " << bone + 1 << " relay Axes_Y changed" << endl;
		//��X����ת
		if (localPoint[2] > 0)
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInX(alpha, alphaVector);
		//��Z����ת
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
		//cout << "skeleton �� " << bone + 1 << " relay OP_Axes_Y changed" << endl;
		//��local X����ת
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
		//��local Z����ת
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
		//cout << "skeleton �� " << bone + 1 << " relay Axes_Z changed" << endl;
		//��X����ת
		if (localPoint[1] > 0)
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInX(alpha, alphaVector);
		//��Y����ת
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
		//cout << "skeleton �� " << bone + 1 << " relay OP_Axes_Z changed" << endl;
		//��X����ת
		if (localPoint[1] > 0)
		{
			alpha = -AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		else
		{
			alpha = AngleInSpace(shadowPoint, Vector3(0, 0, 0), localPoint) * PI / 180;
		}
		rotateInX(alpha, alphaVector);
		//��Y����ת
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
	/*ǣ����������*/
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
�ɳ�ʼģ�ͷ���������ܵ���ת����
ͨ����ת�����ٽ�����ģ��ת��Ϊ���ģ��

�ȳ�ʼ����ת��������
ͨ���������ʵ��ģ�����ܵ���ת�仯������Ӧ��ת����ֵ���Ժ���������Ӧ������д���
���ڸô�����������ģ�ͽ���һ��ˢ��

���Ը÷�����ˢ��ģ��BONECOUNT��
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
	//	//	//���ں���������������ת������ͬ
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
�����ת����
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
��ת������º�ˢ��ģ��
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
�����ת����
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
��תģ�� 16x1
*/
void StandardModel::rotateMesh(double* rotateVector)
{
	for (int i = 0; i < meshVertices->vertices.size(); i++)
	{

	}
}

/*
ΪskeletonData��ÿ���������赱ǰģ��ʵ�ʳ�����Ϣ
*/
void StandardModel::setSkeletonLengthData(const vector<Vector3>& points)
{
	for (int i = 0; i < BONECOUNT; i++)
	{
		skeletonData[i].length = getDistance(points[i + 1], points[skeletonNodeInformation[i + 1].parentIndex]);
	}
}

/*
ΪskeletonData�����ܹ���������������Ϣ
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