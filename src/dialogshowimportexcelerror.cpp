/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogshowimportexcelerror.h"
#include "ui_dialogshowimportexcelerror.h"
#include "src/utils.h"

DialogShowImportExcelError::DialogShowImportExcelError(QStringList * errinfo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowImportExcelError)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setColumnCount(1);
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    //设定默认的规则，表格宽度自动适应窗口
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置表格列标题
    QStringList title;
    title.append("错误信息描述");
    ptr_table->setHorizontalHeaderLabels(title);
    ptr_table->setRowCount(errinfo->count());
    for (int i=0;i<errinfo->count();i++){
        QTableWidgetItem *itemini= new QTableWidgetItem(errinfo->at(i));
        ptr_table->setItem(i, 0, itemini);
    }
}

DialogShowImportExcelError::~DialogShowImportExcelError()
{
    delete ui;
}
