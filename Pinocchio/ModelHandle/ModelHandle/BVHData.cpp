

#include "BVHData.h"
#include "GenerateTool.h"

/*
points 所有结点数组， 0 - 17 共18
preIndex 所有结点的前序结点数组， 0 - 17 共18 0号结点前序为-1
*/
BVHData::BVHData(vector<Vector3>& points, int* preIndex)
{
	//cout << "debug + " + points.size() << endl;
	this->pointCount = 18;
	this->skeletonCount = pointCount - 1;
	this->data = new BVHPoint[pointCount - 1];
	for (int i = 1; i < pointCount; i++)
	{
		Vector3 tempX = points[preIndex[i]] + Vector3(1, 0, 0);
		Vector3 tempY = points[preIndex[i]] + Vector3(0, 1, 0);
		Vector3 tempZ = points[preIndex[i]] + Vector3(0, 0, 1);
		double theta_x = AngleInSpace(points[i], points[preIndex[i]], tempX);
		double theta_y = AngleInSpace(points[i], points[preIndex[i]], tempY);
		double theta_z = AngleInSpace(points[i], points[preIndex[i]], tempZ);
		double len = getDistance(points[i], points[preIndex[i]]);
		//cout << "[" << i - 1 << "] "
		//	<< points[i] << endl
		//	<< points[preIndex[i]] << endl
		//	<< tempX << endl
		//	<< tempY << endl
		//	<< tempZ << endl
		//	<< theta_x << " " << theta_y << " " << theta_z << " " << len << " " << endl;
		this->data[i - 1] = BVHPoint(theta_x, theta_y, theta_z, len);
	}
}

void BVHData::printData()
{
	cout << "**************PRINT BVH DATA:" << endl;
	for (int i = 0; i < skeletonCount; i++)
	{
		cout << "[" << i << "]" << "   " << data[i].theta_x << "  "
			<< data[i].theta_y << "  "
			<< data[i].theta_z << "  "
			<< data[i].length << endl;
	}
}