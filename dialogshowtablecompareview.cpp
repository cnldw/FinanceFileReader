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
    qSort(keys.begin(), keys.end());
    //数据显示
    if(compareData->count()>0){
        for(int i=0;i<compareData->count();i++){
            //第一列行号
            QTableWidgetItem *item= new QTableWidgetItem(QString::number(keys.at(i)));
            ptr_table->setItem(i, 0, item);
            for(int col=0;col<(compareData->value(keys.at(i)).count());col++){
                QTableWidgetItem *item2= new QTableWidgetItem(compareData->value(keys.at(i)).at(col));
                ptr_table->setItem(i, col+1, item2);
            }
        }
    }
}

DialogShowTableCompareView::~DialogShowTableCompareView()
{
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
}
