#include "JAProjekt.h"
#include <QtWidgets/QApplication>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JAProjekt w;
    w.show();
    return a.exec();
}
