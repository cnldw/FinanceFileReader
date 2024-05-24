/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogshowfieldchecklist.h"
#include "ui_dialogshowfieldchecklist.h"
#include "src/msgtoast.h"
#include "src/utils.h"

DialogShowFieldCheckList::DialogShowFieldCheckList(QList<QStringList> *data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowFieldCheckList)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    Utils::setDefaultWindowFonts(this);
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
    ptr_table->setWordWrap(true);


    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制"),this);

    QString text="对表格进行截图保存(Ctrl+Shift+R)";
#ifdef Q_OS_MAC
    text="对表格进行截图保存(Command+Shift+R)";
#endif
    action_ShowSaveScreen= new QAction(text,this);

    QString text_2="对表格滚动截全图保存(Ctrl+Shift+4)";
#ifdef Q_OS_MAC
    text_2="对表格滚动截全图保存(Command+Shift+4)";
#endif
    action_ShowSaveScreen2= new QAction(text_2,this);

    QString text2="对表格进行截图到剪切板(Ctrl+Shift+C)";
#ifdef Q_OS_MAC
    text2="对表格进行截图到剪切板(Ctrl+Shift+C)";
#endif
    action_ShowSaveScreen2ToClipboard= new QAction(text2,this);

    QString text2_2="对表格滚动截全图到剪切板(Ctrl+Shift+D)";
#ifdef Q_OS_MAC
    text2_2="对表格滚动截全图到剪切板(Ctrl+Shift+D)";
#endif
    action_ShowSaveScreen2ToClipboard2= new QAction(text2_2,this);

    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    connect(action_ShowSaveScreen, SIGNAL(triggered()), this, SLOT(saveScreen()));
    connect(action_ShowSaveScreen2ToClipboard, SIGNAL(triggered()), this, SLOT(saveScreen2ToClipboard()));
    connect(action_ShowSaveScreen2, SIGNAL(triggered()), this, SLOT(saveScreen2()));
    connect(action_ShowSaveScreen2ToClipboard2, SIGNAL(triggered()), this, SLOT(saveScreen2ToClipboard2()));
    //截图快捷键
    QShortcut *_shortcut;
    _shortcut = new QShortcut(QKeySequence("Ctrl+Shift+R"), this);
    connect(_shortcut, SIGNAL(activated()),this,SLOT(saveScreen()));
    //复制快捷键-拦截系统的复制函数，使用我们写的
    QShortcut *_shortcut2;
    _shortcut2 = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(_shortcut2, SIGNAL(activated()),this,SLOT(copyToClipboard()));
    //截图到剪切板快捷键
    QShortcut *_shortcut3;
    _shortcut3 = new QShortcut(QKeySequence("Ctrl+Shift+C"), this);
    connect(_shortcut3, SIGNAL(activated()),this,SLOT(saveScreen2ToClipboard()));

    QShortcut *_shortcut4;
    _shortcut4 = new QShortcut(QKeySequence("Ctrl+Shift+4"), this);
    connect(_shortcut4, SIGNAL(activated()),this,SLOT(saveScreen2()));
    QShortcut *_shortcut5;
    _shortcut5 = new QShortcut(QKeySequence("Ctrl+Shift+D"), this);
    connect(_shortcut5, SIGNAL(activated()),this,SLOT(saveScreen2ToClipboard2()));

    QStringList title;
    title.append("条件描述");
    ptr_table->setColumnWidth(0,300);
    title.append("必填字段");
    ptr_table->setHorizontalHeaderLabels(title);

    //设置表格的内容
    if(data->count()>0){
        for (int row = 0; row < data->count(); ++row)
        {
            for(int col=0;col<2;col++){
                if(col<data->at(row).count()){
                    QString value=data->at(row).at(col);
                    QTableWidgetItem *item= new QTableWidgetItem(value);
                    ptr_table->setItem(row, col, item);
                }
            }
        }
    }
    dataisOK=true;
    //bug修正,基于定时器在界面初始化完整之后再设置行高
    resizeFiletimer = new QTimer(this);
    connect(resizeFiletimer,SIGNAL(timeout()),this,SLOT(resizeHeight()));
    resizeFiletimer->start(1);
}

