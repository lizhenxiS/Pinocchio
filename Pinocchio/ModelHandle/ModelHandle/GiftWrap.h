
#ifndef __GIFTWRAP_H__
#define __GIFTWRAP_H__

#include "pinocchioApi.h"
#include "Point2d.h"

/*
�ο����� http://www.tanglei.name/convex-hull-of-points/
*/

class GiftWrap
{

private :
	vector<Point2d> points;		//2Dƽ��ĵ㼯
	vector<Vector3> vertices;	//3D�ռ��еĵ㼯
	vector<Point2d> convexHull;	//�㼯��͹��
	void giftWraping();
	bool to_left(Point2d a, Point2d b, Point2d p);	//���p�Ƿ���ab���

public :
	GiftWrap();
	GiftWrap(const vector<Vector3> vertices);
	bool isIn(const Vector3 vertice);
	vector<Point2d> getConvexHull();

};



#endif	//__GIFTWRAP_H__


