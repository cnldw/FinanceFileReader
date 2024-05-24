/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogoktools.h"
#include "ui_dialogoktools.h"
#include "src/utils.h"

DialogOKTools::DialogOKTools(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOKTools)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
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
    ptr_table->setColumnWidth(0, 50);
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    //设置表格列标题
    QStringList title;
    title.append(" 选择 ");
    title.append("文件路径");
    title.append("OK文件");
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

DialogOKTools::~DialogOKTools()
{
    delete ui;
}
/**
 * @brief DialogOKTools::dragEnterEvent 拖拽支持
 * @param event
 */
void DialogOKTools::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
    }
}

void DialogOKTools::dropEvent(QDropEvent *event)
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
        qDebug()<<path;
        if(QFileInfo(path).isDir()){
            Utils::getFileListFromDirSkipOkfile(path,&fileList);
        }
        else{
            if(!path.endsWith(".ok",Qt::CaseInsensitive)){
                fileList.append(path);
            }
        }
    }
    if(fileList.count()>0){
        ptr_table->setRowCount(fileList.count());
        for(int i=0;i<fileList.count();i++){
            QString filepath=fileList.at(i);
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
            //检查当前是否存在OK文件
            QString okfile="";
            if(Utils::isFileExist(filepath+".ok")){
                okfile=QFileInfo(filepath+".ok").fileName();
            }
            else if(Utils::isFileExist(filepath+".OK")){
                okfile=QFileInfo(filepath+".OK").fileName();
            }
            else if(Utils::isFileExist(filepath+".Ok")){
                okfile=QFileInfo(filepath+".Ok").fileName();
            }
            else if(Utils::isFileExist(filepath+".oK")){
                okfile=QFileInfo(filepath+".oK").fileName();
            }
            QTableWidgetItem *item2= new QTableWidgetItem(okfile);
            item2->setToolTip(okfile);
            ptr_table->setItem(i,2, item2);
        }
    }
    ui->textBrowser->setText(QString("加载文件%1个").arg(fileList.count()));
}


void DialogOKTools::on_pushButton_clicked()
{
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    fileList.clear();
    ui->textBrowser->setText("");
}

void DialogOKTools::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    tablePopMenu->exec(QCursor::pos());
}

void DialogOKTools::checkSelect(){
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

void DialogOKTools::uncheckSelect(){
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

void DialogOKTools::on_pushButton_2_clicked()
{
    //创建或者刷新OK文件
    if(taskType==0){
        if(ptr_table->rowCount()<1){
            return;
        }
        else{
            int sucessCount=0;
            int failtCount=0;
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
                        //存在OK
                        QString oldokfilepath="";
                        if(Utils::isFileExist(ptr_table->item(i,1)->text()+".ok")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".ok";
                        }
                        else if(Utils::isFileExist(ptr_table->item(i,1)->text()+".OK")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".OK";
                        }
                        else if(Utils::isFileExist(ptr_table->item(i,1)->text()+".oK")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".oK";
                        }
                        else if(Utils::isFileExist(ptr_table->item(i,1)->text()+".Ok")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".Ok";
                        }
                        if(!oldokfilepath.isEmpty()){
                            QFile file(oldokfilepath);
                            if(!file.remove()){
                                ptr_table->item(i,2)->setText("删除原OK文件失败！");
                                ptr_table->item(i,2)->setToolTip(nullptr);
                                failtCount++;
                                faultString.append(ptr_table->item(i,1)->text().append("\r\n"));
                                continue;
                            }
                        }
                        //开始创建新的OK
                        QString okfilepath=ptr_table->item(i,1)->text();
                        if(okType==0){
                            okfilepath.append(".OK");
                        }
                        else {
                            okfilepath.append(".ok");
                        }
                        QFile fp (okfilepath);
                        if(fp.open(QIODevice::ReadWrite|QIODevice::Text)){
                            fp.write("\r\n");
                            fp.close();
                            sucessCount++;
                            ptr_table->item(i,2)->setText(QFileInfo(okfilepath).fileName());
                            ptr_table->item(i,2)->setToolTip(QFileInfo(okfilepath).fileName());
                        }
                        else{
                            failtCount++;
                            faultString.append(ptr_table->item(i,1)->text().append("\r\n"));
                            ptr_table->item(i,2)->setText("创建OK文件失败！");
                            ptr_table->item(i,2)->setToolTip(nullptr);
                        }
                    }
                }
            }
            QApplication::restoreOverrideCursor();
            if(failtCount>0){
                ui->textBrowser->setText(QString("创建或者刷新OK成功%1个\r\n创建或者刷新OK失败%2个\r\n失败文件列表如下,请检查是否文件只读或者无权限修改\r\n%3").arg(sucessCount).arg(failtCount).arg(faultString));
            }
            else{
                ui->textBrowser->setText(QString("创建或者刷新OK成功%1个\r\n创建或者刷新OK失败%2个").arg(sucessCount).arg(failtCount));
            }
        }
    }
    //清空OK文件
    else if(taskType==1){
        if(ptr_table->rowCount()<1){
            return;
        }
        else{
            int sucessCount=0;
            int failtCount=0;
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
                        QString oldokfilepath="";
                        if(Utils::isFileExist(ptr_table->item(i,1)->text()+".ok")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".ok";
                        }
                        else if(Utils::isFileExist(ptr_table->item(i,1)->text()+".OK")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".OK";
                        }
                        else if(Utils::isFileExist(ptr_table->item(i,1)->text()+".oK")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".oK";
                        }
                        else if(Utils::isFileExist(ptr_table->item(i,1)->text()+".Ok")){
                            oldokfilepath=ptr_table->item(i,1)->text()+".Ok";
                        }
                        if(!oldokfilepath.isEmpty()){
                            QFile file(oldokfilepath);
                            if(!file.remove()){
                                ptr_table->item(i,2)->setText("删除原OK文件失败！");
                                ptr_table->item(i,2)->setToolTip(nullptr);
                                failtCount++;
                                faultString.append(ptr_table->item(i,1)->text().append("\r\n"));
                                continue;
                            }
                            else{
                                sucessCount++;
                                ptr_table->item(i,2)->setText("OK文件已删除！");
                                ptr_table->item(i,2)->setToolTip(nullptr);
                            }
                        }
                        else{
                            ptr_table->item(i,2)->setText("不存在OK文件，跳过！");
                            ptr_table->item(i,2)->setToolTip(nullptr);
                        }
                    }
                }
            }
            QApplication::restoreOverrideCursor();
            if(failtCount>0){
                ui->textBrowser->setText(QString("删除OK文件成功%1个\r\n删除OK文件失败%2个\r\n失败文件列表如下,请检查是否文件只读或者无权限修改\r\n%3").arg(sucessCount).arg(failtCount).arg(faultString));
            }
            else{
                ui->textBrowser->setText(QString("删除OK文件成功%1个\r\n删除OK文件失败%2个").arg(sucessCount).arg(failtCount));
            }
        }
    }
}

void DialogOKTools::on_comboBox_currentIndexChanged(int index)
{
    taskType=index;
    if(index==1){
        ui->comboBox_2->setEnabled(false);
    }
    else if(index==0){
        ui->comboBox_2->setEnabled(true);
    }
}

void DialogOKTools::on_comboBox_2_currentIndexChanged(int index)
{
    okType=index;
}

