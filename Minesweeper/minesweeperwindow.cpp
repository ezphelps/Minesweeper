#include "minesweeperwindow.h"
#include "ui_minesweeperwindow.h"

MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MinesweeperWindow)
{
    ui->setupUi(this);
}

MinesweeperWindow::~MinesweeperWindow()
{
    delete ui;
}

