#include "dialogshowtablefiledcheck.h"
#include "ui_dialogshowtablefiledcheck.h"

DialogShowTableFiledCheck::DialogShowTableFiledCheck(QList<QStringList> * data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableFiledCheck)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(2);
    ptr_table->setRowCount(data->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setStretchLastSection(true);//关键
    //设置表格列标题
    QStringList title;
    title.append("列描述");
    ptr_table->setColumnWidth(0,200);
    title.append("列内容");
    ptr_table->setHorizontalHeaderLabels(title);

    //设置表格的内容
    if(data->count()>0){
        for (int row = 0; row < data->count(); ++row)
        {
            for(int col=0;col<2;col++){
                if(col<data->at(row).count()){
                    QString value=data->at(row).at(col);
                    if(data->at(row).at(0)=="字段原始值"||data->at(row).at(0)=="字段解释值"){
                        if(col==0){
                            value=value+"(不包含括号)";
                        }
                        else{
                            value="["+value+"]";
                        }
                    }
                    QTableWidgetItem *item= new QTableWidgetItem(value);
                    ptr_table->setItem(row, col, item);
                }
            }
        }
    }
}

DialogShowTableFiledCheck::~DialogShowTableFiledCheck()
{
    delete ui;
}

