/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/

#include "dialogchooseofdconfig.h"
#include "ui_dialogchooseofdconfig.h"

DialogChooseOFDConfig::DialogChooseOFDConfig(QString ofdini,QList<OFDFileDefinition> * matchOFD,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseOFDConfig)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:13px;font-family:PingFangSC-Regular,sans-serif;");
    ui->tableWidget->setStyleSheet("font-size:13px;font-family:PingFangSC-Light,sans-serif;");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
#endif
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
    ptr_table->setColumnWidth(0, 80);
    ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ptr_table->setColumnWidth(1, 220);
    ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ptr_table->setColumnWidth(3, 160);
    //设置表格列标题
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
    //默认选中第一行
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
