
#include "BVHPoint.h"

BVHPoint::BVHPoint()
{

}

BVHPoint::BVHPoint(double x, double y, double z, double l)
{
	this->theta_x = x;
	this->theta_y = y;
	this->theta_z = z;
	this->length = l;
}
