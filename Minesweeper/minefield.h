/// Minefield is a custom QLabel that registers mouse presses.
/// Written by: ezphelps

#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QCursor>


class MineField : public QLabel
{
    Q_OBJECT
public:
    explicit MineField(QWidget *parent = nullptr);

signals:
    void mousePressed(int x, int y);
    void rightClick(int x, int y_);

protected:
    bool event(QEvent *event) override;

private:
    bool mouseIsDragging;
    int mouseX, mouseY;

};

#endif // MINEFIELD_H
