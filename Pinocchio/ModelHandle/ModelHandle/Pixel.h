

#ifndef __PIXEL_H__
#define __PIXEL_H__

/*
用于计算模型体积的体素单元
*/
class Pixel
{
public:
	double length;		//初始方向沿X轴
	double wide;		//初始方向沿Y轴
	double height;		//初始方向沿Z轴
	int attachPoint;	//关联点序号
	float pos[3];		//体素中心位置

	Pixel();
	Pixel(float* pos, double l, double w, double h, int ap);
	double getVolume();	//获取体素体积

};

#endif	//__PIXEL_H__


