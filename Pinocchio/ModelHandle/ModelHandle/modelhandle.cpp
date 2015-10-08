

#include "modelhandle.h"
#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qfiledialog.h>
#include "GenerateTool.h"

extern float angle;
extern int rotateX;
extern int rotateY;
extern int rotateZ;
extern bool drawSkeleton;
extern bool drawMesh;
extern bool drawCoord;
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
		drawSkeleton = true;
	else
		drawSkeleton = false;
}

void ModelHandle::slotMeshBox()
{
	if (ui.mesh_box->isChecked())
		drawMesh = true;
	else
		drawMesh = false;
}

void ModelHandle::slotCoordBox()
{
	if (ui.coord_box->isChecked())
		drawCoord = true;
	else
		drawCoord = false;
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

	ui.skeleton_1->setEnabled(false);
	ui.skeleton_2->setEnabled(false);
	ui.skeleton_3->setEnabled(false);
	ui.skeleton_4->setEnabled(false);
	ui.skeleton_5->setEnabled(false);
	ui.skeleton_6->setEnabled(false);
	ui.skeleton_7->setEnabled(false);
	ui.skeleton_8->setEnabled(false);
	ui.skeleton_9->setEnabled(false);
	ui.skeleton_10->setEnabled(false);
	ui.skeleton_11->setEnabled(false);
	ui.skeleton_12->setEnabled(false);
	ui.skeleton_13->setEnabled(false);
	ui.skeleton_14->setEnabled(false);
	ui.skeleton_15->setEnabled(false);
	ui.skeleton_16->setEnabled(false);
	ui.skeleton_17->setEnabled(false);

	ui.cst_1->setEnabled(false);
	ui.cst_2->setEnabled(false);
	ui.cst_3->setEnabled(false);
	ui.cst_4->setEnabled(false);
	ui.cst_5->setEnabled(false);
	ui.cst_6->setEnabled(false);
	ui.cst_7->setEnabled(false);
	ui.cst_8->setEnabled(false);
	ui.cst_9->setEnabled(false);
	ui.cst_10->setEnabled(false);
	ui.cst_11->setEnabled(false);
	ui.cst_12->setEnabled(false);
	ui.cst_13->setEnabled(false);
	ui.cst_14->setEnabled(false);
	ui.cst_15->setEnabled(false);
	ui.cst_16->setEnabled(false);
	ui.cst_17->setEnabled(false);

	ui.ClearButton->setEnabled(false);

	ui.skeleton_box->setEnabled(false);
	ui.mesh_box->setEnabled(false);
	ui.coord_box->setEnabled(false);
	ui.smoothBox->setEnabled(false);
	ui.stressBox->setEnabled(false);

	ui.SkeletonChooseBox->setEnabled(false);
	ui.curSkeletonAlpha->setEnabled(false);
	ui.curSkeletonBeta->setEnabled(false);
	ui.curSkeletonLength->setEnabled(false);
	ui.SkeletonChangeButton->setEnabled(false);
}

