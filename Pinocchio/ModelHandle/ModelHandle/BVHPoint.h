
#ifndef _BVHPOINT_H__
#define _BVHPOINT_H__



class BVHPoint
{
public :
	double theta_x;	//�Ը�����Ϊ��㣬xoyΪ��ʼ�棬��x���ƫת��
	double theta_y;	//�Ը�����Ϊ��㣬xoyΪ��ʼ�棬��y���ƫת��
	double theta_z;	//�Ը�����Ϊ��㣬xoyΪ��ʼ�棬��z���ƫת��
	double length;	//��������

	BVHPoint();
	BVHPoint(double x, double y, double z, double l);
};

#endif	//_BVHPOINT_H__

