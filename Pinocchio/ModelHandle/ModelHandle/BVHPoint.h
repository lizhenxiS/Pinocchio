
#ifndef _BVHPOINT_H__
#define _BVHPOINT_H__


#ifndef _RELAYAXES__
#define _RELAYAXES__

enum RelayAxes
{
	Axes_X, Axes_Y, Axes_Z
};

#endif	//_RELAYAXES__


class BVHPoint
{
public :

	RelayAxes defaultAxes;	//默认骨骼均与坐标轴平行，此处为默认平行的坐标轴
	double angle_alpha;	//依照XYZX的顺序，绕默认轴后的第一个轴欧拉旋转角度,角度依照对应轴逆时针计算
	double angle_beta;	//依照XYZX的顺序，绕默认轴后的第二个轴欧拉旋转角度,角度依照对应轴逆时针计算

	double length;	//骨骼长度

	BVHPoint();
	BVHPoint(RelayAxes reAxes, double alpha, double beta, double len);
};

#endif	//_BVHPOINT_H__

