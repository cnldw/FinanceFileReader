#ifndef DIALOGSHOWTABLEROW_H
#define DIALOGSHOWTABLEROW_H

#include <QDialog>

namespace Ui {
class DialogShowTableRow;
}

class DialogShowTableRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableRow(QWidget *parent = 0);
    ~DialogShowTableRow();

private:
    Ui::DialogShowTableRow *ui;
};

#endif // DIALOGSHOWTABLEROW_H
