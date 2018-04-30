#ifndef DIALOGMODIFYCELL_H
#define DIALOGMODIFYCELL_H

#include <QDialog>

namespace Ui {
class DialogModifyCell;
}

class DialogModifyCell : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyCell(QWidget *parent = 0);
    ~DialogModifyCell();

private:
    Ui::DialogModifyCell *ui;
};

#endif // DIALOGMODIFYCELL_H
