#include "JAProjekt.h"

#include <QFileDialog>
#include <thread>

JAProjekt::JAProjekt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    unsigned int n = std::thread::hardware_concurrency();
    ui.coreHorizontalSlider->setValue(n);
    ui.coreLabel->setText(QString::number(n));
}

void JAProjekt::on_loadFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("Image Files (*.bmp)"));
    ui.sourceLocationLabel->setText(fileName);
	if(!fileName.isEmpty())
	{
        bmpEditor.setFilename(fileName.toStdString());
        ui.asmAlgButton->setEnabled(true);
        ui.cppAlgButton->setEnabled(true);
	}
    
}

void JAProjekt::on_cppAlgButton_clicked()
{
    bmpEditor.runAlgorithm(AlgorithmType::cppAlgorithm, ui.coreHorizontalSlider->value());
}

void JAProjekt::on_asmAlgButton_clicked()
{
    bmpEditor.runAlgorithm(AlgorithmType::asmAlgorithm, ui.coreHorizontalSlider->value());
}
