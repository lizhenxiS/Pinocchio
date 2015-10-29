
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

//��������
SkeletonLinkRotate::~SkeletonLinkRotate()
{
	if (meshVertices != NULL)
	{
		delete meshVertices;
	}
}

/*
��ĳһ������ת���ռ���ת����
bone ��ǰ��Ҫ�仯�Ĺ��� points ��������
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
	Vector3 localX;		//������local����ϵ����ǰһ�����뵱ǰ������˵õ�
	Vector3 localY;		//������local����ϵ���뵱ǰ��������һ��
	Vector3 localZ;		//������local����ϵ����ǰ���߲�˻��
	Vector3 boneVector = point - prePoint;	//��ǰ����
	Vector3 preBoneVector = prePoint - preprePoint;	//ǰһ����

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
��ת������º�ˢ��ģ��
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
��ת������º�ˢ��ģ��
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
�����ת����
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
ΪskeletonData�����ܹ���������������Ϣ
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