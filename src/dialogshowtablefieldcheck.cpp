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
#include "src/dialogshowtablefieldcheck.h"
#include "ui_dialogshowtablefieldcheck.h"

DialogShowTableFieldCheck::DialogShowTableFieldCheck(QList<QStringList> * data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableFieldCheck)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);

    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(2);
    ptr_table->setRowCount(data->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setStretchLastSection(true);//关键

    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制光标所在单元格"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

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
                    if(data->at(row).at(0)=="字段原始值"||data->at(row).at(0)=="字段解析值"){
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

void DialogShowTableFieldCheck::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    posCurrentMenu=pos;
    //判断当前鼠标位置是不是在表格单元格位置内
    //空表
    if(ptr_table->rowCount()<1){
        return;
    }
    //如果鼠标点击的是表格的空白位置取到得行y是-1
    if( ptr_table->rowAt(pos.y()) <0){
        return;
    }
    tablePopMenu->clear();
    tablePopMenu->addAction(action_ShowCopyColum);
    tablePopMenu->exec(QCursor::pos());
}

void DialogShowTableFieldCheck::copyToClipboard(){
    if(ptr_table->itemAt(posCurrentMenu)!=nullptr){
        QString text= ptr_table->itemAt(posCurrentMenu)->text();
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
    }
}

DialogShowTableFieldCheck::~DialogShowTableFieldCheck()
{
    delete action_ShowCopyColum;
    delete tablePopMenu;
    delete ui;
}