void ModelHandle::enableSlider()
{
	ui.rotate_x->setEnabled(true);
	ui.rotate_y->setEnabled(true);
	ui.rotate_z->setEnabled(true);
	ui.updown->setEnabled(true);

	ui.skeleton_1->setEnabled(true);
	ui.skeleton_2->setEnabled(true);
	ui.skeleton_3->setEnabled(true);
	ui.skeleton_4->setEnabled(true);
	ui.skeleton_5->setEnabled(true);
	ui.skeleton_6->setEnabled(true);
	ui.skeleton_7->setEnabled(true);
	ui.skeleton_8->setEnabled(true);
	ui.skeleton_9->setEnabled(true);
	ui.skeleton_10->setEnabled(true);
	ui.skeleton_11->setEnabled(true);
	ui.skeleton_12->setEnabled(true);
	ui.skeleton_13->setEnabled(true);
	ui.skeleton_14->setEnabled(true);
	ui.skeleton_15->setEnabled(true);
	ui.skeleton_16->setEnabled(true);
	ui.skeleton_17->setEnabled(true);

	ui.cst_1->setEnabled(true);
	ui.cst_2->setEnabled(true);
	ui.cst_3->setEnabled(true);
	ui.cst_4->setEnabled(true);
	ui.cst_5->setEnabled(true);
	ui.cst_6->setEnabled(true);
	ui.cst_7->setEnabled(true);
	ui.cst_8->setEnabled(true);
	ui.cst_9->setEnabled(true);
	ui.cst_10->setEnabled(true);
	ui.cst_11->setEnabled(true);
	ui.cst_12->setEnabled(true);
	ui.cst_13->setEnabled(true);
	ui.cst_14->setEnabled(true);
	ui.cst_15->setEnabled(true);
	ui.cst_16->setEnabled(true);
	ui.cst_17->setEnabled(true);

	ui.ClearButton->setEnabled(true);

	ui.skeleton_box->setEnabled(true);
	ui.mesh_box->setEnabled(true);
	ui.coord_box->setEnabled(true);
	ui.smoothBox->setEnabled(true);
	ui.stressBox->setEnabled(true);

	ui.SkeletonChooseBox->setEnabled(true);
	ui.curSkeletonAlpha->setEnabled(true);
	ui.curSkeletonBeta->setEnabled(true);
	ui.curSkeletonLength->setEnabled(true);
	ui.SkeletonChangeButton->setEnabled(true);
}

