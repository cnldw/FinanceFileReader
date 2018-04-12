#include "dialogshowtablerow.h"
#include "ui_dialogshowtablerow.h"

DialogShowTableRow::DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableRow)
{
    ui->setupUi(this);

    //初始化表格
    QTableWidget * ptr_table =ui->tableWidget;
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptr_table->setColumnCount(4);
    int width=ptr_table->width()-20;
    ptr_table->setColumnWidth(0,(width/4)-1);
    ptr_table->setColumnWidth(1,(width/4)-1);
    ptr_table->setColumnWidth(2,(width/4)-1);
    ptr_table->setColumnWidth(3,(width/4)-1);
    ptr_table->setRowCount(rowdata->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    //设置表格列标题
    QStringList title;
    title.append("字段中文名");
    title.append("字段英文名");
    title.append("字段值");
    title.append("字典解释");
    ptr_table->setHorizontalHeaderLabels(title);

    //设置表格的内容
    for (int row = 0; row < rowdata->count(); ++row)
    {
        for(int col=0;col<4;col++){
            QTableWidgetItem *item= new QTableWidgetItem(rowdata->at(row).at(col));
            ptr_table->setItem(row, col, item);
        }
    }
}

DialogShowTableRow::~DialogShowTableRow()
{
    delete ui;
}

void DialogShowTableRow::setData(QList<QStringList> * rowdata){
    this->rowdata=rowdata;
}
