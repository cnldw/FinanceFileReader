/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dialogshowtablefieldcheck.h"
#include "ui_dialogshowtablefieldcheck.h"
#include "src/msgtoast.h"
#include "src/utils.h"

#define UNUSED(x) (void)x

DialogShowTableFieldCheck::DialogShowTableFieldCheck(QList<QStringList> * data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableFieldCheck)
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

    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制"),this);
    QString text="对当前窗口进行截图保存(Ctrl+Alt+R)";
#ifdef Q_OS_MAC
    text="对当前窗口进行截图保存(command+Option+R)";
#endif
    action_ShowSaveScreen= new QAction(text,this);
    tablePopMenu->addAction(action_ShowCopyColum);
    tablePopMenu->addAction(action_ShowSaveScreen);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    connect(action_ShowSaveScreen, SIGNAL(triggered()), this, SLOT(saveScreen()));
    //截图快捷键
    QShortcut *_shortcut;
    _shortcut = new QShortcut(QKeySequence("Ctrl+Alt+R"), this);
    connect(_shortcut, SIGNAL(activated()),this,SLOT(saveScreen()));
    //复制快捷键-拦截系统的复制函数，使用我们写的
    QShortcut *_shortcut2;
    _shortcut2 = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(_shortcut2, SIGNAL(activated()),this,SLOT(copyToClipboard()));
    //设置表格列标题
    QStringList title;
    title.append("列描述");
    title.append("列内容");

    ptr_table->setHorizontalHeaderLabels(title);
    ptr_table->setWordWrap(true);
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

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
    // 在这里设置行高的时候，会出现很诡异的事情，某些行的高度变得非常高，所以改用定时器在页面加载完毕后再设置
    dataisOK=true;
    //bug修正,基于定时器在界面初始化完整之后再设置行高
    resizeFiletimer = new QTimer(this);
    connect(resizeFiletimer,SIGNAL(timeout()),this,SLOT(resizeHeight()));
    resizeFiletimer->start(1);
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
        Toast::showMsg(QString("已复制数据到剪切板~"), ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
}

DialogShowTableFieldCheck::~DialogShowTableFieldCheck()
{
    delete action_ShowCopyColum;
    delete tablePopMenu;
    delete ui;
}

void DialogShowTableFieldCheck::saveScreen(){
    QGuiApplication::primaryScreen();
    //获取截图
    QPixmap p = this->grab();
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
        //开始进行截图
        p.save(filePathName,"png");
    }
}


void  DialogShowTableFieldCheck:: resizeHeight(){
    for (int row = 0; row < ptr_table->rowCount();row++)
    {
        ptr_table->resizeRowToContents(row);
    }
    //仅使用一次定时器,后续通过窗口resizeEvent调整
    resizeFiletimer->stop();
}

void DialogShowTableFieldCheck:: resizeEvent (QResizeEvent * event ){

    UNUSED(event);
    if(dataisOK){
        for (int row = 0; row < ptr_table->rowCount();row++)
        {
            ptr_table->resizeRowToContents(row);
        }
    }
}
