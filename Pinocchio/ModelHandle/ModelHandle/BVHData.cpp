//
//
//#include "BVHData.h"
//#include "GenerateTool.h"
//
//
//
//void BVHData::initDefaultSkeleton()
//{
//	defaultSkeleton[0] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[1] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[2] = BVHPoint(Axes_Y, 0, 0, 0);
//	defaultSkeleton[3] = BVHPoint(Axes_X, 180, 0, 0);
//	defaultSkeleton[4] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[5] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[6] = BVHPoint(Axes_Z, 0, 0, 0);
//	defaultSkeleton[7] = BVHPoint(Axes_X, 0, 0, 0);
//	defaultSkeleton[8] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[9] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[10] = BVHPoint(Axes_Z, 0, 0, 0);
//	defaultSkeleton[11] = BVHPoint(Axes_X, 180, 0, 0);
//	defaultSkeleton[12] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[13] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[14] = BVHPoint(Axes_X, 0, 0, 0);
//	defaultSkeleton[15] = BVHPoint(Axes_Y, 180, 0, 0);
//	defaultSkeleton[16] = BVHPoint(Axes_Y, 180, 0, 0);
//}
////
////void BVHData::initDefaultRelayAxes()
////{
////	defaultRelayAxes[0] = Axes_Y;
////	defaultRelayAxes[1] = Axes_Y;
////	defaultRelayAxes[2] = Axes_Y;
////	defaultRelayAxes[3] = Axes_X;
////	defaultRelayAxes[4] = Axes_Y;
////	defaultRelayAxes[5] = Axes_Y;
////	defaultRelayAxes[6] = Axes_Z;
////	defaultRelayAxes[7] = Axes_X;
////	defaultRelayAxes[8] = Axes_Y;
////	defaultRelayAxes[9] = Axes_Y;
////	defaultRelayAxes[10] = Axes_Z;
////	defaultRelayAxes[11] = Axes_X;
////	defaultRelayAxes[12] = Axes_Y;
////	defaultRelayAxes[13] = Axes_Y;
////	defaultRelayAxes[14] = Axes_X;
////	defaultRelayAxes[15] = Axes_Y;
////	defaultRelayAxes[16] = Axes_Y;
////}
//
///*
//points 所有结点数组， 0 - 17 共18
//preIndex 所有结点的前序结点数组， 0 - 17 共18 0号结点前序为-1
//*/
//BVHData::BVHData(vector<Vector3>& points, int* preIndex)
//{
//	//cout << "debug + " + points.size() << endl;
//	initDefaultSkeleton();
//	this->pointCount = 18;
//	this->skeletonCount = pointCount - 1;
//	this->data = new BVHPoint[pointCount - 1];
//	for (int i = 1; i < pointCount; i++)
//	{
//		Vector3 prePoint = points[preIndex[i]];
//		Vector3 selfPoint = points[i] - prePoint;	//local坐标系下自身坐标
//		RelayAxes tempAxes = defaultSkeleton[i - 1].defaultAxes;
//		double temp_alpha;
//		double temp_beta;
//		double temp_length = getDistance(points[i], prePoint);
//		Vector3 temp;
//
//		switch (tempAxes)
//		{
//		case Axes_X:
//			temp = Vector3(points[i][0] - prePoint[0], 0, points[i][2] - prePoint[2]);	//local坐标系下xoz平面投影点
//			/*绕Y转*/
//			if (temp[2] >= 0)
//			{
//				temp_alpha = -AngleInSpace(Vector3(1, 0, 0), Vector3(0, 0, 0), temp);
//			}
//			else
//			{
//				temp_alpha = AngleInSpace(Vector3(1, 0, 0), Vector3(0, 0, 0), temp);
//			}
//			/*绕Z转*/
//			if (temp[1] >= 0)
//			{
//				temp_beta = AngleInSpace(selfPoint, Vector3(0, 0, 0), temp);
//			}
//			else
//			{
//				temp_beta = -AngleInSpace(selfPoint, Vector3(0, 0, 0), temp);
//			}
//			break;
//		case Axes_Y:
//			temp = Vector3(points[i][0] - prePoint[0], points[i][1] - prePoint[1], 0);	//local坐标系下xoy平面投影点
//			/*绕Z转*/
//			if (temp[0] >= 0)
//			{
//				temp_alpha = -AngleInSpace(Vector3(0, 1, 0), Vector3(0, 0, 0), temp);
//			}
//			else
//			{
//				temp_alpha = AngleInSpace(Vector3(0, 1, 0), Vector3(0, 0, 0), temp);
//			}
//			/*绕X转*/
//			if (temp[2] >= 0)
//			{
//				temp_beta = AngleInSpace(selfPoint, Vector3(0, 0, 0), temp);
//			}
//			else
//			{
//				temp_beta = -AngleInSpace(selfPoint, Vector3(0, 0, 0), temp);
//			}
//			break;
//		case Axes_Z:
//			temp = Vector3(0, points[i][1] - prePoint[1], points[i][2] - prePoint[2]);	//local坐标系下yoz平面投影点
//			/*绕X转*/
//			if (temp[1] >= 0)
//			{
//				temp_alpha = -AngleInSpace(Vector3(0, 0, 1), Vector3(0, 0, 0), temp);
//			}
//			else
//			{
//				temp_alpha = AngleInSpace(Vector3(0, 0, 1), Vector3(0, 0, 0), temp);
//			}
//			/*绕Y转*/
//			if (temp[0] >= 0)
//			{
//				temp_beta = AngleInSpace(selfPoint, Vector3(0, 0, 0), temp);
//			}
//			else
//			{
//				temp_beta = -AngleInSpace(selfPoint, Vector3(0, 0, 0), temp);
//			}
//		default:
//			break;
//		}
//
//		this->data[i - 1] = BVHPoint(tempAxes, temp_alpha, temp_beta, temp_length);
//	}
//}
//
//void BVHData::printData()
//{
//	cout << "**************PRINT BVH DATA:" << endl;
//	cout << "NUM RELAYAXES ALPHA BETA LENGTH" << endl;
//	for (int i = 0; i < skeletonCount; i++)
//	{
//		cout << "[" << i << "]" << "   " << data[i].defaultAxes << "  "
//			<< data[i].angle_alpha << "  "
//			<< data[i].angle_beta << "  "
//			<< data[i].length << endl;
//	}
//}