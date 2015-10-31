/********************************************************************************
** Form generated from reading UI file 'modelhandle.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELHANDLE_H
#define UI_MODELHANDLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_ModelHandleClass
{
public:
    GLWidget *showWidget;
    QPushButton *ClearButton;
    QGroupBox *groupBox;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QSlider *updown;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *rotate_x;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSlider *rotate_y;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSlider *rotate_z;
    QSlider *skeleton_2;
    QSlider *skeleton_3;
    QSlider *skeleton_4;
    QSlider *skeleton_5;
    QSlider *skeleton_6;
    QSlider *skeleton_7;
    QSlider *skeleton_8;
    QSlider *skeleton_9;
    QSlider *skeleton_1;
    QSlider *skeleton_10;
    QSlider *skeleton_16;
    QSlider *skeleton_14;
    QSlider *skeleton_15;
    QSlider *skeleton_17;
    QSlider *skeleton_13;
    QSlider *skeleton_12;
    QSlider *skeleton_11;
    QPushButton *ReadButton;
    QCheckBox *skeleton_box;
    QCheckBox *mesh_box;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QFrame *light;
    QPushButton *light_1;
    QPushButton *light_2;
    QPushButton *light_3;
    QLabel *label_26;
    QLabel *label_14;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QSlider *cst_1;
    QLabel *label_37;
    QLabel *label_38;
    QSlider *cst_2;
    QSlider *cst_3;
    QSlider *cst_4;
    QSlider *cst_5;
    QSlider *cst_6;
    QSlider *cst_7;
    QSlider *cst_8;
    QSlider *cst_9;
    QSlider *cst_10;
    QSlider *cst_11;
    QSlider *cst_12;
    QSlider *cst_13;
    QSlider *cst_14;
    QSlider *cst_15;
    QSlider *cst_16;
    QSlider *cst_17;
    QPushButton *Obj_out;
    QPushButton *energyButton;
    QCheckBox *smoothBox;
    QCheckBox *stressBox;
    QCheckBox *coord_box;
    QGroupBox *groupBox_2;
    QComboBox *SkeletonChooseBox;
    QLabel *label_39;
    QLabel *label_40;
    QLabel *label_41;
    QPushButton *SkeletonChangeButton;
    QSlider *curSkeletonAlpha;
    QSlider *curSkeletonBeta;
    QCheckBox *voxel_box;

    void setupUi(QWidget *ModelHandleClass)
    {
        if (ModelHandleClass->objectName().isEmpty())
            ModelHandleClass->setObjectName(QStringLiteral("ModelHandleClass"));
        ModelHandleClass->resize(1400, 600);
        showWidget = new GLWidget(ModelHandleClass);
        showWidget->setObjectName(QStringLiteral("showWidget"));
        showWidget->setGeometry(QRect(0, 0, 600, 600));
        ClearButton = new QPushButton(ModelHandleClass);
        ClearButton->setObjectName(QStringLiteral("ClearButton"));
        ClearButton->setGeometry(QRect(720, 540, 70, 30));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setWeight(75);
        ClearButton->setFont(font);
        groupBox = new QGroupBox(ModelHandleClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(630, 20, 231, 181));
        QFont font1;
        font1.setPointSize(10);
        groupBox->setFont(font1);
        layoutWidget_3 = new QWidget(groupBox);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(20, 150, 191, 29));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_5->setSpacing(25);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font);

        horizontalLayout_5->addWidget(label_5);

        updown = new QSlider(layoutWidget_3);
        updown->setObjectName(QStringLiteral("updown"));
        updown->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(updown);

        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 191, 122));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(10);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(25);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        horizontalLayout->addWidget(label);

        rotate_x = new QSlider(layoutWidget);
        rotate_x->setObjectName(QStringLiteral("rotate_x"));
        rotate_x->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(rotate_x);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(25);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        rotate_y = new QSlider(layoutWidget);
        rotate_y->setObjectName(QStringLiteral("rotate_y"));
        rotate_y->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(rotate_y);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(25);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        horizontalLayout_3->addWidget(label_3);

        rotate_z = new QSlider(layoutWidget);
        rotate_z->setObjectName(QStringLiteral("rotate_z"));
        rotate_z->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(rotate_z);


        verticalLayout->addLayout(horizontalLayout_3);

        skeleton_2 = new QSlider(ModelHandleClass);
        skeleton_2->setObjectName(QStringLiteral("skeleton_2"));
        skeleton_2->setGeometry(QRect(640, 240, 15, 120));
        skeleton_2->setOrientation(Qt::Vertical);
        skeleton_3 = new QSlider(ModelHandleClass);
        skeleton_3->setObjectName(QStringLiteral("skeleton_3"));
        skeleton_3->setGeometry(QRect(670, 240, 15, 120));
        skeleton_3->setOrientation(Qt::Vertical);
        skeleton_4 = new QSlider(ModelHandleClass);
        skeleton_4->setObjectName(QStringLiteral("skeleton_4"));
        skeleton_4->setGeometry(QRect(700, 240, 15, 120));
        skeleton_4->setOrientation(Qt::Vertical);
        skeleton_5 = new QSlider(ModelHandleClass);
        skeleton_5->setObjectName(QStringLiteral("skeleton_5"));
        skeleton_5->setGeometry(QRect(730, 240, 15, 120));
        skeleton_5->setOrientation(Qt::Vertical);
        skeleton_6 = new QSlider(ModelHandleClass);
        skeleton_6->setObjectName(QStringLiteral("skeleton_6"));
        skeleton_6->setGeometry(QRect(760, 240, 15, 120));
        skeleton_6->setOrientation(Qt::Vertical);
        skeleton_7 = new QSlider(ModelHandleClass);
        skeleton_7->setObjectName(QStringLiteral("skeleton_7"));
        skeleton_7->setGeometry(QRect(790, 240, 15, 120));
        skeleton_7->setOrientation(Qt::Vertical);
        skeleton_8 = new QSlider(ModelHandleClass);
        skeleton_8->setObjectName(QStringLiteral("skeleton_8"));
        skeleton_8->setGeometry(QRect(820, 240, 15, 120));
        skeleton_8->setOrientation(Qt::Vertical);
        skeleton_9 = new QSlider(ModelHandleClass);
        skeleton_9->setObjectName(QStringLiteral("skeleton_9"));
        skeleton_9->setGeometry(QRect(850, 240, 15, 120));
        skeleton_9->setOrientation(Qt::Vertical);
        skeleton_1 = new QSlider(ModelHandleClass);
        skeleton_1->setObjectName(QStringLiteral("skeleton_1"));
        skeleton_1->setGeometry(QRect(610, 240, 15, 120));
        skeleton_1->setOrientation(Qt::Vertical);
        skeleton_10 = new QSlider(ModelHandleClass);
        skeleton_10->setObjectName(QStringLiteral("skeleton_10"));
        skeleton_10->setGeometry(QRect(880, 240, 15, 120));
        skeleton_10->setOrientation(Qt::Vertical);
        skeleton_16 = new QSlider(ModelHandleClass);
        skeleton_16->setObjectName(QStringLiteral("skeleton_16"));
        skeleton_16->setGeometry(QRect(1060, 240, 15, 120));
        skeleton_16->setOrientation(Qt::Vertical);
        skeleton_14 = new QSlider(ModelHandleClass);
        skeleton_14->setObjectName(QStringLiteral("skeleton_14"));
        skeleton_14->setGeometry(QRect(1000, 240, 15, 120));
        skeleton_14->setOrientation(Qt::Vertical);
        skeleton_15 = new QSlider(ModelHandleClass);
        skeleton_15->setObjectName(QStringLiteral("skeleton_15"));
        skeleton_15->setGeometry(QRect(1030, 240, 15, 120));
        skeleton_15->setOrientation(Qt::Vertical);
        skeleton_17 = new QSlider(ModelHandleClass);
        skeleton_17->setObjectName(QStringLiteral("skeleton_17"));
        skeleton_17->setGeometry(QRect(1090, 240, 15, 120));
        skeleton_17->setOrientation(Qt::Vertical);
        skeleton_13 = new QSlider(ModelHandleClass);
        skeleton_13->setObjectName(QStringLiteral("skeleton_13"));
        skeleton_13->setGeometry(QRect(970, 240, 15, 120));
        skeleton_13->setOrientation(Qt::Vertical);
        skeleton_12 = new QSlider(ModelHandleClass);
        skeleton_12->setObjectName(QStringLiteral("skeleton_12"));
        skeleton_12->setGeometry(QRect(940, 240, 15, 120));
        skeleton_12->setOrientation(Qt::Vertical);
        skeleton_11 = new QSlider(ModelHandleClass);
        skeleton_11->setObjectName(QStringLiteral("skeleton_11"));
        skeleton_11->setGeometry(QRect(910, 240, 15, 120));
        skeleton_11->setOrientation(Qt::Vertical);
        ReadButton = new QPushButton(ModelHandleClass);
        ReadButton->setObjectName(QStringLiteral("ReadButton"));
        ReadButton->setGeometry(QRect(640, 540, 70, 30));
        ReadButton->setFont(font);
        skeleton_box = new QCheckBox(ModelHandleClass);
        skeleton_box->setObjectName(QStringLiteral("skeleton_box"));
        skeleton_box->setGeometry(QRect(650, 500, 70, 20));
        mesh_box = new QCheckBox(ModelHandleClass);
        mesh_box->setObjectName(QStringLiteral("mesh_box"));
        mesh_box->setGeometry(QRect(750, 500, 70, 20));
        label_4 = new QLabel(ModelHandleClass);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(610, 220, 16, 16));
        QFont font2;
        font2.setPointSize(13);
        font2.setBold(true);
        font2.setWeight(75);
        label_4->setFont(font2);
        label_6 = new QLabel(ModelHandleClass);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(640, 220, 16, 16));
        label_6->setFont(font2);
        label_7 = new QLabel(ModelHandleClass);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(670, 220, 16, 16));
        label_7->setFont(font2);
        label_8 = new QLabel(ModelHandleClass);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(700, 220, 16, 16));
        label_8->setFont(font2);
        label_9 = new QLabel(ModelHandleClass);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(820, 220, 16, 16));
        label_9->setFont(font2);
        label_10 = new QLabel(ModelHandleClass);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(760, 220, 16, 16));
        label_10->setFont(font2);
        label_11 = new QLabel(ModelHandleClass);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(790, 220, 16, 16));
        label_11->setFont(font2);
        label_12 = new QLabel(ModelHandleClass);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(730, 220, 16, 16));
        label_12->setFont(font2);
        label_13 = new QLabel(ModelHandleClass);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(850, 220, 16, 16));
        label_13->setFont(font2);
        label_18 = new QLabel(ModelHandleClass);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(880, 220, 21, 20));
        label_18->setFont(font2);
        label_19 = new QLabel(ModelHandleClass);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(910, 220, 21, 20));
        label_19->setFont(font2);
        label_20 = new QLabel(ModelHandleClass);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(940, 220, 21, 20));
        label_20->setFont(font2);
        label_21 = new QLabel(ModelHandleClass);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(970, 220, 21, 20));
        label_21->setFont(font2);
        label_22 = new QLabel(ModelHandleClass);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(1000, 220, 21, 20));
        label_22->setFont(font2);
        label_23 = new QLabel(ModelHandleClass);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(1030, 220, 21, 20));
        label_23->setFont(font2);
        label_24 = new QLabel(ModelHandleClass);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(1060, 220, 21, 20));
        label_24->setFont(font2);
        label_25 = new QLabel(ModelHandleClass);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(1090, 220, 21, 20));
        label_25->setFont(font2);
        light = new QFrame(ModelHandleClass);
        light->setObjectName(QStringLiteral("light"));
        light->setGeometry(QRect(650, 575, 80, 20));
        light->setFrameShape(QFrame::StyledPanel);
        light->setFrameShadow(QFrame::Raised);
        light_1 = new QPushButton(light);
        light_1->setObjectName(QStringLiteral("light_1"));
        light_1->setGeometry(QRect(0, 0, 20, 20));
        light_1->setStyleSheet(QStringLiteral("background-color: rgb(255, 36, 16);"));
        light_2 = new QPushButton(light);
        light_2->setObjectName(QStringLiteral("light_2"));
        light_2->setGeometry(QRect(30, 0, 20, 20));
        light_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 255, 0);"));
        light_3 = new QPushButton(light);
        light_3->setObjectName(QStringLiteral("light_3"));
        light_3->setGeometry(QRect(60, 0, 20, 20));
        light_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 36, 16);"));
        label_26 = new QLabel(ModelHandleClass);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(1030, 370, 21, 20));
        label_26->setFont(font2);
        label_14 = new QLabel(ModelHandleClass);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(640, 370, 16, 16));
        label_14->setFont(font2);
        label_27 = new QLabel(ModelHandleClass);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(940, 370, 21, 20));
        label_27->setFont(font2);
        label_28 = new QLabel(ModelHandleClass);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(1060, 370, 21, 20));
        label_28->setFont(font2);
        label_15 = new QLabel(ModelHandleClass);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(790, 370, 16, 16));
        label_15->setFont(font2);
        label_16 = new QLabel(ModelHandleClass);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(850, 370, 16, 16));
        label_16->setFont(font2);
        label_17 = new QLabel(ModelHandleClass);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(670, 370, 16, 16));
        label_17->setFont(font2);
        label_29 = new QLabel(ModelHandleClass);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(880, 370, 21, 20));
        label_29->setFont(font2);
        label_30 = new QLabel(ModelHandleClass);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(820, 370, 16, 16));
        label_30->setFont(font2);
        label_31 = new QLabel(ModelHandleClass);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setGeometry(QRect(1090, 370, 21, 20));
        label_31->setFont(font2);
        label_32 = new QLabel(ModelHandleClass);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setGeometry(QRect(700, 370, 16, 16));
        label_32->setFont(font2);
        label_33 = new QLabel(ModelHandleClass);
        label_33->setObjectName(QStringLiteral("label_33"));
        label_33->setGeometry(QRect(910, 370, 21, 20));
        label_33->setFont(font2);
        label_34 = new QLabel(ModelHandleClass);
        label_34->setObjectName(QStringLiteral("label_34"));
        label_34->setGeometry(QRect(1000, 370, 21, 20));
        label_34->setFont(font2);
        label_35 = new QLabel(ModelHandleClass);
        label_35->setObjectName(QStringLiteral("label_35"));
        label_35->setGeometry(QRect(730, 370, 16, 16));
        label_35->setFont(font2);
        label_36 = new QLabel(ModelHandleClass);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setGeometry(QRect(610, 370, 16, 16));
        label_36->setFont(font2);
        cst_1 = new QSlider(ModelHandleClass);
        cst_1->setObjectName(QStringLiteral("cst_1"));
        cst_1->setGeometry(QRect(610, 390, 16, 91));
        cst_1->setOrientation(Qt::Vertical);
        label_37 = new QLabel(ModelHandleClass);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setGeometry(QRect(760, 370, 16, 16));
        label_37->setFont(font2);
        label_38 = new QLabel(ModelHandleClass);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setGeometry(QRect(970, 370, 21, 20));
        label_38->setFont(font2);
        cst_2 = new QSlider(ModelHandleClass);
        cst_2->setObjectName(QStringLiteral("cst_2"));
        cst_2->setGeometry(QRect(640, 390, 16, 91));
        cst_2->setOrientation(Qt::Vertical);
        cst_3 = new QSlider(ModelHandleClass);
        cst_3->setObjectName(QStringLiteral("cst_3"));
        cst_3->setGeometry(QRect(670, 390, 16, 91));
        cst_3->setOrientation(Qt::Vertical);
        cst_4 = new QSlider(ModelHandleClass);
        cst_4->setObjectName(QStringLiteral("cst_4"));
        cst_4->setGeometry(QRect(700, 390, 16, 91));
        cst_4->setOrientation(Qt::Vertical);
        cst_5 = new QSlider(ModelHandleClass);
        cst_5->setObjectName(QStringLiteral("cst_5"));
        cst_5->setGeometry(QRect(730, 390, 16, 91));
        cst_5->setOrientation(Qt::Vertical);
        cst_6 = new QSlider(ModelHandleClass);
        cst_6->setObjectName(QStringLiteral("cst_6"));
        cst_6->setGeometry(QRect(760, 390, 16, 91));
        cst_6->setOrientation(Qt::Vertical);
        cst_7 = new QSlider(ModelHandleClass);
        cst_7->setObjectName(QStringLiteral("cst_7"));
        cst_7->setGeometry(QRect(790, 390, 16, 91));
        cst_7->setOrientation(Qt::Vertical);
        cst_8 = new QSlider(ModelHandleClass);
        cst_8->setObjectName(QStringLiteral("cst_8"));
        cst_8->setGeometry(QRect(820, 390, 16, 91));
        cst_8->setOrientation(Qt::Vertical);
        cst_9 = new QSlider(ModelHandleClass);
        cst_9->setObjectName(QStringLiteral("cst_9"));
        cst_9->setGeometry(QRect(850, 390, 16, 91));
        cst_9->setOrientation(Qt::Vertical);
        cst_10 = new QSlider(ModelHandleClass);
        cst_10->setObjectName(QStringLiteral("cst_10"));
        cst_10->setGeometry(QRect(880, 390, 16, 91));
        cst_10->setOrientation(Qt::Vertical);
        cst_11 = new QSlider(ModelHandleClass);
        cst_11->setObjectName(QStringLiteral("cst_11"));
        cst_11->setGeometry(QRect(910, 390, 16, 91));
        cst_11->setOrientation(Qt::Vertical);
        cst_12 = new QSlider(ModelHandleClass);
        cst_12->setObjectName(QStringLiteral("cst_12"));
        cst_12->setGeometry(QRect(940, 390, 16, 91));
        cst_12->setOrientation(Qt::Vertical);
        cst_13 = new QSlider(ModelHandleClass);
        cst_13->setObjectName(QStringLiteral("cst_13"));
        cst_13->setGeometry(QRect(970, 390, 16, 91));
        cst_13->setOrientation(Qt::Vertical);
        cst_14 = new QSlider(ModelHandleClass);
        cst_14->setObjectName(QStringLiteral("cst_14"));
        cst_14->setGeometry(QRect(1000, 390, 16, 91));
        cst_14->setOrientation(Qt::Vertical);
        cst_15 = new QSlider(ModelHandleClass);
        cst_15->setObjectName(QStringLiteral("cst_15"));
        cst_15->setGeometry(QRect(1030, 390, 16, 91));
        cst_15->setOrientation(Qt::Vertical);
        cst_16 = new QSlider(ModelHandleClass);
        cst_16->setObjectName(QStringLiteral("cst_16"));
        cst_16->setGeometry(QRect(1060, 390, 16, 91));
        cst_16->setOrientation(Qt::Vertical);
        cst_17 = new QSlider(ModelHandleClass);
        cst_17->setObjectName(QStringLiteral("cst_17"));
        cst_17->setGeometry(QRect(1090, 390, 16, 91));
        cst_17->setOrientation(Qt::Vertical);
        Obj_out = new QPushButton(ModelHandleClass);
        Obj_out->setObjectName(QStringLiteral("Obj_out"));
        Obj_out->setGeometry(QRect(800, 540, 91, 30));
        Obj_out->setFont(font);
        energyButton = new QPushButton(ModelHandleClass);
        energyButton->setObjectName(QStringLiteral("energyButton"));
        energyButton->setGeometry(QRect(960, 80, 75, 71));
        QFont font3;
        font3.setPointSize(25);
        font3.setBold(true);
        font3.setWeight(75);
        energyButton->setFont(font3);
        smoothBox = new QCheckBox(ModelHandleClass);
        smoothBox->setObjectName(QStringLiteral("smoothBox"));
        smoothBox->setGeometry(QRect(930, 170, 71, 16));
        stressBox = new QCheckBox(ModelHandleClass);
        stressBox->setObjectName(QStringLiteral("stressBox"));
        stressBox->setGeometry(QRect(930, 190, 71, 16));
        coord_box = new QCheckBox(ModelHandleClass);
        coord_box->setObjectName(QStringLiteral("coord_box"));
        coord_box->setGeometry(QRect(830, 500, 91, 16));
        groupBox_2 = new QGroupBox(ModelHandleClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(1130, 130, 251, 341));
        groupBox_2->setFont(font);
        SkeletonChooseBox = new QComboBox(groupBox_2);
        SkeletonChooseBox->setObjectName(QStringLiteral("SkeletonChooseBox"));
        SkeletonChooseBox->setGeometry(QRect(80, 40, 141, 22));
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setWeight(50);
        SkeletonChooseBox->setFont(font4);
        label_39 = new QLabel(groupBox_2);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setGeometry(QRect(10, 41, 54, 21));
        QFont font5;
        font5.setPointSize(11);
        font5.setBold(false);
        font5.setWeight(50);
        label_39->setFont(font5);
        label_40 = new QLabel(groupBox_2);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setGeometry(QRect(10, 100, 71, 21));
        label_40->setFont(font4);
        label_41 = new QLabel(groupBox_2);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setGeometry(QRect(10, 150, 71, 21));
        label_41->setFont(font4);
        SkeletonChangeButton = new QPushButton(groupBox_2);
        SkeletonChangeButton->setObjectName(QStringLiteral("SkeletonChangeButton"));
        SkeletonChangeButton->setGeometry(QRect(160, 300, 75, 23));
        SkeletonChangeButton->setFont(font1);
        curSkeletonAlpha = new QSlider(groupBox_2);
        curSkeletonAlpha->setObjectName(QStringLiteral("curSkeletonAlpha"));
        curSkeletonAlpha->setGeometry(QRect(80, 100, 160, 22));
        curSkeletonAlpha->setOrientation(Qt::Horizontal);
        curSkeletonBeta = new QSlider(groupBox_2);
        curSkeletonBeta->setObjectName(QStringLiteral("curSkeletonBeta"));
        curSkeletonBeta->setGeometry(QRect(80, 150, 160, 22));
        curSkeletonBeta->setOrientation(Qt::Horizontal);
        voxel_box = new QCheckBox(ModelHandleClass);
        voxel_box->setObjectName(QStringLiteral("voxel_box"));
        voxel_box->setGeometry(QRect(930, 500, 71, 16));

        retranslateUi(ModelHandleClass);

        QMetaObject::connectSlotsByName(ModelHandleClass);
    } // setupUi

    void retranslateUi(QWidget *ModelHandleClass)
    {
        ModelHandleClass->setWindowTitle(QApplication::translate("ModelHandleClass", "ModelHandle", 0));
        ClearButton->setText(QApplication::translate("ModelHandleClass", "Clear", 0));
        groupBox->setTitle(QApplication::translate("ModelHandleClass", "\346\250\241\345\236\213\350\247\206\350\247\222\350\260\203\346\225\264", 0));
        label_5->setText(QApplication::translate("ModelHandleClass", "Scale", 0));
        label->setText(QApplication::translate("ModelHandleClass", "X", 0));
        label_2->setText(QApplication::translate("ModelHandleClass", "Y", 0));
        label_3->setText(QApplication::translate("ModelHandleClass", "Z", 0));
        ReadButton->setText(QApplication::translate("ModelHandleClass", "Read", 0));
        skeleton_box->setText(QApplication::translate("ModelHandleClass", "skeleton", 0));
        mesh_box->setText(QApplication::translate("ModelHandleClass", "mesh", 0));
        label_4->setText(QApplication::translate("ModelHandleClass", "1", 0));
        label_6->setText(QApplication::translate("ModelHandleClass", "2", 0));
        label_7->setText(QApplication::translate("ModelHandleClass", "3", 0));
        label_8->setText(QApplication::translate("ModelHandleClass", "4", 0));
        label_9->setText(QApplication::translate("ModelHandleClass", "8", 0));
        label_10->setText(QApplication::translate("ModelHandleClass", "6", 0));
        label_11->setText(QApplication::translate("ModelHandleClass", "7", 0));
        label_12->setText(QApplication::translate("ModelHandleClass", "5", 0));
        label_13->setText(QApplication::translate("ModelHandleClass", "9", 0));
        label_18->setText(QApplication::translate("ModelHandleClass", "10", 0));
        label_19->setText(QApplication::translate("ModelHandleClass", "11", 0));
        label_20->setText(QApplication::translate("ModelHandleClass", "12", 0));
        label_21->setText(QApplication::translate("ModelHandleClass", "13", 0));
        label_22->setText(QApplication::translate("ModelHandleClass", "14", 0));
        label_23->setText(QApplication::translate("ModelHandleClass", "15", 0));
        label_24->setText(QApplication::translate("ModelHandleClass", "16", 0));
        label_25->setText(QApplication::translate("ModelHandleClass", "17", 0));
        light_1->setText(QString());
        light_2->setText(QString());
        light_3->setText(QString());
        label_26->setText(QApplication::translate("ModelHandleClass", "15", 0));
        label_14->setText(QApplication::translate("ModelHandleClass", "2", 0));
        label_27->setText(QApplication::translate("ModelHandleClass", "12", 0));
        label_28->setText(QApplication::translate("ModelHandleClass", "16", 0));
        label_15->setText(QApplication::translate("ModelHandleClass", "7", 0));
        label_16->setText(QApplication::translate("ModelHandleClass", "9", 0));
        label_17->setText(QApplication::translate("ModelHandleClass", "3", 0));
        label_29->setText(QApplication::translate("ModelHandleClass", "10", 0));
        label_30->setText(QApplication::translate("ModelHandleClass", "8", 0));
        label_31->setText(QApplication::translate("ModelHandleClass", "17", 0));
        label_32->setText(QApplication::translate("ModelHandleClass", "4", 0));
        label_33->setText(QApplication::translate("ModelHandleClass", "11", 0));
        label_34->setText(QApplication::translate("ModelHandleClass", "14", 0));
        label_35->setText(QApplication::translate("ModelHandleClass", "5", 0));
        label_36->setText(QApplication::translate("ModelHandleClass", "1", 0));
        label_37->setText(QApplication::translate("ModelHandleClass", "6", 0));
        label_38->setText(QApplication::translate("ModelHandleClass", "13", 0));
        Obj_out->setText(QApplication::translate("ModelHandleClass", "OBJ-OUT", 0));
        energyButton->setText(QApplication::translate("ModelHandleClass", "E", 0));
        smoothBox->setText(QApplication::translate("ModelHandleClass", "Smooth", 0));
        stressBox->setText(QApplication::translate("ModelHandleClass", "Stress", 0));
        coord_box->setText(QApplication::translate("ModelHandleClass", "coordinary", 0));
        groupBox_2->setTitle(QApplication::translate("ModelHandleClass", "BVH \350\260\203\350\212\202", 0));
        label_39->setText(QApplication::translate("ModelHandleClass", "\351\252\250\351\252\274:", 0));
        label_40->setText(QApplication::translate("ModelHandleClass", "Alpha:", 0));
        label_41->setText(QApplication::translate("ModelHandleClass", "Beta:", 0));
        SkeletonChangeButton->setText(QApplication::translate("ModelHandleClass", "Change", 0));
        voxel_box->setText(QApplication::translate("ModelHandleClass", "voxel", 0));
    } // retranslateUi

};

namespace Ui {
    class ModelHandleClass: public Ui_ModelHandleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELHANDLE_H
