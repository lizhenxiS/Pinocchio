
#ifndef _BVHPOINT_H__
#define _BVHPOINT_H__



class BVHPoint
{
public :
	double theta_x;	//以父骨骼为起点，xoy为起始面，沿x轴的偏转角
	double theta_y;	//以父骨骼为起点，xoy为起始面，沿y轴的偏转角
	double theta_z;	//以父骨骼为起点，xoy为起始面，沿z轴的偏转角
	double length;	//骨骼长度

	BVHPoint();
	BVHPoint(double x, double y, double z, double l);
};

#endif	//_BVHPOINT_H__

