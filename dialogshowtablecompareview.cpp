#include "dialogshowtablecompareview.h"
#include "ui_dialogshowtablecompareview.h"
#define UNUSED(x) (void)x
DialogShowTableCompareView::DialogShowTableCompareView(QStringList title,QMap<int,QStringList> * compareData,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableCompareView)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog |Qt::WindowMaximizeButtonHint| Qt::WindowCloseButtonHint);

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
    action_ShowCopyColum = new QAction(tr("复制此单元格"),this);
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
            //第一列行号
            QTableWidgetItem *item= new QTableWidgetItem(QString::number(keys.at(i)));
            ptr_table->setItem(i, 0, item);
            //从第二列开始为数据内容
            for(int col=0;col<(compareData->value(keys.at(i)).count())&&col<ptr_table->columnCount();col++){
                QTableWidgetItem *item2= new QTableWidgetItem(compareData->value(keys.at(i)).at(col));
                ptr_table->setItem(i, col+1, item2);
                //从第二行开始，开始判断是否不一样，一旦有不一样的直接就标记此列有不一样的
                if(i==1){
                    //如果第一行等于第二行
                    if((compareData->value(keys.at(i)).at(col))!=(compareData->value(keys.at(i-1)).at(col))){
                        //插入不等标记
                        colNoEqual.insert(col,true);
                        for(int i2=0;i2<=i;i2++){
                            ptr_table->item(i2,col+1)->setBackgroundColor(QColor(241,226,173));
                        }
                    }
                    else{
                        colNoEqual.insert(col,false);
                    }
                }else if(i>1){
                    //先判断前几行是否不相等,如果不相等直接更新本行颜色即可
                    if(colNoEqual.value(col)){
                        ptr_table->item(i,col+1)->setBackgroundColor(QColor(241,226,173));
                    }
                    else{
                        //前两行相等,则拿本行和上一行对比
                        if((compareData->value(keys.at(i)).at(col))!=(compareData->value(keys.at(i-1)).at(col))){
                            //插入不等标记
                            colNoEqual.insert(col,true);
                            for(int i2=0;i2<=i;i2++){
                                ptr_table->item(i2,col+1)->setBackgroundColor(QColor(241,226,173));
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
    //如果鼠标点击的是表格得空白位置取到得行y是-1
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
            //如果是第二圈搜索且搜索到了开始位置
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
