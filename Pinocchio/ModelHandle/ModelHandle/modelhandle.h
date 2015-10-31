#ifndef MODELHANDLE_H
#define MODELHANDLE_H

#include <QtWidgets/QWidget>
#include "ui_modelhandle.h"


class ModelHandle : public QWidget
{
	Q_OBJECT

public:
	ModelHandle(QWidget *parent = 0);
	~ModelHandle();

private:
	Ui::ModelHandleClass ui;
	void disableSlider();
	void enableSlider();
	void initSlider();
	void initBox();
	void setSlider();
	void setConnect();

private slots:
	void slotReadButton();
	void slotClearButton();
	void slotObj_out();

	void slotRotateX();
	void slotRotateY();
	void slotRotateZ();
	void slotUPDOWN();
	void slotEnergyButton();

	void slotSkeleton_1();
	void slotSkeleton_2();
	void slotSkeleton_3();
	void slotSkeleton_4();
	void slotSkeleton_5();
	void slotSkeleton_6();
	void slotSkeleton_7();
	void slotSkeleton_8();
	void slotSkeleton_9();
	void slotSkeleton_10();
	void slotSkeleton_11();
	void slotSkeleton_12();
	void slotSkeleton_13();
	void slotSkeleton_14();
	void slotSkeleton_15();
	void slotSkeleton_16();
	void slotSkeleton_17();

	void slotCst_1();
	void slotCst_2();
	void slotCst_3();
	void slotCst_4();
	void slotCst_5();
	void slotCst_6();
	void slotCst_7();
	void slotCst_8();
	void slotCst_9();
	void slotCst_10();
	void slotCst_11();
	void slotCst_12();
	void slotCst_13();
	void slotCst_14();
	void slotCst_15();
	void slotCst_16();
	void slotCst_17();

	void slotSkeletonBox();
	void slotMeshBox();
	void slotCoordBox();
	void slotVoxelBox();

	void slotSmoothBox();
	void slotStressBox();

	void slotSkeletonChooseBox();
	void slotBVHChange();
};

#endif // MODELHANDLE_H
