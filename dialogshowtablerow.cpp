#include "dialogshowtablerow.h"
#include "ui_dialogshowtablerow.h"
#include <QDebug>

DialogShowTableRow::DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableRow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(4);
    ptr_table->setRowCount(rowdata->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setStretchLastSection(true);//关键
    //设置表格列标题
    QStringList title;
    title.append("字段中文名");
    title.append("字段英文名");
    title.append("字段值");
    title.append("字典解释");
    ptr_table->setHorizontalHeaderLabels(title);

    //设置表格的内容
    if(rowdata->count()>0){
        for (int row = 0; row < rowdata->count(); ++row)
        {
            for(int col=0;col<4;col++){
                QTableWidgetItem *item= new QTableWidgetItem(rowdata->at(row).at(col));
                ptr_table->setItem(row, col, item);
            }
        }
        ptr_table->resizeColumnsToContents();
        //表格初始化完毕后，搜索默认以00开始
        beginColumn=0;
        beginRow=0;
    }
}

DialogShowTableRow::~DialogShowTableRow()
{
    delete ui;
}

void DialogShowTableRow::setData(QList<QStringList> * rowdata){
    this->rowdata=rowdata;
}

void DialogShowTableRow::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        int colcount=ptr_table->columnCount();
        //如果焦点在最后一行最后一列，跳到第一个
        if(beginRow>=rowcount-1&&beginColumn>=colcount-1){
            beginColumn=0;
            beginRow=0;
        }
        for(int i=beginRow;i<rowcount;i++){
            if(beginColumn>=colcount-1){
                beginColumn=0;
                continue;
            }
            for(int j=beginColumn;j<colcount;j++){
                if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(i,j);
                    ptr_table->setFocus();
                    beginColumn=j+1;
                    beginRow=i;
                    return;
                }
                else{
                    if(i>=rowcount-1&&j>=colcount-1){
                        beginColumn=0;
                        beginRow=0;
                    }
                    continue;
                }
            }
        }
    }
}

void DialogShowTableRow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    this->beginRow=currentRow;
    this->beginColumn=currentColumn;
}
