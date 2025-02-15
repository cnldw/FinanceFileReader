/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogchooseofdconfig.h"
#include "ui_dialogchooseofdconfig.h"
#include "src/utils.h"

DialogChooseOFDConfig::DialogChooseOFDConfig(QString ofdini,QList<OFDFileDefinition> * matchOFD,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseOFDConfig)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptr_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ptr_table->setColumnCount(4);
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_table->setColumnWidth(0, 100);
    ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ptr_table->setColumnWidth(1, 220);
    ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ptr_table->setColumnWidth(3, 120);
    QStringList title;
    title.append("配置文件");
    title.append("解析配置");
    title.append("文件说明");
    title.append("字段总数");
    ptr_table->setHorizontalHeaderLabels(title);
    ptr_table->setRowCount(matchOFD->count());
    if(matchOFD->count()==0){
        return;
    }
    for (int i=0;i<matchOFD->count();i++){
        QTableWidgetItem *itemini= new QTableWidgetItem(ofdini);
        ptr_table->setItem(i, 0, itemini);
        QTableWidgetItem *item1= new QTableWidgetItem(matchOFD->at(i).getConfigSegment());
        ptr_table->setItem(i, 1, item1);
        QTableWidgetItem *item2= new QTableWidgetItem(matchOFD->at(i).getDescribe());
        ptr_table->setItem(i, 2, item2);
        QTableWidgetItem *item3= new QTableWidgetItem(QString::number(matchOFD->at(i).getFieldCount()));
        ptr_table->setItem(i, 3, item3);
    }
    ptr_table->selectRow(0);
}

DialogChooseOFDConfig::~DialogChooseOFDConfig()
{
    delete ui;
}

int DialogChooseOFDConfig::getChooeseIndex() const
{
    return chooeseIndex;
}

void DialogChooseOFDConfig::on_pushButton_clicked()
{
    confirm=true;
    this->close();
}

void DialogChooseOFDConfig::on_tableWidget_itemSelectionChanged()
{
    chooeseIndex=ptr_table->selectedRanges().at(0).topRow();
}

bool DialogChooseOFDConfig::getConfirm() const
{
    return confirm;
}

void DialogChooseOFDConfig::setConfirm(bool value)
{
    confirm = value;
}

void DialogChooseOFDConfig::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    chooeseIndex=row;
    confirm=true;
    this->close();
}
