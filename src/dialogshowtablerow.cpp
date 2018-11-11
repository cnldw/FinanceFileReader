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
#include "src/dialogshowtablerow.h"
#include "ui_dialogshowtablerow.h"
#define UNUSED(x) (void)x

DialogShowTableRow::DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableRow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);

    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(4);
    ptr_table->setRowCount(rowdata->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setStretchLastSection(true);//关键

    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
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
        currentColumn=0;
        currentRow=0;
    }
}

DialogShowTableRow::~DialogShowTableRow()
{
    delete action_ShowCopyColum;
    delete tablePopMenu;
    delete ui;
}

void DialogShowTableRow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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

void DialogShowTableRow::copyToClipboard(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount==1){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个单元格复制
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            QString text="";
            QClipboard *board = QApplication::clipboard();
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
            }
            board->setText(text);
        }
        //多个单元格复制
        else{
            QString value="";
            for(int row=topRow;row<=bottomRow;row++){
                for(int col=leftColumn;col<=rigthColumn;col++){
                    if(ptr_table->item(row,col)==nullptr){
                        value.append("");
                    }
                    else{
                        value.append(ptr_table->item(row,col)->text());
                    }
                    if(col<rigthColumn){
                        value.append("\t");
                    }else if(row<bottomRow){
                        value.append("\r\n");
                    }
                }
            }
            QClipboard *board = QApplication::clipboard();
            board->setText(value);
        }
    }
    else if(rangeCount>1){
        QMessageBox::warning(this,tr("警告"),"无法对多重选择区域执行复制!",QMessageBox::Ok,QMessageBox::Ok);
    }
}

void DialogShowTableRow::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        int colcount=ptr_table->columnCount();
        if(endFlag){
            currentRow=0;
            currentColumn=0;
        }
        for(int i=currentRow;i<rowcount;i++){
            //如果搜到了最后一列，跳到下一行
            if(currentColumn>colcount-1){
                currentColumn=0;
                continue;
            }
            for(int j=currentColumn;j<colcount;j++){
                if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(i,j);
                    ptr_table->setFocus();
                    endFlag=false;
                    if(j==colcount-1){
                        currentColumn+=1;
                    }
                    return;
                }
                //判断是否是最后一列最后一行
                if(i==rowcount-1&&j==colcount-1){
                    endFlag=true;
                    break;
                }
                else if(j==colcount-1){
                    currentColumn+=1;
                }
            }
        }
    }
}

void DialogShowTableRow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    UNUSED(previousRow);
    UNUSED(previousColumn);
    this->currentRow=currentRow;
    this->currentColumn=currentColumn+1;
}
