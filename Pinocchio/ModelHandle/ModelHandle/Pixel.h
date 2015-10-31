

#ifndef __PIXEL_H__
#define __PIXEL_H__

/*
用于计算模型体积的体素单元
*/
class Pixel
{
public:
	double length;		//初始方向沿X轴
	double width;		//初始方向沿Y轴
	double height;		//初始方向沿Z轴
	int attachPoint;	//关联点序号
	float pos[3];		//体素中心位置
	double box[8][3];	//体素8个顶点坐标

	Pixel();
	Pixel(float* pos, double l, double w, double h, int ap);
	void updateLWH();	//更新体素长宽高
	double getVolume();	//获取体素体积

};

#endif	//__PIXEL_H__


