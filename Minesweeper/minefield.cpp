/// Minefield is a custom QLabel that registers mouse presses.
/// Written by: ezphelps

#include "minefield.h"
#include <iostream>

using std::cout;
using std::endl;

/// \brief MineField::MineField
/// Constructor
/// \param parent
MineField::MineField(QWidget *parent) : mouseIsDragging(false), QLabel(parent)
{

}

/// \brief MineField::event
/// Catches MouseEvents on the MineField.
/// Emits a signal with the mouse x and y coordinates.
/// \param event
/// \return
bool MineField::event(QEvent *event)
{
    if (event->type() == QEvent:: MouseButtonPress ||
            (mouseIsDragging && event->type() == QEvent::MouseMove))

    {
        mouseIsDragging = true;
        QMouseEvent* mEvent = (QMouseEvent*)event;
        QPoint point = mEvent->pos();
        emit mousePressed(point.x(), point.y());
    }

    else if (event->type() == QEvent::MouseButtonRelease)
    {
        mouseIsDragging = false;
    }

    return QWidget::event(event);
}
