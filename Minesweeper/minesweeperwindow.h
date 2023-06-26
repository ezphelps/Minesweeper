/// MinesweeperWindow represents the ui of the minesweeper gameplay window.
/// Written by: ezphelps

#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QIcon>

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
    void restartButtonSlot();
    void resetMinefieldSlot(int numMines);
    void mouseDraggingSlot(int x, int y);
    void showSelectedSquareSlot(int x, int y);
    void unselectSquareSlot(int x, int y);
    void squareClickedSlot(int x, int y);
    void validSquareSlot(int numMines, int x, int y);
    void hitMineSlot(int x, int y);
    void invalidSquareSlot(int x, int y);
    void rightCLickSlot(int x, int y);
    void spaceHitSlot(int x, int y);
    void displayFlagSlot(int minesLeft, int x, int y);
    void removeFlagSlot(int minesLeft, int x, int y);
    void falseFlagSlot(int x, int y);
    void playerWinsSlot();

signals:
    void setWindowPtr(MinesweeperWindow*);
    void restartButton();
    void mouseDrag(int x, int y);
    void squareClicked(int x, int y);
    void rightClicked(int x, int y);
    void spaceHit(int x, int y);
    void getSquareImage(QString);

private:
    Ui::MinesweeperWindow *ui;
    Model model;
    sspDeserializer deserializer;
    int sizeX, sizeY;
    QImage minefieldImage;
    array<QImage, 13> images;
    array<QImage, 10> timeNums;
    array<QImage, 4> buttonImages;

    void shadeSquare(int i, int j);
    void updateMinefield();
    void setNumMines(int numMines);
};
#endif // MINESWEEPERWINDOW_H
