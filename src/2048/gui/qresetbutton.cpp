#include "gui/qresetbutton.h"
#define UNUSED(x) (void)x
QResetButton::QResetButton( QWidget* parent) : QLabel(parent)
{
    setText("再来一局!");
    setAlignment(Qt::AlignCenter);
    setStyleSheet("QResetButton { background-color: rgb(143,122,102); border-radius: 10px; font: bold; color: white; }");
}

void QResetButton::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
    UNUSED(event);
}