void ModelHandle::initSlider()
{
	ui.rotate_x->setValue(0);
	ui.rotate_y->setValue(0);
	ui.rotate_z->setValue(0);
	ui.updown->setValue(0);

	ui.skeleton_1->setValue(0);
	ui.skeleton_2->setValue(0);
	ui.skeleton_3->setValue(0);
	ui.skeleton_4->setValue(0);
	ui.skeleton_5->setValue(0);
	ui.skeleton_6->setValue(0);
	ui.skeleton_7->setValue(0);
	ui.skeleton_8->setValue(0);
	ui.skeleton_9->setValue(0);
	ui.skeleton_10->setValue(0);
	ui.skeleton_11->setValue(0);
	ui.skeleton_12->setValue(0);
	ui.skeleton_13->setValue(0);
	ui.skeleton_14->setValue(0);
	ui.skeleton_15->setValue(0);
	ui.skeleton_16->setValue(0);
	ui.skeleton_17->setValue(0);

	ui.cst_1->setValue(0);
	ui.cst_2->setValue(0);
	ui.cst_3->setValue(0);
	ui.cst_4->setValue(0);
	ui.cst_5->setValue(0);
	ui.cst_6->setValue(0);
	ui.cst_7->setValue(0);
	ui.cst_8->setValue(0);
	ui.cst_9->setValue(0);
	ui.cst_10->setValue(0);
	ui.cst_11->setValue(0);
	ui.cst_12->setValue(0);
	ui.cst_13->setValue(0);
	ui.cst_14->setValue(0);
	ui.cst_15->setValue(0);
	ui.cst_16->setValue(0);
	ui.cst_17->setValue(0);

	ui.skeleton_box->setChecked(true);
	ui.mesh_box->setChecked(true);
	ui.coord_box->setChecked(true);

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

	ui.skeleton_1->setRange(-200, 200);
	ui.skeleton_2->setRange(-200, 200);
	ui.skeleton_3->setRange(-200, 200);
	ui.skeleton_4->setRange(-200, 200);
	ui.skeleton_5->setRange(-200, 200);
	ui.skeleton_6->setRange(-200, 200);
	ui.skeleton_7->setRange(-200, 200);
	ui.skeleton_8->setRange(-200, 200);
	ui.skeleton_9->setRange(-200, 200);
	ui.skeleton_10->setRange(-200, 200);
	ui.skeleton_11->setRange(-200, 200);
	ui.skeleton_12->setRange(-200, 200);
	ui.skeleton_13->setRange(-200, 200);
	ui.skeleton_14->setRange(-200, 200);
	ui.skeleton_15->setRange(-200, 200);
	ui.skeleton_16->setRange(-200, 200);
	ui.skeleton_17->setRange(-200, 200);

	ui.cst_1->setRange(0, 100);
	ui.cst_2->setRange(0, 100);
	ui.cst_3->setRange(0, 100);
	ui.cst_4->setRange(0, 100);
	ui.cst_5->setRange(0, 100);
	ui.cst_6->setRange(0, 100);
	ui.cst_7->setRange(0, 100);
	ui.cst_8->setRange(0, 100);
	ui.cst_9->setRange(0, 100);
	ui.cst_10->setRange(0, 100);
	ui.cst_11->setRange(0, 100);
	ui.cst_12->setRange(0, 100);
	ui.cst_13->setRange(0, 100);
	ui.cst_14->setRange(0, 100);
	ui.cst_15->setRange(0, 100);
	ui.cst_16->setRange(0, 100);
	ui.cst_17->setRange(0, 100);
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

	connect(ui.skeleton_1, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_1()));
	connect(ui.skeleton_2, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_2()));
	connect(ui.skeleton_3, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_3()));
	connect(ui.skeleton_4, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_4()));
	connect(ui.skeleton_5, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_5()));
	connect(ui.skeleton_6, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_6()));
	connect(ui.skeleton_7, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_7()));
	connect(ui.skeleton_8, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_8()));
	connect(ui.skeleton_9, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_9()));
	connect(ui.skeleton_10, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_10()));
	connect(ui.skeleton_11, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_11()));
	connect(ui.skeleton_12, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_12()));
	connect(ui.skeleton_13, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_13()));
	connect(ui.skeleton_14, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_14()));
	connect(ui.skeleton_15, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_15()));
	connect(ui.skeleton_16, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_16()));
	connect(ui.skeleton_17, SIGNAL(valueChanged(int)), this, SLOT(slotSkeleton_17()));

	connect(ui.cst_1, SIGNAL(valueChanged(int)), this, SLOT(slotCst_1()));
	connect(ui.cst_2, SIGNAL(valueChanged(int)), this, SLOT(slotCst_2()));
	connect(ui.cst_3, SIGNAL(valueChanged(int)), this, SLOT(slotCst_3()));
	connect(ui.cst_4, SIGNAL(valueChanged(int)), this, SLOT(slotCst_4()));
	connect(ui.cst_5, SIGNAL(valueChanged(int)), this, SLOT(slotCst_5()));
	connect(ui.cst_6, SIGNAL(valueChanged(int)), this, SLOT(slotCst_6()));
	connect(ui.cst_7, SIGNAL(valueChanged(int)), this, SLOT(slotCst_7()));
	connect(ui.cst_8, SIGNAL(valueChanged(int)), this, SLOT(slotCst_8()));
	connect(ui.cst_9, SIGNAL(valueChanged(int)), this, SLOT(slotCst_9()));
	connect(ui.cst_10, SIGNAL(valueChanged(int)), this, SLOT(slotCst_10()));
	connect(ui.cst_11, SIGNAL(valueChanged(int)), this, SLOT(slotCst_11()));
	connect(ui.cst_12, SIGNAL(valueChanged(int)), this, SLOT(slotCst_12()));
	connect(ui.cst_13, SIGNAL(valueChanged(int)), this, SLOT(slotCst_13()));
	connect(ui.cst_14, SIGNAL(valueChanged(int)), this, SLOT(slotCst_14()));
	connect(ui.cst_15, SIGNAL(valueChanged(int)), this, SLOT(slotCst_15()));
	connect(ui.cst_16, SIGNAL(valueChanged(int)), this, SLOT(slotCst_16()));
	connect(ui.cst_17, SIGNAL(valueChanged(int)), this, SLOT(slotCst_17()));

	connect(ui.skeleton_box, SIGNAL(stateChanged(int)), this, SLOT(slotSkeletonBox()));
	connect(ui.mesh_box, SIGNAL(stateChanged(int)), this, SLOT(slotMeshBox()));
	connect(ui.coord_box, SIGNAL(stateChanged(int)), this, SLOT(slotCoordBox()));
	connect(ui.smoothBox, SIGNAL(stateChanged(int)), this, SLOT(slotSmoothBox()));
	connect(ui.stressBox, SIGNAL(stateChanged(int)), this, SLOT(slotStressBox()));

	connect(ui.energyButton, SIGNAL(clicked()), this, SLOT(slotEnergyButton()));
	connect(ui.SkeletonChooseBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSkeletonChooseBox()));
	connect(ui.SkeletonChangeButton, SIGNAL(clicked()), this, SLOT(slotBVHChange()));
}