DialogShowFieldCheckList::~DialogShowFieldCheckList()
{
    delete ui;
}

void  DialogShowFieldCheckList:: resizeHeight(){
    for (int row = 0; row < ptr_table->rowCount();row++)
    {
        ptr_table->resizeRowToContents(row);
    }
    //仅使用一次定时器,后续通过窗口resizeEvent调整
    resizeFiletimer->stop();
}

void DialogShowFieldCheckList:: resizeEvent (QResizeEvent * event ){
    Q_UNUSED(event);
    if(dataisOK){
        for (int row = 0; row < ptr_table->rowCount();row++)
        {
            ptr_table->resizeRowToContents(row);

        }
    }
}

/**
 * @brief DialogShowFieldCheckList::SscreenShoot 1截图当前可视区域到文件,2截图整个表格到文件,3截取当前可视区域到剪切板,4截取整个表格到文件
 * @param Type
 */
void DialogShowFieldCheckList::SscreenShoot(int Type){
    QHeaderView * vheader = ui->tableWidget->verticalHeader();
    QHeaderView * hheader = ui->tableWidget->horizontalHeader();
    ptr_table->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ptr_table->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    int iWidth = 0;
    int iHeight = 0;
    QSize oldSize = ptr_table->size();
    //如果截屏全表格,则需要结算表格展开后的宽和高
    if(Type==2||Type==4){
        for (int i = 0; i < ptr_table->columnCount(); i++) {
            iWidth += hheader->sectionSize(i);
        }
        iWidth += vheader->width();
        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            iHeight += vheader->sectionSize(i);
        }
        iHeight += hheader->height();
        ptr_table->resize(iWidth, iHeight);
    }
    QPixmap pixmap(ui->tableWidget->size());
    ptr_table->render(&pixmap);
    //还原宽和高度
    if(Type==2||Type==4){
        ptr_table->resize(oldSize);
    }
    ptr_table->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    ptr_table->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    //根据选项去保存
    if (Type==1||Type==2){
        //获取桌面路径拼接文件路径
        QString filePathName=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/FFReader截图";
        filePathName += QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        filePathName += ".png";
        //弹出保存框
        QString selectedFilter=Q_NULLPTR;
        filePathName = QFileDialog::getSaveFileName(this,("截图另存为"),filePathName,tr("PNG(*.png)"),&selectedFilter);
        if(!filePathName.isEmpty()){
            if(selectedFilter=="PNG(*.png)"&&(!filePathName.endsWith(".png"))){
                filePathName.append(".png");
            }
            pixmap.save(filePathName,"png");
        }
    }
    else if (Type==3||Type==4){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setPixmap(pixmap);
        Toast::showMsg(QString("已复制截图到剪切板~"), ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
}

void DialogShowFieldCheckList::saveScreen(){
    SscreenShoot(1);
}

void DialogShowFieldCheckList::saveScreen2ToClipboard(){
    SscreenShoot(3);
}

void DialogShowFieldCheckList::saveScreen2(){
    SscreenShoot(2);
}

void DialogShowFieldCheckList::saveScreen2ToClipboard2(){
    SscreenShoot(4);
}

void DialogShowFieldCheckList::copyToClipboard(){
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
        Toast::showMsg(QString("已复制数据到剪切板~"), ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
    else if(rangeCount>1){
        Toast::showMsg(QString("无法对多重选择区域执行复制！"), ToastTime::Time::ToastTime_normal,ToastType::Type::ToastType_warn,this);
    }
}

void DialogShowFieldCheckList::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    tablePopMenu->clear();
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
    //复制
    tablePopMenu->addAction(action_ShowCopyColum);
    tablePopMenu->addAction(action_ShowSaveScreen);
    tablePopMenu->addAction(action_ShowSaveScreen2);
    tablePopMenu->addAction(action_ShowSaveScreen2ToClipboard);
    tablePopMenu->addAction(action_ShowSaveScreen2ToClipboard2);

    tablePopMenu->exec(QCursor::pos());
}
