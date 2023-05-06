/// Minefield is a custom QLabel that registers mouse presses.
/// Written by: ezphelps

#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>

class MineField : public QLabel
{
    Q_OBJECT
public:
    explicit MineField(QWidget *parent = nullptr);

signals:
    void mousePressed(int x, int y);

protected:
    bool event(QEvent *event);

private:
    bool mouseIsDragging;

};

#endif // MINEFIELD_H
