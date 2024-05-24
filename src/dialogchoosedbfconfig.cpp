/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogchoosedbfconfig.h"
#include "ui_dialogchoosedbfconfig.h"
#include "src/utils.h"

DialogChooseDBFConfig::DialogChooseDBFConfig(QList<QStringList> * matchDBF,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseDBFConfig)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
    /**************************************************************/
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //一次只能选择一行
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptr_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ptr_table->setColumnCount(4);
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    //设定默认的规则，表格宽度自动适应
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //锁定第二列
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_table->setColumnWidth(0, 220);
    ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ptr_table->setColumnWidth(1, 140);
    ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ptr_table->setColumnWidth(3, 100);
    //设置表格列标题
    QStringList title;
    title.append("配置文件");
    title.append("解析配置");
    title.append("文件说明");
    title.append("字段匹配程度");
    ptr_table->setHorizontalHeaderLabels(title);
    ptr_table->setRowCount(matchDBF->count());
    if(matchDBF->count()==0){
        return;
    }
    for (int i=0;i<matchDBF->count();i++){
        QTableWidgetItem *itemini= new QTableWidgetItem(matchDBF->at(i).at(0));
        ptr_table->setItem(i, 0, itemini);
        QTableWidgetItem *item1= new QTableWidgetItem(matchDBF->at(i).at(1));
        ptr_table->setItem(i, 1, item1);
        QTableWidgetItem *item2= new QTableWidgetItem(matchDBF->at(i).at(2));
        ptr_table->setItem(i, 2, item2);
        QTableWidgetItem *item3= new QTableWidgetItem(matchDBF->at(i).at(3));
        ptr_table->setItem(i, 3, item3);
    }
    //默认选中第一行
    ptr_table->selectRow(0);
}

DialogChooseDBFConfig::~DialogChooseDBFConfig()
{
    delete ui;
}

int DialogChooseDBFConfig::getChooeseIndex() const
{
    return chooeseIndex;
}

void DialogChooseDBFConfig::on_pushButton_clicked()
{
    confirm=true;
    this->close();
}

void DialogChooseDBFConfig::on_tableWidget_itemSelectionChanged()
{
    chooeseIndex=ptr_table->selectedRanges().at(0).topRow();
}

bool DialogChooseDBFConfig::getConfirm() const
{
    return confirm;
}

void DialogChooseDBFConfig::setConfirm(bool value)
{
    confirm = value;
}

void DialogChooseDBFConfig::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    chooeseIndex=row;
    confirm=true;
    this->close();
}
