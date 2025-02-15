/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dialogshowtablerow.h"
#include "ui_dialogshowtablerow.h"
#include "src/dialogmagnify.h"
#include "src/msgtoast.h"
#include "src/utils.h"

#define UNUSED(x) (void)x

DialogShowTableRow::DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableRow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    Utils::setDefaultWindowFonts(this);
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(4);
    ptr_table->setRowCount(rowdata->count());
    ptr_table->setAlternatingRowColors(true);
    //自动换行和默认高度
    ptr_table->setWordWrap(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    //默认情况下字符数多于此数值的行才进行换行自适应
    maxSingleLineCharset=24;

    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

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

    action_Magnify = new QAction(tr("放大镜"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    connect(action_ShowSaveScreen, SIGNAL(triggered()), this, SLOT(saveScreen()));
    connect(action_ShowSaveScreen2ToClipboard, SIGNAL(triggered()), this, SLOT(saveScreen2ToClipboard()));
    connect(action_ShowSaveScreen2, SIGNAL(triggered()), this, SLOT(saveScreen2()));
    connect(action_ShowSaveScreen2ToClipboard2, SIGNAL(triggered()), this, SLOT(saveScreen2ToClipboard2()));
    connect(action_Magnify, SIGNAL(triggered()), this, SLOT(showMagnify()));
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


    //设置表格列标题
    QStringList title;
    title.append("   字段中文名   ");
    title.append("   字段英文名   ");
    title.append("字段值");
    title.append("       字典解释       ");
    ptr_table->setHorizontalHeaderLabels(title);

    //设置表格的内容
    if(rowdata->count()>0){
        for (int row = 0; row < rowdata->count(); ++row)
        {
            for(int col=0;col<4;col++){
                if(col==0){
                    //第5个字段是tips
                    QString tip=((QString)rowdata->at(row).at(4));
                    QTableWidgetItem *item= new QTableWidgetItem(rowdata->at(row).at(col));
                    if(!tip.isEmpty()){
                        item->setToolTip(tip);
                    }
                    ptr_table->setItem(row, col, item);
                }
                else{
                    QTableWidgetItem *item= new QTableWidgetItem(rowdata->at(row).at(col));
                    ptr_table->setItem(row, col, item);
                }
            }
        }
        //表格初始化完毕后，搜索默认以00开始
        searchColumn=0;
        searchRow=0;
    }
    // 在这里设置行高的时候，会出现很诡异的事情，某些行的高度变得非常高，所以改用定时器在页面加载完毕后再设置
    dataisOK=true;
    //bug修正,基于定时器在界面初始化完整之后再设置行高
    resizeFiletimer = new QTimer(this);
    connect(resizeFiletimer,SIGNAL(timeout()),this,SLOT(resizeHeight()));
    resizeFiletimer->start(1);
}

void  DialogShowTableRow:: resizeHeight(){
    for (int row = 0; row < ptr_table->rowCount();row++)
    {
        if (ptr_table->item(row,0)->text().length()>maxSingleLineCharset||ptr_table->item(row,1)->text().length()>maxSingleLineCharset||ptr_table->item(row,2)->text().length()>maxSingleLineCharset||ptr_table->item(row,3)->text().length()>maxSingleLineCharset){
            ptr_table->resizeRowToContents(row);
        }
    }
    //仅使用一次定时器,后续通过窗口resizeEvent调整
    resizeFiletimer->stop();
}

void DialogShowTableRow:: resizeEvent (QResizeEvent * event ){
    UNUSED(event);
    if(dataisOK){
        for (int row = 0; row < ptr_table->rowCount();row++)
        {
            if (ptr_table->item(row,0)->text().length()>maxSingleLineCharset||ptr_table->item(row,1)->text().length()>maxSingleLineCharset||ptr_table->item(row,2)->text().length()>maxSingleLineCharset||ptr_table->item(row,3)->text().length()>maxSingleLineCharset){
                ptr_table->resizeRowToContents(row);
            }
        }
    }
}

DialogShowTableRow::~DialogShowTableRow()
{
    delete action_ShowCopyColum;
    delete action_ShowSaveScreen;
    delete action_Magnify;
    delete tablePopMenu;
    delete ui;
}

void DialogShowTableRow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount==1){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个单元格--允许使用放大镜
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            tablePopMenu->addAction(action_Magnify);
        }
    }
    //截屏
    tablePopMenu->addAction(action_ShowSaveScreen);
    tablePopMenu->addAction(action_ShowSaveScreen2);
    tablePopMenu->addAction(action_ShowSaveScreen2ToClipboard);
    tablePopMenu->addAction(action_ShowSaveScreen2ToClipboard2);

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
        Toast::showMsg(QString("已复制数据到剪切板~"), ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
    else if(rangeCount>1){
        Toast::showMsg(QString("无法对多重选择区域执行复制！"), ToastTime::Time::ToastTime_normal,ToastType::Type::ToastType_warn,this);
    }
}

