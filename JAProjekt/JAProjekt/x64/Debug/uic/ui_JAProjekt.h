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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JAProjektClass
{
public:
    QPushButton *pushButton;
    QToolButton *toolButton;

    void setupUi(QWidget *JAProjektClass)
    {
        if (JAProjektClass->objectName().isEmpty())
            JAProjektClass->setObjectName(QString::fromUtf8("JAProjektClass"));
        JAProjektClass->resize(600, 400);
        pushButton = new QPushButton(JAProjektClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(350, 300, 75, 23));
        toolButton = new QToolButton(JAProjektClass);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(450, 80, 25, 19));

        retranslateUi(JAProjektClass);

        QMetaObject::connectSlotsByName(JAProjektClass);
    } // setupUi

    void retranslateUi(QWidget *JAProjektClass)
    {
        JAProjektClass->setWindowTitle(QCoreApplication::translate("JAProjektClass", "JAProjekt", nullptr));
        pushButton->setText(QCoreApplication::translate("JAProjektClass", "PushButton", nullptr));
        toolButton->setText(QCoreApplication::translate("JAProjektClass", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JAProjektClass: public Ui_JAProjektClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JAPROJEKT_H
