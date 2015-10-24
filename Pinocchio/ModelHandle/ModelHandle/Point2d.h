
#ifndef __POINT2D_H__
#define __POINT2D_H__

class Point2d
{
public :
	double x;
	double y;

	Point2d();
	Point2d(double xx, double yy);

	bool operator > (const Point2d& p);
	bool operator < (const Point2d& p);
	bool operator == (const Point2d& p);
	static bool defaultSort(const Point2d& a, const Point2d& b);
	
};


#endif	//__POINT2D_H__
