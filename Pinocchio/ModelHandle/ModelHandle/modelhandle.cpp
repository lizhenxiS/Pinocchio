

#include "modelhandle.h"
#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qfiledialog.h>
#include "GenerateTool.h"


extern bool changeFromMap;
extern bool SmoothState;
extern bool StressState;

#define PI 3.1415926

ModelHandle::ModelHandle(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.light->setVisible(false);
	
	disableSlider();
	setSlider();
	setConnect();
}

void ModelHandle::slotSkeletonBox()
{
	if (ui.skeleton_box->isChecked())
		ui.showWidget->drawSkeleton = true;
	else
		ui.showWidget->drawSkeleton = false;
}

void ModelHandle::slotMeshBox()
{
	if (ui.mesh_box->isChecked())
		ui.showWidget->drawMesh = true;
	else
		ui.showWidget->drawMesh = false;
}

void ModelHandle::slotCoordBox()
{
	if (ui.coord_box->isChecked())
		ui.showWidget->drawCoord = true;
	else
		ui.showWidget->drawCoord = false;
}

void ModelHandle::slotVoxelBox()
{
	if (ui.voxel_box->isChecked())
		ui.showWidget->drawVoxel = true;
	else
		ui.showWidget->drawVoxel = false;
}

void ModelHandle::slotSmoothBox()
{
	if (ui.smoothBox->isChecked())
	{
		SmoothState = true;
		ui.stressBox->setChecked(false);	
	}
	else
		SmoothState = false;
}

void ModelHandle::slotStressBox()
{
	if (ui.stressBox->isChecked())
	{
		StressState = true;
		ui.smoothBox->setChecked(false);	
	}
	else
		StressState = false;
}

void ModelHandle::disableSlider()
{
	ui.rotate_x->setEnabled(false);
	ui.rotate_y->setEnabled(false);
	ui.rotate_z->setEnabled(false);
	ui.updown->setEnabled(false);

	ui.ClearButton->setEnabled(false);

	ui.skeleton_box->setEnabled(false);
	ui.mesh_box->setEnabled(false);
	ui.coord_box->setEnabled(false);
	ui.voxel_box->setEnabled(false);

	ui.smoothBox->setEnabled(false);
	ui.stressBox->setEnabled(false);

	ui.SkeletonChooseBox->setEnabled(false);
	ui.curSkeletonAlpha->setEnabled(false);
	ui.curSkeletonBeta->setEnabled(false);
	ui.curSkeletonScale->setEnabled(false);
	ui.SkeletonChangeButton->setEnabled(false);
}

void ModelHandle::enableSlider()
{
	ui.rotate_x->setEnabled(true);
	ui.rotate_y->setEnabled(true);
	ui.rotate_z->setEnabled(true);
	ui.updown->setEnabled(true);

	ui.ClearButton->setEnabled(true);

	ui.skeleton_box->setEnabled(true);
	ui.mesh_box->setEnabled(true);
	ui.coord_box->setEnabled(true);
	ui.voxel_box->setEnabled(true);

	ui.smoothBox->setEnabled(true);
	ui.stressBox->setEnabled(true);

	ui.SkeletonChooseBox->setEnabled(true);
	ui.curSkeletonAlpha->setEnabled(true);
	ui.curSkeletonBeta->setEnabled(true);
	ui.curSkeletonScale->setEnabled(true);
	ui.SkeletonChangeButton->setEnabled(true);
}

void ModelHandle::initSlider()
{
	ui.rotate_x->setValue(0);
	ui.rotate_y->setValue(0);
	ui.rotate_z->setValue(0);
	ui.updown->setValue(0);


	int curSkeleton = ui.SkeletonChooseBox->currentIndex();
	double curSkeletonAlpha = ui.showWidget->model->rotateAngle[curSkeleton][0];
	double curSkeletonBeta = ui.showWidget->model->rotateAngle[curSkeleton][1];
	double curSkeletonScale = ui.showWidget->model->skeletonScale[curSkeleton];
	double scaleTemp;
	if (curSkeletonScale > 1)
		scaleTemp = (curSkeletonScale - 1) * 100;
	else
		scaleTemp = (curSkeleton * 5 - 5) / 100;
	ui.curSkeletonAlpha->setValue(curSkeletonAlpha);
	ui.curSkeletonBeta->setValue(curSkeletonBeta);
	ui.curSkeletonScale->setValue(scaleTemp);
}

void ModelHandle::initBox()
{
	ui.skeleton_box->setChecked(true);
	ui.mesh_box->setChecked(true);
	ui.coord_box->setChecked(true);
	ui.voxel_box->setChecked(true);

	QStringList skeletonNames;
	skeletonNames << "skeleton1"
		<< "skeleton2" << "skeleton3"
		<< "skeleton4" << "skeleton5"
		<< "skeleton6" << "skeleton7"
		<< "skeleton8" << "skeleton9"
		<< "skeleton10" << "skeleton11"
		<< "skeleton12" << "skeleton13"
		<< "skeleton14"
		<< "skeleton15" << "skeleton16"
		<< "skeleton17";

	ui.SkeletonChooseBox->addItems(skeletonNames);
}

void ModelHandle::setSlider()
{
	ui.rotate_x->setRange(-180, 180);
	ui.rotate_y->setRange(-180, 180);
	ui.rotate_z->setRange(-180, 180);
	ui.updown->setRange(-90, 90);

	ui.curSkeletonAlpha->setRange(0, 180);
	ui.curSkeletonBeta->setRange(-180, 180);
	ui.curSkeletonScale->setRange(-200, 200);
}