void DialogShowTableRow::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        int colcount=ptr_table->columnCount();
        //开始搜索
        //逻辑--搜到了就允许继续往下搜，搜不到就搜一圈停止
        int beginrow=searchRow;
        int begincol=searchColumn;
        //先从开始搜索的位置搜索到结尾
        for(int i=beginrow;i<rowcount;i++){
            int col=0;
            //搜索开始行时，列从searchCol开始
            if(i==beginrow){
                col=begincol;
            }
            else{
                col=0;
            }
            for(int j=col;j<colcount;j++){
                if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(i,j);
                    ptr_table->setFocus();
                    return;
                }
            }
        }
        //定义从头搜索的结束单元格
        int againendrow=0;
        int againendcol=0;
        //没搜到，从开头搜到开始搜索的列的上一个单元格
        //从某一行的非第一列，重新搜索到上一个单元格
        if(begincol>0){
                againendrow=beginrow;
                againendcol=begincol-1;
        }
        //非第一行的第一列-重新搜索到上一行的最后一列
        else if(beginrow>0&&begincol==0){
            againendrow=beginrow-1;
            againendcol=colcount-1;
        }
        //如果是从首行首列开始搜索的，直接结束
        else if(beginrow==0&&begincol==0){
            Toast::showMsg("搜索了一圈,没找到你要搜索的内容...", ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
            return;
        }
        for(int i=0;i<=againendrow;i++){
            int colend;
            if(i==againendrow){
                colend=againendcol;
            }
            else{
                colend=colcount-1;
            }
            for(int j=0;j<=colend;j++){
                //搜索到了
                if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(i,j);
                    ptr_table->setFocus();
                    return;
                }
            }
        }
        Toast::showMsg("搜索了一圈,没找到你要搜索的内容...", ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
}

/**
 * @brief DialogShowTableRow::SscreenShoot 1截图当前可视区域到文件,2截图整个表格到文件,3截取当前可视区域到剪切板,4截取整个表格到文件
 * @param Type
 */
void DialogShowTableRow::SscreenShoot(int Type){
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

void DialogShowTableRow::saveScreen(){
    SscreenShoot(1);
}

void DialogShowTableRow::saveScreen2ToClipboard(){
    SscreenShoot(3);
}

void DialogShowTableRow::saveScreen2(){
    SscreenShoot(2);
}

void DialogShowTableRow::saveScreen2ToClipboard2(){
    SscreenShoot(4);
}

/**
 * @brief DialogShowTableRow::showMagnify 打开放大镜
 */
void DialogShowTableRow::showMagnify(){
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
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
                DialogMagnify * dialog = new DialogMagnify(text,this);
                dialog->setWindowTitle(QString("数据放大镜"));
                dialog->setModal(false);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                dialog->show();
                dialog->raise();
                dialog->activateWindow();
            }
        }
    }
}


void DialogShowTableRow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    int rowcount=ptr_table->rowCount();
    int colcount=ptr_table->columnCount();
    this->searchRow=currentRow;
    this->searchColumn=currentColumn;
    //焦点在最后一个单元格时，转移到第一行第一列，其余情况起始列+1
    if(searchRow==(rowcount-1)&&searchColumn==(colcount-1)){
        this->searchRow=0;
        this->searchColumn=0;
    }
    else{
        this->searchColumn+=1;
        //溢出到下一行
        if(searchColumn>colcount-1){
            if(searchRow<rowcount-1){
                searchRow+=1;
                searchColumn=0;
            }
            else{
                searchRow=0;
                searchColumn=0;
            }
        }
    }
}

