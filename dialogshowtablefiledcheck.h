#ifndef DIALOGSHOWTABLEFILEDCHECK_H
#define DIALOGSHOWTABLEFILEDCHECK_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QClipboard>
#include <QPoint>
#include <QMenu>

namespace Ui {
class DialogShowTableFiledCheck;
}

class DialogShowTableFiledCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableFiledCheck(QList<QStringList> * data,QWidget *parent = 0);
    ~DialogShowTableFiledCheck();

private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

private:
    Ui::DialogShowTableFiledCheck *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    //
    QPoint posCurrentMenu;
};

#endif // DIALOGSHOWTABLEFILEDCHECK_H
