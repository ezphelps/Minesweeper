/********************************************************************************
** Form generated from reading UI file 'minesweeperwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINESWEEPERWINDOW_H
#define UI_MINESWEEPERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <minefield.h>

QT_BEGIN_NAMESPACE

class Ui_MinesweeperWindow
{
public:
    QWidget *centralwidget;
    MineField *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MinesweeperWindow)
    {
        if (MinesweeperWindow->objectName().isEmpty())
            MinesweeperWindow->setObjectName("MinesweeperWindow");
        MinesweeperWindow->resize(1000, 700);
        centralwidget = new QWidget(MinesweeperWindow);
        centralwidget->setObjectName("centralwidget");
        label = new MineField(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 40, 960, 512));
        MinesweeperWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MinesweeperWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 24));
        MinesweeperWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MinesweeperWindow);
        statusbar->setObjectName("statusbar");
        MinesweeperWindow->setStatusBar(statusbar);

        retranslateUi(MinesweeperWindow);

        QMetaObject::connectSlotsByName(MinesweeperWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MinesweeperWindow)
    {
        MinesweeperWindow->setWindowTitle(QCoreApplication::translate("MinesweeperWindow", "MinesweeperWindow", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MinesweeperWindow: public Ui_MinesweeperWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINESWEEPERWINDOW_H