void ModelHandle::setConnect()
{
	connect(ui.ReadButton, SIGNAL(clicked()), this, SLOT(slotReadButton()));
	connect(ui.ClearButton, SIGNAL(clicked()), this, SLOT(slotClearButton()));
	connect(ui.Obj_out, SIGNAL(clicked()), this, SLOT(slotObj_out()));

	connect(ui.rotate_x, SIGNAL(valueChanged(int)), this, SLOT(slotRotateX()));
	connect(ui.rotate_y, SIGNAL(valueChanged(int)), this, SLOT(slotRotateY()));
	connect(ui.rotate_z, SIGNAL(valueChanged(int)), this, SLOT(slotRotateZ()));
	connect(ui.updown, SIGNAL(valueChanged(int)), this, SLOT(slotUPDOWN()));

	connect(ui.skeleton_box, SIGNAL(stateChanged(int)), this, SLOT(slotSkeletonBox()));
	connect(ui.mesh_box, SIGNAL(stateChanged(int)), this, SLOT(slotMeshBox()));
	connect(ui.coord_box, SIGNAL(stateChanged(int)), this, SLOT(slotCoordBox()));
	connect(ui.voxel_box, SIGNAL(stateChanged(int)), this, SLOT(slotVoxelBox()));

	connect(ui.smoothBox, SIGNAL(stateChanged(int)), this, SLOT(slotSmoothBox()));
	connect(ui.stressBox, SIGNAL(stateChanged(int)), this, SLOT(slotStressBox()));

	connect(ui.energyButton, SIGNAL(clicked()), this, SLOT(slotEnergyButton()));

	connect(ui.SkeletonChooseBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSkeletonChooseBox()));
	connect(ui.SkeletonChangeButton, SIGNAL(clicked()), this, SLOT(slotBVHChange()));
	connect(ui.curSkeletonScale, SIGNAL(valueChanged(int)), this, SLOT(slotSkeletonScale()));
	connect(ui.ScopeButton, SIGNAL(clicked()), this, SLOT(slotScopeButton()));
	connect(ui.optButton, SIGNAL(clicked()), this, SLOT(slotOptButton()));
}

void ModelHandle::slotOptButton()
{
	ui.showWidget->model->autoOptimization();
}

void ModelHandle::slotScopeButton()
{
	ui.showWidget->model->checkSafetyRotateScope();
}

void ModelHandle::slotSkeletonChooseBox()
{
	int curSkeleton = ui.SkeletonChooseBox->currentIndex();
	double curSkeletonAlpha = ui.showWidget->model->rotateAngle[curSkeleton][0];
	double curSkeletonBeta = ui.showWidget->model->rotateAngle[curSkeleton][1];
	double curSkeletonScale = ui.showWidget->model->skeletonScale[curSkeleton];
	double scaleTemp;
	if (curSkeletonScale > 1)
		scaleTemp = (curSkeletonScale - 1) * 100;
	else
		scaleTemp = (curSkeleton * 5 - 5) / 100;
	ui.curSkeletonAlpha->setValue(curSkeletonAlpha);
	ui.curSkeletonBeta->setValue(curSkeletonBeta);
	ui.curSkeletonScale->setValue(scaleTemp);
}

void ModelHandle::slotBVHChange()
{
	double bone = ui.SkeletonChooseBox->currentIndex();
	double newSkeletonAlpha = ui.curSkeletonAlpha->value();
	double newSkeletonBeta = ui.curSkeletonBeta->value();

	ui.showWidget->model->changeSkeleton(bone, newSkeletonAlpha, newSkeletonBeta);
}

void ModelHandle::slotSkeletonScale()
{
	double bone = ui.SkeletonChooseBox->currentIndex();
	double scaleTemp = ui.curSkeletonScale->value();
	if (scaleTemp > 0)
		ui.showWidget->model->changeSingleSkeleton(bone + 1, ((double)scaleTemp) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(bone + 1, (((double)scaleTemp) / 100 + 5) / 5);
}

void ModelHandle::slotObj_out()
{
	ui.showWidget->model->OutPutMesh();
	//ui.Obj_out->setEnabled(false);
}

void ModelHandle::slotRotateX()
{
	ui.showWidget->rotateX = ui.rotate_x->value();
}

void ModelHandle::slotRotateY()
{
	ui.showWidget->rotateY = ui.rotate_y->value();
}

void ModelHandle::slotRotateZ()
{
	ui.showWidget->rotateZ = ui.rotate_z->value();
}

void ModelHandle::slotUPDOWN()
{
	if (ui.updown->value() >= 0)
		ui.showWidget->angle = 60. - ui.updown->value() / 2;
	else
		ui.showWidget->angle = 60.0 - ui.updown->value();
}

void ModelHandle::slotEnergyButton()
{
	ui.showWidget->model->CountingEnergy();
}

void ModelHandle::slotClearButton()
{
	ui.showWidget->model->clearModelChange();
	initSlider();
}

ModelHandle::~ModelHandle()
{

}

void ModelHandle::slotReadButton()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Select Obj Model"), ".",
		tr("Model(*.obj *.ply *.off *.gts *.stl)"));

	ui.light->setVisible(true);
	ui.light->repaint();

	ui.showWidget->inputFileName(file.toStdString());

	enableSlider();
	initSlider();
	initBox();
	ui.light->setVisible(false);
}
