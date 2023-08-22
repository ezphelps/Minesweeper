/// Minefield is a custom QLabel that registers mouse presses.
/// Written by: ezphelps

#include "minefield.h"
#include <iostream>

using std::cout;
using std::endl;

/// \brief MineField::MineField
/// Constructor
/// \param parent
MineField::MineField(QWidget *parent) : mouseIsDragging(false), QLabel(parent),
    mouseX(200), mouseY(200)
{

}

/// \brief MineField::event
/// Catches MouseEvents on the MineField.
/// Emits a signal with the mouse x and y coordinates.
/// \param event
/// \return
bool MineField::event(QEvent *event)
{
    //Mouse release
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mEvent = (QMouseEvent*)event;
        QPoint point = mEvent->pos();

        if(mEvent->button() == Qt::RightButton)
        {
            emit rightClick(point.x(), point.y());
        }
        else
        {
            emit mouseRelease(point.x(), point.y());

            return true;
        }
    }
    //Initiate a pressed mouse
    else if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mEvent = (QMouseEvent*)event;
        QPoint point = mEvent->pos();

        if(mEvent->button() != Qt::RightButton)
        {
            emit mousePressed(point.x(), point.y());
            return true;
        }
    }
    //Mouse is moving with mouse press
    else if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mEvent = (QMouseEvent*)event;
        QPoint point = mEvent->pos();

        if(mEvent->button() != Qt::RightButton)
        {
            emit mousePressed(point.x(), point.y());
            return true;
        }
    }
    //Spacebar Released
    else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);


        if(key->key() == Qt::Key_Space)
        {
            QRect geometry = windowPtr->geometry();

            int sqX, sqY;
            sqX = QCursor::pos().x() - geometry.x() - 20;  // Screen-based curser location subtracting the location of
            sqY = QCursor::pos().y() - geometry.y() - 128; // the mineminesweeperwindow and the minefield location within that.

            emit spaceHit(sqX, sqY);
        }

        return true;
    }

    return QWidget::event(event);
}

/// \brief Minefield::setPtr
/// \param ptr
void MineField::setPtr(MinesweeperWindow *ptr)
{
    windowPtr = ptr;
}
