#pragma once

#include <QtWidgets/QWidget>
#include "ui_JAProjekt.h"

class JAProjekt : public QWidget
{
    Q_OBJECT

public:
    JAProjekt(QWidget *parent = Q_NULLPTR);

private:
    Ui::JAProjektClass ui;
};
