/// MinesweeperWindow represents the ui of the minesweeper gameplay window.
/// Written by: ezphelps

#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include <QMainWindow>
#include <QImage>

#include <model.h>
#include "sspdeserializer.h"

using std::array;

QT_BEGIN_NAMESPACE
namespace Ui { class MinesweeperWindow; }
QT_END_NAMESPACE

class MinesweeperWindow : public QMainWindow
{
    Q_OBJECT

public:
    MinesweeperWindow(QWidget *parent = nullptr);
    ~MinesweeperWindow();

public slots:
    void squareClickedSlot(int x, int y);
    void validSquareSlot(int numMines, int x, int y);
    void invalidSquareSlot(int x, int y);
    void rightCLickSlot(int x, int y);

signals:
    void squareClicked(int x, int y);
    void getSquareImage(QString);
private:
    Ui::MinesweeperWindow *ui;
    Model model;
    sspDeserializer deserializer;
    int sizeX, sizeY;
    QImage minefieldImage;
    array<QImage, 11> images;
};
#endif // MINESWEEPERWINDOW_H
