

#ifndef __ONCEROTATION_H__
#define __ONCEROTATION_H__

#include "pinocchioApi.h"

/*
��¼Χ����������תһ�ε���Ϣ
*/
class OnceRotation
{
public :
	double angle;
	Vector3 lineStart;
	Vector3 lineEnd;

	OnceRotation();
	OnceRotation(double angle, Vector3 lineStart, Vector3 lineEnd);
	Vector3 pointRotate(Vector3 point);
};


#endif	//__ONCEROTATION_H__

