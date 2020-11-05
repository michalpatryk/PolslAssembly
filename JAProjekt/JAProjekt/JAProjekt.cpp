#include "JAProjekt.h"
#include "Benchmark.h"
#include <QFileDialog>
#include <thread>
#include <QtCharts>
#include <fstream>
using namespace QtCharts;
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
        ui.inputHistogramButton->setEnabled(false);
        ui.outputHistogramButton->setEnabled(false);
	}
}

void JAProjekt::on_cppAlgButton_clicked()
{
    bmpEditor.setTreshold(ui.tresholdHorizontalSlider->value()/100.0);
    std::string toBeParsed = bmpEditor.runAlgorithm(AlgorithmType::cppAlgorithm, ui.coreHorizontalSlider->value());
    ui.cppAlglabel->setText(QString::fromStdString(toBeParsed));
    ui.inputHistogramButton->setEnabled(true);
    ui.outputHistogramButton->setEnabled(true);
}

void JAProjekt::on_asmAlgButton_clicked()
{
    bmpEditor.setTreshold(ui.tresholdHorizontalSlider->value()/100.0);
    std::string toBeParsed = bmpEditor.runAlgorithm(AlgorithmType::asmAlgorithm, ui.coreHorizontalSlider->value());
    ui.asmAlglabel->setText(QString::fromStdString(toBeParsed));
    ui.inputHistogramButton->setEnabled(true);
    ui.outputHistogramButton->setEnabled(true);
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
        ui.benchmarkButton->setEnabled(true);
    }
}

void JAProjekt::on_inputHistogramButton_clicked()
{
	if(chartInput != nullptr)
	{
        chartInput->removeSeries(series);
        series->clear();
	}
    
    chartInput = new QChart();
    QBarSet* setRed = new QBarSet("Red");
    QBarSet* setGreen = new QBarSet("Green");
    QBarSet* setBlue = new QBarSet("Blue");
    series = new QBarSeries();
    
    for (int i = 0; i < 256; i++) {
        *setRed << bmpEditor.getPreHistogramR()[i];
        *setGreen << bmpEditor.getPreHistogramG()[i];
        *setBlue << bmpEditor.getPreHistogramB()[i];
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
    series->setBarWidth(2);

    ui.graphicsView->setChart(chartInput);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->show();

}

void JAProjekt::on_outputHistogramButton_clicked()
{
    if (chartInput != nullptr)
    {
        chartInput->removeSeries(series);
        series->clear();
    }

    chartInput = new QChart();
    QBarSet* setRed = new QBarSet("Red");
    QBarSet* setGreen = new QBarSet("Green");
    QBarSet* setBlue = new QBarSet("Blue");
    series = new QBarSeries();
    for (int i = 0; i < 256; i++) {
        *setRed << bmpEditor.getPostHistogramR()[i];
        *setGreen << bmpEditor.getPostHistogramG()[i];
        *setBlue << bmpEditor.getPostHistogramB()[i];
    }
    setRed->setColor(QColor(255, 0, 0));
    setGreen->setColor(QColor(0, 255, 0));
    setBlue->setColor(QColor(0, 0, 255));
    series->append(setRed);
    series->append(setGreen);
    series->append(setBlue);

    chartInput->addSeries(series);
    chartInput->setTitle("Histogram obrazu wyjsciowego");
    chartInput->setAnimationOptions(QChart::SeriesAnimations);

    chartInput->legend()->setVisible(true);
    chartInput->legend()->setAlignment(Qt::AlignBottom);
    series->setBarWidth(2);
    ui.graphicsView->setChart(chartInput);
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->show();
}

void JAProjekt::on_benchmarkButton_clicked()
{
	
   
    bmpEditor.setTreshold(ui.tresholdHorizontalSlider->value() / 100.0);
    Benchmark bench(&bmpEditor);
    bench.run();
   
}
