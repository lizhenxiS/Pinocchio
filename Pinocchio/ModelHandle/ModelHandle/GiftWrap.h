
#ifndef __GIFTWRAP_H__
#define __GIFTWRAP_H__

#include "pinocchioApi.h"
#include "Point2d.h"

/*
参考博客 http://www.tanglei.name/convex-hull-of-points/
*/

class GiftWrap
{

private :
	vector<Point2d> points;		//2D平面的点集
	vector<Vector3> vertices;	//3D空间中的点集
	vector<Point2d> convexHull;	//点集的凸包
	void giftWraping();
	bool to_left(Point2d a, Point2d b, Point2d p);	//检测p是否在ab左边

public :
	GiftWrap();
	GiftWrap(const vector<Vector3> vertices);
	bool isIn(const Vector3 vertice);
	vector<Point2d> getConvexHull();

};



#endif	//__GIFTWRAP_H__


