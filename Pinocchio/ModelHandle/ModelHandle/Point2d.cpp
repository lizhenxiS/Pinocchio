
#include "Point2d.h"
#include "GenerateTool.h"


Point2d::Point2d()
{
	x = 0;
	y = 0;
}

Point2d::Point2d(double xx, double yy)
{
	x = xx;
	y = yy;
}

bool Point2d::defaultSort(const Point2d& a, const Point2d& b)
{
	if (a.x > b.x)
	{
		return true;
	}
	else if (almostEqual(a.x, b.x, 0.0001))
	{
		if (a.y > b.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool Point2d::operator < (const Point2d& p)
{
	if (this->x < p.x)
	{
		return true;
	}
	else if (almostEqual(this->x, p.x, 0.0001))
	{
		if (this->y < p.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool Point2d::operator > (const Point2d& p)
{
	if (this->x > p.x)
	{
		return true;
	}
	else if (almostEqual(this->x, p.x, 0.0001))
	{
		if (this->y > p.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool Point2d::operator == (const Point2d& p)
{
	if (almostEqual(this->x, p.x, 0.0001)
		&& almostEqual(this->y, p.y, 0.0001))
	{
		return true;
	}
	else
	{
		return false;
	}
}



