#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MinesweeperWindow; }
QT_END_NAMESPACE

class MinesweeperWindow : public QMainWindow
{
    Q_OBJECT

public:
    MinesweeperWindow(QWidget *parent = nullptr);
    ~MinesweeperWindow();

private:
    Ui::MinesweeperWindow *ui;
};
#endif // MINESWEEPERWINDOW_H
