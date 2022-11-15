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
#include "dialogeditheaderfooter.h"
#include "ui_dialogeditheaderfooter.h"

DialogEditHeaderFooter::DialogEditHeaderFooter(OFDFileDefinition *ofd,QList<QStringList> header,QList<QStringList> footer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditHeaderFooter)
{
    Q_UNUSED(ofd);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:13px;font-family:PingFangSC-Regular,sans-serif;");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
#endif
    ui->setupUi(this);
    QStringList title;
    title.append("文件中行号");
    title.append("数据内容");
    title.append("数据说明");

    ptr_header_table =ui->tableWidget;
    ptr_header_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_header_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_header_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_header_table->setColumnCount(3);
    ptr_header_table->setRowCount(header.count());
    ptr_header_table->setAlternatingRowColors(true);
    ptr_header_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_header_table->horizontalHeader()->setStretchLastSection(true);
    ptr_header_table->setFocus();
    ptr_header_table->setHorizontalHeaderLabels(title);
    ptr_header_table->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //设定默认的规则，表格宽度自动适应
    ptr_header_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //锁定第一列
    ptr_header_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_header_table->setColumnWidth(0, 90);

    if(header.count()>0){
        for(int i=0;i<header.count();i++){
            QTableWidgetItem *item1= new QTableWidgetItem(header.at(i).at(0));
            item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
            ptr_header_table->setItem(i, 0, item1);

            QTableWidgetItem *item2= new QTableWidgetItem(header.at(i).at(1));
            ptr_header_table->setItem(i, 1, item2);

            QTableWidgetItem *item3= new QTableWidgetItem(header.at(i).at(2));
            item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
            ptr_header_table->setItem(i, 2, item3);
        }
    }

    ptr_footer_table =ui->tableWidget_2;
    ptr_footer_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_footer_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_footer_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_footer_table->setColumnCount(3);
    ptr_footer_table->setRowCount(footer.count());
    ptr_footer_table->setAlternatingRowColors(true);
    ptr_footer_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_footer_table->horizontalHeader()->setStretchLastSection(true);
    ptr_footer_table->setHorizontalHeaderLabels(title);
    ptr_footer_table->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //设定默认的规则，表格宽度自动适应
    ptr_footer_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //锁定第一列
    ptr_footer_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_footer_table->setColumnWidth(0, 90);

    if(footer.count()>0){
        for(int i=0;i<footer.count();i++){
            QTableWidgetItem *item1= new QTableWidgetItem(footer.at(i).at(0));
            item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
            ptr_footer_table->setItem(i, 0, item1);

            QTableWidgetItem *item2= new QTableWidgetItem(footer.at(i).at(1));
            ptr_footer_table->setItem(i, 1, item2);

            QTableWidgetItem *item3= new QTableWidgetItem(footer.at(i).at(2));
            item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
            ptr_footer_table->setItem(i, 2, item3);
        }
    }
}

DialogEditHeaderFooter::~DialogEditHeaderFooter()
{
    delete ui;
}

void DialogEditHeaderFooter::on_pushButton_2_clicked()
{
    this->close();
}


void DialogEditHeaderFooter::on_pushButton_clicked()
{   QStringList header;
    if(ptr_header_table->rowCount()>0){
        for(int i=0;i<ptr_header_table->rowCount();i++){
            header.append(ptr_header_table->item(i,1)->text());
        }
    }
    QStringList footer;
    if(ptr_footer_table->rowCount()>0){
        for(int i=0;i<ptr_footer_table->rowCount();i++){
            footer.append(ptr_footer_table->item(i,1)->text());
        }
    }
    this->close();
    emit sendNewHeaderAndFooter(header,footer);
}
