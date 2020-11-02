#include "JAProjekt.h"

#include <QFileDialog>
#include <thread>
#include <QtCharts>

using namespace QtCharts;
JAProjekt::JAProjekt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    unsigned int n = std::thread::hardware_concurrency();
    ui.coreHorizontalSlider->setValue(n);
    ui.coreLabel->setText(QString::number(n));
	
    QChart* chartInput = new QChart();
    QBarSet* setRed = new QBarSet("Red");
    QBarSet* setGreen = new QBarSet("Green");
    QBarSet* setBlue = new QBarSet("Blue");
    QBarSeries* series = new QBarSeries();
    for (int i = 0; i < 128; i++) {
        *setRed << i % 75;
        *setGreen << (i % 35) + 31;
        *setBlue << (i + 31) % 97;
    }
    setRed->setColor(QColor(255, 0, 0));
    setGreen->setColor(QColor(0, 255, 0));
    setBlue->setColor(QColor(0, 0, 255));
    series->append(setRed);
    series->append(setGreen);
    series->append(setBlue);

    chartInput->addSeries(series);
    chartInput->setTitle("Histogram obrazu wejsciowego");
    chartInput->setAnimationOptions(QChart::SeriesAnimations);

    chartInput->legend()->setVisible(true);
    chartInput->legend()->setAlignment(Qt::AlignBottom);
    series->setBarWidth(1);
    ui.graphicsView->setChart(chartInput);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->show();
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
        ui.inputHistogramButton->setEnabled(true);
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
    ui.asmAlglabel->setText(QString::fromStdString(toBeParsed));
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
        ui.outputHistogramButton->setEnabled(true);
    }
}

void JAProjekt::on_inputHistogramButton_clicked()
{
}

void JAProjekt::on_outputHistogramButton_clicked()
{
}
