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

	void slotSkeletonBox();
	void slotMeshBox();
	void slotCoordBox();
	void slotVoxelBox();

	void slotSmoothBox();
	void slotStressBox();

	void slotSkeletonChooseBox();
	void slotBVHChange();
	void slotSkeletonScale();

	void slotScopeButton();
};

#endif // MODELHANDLE_H
