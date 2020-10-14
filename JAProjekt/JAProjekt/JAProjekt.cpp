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
        bmpEditor.setSourceFilename(fileName.toStdString());
        ui.saveFileButton->setEnabled(true);
	}
    
}

void JAProjekt::on_cppAlgButton_clicked()
{
    std::string toBeParsed = bmpEditor.runAlgorithm(AlgorithmType::cppAlgorithm, ui.coreHorizontalSlider->value());
    ui.cppAlglabel->setText(QString::fromStdString(toBeParsed));
}

void JAProjekt::on_asmAlgButton_clicked()
{
    std::string toBeParsed = bmpEditor.runAlgorithm(AlgorithmType::asmAlgorithm, ui.coreHorizontalSlider->value());
    ui.cppAlglabel->setText(QString::fromStdString(toBeParsed));
}

void JAProjekt::on_saveFileButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "/home/jana", tr("Image Files (*.bmp);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        ui.destinationSourceLabel->setText(fileName);
        bmpEditor.setDestinationFilename(fileName.toStdString());
        ui.asmAlgButton->setEnabled(true);
        ui.cppAlgButton->setEnabled(true);
    }
}
