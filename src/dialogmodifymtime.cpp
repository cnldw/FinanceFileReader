/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogmodifymtime.h"
#include "ui_dialogmodifymtime.h"

DialogModifyMtime::DialogModifyMtime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyMtime)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->dateTimeEdit->setEnabled(false);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:13px;font-family:PingFangSC-Regular,sans-serif;");
    ui->tableWidget->setStyleSheet("font-size:13px;font-family:PingFangSC-Light,sans-serif;");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
#endif
#ifdef Q_OS_MAC
    //修正macos下按钮怪异的表现形状
    ui->pushButton->setFixedHeight(30);
    ui->pushButton_2->setFixedHeight(30);
#endif
    /**************************************************************/
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptr_table->setColumnCount(3);
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    //设定默认的规则，表格宽度自动适应
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //锁定第一列
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_table->setColumnWidth(0, 50);
    //锁定第三列
    ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ptr_table->setColumnWidth(2, 150);
    //设置表格列标题
    QStringList title;
    title.append(" 选择 ");
    title.append("文件路径");
    title.append("文件修改时间");
    ptr_table->setHorizontalHeaderLabels(title);
    //初始化表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_Check = new QAction(tr("勾选"),this);
    connect(action_Check, SIGNAL(triggered()), this, SLOT(checkSelect()));
    tablePopMenu->addAction(action_Check);
    action_UnCheck = new QAction(tr("取消勾选"),this);
    connect(action_UnCheck, SIGNAL(triggered()), this, SLOT(uncheckSelect()));
    tablePopMenu->addAction(action_UnCheck);

}

DialogModifyMtime::~DialogModifyMtime()
{
    delete ui;
}

void DialogModifyMtime::on_comboBox_currentIndexChanged(int index)
{
    if(index==0){
        ui->dateTimeEdit->setEnabled(false);
    }
    if(index==1){
        ui->dateTimeEdit->setEnabled(true);
    }
    if(index==2||index==3||index==4||index==5||index==6){
        ui->dateTimeEdit->setEnabled(false);
    }
}

/**
 * @brief DialogModifyMtime::dragEnterEvent 拖拽支持
 * @param event
 */
void DialogModifyMtime::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
    }
}

/**
 * @brief DialogModifyMtime::dropEvent 拖拽支持
 * @param event
 */
