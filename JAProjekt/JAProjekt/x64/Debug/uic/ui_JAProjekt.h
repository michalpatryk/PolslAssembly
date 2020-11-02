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
#include "QtCharts"

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
    QLabel *aboutLabel;
    QLabel *destinationSourceLabel;
    QChartView *graphicsView;
    QSlider *tresholdHorizontalSlider;
    QLabel *tresholdValueLabel;
    QLabel *tresholdLabel;
    QLabel *coreCountLabel;
    QPushButton *inputHistogramButton;
    QPushButton *outputHistogramButton;

    void setupUi(QWidget *JAProjektClass)
    {
        if (JAProjektClass->objectName().isEmpty())
            JAProjektClass->setObjectName(QString::fromUtf8("JAProjektClass"));
        JAProjektClass->resize(600, 571);
        loadFileButton = new QPushButton(JAProjektClass);
        loadFileButton->setObjectName(QString::fromUtf8("loadFileButton"));
        loadFileButton->setGeometry(QRect(470, 40, 101, 21));
        sourceLocationLabel = new QLabel(JAProjektClass);
        sourceLocationLabel->setObjectName(QString::fromUtf8("sourceLocationLabel"));
        sourceLocationLabel->setGeometry(QRect(20, 40, 431, 16));
        cppAlgButton = new QPushButton(JAProjektClass);
        cppAlgButton->setObjectName(QString::fromUtf8("cppAlgButton"));
        cppAlgButton->setEnabled(false);
        cppAlgButton->setGeometry(QRect(20, 200, 261, 61));
        asmAlgButton = new QPushButton(JAProjektClass);
        asmAlgButton->setObjectName(QString::fromUtf8("asmAlgButton"));
        asmAlgButton->setEnabled(false);
        asmAlgButton->setGeometry(QRect(310, 200, 261, 61));
        cppAlglabel = new QLabel(JAProjektClass);
        cppAlglabel->setObjectName(QString::fromUtf8("cppAlglabel"));
        cppAlglabel->setGeometry(QRect(30, 270, 251, 51));
        asmAlglabel = new QLabel(JAProjektClass);
        asmAlglabel->setObjectName(QString::fromUtf8("asmAlglabel"));
        asmAlglabel->setGeometry(QRect(320, 270, 251, 51));
        saveFileButton = new QPushButton(JAProjektClass);
        saveFileButton->setObjectName(QString::fromUtf8("saveFileButton"));
        saveFileButton->setEnabled(false);
        saveFileButton->setGeometry(QRect(470, 70, 101, 23));
        coreHorizontalSlider = new QSlider(JAProjektClass);
        coreHorizontalSlider->setObjectName(QString::fromUtf8("coreHorizontalSlider"));
        coreHorizontalSlider->setGeometry(QRect(20, 120, 511, 22));
        coreHorizontalSlider->setMinimum(1);
        coreHorizontalSlider->setMaximum(64);
        coreHorizontalSlider->setSliderPosition(1);
        coreHorizontalSlider->setOrientation(Qt::Horizontal);
        coreHorizontalSlider->setTickPosition(QSlider::TicksAbove);
        coreHorizontalSlider->setTickInterval(4);
        coreLabel = new QLabel(JAProjektClass);
        coreLabel->setObjectName(QString::fromUtf8("coreLabel"));
        coreLabel->setGeometry(QRect(540, 120, 21, 16));
        aboutLabel = new QLabel(JAProjektClass);
        aboutLabel->setObjectName(QString::fromUtf8("aboutLabel"));
        aboutLabel->setGeometry(QRect(20, 10, 551, 21));
        destinationSourceLabel = new QLabel(JAProjektClass);
        destinationSourceLabel->setObjectName(QString::fromUtf8("destinationSourceLabel"));
        destinationSourceLabel->setGeometry(QRect(20, 70, 431, 16));
        graphicsView = new QChartView(JAProjektClass);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(20, 350, 551, 192));
        tresholdHorizontalSlider = new QSlider(JAProjektClass);
        tresholdHorizontalSlider->setObjectName(QString::fromUtf8("tresholdHorizontalSlider"));
        tresholdHorizontalSlider->setGeometry(QRect(20, 170, 511, 22));
        tresholdHorizontalSlider->setMaximum(100);
        tresholdHorizontalSlider->setValue(30);
        tresholdHorizontalSlider->setOrientation(Qt::Horizontal);
        tresholdHorizontalSlider->setTickPosition(QSlider::TicksAbove);
        tresholdHorizontalSlider->setTickInterval(5);
        tresholdValueLabel = new QLabel(JAProjektClass);
        tresholdValueLabel->setObjectName(QString::fromUtf8("tresholdValueLabel"));
        tresholdValueLabel->setGeometry(QRect(540, 170, 16, 16));
        tresholdLabel = new QLabel(JAProjektClass);
        tresholdLabel->setObjectName(QString::fromUtf8("tresholdLabel"));
        tresholdLabel->setGeometry(QRect(20, 150, 101, 16));
        coreCountLabel = new QLabel(JAProjektClass);
        coreCountLabel->setObjectName(QString::fromUtf8("coreCountLabel"));
        coreCountLabel->setGeometry(QRect(20, 100, 47, 13));
        inputHistogramButton = new QPushButton(JAProjektClass);
        inputHistogramButton->setObjectName(QString::fromUtf8("inputHistogramButton"));
        inputHistogramButton->setEnabled(false);
        inputHistogramButton->setGeometry(QRect(20, 320, 261, 23));
        outputHistogramButton = new QPushButton(JAProjektClass);
        outputHistogramButton->setObjectName(QString::fromUtf8("outputHistogramButton"));
        outputHistogramButton->setEnabled(false);
        outputHistogramButton->setGeometry(QRect(310, 320, 261, 23));

        retranslateUi(JAProjektClass);
        QObject::connect(coreHorizontalSlider, SIGNAL(valueChanged(int)), coreLabel, SLOT(setNum(int)));
        QObject::connect(tresholdHorizontalSlider, SIGNAL(valueChanged(int)), tresholdValueLabel, SLOT(setNum(int)));

        QMetaObject::connectSlotsByName(JAProjektClass);
    } // setupUi

    void retranslateUi(QWidget *JAProjektClass)
    {
        JAProjektClass->setWindowTitle(QCoreApplication::translate("JAProjektClass", "JAProjekt", nullptr));
        loadFileButton->setText(QCoreApplication::translate("JAProjektClass", "Load file", nullptr));
        sourceLocationLabel->setText(QCoreApplication::translate("JAProjektClass", "Select source file", nullptr));
        cppAlgButton->setText(QCoreApplication::translate("JAProjektClass", "C++ algorithm", nullptr));
        asmAlgButton->setText(QCoreApplication::translate("JAProjektClass", "Assembly algorithm", nullptr));
        cppAlglabel->setText(QCoreApplication::translate("JAProjektClass", "TextLabel", nullptr));
        asmAlglabel->setText(QCoreApplication::translate("JAProjektClass", "TextLabel", nullptr));
        saveFileButton->setText(QCoreApplication::translate("JAProjektClass", "Save file", nullptr));
        coreLabel->setText(QCoreApplication::translate("JAProjektClass", "1", nullptr));
        aboutLabel->setText(QCoreApplication::translate("JAProjektClass", "Binaryzacja obrazu BMP, Micha\305\202 Urba\305\204czyk 2020", nullptr));
        destinationSourceLabel->setText(QCoreApplication::translate("JAProjektClass", "Select destination file", nullptr));
        tresholdValueLabel->setText(QCoreApplication::translate("JAProjektClass", "30", nullptr));
        tresholdLabel->setText(QCoreApplication::translate("JAProjektClass", "Treshold:", nullptr));
        coreCountLabel->setText(QCoreApplication::translate("JAProjektClass", "Cores:", nullptr));
        inputHistogramButton->setText(QCoreApplication::translate("JAProjektClass", "Show input histogram", nullptr));
        outputHistogramButton->setText(QCoreApplication::translate("JAProjektClass", "Show output histogram", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JAProjektClass: public Ui_JAProjektClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JAPROJEKT_H
