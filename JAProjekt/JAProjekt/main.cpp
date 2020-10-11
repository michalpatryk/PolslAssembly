#include "JAProjekt.h"
#include <QtWidgets/QApplication>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JAProjekt w;
    w.show();
    //std::string filename = QFileDialog::getOpenFileName(this,
    //    tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    return a.exec();
}
