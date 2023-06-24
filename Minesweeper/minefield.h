/// Minefield is a custom QLabel that registers mouse presses.
/// Written by: ezphelps

#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QCursor>
#include <QRect>
#include "minesweeperwindow.h"


class MineField : public QLabel
{
    Q_OBJECT
public:
    explicit MineField(QWidget *parent = nullptr);

public slots:
    void setPtr(MinesweeperWindow *ptr);

signals:
    void mousePressed(int x, int y);
    void mouseRelease(int x, int y);
    void rightClick(int x, int y);
    void spaceHit(int x, int y);

protected:
    bool event(QEvent *event) override;

private:
    MinesweeperWindow *windowPtr;
    bool mouseIsDragging;
    int mouseX, mouseY;

};

#endif // MINEFIELD_H
