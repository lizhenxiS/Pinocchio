
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

	RelayAxes defaultAxes;	//Ĭ�Ϲ�������������ƽ�У��˴�ΪĬ��ƽ�е�������
	double angle_alpha;	//����XYZX��˳����Ĭ�����ĵ�һ����ŷ����ת�Ƕ�,�Ƕ����ն�Ӧ����ʱ�����
	double angle_beta;	//����XYZX��˳����Ĭ�����ĵڶ�����ŷ����ת�Ƕ�,�Ƕ����ն�Ӧ����ʱ�����

	double length;	//��������

	BVHPoint();
	BVHPoint(RelayAxes reAxes, double alpha, double beta, double len);
};

#endif	//_BVHPOINT_H__

