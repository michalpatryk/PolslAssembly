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
	if(!fileName.isEmpty())
	{
        ui.sourceLocationLabel->setText(fileName);
        bmpEditor.setFilename(fileName.toStdString());
        ui.asmAlgButton->setEnabled(true);
        ui.cppAlgButton->setEnabled(true);
        ui.saveFileButton->setEnabled(true);
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

void JAProjekt::on_saveFileButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "/home/jana", tr("Image Files (*.bmp);;All Files (*)"));
    ui.sourceLocationLabel->setText(fileName);
}
