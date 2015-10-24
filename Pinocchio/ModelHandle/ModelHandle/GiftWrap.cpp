
#include "GiftWrap.h"
#include "GenerateMesh.h"


GiftWrap::GiftWrap()
{

}

GiftWrap::GiftWrap(const vector<Vector3> vertices)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		this->vertices.push_back(vertices[i]);
		this->points.push_back(Point2d(vertices[i][0], vertices[i][2]));
	}
	giftWraping();
}

bool GiftWrap::isIn(const Vector3 vertice)
{
	int nCross = 0;
	int nCount = convexHull.size();
	for (int i = 0; i < nCount; i++)
	{
		Point2d p1 = convexHull[i];
		Point2d p2 = convexHull[(i + 1) % nCount];
		double slope = (p2.y - p1.y) / (p2.x - p1.x);
		bool cond1 = (p1.x <= vertice[0]) && (vertice[0] < p2.x);
		bool cond2 = (p2.x <= vertice[0]) && (vertice[0] < p1.x);
		bool above = (vertice[2] < slope * (vertice[0] - p1.x) + p1.y);
		if ((cond1 || cond2) && above)
			nCross++;
		return (nCross % 2 != 0);
	}
}

vector<Point2d> GiftWrap::getConvexHull()
{
	return convexHull;
}

//检测p是否在line_start line_end左边
bool GiftWrap::to_left(Point2d line_start, Point2d line_end, Point2d p)
{
	Point2d h1 = line_end;
	Point2d h2 = line_start;
	double h2p[2] = { h2.x - p.x, h2.y - p.y };
	double h1p[2] = { h1.x - p.x, h1.y - p.y };

	//此处未考虑P在ab上的情况
	if ((h2p[0] * h1p[1] - h2p[1] * h1p[0]) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void GiftWrap::giftWraping()
{
	int ltl = 0;	//find the lowest-and-leftmost point
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].y < points[ltl].y ||
			(points[i].y == points[ltl].y && points[i].x < points[ltl].x))
			ltl = i;
	}
	convexHull.push_back(points[ltl]);
	for (int p = ltl;;)
	{
		int q = -1;
		for (int k = 0; k < points.size(); k++)
		{
			if ((k != p) && (q < 0 || !to_left(points[p], points[q], points[k])))
				q = k;	//update q if k lies in right of pq
		}
		if (ltl == q)
			break;		// has find out the convex hull circle
		//find q
		convexHull.push_back(points[q]);
		p = q;
	}
}
