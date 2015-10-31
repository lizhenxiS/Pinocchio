

#ifndef __PIXEL_H__
#define __PIXEL_H__

/*
���ڼ���ģ����������ص�Ԫ
*/
class Pixel
{
public:
	double length;		//��ʼ������X��
	double width;		//��ʼ������Y��
	double height;		//��ʼ������Z��
	int attachPoint;	//���������
	float pos[3];		//��������λ��
	double box[8][3];	//����8����������

	Pixel();
	Pixel(float* pos, double l, double w, double h, int ap);
	void updateLWH();	//�������س����
	double getVolume();	//��ȡ�������

};

#endif	//__PIXEL_H__