void ModelHandle::slotSkeletonChooseBox()
{
	int curSkeleton = ui.SkeletonChooseBox->currentIndex();
	double curSkeletonAlpha = ui.showWidget->model->BvhData->data[curSkeleton].angle_alpha;
	double curSkeletonBeta = ui.showWidget->model->BvhData->data[curSkeleton].angle_beta;
	double curSkeletonLen = ui.showWidget->model->BvhData->data[curSkeleton].length;
	ui.curSkeletonAlpha->setText(QString::fromStdString(toString(curSkeletonAlpha)));
	ui.curSkeletonBeta->setText(QString::fromStdString(toString(curSkeletonBeta)));
	ui.curSkeletonLength->setText(QString::fromStdString(toString(curSkeletonLen)));
}

void ModelHandle::slotBVHChange()
{
	double bone = ui.SkeletonChooseBox->currentIndex() + 1;
	double newSkeletonAlpha = ui.curSkeletonAlpha->text().toDouble();
	double newSkeletonBeta = ui.curSkeletonBeta->text().toDouble();
	double newSkeletonLen = ui.curSkeletonLength->text().toDouble();

	ui.showWidget->model->changeSkeleton(bone, newSkeletonAlpha, newSkeletonBeta, newSkeletonLen);
	ui.showWidget->model->BvhData->printData();
}

void ModelHandle::slotObj_out()
{
	ui.showWidget->model->OutPutMesh();
	//ui.Obj_out->setEnabled(false);
}

void ModelHandle::slotRotateX()
{
	rotateX = ui.rotate_x->value();
}

void ModelHandle::slotRotateY()
{
	rotateY = ui.rotate_y->value();
}

void ModelHandle::slotRotateZ()
{
	rotateZ = ui.rotate_z->value();
}

void ModelHandle::slotUPDOWN()
{
	if (ui.updown->value() >= 0)
		angle = 60. - ui.updown->value() / 2;
	else
		angle = 60.0 - ui.updown->value();
}

void ModelHandle::slotEnergyButton()
{
	ui.showWidget->model->CountingEnergy();
}

