

#ifndef __PIXEL_H__
#define __PIXEL_H__

/*
���ڼ���ģ����������ص�Ԫ
*/
class Pixel
{
public:
	double length;		//��ʼ������X��
	double wide;		//��ʼ������Y��
	double height;		//��ʼ������Z��
	int attachPoint;	//���������
	float pos[3];		//��������λ��

	Pixel();
	Pixel(float* pos, double l, double w, double h, int ap);
	double getVolume();	//��ȡ�������

};

#endif	//__PIXEL_H__


