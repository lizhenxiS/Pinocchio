
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
}

/*
��ĳһ������ת���ռ���ת����
bone ��ǰ��Ҫ�仯�Ĺ��� points ��������
*/
void StandardModel::rotateSkeleton(const int bone, const vector<Vector3>& points, const double alpha, const double beta)
{
	clearRotateVector();

	int boneNode = bone + 1;
	int preIndex = skeletonNodeInformation[boneNode].parentIndex;
	int prepreIndex = skeletonNodeInformation[preIndex].parentIndex;
	Vector3 prePoint = points[preIndex];
	Vector3 preprePoint = points[prepreIndex];
	Vector3 point = points[boneNode];
	int freeActor = skeletonInfo[bone].freeActor;
	Vector3 localX;		//������local����ϵ����ǰһ�����뵱ǰ������˵õ�
	Vector3 localY;		//������local����ϵ���뵱ǰ��������һ��
	Vector3 localZ;		//������local����ϵ����ǰ���߲�˻��
	Vector3 boneVector = point - prePoint;	//��ǰ����
	Vector3 preBoneVector = preprePoint - prePoint;	//ǰһ����

	localX = getCrossProduct(preBoneVector, boneVector);
	localY = boneVector;
	localZ = getCrossProduct(localX, localY);

	double angleX = alpha;
	double angleZ = beta;
	Vector3 temp1;

	switch (freeActor)
	{
	case 0: case -1:
		//�޷�ת���Ĺ���
		break;
	case 1:
		/*���ɶ�Ϊ 1 ʱ����������local X��ת*/
		skeletonPoints[boneNode] = rotateArroundLine(point, prePoint, prePoint + localX, angleX);
		linkRotate[bone][0] = OnceRotation(angleX, prePoint, prePoint + localX);
		break;
	case 2:
		/*���ɶ�Ϊ 2 ʱ������local X��ת��ע���ʱlocal Y local Z��仯�� ����local Z��ת*/
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
	/*ǣ����������*/
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
void StandardModel::refreshMesh()
{
	////outputRotateVector();
	//for (int i = 0; i < meshVertices->vertices.size(); i++)
	//{
	//	Vector3 tempPoint = meshVertices->vertices[i].pos;
	//	Vector3 resultPoint = Vector3(0, 0, 0);
	//	for (int j = 0; j < BONECOUNT; j++)
	//	{
	//		//outputRotateVector();
	//		//Vector3 temp1 = pointRotate(skeletonPoints[j + 1], tempPoint, rotateVector[j * 3 + 1]);
	//		//Vector3 temp2 = pointRotate(skeletonPoints[j + 1], temp1, rotateVector[j * 3]);
	//		//Vector3 temp3 = pointRotate(temp2, rotateVector[3 * j + 2]);
	//		Vector3 temp1 = pointRotate(pointNode, tempPoint, rotateVector[j * 3 + 1]);
	//		Vector3 temp2 = pointRotate(pointNode, temp1, rotateVector[j * 3]);
	//		Vector3 temp3 = pointRotate(temp2, rotateVector[3 * j + 2]);
	//		resultPoint += modelData.attachment->getWeights(i)[j] * temp3;
	//		//cout << tempPoint << endl;
	//		//cout << temp1 << endl;
	//		//cout << temp2 << endl;
	//		//SELFDEBUG;
	//	}
	//	meshVertices->vertices[i].pos = resultPoint;
	//}

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
	for (int i = 0; i < BONECOUNT; i++)
	{
		linkRotate[i][0] = OnceRotation(0, Vector3(0, 0, 0), Vector3(0, 0, 0));
		linkRotate[i][1] = OnceRotation(0, Vector3(0, 0, 0), Vector3(0, 0, 0));
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