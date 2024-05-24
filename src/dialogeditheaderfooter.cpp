/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogeditheaderfooter.h"
#include "ui_dialogeditheaderfooter.h"
#include "src/utils.h"

DialogEditHeaderFooter::DialogEditHeaderFooter(OFDFileDefinition *ofd,QList<QStringList> header,QList<QStringList> footer,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditHeaderFooter)
{
    Q_UNUSED(ofd);
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    Utils::setDefaultWindowFonts(this);
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
