#pragma once

#include <QtWidgets/QWidget>
#include "ui_JAProjekt.h"
#include "BMPEditor.h"
//class to draw graphics
class JAProjekt : public QWidget
{
    Q_OBJECT

public:
    JAProjekt(QWidget *parent = Q_NULLPTR);
    BMPEditor bmpEditor;
private:
    Ui::JAProjektClass ui;
    QChart* chartInput = nullptr;
    QBarSeries* series = nullptr;
private slots:
    void on_loadFileButton_clicked();
    void on_cppAlgButton_clicked();
    void on_asmAlgButton_clicked();
    void on_saveFileButton_clicked();
    void on_inputHistogramButton_clicked();
    void on_outputHistogramButton_clicked();
    void on_benchmarkButton_clicked();
};