void DialogModifyMtime::dropEvent(QDropEvent *event)
{
    //获取拖拽来源的文件信息
    /////////////////////////////////////////////////////////////
    QList<QUrl> urls = event->mimeData()->urls();
    //判断是否包含文件夹,同时获取下参数串，方便稍后传给对比工具，如有需要
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    fileList.clear();
    for(int i=0;i<urls.count();i++){
        QString path=urls.at(i).toLocalFile();
        if(QFileInfo(path).isDir()){
            Utils::getFileListFromDir(path,&fileList);
        }
        else{
            fileList.append(path);
        }
    }
    if(fileList.count()>0){
        ptr_table->setRowCount(fileList.count());
        for(int i=0;i<fileList.count();i++){
            QCheckBox * checkB = new QCheckBox(this); //创建checkbox
            checkB->setCheckState(Qt::Checked); //设置状态
            QWidget *w = new QWidget(this); //创建一个widget
            QHBoxLayout *hLayout = new QHBoxLayout(); //创建布局
            hLayout->addWidget(checkB); //添加checkbox
            hLayout->setMargin(0); //设置边缘距离 否则会很难看
            hLayout->setAlignment(checkB, Qt::AlignCenter); //居中
            w->setLayout(hLayout); //设置widget的布局
            ptr_table->setCellWidget(i,0,w);
            QTableWidgetItem *item= new QTableWidgetItem(fileList.at(i));
            item->setToolTip(fileList.at(i));
            ptr_table->setItem(i, 1, item);
            QFileInfo info(fileList.at(i));
            QTableWidgetItem *item2= new QTableWidgetItem(info.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
            item2->setTextAlignment(Qt::AlignCenter);
            ptr_table->setItem(i,2, item2);
        }
        ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ptr_table->setColumnWidth(0, 50);
        ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        ptr_table->setColumnWidth(2, 150);
    }
    ui->textBrowser->setText(QString("加载文件%1个").arg(fileList.count()));
}

/**
 * @brief DialogModifyMtime::on_pushButton_2_clicked 执行更新
 */
void DialogModifyMtime::on_pushButton_2_clicked()
{
    if(ptr_table->rowCount()<1){
        return;
    }
    else{
        int sucessCount=0;
        int failtCount=0;
        //当前时间
        QDateTime lastModifyTime=QDateTime::currentDateTime();
        //自定义时间
        if(ui->comboBox->currentIndex()==1){
            lastModifyTime=ui->dateTimeEdit->dateTime();
        }
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QString faultString;
        for (int i=0;i<ptr_table->rowCount();i++){
            qApp->processEvents();
            QWidget *w = ptr_table->cellWidget(i, 0);
            QCheckBox * checkBox = qobject_cast<QCheckBox*>(w->children().at(1));
            if ( checkBox )
            {
                if ( checkBox->checkState() == Qt::Checked )
                {
                    QFileInfo info(ptr_table->item(i,1)->text());
                    if(ui->comboBox->currentIndex()==2){
                        lastModifyTime=info.lastModified().addSecs(60);
                    }
                    else if(ui->comboBox->currentIndex()==3){
                        lastModifyTime=info.lastModified().addSecs(-60);
                    }
                    else if(ui->comboBox->currentIndex()==4){
                        lastModifyTime=info.lastModified().addSecs(3600);
                    }
                    else if(ui->comboBox->currentIndex()==5){
                        lastModifyTime=info.lastModified().addSecs(-3600);
                    }
                    else if(ui->comboBox->currentIndex()==6){
                        lastModifyTime=info.lastModified().addDays(1);
                    }
                    else if(ui->comboBox->currentIndex()==7){
                        lastModifyTime=info.lastModified().addDays(-1);
                    }
                    //执行更新
                    Utils::UpdateFileTime(ptr_table->item(i,1)->text(),lastModifyTime);
                    info.refresh();
                    QDateTime lastModifyTimeNew=info.lastModified();
                    if(lastModifyTime.toString("yyyy-MM-dd hh:mm:ss")==lastModifyTimeNew.toString("yyyy-MM-dd hh:mm:ss")){
                        sucessCount++;
                        ptr_table->item(i,2)->setText(lastModifyTimeNew.toString("yyyy-MM-dd hh:mm:ss"));
                    }
                    else{
                        failtCount++;
                        faultString.append(ptr_table->item(i,1)->text().append("\r\n"));
                    }
                }
            }
        }
        QApplication::restoreOverrideCursor();
        if(failtCount>0){
            ui->textBrowser->setText(QString("更新时间戳成功文件%1个\r\n更新时间戳失败文件%2个\r\n失败文件列表如下,请检查是否文件只读或者无权限修改\r\n%3").arg(sucessCount).arg(failtCount).arg(faultString));
        }
        else{
            ui->textBrowser->setText(QString("更新时间戳成功文件%1个\r\n更新时间戳失败文件%2个").arg(sucessCount).arg(failtCount));
        }
    }
}

/**
 * @brief DialogModifyMtime::on_pushButton_clicked 清除
 */
void DialogModifyMtime::on_pushButton_clicked()
{
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    fileList.clear();
    ui->textBrowser->setText("");
}

void DialogModifyMtime::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    tablePopMenu->exec(QCursor::pos());
}


void DialogModifyMtime::checkSelect(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount>0){
        for (int i=0;i<rangeCount;i++){
            int topRow=itemsRange.at(i).topRow();
            int bottomRow=itemsRange.at(i).bottomRow();
            for(int row=topRow;row<=bottomRow;row++){
                QCheckBox * checkB = new QCheckBox(this); //创建checkbox
                checkB->setCheckState(Qt::Checked); //设置状态
                QWidget *w = new QWidget(this); //创建一个widget
                QHBoxLayout *hLayout = new QHBoxLayout(); //创建布局
                hLayout->addWidget(checkB); //添加checkbox
                hLayout->setMargin(0); //设置边缘距离 否则会很难看
                hLayout->setAlignment(checkB, Qt::AlignCenter); //居中
                w->setLayout(hLayout); //设置widget的布局
                ptr_table->setCellWidget(row,0,w);
            }
        }
    }
}

void DialogModifyMtime::uncheckSelect(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount>0){
        for (int i=0;i<rangeCount;i++){
            int topRow=itemsRange.at(i).topRow();
            int bottomRow=itemsRange.at(i).bottomRow();
            for(int row=topRow;row<=bottomRow;row++){
                QCheckBox * checkB = new QCheckBox(this); //创建checkbox
                checkB->setCheckState(Qt::Unchecked); //设置状态
                QWidget *w = new QWidget(this); //创建一个widget
                QHBoxLayout *hLayout = new QHBoxLayout(); //创建布局
                hLayout->addWidget(checkB); //添加checkbox
                hLayout->setMargin(0); //设置边缘距离 否则会很难看
                hLayout->setAlignment(checkB, Qt::AlignCenter); //居中
                w->setLayout(hLayout); //设置widget的布局
                ptr_table->setCellWidget(row,0,w);
            }
        }
    }
}
