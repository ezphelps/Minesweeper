/// MinesweeperWindow represents the ui of the minesweeper gameplay window.
/// Written by: ezphelps

#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include <QMainWindow>
#include <QImage>

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
    void squareClicked(int x, int y);

private:
    Ui::MinesweeperWindow *ui;
    int sizeX, sizeY;
    array<array<int, 16>, 30> minefieldLayout;
    QImage minefieldImage;
};
#endif // MINESWEEPERWINDOW_H
