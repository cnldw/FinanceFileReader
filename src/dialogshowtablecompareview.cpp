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
#include "src/dialogshowtablecompareview.h"
#include "ui_dialogshowtablecompareview.h"
#define UNUSED(x) (void)x

DialogShowTableCompareView::DialogShowTableCompareView(QStringList title,QMap<int,QStringList> * compareData,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableCompareView)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    //初始化背景色
    this->backcolor=QColor (241,226,173);
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(title.count());
    ptr_table->setRowCount(compareData->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);

    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制光标所在单元格"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

    //设置表格标题
    ptr_table->setHorizontalHeaderLabels(title);
    //获取数据行
    QList<int> keys=compareData->keys();
    //排序
    std::sort(keys.begin(), keys.end());
    //数据显示
    if(compareData->count()>0){
        for(int i=0;i<compareData->count();i++){
            //第一列显示数据的原始行号
            QTableWidgetItem *item= new QTableWidgetItem(QString::number(keys.at(i)));
            ptr_table->setItem(i, 0, item);
            //从第二列开始为数据内容
            for(int index=0;index<(compareData->value(keys.at(i)).count())&&index<ptr_table->columnCount()-1;index++){
                //每行的第一列为该行数据在原始记录中的行号,亲,请注意
                QTableWidgetItem *item2= new QTableWidgetItem(compareData->value(keys.at(i)).at(index));
                ptr_table->setItem(i, index+1, item2);
                //从第二行开始，开始判断是否不一样，一旦有不一样的直接就标记此列有不一样的
                if(i==1){
                    //如果第一行等于第二行
                    if((compareData->value(keys.at(i)).at(index))!=(compareData->value(keys.at(i-1)).at(index))){
                        //插入不等标记
                        colNoEqual.append(index);
                        for(int i2=0;i2<=i;i2++){
                            ptr_table->item(i2,index+1)->setBackgroundColor(backcolor);
                        }
                    }
                }else if(i>1){
                    //先判断前几行是否不相等,如果不相等直接更新本行颜色即可
                    if(colNoEqual.contains(index)){
                        ptr_table->item(i,index+1)->setBackgroundColor(backcolor);
                    }
                    else{
                        //前N行相等,则拿本行和上一行对比,并且如果检测到不一致,需从第一行开始更新背景色
                        if((compareData->value(keys.at(i)).at(index))!=(compareData->value(keys.at(i-1)).at(index))){
                            //插入不等标记
                            colNoEqual.append(index);
                            for(int i2=0;i2<=i;i2++){
                                ptr_table->item(i2,index+1)->setBackgroundColor(backcolor);
                            }
                        }
                    }
                }
            }
        }
        ptr_table->resizeColumnsToContents();
    }
}

DialogShowTableCompareView::~DialogShowTableCompareView()
{
    delete action_ShowCopyColum;
    delete tablePopMenu;
    delete ui;
}

void DialogShowTableCompareView::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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

void DialogShowTableCompareView::copyToClipboard(){
    if(ptr_table->itemAt(posCurrentMenu)!=nullptr){
        QString text= ptr_table->itemAt(posCurrentMenu)->text();
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
    }
}

void DialogShowTableCompareView::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    UNUSED(previousRow);
    UNUSED(previousColumn);
    //记录当前所在行
    rowcurrent=currentRow;
    //当前所在列
    colcurrent=currentColumn;
    //更新列跳转搜索开始列
    colSearch=currentColumn;
}

void DialogShowTableCompareView::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
        bool returnSearch=false;
        int begin=colSearch;
        if((colSearch+1)>=(ptr_table->columnCount()-1)){
            colSearch=-1;
        }
        colSearch+=1;
        for(;colSearch<ptr_table->columnCount();colSearch++){
            if((ptr_table->horizontalHeaderItem(colSearch)->text()).contains(text,Qt::CaseInsensitive)){
                ptr_table->setCurrentCell(rowcurrent,colSearch);
                ptr_table->setFocus();
                if(colSearch==begin){
                    //如果搜了一圈在开始搜索的列找到了目标列，刷新下提示
                    on_tableWidget_currentCellChanged(rowcurrent,colcurrent,0,0);
                }
                return;
            }
            //如果是第二圈搜索且搜索到了开始位置,则终止搜索
            if(returnSearch&&colSearch>=begin){
                return;
            }
            //搜到最后一列还没找到,则跳到第1列继续,保证搜索形成一个圆环
            else if(colSearch>=(ptr_table->columnCount()-1)){
                colSearch=-1;
                returnSearch=true;
            }
        }
    }
}

void DialogShowTableCompareView::on_pushButton_2_clicked()
{
    if(colNoEqual.isEmpty()){
        return;
    }else{
        if(colcurrent!=0){
            for(int col=colcurrent-1;col>=0;col--){
                if(colNoEqual.contains(col-1)){
                    ptr_table->setCurrentCell(rowcurrent,col);
                    ptr_table->setFocus();
                    break;
                }
                //如果找到第一个结束，将光标强制显示到第一个一样的
                if(col==0){
                    ptr_table->setCurrentCell(rowcurrent,colNoEqual.first()+1);
                    ptr_table->setFocus();
                }
            }
        }

    }
}

void DialogShowTableCompareView::on_pushButton_3_clicked()
{
    if(colNoEqual.isEmpty()){
        return;
    }else{
        if(colcurrent<ptr_table->columnCount()-1){
            for(int col=colcurrent+1;col<ptr_table->columnCount();col++){
                if(colNoEqual.contains(col-1)){
                    ptr_table->setCurrentCell(rowcurrent,col);
                    ptr_table->setFocus();
                    break;
                }
                if(col==ptr_table->columnCount()-1){
                    ptr_table->setCurrentCell(rowcurrent,colNoEqual.last()+1);
                    ptr_table->setFocus();
                }
            }
        }

    }
}