void ModelHandle::slotSkeleton_1()
{
	if (ui.skeleton_1->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(1, ((double)ui.skeleton_1->value())/100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(1, (((double)ui.skeleton_1->value())/100 + 5)/5);
}

void ModelHandle::slotSkeleton_2()
{
	if (ui.skeleton_2->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(2, ((double)ui.skeleton_2->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(2, (((double)ui.skeleton_2->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_3()
{
	if (ui.skeleton_3->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(3, ((double)ui.skeleton_3->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(3, (((double)ui.skeleton_3->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_4()
{
	if (ui.skeleton_4->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(4, ((double)ui.skeleton_4->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(4, (((double)ui.skeleton_4->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_5()
{
	if (ui.skeleton_5->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(5, ((double)ui.skeleton_5->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(5, (((double)ui.skeleton_5->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_6()
{
	if (ui.skeleton_6->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(6, ((double)ui.skeleton_6->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(6, (((double)ui.skeleton_6->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_7()
{
	if (ui.skeleton_7->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(7, ((double)ui.skeleton_7->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(7, (((double)ui.skeleton_7->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_8()
{
	if (ui.skeleton_8->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(8, ((double)ui.skeleton_8->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(8, (((double)ui.skeleton_8->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_9()
{
	if (ui.skeleton_9->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(9, ((double)ui.skeleton_9->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(9, (((double)ui.skeleton_9->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_10()
{
	if (ui.skeleton_10->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(10, ((double)ui.skeleton_10->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(10, (((double)ui.skeleton_10->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_11()
{
	if (ui.skeleton_11->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(11, ((double)ui.skeleton_11->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(11, (((double)ui.skeleton_11->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_12()
{
	if (ui.skeleton_12->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(12, ((double)ui.skeleton_12->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(12, (((double)ui.skeleton_12->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_13()
{
	if (ui.skeleton_13->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(13, ((double)ui.skeleton_13->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(13, (((double)ui.skeleton_13->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_14()
{
	if (ui.skeleton_14->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(14, ((double)ui.skeleton_14->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(14, (((double)ui.skeleton_14->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_15()
{
	if (ui.skeleton_15->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(15, ((double)ui.skeleton_15->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(15, (((double)ui.skeleton_15->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_16()
{
	if (ui.skeleton_16->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(16, ((double)ui.skeleton_16->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(16, (((double)ui.skeleton_16->value()) / 100 + 5) / 5);
}

void ModelHandle::slotSkeleton_17()
{
	if (ui.skeleton_17->value() > 0)
		ui.showWidget->model->changeSingleSkeleton(17, ((double)ui.skeleton_17->value()) / 100 + 1);
	else
		ui.showWidget->model->changeSingleSkeleton(17, (((double)ui.skeleton_17->value()) / 100 + 5) / 5);
}

void ModelHandle::slotCst_1()
{
	ui.showWidget->model->ChangeGrowScale(1, (double)ui.cst_1->value() / 100);
}

void ModelHandle::slotCst_2()
{
	ui.showWidget->model->ChangeGrowScale(2, (double)ui.cst_2->value() / 100);
}

void ModelHandle::slotCst_3()
{
	ui.showWidget->model->ChangeGrowScale(3, (double)ui.cst_3->value() / 100);
}

void ModelHandle::slotCst_4()
{
	ui.showWidget->model->ChangeGrowScale(4, (double)ui.cst_4->value() / 100);
}

void ModelHandle::slotCst_5()
{
	ui.showWidget->model->ChangeGrowScale(5, (double)ui.cst_5->value() / 100);
}

void ModelHandle::slotCst_6()
{
	ui.showWidget->model->ChangeGrowScale(6, (double)ui.cst_6->value() / 100);
}

void ModelHandle::slotCst_7()
{
	ui.showWidget->model->ChangeGrowScale(7, (double)ui.cst_7->value() / 100);
}

void ModelHandle::slotCst_8()
{
	ui.showWidget->model->ChangeGrowScale(8, (double)ui.cst_8->value() / 100);
}

void ModelHandle::slotCst_9()
{
	ui.showWidget->model->ChangeGrowScale(9, (double)ui.cst_9->value() / 100);
}

void ModelHandle::slotCst_10()
{
	ui.showWidget->model->ChangeGrowScale(10, (double)ui.cst_10->value() / 100);
}

void ModelHandle::slotCst_11()
{
	ui.showWidget->model->ChangeGrowScale(11, (double)ui.cst_11->value() / 100);
}

void ModelHandle::slotCst_12()
{
	ui.showWidget->model->ChangeGrowScale(12, (double)ui.cst_12->value() / 100);
}

void ModelHandle::slotCst_13()
{
	ui.showWidget->model->ChangeGrowScale(13, (double)ui.cst_13->value() / 100);
}

void ModelHandle::slotCst_14()
{
	ui.showWidget->model->ChangeGrowScale(14, (double)ui.cst_14->value() / 100);
}

void ModelHandle::slotCst_15()
{
	ui.showWidget->model->ChangeGrowScale(15, (double)ui.cst_15->value() / 100);
}

void ModelHandle::slotCst_16()
{
	ui.showWidget->model->ChangeGrowScale(16, (double)ui.cst_16->value() / 100);
}

void ModelHandle::slotCst_17()
{
	ui.showWidget->model->ChangeGrowScale(17, (double)ui.cst_17->value() / 100);
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
	ui.light->setVisible(false);
}
