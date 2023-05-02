#include "minesweeperwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MinesweeperWindow w;
    w.show();
    return a.exec();
}
