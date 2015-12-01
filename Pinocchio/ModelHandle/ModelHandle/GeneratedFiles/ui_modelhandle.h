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
    QPushButton *ReadButton;
    QCheckBox *skeleton_box;
    QCheckBox *mesh_box;
    QFrame *light;
    QPushButton *light_1;
    QPushButton *light_2;
    QPushButton *light_3;
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
    QLabel *label_42;
    QSlider *curSkeletonScale;
    QCheckBox *voxel_box;
    QPushButton *ScopeButton;

    void setupUi(QWidget *ModelHandleClass)
    {
        if (ModelHandleClass->objectName().isEmpty())
            ModelHandleClass->setObjectName(QStringLiteral("ModelHandleClass"));
        ModelHandleClass->resize(1200, 600);
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
        Obj_out = new QPushButton(ModelHandleClass);
        Obj_out->setObjectName(QStringLiteral("Obj_out"));
        Obj_out->setGeometry(QRect(800, 540, 91, 30));
        Obj_out->setFont(font);
        energyButton = new QPushButton(ModelHandleClass);
        energyButton->setObjectName(QStringLiteral("energyButton"));
        energyButton->setGeometry(QRect(680, 260, 75, 71));
        QFont font2;
        font2.setPointSize(25);
        font2.setBold(true);
        font2.setWeight(75);
        energyButton->setFont(font2);
        smoothBox = new QCheckBox(ModelHandleClass);
        smoothBox->setObjectName(QStringLiteral("smoothBox"));
        smoothBox->setGeometry(QRect(650, 350, 71, 16));
        stressBox = new QCheckBox(ModelHandleClass);
        stressBox->setObjectName(QStringLiteral("stressBox"));
        stressBox->setGeometry(QRect(650, 370, 71, 16));
        coord_box = new QCheckBox(ModelHandleClass);
        coord_box->setObjectName(QStringLiteral("coord_box"));
        coord_box->setGeometry(QRect(830, 500, 91, 16));
        groupBox_2 = new QGroupBox(ModelHandleClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(910, 90, 251, 341));
        groupBox_2->setFont(font);
        SkeletonChooseBox = new QComboBox(groupBox_2);
        SkeletonChooseBox->setObjectName(QStringLiteral("SkeletonChooseBox"));
        SkeletonChooseBox->setGeometry(QRect(80, 40, 141, 22));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        SkeletonChooseBox->setFont(font3);
        label_39 = new QLabel(groupBox_2);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setGeometry(QRect(10, 41, 54, 21));
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(false);
        font4.setWeight(50);
        label_39->setFont(font4);
        label_40 = new QLabel(groupBox_2);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setGeometry(QRect(10, 100, 71, 21));
        label_40->setFont(font3);
        label_41 = new QLabel(groupBox_2);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setGeometry(QRect(10, 150, 71, 21));
        label_41->setFont(font3);
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
        label_42 = new QLabel(groupBox_2);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setGeometry(QRect(10, 200, 71, 21));
        label_42->setFont(font3);
        curSkeletonScale = new QSlider(groupBox_2);
        curSkeletonScale->setObjectName(QStringLiteral("curSkeletonScale"));
        curSkeletonScale->setGeometry(QRect(80, 200, 160, 22));
        curSkeletonScale->setOrientation(Qt::Horizontal);
        voxel_box = new QCheckBox(ModelHandleClass);
        voxel_box->setObjectName(QStringLiteral("voxel_box"));
        voxel_box->setGeometry(QRect(930, 500, 71, 16));
        ScopeButton = new QPushButton(ModelHandleClass);
        ScopeButton->setObjectName(QStringLiteral("ScopeButton"));
        ScopeButton->setGeometry(QRect(640, 460, 75, 23));

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
        light_1->setText(QString());
        light_2->setText(QString());
        light_3->setText(QString());
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
        label_42->setText(QApplication::translate("ModelHandleClass", "Scale:", 0));
        voxel_box->setText(QApplication::translate("ModelHandleClass", "voxel", 0));
        ScopeButton->setText(QApplication::translate("ModelHandleClass", "CountScope", 0));
    } // retranslateUi

};

namespace Ui {
    class ModelHandleClass: public Ui_ModelHandleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELHANDLE_H
