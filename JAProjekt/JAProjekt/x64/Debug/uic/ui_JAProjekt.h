/********************************************************************************
** Form generated from reading UI file 'JAProjekt.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JAPROJEKT_H
#define UI_JAPROJEKT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JAProjektClass
{
public:
    QPushButton *loadFileButton;
    QLabel *sourceLocationLabel;
    QPushButton *cppAlgButton;
    QPushButton *asmAlgButton;
    QLabel *cppAlglabel;
    QLabel *asmAlglabel;
    QPushButton *saveFileButton;
    QSlider *coreHorizontalSlider;
    QLabel *coreLabel;

    void setupUi(QWidget *JAProjektClass)
    {
        if (JAProjektClass->objectName().isEmpty())
            JAProjektClass->setObjectName(QString::fromUtf8("JAProjektClass"));
        JAProjektClass->resize(600, 400);
        loadFileButton = new QPushButton(JAProjektClass);
        loadFileButton->setObjectName(QString::fromUtf8("loadFileButton"));
        loadFileButton->setGeometry(QRect(470, 10, 101, 21));
        sourceLocationLabel = new QLabel(JAProjektClass);
        sourceLocationLabel->setObjectName(QString::fromUtf8("sourceLocationLabel"));
        sourceLocationLabel->setGeometry(QRect(20, 10, 431, 16));
        cppAlgButton = new QPushButton(JAProjektClass);
        cppAlgButton->setObjectName(QString::fromUtf8("cppAlgButton"));
        cppAlgButton->setEnabled(false);
        cppAlgButton->setGeometry(QRect(20, 80, 261, 61));
        asmAlgButton = new QPushButton(JAProjektClass);
        asmAlgButton->setObjectName(QString::fromUtf8("asmAlgButton"));
        asmAlgButton->setEnabled(false);
        asmAlgButton->setGeometry(QRect(310, 80, 261, 61));
        cppAlglabel = new QLabel(JAProjektClass);
        cppAlglabel->setObjectName(QString::fromUtf8("cppAlglabel"));
        cppAlglabel->setGeometry(QRect(30, 150, 251, 51));
        asmAlglabel = new QLabel(JAProjektClass);
        asmAlglabel->setObjectName(QString::fromUtf8("asmAlglabel"));
        asmAlglabel->setGeometry(QRect(320, 150, 251, 51));
        saveFileButton = new QPushButton(JAProjektClass);
        saveFileButton->setObjectName(QString::fromUtf8("saveFileButton"));
        saveFileButton->setGeometry(QRect(470, 360, 101, 23));
        coreHorizontalSlider = new QSlider(JAProjektClass);
        coreHorizontalSlider->setObjectName(QString::fromUtf8("coreHorizontalSlider"));
        coreHorizontalSlider->setGeometry(QRect(20, 40, 511, 22));
        coreHorizontalSlider->setMinimum(1);
        coreHorizontalSlider->setMaximum(64);
        coreHorizontalSlider->setSliderPosition(1);
        coreHorizontalSlider->setOrientation(Qt::Horizontal);
        coreHorizontalSlider->setTickPosition(QSlider::TicksAbove);
        coreHorizontalSlider->setTickInterval(4);
        coreLabel = new QLabel(JAProjektClass);
        coreLabel->setObjectName(QString::fromUtf8("coreLabel"));
        coreLabel->setGeometry(QRect(540, 40, 21, 16));

        retranslateUi(JAProjektClass);
        QObject::connect(coreHorizontalSlider, SIGNAL(sliderMoved(int)), coreLabel, SLOT(setNum(int)));

        QMetaObject::connectSlotsByName(JAProjektClass);
    } // setupUi

    void retranslateUi(QWidget *JAProjektClass)
    {
        JAProjektClass->setWindowTitle(QCoreApplication::translate("JAProjektClass", "JAProjekt", nullptr));
        loadFileButton->setText(QCoreApplication::translate("JAProjektClass", "Load file", nullptr));
        sourceLocationLabel->setText(QCoreApplication::translate("JAProjektClass", "TextLabel", nullptr));
        cppAlgButton->setText(QCoreApplication::translate("JAProjektClass", "C++ algorithm", nullptr));
        asmAlgButton->setText(QCoreApplication::translate("JAProjektClass", "Assembly algorithm", nullptr));
        cppAlglabel->setText(QCoreApplication::translate("JAProjektClass", "TextLabel", nullptr));
        asmAlglabel->setText(QCoreApplication::translate("JAProjektClass", "TextLabel", nullptr));
        saveFileButton->setText(QCoreApplication::translate("JAProjektClass", "Save file", nullptr));
        coreLabel->setText(QCoreApplication::translate("JAProjektClass", "1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JAProjektClass: public Ui_JAProjektClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JAPROJEKT_H
