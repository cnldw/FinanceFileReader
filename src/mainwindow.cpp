/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/fixedfielddefinition.h"
#include "src/ofdfielddefinition.h"
#include "src/dialogshowtablerow.h"
#include "src/dialogchoosefiletype.h"
#include "src/dialogshowtablefieldcheck.h"
#include "src/dialogshowtablecompareview.h"
#include "src/dialogmytip.h"
#include "src/dialogmodifycell.h"
#include "src/dialogaboutauthor.h"
#include "src/dialogaboutthis.h"
#include "src/dialogmergetip.h"
#include "src/dialogpreference.h"
#include "src/dialogmodifyrow.h"
#include "src/createofdwindow.h"
#include "src/dialogchooseexporttype.h"
#include "src/dialogshowcharset.h"
#include "src/dialogmodifymtime.h"
#include "src/dialogmagnify.h"
#include "src/dialogchooseofdconfig.h"
#include "src/dialogchoosedbfconfig.h"
#include "src/dialogeditheaderfooter.h"
#include "src/dialogforcenumber.h"
#include "src/dialogshareqrcode.h"
#include "src/qdbf/qdbfrecord.h"
#include "src/qdbf/qdbffield.h"
#include "src/dbffielddefinition.h"
#include "src/dialogoktools.h"
#include "src/msgtoast.h"
#include "src/dialogshowimportexcelerror.h"
#include "src/dialogshowfieldchecklist.h"
#include "src/fieldcheckitem.h"
#include "src/dialogshowfieldchecklist.h"
#include "src/dialogchoosefieldcheckexportreport.h"
#include "src/formfieldcheckedittools.h"
#include <private/qzipreader_p.h>
#include "src/dialogchoosefilefromzip.h"

#define UNUSED(x) (void)x

MainWindow::MainWindow(int argc, char *argv[],QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle(appName);

#ifdef Q_OS_LINUX
    ui->actiondifftools->setVisible(false);
    ui->actionfileedit->setVisible(false);
#endif
#ifdef Q_OS_MAC
    ui->actiondifftools->setVisible(false);
    ui->actionfileedit->setVisible(false);
#endif

    ui->menudbftools->menuAction()->setVisible(false);
    ui->lineTextText->setToolTip("输入你要搜索的内容,数值类搜索请不要输入千位分隔符~~");
    ui->framePage->setVisible(false);
    ptr_table=ui->tableWidget;
    ptr_table->setAlternatingRowColors(true);
    tableHeight=ptr_table->height();
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setFocus();

    tablePopMenu = new QMenu(ptr_table);
    action_ShowDetails = new QAction(tr("查看此行记录"),this);
    connect(action_ShowDetails, SIGNAL(triggered()), this, SLOT(showRowDetails()));
    action_ShowCopyColum = new QAction(tr("复制"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboardWithoutTitle()));
    action_ShowCopyColumWithTitle = new QAction(tr("复制(带字段标题)"),this);
    connect(action_ShowCopyColumWithTitle, SIGNAL(triggered()), this, SLOT(copyToClipboardWithTitle()));
    action_Magnify = new QAction(tr("放大镜"),this);
    connect(action_Magnify, SIGNAL(triggered()), this, SLOT(showMagnify()));
    action_ShowOFDAnalysis = new QAction(tr("OFD/类OFD/字段定长字段合法分析"),this);
    connect(action_ShowOFDAnalysis, SIGNAL(triggered()), this, SLOT(showOFDOrFixedFiledAnalysis()));
    action_EditCompareData= new QAction(tr("将此行数据加入比对器"),this);
    connect(action_EditCompareData, SIGNAL(triggered()), this, SLOT(editCompareData()));
    action_EditCompareDataBatch= new QAction(tr("将选择的行批量加入比对器"),this);
    connect(action_EditCompareDataBatch, SIGNAL(triggered()), this, SLOT(editCompareData()));
    action_ModifyCell= new QAction(tr("编辑光标所在单元格"),this);
    connect(action_ModifyCell, SIGNAL(triggered()), this, SLOT(showModifyCell()));
    action_ModifyCellBatch= new QAction(tr("批量编辑选中单元格"),this);
    connect(action_ModifyCellBatch, SIGNAL(triggered()), this, SLOT(showModifyCellBatch()));
    action_DeleteRowData= new QAction(tr("从文件中删除光标所在行的整行记录"),this);
    connect(action_DeleteRowData, SIGNAL(triggered()), this, SLOT(deleteRowDataFromFileAndTable()));
    action_CopyRowData= new QAction(tr("复制光标所在整行原始数据"),this);
    connect(action_CopyRowData, SIGNAL(triggered()), this, SLOT(copyRowData()));
    action_addCopyedData2PreviousRow= new QAction(tr("将剪切板上的数据插入到此行上方"),this);
    connect(action_addCopyedData2PreviousRow, SIGNAL(triggered()), this, SLOT(addRowDataPreviousRow()));
    action_addCopyedData2NextRow= new QAction(tr("将剪切板上的数据插入到此行下方"),this);
    connect(action_addCopyedData2NextRow, SIGNAL(triggered()), this, SLOT(addRowDataNextRow()));
    action_addCopyedData2End= new QAction(tr("将剪切板上的数据插入到文件尾"),this);
    connect(action_addCopyedData2End, SIGNAL(triggered()), this, SLOT(addRowDataEndRow()));
    action_addNewLineData2End= new QAction(tr("新增一行数据记录到文件尾"),this);
    connect(action_addNewLineData2End, SIGNAL(triggered()), this, SLOT(addNewLineRowDataEndRow()));
    action_ModifyRow= new QAction(tr("编辑光标所在的整行数据"),this);
    connect(action_ModifyRow, SIGNAL(triggered()), this, SLOT(showMoaifyRow()));
    action_ShowCharacter = new QAction(tr("十六进制字符编码透视(乱码分析)"),this);
    connect(action_ShowCharacter, SIGNAL(triggered()), this, SLOT(showCharacter()));
    action_CsvForceNumber= new QAction(tr("对此列调整数据格式"),this);
    connect(action_CsvForceNumber, SIGNAL(triggered()), this, SLOT(forceNumber()));
    action_ShareUseQrCode = new QAction(tr("使用二维码分享此单元格"),this);
    connect(action_ShareUseQrCode, SIGNAL(triggered()), this, SLOT(showQrcode()));

    //初始化tips，pua字符清单等其他东西
    init_Others();
    //哪些数据类型当作数字处理//////
    dbfIsNumberFieldType.append(QDbf::QDbfField::FloatingPoint);
    dbfIsNumberFieldType.append(QDbf::QDbfField::Number);
    dbfIsNumberFieldType.append(QDbf::QDbfField::Integer);
    dbfIsNumberFieldType.append(QDbf::QDbfField::Currency);

    //监控表格进度条的变化
    connect (ptr_table->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(acceptVScrollValueChanged(int)));
    //Excel导入处理进度
    connect(this,&MainWindow::signals_update_import_excel_status,this,&MainWindow::update_import_excel_status);
    connect(this,&MainWindow::signals_update_zip_extract_status,this,&MainWindow::update_zip_extract_status);
    //开始初始化状态栏
    initStatusBar();
    //开始进行配置加载
    //加载程序设置
    load_Setting();
    /*加载完设置后进行设置应用*/
    //默认页面视图模式
    if(this->defaultViewMode==1){ui->frameInfo->setHidden(true);ui->viewMode->setText("标准视图");}
    if(this->defaultPageSizeType==0){ui->labelPageSize->setText("(10万行/页)");this->pageRowSize=100000;}
    else if(this->defaultPageSizeType==1){ui->labelPageSize->setText("(20万行/页)");this->pageRowSize=200000;}
    else if(this->defaultPageSizeType==2){ui->labelPageSize->setText("(50万行/页)");this->pageRowSize=500000;}
    else if(this->defaultPageSizeType==3){ui->labelPageSize->setText("(100万行/页)");this->pageRowSize=1000000;}
    else if(this->defaultPageSizeType==4){ui->labelPageSize->setText("(200万行/页)");this->pageRowSize=2000000;}
    else{ui->labelPageSize->setText("(10万行/页)");this->pageRowSize=100000;}
    //加载web平台插件
    load_PluginList();
    //加载其他配置
    loadConfig();
    //随机提醒
    randomTips();
    //设置字体
    Utils::setDefaultWindowFonts(this,true);
    Utils::setDefaultWindowFonts(statusLabel_ptr_showMessage);
    this->repaint();

    //判断是否启动时读取文件
    //获取启动参数--如果参数中有文件路径，则稍后在窗口初始化完毕后打开文件
    //需要注意的是，程序启动时，窗口还未初始化完毕的时候打开文件会出现问题，所以这里只加载文件的路径，不打开文件，打开文件的动作放到窗口的resizeEvent事件中去
    if(argc>1){
        //读取第一个可读取的文件参数
        for(int i=1;i<argc;i++){
            if(Utils::isFileExist(QString::fromLocal8Bit(argv[i]))&&(QString::fromLocal8Bit(argv[i])!=QCoreApplication::applicationFilePath()))
            {
                startUpOrDragfile=QString::fromLocal8Bit(argv[i]);
                break;
            }
        }
    }
    //启动一个定时器在窗口启动完毕后检查是否需要在启动程序时读取文件
    loadFiletimer = new QTimer(this);
    //初始化定时器
    connect(loadFiletimer,SIGNAL(timeout()),this,SLOT(loadFileOnWindowisOpenOrDragEnter()));
    //每100ms检测一次--并且在程序退出前一直工作
    loadFiletimer->start(100);
    //关联后台线程导出xlsx和前台线程接受结果的曹函数
    watcherXlsxSaveStatus_ = new QFutureWatcher<int>;
    connect(watcherXlsxSaveStatus_, &QFutureWatcher<int>::finished,this, &MainWindow::save2XlsxFinished);
    watcherXlsxImportStatus_ = new QFutureWatcher<int>;
    connect(watcherXlsxImportStatus_, &QFutureWatcher<int>::finished,this, &MainWindow::importFromXlsxFinished);
    watcherZipExtractStatus_ = new QFutureWatcher<int>;
    connect(watcherZipExtractStatus_, &QFutureWatcher<int>::finished,this, &MainWindow::extractZipFinished);
}

/**
 * @brief MainWindow::loadFileOnWindowisOpen 监视是否需要加载文件，被定时器定时调用的一个函数，用于随程序启动打开文件和拖拽打开文件
 *
 */
void MainWindow::loadFileOnWindowisOpenOrDragEnter(){
    if(!startUpOrDragfile.isEmpty()){
        //如果没有忽略，则直接返回
        if(!ignoreFileChangeAndOpenNewFile()){
            return;
        }
        //windows系统时,如果路径里的斜杠是\有时会有问题，替换为/
#ifdef Q_OS_WIN32
        startUpOrDragfile=startUpOrDragfile.replace("\\","/");
#endif
        QString filePath=startUpOrDragfile;
        //置空此变量，防止重复加载
        startUpOrDragfile="";
        isUpdateData=true;
        initFile(filePath);
        isUpdateData=false;
    }
}

/**
 * @brief MainWindow::initStatusBar 状态栏初始化，我们将状态栏切分为几块，分别用于显示文件记录数，文件行列等信息，实际往状态栏写数据显示不使用本方法
 */
void MainWindow::initStatusBar(){
    //显示总记录数的标签
    statusLabel_ptr_showCount = new QLabel;
    statusLabel_ptr_showCount->setMinimumSize(300, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showCount);
    statusLabel_ptr_showCount->setText(tr("记录数:0,共计0页"));
    //显示行列的标签
    statusLabel_ptr_showRowAndCol = new QLabel;
    statusLabel_ptr_showRowAndCol->setMinimumSize(200, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showRowAndCol);
    statusLabel_ptr_showRowAndCol->setText(tr("文件内0行0列"));
    statusLabel_ptr_showRowAndCol->setToolTip(tr("此处显示当前选择的字段在原文件中的行和列(每个中文字符计两列)"));
    //显示通用消息的标签
    statusLabel_ptr_showMessage = new QLabel;
    statusLabel_ptr_showMessage->setMinimumSize(500, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showMessage);
    statusLabel_ptr_showMessage->setText(tr(""));
    statusLabel_ptr_showMessage->setToolTip(tr("此处显示各种信息,可右键复制"));
    //设置自定义菜单
    showMessagePopMenu=new QMenu(statusLabel_ptr_showMessage);
    action_ShowCopy = new QAction(tr("复制到剪切板"),this);
    connect(action_ShowCopy, SIGNAL(triggered()), this, SLOT(copyMessage()));
    action_GoToFirstNotNumberLine = new QAction(tr("跳转到第1个非法数值的位置"),this);
    connect(action_GoToFirstNotNumberLine, SIGNAL(triggered()), this, SLOT(gotoFirstNotNumber()));
    //开启自定义菜单并进行信号绑定
    statusLabel_ptr_showMessage->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(statusLabel_ptr_showMessage, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMessage_customContextMenuRequested(QPoint)));
}

/**
 * @brief MainWindow::~MainWindow 析构函数
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete statusLabel_ptr_showCount;
    statusLabel_ptr_showCount=nullptr;
    delete statusLabel_ptr_showRowAndCol;
    statusLabel_ptr_showRowAndCol=nullptr;
    delete statusLabel_ptr_showMessage;
    statusLabel_ptr_showMessage=nullptr;
    delete ptr_table;
    ptr_table=nullptr;
    delete xlsx;
    xlsx=nullptr;
    delete loadFiletimer;
    loadFiletimer=nullptr;
    delete watcherXlsxSaveStatus_;
    watcherXlsxSaveStatus_=nullptr;
    delete watcherXlsxImportStatus_;
    watcherXlsxImportStatus_=nullptr;
    delete watcherZipExtractStatus_;
    watcherZipExtractStatus_=nullptr;
}

/**
 * @brief MainWindow::dragEnterEvent 拖拽支持
 * @param event
 */
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
    }
}

/**
 * @brief MainWindow::dropEvent 拖拽支持
 * @param event
 */
void MainWindow::dropEvent(QDropEvent *event)
{
    if(!configLoadCompleted){
        statusBar_disPlayMessage(tr("正在加载配置,稍后再拖入文件试试..."));
        return;
    }
    //获取拖拽来源的文件信息
    /////////////////////////////////////////////////////////////
    QList<QUrl> urls = event->mimeData()->urls();
    //判断是否包含文件夹,同时获取下参数串，方便稍后传给对比工具，如有需要
    QStringList pars;
    for(int i=0;i<urls.count();i++){
        pars.append(urls.at(i).toLocalFile());
        if(QFileInfo(urls.at(i).toLocalFile()).isDir()){
            statusBar_disPlayMessage(tr("不接受读取文件夹,请不要拖放文件夹进来哟..."));
            return;
        }
    }
    //判断是否进入文件比对功能-拖进来的文件数大于1
    if(urls.count()>1){
        //仅仅windows系统支持文件对比
#ifdef Q_OS_WIN32
        DialogMyTip dialog2("同时拖放了多个文件进来，你需要对比这些文件的差异么？,如果你不想使用对比工具对比文件而是要解析文件，请每次拖放一个文件进来！",this);
        dialog2.setWindowTitle("警告！");
        dialog2.setModal(true);
        dialog2.exec();
        if(dialog2.getBoolFlag()){
            //调用打开Winmerge然后return
            //拼接调用路径，包含文件路径
            QString winmergepath=QApplication::applicationDirPath()+"/plugin/WinMerge/WinMergeU.exe";
            if(Utils::isFileExist(winmergepath)){
                QProcess::startDetached(winmergepath,pars);
            }else{
                statusBar_disPlayMessage("找不到WinMerge插件,请安装到plugin目录下！");
            }
            return;
        }
        else{
            //否,提示用户
            statusBar_disPlayMessage("请拖放一个文件进来以解析该文件！");
            return;
        }
#endif
        //非windows系统，提示不要拖放多个文件
        statusBar_disPlayMessage(tr("拖进来一个文件试试~,文件太多啦"));
        return;
    }
    //仅有一个参数且是文件
    QString fileNameFromDrag = urls.first().toLocalFile();
    //////////////////////////////////////////////////////////////
    //开始判断如何打开文件
    if(defaultNewFileMode==1){
        //windows特殊处理
#ifdef Q_OS_WIN32
        fileNameFromDrag=fileNameFromDrag.replace("\\","/");
#endif
        //判断是否是同一个文件--不是同一个--则新实例打开
        if(fileNameFromDrag!=currentOpenFilePath&&!currentOpenFilePath.isEmpty()&&currentOpenFileType!=openFileType::NotFileOrErr){
            qDebug()<<"即将启动新的实例,从"<<QCoreApplication::applicationFilePath();
            QString program=QCoreApplication::applicationFilePath();
            QStringList arguments;
            arguments<<(QCoreApplication::applicationFilePath())<<fileNameFromDrag;
            QProcess::startDetached(program, arguments);
            return;
        }
    }
    //只要是新文件直接新实例打开
    else if (defaultNewFileMode==2&&!currentOpenFilePath.isEmpty()&&currentOpenFileType!=openFileType::NotFileOrErr){
        qDebug()<<"即将启动新的实例,从"<<QCoreApplication::applicationFilePath();
        QString program=QCoreApplication::applicationFilePath();
        QStringList arguments;
        arguments<<(QCoreApplication::applicationFilePath())<<fileNameFromDrag;
        QProcess::startDetached(program, arguments);
        return;
    }
    //如果未在新实例打开-则进入在本实例打开的判断流程----
    //如果在当前实例打开，需要判断是不是当前文件修改了未保存
    if(!ignoreFileChangeAndOpenNewFile()){
        return;
    }
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再拖放文件进来...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    //清除旧数据//文件基础检测通过，进行文件加载
    this->ui->currentOpenFilePathLineText->setText("");
    fileChanged=false;
    this->setWindowTitle(appName);
    //首先清除原来的显示信息
    clear_OldData();
    clear_Display_Info();
    clear_Table_Info();
    //升起窗口
    this->activateWindow();
    this->raise();
    if (!fileNameFromDrag.isEmpty())
    {
        //交给定时任务去加载文件
        startUpOrDragfile=fileNameFromDrag;
    }
}

/**
 * @brief MainWindow::reCalculateTableBeginAndEnd
 */
void  MainWindow::reCalculateTableBeginAndEnd(){
    if(!isUpdateData){
        int currentTableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
        int currentTableRowEnd=ptr_table->rowAt(ptr_table->verticalScrollBar()->y()+ptr_table->size().height());
        //无需加载
        if(currentTableRowBegin<0&&currentTableRowEnd<0){
            return;
        }
        //修正--滚动到最下面时这个值可能是-1
        if(currentTableRowEnd<0){
            currentTableRowEnd=ptr_table->rowCount()-1;
        }
        //当前表格可见区域有多少行
        int tableDisplayRow=currentTableRowEnd-currentTableRowBegin+1;
        //当已加载数据达到20页时,清理一次全部数据
        if(rowHasloaded.count()>tableDisplayRow*20){
            qDebug()<<"对加载记录进行全清理";
            ptr_table->clearContents();
            rowHasloaded.clear();
            //重设0,下面再计算要开始和结束的位置
            tableRowBegin=0;
            tableRowEnd=0;
        }
        //如果本次要显示的范围已经在上一次的范围内则不再刷新，降低滚动进度条时的调用填充数据函数的频率
        if(currentTableRowBegin>=tableRowBegin&&currentTableRowEnd<=tableRowEnd){
            //qDebug()<<"本页数据已加载,跳过";
            return;
        }
        else{
            //qDebug()<<"开始执行新的加载任务";
            int maxRow=ptr_table->rowCount()-1;
            //新的起始位置和结束位置,上下各2倍冗余,一次共加载5页,免得进度条稍微滚动又要刷新
            tableRowBegin=(currentTableRowBegin-tableDisplayRow*2)<0?0:(currentTableRowBegin-tableDisplayRow*2);
            tableRowEnd=(currentTableRowEnd+tableDisplayRow*2)>maxRow?maxRow:(currentTableRowEnd+tableDisplayRow*2);
            //qDebug()<<"本次加载记录数"<<tableRowEnd-tableRowBegin+1;
            //不同数据类型插入点
            if(currentOpenFileType==openFileType::OFDFile){
                display_OFDTable();
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                display_CSVTable();
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                display_FIXEDTable();
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                display_DBFTable();
            }
        }
    }
}

/**
 * @brief MainWindow::resizeEvent窗口缩放处理事件,当窗口缩放时，界面能显示的内容变化，需要重新调用display函数去显示数据
 * @param event
 */
void MainWindow:: resizeEvent (QResizeEvent * event ){
    event->ignore();
    reCalculateTableBeginAndEnd();
}

/**
 * @brief MainWindow::acceptVScrollValueChanged主界面表格滚动事件，滚动时判断当前显示范围，实现懒加载
 * @param value
 */
void MainWindow::acceptVScrollValueChanged(int value)
{
    UNUSED (value);
    reCalculateTableBeginAndEnd();
}

/**
 * @brief MainWindow::statusBar_clear_statusBar状态栏信息清理还原
 */
void MainWindow::statusBar_clear_statusBar(){
    statusLabel_ptr_showCount->setText(tr("记录数:0,共计0页"));
    statusLabel_ptr_showRowAndCol->setText(tr("文件内0行0列"));
    statusLabel_ptr_showMessage->setText(nullptr);
}

/**
 * @brief MainWindow::open_file_Dialog打开文件的弹窗
 */
void MainWindow::open_file_Dialog(){
    //如果没有忽略，则直接返回
    if(!ignoreFileChangeAndOpenNewFile()){
        return;
    }
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再打开新的文件...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    dataBlocked=true;
    //1.8.6开始,打开文件路径默认为桌面
    QString openpath=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    if(!currentOpenFilePath.isEmpty()){
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
        }
    }
    QString  file = QFileDialog::getOpenFileName(this, tr("打开"),openpath , tr("所有文件(*.*)"));
    if(file.length() != 0) {
        isUpdateData=true;
        initFile(file);
        isUpdateData=false;
    } else {
        //放弃了读取文件
    }
    dataBlocked=false;
}

/**
 *0 展示检查结果
 *1 导出检查结果
 */
void MainWindow::primaryCheck(int type,QString fileName,int exportType,int openatexported){
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    primaryFirstRow.clear();
    primaryCheckResult.clear();
    needReCheckPrimary=false;
    QList <uint> primaryKeyFieldList;
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    else if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    else if(currentOpenFileType==openFileType::DBFFile){statusBar_disPlayMessage("不支持DBF文件使用此功能...");return;}
    else{
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的文件没有数据记录...");
            return;
        }
        else{
            if(currentOpenFileType==openFileType::OFDFile){primaryKeyFieldList=ofd.getPrimaryKeyFieldList();}
            else if(currentOpenFileType==openFileType::CSVFile){primaryKeyFieldList=csv.getPrimaryKeyFieldList();}
            else if(currentOpenFileType==openFileType::FIXEDFile){primaryKeyFieldList=fixed.getPrimaryKeyFieldList();}
        }
        if(primaryKeyFieldList.count()>0){
            //开始获取数据
            uint rowCount=commonContentQByteArrayList.count();
            for (uint row=0;row<rowCount;row++){
                QStringList rowdata;
                if(currentOpenFileType==openFileType::OFDFile){
                    rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                }
                else if(currentOpenFileType==openFileType::CSVFile){
                    rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row);
                }
                QString key;
                for (auto it = primaryKeyFieldList.begin(); it != primaryKeyFieldList.end(); ++it) {
                    if (key.isEmpty()){
                        key.append(rowdata.at(*it).isEmpty()?"NULL":rowdata.at(*it));
                    }
                    else{
                        key.append("-").append(rowdata.at(*it).isEmpty()?"NULL":rowdata.at(*it));
                    }
                }
                if(!primaryFirstRow.contains(key)){
                    primaryFirstRow.insert(key,row);
                }
                else{
                    primaryItem item;
                    item.primaryKey=key;
                    item.row=row;
                    item.firstRow=primaryFirstRow.value(key);
                    primaryCheckResult.append(item);
                }
                if((row%1000==0)||(row==rowCount-1)){
                    statusBar_disPlayMessage(QString("正在分析主键冲突中,请勿进行其他操作,已检查%1行").arg(QString::number(row+1)));
                    qApp->processEvents();
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
            //开始执行后续动作
            //显示检查结果
            if(type==0){
                if(primaryCheckResult.count()>500000){
                    statusBar_disPlayMessage("倒霉蛋,主键冲突数据好多哟,请使用“导出当前文件主键冲突检查报告”的方式查看...");
                }
                else if(primaryCheckResult.count()>0){
                    dialogDialogShowPrimaryCheck = new DialogShowPrimaryCheck(&primaryCheckResult,this);
                    connect(dialogDialogShowPrimaryCheck,&DialogShowPrimaryCheck::sendSignalsClosePrimaryCheck,this,&MainWindow::getSignalsClosePrimaryCheck);
                    connect(dialogDialogShowPrimaryCheck,&DialogShowPrimaryCheck::sendActionFromPrimaryCheckTools,this,&MainWindow::getActionFromPrimaryCheckTools);
                    dialogDialogShowPrimaryCheck->setWindowTitle("查看主键冲突检查报告");
                    dialogDialogShowPrimaryCheck->setModal(false);
                    dialogDialogShowPrimaryCheck->setAttribute(Qt::WA_DeleteOnClose);
                    dialogDialogShowPrimaryCheck->show();
                    dialogDialogShowPrimaryCheck->raise();
                    dialogDialogShowPrimaryCheck->activateWindow();
                    statusBar_disPlayMessage(QString("检查完毕,共计%1行主键冲突数据,请查看结果...").arg(QString::number(primaryCheckResult.count())));
                }
                else{
                    statusBar_disPlayMessage("检查完毕,没有主键冲突...");
                }
            }
            //导出检查报告
            else if(type==1){
                //开始写入数据
                QApplication::setOverrideCursor(Qt::WaitCursor);
                QFile data(fileName);
                if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
                    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
                    QString sb;
                    QString title="";
                    if(fileName.contains("/")){
                        title=fileName.mid(fileName.lastIndexOf("/")+1);
                        title=title.left(title.length()-0);
                    }else{
                        title=title.left(title.length()-0);
                    }
                    if(exportType==0){
                        //html文件头
                        sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
                        //内联的表格样式,内容太多,base64存储
                        sb.append(QByteArray::fromBase64("PHN0eWxlIHR5cGU9InRleHQvY3NzIj4KLnRhYmxlCnsKcGFkZGluZzogMDsKbWFyZ2luOiAwOwp9CnRoIHsKZm9udDogYm9sZCAxMnB4ICJUcmVidWNoZXQgTVMiLCBWZXJkYW5hLCBBcmlhbCwgSGVsdmV0aWNhLCBzYW5zLXNlcmlmOwpjb2xvcjogIzRmNmI3MjsKYm9yZGVyLXJpZ2h0OiAxcHggc29saWQgI0MxREFENzsKYm9yZGVyLWJvdHRvbTogMXB4IHNvbGlkICNDMURBRDc7CmJvcmRlci10b3A6IDFweCBzb2xpZCAjQzFEQUQ3OwpsZXR0ZXItc3BhY2luZzogMnB4Owp0ZXh0LXRyYW5zZm9ybTogdXBwZXJjYXNlOwp0ZXh0LWFsaWduOiBsZWZ0OwpwYWRkaW5nOiA2cHggNnB4IDZweCAxMnB4OwpiYWNrZ3JvdW5kOiAjQ0FFOEVBIG5vLXJlcGVhdDsKd29yZC1icmVhazoga2VlcC1hbGw7CndoaXRlLXNwYWNlOm5vd3JhcDsKfQp0ciB7CndvcmQtYnJlYWs6IGtlZXAtYWxsOwp3aGl0ZS1zcGFjZTpub3dyYXA7Cn0KdGQgewpib3JkZXItcmlnaHQ6IDFweCBzb2xpZCAjQzFEQUQ3Owpib3JkZXItYm90dG9tOiAxcHggc29saWQgI0MxREFENzsKZm9udC1zaXplOjE0cHg7CnBhZGRpbmc6IDJweCA2cHggMnB4IDZweDsKY29sb3I6ICM0ZjZiNzI7Cn0KLm51bWJlcnsKdGV4dC1hbGlnbjogcmlnaHQ7Cn0KPC9zdHlsZT4K"));
                        //标题表头
                        sb.append("<table>\r\n<tr><th>主键键值</th><th>数据行号</th><th>冲突行号</th></tr>\r\n");
                    }
                    else{
                        sb.append("主键键值\t数据行号\t冲突行号\r\n");
                    }
                    data.write(codec->fromUnicode(sb));
                    data.flush();
                    sb.clear();
                    ////开始获取数据
                    int rowCount=primaryCheckResult.count();
                    if(rowCount>0){
                        for (int row=0;row<rowCount;row++){
                            if(exportType==0){
                                sb.append("<tr><td>").append(primaryCheckResult.at(row).primaryKey).append("</td>");
                                sb.append("<td class=\"number\">").append(QString::number(primaryCheckResult.at(row).row+1)).append("</td>");
                                sb.append("<td class=\"number\">").append(QString::number(primaryCheckResult.at(row).firstRow+1)).append("</td></tr>\r\n");
                            }
                            else{
                                sb.append(primaryCheckResult.at(row).primaryKey);
                                sb.append("\t").append(QString::number(primaryCheckResult.at(row).row+1));
                                sb.append("\t").append(QString::number(primaryCheckResult.at(row).firstRow+1)).append("\r\n");
                            }
                            if((row%1000==0)||(row==rowCount-1)){
                                data.write(codec->fromUnicode(sb));
                                data.flush();
                                sb.clear();
                                statusBar_disPlayMessage(QString("主键冲突检查报告导出中,请勿进行其他操作,已写入%1行,共计%2行").arg(QString::number(row+1),QString::number(rowCount)));
                                qApp->processEvents();
                                if(abortExit){
                                    QApplication::restoreOverrideCursor();
                                    qDebug()<<"强制立即终止任务";
                                    return;
                                }
                            }
                        }
                    }
                    if(exportType==0){
                        sb.append("</table>\r\n</body>\r\n</html>");
                    }
                    data.write(codec->fromUnicode(sb));
                    data.flush();
                    data.close();
                    statusBar_disPlayMessage(tr("报告成功导出到%1").arg(fileName));
                    if(openatexported==1){
                        bool is_open = QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
                        if(!is_open)
                        {
                            statusBar_disPlayMessage("文件打开失败,请尝试手工打开~~~");
                            return;
                        }
                    }
                }
                else{
                    statusBar_disPlayMessage(tr("报告导出失败,请重试..."));
                }
                dataBlocked=false;
                isExportFile=false;
                QApplication::restoreOverrideCursor();
            }
        }
        else{
            statusBar_disPlayMessage(tr("没有配置适用于该文件的字段必填检查规则,请配置后重试..."));
        }
    }
}

/**
 * @brief MainWindow::statusBar_display_rowsCount状态栏显示总行数
 * @param rowsCount
 */
void MainWindow::statusBar_display_rowsCount(int rowsCount){
    int count=(rowsCount + pageRowSize - 1) / pageRowSize;
    statusLabel_ptr_showCount->setText(QString("记录数:%1行,共计%2页").arg(QString::number(rowsCount),QString::number(count)));
}

void MainWindow::statusBar_display_rowsCount(int rowsCount, QString charset){
    int count=(rowsCount + pageRowSize - 1) / pageRowSize;
    statusLabel_ptr_showCount->setText(QString("记录数:%1行,共计%2页-%3").arg(QString::number(rowsCount),QString::number(count),charset));
}

/**
 * @brief MainWindow::statusBar_display_rowsAndCol 状态栏显示行列，当length为-1时只显示行，当length为0时只显示行列
 * @param row
 * @param col
 * @param length
 */
void MainWindow::statusBar_display_rowsAndCol(int row,int col,int length){
    if(length==-1){
        statusLabel_ptr_showRowAndCol->setText(tr("源文件%1行").arg(QString::number(row)));
    }
    else if(length==0){
        statusLabel_ptr_showRowAndCol->setText("源文件"+QString::number(row)+"行,"+QString::number(col)+"列");
    }
    else{
        statusLabel_ptr_showRowAndCol->setText("源文件"+QString::number(row)+"行,"+QString::number(col)+"列,长度为"+QString::number(length));
    }
}

/**
 * @brief MainWindow::load_Setting 程序设置加载方法-读取程序配置文件，如果失败则创建一个默认的配置文件
 */
void MainWindow::load_Setting(){
    QString Settingini=Utils::getConfigPath()+"Setting.ini";
    QSettings loadedSettingInfoIni(Settingini,QSettings::IniFormat,nullptr);
    if(Utils::isFileExist(Settingini)){
        //加载ini文件
        //目前仅接收UTF-8编码的配置文件
        loadedSettingInfoIni.setIniCodec("UTF-8");
        loadedSettingInfoIni.beginGroup("setting");
        QStringList infoList =loadedSettingInfoIni.childKeys();
        loadedSettingInfoIni.endGroup();
        if(infoList.count()>0){
            for(int aaa=0;aaa<infoList.count();aaa++){
                QString key=infoList.at(aaa);
                QString value=loadedSettingInfoIni.value("setting/"+infoList.at(aaa)).toString();
                //读取配置
                //数据读取压缩等级
                if(key=="compresslevel"){
                    bool okflag;
                    int level=value.toInt(&okflag,10);
                    if(okflag){
                        if(level<-1){
                            this->dataCompressLevel=-1;
                        }
                        else if(level>9){
                            this->dataCompressLevel=9;
                        }
                        else{
                            this->dataCompressLevel=level;
                        }
                    }
                    else{
                        this->dataCompressLevel=0;
                    }
                }
                //默认视图模式
                if(key=="defaultviewmode"){
                    if(value!="0"&&value!="1"){
                        this->defaultViewMode=0;
                    }else{
                        this->defaultViewMode=value.toInt();
                    }
                }
                //默认新文件打开方式
                if(key=="defaultnewfilemode"){
                    if(value!="0"&&value!="1"&&value!="2"){
                        this->defaultNewFileMode=0;
                    }else{
                        this->defaultNewFileMode=value.toInt();
                    }
                }
                //默认每页行数类别
                if(key=="defaultpagesizetype"){
                    if(value!="0"&&value!="1"&&value!="2"&&value!="3"&&value!="4"){
                        this->defaultPageSizeType=0;
                    }else{
                        this->defaultPageSizeType=value.toInt();
                    }
                }
                //字段必填检查
                if(key=="enablefieldcheck"){
                    if(value=="1"){
                        this->checkFieldFlag=true;
                    }else{
                        this->checkFieldFlag=false;
                    }
                }
                if(key=="standfontsize"){
                    if(value=="1"){
                        this->standFontSize=1;
                    }else if(value=="2"){
                        this->standFontSize=2;
                    }
                    else {
                        this->standFontSize=0;
                    }
                }
                //无论是否加载到了字体大小设置，都要初始化一次
                setStandFontSize();
                QFontDatabase fontDatabase;
                //全局字体
                if(key=="firstuifont"){
                    if(!value.isEmpty()&&fontDatabase.families().contains(value)){
                        firstUIFontGlobal=value;
                    }
                }
                //生僻字补充字体
                if(key=="firstrarecharactersfont"){
                    if(!value.isEmpty()&&fontDatabase.families().contains(value)){
                        firstRareCharactersFontGlobal=value;
                    }
                }
            }
        }
    }else{
        QFile tempFile(Settingini);
        //创建初始化的设置文件
        if(tempFile.open(QFile::WriteOnly | QIODevice::Truncate))
        {
            QTextStream out(&tempFile);
            out<<"[setting]";
        }
        tempFile.close();
        //提取初始化参数写入配置文件
        QSettings loadedSettingInfoIni(Settingini,QSettings::IniFormat,nullptr);
        loadedSettingInfoIni.setIniCodec("UTF-8");
        loadedSettingInfoIni.beginGroup("setting");
        loadedSettingInfoIni.setValue("compresslevel",dataCompressLevel);
        loadedSettingInfoIni.setValue("defaultviewmode",defaultViewMode);
        loadedSettingInfoIni.setValue("defaultnewfilemode",defaultNewFileMode);
        loadedSettingInfoIni.setValue("defaultpagesizetype",defaultPageSizeType);
        loadedSettingInfoIni.setValue("enablefieldcheck","1");
        loadedSettingInfoIni.setValue("standfontsize","0");
        loadedSettingInfoIni.setValue("firstuifont","");
        loadedSettingInfoIni.setValue("firstrarecharactersfont","");
        loadedSettingInfoIni.endGroup();
        loadedSettingInfoIni.sync();
    }
}

void MainWindow::load_PluginList(){
#ifdef Q_OS_MAC
    return;
#endif
#ifdef Q_OS_LINUX
    return;
#endif
#ifdef Q_OS_WIN32
    //仅windows支持web插件平台
    QString path=QApplication::applicationDirPath()+"/plugin-web/plugin-list.json";
    if(Utils::isFileExist(path)){
        QFile loadFile(path);

        if(!loadFile.open(QIODevice::ReadOnly))
        {
            return;
        }

        QByteArray allData = loadFile.readAll();
        loadFile.close();

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));

        if(jsonError.error != QJsonParseError::NoError)
        {
            return;
        }
        QJsonObject rootObj = jsonDoc.object();
        if(rootObj.value("plugin").isArray()){
            QJsonArray array=rootObj.value("plugin").toArray();
            if(array.count()>0){
                for(int i=0;i<array.count();i++){
                    QJsonObject item=array.at(i).toObject();
                    QString name="";
                    QString path="";
                    if(item.value("name").isString()){
                        name=item.value("name").toString();
                    }
                    if(item.value("path").isString()){
                        path=item.value("path").toString();
                    }
                    if(!name.isEmpty()&&!path.isEmpty()){
                        qDebug()<<name<<path;
                    }
                    if(pluginpath.count()==0){
                        ui->menu_3->addSeparator();
                    }
                    //存储插件到插件列表
                    pluginpath.insert(i,path);
                    //建立action
                    QAction *action=new QAction(name,this);
                    //存储objectname为插件名|插件id,方便在槽函数中获取
                    action->setObjectName(name+"|"+QString::number(i));
                    ui->menu_3->addAction(action);
                    connect(action, SIGNAL(triggered()), this, SLOT(openPlugin()));
                }
            }
        }
    }
#endif
}


/**
 * @brief MainWindow::initFile 文件初始化方法 无论是拖拽进来的文件还是打开菜单打开的文件还是重新打开文件，最终都要从这个函数开始初始化加载文件，这个函数会对文件类型做个初步的判断，然后交付相对应的文件类别的加载方法
 * @param filePath
 */
void MainWindow::initFile(QString filePath, bool keepdbfDisplayType, bool keepdbfTrimType,bool fromZip){
    //重设开始时间
    time_Start = QDateTime::currentDateTime();
    currentOpenFileType=openFileType::NotFileOrErr;
    //设置当前打开的路径////////////////////////////////////////////
    currentOpenFilePath=filePath;
    ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
    statusBar_disPlayMessage("文件加载中...");
    /////////////////////////////////////////////////////////////
    //初始化文件时,检查配置是否加载完毕
    if(!configLoadCompleted){
        statusBar_disPlayMessage(tr("正在加载配置,稍后再拖入文件试试..."));
        return;
    }
    //恢复文件修改标志
    fileChanged=false;
    //首先清除原来的显示信息
    clear_OldData(keepdbfDisplayType,keepdbfTrimType);
    clear_Display_Info();
    clear_Table_Info();
    //获取完整的文件名
    //将windows下的反斜杠的路径替换为标准的右斜杠
#ifdef Q_OS_WIN32
    currentOpenFilePath=currentOpenFilePath.replace("\\","/");
#endif
    QFileInfo info(currentOpenFilePath);
    currentFileName=info.fileName();
    QString currentFileDir=info.absoluteDir().path();
    QString currentFileBaseName=info.baseName();
    //文件中有多个.时,baseName只有第一个.之前的部分
    if(currentFileName.contains(".")){
        int last = currentFileName.lastIndexOf (".");
        currentFileBaseName = currentFileName.left (last);
    }
    //去除baseName的开头的.
    while (true){
        if(currentFileBaseName.startsWith(".")){
            currentFileBaseName=currentFileBaseName.right(currentFileBaseName.length()-1);
        }
        else{
            break;
        }
        if (currentFileBaseName.length()==0){
            currentFileBaseName="未知文件名";
            break;
        }
    }
    if(!fromZip){
        fileFromZip=false;
    }
    this->setWindowTitle(appName+"-"+currentFileName);
    //前置拦击-常用压缩包
    if(currentFileName.endsWith(".zipx",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".rar",Qt::CaseInsensitive)||currentFileName.endsWith(".7z",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".tar",Qt::CaseInsensitive)||currentFileName.endsWith(".gz",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".tar.gz",Qt::CaseInsensitive)||currentFileName.endsWith(".tgz",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".bz",Qt::CaseInsensitive)||currentFileName.endsWith(".bz2",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".tbz",Qt::CaseInsensitive)||currentFileName.endsWith(".tbz2",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".xz",Qt::CaseInsensitive)||currentFileName.endsWith(".txz",Qt::CaseInsensitive)){
        statusBar_disPlayMessage("非zip格式的压缩文件请解压后解析!");
        currentOpenFileType=openFileType::NotFileOrErr;
        return;
    }
    //前置拦击-常用音频视频文件
    if(currentFileName.endsWith(".aac",Qt::CaseInsensitive)||
            currentFileName.endsWith(".avi",Qt::CaseInsensitive)||
            currentFileName.endsWith(".mid",Qt::CaseInsensitive)||
            currentFileName.endsWith(".mid",Qt::CaseInsensitive)||
            currentFileName.endsWith(".mp3",Qt::CaseInsensitive)||
            currentFileName.endsWith(".oga",Qt::CaseInsensitive)||
            currentFileName.endsWith(".wav",Qt::CaseInsensitive)||
            currentFileName.endsWith(".3gp",Qt::CaseInsensitive)||
            currentFileName.endsWith(".3g2",Qt::CaseInsensitive)||
            currentFileName.endsWith(".mpeg",Qt::CaseInsensitive)||
            currentFileName.endsWith(".ogv",Qt::CaseInsensitive)||
            currentFileName.endsWith(".ogv",Qt::CaseInsensitive)||
            currentFileName.endsWith(".webm",Qt::CaseInsensitive)||
            currentFileName.endsWith(".mp4",Qt::CaseInsensitive)||
            currentFileName.endsWith(".mkv",Qt::CaseInsensitive)||
            currentFileName.endsWith(".wmv",Qt::CaseInsensitive)){
        statusBar_disPlayMessage("不支持解析音视频文件!");
        currentOpenFileType=openFileType::NotFileOrErr;
        return;
    }
    //前置拦击-常用图像文件
    if(currentFileName.endsWith(".bmp",Qt::CaseInsensitive)||
            currentFileName.endsWith(".gif",Qt::CaseInsensitive)||
            currentFileName.endsWith(".ico",Qt::CaseInsensitive)||
            currentFileName.endsWith(".jpeg",Qt::CaseInsensitive)||
            currentFileName.endsWith(".jpg",Qt::CaseInsensitive)||
            currentFileName.endsWith(".png",Qt::CaseInsensitive)||
            currentFileName.endsWith(".svg",Qt::CaseInsensitive)||
            currentFileName.endsWith(".tif",Qt::CaseInsensitive)||
            currentFileName.endsWith(".webp",Qt::CaseInsensitive)){
        statusBar_disPlayMessage("不支持解析图像文件!");
        currentOpenFileType=openFileType::NotFileOrErr;
        return;
    }
    //其他
    if(currentFileName.endsWith(".exe",Qt::CaseInsensitive)||currentFileName.endsWith(".xls",Qt::CaseInsensitive)||currentFileName.endsWith(".xlsx",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".doc",Qt::CaseInsensitive)||currentFileName.endsWith(".docx",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".ppt",Qt::CaseInsensitive)||currentFileName.endsWith(".pptx",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".html",Qt::CaseInsensitive)||currentFileName.endsWith(".sql",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".pdf",Qt::CaseInsensitive)){
        statusBar_disPlayMessage("不支持解析此类文件!");
        currentOpenFileType=openFileType::NotFileOrErr;
        return;
    }
    //1.10.4版本开始支持直接读取zip文件
    if(currentFileName.endsWith(".zip",Qt::CaseInsensitive)){
        QFile file(currentOpenFilePath);
        if (!file.open(QIODevice::ReadOnly)){
            statusBar_disPlayMessage(QString("无法打开zip文件[%1],请重试...").arg(currentOpenFilePath));
            return;
        }
        QZipReader reader(&file);
        if (!reader.isReadable()){
            statusBar_disPlayMessage(QString("无法读取zip文件[%1],请重试...").arg(currentOpenFilePath));
            return;
        }
        const auto fileList = reader.fileInfoList();
        int fileCount=0;
        for (auto it = fileList.begin(); it != fileList.end(); ++it){
            if (it->isFile || (!it->isDir && !it->isFile && !it->isSymLink)){\
                fileCount++;
            }
        }
        reader.close();
        if(fileCount>0){
            DialogChooseFileFromZip  dialog(currentOpenFilePath,this);
            dialog.setWindowTitle(QString("从压缩包[%1]中选择需要解析的文件(不支持带密码或加密压缩包/超大压缩包)").arg(currentFileName));
            dialog.setModal(true);
            dialog.exec();
            dialog.raise();
            int index=dialog.getChooeseIndex();
            zipFileInfoList=dialog.getZipFileInfoList();
            zipFileListMdTime=dialog.getZipFileListMdTime();
            zipExtractAll=dialog.getExtractAll();
            zipFileCodec=dialog.getZipFileUseCodec();
            zipFileNameSystemCodecList=dialog.getZipFileNameSystemCodecList();
            if(!dialog.getConfirm()){
                zipfilePath="";
                zipTargetFileInZip="";
                zipExtractAll=false;
                zipExtractSucess=false;
                zipOutPutFileDir="";
                statusBar_disPlayMessage("没有从压缩包中选择需要解析的文件,放弃解析...");
                return;
            }
            else{
                zipfilePath=currentOpenFilePath;
                zipFileName=currentFileName;
                zipTargetFileInZip=zipFileInfoList.at(index).filePath;
                zipOutPutFileDir=currentFileDir+"/FFReader自动解压/"+currentFileBaseName+"/";
                dataBlockedMessage="正在解压缩文件,请耐心等待解压完成后再进行其他操作...";
                statusBar_disPlayMessage(dataBlockedMessage);
                dataBlocked=true;
                auto future = QtConcurrent::run(this, &MainWindow::extractFileFromZipFile);
                watcherZipExtractStatus_->setFuture(future);
                return;
            }
        }
        else{
            return;
        }
    }
    /*读取一行识别一下文件换行符*/
    QFile file(currentOpenFilePath);
    if (file.open(QFile::ReadOnly))
    {
        QString qstr=file.readLine();
        qDebug()<<qstr;
        if(qstr.endsWith("\r\n")){
            currentFileNewLineType=newLineType::CRLF;
        }
        else if(qstr.endsWith("\n")) {
            currentFileNewLineType=newLineType::LF;
        }
        else if(qstr.endsWith("\r")) {
            currentFileNewLineType=newLineType::CR;
        }
        else{
            currentFileNewLineType=newLineType::None;
        }
        file.close();
    }

    /*
        文件判断逻辑，先判断是不是OFD，再判断是不是索引，如果都校验失败，则进入非OF体系的判断
        在明确的确定下，则return完成判断，不明确的情况下，则goto到通用的csv和定长文件判断逻辑去
        v1.9.7 dbf--先判断是否dbf,dbf后缀的直接交给dbf初始化函数
    */
    if(currentFileName.endsWith(".dbf",Qt::CaseInsensitive)){
        load_dbfFile();
        return;
    }
    //字段必填检查报告--跳转到非OFD走自动识别
    if(currentFileName.contains("字段必填检查报告")||currentFileName.contains("主键冲突检查报告")){
        goto NOT_OF_FILE;
    }
    ////////////////OFD数据文件的判断////////////////////////////
    if(currentFileName.startsWith("OFD",Qt::CaseInsensitive)){
        //开始拆解文件名
        QString fixName=currentFileName;
        //FFreader导出的csv文件，直接跳转到非OFD文件去解析
        if(fixName.endsWith(".csv")){
            goto NOT_OF_FILE;
        }
        if(fixName.contains(".",Qt::CaseInsensitive)){
            fixName=fixName.mid(0,fixName.indexOf("."));
        }
        //OFD文件名处理完毕后，开始拆解文件名
        QStringList nameList=fixName.split("_");
        //正常的OFD文件应该至少有5段信息组成,另外中登TA和管理人交互的文件还有批次号,如果不是，则跳转到非OF文件检查
        if(nameList.count()<5){
            goto NOT_OF_FILE;
        }
        else{
            /*开始初步分析文件*/
            //从文件头获取各种信息
            //Code
            QString sendCode=nameList.at(1);
            QString recCode=nameList.at(2);
            QString dateInfo=nameList.at(3);
            QString fileTypeCode=nameList.at(4);
            //从配置文件获取名称信息
            QString sendName=(loadedOfdCodeInfo.value(sendCode)).getName();
            QString recName=(loadedOfdCodeInfo.value(recCode)).getName();
            //刷新UI
            ui->lineEditSendCode->setText(sendCode);
            ui->lineEditRecCode->setText(recCode);
            ui->labelFileTransferDate->setText("传递日期:");
            ui->lineEditFileTransferDate->setText(dateInfo);
            ui->lineEditFileType->setText(fileTypeCode);
            ui->lineEditSenfInfo->setText(sendName);
            ui->lineEditRecInfo->setText(recName);
            //判断是否是ok文件
            if(currentOpenFilePath.endsWith(".OK",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("这是一个OFD文件的OK文件,请解析原始数据文件...");
                return;
            }
            //判断是否是恒生系统sqllder导入时生成的bad文件
            if(currentOpenFilePath.endsWith(".BAD",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("这是一个sqlldr导入生成的bad文件,请解析原始数据文件...");
                return;
            }
            //判断是否是恒生系统sqllder导入时的控制文件
            if(currentOpenFilePath.endsWith(".CTL",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("是一个sqlldr导入使用的控制文件,请解析原始数据文件...");
                return;
            }
            //判断是否是快捷方式
            if(currentOpenFilePath.endsWith(".LNK",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("我没猜错的话这是一个文件快捷方式,请解析原始数据文件...");
                return;
            }
            //记录从文件里读取的文件发送信息
            //此处开始加载OFD数据文件
            load_ofdDataFile(fileTypeCode,sendCode,recCode);
            return;
        }
    }
    /////////////判断索引文件///////////
    if(currentFileName.length()>10&&currentFileName.startsWith("OF",Qt::CaseInsensitive))
    {
        //开始解析非OFD文件
        //检索到了文件头标识
        //开始拆解文件名
        QString fixName=currentFileName;
        if(fixName.contains(".",Qt::CaseInsensitive)){
            fixName=fixName.mid(0,fixName.indexOf("."));
        }
        //索引文件名处理完毕后，开始拆解文件名
        QStringList nameList=fixName.split("_");
        //OFX_发送方_接受方_日期.TXT
        //OFX_发送方_接受方_日期_批次号.TXT
        //正常的OFD索引文件应该至少有4段信息组成
        if(nameList.count()<4){
            goto NOT_OF_FILE;
        }
        if(!loadedOfdIndexFileInfo.value(nameList.at(0)).isEmpty()){
            {
                /*开始初步分析文件*/
                //从文件头获取各种信息
                //Code
                QString sendCode=nameList.at(1);
                QString recCode=nameList.at(2);
                QString dateInfo=nameList.at(3);
                QString fileIndexTypeCode=nameList.at(0);
                //名称信息
                QString sendName=loadedOfdCodeInfo.value(sendCode).getName();
                QString recName=loadedOfdCodeInfo.value(recCode).getName();
                QString fileIndexTypeName=loadedOfdIndexFileInfo.value(fileIndexTypeCode);
                //刷新UI
                ui->lineEditSendCode->setText(sendCode);
                ui->lineEditRecCode->setText(recCode);
                ui->lineEditFileTransferDate->setText(dateInfo);
                ui->lineEditFileType->setText(fileIndexTypeCode);
                ui->lineEditSenfInfo->setText(sendName);
                ui->lineEditRecInfo->setText(recName);
                ui->lineEditFileDescribe->setText(fileIndexTypeName);
                ui->lineEditFileDescribe->setToolTip(fileIndexTypeName);
                //此处开始加载索引文件
                load_ofdIndexFile();
                return;
            }
        }
        else{
            qDebug()<<"索引识别异常";
            //加载索引异常,尝试识别下是不是其他类型文件
            goto NOT_OF_FILE;
        }
    }
    //非OF体系的文件，既不是OFD数据文件，又不是索引文件，则不放弃，检测下是否是csv或者定长文件
NOT_OF_FILE:
    //20220302新加逻辑,先看看是不是类OFD的文件
    bool isLikeOFD=false;
    bool isLikeOFDIndex=false;
    if(likeOFDDataFilename.count()>0){
        QHash<QString, QString>::iterator h2;
        for (h2=likeOFDDataFilename.begin(); h2!=likeOFDDataFilename.end(); ++h2){
            QString Name=h2.key().toUpper();
            QString pattern(Name);
            QRegExp rx(pattern);
            rx.setPatternSyntax(QRegExp::Wildcard);
            bool match = rx.exactMatch(currentFileName.toUpper());
            //一旦匹配到，则把此文件当作OFD文件去尝试解析
            if(match){
                isLikeOFD=true;
                likeOFDFileType=likeOFDDataFilename.value(h2.key());
                break;
            }
        }
    }
    if(likeOFDIndexFilename.count()>0){
        QHash<QString, QString>::iterator h2;
        for (h2=likeOFDIndexFilename.begin(); h2!=likeOFDIndexFilename.end(); ++h2){
            QString Name=h2.key().toUpper();
            QString pattern(Name);
            QRegExp rx(pattern);
            rx.setPatternSyntax(QRegExp::Wildcard);
            bool match = rx.exactMatch(currentFileName.toUpper());
            if(match){
                isLikeOFDIndex=true;
                likeOFDFileType=likeOFDIndexFilename.value(h2.key());
                break;
            }
        }
    }
    if(isLikeOFD&&!isLikeOFDIndex){
        qDebug()<<likeOFDFileType;
        qDebug()<<"类OFD文件";
        //开始拆解文件名
        QString fixName=currentFileName;
        if(fixName.contains(".",Qt::CaseInsensitive)){
            fixName=fixName.mid(0,fixName.indexOf("."));
        }
        //类OFD文件名处理完毕后，开始拆解文件名---对于类OFD我们允许文件名不符合OFD的几段规范
        QStringList nameList=fixName.split("_");
        //正常的OFD文件应该至少有5段信息组成,另外中登TA和管理人交互的文件还有批次号,如果不是，则跳转到非OF文件检查
        if(nameList.count()<5){
            QString sendCode="";
            QString recCode="";
            if(nameList.count()>1){
                sendCode=nameList.at(1);
                recCode=nameList.at(2);
            }
            //尝试直接去解析文件,对于类OFD只要配置了OFD_like.ini就当ofd去解析，哪怕文件名不是标准的
            load_ofdDataFile("@@",sendCode,recCode);
            return;
        }
        else{
            /*开始初步分析文件*/
            //从文件头获取各种信息
            //Code
            QString sendCode=nameList.at(1);
            QString recCode=nameList.at(2);
            QString dateInfo=nameList.at(3);
            QString fileTypeCode=nameList.at(4);
            //从配置文件获取名称信息
            QString sendName=(loadedOfdCodeInfo.value(sendCode)).getName();
            QString recName=(loadedOfdCodeInfo.value(recCode)).getName();
            //刷新UI
            ui->lineEditSendCode->setText(sendCode);
            ui->lineEditRecCode->setText(recCode);
            ui->labelFileTransferDate->setText("传递日期:");
            ui->lineEditFileTransferDate->setText(dateInfo);
            ui->lineEditFileType->setText(fileTypeCode);
            ui->lineEditSenfInfo->setText(sendName);
            ui->lineEditRecInfo->setText(recName);
            //判断是否是ok文件
            if(currentOpenFilePath.endsWith(".OK",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("这是一个类OFD文件的OK文件,请解析原始数据文件...");
                return;
            }
            //判断是否是恒生系统sqllder导入时生成的bad文件
            if(currentOpenFilePath.endsWith(".BAD",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("这是一个sqlldr导入生成的bad文件,请解析原始数据文件...");
                return;
            }
            //判断是否是恒生系统sqllder导入时的控制文件
            if(currentOpenFilePath.endsWith(".CTL",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("是一个sqlldr导入使用的控制文件,请解析原始数据文件...");
                return;
            }
            //判断是否是快捷方式
            if(currentOpenFilePath.endsWith(".LNK",Qt::CaseInsensitive)){
                statusBar_disPlayMessage("我没猜错的话这是一个文件快捷方式,请解析原始数据文件...");
                return;
            }
            //记录从文件里读取的文件发送信息
            //此处开始加载OFD数据文件
            load_ofdDataFile(fileTypeCode,sendCode,recCode);
            return;
        }
    }
    else if(!isLikeOFD&&isLikeOFDIndex){
        qDebug()<<"类OFD索引文件";
        //开始拆解文件名
        QString fixName=currentFileName;
        if(fixName.contains(".",Qt::CaseInsensitive)){
            fixName=fixName.mid(0,fixName.indexOf("."));
        }
        //索引文件名处理完毕后，开始拆解文件名
        QStringList nameList=fixName.split("_");
        //OFX_发送方_接受方_日期.TXT
        //OFX_发送方_接受方_日期_批次号.TXT
        //正常的OFD索引文件应该至少有4段信息组成---类OFD的索引，不要求遵守此规则
        if(nameList.count()<4){
            load_ofdIndexFile();
            return;
        }
        else{
            /*开始初步分析文件*/
            //从文件头获取各种信息
            //Code
            QString sendCode=nameList.at(1);
            QString recCode=nameList.at(2);
            QString dateInfo=nameList.at(3);
            QString fileIndexTypeCode=nameList.at(0);
            //名称信息
            QString sendName=loadedOfdCodeInfo.value(sendCode).getName();
            QString recName=loadedOfdCodeInfo.value(recCode).getName();
            QString fileIndexTypeName=loadedOfdIndexFileInfo.value(fileIndexTypeCode);
            //刷新UI
            ui->lineEditSendCode->setText(sendCode);
            ui->lineEditRecCode->setText(recCode);
            ui->lineEditFileTransferDate->setText(dateInfo);
            ui->lineEditFileType->setText(fileIndexTypeCode);
            ui->lineEditSenfInfo->setText(sendName);
            ui->lineEditRecInfo->setText(recName);
            ui->lineEditFileDescribe->setText(fileIndexTypeName);
            ui->lineEditFileDescribe->setToolTip(fileIndexTypeName);
            //此处开始加载索引文件
            load_ofdIndexFile();
            return;
        }
    }
    //开始判断是不是csv类别的文件
    ////////////////////////////////////////////
    QList<matchIndex> csvMatchList;
    if(csvConfigList.count()>0){
        for(int l1=0;l1<csvConfigList.count();l1++){
            if(csvConfigList.at(l1).getConfigSegmentList().count()>0){
                for(int l2=0;l2<csvConfigList.at(l1).getConfigSegmentList().count();l2++){
                    QString Name=csvConfigList.at(l1).getConfigSegmentList().at(l2).getFileName().toUpper();
                    QString pattern(Name);
                    QRegExp rx(pattern);
                    rx.setPatternSyntax(QRegExp::Wildcard);
                    bool match = rx.exactMatch(currentFileName.toUpper());
                    if(match){
                        matchIndex a;
                        a.configFileIndex=l1;
                        a.configSegmentIndex=l2;
                        csvMatchList.append(a);
                    }
                }
            }
        }
    }
    //定长文件
    ///////////////////////////////////////////
    QList<matchIndex> fixedMatchList;
    if(fixedConfigList.count()>0){
        for(int l1=0;l1<fixedConfigList.count();l1++){
            if(fixedConfigList.at(l1).getConfigSegmentList().count()>0){
                for(int l2=0;l2<fixedConfigList.at(l1).getConfigSegmentList().count();l2++){
                    QString Name=fixedConfigList.at(l1).getConfigSegmentList().at(l2).getFileName().toUpper();
                    QString pattern(Name);
                    QRegExp rx(pattern);
                    rx.setPatternSyntax(QRegExp::Wildcard);
                    bool match = rx.exactMatch(currentFileName.toUpper());
                    if(match){
                        matchIndex a;
                        a.configFileIndex=l1;
                        a.configSegmentIndex=l2;
                        fixedMatchList.append(a);
                    }
                }
            }
        }
    }
    //判断是进入
    if(csvMatchList.count()>0&&fixedMatchList.count()==0){
        load_csvFile(csvMatchList);
        return;
    }
    else if(csvMatchList.count()==0&&fixedMatchList.count()>0){
        load_fixedFile(fixedMatchList);
        return;
    }
    //同时两种类型的文件名字都匹配到了
    else if(csvMatchList.count()>0&&fixedMatchList.count()>0){
        DialogChooseFileType  dialog2(this);
        dialog2.setWindowTitle("打开的文件文件名匹配到了两种文件大类型,请确认");
        dialog2.setModal(true);
        dialog2.exec();
        //从弹窗中获取结果
        int type=dialog2.getFileType();
        if(type==2){
            load_csvFile(csvMatchList);
            return;
        }
        else if(type==3){
            load_fixedFile(fixedMatchList);
            return;
        }
        else{
            statusBar_disPlayMessage("放弃读取!");
            return;
        }
    }
    //自动化csv文件分析
    //此部分构造一个自动化识别csv文件的方案，尝试免配置自动解析csv文件，支持带标题行和不带标题行的竖线分割，tab分割，逗号分割三种方式
    //支持自动识别的文件编码方式
    QString filecharset="UTF-8";
    QFile dataFile(currentOpenFilePath);
    //判断如果文件打开成功,则开始读取
    if (dataFile.open(QFile::ReadOnly))
    {
        int lineNumber=0;
        //存放前50行
        QList<QByteArray> line50Qlist;
        //存放识别编码的文本样本
        QByteArray line50ByteArray;
        while (!dataFile.atEnd()&&lineNumber<50)
        {
            QByteArray line=dataFile.readLine();
            line50Qlist.append(line);
            line50ByteArray.append(line);
            lineNumber++;
        }
        dataFile.close();
        //读取前50行结束后开始自动识别编码-感谢libucd项目，感谢mozilla
        if(line50ByteArray.count()>0&&line50Qlist.count()>0){
            //自动识别编码开始
            filecharset=m_libucd.CharsetDetect(line50ByteArray);
            qDebug()<<"自动识别出来的原始编码格式"<<filecharset;
            //不在编码白名单的编码方式暂不支持，统一使用UTF-8解析
            if(!allowCharsetList.contains(filecharset.toUpper())){
                //不在允许的编码范围的默认转义
                if(filecharset.toUpper()=="WINDOWS-1252"||filecharset.toUpper()=="WINDOWS-1251"||filecharset.toUpper()=="KOI8-R"){
                    filecharset="GB18030";
                }
                else{
                    filecharset="UTF-8";
                }
            }
            //将gb18030大写
            if(filecharset.toUpper()=="GB18030"){
                filecharset="GB18030";
            }
            //自动识别编码结束
            QTextCodec *codec=QTextCodec::codecForName(filecharset.toLocal8Bit());
            if(autoFlagList.count()>0){
                QString useflag="";
                int lastCount=1;
                double lastCV=999;
                int titleRow=0;
                int dataBeginRow=1;
                QList<QStringList > lineQListQStringListUse;
                QString fileini="自动解析引擎";
                QString fileName="";
                QString fileDescribe="自动识别的固定分隔符文件";
                QString endwithflag="0";
                int contextBgein=-1;
                //-1自动识别
                int ignorerow=-1;
                bool clearQuotesStr=true;
                int fieldCount=-1;
                QList <CsvFieldDefinition> fieldTitleList;
                QStringList fieldTitleStrList;
                for(int autoFlagIndex=0;autoFlagIndex<autoFlagList.count();autoFlagIndex++){
                    QString thisflag=autoFlagList.at(autoFlagIndex);
                    QRegExp rx("\\"+thisflag+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                    qDebug()<<"开始尝试基于"<<thisflag<<"解析数据...";
                    //只有一行数据
                    //如果文件只有一行数据，则对该行数据进行使用不同的分隔符拆分，拆分出来的字段数最多的则获得解析权，拆分出来的列不得低于2
                    if(line50Qlist.count()==1){
                        QString line = codec->toUnicode(line50Qlist.at(0)).remove('\r').remove('\n');
                        QStringList fieldList=line.split(rx);
                        //只有一行数据就看谁拆分出来的多
                        if(fieldList.count()>lastCount){
                            contextBgein=0;
                            lineQListQStringListUse.clear();
                            lineQListQStringListUse.append(fieldList);
                            lastCount=fieldList.count();
                            useflag=thisflag;
                        }
                    }
                    //文件内容行数大于1，开始分析多行数据--基于离线系数决定使用哪个分隔符
                    //1:离线系数越低证明按这个分隔符解析出来的数据越整齐,更像是格式化的数据
                    //2:考虑到文件可能存在文件头和文件尾,针对同一个类型的文件，行数越少，文件头和尾对离散程度影响越高
                    else{
                        QList<QStringList > lineQListQStringList;
                        //step1-对前50行数据进行分隔符拆分
                        float avgCount=0;
                        for(int lineN=0;lineN<line50Qlist.count();lineN++){
                            QStringList line=codec->toUnicode(line50Qlist.at(lineN)).replace("\r","").replace("\n","").split(rx);
                            lineQListQStringList.append(line);
                            qDebug()<<line;
                            avgCount+=line.count();
                        }
                        //step2-统计离散系数
                        double CV=Utils::CVCcal(lineQListQStringList);
                        qDebug()<<"上一个分隔符离散系数"<<lastCV;
                        qDebug()<<"本分隔符离散系数"<<CV;
                        //平均列控制
                        avgCount=avgCount/lineQListQStringList.count();
                        qDebug()<<"平均列"<<avgCount;
                        //平均不到2列直接跳过本分隔符
                        if(avgCount<2){
                            continue;
                        }
                        //如果这个分割符分出来的比上一个分隔符的离散系数更低，则更新
                        if(CV<=lastCV){
                            useflag=thisflag;
                            lineQListQStringListUse=lineQListQStringList;
                            lastCV=CV;
                            //step3-开始循环分析列数,根据最大的列数决定以多少列展示数据
                            int previousRowFieldCount=1;
                            //疑似数据起始行（含标题）--从这一行开始往后最大的列
                            contextBgein=-1;
                            lastCount=-1;
                            for(int lineN=0;lineN<lineQListQStringList.count();lineN++){
                                //首次遇到大于1列的-认为该行数据就是起始行
                                if(previousRowFieldCount==1&&lineQListQStringList.at(lineN).count()>1&&lineN>contextBgein){
                                    previousRowFieldCount=lineQListQStringList.at(lineN).count();
                                    lastCount=previousRowFieldCount;
                                    contextBgein=lineN;
                                }
                                //非首次遇到-并且解析到了更多的列-则更新起始行--正常来说数据每一行的列数应该都是一样的，如遇到，则代表开始到了数据行
                                else if(lineQListQStringList.at(lineN).count()>previousRowFieldCount&&lineN>contextBgein){
                                    previousRowFieldCount=lineQListQStringList.at(lineN).count();
                                    if(previousRowFieldCount>lastCount){
                                        lastCount=previousRowFieldCount;
                                    }
                                    contextBgein=lineN;
                                }
                            }
                        }
                    }
                }
                //step4-检查识别结果
                if(lastCount>1&&!useflag.isEmpty()){
                    //step5-进阶分析标题和其他情况
                    qDebug()<<"最终决定使用的分隔符"<<useflag;
                    qDebug()<<"列数"<<lastCount;
                    qDebug()<<"开始行号"<<contextBgein+1;
                    bool firstRowIsTitle=false;
                    //从疑似第一行开始，做进阶分析
                    //先判断该行数据是否是标题行------取第一行
                    float firstCheck=1;
                    float ct=0;
                    float all=0;
                    QStringList firstRowfieldList=lineQListQStringListUse.at(contextBgein);
                    for(int xx=0;xx<firstRowfieldList.count();xx++){
                        int fiedleLength=firstRowfieldList.at(xx).length();
                        int in=0;
                        while (in<fiedleLength) {
                            if(firstRowfieldList.at(xx).at(in)>=QChar('A')){
                                ct++;
                            }
                            all++;
                            in++;
                        }
                    }
                    //直接满足标题要求
                    firstCheck=ct/all;
                    if((firstCheck)>titlecheck){
                        firstRowIsTitle=true;
                    }
                    //不满足标题行
                    else {
                        if(contextBgein+1<lineQListQStringListUse.count()){
                            if(lineQListQStringListUse.at(contextBgein+1).count()==firstRowfieldList.count()){
                                float secondCheck=1;
                                float c2t=0;
                                float all2=0;
                                QStringList secondRowfieldList=lineQListQStringListUse.at(contextBgein+1);
                                for(int xx=0;xx<secondRowfieldList.count();xx++){
                                    int fiedleLength=secondRowfieldList.at(xx).length();
                                    int in=0;
                                    while (in<fiedleLength) {
                                        if(secondRowfieldList.at(xx).at(in)>=QChar('A')){
                                            c2t++;
                                        }
                                        all2++;
                                        in++;
                                    }
                                }
                                secondCheck=c2t/all2;
                                //第一行不过标题临界值，但是第一和第二行差异过大，我们依然认为第一行就是标题
                                if(abs(secondCheck-firstCheck)>titleandcontextcheck){
                                    firstRowIsTitle=true;
                                }
                                else{
                                    firstRowIsTitle=false;
                                }
                            }
                            else{
                                firstRowIsTitle=false;
                            }
                        }
                        else{
                            firstRowIsTitle=false;
                        }
                    }
                    if(firstRowIsTitle){
                        fieldTitleStrList.clear();
                        titleRow=contextBgein+1;
                        dataBeginRow=contextBgein+2;
                        fieldCount=firstRowfieldList.count();
                        fieldTitleList.clear();
                        for(int xx=0;xx<firstRowfieldList.count();xx++){
                            CsvFieldDefinition fieldItem;
                            QString name=firstRowfieldList.at(xx);
                            if(name.isEmpty()){
                                name="未定义的字段名";
                            }
                            fieldItem.setFieldDescribe(name);
                            fieldTitleStrList.append(name);
                            fieldTitleList.append(fieldItem);
                        }
                    }
                    else{
                        fieldTitleStrList.clear();
                        titleRow=0;
                        dataBeginRow=contextBgein+1;
                        fieldCount=firstRowfieldList.count();
                        fieldTitleList.clear();
                        for(int xx=0;xx<firstRowfieldList.count();xx++){
                            CsvFieldDefinition fieldItem;
                            QString name="第"+QString::number(xx+1)+"列";
                            fieldItem.setFieldDescribe(name);
                            //添加此字段信息到文件定义
                            fieldTitleList.append(fieldItem);
                            fieldTitleStrList.append(name);
                            fieldTitleList.append(fieldItem);
                        }
                    }
                    //优化描述展示,毕竟直接展示制表符等控制字符不是很优雅
                    QString flagd="";
                    if(useflag=="\t"){
                        flagd="制表符";
                    }
                    else if(useflag.length()==1&&useflag.at(0)==QChar(1)){
                        flagd="SOH控制符";
                    }
                    else if(useflag.length()==1&&useflag.at(0)==QChar(31)){
                        flagd="1FH(US)单元分割符";
                    }
                    else {
                        flagd=useflag;
                    }
                    fileName="基于 "+flagd+" 的固定分隔符文件";
                    fileDescribe=fileName;
                    //构建一个可用的解析配置
                    CsvFileDefinition fileDef;
                    fileDef.setFfAuto(true);
                    fileDef.setAutoecoding(true);
                    fileDef.setSplit(useflag);
                    fileDef.setEcoding(filecharset);
                    fileDef.setFileIni(fileini);
                    fileDef.setUseAble(true);
                    fileDef.setFieldCount(fieldCount);
                    fileDef.setClearQuotes(clearQuotesStr);
                    //默认不启用trim
                    fileDef.setTrim(false);
                    fileDef.setEndwithflag(endwithflag);
                    fileDef.setEndIgnoreRow(ignorerow);
                    fileDef.setFileDescribe(fileDescribe);
                    fileDef.setTitlerowindex(titleRow);
                    fileDef.setDatabeginrowindex(dataBeginRow);
                    fileDef.setFileNameWithCount(fileName);
                    fileDef.setFieldList(fieldTitleList);
                    //当前打开的文件类型为csv
                    currentOpenFileType=openFileType::CSVFile;
                    //开始加载数据
                    csv=fileDef;
                    load_csvFileData(fieldTitleStrList);
                    return;
                }
            }
        }
    }
    //完全无法识别的文件
    /////////////////////////////////////////
    statusBar_disPlayMessage("文件无法识别,请尝试配置解析规则,目前工具支持各类OFD文件,CSV文件,以及其他定长字段文件");
    return;
}

/**
 * @brief MainWindow::load_indexFile索引文件的加载方法
 */
void MainWindow::load_ofdIndexFile(){
    currentOpenFileType=openFileType::OFDIndex;
    ui->labelFileTransferDate->setText("传递日期:");
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly))
    {
        QString lineQString;
        int lineNumber=0;
        while (!dataFile.atEnd())
        {
            lineQString = codecOFD->toUnicode(dataFile.readLine()).remove('\r').remove('\n');
            //文件体
            if(lineNumber>5){
                ofdindexFileDataList.append(lineQString);
            }
            //文件头部
            else{
                switch (lineNumber) {
                case 0:
                    //文件第一行OF标记
                    ofdIndexFileHeaderMap.insert("filebegin",lineQString);
                    break;
                case 1:
                    //文件第二行版本
                    ofdIndexFileHeaderMap.insert("version",lineQString);
                    break;
                case 2:
                    //文件第三行发送方代码
                    ofdIndexFileHeaderMap.insert("sendcode",lineQString);
                    break;
                case 3:
                    //文件第四行接收方代码
                    ofdIndexFileHeaderMap.insert("recivecode",lineQString);
                    break;
                case 4:
                    //文件第五行文件传递日期
                    ofdIndexFileHeaderMap.insert("filedate",lineQString);
                    break;
                case 5:
                    //文件第6行文件记录数
                    ofdIndexFileHeaderMap.insert("count",lineQString);
                    break;
                default:
                    break;
                }
            }
            lineNumber++;
        }
        //判断文件是否为空
        if(lineNumber<2){
            currentOpenFileType=openFileType::NotFileOrErr;
            statusBar_disPlayMessage("无效的文件,请检查文件是否满足索引文件规范...");
            return;
        }
        //处理最后一行
        QString lastLine=ofdindexFileDataList.last();
        if(lastLine.startsWith("OFDCFEND")){
            ofdIndexFileHeaderMap.insert("fileend",lastLine);
            ofdindexFileDataList.removeLast();
        }
        dataFile.close();
    }
    //读取完毕OFD文件的信息后，进行数据显示
    init_display_OFDIndexTable();
}

/**
 * @brief MainWindow::load_ofdDataFile OFD文件的加载方法
 * @param fileTypeFromFileName
 * @param sendCode
 * @param recvCode
 */
void MainWindow::load_ofdDataFile(QString fileTypeFromFileName,QString sendCode,QString recvCode){
    QString fileType=fileTypeFromFileName;
    qDebug()<<"开始加载的文件类别"<<fileType;
    //当前打开的文件类别为1，OFD文件
    currentOpenFileType=openFileType::OFDFile;
    //读取文件结束位置
    //第10行文件字段记录数//先读取到这一行看能否获取到字段记录数
    int lineEnd=9;
    //从文件中读取到的字段总和--用于稍后比对
    int countNumberFromFile=0;
    //从文件中读取到的文件版本号--用于稍后比对
    QString versionFromFile="";
    //从文件中读取到的字段列表-用于稍后比对
    QStringList fieldNameFromFile;
    //从文件中读取到的第一行数据的长度,如果是空文件就赋值-1
    int firstLineDateLenght=-1;
    //开始准备打开文件
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly))
    {
        QByteArray lineQByteArray;
        QString lineQString;
        int lineNumber=0;
        bool atLastRow=false;
        while (!dataFile.atEnd())
        {
            lineQByteArray=dataFile.readLine();
            //根据OFD文件的编码解开成unicode字符串
            lineQString = codecOFD->toUnicode(lineQByteArray);
            atLastRow=dataFile.atEnd();
            //记录需要截取的换行符长度
            if(lineQString.endsWith("\r\n")){
                newLineCharsetLength=2;
            }
            else {
                newLineCharsetLength=1;
            }
            //第一行数据不trim
            if(lineNumber==lineEnd&&lineNumber!=9&&!atLastRow){
                //数据行不能trim
                lineQString=lineQString.left(lineQString.length()-newLineCharsetLength);
            }
            else{
                if(!atLastRow){
                    lineQString=lineQString.left(lineQString.length()-newLineCharsetLength).trimmed();
                }
                else{
                    if(lineQString.endsWith("\r\n")){
                        lineQString=lineQString.left(lineQString.length()-newLineCharsetLength).trimmed();
                    }
                    else if(lineQString.endsWith("\r")){
                        lineQString=lineQString.left(lineQString.length()-newLineCharsetLength).trimmed();
                    }
                    else if(lineQString.endsWith("\n")){
                        lineQString=lineQString.left(lineQString.length()-newLineCharsetLength).trimmed();
                    }
                    else{
                        lineQString=lineQString.trimmed();
                    }
                }
            }
            //文件的第二行是文件版本
            if(lineNumber==1){
                versionFromFile=lineQString.trimmed();
                //获取不到版本号，退出
                if(versionFromFile.isEmpty()){
                    currentOpenFileType=openFileType::NotFileOrErr;
                    dataFile.close();
                    statusBar_disPlayMessage("解析失败,未从文件第2行读取到OFD/类OFD文件的版本号信息");
                    return;
                }
            }
            if(lineNumber==6){
                QString fileTypeFromFile=lineQString.trimmed();
                //类OFD取文件里的文件类型
                if(fileTypeFromFile!=fileType){
                    qDebug()<<"文件内的文件类型和文件名里的不一致,尝试取文件里的";
                    //不一致-且文件内内容是非空
                    if(!fileTypeFromFile.isEmpty()){
                        fileType=fileTypeFromFile;
                        fileType=fileTypeFromFile;
                        ui->lineEditFileType->setText(fileTypeFromFile);
                    }
                    //不一致-但是文件内是空，且传递过来的是@@
                    else if(fileTypeFromFile.isEmpty()&&fileType=="@@"){
                        currentOpenFileType=openFileType::NotFileOrErr;
                        dataFile.close();
                        statusBar_disPlayMessage("解析失败,无法合理的从文件名或者文件第10行断定文件类型,请检查文件");
                    }
                    //不一致-但是文件内是空，且传过来的不是@@-还是使用文件名里的吧
                    else{
                        qDebug()<<"依然使用文件名里的文件类型";
                    }
                }
            }
            //文件的第10行记录了该文件有多少个字段
            //如果读取本行成功，则延长读取结束到第一行数据
            if(lineNumber==9){
                QString count=lineQString;
                bool flag=false;
                countNumberFromFile=count.toInt(&flag);
                if(!flag)
                {
                    currentOpenFileType=openFileType::NotFileOrErr;
                    dataFile.close();
                    statusBar_disPlayMessage("解析失败,从文件第10行读取该文件接口字段总数失败,请检查文件");
                    return;
                }else{
                    //如果读取记录数成功,则延长结束行,读取文件的字段记录,读取到第一个数据行结束
                    //读取第一个记录行的长度，方便稍后比对文件解析
                    //字段列表结束下一行是记录数，再下一行是第一行数据
                    lineEnd=lineEnd+countNumberFromFile+2;
                }
            }
            //从第11行开始,为字段名字-直至设定的结束行的前两行
            if(lineNumber>9&&lineNumber<lineEnd-1){
                fieldNameFromFile.append(lineQString);
            }
            //读取到了文件记录数
            if(lineNumber>9&&lineNumber==lineEnd-1){
                bool dataCountOk=false;
                int dataCount=lineQString.toInt(&dataCountOk,10);
                if(dataCountOk){
                    //没有数据行，不再循环读取下一行数据行了
                    if(dataCount<1){
                        break;
                    }
                }
                else{
                    currentOpenFileType=openFileType::NotFileOrErr;
                    dataFile.close();
                    statusBar_disPlayMessage("解析失败,文件数据记录数["+lineQString+"]不是有效的数值,请检查文件是否合法");
                    return;
                }
            }
            //读取到了第一行记录，哈哈哈
            if(lineNumber>9&&lineNumber==lineEnd){
                //当记录数大于0的情况下，如果有极端情况，数据记录数大于0，但是却没任何记录，直接是结束符号，则特殊判断
                if(lineQString!="OFDCFEND"){
                    //这里直接使用lineByteArray长度-换行符
                    firstLineDateLenght=lineQByteArray.length()-newLineCharsetLength;
                }
                else{
                    firstLineDateLenght=-1;
                    break;
                }
            }
            lineNumber++;
            //如果到达了终止点就跳出
            if(lineNumber>lineEnd){
                break;
            }
        }
        //提取文件首部的重要信息结束,关闭文件
        dataFile.close();
        //判断是否读取到了一个空的文件
        //OFD文件行记录数最小也不可能小于10行
        if(lineNumber<10){
            currentOpenFileType=openFileType::NotFileOrErr;
            statusBar_disPlayMessage("无效的文件,请检查文件是否满足OFD/类OFD文件规范...");
            return;
        }
        QString useini="OFD_"+versionFromFile+".ini";
        ui->lineEditUseIni->setText(useini);
        ui->lineEditUseIni->setToolTip(nullptr);
        //判断对应的配置文件是否存在
        //从文件头部收取到了足够的信息后，我们要开始尝试用配置解析文件了
        //根据配置和从文件里采集到的文件字段总数，首行数据的长度综合分析，决定使用哪个配置文件解析文件
        QString path=Utils::getConfigPath()+useini;
        if(ofdQuickMatchIndex.contains(versionFromFile)&&ofdConfigList.count()>ofdQuickMatchIndex.value(versionFromFile)&&ofdConfigList.at(ofdQuickMatchIndex.value(versionFromFile)).getConfigSegmentList().count()>0){
            QList<OFDFileDefinition> defList=ofdConfigList.at(ofdQuickMatchIndex.value(versionFromFile)).getConfigSegmentList();
            //失败原因列表
            QList<OFDFaultCause> faultList;
            //这里开始根据文件类别筛查基本满足要求的配置
            //1.8.9版本开始，新增多配置匹配手工选择解析类别
            //比如一个文件既满足开放式基金配置，又满足银行理财子配置，则弹窗让你选择使用哪一个
            QList<OFDFileDefinition> matchOFD;
            for (int i = 0; i <defList.count(); ++i) {
                OFDFileDefinition ofdFileDefinition=defList.at(i);
                //标记使用的解析器
                QString config=ofdFileDefinition.getConfigSegment();
                QString name=ofdFileDefinition.getDescribe();
                if(ofdFileDefinition.getUseForVersionAndType()==(versionFromFile+"_"+fileType)){
                    //先判断配置文件是否可用，不可用的配置直接忽略
                    if(!ofdFileDefinition.getUseAble()){
                        //首行数据不满足
                        OFDFaultCause fault;
                        fault.setConfig(config);
                        fault.setName(name);
                        fault.setCause("配置文件错误无法使用此配置解析任何文件,错误原因:"+ofdFileDefinition.getMessage());
                        faultList.append(fault);
                        continue;
                    }
                    //判断字段数
                    if(ofdFileDefinition.getFieldCount()==countNumberFromFile){
                        //字段一致，开始循环遍历字段
                        //遍历比对字段是否一致
                        bool fieldOk=true;
                        for(int ff=0;ff<fieldNameFromFile.count();ff++){
                            if((QString(fieldNameFromFile.at(ff))).toUpper()!=ofdFileDefinition.getFieldList().at(ff).getFieldName().toUpper()){
                                OFDFaultCause fault;
                                fault.setConfig(config);
                                fault.setName(name);
                                fault.setCause("配置文件"+useini+"中的解析器"+config+"所代表的文件第["+QString::number(ff+1)+"]个字段是["+ofdFileDefinition.getFieldList().at(ff).getFieldName().toUpper()+"],但是从文件第["+QString::number(11+ff)+"]行获取到的字段是["+(QString(fieldNameFromFile.at(ff))).toUpper()+"] 字段名(忽略大小写)不一致,请检查文件是否满足接口标准,或者配置是否有误");
                                faultList.append(fault);
                                fieldOk=false;
                                break;
                            }
                        }
                        //如果字段也都一致
                        if(fieldOk){
                            //字段一致的情况下，判断是不是读取到了文件第一行，如果没有读取到文件第一行就是空文件，直接解析，如果解析到了第一行，再比较下长度，以防万一
                            //首行长度非-1
                            if(firstLineDateLenght!=-1)
                            {
                                //开始判断首行---满足
                                if(firstLineDateLenght ==ofdFileDefinition.getRowLength()){
                                    matchOFD.append(ofdFileDefinition);
                                    continue;
                                }
                                else{
                                    //首行数据不满足
                                    OFDFaultCause fault;
                                    fault.setConfig(config);
                                    fault.setName(name);
                                    fault.setCause("配置文件"+useini+"中的解析器"+config+"所代表的文件的文件每行数据长度是["+QString::number(ofdFileDefinition.getRowLength())+"]字节,但是从文件第1行数据记录获取到的数据长度是["+QString::number(firstLineDateLenght)+"]字节,请检查文件是否满足该配置对应的接口标准,或者配置是否有误");
                                    faultList.append(fault);
                                }
                            }
                            //空数据无法校验首行记录，直接认定满足
                            else{
                                matchOFD.append(ofdFileDefinition);
                                continue;
                            }
                        }
                    }
                    else{
                        OFDFaultCause fault;
                        fault.setConfig(config);
                        fault.setName(name);
                        fault.setCause(QString("打开的文件的字段数是[%1]和配置文件中的[%2]不一致,请检查文件是否满足该配置对应的接口标准,或者配置是否有误").arg(QString::number(countNumberFromFile),QString::number(ofdFileDefinition.getFieldCount())));
                        faultList.append(fault);
                    }
                }
            }
            //如果至少一条满足，则准备进入解析步骤
            if(matchOFD.count()>0){
                //只有一条满足的，直接干
                if(matchOFD.count()==1){
                    if(matchOFD.at(0).getUseForTA().count()>0&&!matchOFD.at(0).getUseForTA().at(0).isEmpty()&&!matchOFD.at(0).getUseForTA().contains(sendCode)&&!matchOFD.at(0).getUseForTA().contains(recvCode)){
                        qDebug()<<matchOFD.at(0).getConfigSegment()<<matchOFD.at(0).getUseForTA()<<QString("不匹配TA配置,发送方%1,接收方%2").arg(sendCode,recvCode);
                        OFDFaultCause fault;
                        fault.setConfig(matchOFD.at(0).getConfigSegment());
                        fault.setName(matchOFD.at(0).getDescribe());
                        QString ta = matchOFD.at(0).getUseForTA().join(",");
                        fault.setCause(QString("该配置文件仅适用于和(%1)TA交互的文件,如需要使用此配置解析,请编辑配置文件将新的TA加入").arg(ta));
                        faultList.append(fault);
                        goto display_OFDFaultCause;
                    }
                    else{
                        ofd=matchOFD.at(0);
                    }
                }
                else{
                    qDebug()<<"多个配置满足解析";
                    //分命中TA与否
                    QList<OFDFileDefinition> matchOFDNoTa;
                    QList<OFDFileDefinition> matchOFDUseForTa;
                    //排序
                    std::sort(matchOFD.begin(), matchOFD.end(),compareOFDData);
                    for(int i=0;i<matchOFD.count();i++){
                        OFDFileDefinition item=matchOFD.at(i);
                        if(item.getUseForTA().count()>0&&!item.getUseForTA().at(0).isEmpty()&&(item.getUseForTA().contains(sendCode)||item.getUseForTA().contains(recvCode))){
                            matchOFDUseForTa.append(item);
                            qDebug()<<item.getConfigSegment()<<item.getUseForTA()<<QString("匹配TA配置,发送方%1,接收方%2").arg(sendCode,recvCode);
                        }
                        //如果有多个可用配置则针对TA的配置则不再列入可选配置
                        else if(item.getUseForTA().count()>0&&!item.getUseForTA().at(0).isEmpty()&&!item.getUseForTA().contains(sendCode)&&!item.getUseForTA().contains(recvCode)){
                            qDebug()<<item.getConfigSegment()<<item.getUseForTA()<<QString("不匹配TA配置,发送方%1,接收方%2").arg(sendCode,recvCode);
                            OFDFaultCause fault;
                            fault.setConfig(item.getConfigSegment());
                            fault.setName(item.getDescribe());
                            QString ta = item.getUseForTA().join(",");
                            fault.setCause(QString("该配置文件仅适用于和(%1)TA交互的文件,如需要使用此配置解析,请编辑配置文件将新的TA加入").arg(ta));
                            faultList.append(fault);
                            continue;
                        }
                        else{
                            matchOFDNoTa.append(item);
                            qDebug()<<item.getConfigSegment()<<item.getUseForTA()<<QString("匹配非TA配置,发送方%1,接收方%2").arg(sendCode,recvCode);
                        }
                    }
                    //既然匹配TA的都有多个，那么不妨让用户TA不匹配的也选择下
                    if(matchOFDUseForTa.count()>1&&matchOFDNoTa.count()>0){
                        matchOFD=matchOFDUseForTa;
                        matchOFD.append(matchOFDNoTa);
                    }
                    //只匹配到1个适用于TA的，直接使用
                    else if(matchOFDUseForTa.count()==1&&matchOFDNoTa.count()>=0){
                        matchOFD=matchOFDUseForTa;
                    }
                    else if(matchOFDUseForTa.count()==0&&matchOFDNoTa.count()==0){
                        goto display_OFDFaultCause;
                    }
                    else{
                        matchOFD=matchOFDNoTa;
                    }
                    if(matchOFD.count()>1){
                        DialogChooseOFDConfig  dialog2(useini,&matchOFD,this);
                        dialog2.setWindowTitle("请选择用于解析此文件的解析器配置");
                        dialog2.setModal(true);
                        dialog2.exec();
                        //从弹窗中获取结果
                        int index=dialog2.getChooeseIndex();
                        if(!dialog2.getConfirm()){
                            statusBar_disPlayMessage("没有选择解析方案,放弃解析...");
                            return;
                        }
                        else{
                            ofd=matchOFD.at(index);
                        }
                    }
                    else if(matchOFD.count()==1){
                        ofd=matchOFD.at(0);
                    }
                    else{
                        statusBar_disPlayMessage("没有可用的解析方案,无法解析...");
                        return;
                    }
                }
                ui->lineEditFileDescribe->setText(ofd.getDescribe());
                ui->lineEditFileDescribe->setToolTip(ofd.getDescribe());
                QFile dataFile(currentOpenFilePath);
                //判断如果文件打开成功,则开始读取
                //革命尚未成功,同志还需努力,要不惜一切办法,提高读取速度
                if (dataFile.open(QFile::ReadOnly))
                {
                    int lineNumber=0;
                    //数据行开始的位置,10行文件头,1行行记录标识+字段行数
                    int beginIndex=11+ofd.getFieldCount();
                    bool sucessFlag=true;
                    bool mergeSucessFlag=false;
                    bool mergeFlag=false;
                    int ofdrowLength=ofd.getRowLength();
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    bool lastRowIsOFDCFEND=false;
                    bool readToLastRow=true;
                    lastRowHasNewLine=true;
                    while (!dataFile.atEnd())
                    {
                        lineQByteArray=dataFile.readLine();
                        lineQString = codecOFD->toUnicode(lineQByteArray);
                        if(lineQString.endsWith("\r\n")){
                            newLineCharsetLength=2;
                            lineQString=lineQString.left(lineQString.length()-newLineCharsetLength);
                            lineQByteArray=lineQByteArray.left(lineQByteArray.length()-newLineCharsetLength);
                        }
                        else if(lineQString.endsWith("\r")||lineQString.endsWith("\n")){
                            newLineCharsetLength=1;
                            lineQString=lineQString.left(lineQString.length()-newLineCharsetLength);
                            lineQByteArray=lineQByteArray.left(lineQByteArray.length()-newLineCharsetLength);
                        }
                        //最后一行且没有换行符--这种情况正常来说只发生在最后一行
                        else{
                            newLineCharsetLength=0;
                            if(dataFile.atEnd()){
                                lastRowHasNewLine=false;
                            }
                        }
                        //如果此行记录小于数据开始行,则认为是文件头,存入文件头
                        if(lineNumber<beginIndex){
                            commonHeaderQStringList.append(lineQString);
                        }
                        //数据行,进行数据行分析
                        else{
                            /*******标准数据行*******/
                            //分析是否读取到了OFDCFEND结束标记,是的话直接跳出循环
                            if(lineQString.length()==8){
                                if(QString::compare(lineQString,"OFDCFEND",Qt::CaseInsensitive)==0){
                                    lastRowIsOFDCFEND=true;
                                    ofdFooterQString="OFDCFEND";
                                    ofdFooterRowNumber=lineNumber+1;
                                    if(!dataFile.atEnd()){
                                        readToLastRow=false;
                                    }
                                    break;
                                }
                            }
                            //本行记录长度和接口约束的一致,存入--排除特殊情况，字段中存在换行符但是Qt读取数据时没换行
                            if(lineQByteArray.length()==ofdrowLength&&!lineQString.contains("\r")&&!lineQString.contains("\n")){
                                if(dataCompressLevel==0){
                                    commonContentQByteArrayList.append(lineQByteArray);
                                }
                                else{
                                    commonContentQByteArrayList.append(qCompress(lineQByteArray,dataCompressLevel));
                                }
                            }
                            /*******其他情况*******/
                            //如果本行数据长度和接口定义不一致,则尝试读取下一行,分析是否进行数据合并
                            else{
                                //如果已经是最后一行了,且内容不是结束标志--这个问题比较严肃,要提醒用户
                                if(dataFile.atEnd()){
                                    lastRowIsOFDCFEND=false;
                                    ofdFooterQString=lineQString;
                                    ofdFooterRowNumber=lineNumber+1;
                                    break;
                                }
                                QString nextLine = "";
                                //情况1Qt没换行但是含换行符号
                                if(lineQString.contains("\r")){
                                    int index=lineQString.indexOf("\r");
                                    nextLine=lineQString.mid(index+1);
                                    lineQString=lineQString.mid(0,index);
                                }
                                else if(lineQString.contains("\n")){
                                    int index=lineQString.indexOf("\r");
                                    nextLine=lineQString.mid(index+1);
                                    lineQString=lineQString.mid(0,index);
                                }
                                else{
                                    //读取下一行--QT读取换行
                                    nextLine = codecOFD->toUnicode(dataFile.readLine());
                                    if(nextLine.endsWith("\r\n")){
                                        newLineCharsetLength=2;
                                    }
                                    else if(nextLine.endsWith("\r")||nextLine.endsWith("\n")){
                                        newLineCharsetLength=1;
                                    }
                                    else{
                                        newLineCharsetLength=0;
                                    }
                                    //去除下一行的换行符获取该行实际数据
                                    nextLine=nextLine.left(nextLine.length()-newLineCharsetLength);
                                }
                                //如果下一行是文件结束标志,则不再进行合并尝试,直接报错
                                if(nextLine.length()==8&&QString::compare(nextLine,"OFDCFEND",Qt::CaseInsensitive)==0){
                                    sucessFlag=false;
                                    currentOpenFileType=openFileType::NotFileOrErr;
                                    statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                    QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getRowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"]行长度为["+QString::number(lineQByteArray.length())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                    break;
                                }
                                //如果下一行不是结束标志OFDCFEND,则尝试进行数据合并
                                else{
                                    //弹窗确认--只弹窗一次
                                    if(!mergeFlag){
                                        qApp->processEvents();
                                        QApplication::restoreOverrideCursor();
                                        DialogMergeTip  dialog2(this);
                                        dialog2.setWindowTitle("数据异常换行修复提示");
                                        dialog2.setModal(true);
                                        dialog2.exec();
                                        //从弹窗中获取结果
                                        mergeFlag=dialog2.getMergeFlag();
                                    }
                                    //如果确认需要合并---原则上这个功能只能修复字符串字段中的换行,只补空格
                                    //需要合并分三种情况上一行的长度+下一行的长度等于定义长度（人为插入的换行）
                                    //异常加入的换行符计入了长度,可能为\r \n \r\n三种情况，占用长度为1，2，以及多了换行符，两行的长度还是标准加一起的
                                    if (mergeFlag){
                                        //硬换行-两行的长度加起来刚好
                                        if(codecOFD->fromUnicode(lineQString+nextLine).length()==(ofdrowLength-0)){
                                            if(dataCompressLevel==0){
                                                commonContentQByteArrayList.append(codecOFD->fromUnicode(lineQString+nextLine));
                                            }
                                            else{
                                                commonContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(lineQString+nextLine),dataCompressLevel));
                                            }
                                            mergeSucessFlag=true;
                                            fileChanged=true;
                                            QApplication::setOverrideCursor(Qt::WaitCursor);
                                        }
                                        //替换了一个字符
                                        else if(codecOFD->fromUnicode(lineQString+nextLine).length()==(ofdrowLength-1)){
                                            //20210405算法优化，补充空格补充到字段的末尾
                                            for(int i=0;i<ofd.getFieldCount();i++){
                                                //这一行的长度正好是第i个字段的结束位置
                                                //发生换行的位置是第i个字段和第i+1个字段之间--这种情况对下一个字段进行补长度
                                                if(i<ofd.getFieldCount()-1){
                                                    if(ofd.getFieldList().at(i+1).getRowBeginIndex()==codecOFD->fromUnicode(lineQString).length()){
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        //对下一个字段按缺失的长度位置进行插入空格修补
                                                        nextLineByteArray=nextLineByteArray.insert(ofd.getFieldList().at(i+1).getLength()-1," ");
                                                        if(dataCompressLevel==0){
                                                            commonContentQByteArrayList.append(codecOFD->fromUnicode(lineQString).append(nextLineByteArray));
                                                        }
                                                        else{
                                                            commonContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(lineQString).append(nextLineByteArray),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        QApplication::setOverrideCursor(Qt::WaitCursor);
                                                        break;
                                                    }
                                                    //发生换行的位置是第i个字段,对第i个字段最后面补空格
                                                    else if(ofd.getFieldList().at(i+1).getRowBeginIndex()>codecOFD->fromUnicode(lineQString).length()){
                                                        QByteArray lineByteArray=codecOFD->fromUnicode(lineQString);
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        if(dataCompressLevel==0){
                                                            commonContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-1," "));
                                                        }
                                                        else{
                                                            commonContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-1," "),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        QApplication::setOverrideCursor(Qt::WaitCursor);
                                                        break;
                                                    }
                                                }
                                                //如果是最后一个字段中间有毛病
                                                else{
                                                    QByteArray lineByteArray=codecOFD->fromUnicode(lineQString);
                                                    QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                    if(dataCompressLevel==0){
                                                        commonContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).append(" "));
                                                    }
                                                    else{
                                                        commonContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).append(" "),dataCompressLevel));
                                                    }
                                                    mergeSucessFlag=true;
                                                    fileChanged=true;
                                                    QApplication::setOverrideCursor(Qt::WaitCursor);
                                                    break;
                                                }
                                            }
                                        }
                                        //替换了两个字符
                                        else if(codecOFD->fromUnicode(lineQString+nextLine).length()==(ofdrowLength-2)){
                                            for(int i=0;i<ofd.getFieldCount();i++){
                                                //这一行的长度正好是第i个字段的结束位置
                                                //发生换行的位置是第i个字段和第i+1个字段之间--这种情况对下一个字段进行补长度
                                                if(i<ofd.getFieldCount()-1){
                                                    if(ofd.getFieldList().at(i+1).getRowBeginIndex()==codecOFD->fromUnicode(lineQString).length()){
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        //对下一个字段按缺失的长度位置进行插入空格修补
                                                        nextLineByteArray=nextLineByteArray.insert(ofd.getFieldList().at(i+1).getLength()-2,"  ");
                                                        if(dataCompressLevel==0){
                                                            commonContentQByteArrayList.append(codecOFD->fromUnicode(lineQString).append(nextLineByteArray));
                                                        }
                                                        else{
                                                            commonContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(lineQString).append(nextLineByteArray),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        QApplication::setOverrideCursor(Qt::WaitCursor);
                                                        break;
                                                    }
                                                    //发生换行的位置是第i个字段,对第i个字段最后面补空格
                                                    else if(ofd.getFieldList().at(i+1).getRowBeginIndex()>codecOFD->fromUnicode(lineQString).length()){
                                                        QByteArray lineByteArray=codecOFD->fromUnicode(lineQString);
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        if(dataCompressLevel==0){
                                                            commonContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-2,"  "));
                                                        }
                                                        else{
                                                            commonContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-2,"  "),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        QApplication::setOverrideCursor(Qt::WaitCursor);
                                                        break;
                                                    }
                                                    //如果是最后一个字段中间有毛病
                                                    else{
                                                        QByteArray lineByteArray=codecOFD->fromUnicode(lineQString);
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        if(dataCompressLevel==0){
                                                            commonContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).append("  "));
                                                        }
                                                        else{
                                                            commonContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).append("  "),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        QApplication::setOverrideCursor(Qt::WaitCursor);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else{
                                            sucessFlag=false;
                                            mergeSucessFlag=false;
                                            currentOpenFileType=openFileType::NotFileOrErr;
                                            statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                            QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getRowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"]行长度为["+QString::number(lineQByteArray.length())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                            break;
                                        }
                                    }
                                    //如果放弃合并,则放弃读取
                                    else{
                                        sucessFlag=false;
                                        currentOpenFileType=openFileType::NotFileOrErr;
                                        statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getRowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"]行长度为["+QString::number(lineQByteArray.length())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                        break;
                                    }
                                }
                            }
                        }
                        lineNumber++;
                        //为了防止UI卡死,进行循环读取文件时,考虑支持下窗口事件接收
                        //每读取1000行更新下窗口事件
                        if((lineNumber%5000)==0){
                            statusBar_disPlayMessage(QString("文件读取中,已读取数据记录%1行").arg(QString::number(lineNumber)));
                            qApp->processEvents();
                            //强制终止事件-立即return退出
                            if(abortExit){
                                QApplication::restoreOverrideCursor();
                                qDebug()<<"强制立即终止任务";
                                return;
                            }
                        }
                    }
                    QApplication::restoreOverrideCursor();
                    dataFile.close();
                    //如果读取是成功的则开始解析
                    if(sucessFlag){
                        //是否发生了合并
                        if(mergeSucessFlag){
                            this->setWindowTitle(appName+"-"+currentFileName+"-文件修复待保存");
                        }
                        init_OFDTable();
                        //没读取到最后一行就遇到了结束标志
                        if(!readToLastRow){
                            QMessageBox::warning(this,tr("提示"),QString("\r\n\r\n当前打开的OFD/类OFD文件还没读取到最后一行就遇到了\"OFDCFEND\"行,所以提前结束了数据读取,\"OFDCFEND\"正常应该出现在最后一行作为文件结束行!!!\r\n\r\n"),QMessageBox::Ok,QMessageBox::Ok);
                        }
                        //读取到了最后一行,但是不是结束标志,提示最后一行是不是OFDCFEND
                        else if(!lastRowIsOFDCFEND){
                            QMessageBox::warning(this,tr("提示"),QString("\r\n\r\n当前打开的OFD/类OFD文件最后一行内容是\"%1\"不是\"OFDCFEND\",正常来说OFD/类OFD文件应该以\"OFDCFEND\"为结束行!!!\r\n\r\n").arg(ofdFooterQString),QMessageBox::Ok,QMessageBox::Ok);
                        }
                    }
                    //如果失败了,则释放内存
                    //此处很重要,是否启用文件另存使用了commonHeaderQStringList是否为空的判断
                    else{
                        commonHeaderQStringList.clear();
                        commonContentQByteArrayList.clear();
                        ofdFooterQString="";
                        ofdFooterRowNumber=0;
                    }
                }else{
                    currentOpenFileType=openFileType::NotFileOrErr;
                    statusBar_disPlayMessage("解析失败:文件读取失败,请重试...");
                    return;
                }
            }
            //如果没找到满足的解析配置
            else{
display_OFDFaultCause:
                currentOpenFileType=openFileType::NotFileOrErr;
                //显示失败原因
                if(faultList.count()>0){
                    //调用展示OFD加载失败的函数
                    statusBar_disPlayMessage("配置文件["+useini+"]无法解析该文件,请参阅错误提示检查文件或修正配置...");
                    display_OFDFaultCause(useini,faultList);
                }
                //如果未解析成功，且失败原因为0，则说明配置文件里压根没合适的配置
                else{
                    statusBar_disPlayMessage("配置文件["+useini+"]无法解析该文件,请检查文件或在配置文件中新增["+fileType+"]配置段...");
                }
                return;
            }
        }
        else{
            statusBar_disPlayMessage("文件第2行的版本号为["+versionFromFile+"]但是配置文件["+useini+"]不存在或者无有效配置,因此无法解析本文件,请新增配置后解析...");
            return;
        }
    }
    else{
        currentOpenFileType=openFileType::NotFileOrErr;
        statusBar_disPlayMessage("解析失败,文件打开失败...");
        return;
    }
}

/**
 * @brief MainWindow::load_csvFile加载CSV文件的函数
 * @param csvMatchList
 */
void MainWindow::load_csvFile(QList<matchIndex> csvMatchList){
    if(csvMatchList.count()<1){
        return;
    }
    //由于csv文件有可能同一个文件名，但是实际文件版本不同，所以需要做版本校验
    QFile dataFile(currentOpenFilePath);
    if(csvConfigList.count()<1){
        statusBar_disPlayMessage("配置目录中，无任何关于CSV文件的配置，请配置后再使用");
        return;
    }
    else{
        //检索匹配到的所有配置获取最大探测行号
        int aa=0;
        int rowNeedLoad=50;
        //识别最大化预加载行数，用于后续分析
        for(;aa<csvMatchList.count();aa++){
            //如果文件名可用且配置可用，则加入
            CsvFileDefinition csvDef=csvConfigList.at(csvMatchList.at(aa).configFileIndex).getConfigSegmentList().at(csvMatchList.at(aa).configSegmentIndex);
            //加载头部+尾部+10行buffer
            int j=csvDef.getDatabeginrowindex()+csvDef.getDatabeginrowindex()+10;
            if(j>rowNeedLoad){
                rowNeedLoad=j;
            }
        }
        QByteArray useForCharsetAnalyze;
        QList<QByteArray> useForFileAnalyze;
        if (dataFile.open(QFile::ReadOnly))
        {
            int row=0;
            while (!dataFile.atEnd()&&row<rowNeedLoad)
            {
                QByteArray rowdata=dataFile.readLine();
                //单行大于20480字节--拒绝读取
                if(rowdata.size()>163840){
                    dataFile.close();
                    statusBar_disPlayMessage("单行数据过长,无法正常解析...");
                    currentOpenFileType=openFileType::NotFileOrErr;
                    return;
                }
                useForCharsetAnalyze.append(rowdata);
                useForFileAnalyze.append(rowdata);
                row++;
            }
            dataFile.close();
            //如果一行数据都没读到，则提示是空文件，结束探测--不再做任何解析尝试
            if(row==0){
                dataFile.close();
                statusBar_disPlayMessage("空文件,没有任何数据记录可工解析");
                currentOpenFileType=openFileType::NotFileOrErr;
                return;
            }
        }
        else{
            int first = currentOpenFilePath.lastIndexOf ("/");
            QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
            statusBar_disPlayMessage("文件:"+fileName+"打开失败,请重试");
            currentOpenFileType=openFileType::NotFileOrErr;
            return;
        }
        currentOpenFileType=openFileType::CSVFile;
        //在这里记录下各个配置文件失败无法使用的原因，如果全部匹配失败无法解析，则给用户一个交代！
        QList<FaultCause> faultList;
        int dd=0;
        for(;dd<csvMatchList.count();dd++){
            //如果文件名可用且配置可用，则加入
            CsvFileDefinition csvDef=csvConfigList.at(csvMatchList.at(dd).configFileIndex).getConfigSegmentList().at(csvMatchList.at(dd).configSegmentIndex);
            if(csvDef.getUseAble()){
                int dataBeginRow=csvDef.getDatabeginrowindex();
                int endIgnore=csvDef.getEndIgnoreRow();
                QString filecharset=csvDef.getEcoding();
                //支持自动分析编码方式
                if(filecharset.toUpper()=="AUTO"){
                    //自动识别编码开始
                    filecharset=m_libucd.CharsetDetect(useForCharsetAnalyze);
                    qDebug()<<"基于配置自动识别出来的原始编码格式"<<filecharset;
                    //不在编码白名单的编码方式暂不支持，统一使用UTF-8解析
                    if(!allowCharsetList.contains(filecharset.toUpper())){
                        //不在允许的编码范围的默认转义
                        if(filecharset.toUpper()=="WINDOWS-1252"||filecharset.toUpper()=="WINDOWS-1251"||filecharset.toUpper()=="KOI8-R"){
                            filecharset="GB18030";
                        }
                        else{
                            filecharset="UTF-8";
                        }
                    }
                    //将gb18030大写
                    if(filecharset.toUpper()=="GB18030"){
                        filecharset="GB18030";
                    }
                }
                csvDef.setEcoding(filecharset);
                QTextCodec *codec=QTextCodec::codecForName(filecharset.toLocal8Bit());
                QList<QString>csvData;
                QString line;
                //按本配置解析文件内容
                int row=0;
                qDebug()<<"开始预加载文件内容判断是否符合本规则"<<filecharset;
                while (row<(dataBeginRow+endIgnore+10)&&row<useForFileAnalyze.count())
                {
                    line = codec->toUnicode(useForFileAnalyze.at(row));
                    line=line.remove('\r').remove('\n');
                    csvData.append(line);
                    row++;
                }
                //首行检查
                if(!csvDef.getFirstrowcheck().isEmpty()){
                    if(csvData.count()>0){
                        if(csvDef.getFirstrowcheck()!=csvData.at(0)){
                            FaultCause item;
                            item.setConfigIndex2(csvMatchList.at(dd));
                            item.setCause(QString("配置文件中标注文件第1行内容应该是[%1]，但是打开的文件第1行是[%2],不满足首行检查要求,无法确认该文件符合本解析规则").arg(csvDef.getFirstrowcheck(),csvData.at(0)));
                            faultList.append(item);
                            continue;
                        }
                    }
                }
                //尾行检查--只有空文件的时候才检查,因为文件很大的话可能压根没加载最后一行
                if(!csvDef.getLastrowcheck().isEmpty()){
                    //行数恰好满足0行数据要求
                    if(csvData.count()==(dataBeginRow+endIgnore-1)){
                        if(csvDef.getLastrowcheck()!=csvData.last()){
                            FaultCause item;
                            item.setConfigIndex2(csvMatchList.at(dd));
                            item.setCause(QString("配置文件中标注文件最后1行内容应该是[%1]，但是打开的文件最后1行是[%2],不满足尾行检查要求,无法确认该文件符合本解析规则").arg(csvDef.getLastrowcheck(),csvData.last()));
                            faultList.append(item);
                            continue;
                        }
                    }
                }
                //存在标题行----需要从文件内提取标题行
                if(csvDef.getTitlerowindex()>0){
                    //存在标题行但是文件内的数据行数低于标题所在行，无内容，无法判断
                    if(csvData.count()<csvDef.getTitlerowindex()){
                        FaultCause item;
                        item.setConfigIndex2(csvMatchList.at(dd));
                        item.setCause("未在本配置描述的文件第"+QString::number(csvDef.getTitlerowindex())+"行找到有效标题行,无法认定文件属于此类型");
                        faultList.append(item);
                        continue;
                    }
                    //存在标题行并且数据文件内也有标题行数据，比对标题有多少列，和定义的一致否
                    else{
                        QString titleRowString=csvData.at(csvDef.getTitlerowindex()-1);
                        if(titleRowString.isEmpty()){
                            FaultCause item;
                            item.setConfigIndex2(csvMatchList.at(dd));
                            item.setCause("在本配置描述的文件第"+QString::number(csvDef.getTitlerowindex())+"行找到的标题内容为空，无法提取接口文件列名信息");
                            faultList.append(item);
                            continue;
                        }
                        //如果需要忽略最后的分隔符
                        //注意不能只是减去最后一位，也许分隔符长度不是1呢
                        if(csvDef.getEndwithflag()=="1"){
                            titleRowString= titleRowString.left(titleRowString.length()-csvDef.getSplit().length());
                        }
                        QStringList fieldTitle;
                        //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                        if(csvDef.getClearQuotes()&&csvDef.getSplit().length()==1){
                            QRegExp rx("\\"+csvDef.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                            fieldTitle=titleRowString.split(rx);
                        }
                        else{
                            fieldTitle=titleRowString.split(csvDef.getSplit());
                        }
                        //如果文件字段数是-1.则代表根据标题或者第一行数据自动分析列数和列标题，以标题行或者第一行数据自己造标题数据为准来更新列数
                        if(csvDef.getFieldCount()==-1){
                            int countFromtitle=fieldTitle.count();
                            if(countFromtitle<2){
                                FaultCause item;
                                item.setConfigIndex2(csvMatchList.at(dd));
                                item.setCause("在本配置描述的文件第"+QString::number(csvDef.getTitlerowindex())+"行使用分隔符["+csvDef.getSplit()+"]找到的标题列数为小于2，可能不是该类型的文件");
                                faultList.append(item);
                                continue;
                            }
                            else{
                                //检查版本
                                if(csvDef.getVersioncheckrow()>0){
                                    if(csvData.count()<csvDef.getVersioncheckrow()){
                                        FaultCause item;
                                        item.setConfigIndex2(csvMatchList.at(dd));
                                        item.setCause("在本配置描述的文件第"+QString::number(csvDef.getVersioncheckrow())+"行为文件内版本检查行，但是打开的文件无法获取到本行数据，可能不是该类型的文件");
                                        faultList.append(item);
                                        continue;
                                    }
                                    else{
                                        if(!checkCSVVersion(csvDef,csvData.at(csvDef.getVersioncheckrow()-1))){
                                            if (csv.getVersioncheckmode()==0){
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause(QString("配置文件中标注文件第%1行内容应该是版本号[%2]，但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                                faultList.append(item);
                                                continue;
                                            }
                                            else {
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause(QString("配置文件中标注文件第%1行内容应该包含版本号[%2]，但是打开的文件第%1行是[%3],无法模糊匹配到版本号,无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                                faultList.append(item);
                                                continue;
                                            }
                                        }
                                    }
                                }
                                //开始加载配置文件
                                csv=csvDef;
                                //开始更新文件列数到临时csv配置变量
                                csv.setFieldCount(fieldTitle.count());
                                //开始更新列标题到临时csv配置变量
                                QList <CsvFieldDefinition> fieldList;
                                //10开始循环加载本文件类型的字段信息///////////////
                                for(int r=0;r<fieldTitle.count();r++){
                                    CsvFieldDefinition fieldItem;
                                    QString name=fieldTitle.at(r);
                                    if(name.isEmpty()){
                                        fieldItem.setFieldDescribe("未定义的字段名(第"+QString::number(r+1)+"列)");
                                    }
                                    else{
                                        fieldItem.setFieldDescribe(name);
                                    }
                                    //添加此字段信息到文件定义
                                    fieldList.append(fieldItem);
                                }
                                csv.setFieldList(fieldList);
                                load_csvFileData(fieldTitle);
                                return;
                            }
                        }
                        //如果定义的文件字段数和文件内的一致，则就是该版本的文件！
                        else if(fieldTitle.count()==csvDef.getFieldCount()){
                            if(csvDef.getVersioncheckrow()>0){
                                //版本检查
                                if(csvData.count()<csvDef.getVersioncheckrow()){
                                    FaultCause item;
                                    item.setConfigIndex2(csvMatchList.at(dd));
                                    item.setCause("在本配置描述的文件第"+QString::number(csvDef.getVersioncheckrow())+"行为文件内版本检查行，但是打开的文件无法获取到本行数据，可能不是该类型的文件");
                                    faultList.append(item);
                                    continue;
                                }
                                else{
                                    if(!checkCSVVersion(csvDef,csvData.at(csvDef.getVersioncheckrow()-1))){
                                        if (csv.getVersioncheckmode()==0){
                                            FaultCause item;
                                            item.setConfigIndex2(csvMatchList.at(dd));
                                            item.setCause(QString("配置文件中标注文件第%1行内容应该是版本号[%2]，但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                            faultList.append(item);
                                            continue;
                                        }
                                        else {
                                            FaultCause item;
                                            item.setConfigIndex2(csvMatchList.at(dd));
                                            item.setCause(QString("配置文件中标注文件第%1行内容应该包含版本号[%2]，但是打开的文件第%1行是[%3],无法模糊匹配到版本号,无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                            faultList.append(item);
                                            continue;
                                        }
                                    }
                                }
                            }
                            //开始加载数据
                            csv=csvDef;
                            load_csvFileData(fieldTitle);
                            return;
                        }
                        else{
                            FaultCause item;
                            item.setConfigIndex2(csvMatchList.at(dd));
                            item.setCause("在文件第"+QString::number(csvDef.getTitlerowindex())+"行找到的标题列数["+QString::number(fieldTitle.count())+"]和配置文件中的["+QString::number(csvDef.getFieldCount())+"]不一致,可能不是本版本的文件,无法解析,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                            faultList.append(item);
                            continue;
                        }
                    }
                }
                //如果不存在标题行---标题定义在配置文件
                else{
                    //文件内的行数<=文件头+文件尾部忽略行【既文件无实质性数据内容】
                    if (csvData.count()<=(dataBeginRow+endIgnore-1)){
                        if(csvDef.getVersioncheckrow()>0){
                            if(csvData.count()<csvDef.getVersioncheckrow()){
                                FaultCause item;
                                item.setConfigIndex2(csvMatchList.at(dd));
                                item.setCause(QString("在本配置描述的文件第%1行为文件内版本检查行,但是打开的文件只有%2行,无法获取到本行数据,可能不是该类型的文件").arg(QString::number(csvDef.getVersioncheckrow()),QString::number(csvData.count())));
                                faultList.append(item);
                                continue;
                            }
                            else{
                                if(!checkCSVVersion(csvDef,csvData.at(csvDef.getVersioncheckrow()-1))){
                                    if (csv.getVersioncheckmode()==0){
                                        FaultCause item;
                                        item.setConfigIndex2(csvMatchList.at(dd));
                                        item.setCause(QString("配置文件中标注文件第%1行内容应该是版本号[%2],但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                        faultList.append(item);
                                        continue;
                                    }
                                    else {
                                        FaultCause item;
                                        item.setConfigIndex2(csvMatchList.at(dd));
                                        item.setCause(QString("配置文件中标注文件第%1行内容应该包含版本号[%2],但是打开的文件第%1行是[%3],无法模糊匹配到版本号,无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                        faultList.append(item);
                                        continue;
                                    }
                                }
                                //空文件，版本号又对
                                else{
                                    //文件记录数正好等于空文件行数
                                    if(csvData.count()==(dataBeginRow+endIgnore-1)){
                                        //开始加载配置文件
                                        csv=csvDef;
                                        QStringList fieldTitle;
                                        //开始循环设置本文件类型的字段信息///////////////
                                        if(csv.getFieldCount()>0){
                                            QStringList fieldTitle;
                                            for(int tc=0;tc<csv.getFieldList().count();tc++){
                                                fieldTitle.append(csv.getFieldList().at(tc).getFieldDescribe());
                                            }
                                            load_csvFileData(fieldTitle);
                                            return;
                                        }
                                        //空文件,且自动获取列数--这个时候需要尝试去文件中获取列信息--基于第一行数据
                                        else{
                                            if(csvData.count()>=csvDef.getDatabeginrowindex()){
                                                QString firstDataRowString=csvData.at(csvDef.getDatabeginrowindex()-1);
                                                //如果需要忽略最后一个多余的分隔符
                                                if(csvDef.getEndwithflag()=="1"){
                                                    firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                                                }
                                                int fieldCountFirstRow=0;
                                                //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                                                if(csvDef.getClearQuotes()&&csvDef.getSplit().length()==1){
                                                    QRegExp rx("\\"+csvDef.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                                                    fieldCountFirstRow=firstDataRowString.split(rx).count();
                                                }
                                                else{
                                                    fieldCountFirstRow=firstDataRowString.split(csvDef.getSplit()).count();
                                                }
                                                if(fieldCountFirstRow<2){
                                                    FaultCause item;
                                                    item.setConfigIndex2(csvMatchList.at(dd));
                                                    item.setCause("在本配置描述的文件第"+QString::number(csvDef.getTitlerowindex())+"行使用分隔符["+csvDef.getSplit()+"]找到的数据列数小于2,无法断定为该类型的文件...");
                                                    faultList.append(item);
                                                    continue;
                                                }
                                                else{
                                                    //开始加载配置文件
                                                    csv=csvDef;
                                                    //开始更新文件列数到临时csv配置变量
                                                    csv.setFieldCount(fieldCountFirstRow);
                                                    //开始更新列标题到临时csv配置变量
                                                    QList <CsvFieldDefinition> fieldList;
                                                    QStringList fieldTitle;
                                                    //开始循环设置本文件类型的字段信息///////////////
                                                    //对于不带标题的并且字段数是auto的，直接设置标题
                                                    for(int r=0;r<fieldCountFirstRow;r++){
                                                        CsvFieldDefinition fieldItem;
                                                        QString name="未定义的字段名(第"+QString::number(r+1)+"列)";
                                                        fieldTitle.append(name);
                                                        fieldItem.setFieldDescribe(name);
                                                        //添加此字段信息到文件定义
                                                        fieldList.append(fieldItem);
                                                    }
                                                    csv.setFieldList(fieldList);
                                                    load_csvFileData(fieldTitle);
                                                    return;
                                                }
                                            }
                                            else{
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause("文件内无标题行且字段数配置配置为AUTO的情况下,至少需要一行数据内容才能分析文件是否满足此规则...");
                                                faultList.append(item);
                                                continue;
                                            }
                                        }
                                    }
                                    else {
                                        FaultCause item;
                                        item.setConfigIndex2(csvMatchList.at(dd));
                                        item.setCause(QString("根据此解析规则,空文件至少也应该有%1行,%2行文件头,%3行文件尾~~,但是目前文件只有%4行,无法断定打开的文件满足此解析规则...").arg(QString::number(dataBeginRow+endIgnore-1),QString::number(dataBeginRow-1),QString::number(endIgnore),QString::number(csvData.count())));
                                        faultList.append(item);
                                        continue;
                                    }
                                }
                            }
                        }
                        else{
                            FaultCause item;
                            item.setConfigIndex2(csvMatchList.at(dd));
                            item.setCause("基于该配置文件解析当前文件得出的结论是:当前文件应该是一个不含数据记录的空文件并且未开启版本检查,程序无法确认本配置符合这个文件的解析规则,程序至少需要一行数据去评估是否满足本解析规则...");
                            faultList.append(item);
                            continue;
                        }
                    }
                    else{
                        //有数据的第一种情况
                        //配置不带标题行，但是字段数又是auto的场景
                        if(csvDef.getFieldCount()==-1){
                            QString firstDataRowString=csvData.at(csvDef.getDatabeginrowindex()-1);
                            //如果需要忽略最后一个多余的分隔符
                            if(csvDef.getEndwithflag()=="1"){
                                firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                            }
                            int fieldCountFirstRow=0;
                            //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                            if(csvDef.getClearQuotes()&&csvDef.getSplit().length()==1){
                                QRegExp rx("\\"+csvDef.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                                fieldCountFirstRow=firstDataRowString.split(rx).count();
                            }
                            else{
                                fieldCountFirstRow=firstDataRowString.split(csvDef.getSplit()).count();
                            }
                            if(fieldCountFirstRow<2){
                                FaultCause item;
                                item.setConfigIndex2(csvMatchList.at(dd));
                                item.setCause("在本配置描述的文件第"+QString::number(csvDef.getTitlerowindex())+"行使用分隔符["+csvDef.getSplit()+"]找到的数据列数小于2,无法断定为该类型的文件");
                                faultList.append(item);
                                continue;
                            }
                            else{
                                if(csvDef.getVersioncheckrow()>0){
                                    if(csvData.count()<csvDef.getVersioncheckrow()){
                                        FaultCause item;
                                        item.setConfigIndex2(csvMatchList.at(dd));
                                        item.setCause("在本配置描述的文件第"+QString::number(csvDef.getVersioncheckrow())+"行为文件内版本检查行,但是打开的文件无法获取到本行数据,可能不是该类型的文件");
                                        faultList.append(item);
                                        continue;
                                    }
                                    else{
                                        if(!checkCSVVersion(csvDef,csvData.at(csvDef.getVersioncheckrow()-1))){
                                            if (csv.getVersioncheckmode()==0){
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause(QString("配置文件中标注文件第%1行内容应该是版本号[%2],但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                                faultList.append(item);
                                                continue;
                                            }
                                            else {
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause(QString("配置文件中标注文件第%1行内容应该包含版本号[%2],但是打开的文件第%1行是[%3],无法模糊匹配到版本号,无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                                faultList.append(item);
                                                continue;
                                            }
                                        }
                                    }
                                }
                                //开始加载配置文件
                                csv=csvDef;
                                //开始更新文件列数到临时csv配置变量
                                csv.setFieldCount(fieldCountFirstRow);
                                //开始更新列标题到临时csv配置变量
                                QList <CsvFieldDefinition> fieldList;
                                QStringList fieldTitle;
                                //开始循环设置本文件类型的字段信息///////////////
                                //对于不带标题的并且字段数是auto的，直接设置标题
                                for(int r=0;r<fieldCountFirstRow;r++){
                                    CsvFieldDefinition fieldItem;
                                    QString name="未定义的字段名(第"+QString::number(r+1)+"列)";
                                    fieldTitle.append(name);
                                    fieldItem.setFieldDescribe(name);
                                    //添加此字段信息到文件定义
                                    fieldList.append(fieldItem);
                                }
                                csv.setFieldList(fieldList);
                                load_csvFileData(fieldTitle);
                                return;
                            }
                        }
                        //有数据的第2种场景
                        //存在明确的字段数定义在配置文件中时
                        else{
                            QString firstDataRowString=csvData.at(csvDef.getDatabeginrowindex()-1);
                            //如果需要忽略最后一个多余的分隔符
                            if(csvDef.getEndwithflag()=="1"){
                                firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                            }
                            int fieldCount=0;
                            //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                            if(csvDef.getClearQuotes()&&csvDef.getSplit().length()==1){
                                QRegExp rx("\\"+csvDef.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                                fieldCount=firstDataRowString.split(rx).count();
                            }
                            else{
                                fieldCount=firstDataRowString.split(csvDef.getSplit()).count();
                            }
                            //如果第一行数据的文件字段数和文件内的一致，则就是该版本的文件！
                            if(fieldCount==csvDef.getFieldCount()){
                                if(csvDef.getVersioncheckrow()>0){
                                    if(csvData.count()<csvDef.getVersioncheckrow()){
                                        FaultCause item;
                                        item.setConfigIndex2(csvMatchList.at(dd));
                                        item.setCause("在本配置描述的文件第"+QString::number(csvDef.getVersioncheckrow())+"行为文件内版本检查行，但是打开的文件无法获取到本行数据，可能不是该类型的文件");
                                        faultList.append(item);
                                        continue;
                                    }
                                    else{
                                        if(!checkCSVVersion(csvDef,csvData.at(csvDef.getVersioncheckrow()-1))){
                                            if (csv.getVersioncheckmode()==0){
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause(QString("配置文件中标注文件第%1行内容应该是版本号[%2],但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                                faultList.append(item);
                                                continue;
                                            }
                                            else {
                                                FaultCause item;
                                                item.setConfigIndex2(csvMatchList.at(dd));
                                                item.setCause(QString("配置文件中标注文件第%1行内容应该包含版本号[%2],但是打开的文件第%1行是[%3],无法模糊匹配到版本号,无法确认该文件符合本解析规则").arg(QString::number(csvDef.getVersioncheckrow()),csvDef.getVersion(),csvData.at(csvDef.getVersioncheckrow()-1)));
                                                faultList.append(item);
                                                continue;
                                            }
                                        }
                                    }
                                }
                                //开始加载数据
                                csv=csvDef;
                                //文件内没标题的从配置读取
                                QStringList fieldTitle;
                                for(int tc=0;tc<csv.getFieldList().count();tc++){
                                    fieldTitle.append(csv.getFieldList().at(tc).getFieldDescribe());
                                }
                                load_csvFileData(fieldTitle);
                                return;
                            }
                            else{
                                FaultCause item;
                                item.setConfigIndex2(csvMatchList.at(dd));
                                item.setCause("在文件第"+QString::number(csvDef.getDatabeginrowindex())+"行(基于该配置的第一行有效数据)找到的数据有["+QString::number(fieldCount)+"]列数据,和配置文件中定义的列数["+QString::number(csvDef.getFieldCount())+"]不一致，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                                faultList.append(item);
                                continue;
                            }
                        }
                    }
                }
            }
            else{
                FaultCause item;
                item.setConfigIndex2(csvMatchList.at(dd));
                item.setCause("配置文件不可用");
                faultList.append(item);
            }
        }
        display_CSVFaultCause(faultList);
        //如果遍历完还没找到合适的配置，则提示
        currentOpenFileType=openFileType::NotFileOrErr;
        statusBar_disPlayMessage("解析失败,请查看具体解析失败原因后修正配置或者确认是否文件格式错误");
        return;
    }
}

/**
 * @brief MainWindow::load_fixedFile加载定长文件的函数
 * @param csvMatchList
 */
void MainWindow::load_fixedFile(QList<matchIndex> fixedMatchList){
    if(fixedMatchList.count()<1){
        return;
    }
    //由于fixed文件有可能同一个文件名，但是实际文件版本不同，所以需要做版本校验
    QFile dataFile(currentOpenFilePath);
    //关于定长文件类型可用的版本
    if(fixedConfigList.count()<1){
        statusBar_disPlayMessage("配置目录中，无任何关于定长文件的配置，请配置后再使用");
        return;
    }
    else{
        //检索匹配到的所有配置获取最大探测行号
        int aa=0;
        int rowNeedLoad=50;
        //识别最大化预加载行数，用于后续分析
        for(;aa<fixedMatchList.count();aa++){
            //如果文件名可用且配置可用，则加入
            FIXEDFileDefinition fixedDef=fixedConfigList.at(fixedMatchList.at(aa).configFileIndex).getConfigSegmentList().at(fixedMatchList.at(aa).configSegmentIndex);
            //加载头部+尾部+10行buffer
            int j=fixedDef.getDataRowBeginIndex()+fixedDef.getEndIgnoreRow()+10;
            if(j>rowNeedLoad){
                rowNeedLoad=j;
            }
        }
        QList<QByteArray> useForFileAnalyze;
        if (dataFile.open(QFile::ReadOnly))
        {
            int row=0;
            while (!dataFile.atEnd()&&row<rowNeedLoad)
            {
                QByteArray rowdata=dataFile.readLine();
                //单行大于20480字节--拒绝读取
                if(rowdata.size()>163840){
                    dataFile.close();
                    statusBar_disPlayMessage("单行数据过长,无法正常解析...");
                    currentOpenFileType=openFileType::NotFileOrErr;
                    return;
                }
                useForFileAnalyze.append(rowdata);
                row++;
            }
            dataFile.close();
            //如果一行数据都没读到，则提示是空文件，结束探测--不再做任何解析尝试
            if(row==0){
                dataFile.close();
                statusBar_disPlayMessage("空文件,没有任何数据记录可工解析");
                currentOpenFileType=openFileType::NotFileOrErr;
                return;
            }
        }
        else{
            int first = currentOpenFilePath.lastIndexOf ("/");
            QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
            statusBar_disPlayMessage("文件:"+fileName+"打开失败,请重试");
            currentOpenFileType=openFileType::NotFileOrErr;
            return;
        }
        currentOpenFileType=openFileType::FIXEDFile;
        //在这里记录下各个配置文件失败无法使用的原因，如果全部匹配失败无法解析，则给用户一个交代！
        QList<FaultCause> faultList;
        int dd=0;
        for(;dd<fixedMatchList.count();dd++){
            //如果文件名可用且配置可用，则加入
            FIXEDFileDefinition fixedDef=fixedConfigList.at(fixedMatchList.at(dd).configFileIndex).getConfigSegmentList().at(fixedMatchList.at(dd).configSegmentIndex);
            if(fixedDef.getUseAble()){
                int dataBeginRow=fixedDef.getDataRowBeginIndex();
                int endIgnore=fixedDef.getEndIgnoreRow();
                //查找可用配置结束，开始分析文件到底是哪个版本
                //最大化获取文件内的样本数据
                QTextCodec *codec=QTextCodec::codecForName(fixedDef.getEcoding().toLocal8Bit());
                QList<QString>fixedData;
                QString line;
                //当前读取到的行数
                int row=0;
                qDebug()<<"开始预加载文件内容判断是否符合本规则";
                while (row<(dataBeginRow+endIgnore+10)&&row<useForFileAnalyze.count())
                {
                    line = codec->toUnicode(useForFileAnalyze.at(row));
                    line=line.remove('\r').remove('\n');
                    fixedData.append(line);
                    row++;
                }
                //首先进行一系列的文件头校验,这是1.9.20版本新增的功能
                //首行数据是否满足的校验
                if((!fixedDef.getFirstrowcheck().isEmpty())&&fixedData.count()>0&&fixedDef.getFirstrowcheck()!=fixedData.at(0)){
                    FaultCause item;
                    item.setConfigIndex2(fixedMatchList.at(dd));
                    item.setCause(QString("配置文件中标注文件第1行内容应该是[%1]，但是打开的文件第1行是[%2],无法确认该文件符合本解析规则").arg(fixedDef.getFirstrowcheck(),fixedData.at(0)));
                    faultList.append(item);
                    continue;
                }
                //文件的版本是否满足的校验--版本号trim
                if(fixedDef.getVersioncheckrow()>0&&fixedData.count()>=fixedDef.getVersioncheckrow()){
                    if(fixedDef.getVersioncheckmode()==0){
                        if(fixedDef.getVersion()!=fixedData.at(fixedDef.getVersioncheckrow()-1).trimmed()){
                            FaultCause item;
                            item.setConfigIndex2(fixedMatchList.at(dd));
                            item.setCause(QString("配置文件中标注文件第%1行内容应该是版本号[%2]，但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(fixedDef.getVersioncheckrow()),fixedDef.getVersion(),fixedData.at(fixedDef.getVersioncheckrow()-1)));
                            faultList.append(item);
                            continue;
                        }
                    }
                    else{
                        if(!fixedData.at(fixedDef.getVersioncheckrow()-1).trimmed().contains(fixedDef.getVersion(),Qt::CaseSensitive)){
                            FaultCause item;
                            item.setConfigIndex2(fixedMatchList.at(dd));
                            item.setCause(QString("配置文件中标注文件第%1行内容应该包含版本号[%2]，但是打开的文件第%1行是[%3],无法确认该文件符合本解析规则").arg(QString::number(fixedDef.getVersioncheckrow()),fixedDef.getVersion(),fixedData.at(fixedDef.getVersioncheckrow()-1)));
                            faultList.append(item);
                            continue;
                        }
                    }
                }
                //字段总数校验
                if(fixedDef.getFieldcountcheckrow()>0&&fixedData.count()>=fixedDef.getFieldcountcheckrow()){
                    QString fieldCountFromFile=fixedData.at(fixedDef.getFieldcountcheckrow()-1);
                    bool flag=false;
                    int fileCount =fieldCountFromFile.trimmed().toInt(&flag);
                    //此行数据可以转换成数值
                    if(flag){
                        //不满足字段数check
                        if(fileCount!=fixedDef.getFieldCountMax()){
                            FaultCause item;
                            item.setConfigIndex2(fixedMatchList.at(dd));
                            item.setCause(QString("配置文件中标注文件第%1行内容应该是字段数[%2]，但是打开的文件第%1行是[%3](数值已清除空格和前缀0),文件实际字段数和该解析规则定义的不一致,无法确认该文件符合本解析规则").arg(QString::number(fixedDef.getFieldcountcheckrow()),QString::number(fixedDef.getFieldCountMax()),QString::number(fileCount)));
                            faultList.append(item);
                            continue;
                        }
                    }
                    //呵呵，无法转换
                    else{
                        FaultCause item;
                        item.setConfigIndex2(fixedMatchList.at(dd));
                        item.setCause(QString("配置文件中标注文件第%1行内容应该是字段数，但是打开的文件第%1行是[%2],该内容不是一个有效的数值,无法确认该文件符合本解析规则").arg(QString::number(fixedDef.getFieldcountcheckrow()),fixedData.at(fixedDef.getFieldcountcheckrow()-1)));
                        faultList.append(item);
                        continue;
                    }
                }
                //字段明细检查
                if(fixedDef.getFielddetailcheckbeginrow()>0){
                    //最低要有这么多行数据,从字段明细第一行开始+字段数,不然没法比较啊
                    int fileRowMinNeed=fixedDef.getFielddetailcheckbeginrow()+fixedDef.getFieldCountMax();
                    if(fixedData.count()<fileRowMinNeed){
                        FaultCause item;
                        item.setConfigIndex2(fixedMatchList.at(dd));
                        item.setCause(QString("配置文件中标注文件从第%1行开始的共计%2行是文件内字段明细列表,但是打开的文件总行数小于字段明细结束行,无法进行字段明细检查,无法确认该文件符合本解析规则").arg(QString::number(fixedDef.getFielddetailcheckbeginrow()),fixedDef.getFieldCountMax()));
                        faultList.append(item);
                        continue;
                    }
                    else{
                        //开始进行字段明细检查
                        int fiexdIndex=fixedDef.getFielddetailcheckbeginrow()-1;
                        int fiexdIndexEnd=fixedDef.getFielddetailcheckbeginrow()-1+fixedDef.getFieldCountMax();
                        int fieldIndex=0;
                        bool checkOK=true;
                        for(;fiexdIndex<=fiexdIndexEnd&&fieldIndex<fixedDef.getFieldList().count();){
                            //字段名容忍大小写不一致
                            if(fixedDef.getFieldList().at(fieldIndex).getFieldName().toUpper()!=fixedData.at(fiexdIndex).toUpper()){
                                checkOK=false;
                                FaultCause item;
                                item.setConfigIndex2(fixedMatchList.at(dd));
                                item.setCause(QString("配置文件中标注文件第%1行内容应该是字段描述[%2]，但是打开的文件第%1行是[%3],实际文件的字段描述和该解析规则定义的不一致,无法确认该文件符合本解析规则").arg(QString::number(fiexdIndex+1),fixedDef.getFieldList().at(fieldIndex).getFieldName(),fixedData.at(fiexdIndex)));
                                faultList.append(item);
                                break;
                            }
                            fiexdIndex++;
                            fieldIndex++;
                        }
                        //检查ok的情况下
                        if(checkOK){
                            //再判断下是不是说根据这个解析规则来看,这是一个空文件,如果是，则直接解析
                            //标准的空文件是数据开始行-1+尾部忽略行,即一行数据也没有但是结构完整,这里不做强制要求,所以条件设置为<=
                            if(fixedData.count()<=(dataBeginRow+endIgnore-1)){
                                qDebug()<<"空文件解析";
                                fixed=fixedDef;
                                load_fixedFileData();
                                return;
                            }
                        }
                        //检查中遇到字段不吻合,直接跳到下一个配置
                        else{
                            continue;
                        }
                    }
                }
                //文件内的数据行数低于第一行数据，无内容，无法判断
                //开启字段明细检查的文件,其实空文件已经在上面文件字段明细检查时拦截了
                if(fixedData.count()<dataBeginRow){
                    FaultCause item;
                    item.setConfigIndex2(fixedMatchList.at(dd));
                    item.setCause("配置文件中标注文件第"+QString::number(dataBeginRow)+"行开始是数据行了，但是打开的文件只有["+QString::number(fixedData.count())+"]行哟，无有效数据,无法确认该文件符合本解析规则");
                    faultList.append(item);
                    continue;
                }
                //文件内的行数<文件头+文件尾部忽略行【既文件无实质性数据内容】
                else if(fixedData.count()<=(dataBeginRow+endIgnore-1)){
                    FaultCause item;
                    item.setConfigIndex2(fixedMatchList.at(dd));
                    item.setCause("基于该配置文件解析当前文件得出的结论是:当前文件应该是一个不含数据记录的空文件,请确认是否1行数据记录都没有(定长文件解析不支持在未配置字段明细检查的情况下进行空文件解析,工具需要根据首行数据内容识别文件类别)....");
                    faultList.append(item);
                    continue;
                }
                QString firstDataRowString=fixedData.at(fixedDef.getDataRowBeginIndex()-1);
                //如果第一行数据的长度在可接受的长度内，则允许解析
                //判断是字节定长还是字符定长
                //字节定长
                if(fixedDef.getFieldlengthtype()==0){
                    int rowLength=codec->fromUnicode(firstDataRowString).length();
                    //如果行长度包含这种长度
                    if(fixedDef.getRowLengthHash().contains(rowLength)){
                        //开始加载数据
                        fixed=fixedDef;
                        load_fixedFileData();
                        return;
                    }
                    else{
                        FaultCause item;
                        item.setConfigIndex2(fixedMatchList.at(dd));
                        item.setCause("在文件第"+QString::number(fixedDef.getDataRowBeginIndex())+"行找到的数据行长度为["+QString::number(rowLength)+"]字节,配置文件无法识别，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                        faultList.append(item);
                        continue;
                    }
                }
                //字符定长
                else{
                    int rowLength=firstDataRowString.length();
                    //如果行长度包含这种长度
                    if(fixedDef.getRowLengthHash().contains(rowLength)){
                        //开始加载数据
                        fixed=fixedDef;
                        load_fixedFileData();
                        return;
                    }
                    else{
                        FaultCause item;
                        item.setConfigIndex2(fixedMatchList.at(dd));
                        item.setCause("在文件第"+QString::number(fixedDef.getDataRowBeginIndex())+"行找到的数据行长度为["+QString::number(rowLength)+"]字符,配置文件无法识别，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                        faultList.append(item);
                        continue;
                    }
                }
            }
            else{
                FaultCause item;
                item.setConfigIndex2(fixedMatchList.at(dd));
                item.setCause("配置文件不可用");
                faultList.append(item);
            }
        }
        display_FIXEDFaultCause(faultList);
        //如果遍历完还没找到合适的配置，则提示
        currentOpenFileType=openFileType::NotFileOrErr;
        statusBar_disPlayMessage("解析失败,请查看具体解析失败原因后修正配置或者确认是否文件格式错误");
        return;
    }
}

/**
 * @brief MainWindow::load_fixedFileData识别出来定长文件类型后，加载定长文件数据的函数
 */
void MainWindow::load_fixedFileData(){
    //使用的ini配置
    ui->lineEditUseIni->setText(fixed.getFileIni());
    ui->lineEditUseIni->setToolTip(fixed.getFileIni());
    //文件类型描述
    ui->lineEditFileDescribe->setText(fixed.getFileDescribe());
    ui->lineEditFileDescribe->setToolTip(fixed.getFileDescribe());
    //文件类型描述
    ui->lineEditFileType->setText("字段定长");

    //当加载的文件类别是fixed时，传递日期栏目改为文件正则匹配到的配置;
    ui->labelFileTransferDate->setText("解析配置:");
    ui->lineEditFileTransferDate->setText(fixed.getConfigSegment());
    QList<int > rowLengthList;
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly))
    {
        QTextCodec *codec=(QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()));
        QByteArray lineQByteArray;
        QString lineQString;
        int lineNumber=0;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        while (!dataFile.atEnd())
        {
            lineQByteArray=dataFile.readLine();
            if(dataFile.atEnd()){
                lineQString = codec->toUnicode(lineQByteArray);
                if(lineQString.endsWith("\r\n")){
                    lastRowHasNewLine=true;
                    lineQString=lineQString.remove("\r\n");
                }
                else if(lineQString.endsWith("\r")){
                    lastRowHasNewLine=true;
                    lineQString=lineQString.remove("\r");
                }
                else if(lineQString.endsWith("\n")){
                    lastRowHasNewLine=true;
                    lineQString=lineQString.remove("\n");
                }
                else{
                    lastRowHasNewLine=false;
                }
            }
            else{
                lineQString = codec->toUnicode(lineQByteArray).remove('\r').remove('\n');
            }
            if(lineNumber<fixed.getDataRowBeginIndex()-1){
                commonHeaderQStringList.append(lineQString);
            }
            else{
                //切记注意，在这里我们使用数据压缩算法对数据进行压缩---获取的时候需解压使用
                if(dataCompressLevel==0){
                    commonContentQByteArrayList.append(codec->fromUnicode(lineQString));
                }
                else{
                    commonContentQByteArrayList.append(qCompress(codec->fromUnicode(lineQString),dataCompressLevel));
                }
                //记录每行长度
                if(fixed.getFieldlengthtype()==0){
                    rowLengthList.append(codec->fromUnicode(lineQString).length());
                }
                else{
                    rowLengthList.append(lineQString.length());
                }
            }
            lineNumber++;
            //为了防止UI卡死,进行循环读取文件时,考虑支持下窗口事件接收
            //每读取1000行更新下窗口事件
            if((lineNumber%1000)==0){
                statusBar_disPlayMessage(QString("文件读取中,已读取数据记录%1行").arg(QString::number(lineNumber)));
                qApp->processEvents();
                //强制终止事件-立即return退出
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        QApplication::restoreOverrideCursor();
        dataFile.close();
        //从文件读取到内存后，修正文件内容
        //文件尾部提取-兼容空文件
        //1.9.20版本新增保护功能，如果配置了忽略xx行文件尾,但是探测最后一行数据时如果长度=配置的长度,则认为最后一行是数据,则不再去除,以免文件缺失文件尾的时候解析少了一行数据
        if(fixed.getEndIgnoreRow()>0&&commonContentQByteArrayList.count()>0){
            int lastRowLenght=0;
            QString lastlineUnicode=lineQString;
            //记录每行长度
            if(fixed.getFieldlengthtype()==0){
                lastRowLenght=codec->fromUnicode(lastlineUnicode).length();
            }
            else{
                lastRowLenght=lastlineUnicode.length();
            }
            if(fixed.getRowLengthHash().contains(lastRowLenght)){
                //最后一行疑似是数据,不再去除文件尾
            }
            else{
                if(commonContentQByteArrayList.count()>=fixed.getEndIgnoreRow()){
                    for(int ic=1;ic<=fixed.getEndIgnoreRow();ic++){
                        if(dataCompressLevel==0){
                            commonFooterQStringList.insert(0,codec->toUnicode(commonContentQByteArrayList.last()));
                        }
                        else{
                            commonFooterQStringList.insert(0,codec->toUnicode(qUncompress(commonContentQByteArrayList.last())));
                        }
                        commonContentQByteArrayList.removeLast();
                        rowLengthList.removeLast();
                    }
                }
            }
        }
        //校验文件体的每一行的长度
        int count=rowLengthList.count();
        //空文件不校验
        qDebug()<<commonContentQByteArrayList.count();
        if(commonContentQByteArrayList.count()>0){
            for(int row=0;row<count;row++){
                if(!fixed.getRowLengthHash().contains(rowLengthList.at(row))){
                    QString lengths="";
                    QList <int> length=fixed.getRowLengthHash().keys();
                    lengths.append(QString::number(length.at(0))).append("(").append(QString::number(fixed.getRowLengthHash().value(length.at(0)))).append("个字段)");
                    if(length.count()>1){
                        for(int ff=1;ff<length.count();ff++){
                            lengths.append("/").append(QString::number(length.at(ff))).append("(").append(QString::number(fixed.getRowLengthHash().value(length.at(ff)))).append("个字段)");
                        }
                    }
                    if(fixed.getFieldlengthtype()==0){
                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n["+fixed.getFileNameWithVersion()+"]中定义的记录长度和打开的文件中不一致,解析失败\r\n["+fixed.getFileNameWithVersion()+"]中文件定义的数据行长度可以为["+lengths+"]字节\r\n实际打开的文件中第["+QString::number(row+1+commonHeaderQStringList.count())+"]行长度为["+QString::number(rowLengthList.at(row))+"]\r\n请检查是否是文件错误,或者配置定义错误,或者你打开的是别的版本的文件(如果是请先配置!)",QMessageBox::Ok,QMessageBox::Ok);
                    }
                    else{
                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n["+fixed.getFileNameWithVersion()+"]中定义的记录长度和打开的文件中不一致,解析失败\r\n["+fixed.getFileNameWithVersion()+"]中文件定义的数据行长度可以为["+lengths+"]字符\r\n实际打开的文件中第["+QString::number(row+1+commonHeaderQStringList.count())+"]行长度为["+QString::number(rowLengthList.at(row))+"]\r\n请检查是否是文件错误,或者配置定义错误,或者你打开的是别的版本的文件(如果是请先配置!)",QMessageBox::Ok,QMessageBox::Ok);
                    }
                    statusBar_disPlayMessage("文件解析失败!");
                    currentOpenFileType=openFileType::NotFileOrErr;
                    commonHeaderQStringList.clear();
                    commonContentQByteArrayList.clear();
                    commonFooterQStringList.clear();
                    return;
                }
            }
        }
        //如果文件没问题，开始初始化表格
        init_FIXEDTable();
        //文件后置校验
        QString warn="";
        if(!fixed.getLastrowcheck().isEmpty()&&commonFooterQStringList.count()>0){
            if(commonFooterQStringList.last()!=fixed.getLastrowcheck()){
                warn=warn+QString("\r\n\r\n当前打开的文件最后一行内容是\"%1\"不是\"%2\",根据配置的解析规则,正常来说此文件应该以\"%2\"为结束行!!!\r\n\r\n").arg(commonFooterQStringList.last(),fixed.getLastrowcheck());
            }
        }
        else if(!fixed.getLastrowcheck().isEmpty()&&commonFooterQStringList.count()==0){
            warn=warn+QString("\r\n\r\n根据配置的解析规则,正常来说此文件应该以\"%1\"为结束行,当前打开的文件疑似缺少此结束标志!!!\r\n\r\n").arg(fixed.getLastrowcheck());
        }
        //需要添加什么新的后置校验提示，可以添加到这里
        if(!warn.isEmpty()){
            QMessageBox::warning(this,tr("提示"),warn,QMessageBox::Ok,QMessageBox::Ok);
        }
    }
}

/**
 * @brief MainWindow::display_CSVFaultCause当加载csv文件失败后，显示失败原因的函数
 * @param faultList
 */
void MainWindow::display_CSVFaultCause(QList<FaultCause> faultList){
    if(faultList.count()>0){
        //配置文件，配置文件是否可用，配置文件不可用原因，分隔符，是不是使用分隔符做行尾部，标题行，数据起始行，编码信息，字段总和，解析失败原因
        int colCount=12;
        int rowCount=faultList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        title.append("CSV配置文件");
        title.append("文件匹配解析器");
        title.append("文件用途解释");
        title.append("配置文件是否可用");
        title.append("配置文件不可用原因");
        title.append("字段分隔符");
        title.append("每行行尾以分隔符结束");
        title.append("字段标题所在行");
        title.append("数据内容起始行");
        title.append("文件编码方式");
        title.append("每行字段数");
        title.append("解析匹配失败原因");
        ptr_table->setHorizontalHeaderLabels(title);
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置换行
        ptr_table->setWordWrap(true);
        ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
        ptr_table->setColumnWidth(11,300);
        ptr_table->horizontalHeader()->setSectionResizeMode(11, QHeaderView::Stretch);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            CsvFileDefinition csvfileDefinition=csvConfigList.at(faultList.at(row).getConfigIndex2().configFileIndex).getConfigSegmentList().at(faultList.at(row).getConfigIndex2().configSegmentIndex);
            //配置文件
            if(!csvfileDefinition.getFileIni().isEmpty()){
                QTableWidgetItem *item0= new QTableWidgetItem(csvfileDefinition.getFileIni());
                ptr_table->setItem(row, 0, item0);
            }
            //解析器
            if(!csvfileDefinition.getConfigSegment().isEmpty()){
                QTableWidgetItem *item1= new QTableWidgetItem(csvfileDefinition.getConfigSegment());
                ptr_table->setItem(row, 1, item1);
            }
            //文件解释
            if(!csvfileDefinition.getFileDescribe().isEmpty()){
                QTableWidgetItem *item2= new QTableWidgetItem(csvfileDefinition.getFileDescribe());
                ptr_table->setItem(row, 2, item2);
            }
            //配置是否可用
            QString u="不可用";
            if(csvfileDefinition.getUseAble()){
                u="可用";
            }
            QTableWidgetItem *item3= new QTableWidgetItem(u);
            ptr_table->setItem(row, 3, item3);
            //不可用原因
            if(!csvfileDefinition.getMessage().isEmpty()){
                QTableWidgetItem *item4= new QTableWidgetItem(csvfileDefinition.getMessage());
                ptr_table->setItem(row, 4, item4);
            }
            if(csvfileDefinition.getUseAble()){
                //字段分隔符
                QString split=csvfileDefinition.getSplit();
                if(!split.isEmpty()){
                    if(split==" "){
                        split=split+"(空格)";
                    }
                    if(split=="\t"){
                        split=split+"(制表符)";
                    }
                    if(split==","){
                        split=split+"(英文逗号)";
                    }
                    if(split=="，"){
                        split=split+"(中文逗号)";
                    }
                    if(split=="|"){
                        split=split+"(竖线)";
                    }
                    QTableWidgetItem *item5= new QTableWidgetItem(split);
                    ptr_table->setItem(row, 5, item5);
                }
                //分隔符结尾
                QString s="否";
                if(csvfileDefinition.getEndwithflag()=="1"){
                    s="行尾为分隔符"+csvfileDefinition.getSplit();
                }
                QTableWidgetItem *item6= new QTableWidgetItem(s);
                ptr_table->setItem(row, 6, item6);
                //标题所在行
                if(csvfileDefinition.getTitlerowindex()<1){
                    QTableWidgetItem *item7= new QTableWidgetItem("文件内不包含标题行");
                    ptr_table->setItem(row, 7, item7);
                }
                else{
                    QTableWidgetItem *item7= new QTableWidgetItem(QString::number(csvfileDefinition.getTitlerowindex()));
                    ptr_table->setItem(row, 7, item7);
                }
                //数据起始行
                QTableWidgetItem *item8= new QTableWidgetItem(QString::number(csvfileDefinition.getDatabeginrowindex()));
                ptr_table->setItem(row, 8, item8);
                //文件编码
                if(!csvfileDefinition.getEcoding().isEmpty()){
                    QTableWidgetItem *item9= new QTableWidgetItem(csvfileDefinition.getEcoding());
                    ptr_table->setItem(row, 9, item9);
                }
                //每行字段数
                QTableWidgetItem *item10= new QTableWidgetItem(QString::number(csvfileDefinition.getFieldCount()));
                ptr_table->setItem(row, 10, item10);
            }
            //解析失败原因
            if(!faultList.at(row).getCause().isEmpty()){
                QTableWidgetItem *item11= new QTableWidgetItem(faultList.at(row).getCause());
                ptr_table->setItem(row, 11, item11);
            }
        }
        ptr_table->resizeRowsToContents();
        statusBar_display_rowsCount(rowCount);
        currentOpenFileType=openFileType::NotFileOrErr;
    }
}

/**
 * @brief MainWindow::display_FIXEDFaultCause当加载定长文件失败时，显示失败原因的方法
 * @param faultList
 */
void MainWindow::display_FIXEDFaultCause(QList<FaultCause> faultList){
    if(faultList.count()>0){
        int colCount=10;
        int rowCount=faultList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        title.append("FIXED配置文件");
        title.append("文件匹配解析器");
        title.append("文件用途解释");
        title.append("配置文件是否可用");
        title.append("配置文件不可用原因");
        title.append("数据内容起始行");
        title.append("文件编码方式");
        title.append("字段长度类型");
        title.append("每行可允许长度");
        title.append("解析匹配失败原因");
        ptr_table->setHorizontalHeaderLabels(title);
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置换行
        ptr_table->setWordWrap(true);
        ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
        ptr_table->setColumnWidth(9,300);
        ptr_table->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            FIXEDFileDefinition fixedfileDefinition=fixedConfigList.at(faultList.at(row).getConfigIndex2().configFileIndex).getConfigSegmentList().at(faultList.at(row).getConfigIndex2().configSegmentIndex);
            //配置文件
            if(!fixedfileDefinition.getFileIni().isEmpty()){
                QTableWidgetItem *item0= new QTableWidgetItem(fixedfileDefinition.getFileIni());
                ptr_table->setItem(row, 0, item0);
            }
            //解析器
            if(!fixedfileDefinition.getConfigSegment().isEmpty()){
                QTableWidgetItem *item1= new QTableWidgetItem(fixedfileDefinition.getConfigSegment());
                ptr_table->setItem(row, 1, item1);
            }
            //文件解释
            if(!fixedfileDefinition.getFileDescribe().isEmpty()){
                QTableWidgetItem *item2= new QTableWidgetItem(fixedfileDefinition.getFileDescribe());
                ptr_table->setItem(row, 2, item2);
            }
            //配置是否可用
            QString u="不可用";
            if(fixedfileDefinition.getUseAble()){
                u="可用";
            }
            QTableWidgetItem *item3= new QTableWidgetItem(u);
            ptr_table->setItem(row, 3, item3);
            //不可用原因
            if(!fixedfileDefinition.getMessage().isEmpty()){
                QTableWidgetItem *item4= new QTableWidgetItem(fixedfileDefinition.getMessage());
                ptr_table->setItem(row, 4, item4);
            }
            if(fixedfileDefinition.getUseAble()){
                //数据起始行
                QTableWidgetItem *item8= new QTableWidgetItem(QString::number(fixedfileDefinition.getDataRowBeginIndex()));
                ptr_table->setItem(row, 5, item8);
                //文件编码
                if(!fixedfileDefinition.getEcoding().isEmpty()){
                    QTableWidgetItem *item9= new QTableWidgetItem(fixedfileDefinition.getEcoding());
                    ptr_table->setItem(row, 6, item9);
                }
                //字段长度类型
                if(fixedfileDefinition.getFieldlengthtype()==0){
                    QTableWidgetItem *item10= new QTableWidgetItem("字节定长");
                    ptr_table->setItem(row, 7, item10);
                }
                else{
                    QTableWidgetItem *item10= new QTableWidgetItem("字符定长");
                    ptr_table->setItem(row, 7, item10);
                }
                //每行允许的长度
                QString lengths="";
                QList <int> length=fixedfileDefinition.getRowLengthHash().keys();
                lengths.append(QString::number(length.at(0)));
                if(length.count()>1){
                    for(int ff=1;ff<length.count();ff++){
                        lengths.append("/").append(QString::number(length.at(ff)));
                    }
                }
                QTableWidgetItem *item10= new QTableWidgetItem(lengths);
                ptr_table->setItem(row, 8, item10);
            }
            //解析失败原因
            if(!faultList.at(row).getCause().isEmpty()){
                QTableWidgetItem *item11= new QTableWidgetItem(faultList.at(row).getCause());
                ptr_table->setItem(row, 9, item11);
            }
        }
        ptr_table->resizeRowsToContents();
        statusBar_display_rowsCount(rowCount);
        currentOpenFileType=openFileType::NotFileOrErr;
    }
}

/**
 * @brief MainWindow::display_OFDFaultCause OFD文件解析失败原因
 * @param useini
 * @param faultList
 */
void MainWindow::display_OFDFaultCause(QString useini,QList<OFDFaultCause> faultList){
    if(faultList.count()>0){
        int colCount=4;
        int rowCount=faultList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        title.append("使用的配置文件");
        title.append("文件匹配解析器");
        title.append("文件用途解释");
        title.append("解析失败原因");
        ptr_table->setHorizontalHeaderLabels(title);
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);

        //设置换行
        ptr_table->setWordWrap(true);
        ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ptr_table->setColumnWidth(3,300);
        ptr_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            //配置文件
            QTableWidgetItem *item0= new QTableWidgetItem(useini);
            ptr_table->setItem(row, 0, item0);
            //解析器
            QTableWidgetItem *item1= new QTableWidgetItem(faultList.at(row).getConfig());
            ptr_table->setItem(row, 1, item1);
            //文件用途
            QTableWidgetItem *item2= new QTableWidgetItem(faultList.at(row).getName());
            ptr_table->setItem(row, 2, item2);
            //失败原因
            QTableWidgetItem *item3= new QTableWidgetItem(faultList.at(row).getCause());
            ptr_table->setItem(row, 3, item3);
        }
        statusBar_display_rowsCount(rowCount);
        ptr_table->resizeRowsToContents();
        currentOpenFileType=openFileType::NotFileOrErr;
    }
}

/**
 * @brief MainWindow::load_csvFileData当时识别出来csv文件类别后，加载csv文件数据的方法
 * @param fieldTitle
 */
void MainWindow::load_csvFileData(QStringList fieldTitle){
    //使用的ini配置
    ui->lineEditUseIni->setText(csv.getFileIni());
    ui->lineEditUseIni->setToolTip(csv.getFileIni());
    //文件类型描述
    ui->lineEditFileDescribe->setText(csv.getFileDescribe());
    ui->lineEditFileDescribe->setToolTip(csv.getFileDescribe());
    //文件类型描述
    ui->lineEditFileType->setText("CSV文件");
    //当加载的文件类别是csv时，传递日期栏目改为文件正则匹配到的配置;
    ui->labelFileTransferDate->setText("解析配置:");
    ui->lineEditFileTransferDate->setText(csv.getConfigSegment());
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly))
    {
        QTextCodec *codec=QTextCodec::codecForName(csv.getEcoding().toLocal8Bit());
        QString line2;
        int lineNumber=0;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        while (!dataFile.atEnd())
        {
            line2 = codec->toUnicode(dataFile.readLine()).remove('\r').remove('\n');
            if(lineNumber<csv.getDatabeginrowindex()-1){
                commonHeaderQStringList.append(line2);
            }
            else{
                if(dataCompressLevel==0){
                    commonContentQByteArrayList.append(codec->fromUnicode(line2));
                }
                else{
                    commonContentQByteArrayList.append(qCompress(codec->fromUnicode(line2),dataCompressLevel));
                }
            }
            lineNumber++;
            //为了防止UI卡死,进行循环读取文件时,考虑支持下窗口事件接收
            //每读取1000行更新下窗口事件
            if((lineNumber%1000)==0){
                statusBar_disPlayMessage(QString("文件读取中,已读取数据记录%1行").arg(QString::number(lineNumber)));
                qApp->processEvents();
                //强制终止事件-立即return退出
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        QApplication::restoreOverrideCursor();
        dataFile.close();
        //从文件读取到内存后，修正文件内容
        //文件尾部提取//基于配置
        //1.9.20版本新增保护功能，如果配置了忽略xx行文件尾,但是探测最后一行数据时如果列数=配置的列,则认为最后一行是数据,则不再去除,以免文件缺失文件尾的时候解析少了一行数据
        //1.9.28版本新增保护，即使最后一行列数=配置的列数，但是配置了最后一行静态内容识别并且满足，则去除最后一行
        if(csv.getEndIgnoreRow()>0&&commonContentQByteArrayList.count()>0){
            QString lastlineUnicode=line2;
            //字段拆分
            QRegExp rx("\\"+csv.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
            QStringList fieldList=lastlineUnicode.split(rx);
            //大于1列
            if(fieldList.count()==csv.getFieldCount()&&csv.getFieldCount()>1){
                //疑似缺失文件尾部,不再进行尾部处理
            }
            else if(fieldList.count()==csv.getFieldCount()&&csv.getFieldCount()==1&&commonContentQByteArrayList.count()>0&&!csv.getLastrowcheck().isEmpty()&&lastlineUnicode!=csv.getLastrowcheck()){
                //疑似缺失文件尾部,不再进行尾部处理
            }
            else if(commonContentQByteArrayList.count()>=csv.getEndIgnoreRow()){
                for(int ic=1;ic<=csv.getEndIgnoreRow();ic++){
                    if(dataCompressLevel==0){
                        commonFooterQStringList.insert(0,codec->toUnicode(commonContentQByteArrayList.last()));
                    }
                    else{
                        commonFooterQStringList.insert(0,codec->toUnicode(qUncompress(commonContentQByteArrayList.last())));
                    }
                    commonContentQByteArrayList.removeLast();
                }
            }
        }
        //文件尾部提取//基于自动分析
        //自动分析csv时，配置文件结尾忽略为-1，自动检查
        else if(csv.getEndIgnoreRow()==-1){
            int ignore=0;
            for(int i=commonContentQByteArrayList.count()-1;i>=0;i--){
                QString lastline;
                if(dataCompressLevel==0){
                    lastline=codec->fromUnicode(commonContentQByteArrayList.last());
                }
                else{
                    lastline=codec->fromUnicode(qUncompress(commonContentQByteArrayList.last()));
                }
                //字段拆分
                QRegExp rx("\\"+csv.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                QStringList fieldList=lastline.split(rx);
                //列数小于自动识别出来的列
                if(fieldList.count()<csv.getFieldCount()){
                    commonFooterQStringList.insert(0,lastline);
                    commonContentQByteArrayList.removeLast();
                    ignore++;
                }
                //列数等于配置，终止
                else{
                    break;
                }
                if((i%1000)==0){
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
            //回填csv配置
            csv.setEndIgnoreRow(ignore);
        }
    }
    //初始化表格
    init_CSVTable(fieldTitle);
    //单列文件
    if(csv.getFieldCount()==1){
        ui->lineEditFileType->setText("单列文件");
    }
    //文件后置校验
    QString warn="";
    if(!csv.getLastrowcheck().isEmpty()&&commonFooterQStringList.count()>0){
        if(commonFooterQStringList.last()!=csv.getLastrowcheck()){
            warn=warn+QString("\r\n\r\n当前打开的文件最后一行内容是\"%1\"不是\"%2\",根据配置的解析规则,正常来说此文件应该以\"%2\"为结束行!!!\r\n\r\n").arg(commonFooterQStringList.last(),csv.getLastrowcheck());
        }
    }
    //需要添加什么新的后置校验提示，可以添加到这里
    if(!warn.isEmpty()){
        QMessageBox::warning(this,tr("提示"),warn,QMessageBox::Ok,QMessageBox::Ok);
    }
}

/**
 * @brief MainWindow::load_dbfFile dbf文件加载
 */
void MainWindow::load_dbfFile(){
    //默认只读
    if (!dbftablefile.open(currentOpenFilePath,QDbf::QDbfTable::ReadOnly)) {
        currentOpenFileType=openFileType::NotFileOrErr;
        statusBar_disPlayMessage("无法打开此DBF文件,可能文件被占用/文件损坏/不支持的格式,请检查...");
        return;
    }
    //设置默认GB18030编码
    if(dbftablefile.codepage()==QDbf::QDbfTable::CodepageNotSet){
        qDebug()<<"未设置编码方式-设置默认GB18030解析";
        dbftablefile.setCodepage(QDbf::QDbfTable::GB18030);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::GB18030);
    }
    if(dbftablefile.codepage()==QDbf::QDbfTable::UnsupportedCodepage){
        qDebug()<<"不支持的编码方式-设置默认GB18030解析";
        dbftablefile.setCodepage(QDbf::QDbfTable::GB18030);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::GB18030);
    }
    //将编码信息存入dbf变量//
    switch (dbftablefile.codepage()){
    case QDbf::QDbfTable::IBM437:
        dbf.setEcoding("IBM437");
        break;
    case QDbf::QDbfTable::IBM850:
        dbf.setEcoding("IBM850");
        break;
    case QDbf::QDbfTable::IBM866:
        dbf.setEcoding("IBM866");
        break;
    case QDbf::QDbfTable::Windows1250:
        dbf.setEcoding("Windows1250");
        break;
    case QDbf::QDbfTable::Windows1251:
        dbf.setEcoding("Windows1251");
        break;
    case QDbf::QDbfTable::Windows1252:
        dbf.setEcoding("Windows1252");
        break;
    case QDbf::QDbfTable::GB18030:
        dbf.setEcoding("GB18030");
        break;
    case QDbf::QDbfTable::Big5:
        dbf.setEcoding("Big5");
        break;
    case QDbf::QDbfTable::Shift_JIS:
        dbf.setEcoding("Shift-JIS");
        break;
    case QDbf::QDbfTable::CodepageNotSet:
        dbf.setEcoding("GB18030");
        break;
    case QDbf::QDbfTable::UnsupportedCodepage:
        dbf.setEcoding("GB18030");
        break;
    }
    //当前打开的文件类别为4，dbf文件
    currentOpenFileType=openFileType::DBFFile;
    //dbf工具箱-只有打开dbf文件时才显示
    ui->menudbftools->menuAction()->setVisible(true);
    ui->actionTableStructure->setVisible(false);
    if(dbfDisplayType==0){
        ui->actionalldata->setEnabled(false);
        ui->actionnotdelete->setEnabled(true);
        ui->actionjustdelete->setEnabled(true);
    }
    else  if(dbfDisplayType==1){
        ui->actionalldata->setEnabled(true);
        ui->actionnotdelete->setEnabled(false);
        ui->actionjustdelete->setEnabled(true);
    }
    else  if(dbfDisplayType==2) {
        ui->actionalldata->setEnabled(true);
        ui->actionnotdelete->setEnabled(true);
        ui->actionjustdelete->setEnabled(false);
    }
    //trim////////////////////////////////////////////
    if(dbfTrimType==0){
        ui->actionleftright->setEnabled(false);
        ui->actionleft->setEnabled(true);
        ui->actionright->setEnabled(true);
    }
    else  if(dbfTrimType==1){
        ui->actionleftright->setEnabled(true);
        ui->actionleft->setEnabled(false);
        ui->actionright->setEnabled(true);
    }
    else  if(dbfTrimType==2) {
        ui->actionleftright->setEnabled(true);
        ui->actionleft->setEnabled(true);
        ui->actionright->setEnabled(false);
    }
    //block-----
    dataBlocked=true;
    dataBlockedMessage="正在处理DBF数据记录,请稍候...";
    isUpdateData=true;
    //读取一次record-用于获取列的信息
    QDbf::QDbfRecord record = dbftablefile.record();
    //取dbf字段类型、字段长度、精度、字段名等等等
    QList<DbfFieldDefinition> fieldList;
    for (int i=0;i<record.count();i++){
        DbfFieldDefinition field;
        //字段名称全部以大写展示
        field.setFieldName(record.fieldName(i).toUpper());
        field.setFieldOriginName(record.fieldName(i));
        field.setFieldType(record.field(i).type());
        //debug
        qDebug()<<record.fieldName(i)<<QString::number(record.field(i).type());
        field.setLength(record.field(i).length());
        field.setDecLength(record.field(i).precision());
        fieldList.append(field);
    }
    //初始化构建dbf变量//////////////////////
    dbf.setFieldList(fieldList);
    dbf.setFieldCount(record.count());
    dbf.setFileIni(nullptr);
    dbf.setFileDescribe(nullptr);
    dbf.setFileType(nullptr);
    //开始适配配置文件////////////////////////////////////////////////////////////////
    QList<dbfMatchInfo> matchListInfo;
    //遍历DBF配置
    if(dbfConfigList.count()>0){
        for (int ll=0;ll< dbfConfigList.count();ll++) {
            DbfFileConfig dbfFileConfig=dbfConfigList.at(ll);
            if(dbfFileConfig.getSupportFileList().count()>0){
                for(int kk=0;kk<dbfFileConfig.getSupportFileList().count();kk++){
                    bool matchOk=false;
                    QString filename=dbfFileConfig.getSupportFileList().at(kk).fileName;
                    QString pattern(filename.toUpper());
                    QRegExp rx(pattern);
                    rx.setPatternSyntax(QRegExp::Wildcard);
                    matchOk = rx.exactMatch(QFileInfo(currentOpenFilePath).fileName().toUpper());
                    //qDebug()<<"配置文件:"<<filename.toUpper();
                    //qDebug()<<"读取的文件:"<<QFileInfo(currentOpenFilePath).fileName().toUpper();
                    //文件命匹配
                    if(matchOk){
                        //记录匹配到的文件名
                        qDebug()<<"匹配文件名:"<<filename.toUpper();
                        qDebug()<<"配置文件:"<<dbfFileConfig.fileIniName;
                        //测算匹配度
                        float matchFieldCount=0;
                        for(int ff=0;ff<fieldList.count();ff++){
                            if(dbfFileConfig.getFieldDescribe().contains(fieldList.at(ff).getFieldName().toUpper())){
                                matchFieldCount+=1;
                            }
                        }
                        //存入匹配信息,字段匹配度大于30%的才存入
                        if(matchFieldCount/fieldList.count()>0.3){
                            dbfMatchInfo a;
                            a.configFileIndex=ll;
                            a.ini=dbfFileConfig.fileIniName;
                            a.matchFileType=dbfFileConfig.getSupportFileList().at(kk).fileName;
                            a.matchfileDescribe=dbfFileConfig.getSupportFileList().at(kk).fileDescribe;
                            a.matchDegree=matchFieldCount/fieldList.count();
                            matchListInfo.append(a);
                        }
                    }
                }
            }
        }
    }

    bool configisok=false;
    int useconfigFileIndex=0;
    if(matchListInfo.count()==1){
        useconfigFileIndex=matchListInfo.at(0).configFileIndex;
        dbf.setFileIni(matchListInfo.at(0).ini);
        dbf.setFileDescribe(matchListInfo.at(0).matchfileDescribe);
        dbf.setFileType(matchListInfo.at(0).matchFileType);
        configisok=true;
    }
    else if (matchListInfo.count()>1){
        qDebug()<<"匹配到多个配置";
        //排序
        std::sort(matchListInfo.begin(), matchListInfo.end(),compareDBFData);
        QList<QStringList> match;
        for(int i=0;i<matchListInfo.count();i++){
            QStringList item;
            item.append(matchListInfo.at(i).ini);
            item.append(matchListInfo.at(i).matchFileType);
            item.append(matchListInfo.at(i).matchfileDescribe);
            item.append(QString::number(matchListInfo.at(i).matchDegree*100,'f',2)+"%");
            match.append(item);
        }
        DialogChooseDBFConfig dialog2(&match,this);
        dialog2.setWindowTitle("打开的文件匹配到了多个解析配置,请选择使用哪一个配置解析文件");
        dialog2.setModal(true);
        dialog2.exec();
        //从弹窗中获取结果
        int index=dialog2.getChooeseIndex();
        //没有选择，默认使用第一个
        if(!dialog2.getConfirm()){
            useconfigFileIndex=matchListInfo.at(0).configFileIndex;
            dbf.setFileIni(matchListInfo.at(0).ini);
            dbf.setFileDescribe(matchListInfo.at(0).matchfileDescribe);
            dbf.setFileType(matchListInfo.at(0).matchFileType);
            configisok=true;
        }
        else{
            useconfigFileIndex=matchListInfo.at(index).configFileIndex;
            dbf.setFileIni(matchListInfo.at(index).ini);
            dbf.setFileDescribe(matchListInfo.at(index).matchfileDescribe);
            dbf.setFileType(matchListInfo.at(index).matchFileType);
            configisok=true;
        }
    }
    if(configisok){
        QList<DbfFieldDefinition> fieldList2;
        for (int i=0;i<dbf.getFieldCount();i++){
            DbfFieldDefinition field=dbf.getFieldList().at(i);
            field.setFieldDescribe(dbfConfigList.at(useconfigFileIndex).getFieldDescribe().value(field.getFieldName().toUpper()));
            fieldList2.append(field);
        }
        dbf.setFieldList(fieldList2);
    }
    if(!dbf.getFileType().isEmpty()){
        ui->lineEditFileType->setText(dbf.getFileType());
    }
    else{
        ui->lineEditFileType->setText("DBF文件");
    }
    ui->lineEditUseIni->setText(dbf.getFileIni());
    ui->lineEditUseIni->setToolTip(dbf.getFileIni());
    ui->lineEditFileDescribe->setText(dbf.getFileDescribe());
    ui->lineEditFileDescribe->setToolTip(dbf.getFileDescribe());
    //////////////////////////////////////
    //准备循环取数据，识别数据行的删除标志等
    int rowCount=dbftablefile.size();
    //数据加载///////////////数据识别////基于要显示的类别来决定加载哪些行数据
    int rowindex=0;
    dbfRowMap.clear();
    if(rowCount>0){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        for(int row=0;row<rowCount;row++){
            dbftablefile.seek(row);
            //全浏览,每一行都要插入map
            if(dbfDisplayType==0){
                dbfRowMap.insert(rowindex,row);
                rowindex++;
            }
            //仅浏览没删除的数据
            else if (dbfDisplayType==1&&(!dbftablefile.record().isDeleted())){
                dbfRowMap.insert(rowindex,row);
                rowindex++;
            }
            //仅浏览删除的数据
            else if (dbfDisplayType==2&&(dbftablefile.record().isDeleted())){
                dbfRowMap.insert(rowindex,row);
                rowindex++;
            }
            if((row%1000)==0){
                statusBar_disPlayMessage(QString("DBF记录处理中,已处理数据记录%1行").arg(QString::number(row+1)));
                qApp->processEvents();
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        QApplication::restoreOverrideCursor();
    }
    dataBlocked=false;
    isUpdateData=false;
    //开始初始化表格
    init_DBFTable();
}

/**
 * @brief MainWindow::init_display_IndexTable OFD索引数据的表格显示
 *
 */
void MainWindow::init_display_OFDIndexTable(){
    if(!ofdindexFileDataList.empty()){
        int colCount=1;
        int rowCount=ofdindexFileDataList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        title.append("索引包含的文件信息");
        ptr_table->setHorizontalHeaderLabels(title);
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            QTableWidgetItem *item= new QTableWidgetItem(ofdindexFileDataList.at(row));
            ptr_table->setItem(row, 0, item);
        }
        ptr_table->resizeColumnsToContents();
        statusBar_display_rowsCount(rowCount);
        if(!likeOFDFileType.isEmpty()){
            statusBar_disPlayMessage(likeOFDFileType);
        }
    }
    else
    {
        statusBar_disPlayMessage(tr("没有数据可供显示~"));
    }
}

/**
 * @brief MainWindow::init_OFDTable加载OFD数据完毕后，初始化OFD文件所需要的表格的函数，目前在分页支持下，默认只初始化第一页数据
 */
void MainWindow::init_OFDTable(){
    if(ofd.getUseAble()){
        int colCount=ofd.getFieldCount();
        int rowCount=commonContentQByteArrayList.count();
        //设置表格列数
        ptr_table->setColumnCount(colCount);
        //////////////分页逻辑//////////////////////
        //根据分页情况来决定是否显示分页按钮和设置表格行数
        //不分页
        if(rowCount<=pageRowSize){
            ui->framePage->setVisible(false);
            ptr_table->setRowCount(rowCount);
        }
        //分页-加载第一页
        else{
            ui->framePage->setVisible(true);
            ui->pageText->setText("1");
            //表格行数为第一页的行数
            ptr_table->setRowCount(pageRowSize);
            pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
            //显示到第一页的时候,禁用首页和上一页按钮
            ui->pushButtonPageFirst->setEnabled(false);
            ui->pushButtonPagePrevious->setEnabled(false);
        }
        //////////////分页逻辑//////////////////////
        //设置表格列标题
        QStringList title;
        for(int i=0;i<colCount;i++){
            //仅获取列的中文备注当作列标题
            QString t=ofd.getFieldList().at(i).getFieldDescribe();
            //初始化列宽度设置为0，保障加载数据时，至少进行一次列宽度重设，解决一些坑坑的问题，请注意不要修改此处代码
            columnWidth.insert(i,0);
            title.append(t);
        }
        //设置标题
        ptr_table->setHorizontalHeaderLabels(title);
        //设置Tips
        if(commonFieldTips.contains("OFD"+ofd.getDictionary())){
            //设置tips
            for(int tipindex=0;tipindex<colCount;tipindex++){
                QString tips=commonFieldTips.value("OFD"+ofd.getDictionary()).value(ofd.getFieldList().at(tipindex).getFieldName());
                if(!tips.isEmpty()){
                    ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
                }
            }
        }
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        //按行读取ofdFileContentQByteArrayList,边读取边解析
        if(rowCount>0){
            //获取当前table的高度
            int higth=ptr_table->size().height();
            tableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
            tableRowEnd=tableRowBegin+(higth/rowHight);
            display_OFDTable();
            ptr_table->resizeColumnsToContents();
        }
        else{
            //如果没有数据,也执行下自动设置列宽,增加空数据的显示美感
            ptr_table->resizeColumnsToContents();
        }
        statusBar_display_rowsCount(rowCount);
        if(!likeOFDFileType.isEmpty()){
            statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒-%3-%4个必填较验规则").arg(QString::number(rowCount),QString::number(static_cast<double>(time_Start.msecsTo(QDateTime::currentDateTime()))/1000.00,'f',2),likeOFDFileType,QString::number(ofd.getFieldcheckList().count())));
        }
        else {
            statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒-%3个必填较验规则").arg(QString::number(rowCount),QString::number(static_cast<double>(time_Start.msecsTo(QDateTime::currentDateTime()))/1000.00,'f',2),QString::number(ofd.getFieldcheckList().count())));
        }
    }
    else
    {
        statusBar_disPlayMessage(ofd.getMessage());
    }
}


/**
 * @brief MainWindow::init_DBFTable加载dbf数据完毕后，初始化dbf文件所需要的表格的函数，目前在分页支持下，默认只初始化第一页数据
 */
void MainWindow::init_DBFTable(){
    int colCount=dbf.getFieldCount();
    //注意取map--我们不一定要展示所有数据,按用户选择类型来
    int rowCount=dbfRowMap.count();
    //设置表格列数
    ptr_table->setColumnCount(colCount);
    //////////////分页逻辑//////////////////////
    //根据分页情况来决定是否显示分页按钮和设置表格行数
    //不分页
    if(rowCount<=pageRowSize){
        ui->framePage->setVisible(false);
        ptr_table->setRowCount(rowCount);
    }
    //分页-加载第一页
    else{
        ui->framePage->setVisible(true);
        ui->pageText->setText("1");
        //表格行数为第一页的行数
        ptr_table->setRowCount(pageRowSize);
        pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
        //显示到第一页的时候,禁用首页和上一页按钮
        ui->pushButtonPageFirst->setEnabled(false);
        ui->pushButtonPagePrevious->setEnabled(false);
    }
    //////////////分页逻辑//////////////////////
    //设置表格列标题
    QStringList title;
    for(int i=0;i<colCount;i++){
        QString t=dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe();
        //初始化列宽度设置为0，保障加载数据时，至少进行一次列宽度重设，解决一些坑坑的问题，请注意不要修改此处代码
        columnWidth.insert(i,0);
        title.append(t);
    }
    //设置标题
    ptr_table->setHorizontalHeaderLabels(title);
    if(commonFieldTips.contains("DBF"+dbf.getFileIni())){
        //设置tips
        for(int tipindex=0;tipindex<colCount;tipindex++){
            QString tips=commonFieldTips.value("DBF"+dbf.getFileIni()).value(dbf.getFieldList().at(tipindex).getFieldName().toUpper());
            if(!tips.isEmpty()){
                ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
            }
        }
    }
    //设置表格的选择方式
    ptr_table->setSelectionBehavior(tableSelectionBehavior);
    //设置编辑方式
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
    //设置表格的内容
    if(rowCount>0){
        int higth=ptr_table->size().height();
        tableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
        tableRowEnd=tableRowBegin+(higth/rowHight);
        display_DBFTable();
        ptr_table->resizeColumnsToContents();
    }
    else{
        ptr_table->resizeColumnsToContents();
    }
    statusBar_display_rowsCount(rowCount);
    QString add="";
    if(dbfDisplayType==0){
        add="显示所有记录";
    }
    else if(dbfDisplayType==1){
        add="仅显示未删除的记录";
    }
    else if(dbfDisplayType==2){
        add="仅显示已删除的记录";
    }
    statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-%2-耗时%3秒").arg(QString::number(rowCount),add,QString::number(static_cast<double>(time_Start.msecsTo(QDateTime::currentDateTime()))/1000.00,'f',2)));
}

/**
 * @brief MainWindow::init_FIXEDTable初始化定长文件函数，目前在分页支持下，默认只初始化第一页数据
 */
void MainWindow::init_FIXEDTable(){
    int colCount=fixed.getFieldCountMax();
    int rowCount=commonContentQByteArrayList.count();
    //////////////分页逻辑//////////////////////
    //根据分页情况来决定是否显示分页按钮和设置表格行数
    //不分页
    if(rowCount<=pageRowSize){
        ui->framePage->setVisible(false);
        ptr_table->setRowCount(rowCount);
    }
    //分页-加载第一页
    else{
        ui->framePage->setVisible(true);
        ui->pageText->setText("1");
        //表格行数为第一页的行数
        ptr_table->setRowCount(pageRowSize);
        pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
        //显示到第一页的时候,禁用首页和上一页按钮
        ui->pushButtonPageFirst->setEnabled(false);
        ui->pushButtonPagePrevious->setEnabled(false);
    }
    //////////////分页逻辑//////////////////////
    ptr_table->setColumnCount(colCount);
    //设置表格列标题
    QStringList title;
    for(int i=0;i<colCount;i++){
        //仅获取列的中文备注当作列标题
        QString t=fixed.getFieldList().at(i).getFieldDescribe();
        //初始化列宽度设置为0，保障加载数据时，至少进行一次列宽度重设，解决一些坑坑的问题，请注意不要修改此处代码
        columnWidth.insert(i,0);
        title.append(t);
    }
    //设置标题
    ptr_table->setHorizontalHeaderLabels(title);
    if(commonFieldTips.contains("FIXED"+fixed.getFileIni())){
        //设置tips
        for(int tipindex=0;tipindex<colCount;tipindex++){
            QString tips=commonFieldTips.value("FIXED"+fixed.getFileIni()).value(fixed.getFieldList().at(tipindex).getFieldDescribe());
            if(!tips.isEmpty()){
                ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
            }
        }
    }
    //设置表格的选择方式
    ptr_table->setSelectionBehavior(tableSelectionBehavior);
    //设置编辑方式
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
    //设置表格的内容
    if(rowCount>0){
        //获取当前table的高度
        int higth=ptr_table->size().height();
        tableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
        tableRowEnd=tableRowBegin+(higth/rowHight);
        display_FIXEDTable();
        ptr_table->resizeColumnsToContents();
    }
    else{
        //如果没有数据,也执行下自动设置列宽,增加空数据的显示美感
        ptr_table->resizeColumnsToContents();
    }
    statusBar_display_rowsCount(rowCount);
    statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒-%3个必填较验规则").arg(QString::number(rowCount),QString::number(static_cast<double>(time_Start.msecsTo(QDateTime::currentDateTime()))/1000.00,'f',2),QString::number(fixed.getFieldcheckList().count())));
}

/**
 * @brief MainWindow::init_CSVTable初始化csv文件函数，目前在分页支持下，默认只初始化第一页数据
 * @param title
 */
void MainWindow::init_CSVTable(QStringList title){
    if(csv.getUseAble()){
        ///////////////////////CSV数值类型判断///////////////////////////////////////////
        //判断哪些列是否是数值,判断20行数据或者低于20行判读所有行，全部是数值的才认为是数值、但是允许忽略空值
        //仅仅判断包含小数点的小数，整数暂时不管，大部分整数列都是字典值，不是数值，没必要
        //循环识别每列的数据类型
        QSet<int> lastColIsEmptyOrIsInteger;
        for(int rowIndex=0;rowIndex<20&&rowIndex<commonContentQByteArrayList.count();rowIndex++){
            //获取本行数据
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowIndex);
            //首行识别
            if(rowIndex==0){
                for(int colIndex=0;colIndex<title.count();colIndex++){
                    FieldIsNumber isnumber;
                    bool convertOk=false;
                    QString vav="";
                    //注意不能去掉判断，csv文件如果列残缺会取不到所有列
                    if(colIndex<rowdata.count()){
                        vav=rowdata.at(colIndex);
                    }
                    vav.toDouble(&convertOk);
                    //最起码是数值才进入该流程
                    if(convertOk&&!vav.contains(" ")){
                        //识别识别带小数点的小数
                        if(vav.contains(".",Qt::CaseSensitive)){
                            isnumber.setIsNumber(true);
                            isnumber.setDecimalLength(vav.split(".")[1].length());
                        }
                        //识别只有整数字符串暂不考虑自动设置为数据格式,但是不排斥用户自己设置,1.9.3版本提供用户自己设置的功能
                        //整数字符串有可能是流水号,直接认定为数值有点太残暴了
                        else{
                            isnumber.setDecimalLength(0);
                            isnumber.setIsNumber(false);
                            lastColIsEmptyOrIsInteger.insert(colIndex);
                        }
                        CsvFieldIsNumberOrNot.insert(colIndex,isnumber);
                    }
                    //转换不成功
                    else{
                        isnumber.setDecimalLength(0);
                        isnumber.setIsNumber(false);
                        CsvFieldIsNumberOrNot.insert(colIndex,isnumber);
                        //如果本行某列为空则下一次判断本列是否为数值时，依然参与判断
                        if(vav.isEmpty()){
                            lastColIsEmptyOrIsInteger.insert(colIndex);
                        }
                    }
                }
            }
            //非首行
            else{
                for(int colIndex=0;colIndex<title.count();colIndex++){
                    FieldIsNumber isnumber;
                    bool convertOk=false;
                    QString vav="";
                    //注意不能去掉判断，csv文件如果列残缺会取不到所有列
                    if(colIndex<rowdata.count()){
                        vav=rowdata.at(colIndex);
                    }
                    vav.toDouble(&convertOk);
                    /////////////////////////分类处理/////////////////////
                    if((CsvFieldIsNumberOrNot.contains(colIndex)&&CsvFieldIsNumberOrNot.value(colIndex).getIsNumber())||lastColIsEmptyOrIsInteger.contains(colIndex)){
                        //数值
                        if(convertOk&&!vav.contains(" ")){
                            //非空所以移除空列标志
                            if(lastColIsEmptyOrIsInteger.contains(colIndex)){
                                lastColIsEmptyOrIsInteger.remove(colIndex);
                            }
                            int len=0;
                            //小数
                            if(vav.contains(".",Qt::CaseSensitive)){
                                len=vav.split(".")[1].length();
                                if (CsvFieldIsNumberOrNot.contains(colIndex)&&CsvFieldIsNumberOrNot.value(colIndex).getIsNumber()){
                                    if(len>CsvFieldIsNumberOrNot.value(colIndex).getDecimalLength()){
                                        isnumber.setIsNumber(true);
                                        isnumber.setDecimalLength(len);
                                        CsvFieldIsNumberOrNot.insert(colIndex,isnumber);
                                    }
                                }
                                else{
                                    isnumber.setIsNumber(true);
                                    isnumber.setDecimalLength(len);
                                    CsvFieldIsNumberOrNot.insert(colIndex,isnumber);
                                }
                            }
                            //整数
                            else{
                                isnumber.setDecimalLength(0);
                                isnumber.setIsNumber(false);
                                lastColIsEmptyOrIsInteger.insert(colIndex);
                                continue;
                            }
                        }
                        //非数值
                        else{
                            if(vav.isEmpty()){
                                //空则继续空
                                if(lastColIsEmptyOrIsInteger.contains(colIndex)){
                                    continue;
                                }
                                //不空则新增空
                                else{
                                    lastColIsEmptyOrIsInteger.insert(colIndex);
                                    continue;
                                }
                            }
                            //只要发生一次本行非空,非数值,则剥夺一切
                            else{
                                //剥夺数值
                                if (CsvFieldIsNumberOrNot.contains(colIndex)&&CsvFieldIsNumberOrNot.value(colIndex).getIsNumber()){
                                    isnumber.setIsNumber(false);
                                    if(csv.getFieldList().count()>colIndex&&csv.getFieldList().at(colIndex).getIsNumber()!=1){
                                        isnumber.setDecimalLength(0);
                                    }
                                    CsvFieldIsNumberOrNot.insert(colIndex,isnumber);
                                }
                                //剥夺空判断
                                if(lastColIsEmptyOrIsInteger.contains(colIndex)){
                                    lastColIsEmptyOrIsInteger.remove(colIndex);
                                }
                            }
                        }
                    }
                    //上一行不是数值且不是空，直接往后走
                    else{
                        continue;
                    }
                }
            }
        }
        int colCount=title.count();
        int rowCount=commonContentQByteArrayList.count();
        ptr_table->setColumnCount(colCount);
        //////////////分页逻辑//////////////////////
        //根据分页情况来决定是否显示分页按钮和设置表格行数
        //不分页
        if(rowCount<=pageRowSize){
            ui->framePage->setVisible(false);
            ptr_table->setRowCount(rowCount);
        }
        //分页-加载第一页
        else{
            ui->framePage->setVisible(true);
            ui->pageText->setText("1");
            //表格行数为第一页的行数
            ptr_table->setRowCount(pageRowSize);
            pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
            //显示到第一页的时候,禁用首页和上一页按钮
            ui->pushButtonPageFirst->setEnabled(false);
            ui->pushButtonPagePrevious->setEnabled(false);
        }
        //////////////分页逻辑//////////////////////
        for(int a=0;a<title.count()&&a<csv.getFieldList().count();a++){
            int numberShiftint=csv.getFieldList().at(a).getDecimalPointShift();
            if(csv.getFieldList().at(a).getIsNumber()==1&&numberShiftint>0){
                //修正标题--增加缩放描述
                switch (numberShiftint){
                case 1:
                    title.replace(a,QString(title.at(a)+"[缩小%1倍]").arg(10));
                    break;
                case 2:
                    title.replace(a,QString(title.at(a)+"[缩小%1倍]").arg(100));
                    break;
                case 3:
                    title.replace(a,QString(title.at(a)+"[缩小%1倍]").arg(1000));
                    break;
                case 4:
                    title.replace(a,QString(title.at(a)+"[缩小%1倍]").arg(10000));
                    break;
                default:
                    break;
                }
            }
        }

        //设置标题
        ptr_table->setHorizontalHeaderLabels(title);
        if(commonFieldTips.contains("CSV"+csv.getFileIni())){
            //设置tips
            for(int tipindex=0;tipindex<colCount;tipindex++){
                QString tips=commonFieldTips.value("CSV"+csv.getFileIni()).value(csv.getFieldList().at(tipindex).getFieldDescribe());
                if(!tips.isEmpty()){
                    ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
                }
            }
        }
        for(int i=0;i<title.count();i++){
            //初始化列宽度设置为0，保障加载数据时，至少进行一次列宽度重设，解决一些坑坑的问题，请注意不要修改此处代码
            columnWidth.insert(i,0);
        }
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        if(rowCount>0){
            //获取当前table的高度
            int higth=ptr_table->size().height();
            tableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
            tableRowEnd=tableRowBegin+(higth/rowHight);
            ptr_table->resizeColumnsToContents();
            display_CSVTable();
        }
        else{
            //如果没有数据,也执行下自动设置列宽,增加空数据的显示美感
            ptr_table->resizeColumnsToContents();
        }
        statusBar_display_rowsCount(rowCount,csv.getEcoding()+(csv.getAutoecoding()?"[自动识别]":""));
        if(csv.getFfAuto()){
            statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-[使用自动解析引擎解析,建议配置解析规则获得更好体验]-耗时%2秒-%3个必填较验规则").arg(QString::number(rowCount),QString::number(static_cast<double>(time_Start.msecsTo(QDateTime::currentDateTime()))/1000.00,'f',2),QString::number(csv.getFieldcheckList().count())));
        }
        else{
            statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒-%3个必填较验规则").arg(QString::number(rowCount),QString::number(static_cast<double>(time_Start.msecsTo(QDateTime::currentDateTime()))/1000.00,'f',2),QString::number(csv.getFieldcheckList().count())));
        }
    }
    else{
        statusBar_disPlayMessage(csv.getMessage());
    }
}

/**
 * @brief MainWindow::display_OFDTable 显示OFD文件内容到表格的方法，注意懒加载机制，依赖于窗口变化，滚动条滚动进行增量加载
 *
 * 数据懒加载机制,当数据量很多时,仅仅加载界面显示的这些行,未显示的不加载,当拖动滚动条时,重复调用此方法进行重新补充显示
 *
 * 无bug万岁
 */

void MainWindow::display_OFDTable(bool clearDirtyData){
    QList<int> needRestwitdh;
    int rowCount=ptr_table->rowCount();
    int colCount=ptr_table->columnCount();
    //防止渲染边界超过表总行数
    if(tableRowBegin<0){
        tableRowBegin=0;
    }
    if(tableRowEnd>=rowCount){
        tableRowEnd=rowCount-1;
    }
    for (int rowInTable = tableRowBegin; rowInTable <= tableRowEnd; ++rowInTable)
    {
        //如果此行已经加载过了,则不再加载
        if(rowHasloaded.contains(rowInTable)){
            continue;
        }else{
            rowHasloaded.insert(rowInTable,true);
            //进行关系转换,计算本行在文件记录List中的行数
            int rowInFileContent=(currentPage-1)*pageRowSize+rowInTable;
            //在分页模式下重设表头行号，以免非第一页的表头仍然从1开始计算行号
            QTableWidgetItem *itemVerticalHeaderItem= new QTableWidgetItem();
            ptr_table->setVerticalHeaderItem(rowInTable,itemVerticalHeaderItem);
            //行号取文件记录行号
            itemVerticalHeaderItem->setText(QString::number(rowInFileContent+1));
            //获取本行数据
            QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowInFileContent);
            //进行必填较验检查
            QMap<int,QString> checkresult;
            if(checkFieldFlag){
                checkRowFieldResult(rowdata,checkresult);
            }
            for(int col=0;col<colCount;col++){
                QString values=rowdata.at(col);
                //仅对数据非空单元格赋值
                if(!values.isEmpty()){
                    bool isDouble=false;
                    //数值类字段千位分隔符展示
                    if(ofd.getFieldList().at(col).getFieldType()=="N"){
                        isDouble=true;
                        values=Utils::CovertDoubleQStringWithThousandSplit(values);
                    }
                    int colLength=values.length();
                    if(!columnWidth.contains(col)){
                        needRestwitdh.append(col);
                        columnWidth.insert(col,colLength);
                    }else
                        if(colLength>columnWidth.value(col)){
                            needRestwitdh.append(col);
                            columnWidth.insert(col,colLength);
                        }
                    QTableWidgetItem *item= new QTableWidgetItem();
                    if(isDouble){
                        item->setTextAlignment(Qt::AlignRight);
                    }
                    ptr_table->setItem(rowInTable, col, item);
                    item->setText(values);
                }
                //空值
                else{
                    if (checkresult.contains(col)){
                        QTableWidgetItem *item= new QTableWidgetItem();
                        ptr_table->setItem(rowInTable, col, item);
                        item->setToolTip(checkresult.value(col));
                        item->setBackground(QColor(0XED5B56));
                    }
                    //仅在修改数据时使用,防止修改数据为空值时残留脏数据显示
                    else if(clearDirtyData){
                        ptr_table->setItem(rowInTable, col, nullptr);
                    }
                }
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
        //判断某个字段的长度是不是非常的长,如果超过48字符,则标题栏居左,以免不太好找到标题信息
        if(columnWidth.contains(needRestwitdh.at(cc))&&columnWidth.value(needRestwitdh.at(cc))>48&&ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->textAlignment()!=Qt::AlignLeft){
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setTextAlignment(Qt::AlignLeft);
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setToolTip(ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip().isEmpty()?"超长字段,标题自动居左显示~":ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip()+"-超长字段,标题自动居左显示~");
        }
    }
}
/**
 * @brief MainWindow::display_FIXEDTable 显示定长文件内容到表格的方法，注意懒加载机制，依赖于窗口变化，滚动条滚动进行增量加载
 */
void MainWindow::display_FIXEDTable(bool clearDirtyData){
    QList<int> needRestwitdh;
    int rowCount=ptr_table->rowCount();
    int colCount=ptr_table->columnCount();
    //防止渲染边界超过表总行数
    if(tableRowBegin<0){
        tableRowBegin=0;
    }
    if(tableRowEnd>=rowCount){
        tableRowEnd=rowCount-1;
    }
    for (int rowInTable = tableRowBegin; rowInTable <= tableRowEnd; ++rowInTable)
    {
        //如果此行已经加载过了,则不再加载
        if(rowHasloaded.contains(rowInTable)){
            continue;
        }else{
            rowHasloaded.insert(rowInTable,true);
            //进行关系转换,计算本行在文件记录List中的行数
            int rowInFileContent=(currentPage-1)*pageRowSize+rowInTable;
            //在分页模式下重设表头行号，以免非第一页的表头仍然从1开始计算行号
            QTableWidgetItem *itemVerticalHeaderItem= new QTableWidgetItem();
            ptr_table->setVerticalHeaderItem(rowInTable,itemVerticalHeaderItem);
            //行号取文件记录行号
            itemVerticalHeaderItem->setText(QString::number(rowInFileContent+1));
            //获取本行数据
            QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowInFileContent);
            //进行必填较验检查
            QMap<int,QString> checkresult;
            if(checkFieldFlag){
                checkRowFieldResult(rowdata,checkresult);
            }
            for(int col=0;col<colCount&&col<rowdata.count();col++){
                QString values=rowdata.at(col);
                //仅对数据非空单元格赋值
                if(!values.isEmpty()){
                    bool isDouble=false;
                    //数值类字段千位分隔符展示
                    if(fixed.getFieldList().at(col).getFieldType()=="N"){
                        isDouble=true;
                        values=Utils::CovertDoubleQStringWithThousandSplit(values);
                    }
                    int colLength=values.length();
                    if(!columnWidth.contains(col)){
                        needRestwitdh.append(col);
                        columnWidth.insert(col,colLength);
                    }else
                        if(colLength>columnWidth.value(col)){
                            needRestwitdh.append(col);
                            columnWidth.insert(col,colLength);
                        }
                    QTableWidgetItem *item= new QTableWidgetItem();
                    if(isDouble){
                        item->setTextAlignment(Qt::AlignRight);
                    }
                    ptr_table->setItem(rowInTable, col, item);
                    item->setText(values);
                }
                else{
                    if (checkresult.contains(col)){
                        QTableWidgetItem *item= new QTableWidgetItem();
                        ptr_table->setItem(rowInTable, col, item);
                        item->setToolTip(checkresult.value(col));
                        item->setBackground(QColor(0XED5B56));
                    }
                    //仅在修改数据时使用,防止修改数据为空值时残留脏数据显示
                    else if(clearDirtyData){
                        ptr_table->setItem(rowInTable, col, nullptr);
                    }
                }
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
        //判断某个字段的长度是不是非常的长,如果超过48字符,则标题栏居左,以免不太好找到标题信息
        if(columnWidth.contains(needRestwitdh.at(cc))&&columnWidth.value(needRestwitdh.at(cc))>48&&ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->textAlignment()!=Qt::AlignLeft){
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setTextAlignment(Qt::AlignLeft);
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setToolTip(ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip().isEmpty()?"超长字段,标题自动居左显示~":ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip()+"-超长字段,标题自动居左显示~");
        }
    }
}

/**
 * @brief MainWindow::display_CSVTable 显示CSV文件内容到表格的方法，注意懒加载机制，依赖于窗口变化，滚动条滚动进行增量加载
 */
void MainWindow::display_CSVTable(){
    QList<int> needRestwitdh;
    int rowCount=ptr_table->rowCount();
    int colCount=ptr_table->columnCount();
    //防止渲染边界超过表总行数
    if(tableRowBegin<0){
        tableRowBegin=0;
    }
    if(tableRowEnd>=rowCount){
        tableRowEnd=rowCount-1;
    }
    for (int rowInTable = tableRowBegin; rowInTable <= tableRowEnd; ++rowInTable)
    {
        //如果此行已经加载过了,则不再加载
        if(rowHasloaded.contains(rowInTable)){
            continue;
        }else{
            rowHasloaded.insert(rowInTable,true);
            //进行关系转换,计算本行在文件记录List中的行数
            int rowInFileContent=(currentPage-1)*pageRowSize+rowInTable;
            //在分页模式下重设表头行号，以免非第一页的表头仍然从1开始计算行号
            QTableWidgetItem *itemVerticalHeaderItem= new QTableWidgetItem();
            ptr_table->setVerticalHeaderItem(rowInTable,itemVerticalHeaderItem);
            //行号取文件记录行号
            itemVerticalHeaderItem->setText(QString::number(rowInFileContent+1));
            //获取本行数据
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowInFileContent);
            //csv文件数据的某一行可能是不完整的，做忽略容忍！，允许正常解析
            //后续将不满足的数据记录在案
            //进行必填较验检查
            QMap<int,QString> checkresult;
            if(checkFieldFlag){
                checkRowFieldResult(rowdata,checkresult);
            }
            for(int col=0;col<colCount&&col<rowdata.count();col++){
                QString values=rowdata.at(col);
                //仅对数据非空单元格赋值
                if(!values.isEmpty()){
                    bool isDouble=false;
                    //如果本列在设定的数值列
                    if(CsvFieldIsNumberOrNot.contains(col)&&CsvFieldIsNumberOrNot.value(col).getIsNumber()){
                        //依然进行转换判断是否真的是数值，我们仅仅对真的数值设置数值格式
                        values.toDouble(&isDouble);
                        //转换成功
                        if(isDouble){
                            values=Utils::CovertDoubleQStringWithThousandSplit(values);
                        }
                    }
                    int colLength=values.length();
                    if(!columnWidth.contains(col)){
                        needRestwitdh.append(col);
                        columnWidth.insert(col,colLength);
                    }else
                        if(colLength>columnWidth.value(col)){
                            needRestwitdh.append(col);
                            columnWidth.insert(col,colLength);
                        }
                    QTableWidgetItem *item= new QTableWidgetItem();
                    //经过转换认证的真数值，靠右站站
                    if(isDouble){
                        item->setTextAlignment(Qt::AlignRight);
                    }
                    ptr_table->setItem(rowInTable, col, item);
                    item->setText(values);
                }
                else{
                    if (checkresult.contains(col)){
                        QTableWidgetItem *item= new QTableWidgetItem();
                        ptr_table->setItem(rowInTable, col, item);
                        item->setToolTip(checkresult.value(col));
                        item->setBackground(QColor(0XED5B56));
                    }
                }
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
        //判断某个字段的长度是不是非常的长,如果超过48字符,则标题栏居左,以免不太好找到标题信息
        if(columnWidth.contains(needRestwitdh.at(cc))&&columnWidth.value(needRestwitdh.at(cc))>48&&ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->textAlignment()!=Qt::AlignLeft){
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setTextAlignment(Qt::AlignLeft);
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setToolTip(ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip().isEmpty()?"超长字段,标题自动居左显示~":ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip()+"-超长字段,标题自动居左显示~");
        }
    }
}

void MainWindow::display_DBFTable(){
    QList<int> needRestwitdh;
    int rowCount=ptr_table->rowCount();
    int colCount=ptr_table->columnCount();
    //防止渲染边界超过表总行数
    if(tableRowBegin<0){
        tableRowBegin=0;
    }
    if(tableRowEnd>=rowCount){
        tableRowEnd=rowCount-1;
    }
    for (int rowInTable = tableRowBegin; rowInTable <= tableRowEnd; ++rowInTable)
    {
        //如果此行已经加载过了,则不再加载
        if(rowHasloaded.contains(rowInTable)){
            continue;
        }else{
            rowHasloaded.insert(rowInTable,true);
            //进行关系转换,计算本行在map中的key值
            int rowInDbfRowMap=(currentPage-1)*pageRowSize+rowInTable;
            //在分页模式下重设表头行号，以免非第一页的表头仍然从1开始计算行号
            QTableWidgetItem *itemVerticalHeaderItem= new QTableWidgetItem();
            //
            QString data=QString::number(rowInDbfRowMap+1);
            //获取本行数据
            QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowInDbfRowMap,&dbfRowMap,true,dbfTrimType);
            //删除处理-对于0模式下,对于删除的行展示为红色背景
            if(dbfDisplayType==0&&rowdata.last()=="1"){
                itemVerticalHeaderItem->setForeground(Qt::red);
                data=QString::number(rowInDbfRowMap+1);
                itemVerticalHeaderItem->setToolTip("此行为已删除的记录");
            }
            itemVerticalHeaderItem->setText(data);
            ptr_table->setVerticalHeaderItem(rowInTable,itemVerticalHeaderItem);
            for(int col=0;col<colCount;col++){
                QString values=rowdata.at(col);
                //仅对数据非空单元格赋值
                if(!values.isEmpty()){
                    bool isDouble=false;
                    //数值类字段千位分隔符展示
                    int type=dbf.getFieldList().at(col).getFieldType();
                    if(dbfIsNumberFieldType.contains(type)){
                        isDouble=true;
                        values=Utils::CovertDoubleQStringWithThousandSplit(values);
                    }
                    int colLength=values.toLocal8Bit().length();
                    if(!columnWidth.contains(col)){
                        needRestwitdh.append(col);
                        columnWidth.insert(col,colLength);
                    }else
                        if(colLength>columnWidth.value(col)){
                            needRestwitdh.append(col);
                            columnWidth.insert(col,colLength);
                        }
                    QTableWidgetItem *item= new QTableWidgetItem();
                    //是否对删除的数据全行设置为红色,纠结-启用
                    if(dbfDisplayType==0&&rowdata.last()=="1"){
                        item->setForeground(Qt::red);
                    }
                    if(isDouble){
                        item->setTextAlignment(Qt::AlignRight);
                    }
                    ptr_table->setItem(rowInTable, col, item);
                    item->setText(values);
                }
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
        //判断某个字段的长度是不是非常的长,如果超过48字符,则标题栏居左,以免不太好找到标题信息
        if(columnWidth.contains(needRestwitdh.at(cc))&&columnWidth.value(needRestwitdh.at(cc))>48&&ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->textAlignment()!=Qt::AlignLeft){
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setTextAlignment(Qt::AlignLeft);
            ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->setToolTip(ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip().isEmpty()?"超长字段,标题自动居左显示~":ptr_table->horizontalHeaderItem(needRestwitdh.at(cc))->toolTip()+"-超长字段,标题自动居左显示~");
        }
    }
}

/**
 * @brief MainWindow::clear_Table_Info 清空表格内容--包含包含数据信息等
 */
void MainWindow::clear_Table_Info(){
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    ptr_table->setColumnCount(0);
    ptr_table->setWordWrap(false);
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

/**
 * @brief MainWindow::clear_Table_Contents清空表格信息-仅清理内容，用于当前文件重新加载内容
 */
void MainWindow::clear_Table_Contents(){
    ptr_table->clearContents();
}

/**
 * @brief MainWindow::clear_Display_Info清理文件信息栏的函数，用于清理工具栏的所有框框
 */
void MainWindow::clear_Display_Info(){
    ui->lineEditSendCode->setText(nullptr);
    ui->lineEditRecCode->setText(nullptr);
    ui->lineEditFileTransferDate->setText(nullptr);
    ui->lineEditFileType->setText(nullptr);
    ui->lineEditSenfInfo->setText(nullptr);
    ui->lineEditRecInfo->setText(nullptr);
    ui->lineEditFileDescribe->setText(nullptr);
    ui->lineEditFileDescribe->setToolTip(nullptr);
    ui->lineEditUseIni->setText(nullptr);
    ui->lineEditUseIni->setToolTip(nullptr);
    statusBar_clear_statusBar();
}

/**
 * @brief MainWindow::clear_OldData清除旧数据，将加载到内存的数据全部清除-重新加载文件前请务必调用此方法
 */
void MainWindow::clear_OldData( bool keepdbfDisplayType, bool keepdbfTrimType){
    ofdIndexFileHeaderMap.clear();
    ofdindexFileDataList.clear();
    ofdFooterQString="";
    fileContentQByteArrayListFromExcel.clear();
    likeOFDFileType="";
    lastRowHasNewLine=true;
    commonHeaderQStringList.clear();
    commonContentQByteArrayList.clear();
    commonFooterQStringList.clear();
    primaryFirstRow.clear();
    primaryCheckResult.clear();

    if(dbftablefile.isOpen()){
        dbftablefile.close();
    }
    if(!keepdbfDisplayType){
        dbfDisplayType=0;
    }
    if(!keepdbfTrimType){
        dbfTrimType=0;
    }
    //默认隐藏dbf工具箱
    ui->menudbftools->menuAction()->setVisible(false);
    //清理行映射关系
    dbfRowMap.clear();
    rowHasloaded.clear();
    compareData.clear();
    columnWidth.clear();
    CsvFieldIsNumberOrNot.clear();
    //记录当前所在行
    tableRowCurrent=0;
    //当前所在列
    tableColCurrent=0;
    //更新列跳转搜索开始列
    tableColSearch=0;
    firstNotNumberLine=0;
    ///////////////分页组件重置/////////////////////
    //当前页号和总页数
    currentPage=1;
    pageCount=1;
    //隐藏分页组件
    ui->framePage->setVisible(false);
    //恢复按钮可点击性
    ui->pushButtonPageFirst->setEnabled(true);
    ui->pushButtonPagePrevious->setEnabled(true);
    ui->pushButtonPageNext->setEnabled(true);
    ui->pushButtonPageLast->setEnabled(true);
    //重新打开新的文件前关闭
    if(dialogDialogShowPrimaryCheck!=nullptr){
        dialogDialogShowPrimaryCheck->close();
        dialogDialogShowPrimaryCheck=nullptr;
    }
    /////////////////////////////////////////////
}

/**
 * @brief MainWindow::statusBar_disPlayMessage 状态栏显示消息的函数
 * @param text
 */
void MainWindow::statusBar_disPlayMessage(QString text){
    statusLabel_ptr_showMessage->setText(text);
    statusLabel_ptr_showMessage->setToolTip(text);
}


/**
 * @brief MainWindow::on_fileOpen_triggered点击打开文件按钮的槽函数
 */
void MainWindow::on_fileOpen_triggered()
{
    open_file_Dialog();
}

/**
 * @brief MainWindow::on_aboutOpen_triggered 点击关于菜单的槽函数，打开程序关于界面
 */
void MainWindow::on_aboutOpen_triggered()
{
    DialogAboutThis * dialog = new DialogAboutThis(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

/**
 * @brief MainWindow::on_actionAboutQt_triggered 关于qt
 */
void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,tr("关于QT"));
}

/**
 * @brief MainWindow::on_pushButtonOpenFile_clicked 菜单里的打开文件
 */
void MainWindow::on_pushButtonOpenFile_clicked()
{
    open_file_Dialog();
}

/**
 * @brief MainWindow::on_pushButtonOpenFile_2_clicked 文件检查
 */
void MainWindow::on_pushButtonOpenFile_2_clicked()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用文件检查功能...");
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        if(!ofdIndexFileHeaderMap.isEmpty()){
            //组织要显示的内容
            QString info;
            info.append("文件解析情况如下:\r\n");
            info.append("文件发送者代码:").append(ofdIndexFileHeaderMap.value("sendcode")).append("\r\n");
            info.append("文件接收者代码:").append(ofdIndexFileHeaderMap.value("recivecode")).append("\r\n");
            info.append("文件传递日期:").append(ofdIndexFileHeaderMap.value("filedate")).append("\r\n");
            info.append("文件内标识的总记录数:").append(ofdIndexFileHeaderMap.value("count")).append("成功加载记录数:").append(QString::number(ofdindexFileDataList.count())).append("\r\n");
            if(currentFileNewLineType!=newLineType::CRLF&&currentFileNewLineType!=newLineType::None){
                info.append("一般情况下OFD类文件的换行符应该为\\r\\n(CRLF),但目前不是~\r\n");
            }
            QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    else if(currentOpenFileType==openFileType::OFDFile){
        if(!commonHeaderQStringList.isEmpty()||commonHeaderQStringList.count()>=10){
            //组织要显示的内容
            QString info;
            info.append("文件解析情况如下:\r\n");
            info.append("文件发送者代码:").append(commonHeaderQStringList.at(2)).append("\r\n");
            info.append("文件接收者代码:").append(commonHeaderQStringList.at(3)).append("\r\n");
            info.append("文件传递日期:").append(commonHeaderQStringList.at(4)).append("\r\n");
            bool ok=false;
            int  fieldCount=commonHeaderQStringList.at(9).toInt(&ok);
            if(ok){
                int countRow=10+fieldCount;
                if(countRow>commonHeaderQStringList.count()-1){
                    info.append("从原文件读取记录数标志失败,").append("成功加载记录数:").append(QString::number(commonContentQByteArrayList.count())).append("\r\n");
                }
                else{
                    info.append("文件内标识的总记录数:").append(commonHeaderQStringList.at(countRow)).append(",成功加载记录数:").append(QString::number(commonContentQByteArrayList.count())).append("\r\n");
                }
            }else{
                info.append("读取文件字段数错误");
            }
            if(currentFileNewLineType!=newLineType::CRLF&&currentFileNewLineType!=newLineType::None){
                info.append("一般情况下OFD类文件的换行符应该为\\r\\n(CRLF),但目前不是~\r\n");
            }
            if(lastRowHasNewLine){
                info.append("文件最后一行有换行符~~\r\n");
            }
            else{
                info.append("文件最后一行没有换行符!!\r\n");
            }
            QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        //组织要显示的内容
        QString info;
        info.append("文件解析情况如下:\r\n");
        info.append("使用的配置文件:").append(csv.getFileIni()).append("\r\n");
        info.append("使用的解析器配置:[").append(csv.getFileNameWithCount()).append("]\r\n");
        info.append("加载的文件头行数:").append(QString::number(commonHeaderQStringList.count())).append("\r\n");
        info.append("加载的数据行数:").append(QString::number(commonContentQByteArrayList.count())).append("\r\n");
        info.append("配置的文件尾行数:").append(QString::number(csv.getEndIgnoreRow())).append(",实际加载的文件尾行数").append(QString::number(commonFooterQStringList.count())).append("\r\n");
        QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        //组织要显示的内容
        QString info;
        info.append("文件解析情况如下:\r\n");
        info.append("使用的配置文件:").append(fixed.getFileIni()).append("\r\n");
        info.append("使用的解析器配置:[").append(fixed.getFileNameWithVersion()).append("]\r\n");
        info.append("加载的文件头行数:").append(QString::number(commonHeaderQStringList.count())).append("\r\n");
        if(fixed.getRowcountcheckrow()>0&&fixed.getRowcountcheckrow()>=fixed.getRowcountcheckrow()){
            bool ok=false;
            commonHeaderQStringList.at(fixed.getRowcountcheckrow()-1).toInt(&ok,10);
            if(ok){
                info.append("文件内标识的总记录数").append(commonHeaderQStringList.at(fixed.getRowcountcheckrow()-1)).append(",实际加载的数据行数:").append(QString::number(commonContentQByteArrayList.count())).append("\r\n");
            }else{
                info.append("从文件头加载记录数失败(文件记录数所在行不是有效的数字)").append(",实际加载的数据行数:").append(QString::number(commonContentQByteArrayList.count())).append("\r\n");
            }
        }
        else{
            info.append("加载的数据行数:").append(QString::number(commonContentQByteArrayList.count())).append("\r\n");
        }
        info.append("配置的文件尾行数:").append(QString::number(fixed.getEndIgnoreRow())).append(",实际加载的文件尾行数:").append(QString::number(commonFooterQStringList.count())).append("\r\n");
        if(lastRowHasNewLine){
            info.append("文件最后一行有换行符~~\r\n");
        }
        else{
            info.append("文件最后一行没有换行符!!\r\n");
        }
        QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
    }
    //数据类型插入点
}

void MainWindow::copyToClipboardWithoutTitle(){
    copyToClipboard(false);
}

void MainWindow::copyToClipboardWithTitle(){
    copyToClipboard(true);
}

bool MainWindow::checkCSVVersion(CsvFileDefinition csv,QString versionRowData){
    if(csv.getVersioncheckrow()<1){
        return true;
    }
    else {
        if (csv.getVersioncheckmode()==0){
            if(versionRowData.trimmed()==csv.getVersion()){
                return true;
            }
            else{
                return false;
            }
        }
        else if (csv.getVersioncheckmode()>0){
            if(versionRowData.trimmed().contains(csv.getVersion(),Qt::CaseSensitive)){
                return true;
            }
            else{
                return false;
            }
        }
        else {
            return false;
        }
    }
}

/**
 * @brief MainWindow::copyToClipboard
 * 数据复制
 */
void MainWindow::copyToClipboard(bool withTitle){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount==1){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个单元格复制-取单元格
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            QString text="";
            QClipboard *board = QApplication::clipboard();
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
            }
            board->setText(text);
            statusBar_disPlayMessage(QString("已复制数据:\"%1\"到剪切板").arg(text));
        }
        //多个单元格复制-取原始数据
        else{
            int selectSum=(rigthColumn-leftColumn+1)*(bottomRow-topRow+1);
            if(selectSum>1000000){
                Toast::showMsg(QString("警告:选择的单元格超过1,000,000,无法复制！"), ToastTime::Time::ToastTime_normal,ToastType::Type::ToastType_warn,this);
            }
            else{
                QString value="";
                //插入标题
                if(withTitle){
                    for(int col=leftColumn;col<=rigthColumn;col++){
                        value.append(ptr_table->horizontalHeaderItem(col)->text());
                        if(col<rigthColumn){
                            value.append("\t");
                        }else{
                            value.append("\r\n");
                        }
                    }
                }
                int rowRealInContent=0;
                //在错误展示和OFD索引文件模式下复制取表格数据
                if(currentOpenFileType==openFileType::NotFileOrErr||currentOpenFileType==openFileType::OFDIndex){
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
                }
                //OFD模式下的复制
                else if(currentOpenFileType==openFileType::OFDFile){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            //数据不能取自表格，表格数据基于懒加载机制的情况下数据会不全
                            if(ofd.getFieldList().at(col).getFieldType()=="N"){
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,col)));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,col));
                            }
                            if(col<rigthColumn){
                                value.append("\t");
                            }else if(row<bottomRow){
                                value.append("\r\n");
                            }
                        }
                    }
                }
                //CSV文件下的复制
                else if(currentOpenFileType==openFileType::CSVFile){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            //数据不能取自表格，表格数据基于懒加载机制的情况下数据会不全
                            //预防csv行不足的情况
                            if(col<rowdata.count()){
                                if(CsvFieldIsNumberOrNot.contains(col)&&CsvFieldIsNumberOrNot.value(col).getIsNumber()){
                                    value.append(Utils::CovertDoubleQStringWithThousandSplit(rowdata.at(col)));
                                }
                                else{
                                    value.append(rowdata.at(col));
                                }
                            }else{
                                value.append("");
                            }
                            if(col<rigthColumn){
                                value.append("\t");
                            }else if(row<bottomRow){
                                value.append("\r\n");
                            }
                        }
                    }
                }
                //FIXED模式下的复制
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            //数据不能取自表格，表格数据基于懒加载机制的情况下数据会不全
                            if(fixed.getFieldList().at(col).getFieldType()=="N"){
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,col)));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,col));
                            }
                            if(col<rigthColumn){
                                value.append("\t");
                            }else if(row<bottomRow){
                                value.append("\r\n");
                            }
                        }
                    }
                }
                //DBF
                else if(currentOpenFileType==openFileType::DBFFile){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            if(dbfColIsNumber(col)){
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,col,&dbfRowMap,dbfTrimType)));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,col,&dbfRowMap,dbfTrimType));
                            }
                            if(col<rigthColumn){
                                value.append("\t");
                            }else if(row<bottomRow){
                                value.append("\r\n");
                            }
                        }
                    }
                }
                QClipboard *board = QApplication::clipboard();
                board->setText(value);
                statusBar_disPlayMessage("已复制"+QString::number(selectSum)+"个单元格数据到剪切板～");
            }
        }
    }
    else if(rangeCount>1){
        Toast::showMsg(QString("无法对多重选择区域执行复制！"), ToastTime::Time::ToastTime_normal,ToastType::Type::ToastType_warn,this);
    }
}

/**
 * @brief MainWindow::editCompareData 编辑比对器内容
 */
void MainWindow::editCompareData(){
    const int MaxCompareRow=200;
    int rowRealInContent=0;
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    int topRow=itemsRange.at(0).topRow();
    int bottomRow=itemsRange.at(0).bottomRow();
    //单行加入比对器--单行并且单个选择器
    if(topRow==bottomRow&&itemsRange.count()==1){
        rowRealInContent=(currentPage-1)*pageRowSize+topRow;
        if(!compareData.contains(rowRealInContent+1)){
            if(compareData.count()>=MaxCompareRow){
                statusBar_disPlayMessage(QString("比对器内已有"+QString::number(MaxCompareRow)+"行数据，每次比对最多"+QString::number(MaxCompareRow)+"行！"));
            }
            else{
                //数据加入
                //OFD
                if(currentOpenFileType==openFileType::OFDFile){
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                }
                //CSV
                else if(currentOpenFileType==openFileType::CSVFile){
                    compareData.insert(rowRealInContent+1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent));
                }
                //FIXED
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                }
                //DBF
                else if(currentOpenFileType==openFileType::DBFFile){
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType));
                }
                statusBar_disPlayMessage(QString("比对器内已加入%1行数据").arg(QString::number(compareData.count())));
            }
        }
        else{
            //移除数据行
            compareData.remove(rowRealInContent+1);
            if(compareData.count()>0){
                statusBar_disPlayMessage(QString("比对器内还有%1行数据").arg(QString::number(compareData.count())));
            }
            else{
                statusBar_disPlayMessage(QString("比对器已清空"));
            }
        }
    }
    //单行多选择器--或者多行但选择器--或者多行多选择器
    else{
        int selectSum=bottomRow-topRow+1;
        //如果range大于1，循环累加行
        if(rangeCount>1){
            for(int cc=1;cc<rangeCount;cc++){
                selectSum+=itemsRange.at(cc).bottomRow()-itemsRange.at(cc).topRow()+1;
            }
        }
        if(selectSum>MaxCompareRow){
            statusBar_disPlayMessage("警告:最多进行"+QString::number(MaxCompareRow)+"行数据比对,目前选择了"+QString::number(selectSum)+"行!");
        }
        else if((selectSum+compareData.count())>MaxCompareRow){
            statusBar_disPlayMessage("警告:最多进行"+QString::number(MaxCompareRow)+"行数据比对,比对器内已有"+QString::number(compareData.count())+"行，目前还可加入"+QString::number(MaxCompareRow-compareData.count())+"行!");
        }
        //开始数据加入
        else{
            //OFD
            if(currentOpenFileType==openFileType::OFDFile){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                        }
                    }
                }
            }
            //CSV
            else if(currentOpenFileType==openFileType::CSVFile){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent));
                        }
                    }
                }
            }
            //FIXED
            else if(currentOpenFileType==openFileType::FIXEDFile){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                        }
                    }
                }
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType));
                        }
                    }
                }
            }
            statusBar_disPlayMessage(QString("比对器内已加入%1行数据").arg(QString::number(compareData.count())));
        }
    }
}


void MainWindow::deleteRowDataFromFileAndTable(){
    //正在进行的阻断操作时，禁止删除数据
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        DialogMyTip  dialog2("真的要删除选择的数据行吗？",this);
        dialog2.setWindowTitle("警告！");
        dialog2.setModal(true);
        dialog2.exec();
        //从弹窗中获取结果//如果放弃删除数据
        if(!dialog2.getBoolFlag()){
            statusBar_disPlayMessage("放弃删除...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在删除数据,请稍候...";
        statusBar_disPlayMessage(dataBlockedMessage);
        //获取选择的范围
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        //先整理明确需要删除的行
        QSet<int> deleteRowSet;
        for(int i=0;i<itemsRange.count();i++){
            int top=itemsRange.at(i).topRow();
            int bottom=itemsRange.at(i).bottomRow();
            for(int j=top;j<=bottom;j++){
                deleteRowSet.insert(j);
                if((j%5000==0)){
                    qApp->processEvents();
                    statusBar_disPlayMessage(QString("正在评估数据中,本页%1行,已评估需删除%2行").arg(QString::number(ptr_table->rowCount()),QString::number(deleteRowSet.count())));
                    if(deleteRowSet.count()>200000){
                        statusBar_disPlayMessage("请不要一次选择删除超过20万行数据...");
                        dataBlocked=false;
                        QApplication::restoreOverrideCursor();
                        return;
                    }
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        //如果多选择器的选择顺序不是从上往下，存在可能deleteRowList里的顺序不是完全顺序的，这里进行重新排序
        //排序后从后一个开始删除，确保删除正确
        QList<int> deleteRowList=deleteRowSet.values();
        std::sort(deleteRowList.begin(), deleteRowList.end());
        int count=deleteRowList.count();
        //判断是不是要删除所有行--删除所有行要使用单独的方法
        //如果删除所有行-既全选能删除所有行，表明目前的文件未分页
        if(count==commonContentQByteArrayList.count()){
            //清除所有内容
            clear_Table_Contents();
            //设置表格行数为0
            ptr_table->setRowCount(0);
            //清空加载的原始数据
            commonContentQByteArrayList.clear();
            //清空行加载记录
            rowHasloaded.clear();
            //更新记录数标记
            reCalculateFileCountLine();
            //清空比对器内容
            compareData.clear();
            //更新提示-更新主窗口标题
            statusBar_disPlayMessage(QString("选择的%1行数据已删除,请记得保存文件哟...").arg(QString::number(count)));
            this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
            //更新文件修改标记
            fileChanged=true;
            //大功告成-处理下分页的问题
            currentPage=1;
            pageCount=1;
            ui->framePage->setVisible(false);
            if(dialogDialogShowPrimaryCheck!=nullptr){
                needReCheckPrimary=true;
                statusBar_disPlayMessage(QString("选择的%1数据行已删除,检测到数据行更新,请重新进行主键冲突检查...").arg(QString::number(count)));
            }
            else{
                statusBar_disPlayMessage(QString("选择的%1数据行已删除...").arg(QString::number(count)));
            }
        }
        else{
            int row=0;
            int rowRealInContent=0;
            compareData.clear();
            QApplication::setOverrideCursor(Qt::WaitCursor);
            for(int w=count-1;w>=0;w--){
                row=deleteRowList.at(w);
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                commonContentQByteArrayList.removeAt(rowRealInContent);
                if((w%5000==0)){
                    statusBar_disPlayMessage(QString("正在删除数据中,选择%1行,已删除%2行...").arg(QString::number(count),QString::number(count-w)));
                    qApp->processEvents();
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
            QApplication::restoreOverrideCursor();
            rowHasloaded.clear();
            reCalculateFileCountLine();
            int rowCount=commonContentQByteArrayList.count();
            int targetRow=deleteRowList.at(0);
            int targetRowRealInContent=(currentPage-1)*pageRowSize+targetRow;
            if(targetRowRealInContent+1>rowCount){
                if(targetRowRealInContent>0){
                    targetRow=targetRow-1;
                }
            }
            int tmpPageCount=(rowCount + pageRowSize - 1) / pageRowSize;
            if(tmpPageCount==pageCount){
                pageJump(currentPage,targetRow);
            }
            else{
                if(tmpPageCount<2){
                    pageCount=1;
                    currentPage=1;
                    ui->framePage->setVisible(false);
                    pageJump(1,targetRow);
                }
                else{
                    if(currentPage==pageCount){
                        pageCount=tmpPageCount;
                        currentPage=pageCount;
                        ui->framePage->setVisible(true);
                        pageJump(pageCount,pageRowSize-1);
                    }
                    else{
                        pageCount=tmpPageCount;
                        ui->framePage->setVisible(true);
                        pageJump(currentPage,targetRow);
                    }
                }
            }
            //更新提示
            statusBar_disPlayMessage(QString("选择的%1行数据已删除,请记得保存文件哟...").arg(QString::number(count)));
            this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
            //更新文件修改标记//////////////////////////////////////////////////////////
            fileChanged=true;
            if(dialogDialogShowPrimaryCheck!=nullptr){
                needReCheckPrimary=true;
                statusBar_disPlayMessage(QString("选择的%1数据行已删除,检测到数据行更新,请重新进行主键冲突检查...").arg(QString::number(count)));
            }
            else{
                statusBar_disPlayMessage(QString("选择的%1数据行已删除...").arg(QString::number(count)));
            }
        }
        dataBlocked=false;
        //更新总记录数信息
        statusBar_display_rowsCount(commonContentQByteArrayList.count());
    }
}

void MainWindow::copyRowData(){
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        QString version=ui->lineEditUseIni->text()+"|Author:"+QString(QByteArray::fromBase64(AUTHOR_EMAIL));
        QString type="";
        if(currentOpenFileType==openFileType::OFDFile){
            type=ofd.getConfigSegment();
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            type=fixed.getConfigSegment();
        }
        QString needCopyData=QString().append(version).append("\r\n").append(type).append("\r\n");
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        //先整理明确需要复制的行
        QList<int> copyRowList;
        for(int i=0;i<itemsRange.count();i++){
            int top=itemsRange.at(i).topRow();
            int bottom=itemsRange.at(i).bottomRow();
            for(int j=top;j<=bottom;j++){
                //有可能两个选择范围是在一行的，不要重复加入行
                if(!copyRowList.contains(j)){
                    copyRowList.append(j);
                }
            }
        }
        //判断是否数据超过了5万行
        int count=copyRowList.count();
        if(count>50000){
            DialogMyTip dialog2("即将复制大量数据到剪切板,数据有"+QString::number(count)+"行,可能会比较慢或者占用大量剪切板空间，确定继续吗？",this);
            dialog2.setWindowTitle("警告！");
            dialog2.setModal(true);
            dialog2.exec();
            if(!dialog2.getBoolFlag()){
                statusBar_disPlayMessage("放弃复制...");
                return;
            }
        }
        dataBlocked=true;
        dataBlockedMessage="正在复制数据,请稍候...";
        //对需要复制的行排序--当数据量比较大时将会比较耗时
        std::sort(copyRowList.begin(), copyRowList.end());
        statusBar_disPlayMessage(dataBlockedMessage);
        int rowRealInContent=0;
        for(int j=0;j<count;j++){
            //将本行数据加入待复制列表
            //做行号转换
            rowRealInContent=(currentPage-1)*pageRowSize+copyRowList.at(j);
            if(dataCompressLevel==0){
                needCopyData.append(codecOFD->toUnicode(commonContentQByteArrayList.at(rowRealInContent)));
            }
            else{
                needCopyData.append(codecOFD->toUnicode(qUncompress(commonContentQByteArrayList.at(rowRealInContent))));
            }
            if(j<count-1){
                needCopyData.append("\r\n");
            }
        }
        //数据准备完毕开始复制
        QClipboard *board = QApplication::clipboard();
        board->setText(needCopyData);
        dataBlocked=false;
        statusBar_disPlayMessage(QString("已复制%1行文件原始记录数据到剪切板").arg(QString::number(count)));
    }
}

void MainWindow::addRowDataPreviousRow(){
    addNewRowData(-1);
}

void MainWindow::addRowDataNextRow(){
    addNewRowData(1);
}

void MainWindow::addRowDataEndRow(){
    addNewRowData(0);
}

void MainWindow::addNewLineRowDataEndRow(){
    addNewRowData(2);
}

void MainWindow::addNewRowData(int location){
    int insertIndexInContent=0;
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    //////////////计算文件插入位置/////////////////////////////////
    //跳转标记插入位置
    int fIndex=0;
    //当前文件是空文件
    if(itemsRange.count()<1){
        insertIndexInContent=0;
    }
    //当前文件是非空文件
    else{
        fIndex=itemsRange.at(0).topRow()+((currentPage-1)*pageRowSize);
        //文件非空-插入到上方
        //向上插入不能在多行选择时显示菜单，否则这里的插入位置会存在缺陷
        if(location==-1){
            insertIndexInContent=itemsRange.at(0).topRow()+((currentPage-1)*pageRowSize);
        }
        //文件非空-插入到下方
        //向下插入不能在多行选择时显示菜单，否则这里的插入位置会存在缺陷
        else if(location==1){
            insertIndexInContent=itemsRange.at(0).topRow()+1+((currentPage-1)*pageRowSize);;
        }
        //文件非空-插入到文件尾
        else {
            insertIndexInContent=commonContentQByteArrayList.count();
        }
    }
    //////////////////////////////////////////////////////////
    //插入空数据行
    if(location==2){
        DialogMyTip dialog2("真的要为文件新增一行数据么？如果确定，请新增后编辑数据内容！",this);
        dialog2.setWindowTitle("警告！");
        dialog2.setModal(true);
        dialog2.exec();
        if(!dialog2.getBoolFlag()){
            statusBar_disPlayMessage("放弃新增行...");
            return;
        }
        QByteArray newLine;
        //使用更为安全的方式新增空记录
        if(currentOpenFileType==openFileType::OFDFile){
            int Length=ofd.getRowLength();
            for(int charIndex=0;charIndex<Length;charIndex++){
                newLine.append(QChar(' ').toLatin1());
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            while(true){
                newLine.append(QChar(' ').toLatin1());
                if (fixed.getRowLengthHash().contains(newLine.length())){
                    break;
                }
            }
        }
        if(dataCompressLevel==0){
            commonContentQByteArrayList.insert(insertIndexInContent,newLine);
        }
        else{
            commonContentQByteArrayList.insert(insertIndexInContent,qCompress(newLine,dataCompressLevel));
        }
        //更新记录数标记
        reCalculateFileCountLine();
        //新的总行数
        int rowCount=commonContentQByteArrayList.count();
        //添加完数据没有引发分页
        if(rowCount<=pageRowSize){
            //跳到第一页重加载
            pageJump(1);
            //滚动进度条
            ptr_table->scrollToBottom();
            //选中新增的行
            QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(insertIndexInContent,0,insertIndexInContent,ptr_table->columnCount()-1);
            ptr_table->clearSelection();
            ptr_table->setRangeSelected(addRange,true);
        }
        //添加完数据数据大于1页，或者原本数据就大于1页-则跳到最后一页，并选中最后一行
        else{
            ui->framePage->setVisible(true);
            //表格行数为第一页的行数
            pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
            //跳转到最后一页的底部
            pageJump(pageCount,-1);
            //选中新增的行
            QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(ptr_table->rowCount()-1,0,ptr_table->rowCount()-1,ptr_table->columnCount()-1);
            ptr_table->clearSelection();
            ptr_table->setRangeSelected(addRange,true);
        }
        //---
        fileChanged=true;
        dataBlocked=false;
        //更新总记录数
        statusBar_display_rowsCount(rowCount);
        statusBar_disPlayMessage(QString("新增空行完毕,请编辑填充数据..."));
        this->setWindowTitle(appName+"-"+currentFileName+"-新增数据行待编辑保存");
        if(dialogDialogShowPrimaryCheck!=nullptr){
            needReCheckPrimary=true;
            statusBar_disPlayMessage("检测到数据行更新,请重新进行主键冲突检查...");
        }
    }
    //插入剪切板数据
    else{
        qDebug()<<"开始插入数据";
        //开始获取剪切板内容
        QClipboard *board = QApplication::clipboard();
        //对换行符切割
        //不同操作系统系统复制过来的数据，换行符号不同，我们这里做统一替换
        //一个坑爹的问题
        QStringList data=board->text().replace("\r\n","\n").replace("\r","\n").split("\n");
        //开始进行数据比对
        if(data.count()<3||!data.at(0).contains("Author")){
            statusBar_disPlayMessage("剪切板上无有效的数据,无法插入!");
            return;
        }
        else {
            QString version=data.at(0);
            QString fileType=data.at(1);
            if(currentOpenFileType==openFileType::OFDFile){
                if(fileType!=ofd.getConfigSegment()){
                    statusBar_disPlayMessage(QString("不能将[%1]文件的数据插入到[%2]文件,不匹配的文件类型!").arg(fileType,ofd.getConfigSegment()));
                    return;
                }
                else if(version!=ui->lineEditUseIni->text()+"|Author:"+QByteArray::fromBase64(AUTHOR_EMAIL)){
                    statusBar_disPlayMessage("不同版本的文件,无法将数据插入!");
                    return;
                }
            }
            else if (currentOpenFileType==openFileType::FIXEDFile){
                if(fileType!=fixed.getConfigSegment()){
                    statusBar_disPlayMessage(QString("不能将[%1]文件的数据插入到[%2]文件,不匹配的文件类型!").arg(fileType,fixed.getConfigSegment()));
                    return;
                }
                else if(version!=ui->lineEditUseIni->text()+"|Author:"+QByteArray::fromBase64(AUTHOR_EMAIL)){
                    statusBar_disPlayMessage("不同版本的文件,无法将数据插入!");
                    return;
                }
            }
            //文件版本和文件类型匹配开始准备数据转换---开始准备插入数据
            QList<QByteArray> dataQByteArrayList;
            //提取剪切板数据并校验长度
            //从最后一行倒序提取-边提取边删除QStringlist的最后一行，释放内存
            int count=data.count();
            bool dataok=true;
            if(currentOpenFileType==openFileType::OFDFile){
                for(int i=count-1;i>=2;i--){
                    QByteArray row=codecOFD->fromUnicode(data.at(i));
                    if(row.length()==ofd.getRowLength()){
                        if(dataCompressLevel==0){
                            dataQByteArrayList.append(row);
                        }
                        else{
                            dataQByteArrayList.append(qCompress(row,dataCompressLevel));
                        }
                        data.removeAt(i);
                    }
                    else{
                        dataok=false;
                        statusBar_disPlayMessage(QString("剪切板上第%1行数据的长度不满足接口规范,放弃插入剪切板数据!").arg(QString::number(i)));
                        break;
                    }
                }
            }
            else if (currentOpenFileType==openFileType::FIXEDFile){
                for(int i=count-1;i>=2;i--){
                    QTextCodec *codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
                    QByteArray row=codec->fromUnicode(data.at(i));
                    if(fixed.getRowLengthHash().contains(row.length())){
                        //字节数据插入一行数据，QStringlist删除一行数据
                        if(dataCompressLevel==0){
                            dataQByteArrayList.append(row);
                        }
                        else{
                            dataQByteArrayList.append(qCompress(row,dataCompressLevel));
                        }
                        data.removeAt(i);
                    }
                    else{
                        dataok=false;
                        statusBar_disPlayMessage(QString("剪切板上第%1行数据的长度不满足接口规范,放弃插入剪切板数据!").arg(QString::number(i)));
                        break;
                    }
                }
            }
            if(dataok){
                dataBlocked=true;
                dataBlockedMessage="正在写入剪切板数据";
                compareData.clear();
                int dataCount=dataQByteArrayList.count();
                //倒序插入，边插入边删除dataQByteArrayList
                //注意这里不要调整顺序，此处倒序和从剪切板拉取数据的倒序相结合，倒倒得正
                int indexAdd=0;
                for(int j=dataCount-1;j>=0;j--){
                    commonContentQByteArrayList.insert(insertIndexInContent+indexAdd,dataQByteArrayList.at(j));
                    dataQByteArrayList.removeAt(j);
                    indexAdd++;
                }
                reCalculateFileCountLine();
                //新的总行数
                int rowCount=commonContentQByteArrayList.count();
                ///////////////数据插入完毕处理页面跳转显示的问题///////////////////
                //添加完数据没有引发分页-原来也就1页
                if(rowCount<=pageRowSize){
                    //借助跳到第一页的方式重加载
                    pageJump(1);
                    //滚动进度条
                    if(location==0){
                        ptr_table->scrollToBottom();
                    }
                    else{
                        ptr_table->setCurrentCell(insertIndexInContent+dataCount-1,0);
                    }
                    //选中新增的行
                    QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(insertIndexInContent,0,insertIndexInContent+dataCount-1,ptr_table->columnCount()-1);
                    ptr_table->clearSelection();
                    ptr_table->setRangeSelected(addRange,true);
                    fileChanged=true;
                    dataBlocked=false;
                    //强制触发下刷新，避免显示数据不完整
                    acceptVScrollValueChanged(0);
                    //更新总记录数
                    statusBar_display_rowsCount(rowCount);
                    statusBar_disPlayMessage(QString("剪切板上的%1行数据插入到本文件完毕,记得保存文件哟...").arg(QString::number(dataCount)));
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    if(dialogDialogShowPrimaryCheck!=nullptr){
                        needReCheckPrimary=true;
                        statusBar_disPlayMessage("检测到数据行更新,请重新进行主键冲突检查...");
                    }
                }
                //添加完数据数据大于1页，或者原本数据就大于1页-需要判断跳转的位置
                else{
                    //展示分页组件
                    ui->framePage->setVisible(true);
                    //更新总行数
                    statusBar_display_rowsCount(rowCount);
                    //统计新的页数
                    pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
                    //更新文件修改标记-释放阻断
                    fileChanged=true;
                    dataBlocked=false;
                    //重要的逻辑---判断需要跳转的位置
                    //跳到文件末尾
                    if (location==0){
                        fIndex=commonContentQByteArrayList.count()-dataCount;
                    }
                    //跳转到插入点的下方
                    else if(location==1){
                        fIndex+=1;
                    }
                    //计算页码和位置
                    int targetPage=(fIndex/pageRowSize)+1;
                    int targetTableIndex=(fIndex%pageRowSize);
                    pageJump(targetPage);
                    //滚动并选中第一行
                    ptr_table->setCurrentCell(targetTableIndex,0);
                    QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(targetTableIndex,0,targetTableIndex,ptr_table->columnCount()-1);
                    ptr_table->clearSelection();
                    ptr_table->setRangeSelected(addRange,true);
                    if(dataCount==1){
                        statusBar_disPlayMessage(QString("剪切板上的%1行数据插入到本文件完毕,记得保存文件哟...").arg(QString::number(dataCount)));
                    }
                    else{
                        statusBar_disPlayMessage(QString("剪切板上的%1行数据插入到本文件完毕,已选中插入的第一行数据,记得保存文件哟...").arg(QString::number(dataCount)));
                    }
                    //强制触发下刷新，避免显示数据不完整
                    acceptVScrollValueChanged(0);
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    if(dialogDialogShowPrimaryCheck!=nullptr){
                        needReCheckPrimary=true;
                        statusBar_disPlayMessage("检测到数据行更新,请重新进行主键冲突检查...");
                    }
                }
            }
        }
    }
}

/**
 * @brief MainWindow::showRowDetails 显示行记录详情
 */
void MainWindow::showRowDetails(){
    //行
    int dataRowCurrent=(currentPage-1)*pageRowSize+tableRowCurrent;
    int colCount=ptr_table->columnCount();
    statusBar_disPlayMessage(QString("查看第%1行数据").arg(QString::number(dataRowCurrent+1)));
    //定义一个Qlist存储此行的数据,将表格的列转换为行，共计四列
    //数据内容从表格取，从原始数据取还需要转换
    QList<QStringList> rowdata;
    //数据类型插入点
    if(currentOpenFileType==openFileType::OFDFile){
        for(int i=0;i<colCount;i++){
            QStringList colitem;
            if(ptr_table->item(tableRowCurrent,i)==nullptr){
                //字段中文名
                colitem.append(ofd.getFieldList().at(i).getFieldDescribe());
                //字段英文名
                colitem.append(ofd.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(nullptr);
                //字典翻译
                colitem.append(nullptr);
            }
            else{
                QString colvalue=ptr_table->item(tableRowCurrent,i)->text();
                //字段名
                colitem.append(ofd.getFieldList().at(i).getFieldDescribe());
                colitem.append(ofd.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(ofdDictionary.getDictionary(ofd.getDictionary()+"_"+ofd.getFieldList().at(i).getFieldName(),colvalue));
            }
            //tips
            colitem.append(ptr_table->horizontalHeaderItem(i)->toolTip());
            rowdata.append(colitem);
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        for(int i=0;i<colCount;i++){
            QStringList colitem;
            if(ptr_table->item(tableRowCurrent,i)==nullptr){
                //字段中文名
                colitem.append(csv.getFieldList().at(i).getFieldDescribe());
                //字段英文名
                colitem.append(csv.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(nullptr);
                //字典翻译
                colitem.append(nullptr);
            }
            else{
                QString colvalue=ptr_table->item(tableRowCurrent,i)->text();
                //字段名
                colitem.append(csv.getFieldList().at(i).getFieldDescribe());
                colitem.append(csv.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(commonDictionary.value("CSV"+csv.getFileIni()).getDictionary(csv.getFieldList().at(i).getFieldDescribe(),colvalue));
            }
            //tips
            colitem.append(ptr_table->horizontalHeaderItem(i)->toolTip());
            rowdata.append(colitem);
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        for(int i=0;i<colCount;i++){
            QStringList colitem;
            if(ptr_table->item(tableRowCurrent,i)==nullptr){
                //字段中文名
                colitem.append(fixed.getFieldList().at(i).getFieldDescribe());
                //字段英文名
                colitem.append(fixed.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(nullptr);
                //字典翻译
                colitem.append(nullptr);
            }
            else{
                QString colvalue=ptr_table->item(tableRowCurrent,i)->text();
                //字段名
                colitem.append(fixed.getFieldList().at(i).getFieldDescribe());
                colitem.append(fixed.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(commonDictionary.value("FIXED"+fixed.getFileIni()).getDictionary(fixed.getFieldList().at(i).getFieldDescribe(),colvalue));
            }
            //tips
            colitem.append(ptr_table->horizontalHeaderItem(i)->toolTip());
            rowdata.append(colitem);
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        for(int i=0;i<colCount;i++){
            QStringList colitem;
            if(ptr_table->item(tableRowCurrent,i)==nullptr){
                //字段中文名
                colitem.append(dbf.getFieldList().at(i).getFieldDescribe());
                //字段英文名
                colitem.append(dbf.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(nullptr);
                //字典翻译
                colitem.append(nullptr);
            }
            else{
                QString colvalue=ptr_table->item(tableRowCurrent,i)->text();
                //字段名
                colitem.append(dbf.getFieldList().at(i).getFieldDescribe());
                colitem.append(dbf.getFieldList().at(i).getFieldName());
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(commonDictionary.value("DBF"+dbf.getFileIni()).getDictionary(dbf.getFieldList().at(i).getFieldName().toUpper(),colvalue));
            }
            //tips
            colitem.append(ptr_table->horizontalHeaderItem(i)->toolTip());
            rowdata.append(colitem);
        }
    }
    //打开窗口
    DialogShowTableRow * dialog = new DialogShowTableRow(&rowdata,this);
    dialog->setWindowTitle(QString("查看表格行记录-第%1行").arg(QString::number(dataRowCurrent+1)));
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}


/**
 * @brief MainWindow::showMagnify 打开放大镜
 */
void MainWindow::showMagnify(){
    if(ptr_table->item(tableRowCurrent,tableColCurrent)==nullptr||ptr_table->item(tableRowCurrent,tableColCurrent)->text().isEmpty()){
        statusBar_disPlayMessage("该单元格无数据...");
    }
    else{
        QString data=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
        //打开窗口
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        DialogMagnify * dialog = new DialogMagnify(data,this);
        dialog->setWindowTitle(QString("数据放大镜-第%1行第%2列").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1)));
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

/**
 * @brief MainWindow::showCharacter
 */
void MainWindow::showCharacter(){
    //定义一个Qlist存储此行的数据,将表格的列转换为行，共计四列
    QList<QStringList> rowdata;
    QString charset="UTF-8";
    if(ptr_table->item(tableRowCurrent,tableColCurrent)==nullptr||ptr_table->item(tableRowCurrent,tableColCurrent)->text().isEmpty()){
        statusBar_disPlayMessage("该单元格无数据...");
    }
    else{
        QString data=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
        //获取编码方案
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        //获得当前打开的文件的编码方案
        if(currentOpenFileType==openFileType::OFDFile){
            charset="GB18030";
            codec=codecOFD;
        }
        else if (currentOpenFileType==openFileType::CSVFile){
            charset=csv.getEcoding();
            codec=QTextCodec::codecForName(charset.toLocal8Bit());
        }
        else if (currentOpenFileType==openFileType::FIXEDFile){
            charset=fixed.getEcoding();
            codec=QTextCodec::codecForName(charset.toLocal8Bit());
        }
        else if (currentOpenFileType==openFileType::DBFFile){
            //todo
            charset=dbf.getEcoding();
            codec=QTextCodec::codecForName(charset.toLocal8Bit());
        }
        //拆字符循环处理
        for(int i=0;i<data.count();i++){
            QStringList colitem;
            //1.9.23版本，针对4字节Unicode字符做特殊兼容
            //如果是高字节，则下一位是低字节，取两位
            if(data.at(i).isHighSurrogate()&&i<data.count()-1&&data.at(i+1).isLowSurrogate()){
                uint uintunicode = QChar::surrogateToUcs4(data[i].unicode(), data[i+1].unicode());
                QString hex=codec->fromUnicode(data.mid(i,2)).toHex().toUpper();
                //字符
                colitem.append(data.mid(i,2));
                //字节数
                colitem.append(QString::number(hex.length()/2));
                //16进制
                colitem.append(hex);
                //Unicode16进制
                colitem.append(QString::number(uintunicode,16).toUpper());
                //特殊字符说明
                if(specialCharacter.contains(uintunicode)){
                    colitem.append(specialCharacter.value(uintunicode));
                }
                else{
                    colitem.append(nullptr);
                }
                //跳过下一个Qchar的处理
                i++;
            }
            else{
                QString hex=codec->fromUnicode(data.at(i)).toHex().toUpper();
                uint uintunicode=data.at(i).unicode();
                qDebug()<<uintunicode;
                //字符
                colitem.append(data.at(i));
                //字节数
                colitem.append(QString::number(hex.length()/2));
                //16进制
                colitem.append(hex);
                //Unicode16进制
                colitem.append(QString::number(uintunicode,16).toUpper());
                //特殊字符说明
                if(specialCharacter.contains(uintunicode)&&!puaOrDeletedGBKCharacter.contains(uintunicode)){
                    colitem.append(specialCharacter.value(uintunicode));
                }
                else if(!specialCharacter.contains(uintunicode)&&puaOrDeletedGBKCharacter.contains(uintunicode)){
                    colitem.append(puaOrDeletedGBKCharacter.value(uintunicode));
                }
                else{
                    colitem.append(nullptr);
                }
            }
            rowdata.append(colitem);
        }
    }
    //打开窗口
    DialogShowCharset * dialog = new DialogShowCharset(&rowdata,charset,this);
    dialog->setWindowTitle(QString("查看十六进制字符编码"));
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}


void MainWindow::forceNumber(){
    int flag=-1;
    //取当前设定
    if(CsvFieldIsNumberOrNot.contains(tableColCurrent)){
        if(CsvFieldIsNumberOrNot.value(tableColCurrent).getIsNumber()){
            flag=CsvFieldIsNumberOrNot.value(tableColCurrent).getDecimalLength();
        }
        else{
            flag=-1;
        }
    }
    DialogForceNumber  dialog (flag,this);
    dialog.setWindowTitle(QString("对此列调整数据格式-%1").arg(csv.getFieldList().at(tableColCurrent).getFieldDescribe()));
    dialog.setModal(true);
    dialog.exec();
    int flag2=dialog.getFlag();
    if(flag2==-2){
        //statusBar_disPlayMessage("取消设置...");
    }
    //更新设置--没有改动就不必再处理了
    else if (flag2!=flag){
        FieldIsNumber isnumber;
        isnumber.setDecimalLength(flag2);
        if(flag2>-1){
            isnumber.setIsNumber(true);
        }
        else{
            isnumber.setIsNumber(false);
        }
        CsvFieldIsNumberOrNot.insert(tableColCurrent,isnumber);
        //清除重新加载
        rowHasloaded.clear();
        acceptVScrollValueChanged(-1);
        //触发统计
        on_tableWidget_itemSelectionChanged();
    }
}

/**
* @brief MainWindow::copyMessage 复制状态栏信息
*/
void MainWindow:: copyMessage(){
    QString text=statusLabel_ptr_showMessage->text();
    QClipboard *board = QApplication::clipboard();
    board->setText(text);
}

/**
* @brief MainWindow::gotoFirstNotNumber 跳转到第一个非法数值的位置
*/
void MainWindow:: gotoFirstNotNumber(){
    int tableRow=firstNotNumberLine-(currentPage-1)*pageRowSize-1;
    ptr_table->setCurrentCell(tableRow,tableColCurrent);
    ptr_table->setFocus();
}

void MainWindow:: showOFDOrFixedFiledAnalysis(){
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        statusBar_disPlayMessage(QString("分析第%1行第%2列数据数据").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1)));
        //字段中文名
        QString fieldDes="";
        //字段英文名
        QString fieldName="";
        //字段类型
        QString fieldType="";
        //字段长度
        int fieldLength=0;
        //字段小数长度
        int fieldDecLength=0;
        //字段原始值
        QString fieldOaiginal="";
        //字段翻译值
        QString fieldValues="";
        if(currentOpenFileType==openFileType::OFDFile){
            fieldDes=ofd.getFieldList().at(tableColCurrent).getFieldDescribe();
            fieldName=ofd.getFieldList().at(tableColCurrent).getFieldName();
            fieldType=ofd.getFieldList().at(tableColCurrent).getFieldType();
            fieldLength=ofd.getFieldList().at(tableColCurrent).getLength();
            fieldDecLength=ofd.getFieldList().at(tableColCurrent).getDecLength();
            fieldOaiginal=Utils::getOriginalValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
            if(fieldType=="N"){
                fieldValues=Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent));
            }
            else{
                fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            fieldDes=fixed.getFieldList().at(tableColCurrent).getFieldDescribe();
            fieldName=fixed.getFieldList().at(tableColCurrent).getFieldName();
            fieldType=fixed.getFieldList().at(tableColCurrent).getFieldType();
            fieldLength=fixed.getFieldList().at(tableColCurrent).getLength();
            fieldDecLength=fixed.getFieldList().at(tableColCurrent).getDecLength();
            QByteArray row;
            if(dataCompressLevel==0){
                row=commonContentQByteArrayList.at(rowRealInContent);
            }
            else{
                row=qUncompress(commonContentQByteArrayList.at(rowRealInContent));
            }
            if(fixed.getFieldList().at(tableColCurrent).getRowBeginIndex()+fieldLength>row.length()){
                statusBar_disPlayMessage("未初始化的字段,无法分析...");
                return;
            }
            fieldOaiginal=Utils::getOriginalValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,tableColCurrent);
            if(fieldType=="N"){
                fieldValues=Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,tableColCurrent));
            }
            else{
                fieldValues=Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,tableColCurrent);
            }
        }
        QList<QStringList> data;
        //开始存储数据
        /////////////////////////////
        QStringList rowfieldDes;
        rowfieldDes.append("字段定义中文名");
        rowfieldDes.append(fieldDes);
        data.append(rowfieldDes);
        /////////////////////////////
        QStringList rowfieldName;
        rowfieldName.append("字段定义英文名");
        rowfieldName.append(fieldName);
        data.append(rowfieldName);
        /////////////////////////////
        QStringList rowfieldType;
        rowfieldType.append("字段定义类型");
        QString type="";
        if(fieldType=="C"){
            type=fieldType+"(字符型)";
        }
        else if(fieldType=="A"){
            type=fieldType+"(数字字符型)";
        }
        else if(fieldType=="N"){
            type=fieldType+"(数值型)";
        }
        else if(fieldType=="TEXT"){
            type=fieldType+"(不定长文本)";
        }
        rowfieldType.append(type);
        data.append(rowfieldType);
        /////////////////////////////
        QStringList rowfieldLength;
        rowfieldLength.append("字段定义总长度");
        rowfieldLength.append(QString::number(fieldLength));
        data.append(rowfieldLength);
        /////////////////////////////
        QStringList rowfieldDecLength;
        rowfieldDecLength.append("字段定义小数长度");
        rowfieldDecLength.append(QString::number(fieldDecLength));
        data.append(rowfieldDecLength);
        /////////////////////////////
        QStringList rowfieldOaiginal;
        rowfieldOaiginal.append("字段原始值");
        rowfieldOaiginal.append(fieldOaiginal);
        data.append(rowfieldOaiginal);
        /////////////////////////////
        QStringList rowfieldValues;
        rowfieldValues.append("字段解析值");
        rowfieldValues.append(fieldValues);
        data.append(rowfieldValues);
        /////////////////////////////
        QStringList rowfieldHexValues;
        if(currentOpenFileType==openFileType::OFDFile){
            rowfieldHexValues.append("逐字符16进制(GB18030编码)");
            rowfieldHexValues.append(StringToHexStringWithEnCode(fieldValues,codecOFD,true));
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            rowfieldHexValues.append(QString("逐字符16进制(%1编码)").arg(fixed.getEcoding()));
            rowfieldHexValues.append(StringToHexStringWithEnCode(fieldValues,QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()),true));
        }
        data.append(rowfieldHexValues);
        QStringList rowfieldCheck;
        rowfieldCheck.append("字段值合法性");
        if(fieldType=="C"){
            rowfieldCheck.append("字段值符合接口约束");
        }
        else if(fieldType=="A"){
            if(fieldOaiginal.trimmed().isEmpty()){
                rowfieldCheck.append("字段值符合接口约束");
            }
            else if(fieldOaiginal.at(0)==' '){
                rowfieldCheck.append("数字字符型字段不建议以空格开头");
            }
            else{
                QString textTrimed=fieldOaiginal.trimmed();
                bool ok=true;
                for(int i=0;i<textTrimed.length();i++){
                    if(((int)textTrimed.at(i).toLatin1())<48||(((int)textTrimed.at(i).toLatin1())>57&&((int)textTrimed.at(i).toLatin1())<65)||(((int)textTrimed.at(i).toLatin1())>90&&((int)textTrimed.at(i).toLatin1())<97)||((int)textTrimed.at(i).toLatin1())>122){
                        ok=false;
                        break;
                    }
                }
                if(ok){
                    rowfieldCheck.append("字段值符合接口约束");
                }
                else{
                    rowfieldCheck.append("数字字符型字段不建议包含0-9,a-z,A-Z外的其他字符");
                }
            }
        }
        else if(fieldType=="N"){
            if(fieldOaiginal.trimmed().isEmpty()){
                rowfieldCheck.append("数值型字段不建议使用空格填充,部分厂商系统不支持,空值建议填充0");
            }
            else if(fieldOaiginal.contains(" ")){
                rowfieldCheck.append("数值型字段不建议使用空格填充,部分厂商系统不支持,长度补位建议填充0");
            }else{
                bool ok;
                QString(fieldOaiginal).toDouble(&ok);
                if(ok){
                    rowfieldCheck.append("字段值符合接口约束");
                }
                else{
                    rowfieldCheck.append("字段内容错误,本字段不应该包含数值外的字符");
                }
            }
        }
        else if(fieldType=="TEXT"){
            rowfieldCheck.append("字段值符合接口约束");
        }
        else{
            rowfieldCheck.append("未知类型字段,无法分析");
        }
        data.append(rowfieldCheck);
        DialogShowTableFieldCheck * dialog = new DialogShowTableFieldCheck(&data,this);
        dialog->setWindowTitle(QString("分析第%1行第%2列数据数据").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1)));
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

void MainWindow::showModifyCell(){
    if(currentOpenFileType==openFileType::OFDFile){
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        int editRow=tableRowCurrent;
        int editCol=tableColCurrent;
        QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
        int fieldLength=ofd.getFieldList().at(editCol).getLength();
        int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
        QString fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,editCol);
        bool modifyFlag=false;
        QString valueNew="";
        DialogModifyCell  dialog(fieldType,fieldLength,fieldDecLength,fieldValues,this);
        dialog.setWindowTitle(QString("编辑第%1行第%2列-%3").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1),ofd.getFieldList().at(tableColCurrent).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        valueNew=dialog.getValueNew();
        if(modifyFlag){
            if(valueNew==fieldValues){statusBar_disPlayMessage("单元格值没有变化,无需保存...");}
            else{
                int updateBegin=ofd.getFieldList().at(tableColCurrent).getRowBeginIndex();
                QByteArray rowByteArray;
                if(dataCompressLevel==0){rowByteArray=commonContentQByteArrayList.at(rowRealInContent);}
                else{rowByteArray=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                bool result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,0,codecOFD,valueNew,rowByteArray);
                if(result){
                    if(dataCompressLevel==0){
                        commonContentQByteArrayList.replace(rowRealInContent,rowByteArray);
                    }
                    else{
                        commonContentQByteArrayList.replace(rowRealInContent,qCompress(rowByteArray,dataCompressLevel));
                    }
                    rowHasloaded.remove(editRow);
                    display_OFDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                    }
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&ofd.getPrimaryKeyFieldList().contains(editCol)){
                        needReCheckPrimary=true;
                        statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");
                    }
                }
                else{statusBar_disPlayMessage("编辑失败,请重试...");}
            }
        }
        else{
            statusBar_disPlayMessage("取消编辑...");
        }
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        QTextCodec *codec=(QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()));
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        int editRow=tableRowCurrent;
        int editCol=tableColCurrent;
        QString fieldType=fixed.getFieldList().at(editCol).getFieldType();
        int fieldLength=fixed.getFieldList().at(editCol).getLength();
        int fieldDecLength=fixed.getFieldList().at(editCol).getDecLength();
        QString fieldValues=Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,editCol);
        bool modifyFlag=false;
        QString valueNew="";
        DialogModifyCell  dialog(fieldType,fieldLength,fieldDecLength,fieldValues,this);
        dialog.setWindowTitle(QString("编辑第%1行第%2列-%3").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1),fixed.getFieldList().at(tableColCurrent).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        valueNew=dialog.getValueNew();
        if(modifyFlag){
            if(valueNew==fieldValues){statusBar_disPlayMessage("单元格值没有变化,无需保存...");}
            else{
                int updateBegin=fixed.getFieldList().at(tableColCurrent).getRowBeginIndex();
                QByteArray rowByteArray;
                if(dataCompressLevel==0){rowByteArray=commonContentQByteArrayList.at(rowRealInContent);}
                else{rowByteArray=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                if(rowByteArray.length()<updateBegin+fieldLength){
                    while (true){
                        rowByteArray.append(' ');
                        if(fixed.getRowLengthHash().contains(rowByteArray.length())&&rowByteArray.length()>=updateBegin+fieldLength){
                            break;
                        }
                    }
                }
                bool result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,fixed.getFieldlengthtype(),codec,valueNew,rowByteArray);
                if(result){
                    if(dataCompressLevel==0){commonContentQByteArrayList.replace(rowRealInContent,rowByteArray);}
                    else{commonContentQByteArrayList.replace(rowRealInContent,qCompress(rowByteArray,dataCompressLevel));}
                    rowHasloaded.remove(editRow);
                    display_FIXEDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                    }
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&fixed.getPrimaryKeyFieldList().contains(editCol)){
                        needReCheckPrimary=true;
                        statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");
                    }
                }
                else{statusBar_disPlayMessage("编辑失败,请重试...");}
            }
        }
        else{statusBar_disPlayMessage("取消编辑...");}
    }
}

void MainWindow::showModifyCellBatch(){
    if(currentOpenFileType==openFileType::OFDFile){
        int editCol=tableColCurrent;
        QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
        int fieldLength=ofd.getFieldList().at(editCol).getLength();
        int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
        bool modifyFlag=false;
        DialogModifyCell dialog(fieldType,fieldLength,fieldDecLength,"",this);
        dialog.setWindowTitle(QString("批量编辑第%1列多个单元格-%2").arg(QString::number(editCol+1),ofd.getFieldList().at(editCol).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        QString valueNew=dialog.getValueNew();
        if(modifyFlag){
            QByteArray valueByteArray=codecOFD->fromUnicode(valueNew);
            bool flag=Utils::updateOFDOrFixedFieldValue(fieldType,fieldLength,fieldDecLength,0,codecOFD,valueNew,valueByteArray);
            if(flag){
                int rowRealInContent=0;
                QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
                if(itemsRange.count()>=1){
                    int updateBegin=ofd.getFieldList().at(editCol).getRowBeginIndex();
                    int updateEnd=updateBegin+fieldLength;
                    int updatedRow=0;
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            rowRealInContent=(currentPage-1)*pageRowSize+editRow;
                            QByteArray valueNewArrayRow;
                            if(dataCompressLevel==0){valueNewArrayRow=commonContentQByteArrayList.at(rowRealInContent);}
                            else{valueNewArrayRow=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                            int index=0;
                            for(int i=updateBegin;i<updateEnd&&i-updateBegin<valueByteArray.length();i++){
                                valueNewArrayRow[i]=valueByteArray.at(index);
                                index++;
                            }
                            if(dataCompressLevel==0){commonContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);}
                            else{commonContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));}
                            if(compareData.contains(editRow+1)){
                                compareData.remove(editRow+1);
                                compareData.insert(editRow+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,editRow));
                            }
                            updatedRow++;
                            if((updatedRow%1000==0)){
                                statusBar_disPlayMessage(QString("批量更新中,请勿进行其他操作,已更新%1行").arg(QString::number(updatedRow)));
                                qApp->processEvents();
                                if(abortExit){
                                    QApplication::restoreOverrideCursor();
                                    qDebug()<<"强制立即终止任务";
                                    return;
                                }
                            }
                        }
                    }
                    clear_Table_Contents();
                    rowHasloaded.clear();
                    display_OFDTable(true);
                    for(int rangeIndex2=0;rangeIndex2<itemsRange.count();rangeIndex2++){ptr_table->setRangeSelected(itemsRange.at(rangeIndex2),true);}
                    QApplication::restoreOverrideCursor();
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&ofd.getPrimaryKeyFieldList().contains(editCol)){needReCheckPrimary=true;statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");}
                }
                ptr_table->resizeColumnsToContents();
            }
            else{statusBar_disPlayMessage("编辑失败,请重试...");}
        }
        else{statusBar_disPlayMessage("取消编辑...");}
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        QTextCodec *codec=(QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()));
        int editCol=tableColCurrent;
        QString fieldType=fixed.getFieldList().at(editCol).getFieldType();
        int fieldLength=fixed.getFieldList().at(editCol).getLength();
        int fieldDecLength=fixed.getFieldList().at(editCol).getDecLength();
        bool modifyFlag=false;
        DialogModifyCell dialog(fieldType,fieldLength,fieldDecLength,"",this);
        dialog.setWindowTitle(QString("批量编辑第%1列多个单元格-%2").arg(QString::number(editCol+1),fixed.getFieldList().at(editCol).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        QString valueNew=dialog.getValueNew();
        if(modifyFlag){
            QByteArray valueByteArray=codec->fromUnicode(valueNew);
            bool flag=Utils::updateOFDOrFixedFieldValue(fieldType,fieldLength,fieldDecLength,fixed.getFieldlengthtype(),codec,valueNew,valueByteArray);
            if(flag){
                int rowRealInContent=0;
                QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
                if(itemsRange.count()>=1){
                    int updateBegin=fixed.getFieldList().at(editCol).getRowBeginIndex();
                    int updateEnd=updateBegin+fieldLength;
                    int updatedRow=0;
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            rowRealInContent=(currentPage-1)*pageRowSize+editRow;
                            QByteArray valueNewArrayRow;
                            if(dataCompressLevel==0){valueNewArrayRow=commonContentQByteArrayList.at(rowRealInContent);}
                            else{valueNewArrayRow=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                            int index=0;
                            if(valueNewArrayRow.length()<updateEnd){
                                while (true){
                                    valueNewArrayRow.append(' ');
                                    if(fixed.getRowLengthHash().contains(valueNewArrayRow.length())&&valueNewArrayRow.length()>=updateEnd){
                                        break;
                                    }
                                }
                            }
                            for(int i=updateBegin;i<updateEnd&&i-updateBegin<valueByteArray.length();i++){
                                valueNewArrayRow[i]=valueByteArray.at(index);
                                index++;
                            }
                            if(dataCompressLevel==0){commonContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);}
                            else{commonContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));}
                            if(compareData.contains(editRow+1)){
                                compareData.remove(editRow+1);
                                compareData.insert(editRow+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,editRow));
                            }
                            updatedRow++;
                            if((updatedRow%1000==0)){
                                statusBar_disPlayMessage(QString("批量更新中,请勿进行其他操作,已更新%1行").arg(QString::number(updatedRow)));
                                qApp->processEvents();
                                if(abortExit){
                                    QApplication::restoreOverrideCursor();
                                    qDebug()<<"强制立即终止任务";
                                    return;
                                }
                            }
                        }
                    }
                    clear_Table_Contents();
                    rowHasloaded.clear();
                    display_FIXEDTable(true);
                    for(int rangeIndex2=0;rangeIndex2<itemsRange.count();rangeIndex2++){ptr_table->setRangeSelected(itemsRange.at(rangeIndex2),true);}
                    QApplication::restoreOverrideCursor();
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&fixed.getPrimaryKeyFieldList().contains(editCol)){needReCheckPrimary=true;statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");}
                }
                ptr_table->resizeColumnsToContents();
            }
            else{statusBar_disPlayMessage("编辑失败,请重试...");}
        }
        else{statusBar_disPlayMessage("取消编辑...");}
    }
}

void MainWindow::showMoaifyRow(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    if(itemsRange.count()>0){
        int editRow=itemsRange.at(0).topRow();
        int rowRealInContent=(currentPage-1)*pageRowSize+editRow;
        QStringList rowDataOld;
        QList<fieldType> fieldTypeList;
        QTextCodec *codec=QTextCodec::codecForName("GB18030");;
        if(currentOpenFileType==openFileType::OFDFile){
            codec=codecOFD;
            rowDataOld=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            for(int i=0;i<ofd.getFieldList().count();i++){
                fieldType item;
                item.fieldType=ofd.getFieldList().at(i).getFieldType();
                item.fieldLength=ofd.getFieldList().at(i).getLength();
                item.fieldDecLength=ofd.getFieldList().at(i).getDecLength();
                item.fieldName=ofd.getFieldList().at(i).getFieldName();
                item.fieldDescribe=ofd.getFieldList().at(i).getFieldDescribe();
                fieldTypeList.append(item);
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            rowDataOld=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            for(int i=0;i<fixed.getFieldList().count();i++){
                fieldType item;
                item.fieldType=fixed.getFieldList().at(i).getFieldType();
                item.fieldLength=fixed.getFieldList().at(i).getLength();
                item.fieldDecLength=fixed.getFieldList().at(i).getDecLength();
                item.fieldName=fixed.getFieldList().at(i).getFieldName();
                item.fieldDescribe=fixed.getFieldList().at(i).getFieldDescribe();
                fieldTypeList.append(item);
                if(rowDataOld.count()-1<i){
                    rowDataOld.append(QString(""));
                }
            }
        }
        DialogModifyRow * dialog = new DialogModifyRow(fieldTypeList,codec,rowDataOld,this);
        dialog->setWindowTitle(QString("编辑第%1行数据").arg(QString::number(rowRealInContent+1)));
        dialog->setModal(true);
        dialog->exec();
        ///////////////////////////////开始进行数据编辑更新//////////////////////////////////
        if(dialog->getChange()&&dialog->getSaveFlag()){
            QStringList rowDataNew=dialog->getRowDataNew();
            delete dialog;
            dialog=nullptr;
            if(rowDataNew.count()>0){
                QByteArray valueNewArrayRow;
                if(dataCompressLevel==0){
                    valueNewArrayRow=commonContentQByteArrayList.at(rowRealInContent);
                }
                else{
                    valueNewArrayRow=qUncompress(commonContentQByteArrayList.at(rowRealInContent));
                }
                for(int editCol=0;editCol<rowDataNew.count()&&editCol<ptr_table->columnCount();editCol++){
                    QString fieldValues=rowDataOld.count()>editCol?rowDataOld.at(editCol):"";
                    QString valueNew=rowDataNew.count()>editCol?rowDataNew.at(editCol):"";
                    bool result=false;
                    if(fieldValues!=valueNew){
                        if(currentOpenFileType==openFileType::OFDFile){
                            QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
                            int fieldLength=ofd.getFieldList().at(editCol).getLength();
                            int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
                            int updateBegin=ofd.getFieldList().at(editCol).getRowBeginIndex();
                            result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,0,codec,valueNew,valueNewArrayRow);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            QString fieldType=fixed.getFieldList().at(editCol).getFieldType();
                            int fieldLength=fixed.getFieldList().at(editCol).getLength();
                            int fieldDecLength=fixed.getFieldList().at(editCol).getDecLength();
                            int updateBegin=fixed.getFieldList().at(editCol).getRowBeginIndex();
                            if(valueNewArrayRow.length()-1<updateBegin){
                                while (true){
                                    valueNewArrayRow.append(' ');
                                    if(fixed.getRowLengthHash().contains(valueNewArrayRow.length())&&valueNewArrayRow.length()>=updateBegin+fieldLength){
                                        break;
                                    }
                                }
                            }
                            result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,0,codec,valueNew,valueNewArrayRow);
                        }
                        if(!result){
                            statusBar_disPlayMessage("编辑失败,请重试...");
                            return;
                        }
                    }
                }
                if(dataCompressLevel==0){
                    commonContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);
                }
                else{
                    commonContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                }
                rowHasloaded.remove(editRow);
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                    }
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                    }
                }
                statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                fileChanged=true;
                if(dialogDialogShowPrimaryCheck!=nullptr){
                    needReCheckPrimary=true;
                    statusBar_disPlayMessage("检测到数据行更新,请重新进行主键冲突检查...");
                }
            }
        }
        else{
            if(!dialog->getSaveFlag()){
                statusBar_disPlayMessage("放弃编辑...");
            }
            if(dialog->getSaveFlag()&&!dialog->getChange()){
                statusBar_disPlayMessage("数据未修改,无需更新记录...");
            }
            delete dialog;
            dialog=nullptr;
        }
    }
}

/**
 * @brief MainWindow::on_pushButtonPreSearch_clicked 向上搜索
 */
void MainWindow::on_pushButtonPreSearch_clicked()
{
    if(isUpdateData){statusBar_disPlayMessage("正在加载数据,请稍后再使用搜索功能...");return;}
    if(dataBlocked){statusBar_disPlayMessage(dataBlockedMessage);return;}
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    //数据类型加入点
    if((currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile)&&commonContentQByteArrayList.count()<1){
        statusBar_disPlayMessage("打开的文件没有数据记录...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==0){
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的文件没有数据记录...");
                return;
            }
        }
        else{
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的文件在当前模式下没有数据记录...");
                return;
            }
        }
    }
    //向上搜索
    if(ui->lineTextText->text().isEmpty()){
        statusBar_disPlayMessage("请填写你要搜索的内容...");
        return;
    }
    if(tableRowCurrent==0&&tableColCurrent==0){
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        statusBar_disPlayMessage("已经搜索到第一个单元格了,请向下搜索...");
        return;
    }
    dataBlocked=true;
    int oldCurrentPage=currentPage;
    //开始搜索
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->pushButtonPreSearch->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    QString searchText=ui->lineTextText->text();
    //判断是否是当前搜索行,如果是则从焦点单元格前一个单元格搜索,否则从行记录的最后一个单元格搜索
    //搜索当前行时,不能从最后一列开始,要从焦点单元格前一个开始
    int beginCol=tableColCurrent-1;
    int beginRow=tableRowCurrent;
    int colCount=ptr_table->columnCount();
    //分页模式下的跨页搜索
    int rowRealInContent=0;
    while(currentPage>0){
        for(int row=beginRow;row>=0;row--){
            rowRealInContent=(currentPage-1)*pageRowSize+row;
            QStringList rowdata;
            if(currentOpenFileType==openFileType::OFDFile){
                rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            //未取到本行数据,直接下一行
            if(rowdata.count()==0){
                beginCol=colCount-1;
                continue;
            }
            for(int col=rowdata.count()-1>=beginCol?beginCol:rowdata.count()-1;col>=0;col--){
                if(rowdata.at(col).contains(searchText,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(row,col);
                    ptr_table->setFocus();
                    ui->pushButtonPreSearch->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    dataBlocked=false;
                    reCalculateTableBeginAndEnd();
                    on_tableWidget_itemSelectionChanged();
                    return;
                }
            }
            beginCol=colCount-1;
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                qApp->processEvents();
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        //如果不是第一页
        //则跳转到上一行继续搜索
        if(currentPage>1){
            currentPage--;
            //跳到上一页面
            pageJump(currentPage,-1);
            //重新设置从右下角开始继续搜索
            beginCol=ptr_table->columnCount()-1;
            beginRow=ptr_table->rowCount()-1;
        }
        else{
            dataBlocked=false;
            ui->pushButtonPreSearch->setEnabled(true);
            if(oldCurrentPage==currentPage){
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            else{
                ptr_table->scrollToTop();
                tableRowCurrent=0;
                tableColCurrent=0;
                ptr_table->setCurrentCell(0,0);
            }
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            statusBar_disPlayMessage("再往上没有你要搜索的内容了...");
            //搜索到第一页最顶部还没搜到内容，break
            break;
        }
    }
}

/**
 * @brief MainWindow::on_pushButtonNextSearch_clicked 向下搜索
 */
void MainWindow::on_pushButtonNextSearch_clicked()
{
    if(isUpdateData){statusBar_disPlayMessage("正在加载数据,请稍后再使用搜索功能...");return;}
    if(dataBlocked){statusBar_disPlayMessage(dataBlockedMessage);return;}
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    int colCount=ptr_table->columnCount();
    int tableColCount=ptr_table->columnCount();
    //数据类型加入点
    if((currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile)&&commonContentQByteArrayList.count()<1){
        statusBar_disPlayMessage("打开的文件没有数据记录...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfRowMap.count()<1){
            statusBar_disPlayMessage("打开的文件没有数据记录...");
            return;
        }
    }
    //向下搜索
    if(ui->lineTextText->text().isEmpty()){
        statusBar_disPlayMessage("请填写你要搜索的内容...");
        return;
    }
    if(tableRowCurrent==ptr_table->rowCount()-1&&tableColCurrent==colCount-1){
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        statusBar_disPlayMessage("已经搜索到最后一个单元格了,请向上搜索...");
        return;
    }
    dataBlocked=true;
    int oldCurrentPage=currentPage;
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->pushButtonNextSearch->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    QString searchText=ui->lineTextText->text();
    int currentPageRowCount=ptr_table->rowCount();
    int beginCol=tableColCurrent+1;
    int beginRow=tableRowCurrent;
    int rowRealInContent=0;
    while(currentPage<=pageCount){
        for(int row=beginRow;row<currentPageRowCount;row++){
            rowRealInContent=(currentPage-1)*pageRowSize+row;
            QStringList rowdata;
            if(currentOpenFileType==openFileType::OFDFile){
                rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            if(rowdata.count()==tableColCount){
                colCount=tableColCount;
            }
            else if(rowdata.count()==0){
                beginCol=0;
                continue;
            }
            else if(rowdata.count()<colCount){
                colCount=rowdata.count();
            }
            for(int col=beginCol;col<colCount;col++){
                if(rowdata.at(col).contains(searchText,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(row,col);
                    ptr_table->setFocus();
                    ui->pushButtonNextSearch->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    dataBlocked=false;
                    reCalculateTableBeginAndEnd();
                    on_tableWidget_itemSelectionChanged();
                    return;
                }
            }
            beginCol=0;
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                qApp->processEvents();
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        //不是最后一页
        //跳转到下一页
        if(currentPage<pageCount){
            currentPage++;
            //跳到上一页面
            pageJump(currentPage,-2);
            //重新设置从左上角开始继续搜索
            beginCol=0;
            beginRow=0;
            currentPageRowCount=ptr_table->rowCount();
        }
        //最后一页
        else{
            dataBlocked=false;
            ui->pushButtonNextSearch->setEnabled(true);
            if(oldCurrentPage==currentPage){
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            else{
                ptr_table->scrollToBottom();
                tableRowCurrent=currentPageRowCount-1;
                tableColCurrent=colCount-1;
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            statusBar_disPlayMessage("再往下没有你要搜索的内容了...");
            break;
        }
    }
}


void MainWindow::showMessage_customContextMenuRequested(const QPoint &pos)
{
    UNUSED(pos);
    showMessagePopMenu->clear();
    showMessagePopMenu->addAction(action_ShowCopy);
    if(firstNotNumberLine>0){
        showMessagePopMenu->addAction(action_GoToFirstNotNumberLine);
    }
    showMessagePopMenu->exec(QCursor::pos());
}


/**
 * @brief MainWindow::on_tableWidget_customContextMenuRequested 表格右键菜单
 * @param pos
 */
void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    tablePopMenu->clear();
    //判断当前鼠标位置是不是在表格单元格位置内
    //特殊情况当前打开的文件是OFD/字节定长文件，总行数是0的空文件允许插入内容--仅仅允许插入数据
    if((currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0))&&ptr_table->rowCount()<1&&ptr_table->rowAt(pos.y())<0){
        tablePopMenu->addAction(action_addCopyedData2End);
        tablePopMenu->addAction(action_addNewLineData2End);
        tablePopMenu->exec(QCursor::pos());
        return;
    }
    //空表
    if(ptr_table->rowCount()<1){
        return;
    }
    //如果鼠标点击的是表格得空白位置取到得行y是-1
    if(ptr_table->rowAt(pos.y()) <0){
        return;
    }
    //当打开的是OFD文件时,菜单里添加行预览和显示行详情和编辑的菜单
    if(currentOpenFileType==openFileType::OFDFile){
        /////////////////////动态右键菜单///////////////////////////////////////////
        //分模式启用不同的菜单
        //判断启用批量编辑还是使用单个单元格编辑器
        //不能使用selectedItems作为选择范围,因为懒加载机制下部分没有数据的单元格并没有填充item
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int rangeCount=itemsRange.count();
        int selectedRow=0;
        bool allSelectIsOneColumn=true;
        if(rangeCount>=1){
            selectedRow=itemsRange.at(0).bottomRow()-itemsRange.at(0).topRow()+1;
            //第一个选择范围就跨列
            if(itemsRange.at(0).leftColumn()!=itemsRange.at(0).rightColumn()){
                allSelectIsOneColumn=false;
            }
            //如果存在第二个选择区间
            //循环从第第二个遍历选择的范围区间
            for(int i=1;i<rangeCount;i++){
                selectedRow+=(itemsRange.at(i).bottomRow()-itemsRange.at(i).topRow()+1);
                //选择范围跨列,与上一个范围区间列不等,不允许列模式编辑
                if((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())||(itemsRange.at(i).leftColumn()!=itemsRange.at(i-1).leftColumn())){
                    allSelectIsOneColumn=false;
                    break;
                }
            }
            //单行
            if(selectedRow<2){
                //只选择一行的情况下分为选择了多列和一列
                //单行单列
                if(allSelectIsOneColumn){
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addAction(action_Magnify);
                    tablePopMenu->addAction(action_ShowCharacter);
                    tablePopMenu->addAction(action_ShowOFDAnalysis);
                    action_ShareUseQrCode->setText("使用二维码分享此单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_ModifyCell);
                    tablePopMenu->addAction(action_ModifyRow);
                    tablePopMenu->addSeparator();
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
                //单行多列
                else{
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_ModifyRow);
                    tablePopMenu->addSeparator();
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
                //单行删除和复制
                tablePopMenu->addSeparator();
                tablePopMenu->addAction(action_DeleteRowData);
                action_DeleteRowData->setText("删除光标所在行的整行数据记录");
                tablePopMenu->addAction(action_addNewLineData2End);
                tablePopMenu->addSeparator();
                tablePopMenu->addAction(action_CopyRowData);
                action_CopyRowData->setText("复制光标所在整行数据记录(专用于合并插入OFD/类OFD数据)");
                //数据插入菜单
                tablePopMenu->addAction(action_addCopyedData2PreviousRow);
                tablePopMenu->addAction(action_addCopyedData2NextRow);
                tablePopMenu->addAction(action_addCopyedData2End);

            }
            //多行
            else {
                //多行单列
                if(allSelectIsOneColumn){
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误--跨选取复制
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误--跨选取复制
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
                //多行情况下均展示删除选择的行
                tablePopMenu->addSeparator();
                tablePopMenu->addAction(action_DeleteRowData);
                action_DeleteRowData->setText("删除选择范围内的整行数据记录");
                //新增行
                tablePopMenu->addAction(action_addNewLineData2End);
                tablePopMenu->addSeparator();
                tablePopMenu->addAction(action_CopyRowData);
                action_CopyRowData->setText("复制选择范围内的整行数据记录(专用于合并插入OFD/类OFD数据)");
                //多行时展示插入到文件尾部
                tablePopMenu->addAction(action_addCopyedData2End);
            }
        }
    }
    //csv类型的文件
    if(currentOpenFileType==openFileType::CSVFile){
        /////////////////////动态右键菜单///////////////////////////////////////////
        //分模式启用不同的菜单
        //判断启用批量编辑还是使用单个单元格编辑器
        //不能使用selectedItems作为选择范围,因为懒加载机制下部分没有数据的单元格并没有填充item
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int rangeCount=itemsRange.count();
        int selectedRow=0;
        bool allSelectIsOneColumn=true;
        if(rangeCount>=1){
            selectedRow=itemsRange.at(0).bottomRow()-itemsRange.at(0).topRow()+1;
            //第一个选择范围就跨列
            if(itemsRange.at(0).leftColumn()!=itemsRange.at(0).rightColumn()){
                allSelectIsOneColumn=false;
            }
            //如果存在第二个选择区间
            //循环从第第二个遍历选择的范围区间
            for(int i=1;i<rangeCount;i++){
                selectedRow+=(itemsRange.at(i).bottomRow()-itemsRange.at(i).topRow()+1);
                //选择范围跨列,与上一个范围区间列不等,不允许列模式编辑
                if((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())||(itemsRange.at(i).leftColumn()!=itemsRange.at(i-1).leftColumn())){
                    allSelectIsOneColumn=false;
                    break;
                }
            }
            //单行
            if(selectedRow<2){
                //只选择一行的情况下分为选择了多列和一列
                //单行单列
                if(allSelectIsOneColumn){
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享此单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    tablePopMenu->addAction(action_Magnify);
                    tablePopMenu->addAction(action_ShowCharacter);
                    tablePopMenu->addAction(action_CsvForceNumber);
                    //csv文件暂不支持编辑
                    //tablePopMenu->addAction(action_ModifyCell);
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
                //单行多列
                else{
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
            }
            //多行
            else {
                //多行单列
                if(allSelectIsOneColumn){
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addAction(action_CsvForceNumber);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addAction(action_CsvForceNumber);
                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        /////////////////////动态右键菜单///////////////////////////////////////////
        //分模式启用不同的菜单
        //判断启用批量编辑还是使用单个单元格编辑器
        //不能使用selectedItems作为选择范围,因为懒加载机制下部分没有数据的单元格并没有填充item
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int rangeCount=itemsRange.count();
        int selectedRow=0;
        bool allSelectIsOneColumn=true;
        if(rangeCount>=1){
            selectedRow=itemsRange.at(0).bottomRow()-itemsRange.at(0).topRow()+1;
            //第一个选择范围就跨列
            if(itemsRange.at(0).leftColumn()!=itemsRange.at(0).rightColumn()){
                allSelectIsOneColumn=false;
            }
            //如果存在第二个选择区间
            //循环从第第二个遍历选择的范围区间
            for(int i=1;i<rangeCount;i++){
                selectedRow+=(itemsRange.at(i).bottomRow()-itemsRange.at(i).topRow()+1);
                //选择范围跨列,与上一个范围区间列不等,不允许列模式编辑
                if((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())||(itemsRange.at(i).leftColumn()!=itemsRange.at(i-1).leftColumn())){
                    allSelectIsOneColumn=false;
                    break;
                }
            }
            //单行
            if(selectedRow<2){
                //只选择一行的情况下分为选择了多列和一列
                //单行单列
                if(allSelectIsOneColumn){
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addAction(action_Magnify);
                    tablePopMenu->addAction(action_ShowCharacter);
                    if(fixed.getFieldlengthtype()==0){
                        tablePopMenu->addAction(action_ShowOFDAnalysis);
                    }
                    action_ShareUseQrCode->setText("使用二维码分享此单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    if(fixed.getFieldlengthtype()==0){
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_ModifyCell);
                        tablePopMenu->addAction(action_ModifyRow);
                    }
                    tablePopMenu->addSeparator();
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
                //单行多列
                else{
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    if(fixed.getFieldlengthtype()==0){
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_ModifyRow);
                    }
                    tablePopMenu->addSeparator();
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
                if(fixed.getFieldlengthtype()==0){
                    //单行删除和复制
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_DeleteRowData);
                    action_DeleteRowData->setText("删除光标所在行的整行数据记录");
                    tablePopMenu->addAction(action_addNewLineData2End);
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_CopyRowData);
                    action_CopyRowData->setText("复制光标所在整行数据记录(专用于合并插入字段定长数据)");
                    //数据插入菜单
                    tablePopMenu->addAction(action_addCopyedData2PreviousRow);
                    tablePopMenu->addAction(action_addCopyedData2NextRow);
                    tablePopMenu->addAction(action_addCopyedData2End);
                }
            }
            //多行
            else {
                //多行单列
                if(allSelectIsOneColumn){
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        if(fixed.getFieldlengthtype()==0){
                            tablePopMenu->addSeparator();
                            tablePopMenu->addAction(action_ModifyCellBatch);
                        }
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        if(fixed.getFieldlengthtype()==0){
                            tablePopMenu->addSeparator();
                            tablePopMenu->addAction(action_ModifyCellBatch);
                        }
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
                if(fixed.getFieldlengthtype()==0){
                    //多行情况下均展示删除选择的行
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_DeleteRowData);
                    action_DeleteRowData->setText("删除选择范围内的整行数据记录");
                    //新增行
                    tablePopMenu->addAction(action_addNewLineData2End);
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_CopyRowData);
                    action_CopyRowData->setText("复制选择范围内的整行数据记录(专用于合并插入字段定长数据)");
                    //多行时展示插入到文件尾部
                    tablePopMenu->addAction(action_addCopyedData2End);
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        /////////////////////动态右键菜单///////////////////////////////////////////
        //分模式启用不同的菜单
        //判断启用批量编辑还是使用单个单元格编辑器
        //不能使用selectedItems作为选择范围,因为懒加载机制下部分没有数据的单元格并没有填充item
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int rangeCount=itemsRange.count();
        int selectedRow=0;
        bool allSelectIsOneColumn=true;
        if(rangeCount>=1){
            selectedRow=itemsRange.at(0).bottomRow()-itemsRange.at(0).topRow()+1;
            //第一个选择范围就跨列
            if(itemsRange.at(0).leftColumn()!=itemsRange.at(0).rightColumn()){
                allSelectIsOneColumn=false;
            }
            //如果存在第二个选择区间
            //循环从第第二个遍历选择的范围区间
            for(int i=1;i<rangeCount;i++){
                selectedRow+=(itemsRange.at(i).bottomRow()-itemsRange.at(i).topRow()+1);
                //选择范围跨列,与上一个范围区间列不等,不允许列模式编辑
                if((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())||(itemsRange.at(i).leftColumn()!=itemsRange.at(i-1).leftColumn())){
                    allSelectIsOneColumn=false;
                    break;
                }
            }
            //单行
            if(selectedRow<2){
                //只选择一行的情况下分为选择了多列和一列
                //单行单列
                if(allSelectIsOneColumn){
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享此单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    tablePopMenu->addAction(action_Magnify);
                    tablePopMenu->addAction(action_ShowCharacter);
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
                //单行多列
                else{
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    //比对器
                    tablePopMenu->addAction(action_EditCompareData);
                    //行比对齐文字描述的替换
                    if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
                        action_EditCompareData->setText("将此行数据加入比对器");
                    }
                    else{
                        action_EditCompareData->setText("将此行数据从比对器移除");
                    }
                }
            }
            //多行
            else {
                //多行单列
                if(allSelectIsOneColumn){
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
            }
        }
    }
    //索引模式也支持复制
    //为打开文件或者打开失败时，有可能表格显示了失败原因，开启复制功能
    if(currentOpenFileType==openFileType::NotFileOrErr||currentOpenFileType==openFileType::OFDIndex){
        tablePopMenu->addAction(action_ShowCopyColum);
    }
    //显示菜单在鼠标点击的位置
    tablePopMenu->exec(QCursor::pos());
}

/**
 * @brief MainWindow::on_pushButtonColumnJump_clicked 列搜索
 */
void MainWindow::on_pushButtonColumnJump_clicked()
{
    //向左搜索
    columnJump(0);
}

/**
 * @brief MainWindow::on_actionsOpenCompare_triggered 数据比对器
 */
void MainWindow::on_actionsOpenCompare_triggered()
{
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持使用比对器...");
    }
    //数据类型插入点
    else if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
        if(compareData.count()<1){
            statusBar_disPlayMessage("请将需要对比的行加入比对器...");
        }
        else if(compareData.count()==1){
            statusBar_disPlayMessage("只有一行数据,你让臣妾怎么比对...");
        }
        else{
            //打开比对器
            //设置表格列标题
            QStringList title;
            QStringList fieldType;
            title.append("数据行号");
            //OFD
            if(currentOpenFileType==openFileType::OFDFile){
                for(int i=0;i<ofd.getFieldList().count();i++){
                    //仅获取列的中文备注当作列标题
                    title.append(ofd.getFieldList().at(i).getFieldDescribe());
                    fieldType.append(ofd.getFieldList().at(i).getFieldType());
                }
            }
            //CSV
            else if(currentOpenFileType==openFileType::CSVFile){
                for(int i=0;i<csv.getFieldList().count();i++){
                    //仅获取列的中文备注当作列标题
                    title.append(csv.getFieldList().at(i).getFieldDescribe());
                    if(CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                        fieldType.append("N");
                    }
                    else{
                        fieldType.append("S");
                    }
                }
            }
            //FIXED
            else if(currentOpenFileType==openFileType::FIXEDFile){
                for(int i=0;i<fixed.getFieldList().count();i++){
                    //仅获取列的中文备注当作列标题
                    title.append(fixed.getFieldList().at(i).getFieldDescribe());
                    fieldType.append(fixed.getFieldList().at(i).getFieldType());
                }
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                for(int i=0;i<dbf.getFieldList().count();i++){
                    title.append(dbf.getFieldList().at(i).getFieldName()+(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?"":"-"+dbf.getFieldList().at(i).getFieldDescribe()));
                    if(dbfColIsNumber(i)){
                        fieldType.append("N");
                    }
                    else{
                        fieldType.append("S");
                    }
                }
            }
            //打开窗口
            if(title.count()>0){
                DialogShowTableCompareView * dialog = new DialogShowTableCompareView(title,fieldType,&compareData,this);
                dialog->setWindowTitle(QString("行比对器视图"));
                dialog->setModal(false);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                dialog->show();
                dialog->raise();
                dialog->activateWindow();
            }
        }
    }
}

/**
 * @brief MainWindow::on_actionClearCompare_triggered 清除比对器内容
 */
void MainWindow::on_actionClearCompare_triggered()
{
    compareData.clear();
    statusBar_disPlayMessage("比对器内容清除完毕...");
}

/**
 * @brief MainWindow::on_pushButtonExport_clicked 数据导出功能函数
 */
void MainWindow::on_pushButtonExport_clicked()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用数据导出功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    //数据类型判断
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引数据不支持导出");
        return;
    }
    //预先数据判断--0行也支持导出20220730
    if(ptr_table->rowCount()==0){
        if(currentOpenFileType!=openFileType::OFDFile&&currentOpenFileType!=openFileType::CSVFile&&currentOpenFileType!=openFileType::FIXEDFile&&currentOpenFileType!=openFileType::DBFFile){
            return;
        }
    }
    /////////////////导出方案选择///////////////////////////
    bool moreThan=false;
    bool useUTF8=false;
    //超出限制无法导出整个excel
    if(commonContentQByteArrayList.count()>maxExcelRow||dbfRowMap.count()>maxExcelRow){
        moreThan=true;
    }
    //选择导出模式
    DialogChooseExportType dialog(currentOpenFileType,pageCount,currentPage,moreThan,this);
    dialog.setModal(true);
    dialog.exec();
    //导出当前页或者导出整个文件
    int exportType=dialog.getExportType();
    //强制使用UTF-8导出文件-仅适用于csvc和html
    if(dialog.getUtf8()==1){
        useUTF8=true;
    }
    //导出的文件类型
    int exportFileType=dialog.getExportFileType();
    if(exportType==0){
        statusBar_disPlayMessage("没有选择有效的导出方案,导出取消...");
        return;
    }
    //数据类型插入点
    else if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
        //标记阻断事件---正在导出数据
        dataBlocked=true;
        isExportFile=true;
        dataBlockedMessage="正在进行数据导出,请稍后再操作...";
        //获取文件路径时使用系统编码，否则获取路径可能会乱码
        //获取打开的文件的路径
        QString openpath="./";
        if(!currentOpenFilePath.isEmpty()){
            if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
                openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
            }
        }
        //获取打开的文件的名字,去除最后的格式
        QString filename="";
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            QString filenameall=currentOpenFilePath.right(currentOpenFilePath.length()-currentOpenFilePath.lastIndexOf("/"));
            int index=filenameall.lastIndexOf(".");
            if(index==-1){
                filename=filenameall;
            }else{
                filename=filenameall.left(index);
            }
        }
        else{
            int index=currentOpenFilePath.lastIndexOf(".");
            if(index==-1){
                filename=currentOpenFilePath;
            }else{
                filename=currentOpenFilePath.left(index);
            }
        }
        //默认0为导出所有数据
        int exportPage=0;
        //如果当前是OFD文件且要导出csv
        if((exportFileType==12||exportFileType==13||exportFileType==14)&&useUTF8&&currentOpenFileType==openFileType::OFDFile){
            filename=filename.append("_UTF-8");
        }
        //如果是按页导出的
        if(exportType==2){
            exportPage=currentPage;
            filename=filename.append("_Page").append(QString::number(currentPage));
        }
        //文件过滤器,用于追踪选择的保存文件类别
        QString selectedFilter=Q_NULLPTR;
        //弹出保存框
        //数据类型插入点，csv文件不支持导出csv文件
        QString fileTypeSelect="";
        if(exportFileType==10){
            fileTypeSelect="Excel文件(*.xlsx)";
        }
        if(exportFileType==11){
            fileTypeSelect="Html文件(*.html)";
        }
        if(exportFileType==12){
            fileTypeSelect="Csv文件-竖线分割(*.csv)";
        }
        if(exportFileType==13){
            fileTypeSelect="Csv文件-制表符分割(*.csv)";
        }
        if(exportFileType==14){
            fileTypeSelect="Csv文件-逗号分割(*.csv)";
        }
        //弹窗选择文件保存路径
        QString fileNameSave = QFileDialog::getSaveFileName(this,("选择数据导出保存路径"),openpath+filename,fileTypeSelect,&selectedFilter);
        if(!fileNameSave.isEmpty()){
            //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
            if(selectedFilter=="Excel文件(*.xlsx)"&&(!fileNameSave.endsWith(".xlsx"))){
                fileNameSave.append(".xlsx");
            }
            else if(selectedFilter=="Html文件(*.html)"&&(!fileNameSave.endsWith(".html"))){
                fileNameSave.append(".html");
            }
            else if(selectedFilter=="Csv文件-竖线分割(*.csv)"&&(!fileNameSave.endsWith(".csv"))){
                fileNameSave.append(".csv");
            }
            else if(selectedFilter=="Csv文件-制表符分割(*.csv)"&&(!fileNameSave.endsWith(".csv"))){
                fileNameSave.append(".csv");
            }
            else if(selectedFilter=="Csv文件-逗号分割(*.csv)"&&(!fileNameSave.endsWith(".csv"))){
                fileNameSave.append(".csv");
            }
            //覆盖导出先删除原来的文件
            if(Utils::isFileExist(fileNameSave)){
                QFile file(fileNameSave);
                bool r=file.remove();
                if(!r){
                    statusBar_disPlayMessage("覆盖文件失败,目标文件可能正在被占用...");
                    dataBlocked=false;
                    isExportFile=false;
                    return;
                }
            }
            //根据文件类型来判断
            if(fileNameSave.endsWith("xlsx",Qt::CaseSensitive)){
                //这是单纯的导出excel文件地方，不打开
                openXlsx=false;
                save2Xlsx(fileNameSave,exportPage);
                //xlsx的保存，不在这里取消阻断，交后台存储线程
            }
            else if(fileNameSave.endsWith("csv",Qt::CaseSensitive)){
                save2Csv(fileNameSave,exportPage,exportFileType,useUTF8);
                dataBlocked=false;
                isExportFile=false;
            }
            else if(fileNameSave.endsWith("html",Qt::CaseSensitive)){
                save2Html(fileNameSave,exportPage,useUTF8);
                dataBlocked=false;
                isExportFile=false;
            }else{
                statusBar_disPlayMessage("暂不支持的导出类型");
            }
        }
        else{
            dataBlocked=false;
            isExportFile=false;
            statusBar_disPlayMessage("取消导出...");
        }
    }
}

/**
 * 保存为csv
 * */
//待评估分页改造
void MainWindow::save2Csv(QString filename,int pageNum,int splitBy, bool useUTF8){
    //计算导出范围
    //默认从0行开始
    int rowBegin=0;
    //默认为当前打开的文件的最大行记录
    int rowEnd=0;
    switch (currentOpenFileType) {
    case 1:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 4:
        rowEnd=dbfRowMap.count();
        break;
    default:
        break;
    }
    //需要按当前页计算
    if(pageNum>0){
        rowBegin=(pageNum-1)*pageRowSize;
        rowEnd=qMin(pageNum*pageRowSize,rowEnd);
    }
    //决定分隔符类别
    QString split="\t";
    switch (splitBy) {
    case 12:
        split="|";
        break;
    case 13:
        split="\t";
        break;
    case 14:
        split=",";
        break;
    default:
        break;
    }
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QFile data(filename);
    //数据类型插入点
    if(currentOpenFileType==openFileType::OFDFile){
        if(!useUTF8){
            codec=codecOFD;
        }
        if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
            //开始准备待写入的数据
            QString sb;
            //标题
            for(int i=0;i<ofd.getFieldCount();i++){
                if(i<ofd.getFieldCount()-1){
                    sb.append(ofd.getFieldList().at(i).getFieldDescribe()).append(split);
                }
                else{
                    sb.append(ofd.getFieldList().at(i).getFieldDescribe()).append("\r\n");
                }
            }
            data.write(codec->fromUnicode(sb));
            data.flush();
            //文本内容
            sb.clear();
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                    for(int col=0;col<ofd.getFieldCount();col++){
                        if(col<ofd.getFieldCount()-1){
                            sb.append(rowdata.at(col)).append(split);
                        }
                        else{
                            sb.append(rowdata.at(col)).append("\r\n");
                        }
                    }
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
            data.close();
            statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
        }else{
            statusBar_disPlayMessage(tr("数据导出失败,请重试"));
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(!useUTF8){
            codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
        }
        if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
            //开始准备待写入的数据
            QString sb;
            //标题
            for(int i=0;i<fixed.getFieldCountMax();i++){
                if(i<fixed.getFieldCountMax()-1){
                    sb.append(fixed.getFieldList().at(i).getFieldDescribe()).append(split);
                }
                else{
                    sb.append(fixed.getFieldList().at(i).getFieldDescribe()).append("\r\n");
                }
            }
            data.write(codec->fromUnicode(sb));
            data.flush();
            //文本内容
            sb.clear();
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                    for(int col=0;col<fixed.getFieldCountMax();col++){
                        if(col<fixed.getFieldCountMax()-1){
                            sb.append(col<rowdata.count()?rowdata.at(col):"").append(split);
                        }
                        else{
                            sb.append(col<rowdata.count()?rowdata.at(col):"").append("\r\n");
                        }
                    }
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
            data.close();
            statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
        }else{
            statusBar_disPlayMessage(tr("数据导出失败,请重试"));
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(!useUTF8){
            codec=QTextCodec::codecForName(dbf.getEcoding().toLocal8Bit());
        }
        if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
            //开始准备待写入的数据
            QString sb;
            //标题
            for(int i=0;i<dbf.getFieldCount();i++){
                if(i<dbf.getFieldCount()-1){
                    sb.append(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe()).append(split);
                }
                else{
                    sb.append(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe()).append("\r\n");
                }
            }
            data.write(codec->fromUnicode(sb));
            data.flush();
            //文本内容
            sb.clear();
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,row,&dbfRowMap,false,dbfTrimType);
                    for(int col=0;col<dbf.getFieldCount();col++){
                        if(col<dbf.getFieldCount()-1){
                            sb.append(col<rowdata.count()?rowdata.at(col):"").append(split);
                        }
                        else{
                            sb.append(col<rowdata.count()?rowdata.at(col):"").append("\r\n");
                        }
                    }
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
            data.close();
            statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
        }else{
            statusBar_disPlayMessage(tr("数据导出失败,请重试"));
        }
    }
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
}


/**
 * 保存为html
 *
 * */
void MainWindow::save2Html (QString filename,int pageNum, bool useUTF8){
    //计算导出范围
    //默认从0行开始
    int rowBegin=0;
    //默认为当前打开的文件的最大行记录
    int rowEnd=0;
    switch (currentOpenFileType) {
    case 1:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 4:
        rowEnd=dbfRowMap.count();
        break;
    default:
        break;
    }
    //需要按当前页计算
    if(pageNum>0){
        rowBegin=(pageNum-1)*pageRowSize;
        rowEnd=qMin(pageNum*pageRowSize,rowEnd);
    }
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile data(filename);
    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
        //开始准备待写入的数据
        QString sb;
        QString title="";
        if(filename.contains("/")){
            title=filename.mid(filename.lastIndexOf("/")+1);
            title=title.left(title.length()-5);
        }else{
            title=title.left(title.length()-5);
        }
        //html文件头
        //csv文件按描述的编码配置
        //数据类型插入点
        if(currentOpenFileType==openFileType::OFDFile){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\""+(useUTF8?"UTF-8":"GB18030")+"\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        }
        if(currentOpenFileType==openFileType::CSVFile){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\""+(useUTF8?"UTF-8":csv.getEcoding())+"\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\""+(useUTF8?"UTF-8":fixed.getEcoding())+"\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        }
        if(currentOpenFileType==openFileType::DBFFile){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\""+(useUTF8?"UTF-8":dbf.getEcoding())+"\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        }
        //内联的表格样式,内容太多,base64存储
        sb.append(QByteArray::fromBase64("PHN0eWxlIHR5cGU9InRleHQvY3NzIj4KLnRhYmxlCnsKcGFkZGluZzogMDsKbWFyZ2luOiAwOwp9CnRoIHsKZm9udDogYm9sZCAxMnB4ICJUcmVidWNoZXQgTVMiLCBWZXJkYW5hLCBBcmlhbCwgSGVsdmV0aWNhLCBzYW5zLXNlcmlmOwpjb2xvcjogIzRmNmI3MjsKYm9yZGVyLXJpZ2h0OiAxcHggc29saWQgI0MxREFENzsKYm9yZGVyLWJvdHRvbTogMXB4IHNvbGlkICNDMURBRDc7CmJvcmRlci10b3A6IDFweCBzb2xpZCAjQzFEQUQ3OwpsZXR0ZXItc3BhY2luZzogMnB4Owp0ZXh0LXRyYW5zZm9ybTogdXBwZXJjYXNlOwp0ZXh0LWFsaWduOiBsZWZ0OwpwYWRkaW5nOiA2cHggNnB4IDZweCAxMnB4OwpiYWNrZ3JvdW5kOiAjQ0FFOEVBIG5vLXJlcGVhdDsKd29yZC1icmVhazoga2VlcC1hbGw7CndoaXRlLXNwYWNlOm5vd3JhcDsKfQp0ciB7CndvcmQtYnJlYWs6IGtlZXAtYWxsOwp3aGl0ZS1zcGFjZTpub3dyYXA7Cn0KdGQgewpib3JkZXItcmlnaHQ6IDFweCBzb2xpZCAjQzFEQUQ3Owpib3JkZXItYm90dG9tOiAxcHggc29saWQgI0MxREFENzsKZm9udC1zaXplOjE0cHg7CnBhZGRpbmc6IDJweCA2cHggMnB4IDZweDsKY29sb3I6ICM0ZjZiNzI7Cn0KLm51bWJlcnsKdGV4dC1hbGlnbjogcmlnaHQ7Cn0KPC9zdHlsZT4K"));
        //标题表头
        sb.append("<table>\r\n<tr>");
        //文件类型插入点，标题
        //列数
        int colCount=0;
        //默认赋值utf-8
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        if(currentOpenFileType==openFileType::OFDFile){
            colCount=ofd.getFieldCount();
            if(!useUTF8){
                codec=codecOFD;
            }
            else{
                codec=QTextCodec::codecForName("UTF-8");
            }
            for(int i=0;i<colCount;i++){
                if(i<colCount-1){
                    sb.append("<th>").append(ofd.getFieldList().at(i).getFieldDescribe()).append("</th>");
                }
                else{
                    sb.append("<th>").append(ofd.getFieldList().at(i).getFieldDescribe()).append("</th></tr>\r\n");
                }
            }
        }
        if(currentOpenFileType==openFileType::CSVFile){
            colCount=csv.getFieldCount();
            if(useUTF8){
                codec=QTextCodec::codecForName("UTF-8");
            }
            else{
                codec=QTextCodec::codecForName(csv.getEcoding().toLocal8Bit());
            }
            for(int i=0;i<ptr_table->columnCount();i++){
                if(i<colCount-1){
                    sb.append("<th>").append(ptr_table->horizontalHeaderItem(i)->text()).append("</th>");
                }
                else{
                    sb.append("<th>").append(ptr_table->horizontalHeaderItem(i)->text()).append("</th></tr>\r\n");
                }
            }
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            colCount=fixed.getFieldCountMax();
            if(useUTF8){
                codec=QTextCodec::codecForName("UTF-8");
            }
            else{
                codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            }
            for(int i=0;i<colCount;i++){
                if(i<colCount-1){
                    sb.append("<th>").append(fixed.getFieldList().at(i).getFieldDescribe()).append("</th>");
                }
                else{
                    sb.append("<th>").append(fixed.getFieldList().at(i).getFieldDescribe()).append("</th></tr>\r\n");
                }
            }
        }
        if(currentOpenFileType==openFileType::DBFFile){
            colCount=dbf.getFieldCount();
            if(!useUTF8){
                codec=QTextCodec::codecForName(dbf.getEcoding().toLocal8Bit());
            }
            else{
                codec=QTextCodec::codecForName("UTF-8");
            }
            for(int i=0;i<colCount;i++){
                if(i<colCount-1){
                    sb.append("<th>").append(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe()).append("</th>");
                }
                else{
                    sb.append("<th>").append(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe()).append("</th></tr>\r\n");
                }
            }
        }
        //输出表头
        data.write(codec->fromUnicode(sb));
        data.flush();
        //文本内容
        sb.clear();
        //数据类型插入点
        if(currentOpenFileType==openFileType::OFDFile){
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    sb.append("<tr>");
                    QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                    for(int col=0;col<colCount;col++){
                        if(ofd.getFieldList().at(col).getFieldType()=="N"){
                            sb.append("<td class=\"number\">");
                        }
                        else{
                            sb.append("<td>");
                        }
                        sb.append(ofd.getFieldList().at(col).getFieldType()=="N"?Utils::CovertDoubleQStringWithThousandSplit(rowdata.at(col)):rowdata.at(col)).append("</td>");
                    }
                    sb.append("</tr>\r\n");
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::CSVFile){
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    //csv文件按行获取数据
                    QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row);
                    sb.append("<tr>");
                    for(int col=0;col<colCount;col++){
                        if(col<rowdata.count()){
                            QString field=rowdata.at(col);
                            //如果本列在设定的数值列
                            if(CsvFieldIsNumberOrNot.contains(col)&&CsvFieldIsNumberOrNot.value(col).getIsNumber()){
                                bool isDouble=false;
                                //依然进行转换判断是否真的是数值，我们仅仅对真的数值设置数值格式
                                field.toDouble(&isDouble);
                                //转换成功
                                if(isDouble){
                                    field=Utils::CovertDoubleQStringWithThousandSplit(field);
                                    sb.append("<td class=\"number\">");
                                }else{
                                    sb.append("<td>");
                                }
                            }
                            else{
                                sb.append("<td>");
                            }
                            sb.append(field).append("</td>");
                        }
                        else{
                            sb.append("<td></td>");
                        }
                    }
                    sb.append("</tr>\r\n");
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    sb.append("<tr>");
                    QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                    for(int col=0;col<colCount;col++){
                        if(col<rowdata.count()){
                            if(fixed.getFieldList().at(col).getFieldType()=="N"){
                                sb.append("<td class=\"number\">");
                            }
                            else{
                                sb.append("<td>");
                            }
                            sb.append(fixed.getFieldList().at(col).getFieldType()=="N"?Utils::CovertDoubleQStringWithThousandSplit(rowdata.at(col)):rowdata.at(col)).append("</td>");
                        }
                        else{
                            sb.append("<td></td>");
                        }
                    }
                    sb.append("</tr>\r\n");
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::DBFFile){
            int exprow=0;
            if(rowEnd>0){
                for (int row=rowBegin;row<rowEnd;row++){
                    //数据写入--按行读取
                    sb.append("<tr>");
                    QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,row,&dbfRowMap,false,dbfTrimType);
                    for(int col=0;col<colCount;col++){
                        if(col<rowdata.count()){
                            if(dbfColIsNumber(col)){
                                sb.append("<td class=\"number\">");
                            }
                            else{
                                sb.append("<td>");
                            }
                            sb.append(dbfColIsNumber(col)?Utils::CovertDoubleQStringWithThousandSplit(rowdata.at(col)):rowdata.at(col)).append("</td>");
                        }
                        else{
                            sb.append("<td></td>");
                        }
                    }
                    sb.append("</tr>\r\n");
                    exprow++;
                    //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                    //仅1000行或者到最后一行时进行写入
                    if((exprow%1000==0)||(row==rowEnd-1)){
                        data.write(codec->fromUnicode(sb));
                        data.flush();
                        sb.clear();
                        statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(exprow)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                }
            }
        }
        //补充html剩余部分
        sb.append("</table>\r\n</body>\r\n</html>");
        data.write(codec->fromUnicode(sb));
        data.flush();
        data.close();
        statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
    }else{
        statusBar_disPlayMessage(tr("数据导出失败,请重试"));
    }
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
}

/**
 * 保存为xlsx
 * */
void MainWindow::save2Xlsx(QString filename,int pageNum){
    //计算导出范围
    //默认从0行开始
    int rowBegin=0;
    //默认为当前打开的文件的最大行记录
    int rowEnd=0;
    switch (currentOpenFileType) {
    case 1:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 4:
        rowEnd=dbfRowMap.count();
        break;
    default:
        break;
    }
    //分页导出,需要按当前页计算
    if(pageNum>0){
        rowBegin=(pageNum-1)*pageRowSize;
        rowEnd=qMin(pageNum*pageRowSize,rowEnd);
    }
    //禁止导出过大的excel文件-目前不会发生这个情况-目前单页最大100万行=最大允许导出的excel行
    if((rowEnd-rowBegin)>maxExcelRow){
        statusBar_disPlayMessage("当前数据记录大于"+QString::number(maxExcelRow)+"行,请降低分页条数到1000000万行或者以下按分页导出Excel...");
        dataBlocked=false;
        isExportFile=false;
        return;
    }
    xlsxSaveName=filename;
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //标题的样式
    QXlsx::Format formatTitle;
    formatTitle.setFont(QFont("SimSun"));
    formatTitle.setFontBold(true);
    formatTitle.setFontColor(QColor(Qt::black));
    formatTitle.setPatternBackgroundColor(QColor(0,176,80));
    formatTitle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatTitle.setBorderStyle(QXlsx::Format::BorderThin);
    //用来记录列最宽的list
    int columnCount=ptr_table->columnCount();
    QList<int> colWidthArray;
    //初始化赋值
    for(int i=0;i<columnCount;i++){
        colWidthArray.append(0);
    }
    //标题和是否是数值列,数值列需要设置列格式
    QHash<int,QXlsx::Format> numberFormatQhash;
    //标准文本样式
    QXlsx::Format formatBody;
    formatBody.setFont(QFont("SimSun"));
    formatBody.setNumberFormat("@");
    int excelRowNumber=2;
    //导出OFD文件的内容
    if(currentOpenFileType==openFileType::OFDFile){
        //插入标题和数值列的专用格式
        for(int i=0;i<columnCount;i++){
            //插入标题
            xlsx->write(1,i+1,ofd.getFieldList().at(i).getFieldDescribe(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(ofd.getFieldList().at(i).getFieldDescribe().toLocal8Bit().length()+5);
            //插入列格式
            if(ofd.getFieldList().at(i).getFieldType()=="N"){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=ofd.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int exprow=0;
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                for(int col=0;col<columnCount;col++){
                    QString value=rowdata.at(col);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=value.length()/3;
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(ofd.getFieldList().at(col).getFieldType()=="N"&&numberFormatQhash.contains(col)){
                        //对于数值型的数据如果接口文档里给的确实是数值型,则填充到excel时也用数值型
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    //导出CSV文件的内容
    if(currentOpenFileType==openFileType::CSVFile){
        //标题内容
        for(int i=0;i<columnCount;i++){
            xlsx->write(1,i+1,ptr_table->horizontalHeaderItem(i)->text(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(ptr_table->horizontalHeaderItem(i)->text().toLocal8Bit().length()+5);
            //插入列格式
            if(CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=CsvFieldIsNumberOrNot.value(i).getDecimalLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int exprow=0;
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                //csv文件按行获取数据
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row);
                //csv文件的数据可能缺失列，因为读取的时候没有做强制判断，这里判断补充下,如果数据长度不够，补空数据
                while(rowdata.count()<columnCount){
                    rowdata.append("");
                }
                for(int col=0;col<columnCount;col++){
                    QString value=rowdata.at(col);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符和小数点后的数据，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=value.length()/3;
                            if(CsvFieldIsNumberOrNot.contains(col)&&CsvFieldIsNumberOrNot.value(col).getIsNumber()){
                                addLen=addLen+CsvFieldIsNumberOrNot.value(col).getDecimalLength()+3;
                            }
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(CsvFieldIsNumberOrNot.contains(col)&&numberFormatQhash.contains(col)){
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    //fixed文件
    if(currentOpenFileType==openFileType::FIXEDFile){
        //插入标题和数值列的专用格式
        for(int i=0;i<columnCount;i++){
            //插入标题
            xlsx->write(1,i+1,fixed.getFieldList().at(i).getFieldDescribe(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(fixed.getFieldList().at(i).getFieldDescribe().toLocal8Bit().length()+5);
            //插入列格式
            if(fixed.getFieldList().at(i).getFieldType()=="N"){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=fixed.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        int exprow=0;
        //文本内容
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                for(int col=0;col<columnCount;col++){
                    QString value=(col<rowdata.count()?rowdata.at(col):nullptr);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=value.length()/3;
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(fixed.getFieldList().at(col).getFieldType()=="N"&&numberFormatQhash.contains(col)){
                        //对于数值型的数据如果接口文档里给的确实是数值型,则填充到excel时也用数值型
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        //插入标题和数值列的专用格式
        for(int i=0;i<columnCount;i++){
            //插入标题
            xlsx->write(1,i+1,dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe()).toLocal8Bit().length()+5;
            //插入列格式
            if(dbfColIsNumber(i)){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=dbf.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int exprow=0;
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,row,&dbfRowMap,false,dbfTrimType);
                for(int col=0;col<columnCount;col++){
                    QString value=rowdata.at(col);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=(value.indexOf(".")+1)/3;
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(dbfColIsNumber(col)&&numberFormatQhash.contains(col)){
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    //根据每列最大的值设置本列的宽度
    for(int i=0;i<columnCount;i++){
        xlsx->setColumnWidth(i+1,i+1,static_cast<double>(colWidthArray[i]));
    }
    statusBar_disPlayMessage(QString("数据生成完毕,正在写入xlsx文件,请耐心等待..."));
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
    //保存文件////写入文件耗时操作放到新的线程取完成
    auto future = QtConcurrent::run(this, &MainWindow::save2XlxsFile);
    watcherXlsxSaveStatus_->setFuture(future);
}

/**
 * @brief MainWindow::save2XlxsFile 一个可能很耗时的xlsx文件写入过程
 * @return
 */
int MainWindow::save2XlxsFile(){
    //专门为xlsx文件写入更新提示
    dataBlockedMessage="正在进行数据导出写入到xlsx文件,请稍后再操作...";
    //保存文件
    xlsx->saveAs(xlsxSaveName);
    //释放内存空间
    delete xlsx;
    //重建变量
    xlsx=new QXlsx::Document();
    //返回结束
    return 1;
}

int MainWindow::extractFileFromZipFile(){
    QFile file(zipfilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        zipExtractMessage=QString("无法打开zip文件[%1],请重试...").arg(zipfilePath);
        zipExtractSucess=false;
        return 0;
    }
    QZipReader reader(&file);
    if (!reader.isReadable()) {
        zipExtractMessage=QString("无法打开zip文件[%1],请重试...").arg(zipfilePath);
        zipExtractSucess=false;
        return 0;
    }
    if(Utils::isFileExist(zipOutPutFileDir)){
        QFile file(zipOutPutFileDir);
        if (file.exists()) {
            if (!file.remove()) {
                zipExtractMessage=QString("临时目录[%1]创建失败,请重试...").arg(zipOutPutFileDir);
                zipExtractSucess=false;
                return 0;
            }
        }
    }
    if(!Utils::isDirExist(zipOutPutFileDir)){
        QDir dir;
        if (!dir.mkpath(zipOutPutFileDir)) {
            zipExtractMessage=QString("临时目录[%1]创建失败,请重试...").arg(zipOutPutFileDir);
            zipExtractSucess=false;
            return 0;
        }
    }
    zipFileCurrentUnzipIndex=0;
    const auto fileList = reader.fileInfoList();
    bool extractOk=true;
    QSet<QString> zipFileNameFromZip;
    for (auto it = fileList.begin(); it != fileList.end(); ++it){
        zipFileNameFromZip.insert(it->filePath);
    }
    QTextCodec *systemCodec=QTextCodec::codecForName("System");
    QTextCodec *zipCodec=QTextCodec::codecForName("UTF-8");
    for (int i=0;i<zipFileInfoList.count();i++){
        //这个是用户在弹窗界面选择的编码获取的
        QString currenctFilePath=zipFileInfoList.at(i).filePath;
        //这个有可能是乱码的-使用系统默认编码获取的
        QString currenctFilePathSystemCodec=zipFileNameSystemCodecList.at(i);
        uint currentFileSize=zipFileInfoList.at(i).size;
        if ((currenctFilePath==zipTargetFileInZip||zipExtractAll)&&(zipFileNameFromZip.contains(currenctFilePathSystemCodec)||zipFileNameFromZip.contains(currenctFilePath))){
            //有可能因为各种原因导致获取不到数据，这里做各种尝试
            //走系统默认编码方式路径尝试读取
            QByteArray fileData = reader.fileData(currenctFilePathSystemCodec);
            if(currentFileSize>0&&fileData.size()==0){
                //走选择得编码方式路径尝试读取
                fileData = reader.fileData(currenctFilePath);
                if (fileData.size()==0){
                    //走固定的UTF-8编码方式路径尝试读取
                    QTextCodec::setCodecForLocale(zipCodec);
                    fileData = reader.fileData(currenctFilePath);
                    QTextCodec::setCodecForLocale(systemCodec);
                    if(fileData.size()==0){
                        zipExtractMessage=QString("从压缩包中提取[%1]失败,可能是不支持的压缩算法(非Deflate算法)/文件不完整/加密压缩,请重试或者使用系统解压缩工具解压...").arg(currenctFilePath);
                        extractOk=false;
                        break;
                    }
                }
            }
            QString targetFilePath=zipOutPutFileDir+currenctFilePath;
            QString targetFileDir=targetFilePath.left(targetFilePath.lastIndexOf("/")+1);
            if(Utils::isFileExist(targetFileDir)){
                QFile file(targetFileDir);
                if (file.exists()) {
                    if (!file.remove()) {
                        zipExtractMessage=QString("临时目录[%1]创建失败,请重试或检查是否有对该路径的读写权限...").arg(targetFileDir);
                        extractOk=false;
                        break;
                    }
                }
            }
            if(!Utils::isDirExist(targetFileDir)){
                QDir dir;
                if (!dir.mkpath(targetFileDir)) {
                    zipExtractMessage=QString("临时目录[%1]创建失败,请重试或检查是否有对该路径的读写权限...").arg(targetFileDir);
                    extractOk=false;
                    break;
                }
            }
            QFile outFile(targetFilePath);
            if (!outFile.open(QIODevice::WriteOnly)) {
                zipExtractMessage=QString("从压缩包中提取文件[%1]后写入失败,请重试或者检查是否有对路径[%2]的读写权限...").arg(currenctFilePath,targetFileDir);
                extractOk=false;
                break;
            }
            int returnCode=outFile.write(fileData);
            outFile.close();
            if(returnCode==-1){
                zipExtractMessage=QString("从压缩包中提取文件[%1]后写入失败,请重试或者检查是否有对路径[%2]的读写权限...").arg(currenctFilePath,targetFileDir);
                extractOk=false;
                break;
            }
            zipFileCurrentUnzipIndex++;
            emit signals_update_zip_extract_status();
            //恢复文件的修改时间-该时间从压缩包中提取
            Utils::UpdateFileTime(targetFilePath,zipFileListMdTime.value(currenctFilePath));
        }
        else{
            continue;
        }
    }
    reader.close();
    file.close();
    if(extractOk){
        zipExtractSucess=true;
        zipExtractMessage="";
        return 1;
    }
    else{
        zipExtractSucess=false;
        return 0;
    }
}

/**
 * @brief MainWindow::save2XlsxFinished 写入xlsx文件完毕的通知
 */
void MainWindow::save2XlsxFinished(){
    dataBlocked=false;
    statusBar_disPlayMessage(tr("数据成功导出到:%1").arg(xlsxSaveName));
    //如果需要打开文件
    if(openXlsx){
        statusBar_disPlayMessage(tr("使用系统默认程序打开文件:%1").arg(xlsxSaveName));
        bool is_open = QDesktopServices::openUrl(QUrl::fromLocalFile(xlsxSaveName));
        if(!is_open)
        {
            statusBar_disPlayMessage("文件打开失败,请尝试手工打开~~~");
            return;
        }
    }
}


/**
 * @brief MainWindow::on_pushButtonRowJump_clicked 行跳转
 */
void MainWindow::on_pushButtonRowJump_clicked()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用行跳转功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    QString lineStr=ui->lineTextText_2->text();
    if(lineStr.isEmpty()){
        return;
    }
    bool ok=false;
    int  lineNumber=lineStr.toInt(&ok);
    if(!ok){
        statusBar_disPlayMessage(tr("请输入有效的行号,%1不是一个有效的整数").arg(lineStr));
        return;
    }
    if(lineNumber<1){
        statusBar_disPlayMessage(tr("行号不能小于1"));
    }
    else{
        //数据类型加入点
        int targetIndex=0;
        if(currentOpenFileType==openFileType::OFDFile){
            if(commonContentQByteArrayList.count()<1){
                statusBar_disPlayMessage("打开的OFD/类OFD文件没有数据记录...");
                return;
            }
            else if(lineNumber>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==openFileType::CSVFile){
            if(commonContentQByteArrayList.count()<1){
                statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
                return;
            }
            else if(lineNumber>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            if(commonContentQByteArrayList.count()<1){
                statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
                return;
            }
            else if(lineNumber>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==openFileType::DBFFile){
            if(dbfDisplayType==0){
                if(dbfRowMap.count()<1){
                    statusBar_disPlayMessage("打开的DBF文件没有数据记录...");
                    return;
                }
            }
            else{
                if(dbfRowMap.count()<1){
                    statusBar_disPlayMessage("打开的DBF文件在当前模式下没有数据记录...");
                    return;
                }
            }
            if(lineNumber>dbfRowMap.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了当前浏览模式下数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        //判断是否在当前页，是否需要跳行
        int targetPage=(targetIndex/pageRowSize)+1;
        int targetTableIndex=(targetIndex%pageRowSize);
        if(targetPage==currentPage){
            ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
            ptr_table->setFocus();
        }
        else{
            pageJump(targetPage,targetTableIndex);
            ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
            ptr_table->setFocus();
        }
    }
}

/**
 * @brief MainWindow::on_actionsss_triggered 刷新，进行文件重新加载
 */
void MainWindow::on_actionsss_triggered()
{
    //如果没有忽略，则直接返回
    if(!ignoreFileChangeAndOpenNewFile()){
        return;
    }
    //加载数据中时忽略刷新
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再刷新...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(!currentOpenFilePath.isEmpty()){
        isUpdateData=true;
        if(fileFromZip){
            initFile(currentOpenFilePath,false,false,true);
        }
        else{
            initFile(currentOpenFilePath);
        }
        isUpdateData=false;
    }
}

/**
 * @brief MainWindow::on_actionSave_triggered 文件保存功能槽函数
 */
void MainWindow::on_actionSave_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用行跳转功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
    }
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        if(fileChanged){
            //先备份原文件
            if(Utils::isFileExist(currentOpenFilePath)){
                QFile oldfile(currentOpenFilePath);
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyyMMddhhmmss");
                bool r=oldfile.rename(oldfile.fileName()+"."+current_date+".BAK");
                if(!r){
                    statusBar_disPlayMessage("备份原文件失败,请重试或检查文件是否被其他程序占用");
                    return;
                }
            }
            //原文件备份完毕后开始保存新的文件,文件名和之前保持一致
            saveOFDOrFixedFile(currentOpenFilePath);
            if(fileFromZip){
                qDebug()<<"保存的文件来自zip";
                qDebug()<<zipfilePath;
                qDebug()<<zipTargetFileInZip;
                DialogMyTip  dialog2(QString("当前保存的文件来自压缩包,如果你需要将修改后的文件更新到压缩包,请自行使用压缩软件更新\r\n压缩包路径:%1\r\n文件在压缩包中的路径:%2\r\n当前文件保存路径:%3").arg(zipfilePath,zipTargetFileInZip,currentOpenFilePath),this,true);
                dialog2.setWindowTitle("温馨提示");
                dialog2.setModal(true);
                dialog2.exec();
            }
        }
        else{
            statusBar_disPlayMessage("文件没有被修改,无需保存");
        }
    }
    else if(currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("暂不支持保存此类文件");
    }
}

void MainWindow::saveOFDOrFixedFile(QString filepath)
{
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(Utils::isFileExist(filepath)){
        QFile oldfile(filepath);
        bool r=oldfile.remove();
        if(!r){statusBar_disPlayMessage("覆盖文件失败,请重试或检查文件是否被其他程序占用");return;}
    }
    if(!lastRowHasNewLine){
        DialogMyTip  dialog2("当前文件最后一行没有换行符,是否在保存时在最后一行添加换行符？\r\n备注:如果你不确定,请选择\"否\"或关闭该弹窗,将按文件当前特性不添加换行符\r\n",this);
        dialog2.setWindowTitle("温馨提示");
        dialog2.setModal(true);
        dialog2.exec();
        if(dialog2.getBoolFlag()){
            lastRowHasNewLine=true;
        }
    }
    dataBlocked=true;
    dataBlockedMessage="正在保存写入文件,请稍后再操作...";
    QFile newfile(filepath);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (newfile.open(QFile::WriteOnly | QIODevice::Truncate)) {
        if(currentOpenFileType==openFileType::OFDFile){
            QString sb;
            for(int i=0;i<commonHeaderQStringList.length();i++){
                sb.append(commonHeaderQStringList.at(i)).append("\r\n");
            }
            newfile.write(codecOFD->fromUnicode(sb));
            sb.clear();
            int row=0;
            while(row<commonContentQByteArrayList.length()){
                if(dataCompressLevel==0){
                    sb.append(codecOFD->toUnicode(commonContentQByteArrayList.at(row))).append("\r\n");
                }
                else{
                    sb.append(codecOFD->toUnicode(qUncompress(commonContentQByteArrayList.at(row)))).append("\r\n");
                }
                if((row%1000==0)||(row==commonContentQByteArrayList.count()-1)){
                    newfile.write(codecOFD->fromUnicode(sb));
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                row++;
            }
            if(lastRowHasNewLine){
                newfile.write(codecOFD->fromUnicode(ofdFooterQString+"\r\n"));
            }
            else{
                newfile.write(codecOFD->fromUnicode(ofdFooterQString));
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            QString end="\r\n";
            QTextCodec *codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            if(currentFileNewLineType==newLineType::CR){end="\r";}
            else if(currentFileNewLineType==newLineType::CRLF){end="\r\n";}
            else if(currentFileNewLineType==newLineType::LF){end="\n";}
            QString sb;
            for(int i=0;i<commonHeaderQStringList.count();i++){
                sb.append(commonHeaderQStringList.at(i)).append(end);
            }
            newfile.write(codec->fromUnicode(sb));
            sb.clear();
            int row=0;
            while(row<commonContentQByteArrayList.length()){
                if(dataCompressLevel==0){
                    sb.append(codec->toUnicode(commonContentQByteArrayList.at(row))).append(end);
                }
                else{
                    sb.append(codec->toUnicode(qUncompress(commonContentQByteArrayList.at(row)))).append(end);
                }
                if((row%1000==0)||(row==commonContentQByteArrayList.count()-1)){
                    newfile.write(codec->fromUnicode(sb));
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                row++;
            }
            for(int i=0;i<commonFooterQStringList.count();i++){
                if(i<commonFooterQStringList.count()-1){
                    newfile.write(codec->fromUnicode(commonFooterQStringList.at(i)+end));
                }
                else{
                    if(lastRowHasNewLine){
                        newfile.write(codec->fromUnicode(commonFooterQStringList.at(i)+end));
                    }
                    else{
                        newfile.write(codec->fromUnicode(commonFooterQStringList.at(i)));
                    }
                }
            }
        }
        newfile.flush();
        newfile.close();
        fileChanged=false;
        statusBar_disPlayMessage(tr("文件保存完毕,保存在%1").arg(filepath));
        this->setWindowTitle(appName+"-"+currentFileName);
        currentOpenFilePath=filepath;
        ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
    }else{
        statusBar_disPlayMessage(tr("数据保存失败,请重试"));
    }
    //恢复鼠标响应
    dataBlocked=false;
    QApplication::restoreOverrideCursor();
}

/**
 * @brief MainWindow::on_actionSaveAS_triggered 文件另存功能
 */
void MainWindow::on_actionSaveAS_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用行跳转功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
        return;
    }
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        if(currentOpenFileType==openFileType::OFDFile&&commonHeaderQStringList.size()<1){
            statusBar_disPlayMessage("未打开有效的OFD/类OFD文件,无法使用另存功能,请尝试打开一个有效的文件...");
            return;
        }
        QString selectedFilter=Q_NULLPTR;
        QString fileNameSave = QFileDialog::getSaveFileName(this,("另存为"),currentOpenFilePath,tr("所有文件(*);;文本文件(*.TXT)"),&selectedFilter);
        if(!fileNameSave.isEmpty()){
            if(selectedFilter=="文本文件(*.TXT)"&&(!fileNameSave.endsWith(".TXT"))){
                fileNameSave.append(".TXT");
            }
            saveOFDOrFixedFile(fileNameSave);
        }
    }
    else{
        statusBar_disPlayMessage("暂不支持另存此类文件");
    }
}

/**
 * @brief MainWindow::on_actionaboutAuthor_triggered 关于作者
 */
void MainWindow::on_actionaboutAuthor_triggered()
{
    DialogAboutAuthor  *dialog=new DialogAboutAuthor (this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

/**
 * @brief MainWindow::on_tableWidget_cellDoubleClicked 双击单元格则弹出行详情
 * @param row
 * @param column
 */
void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //数据类型判断点
    tableRowCurrent=row;
    tableColCurrent=column;
    if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
        showRowDetails();
    }
}

/**
 * @brief MainWindow::randomTips 随机在状态栏给出一个温馨提醒
 */
void MainWindow::randomTips(){
    srand((unsigned)time(nullptr));
    int index =0;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 99)
    index = QRandomGenerator::global()->bounded(tips.count());
#else
    index =rand()%tips.count();
#endif
    QTime current_time =QTime::currentTime();
    int hour = current_time.hour();
    //深夜11点后，到凌晨5点前的特殊提醒
    if(hour>22||hour<5){
        statusBar_disPlayMessage("温馨提示:深夜了,请注意休息哟!如果你还在坚持排查生产问题,那就祝你好运吧!");
    }
    else{
        statusBar_disPlayMessage("温馨提示:"+tips.at(index));
    }
}

/**
 * @brief MainWindow::on_pushButtonRowJump2_clicked 源文件行跳转功能
 */
//待完善跳页
void MainWindow::on_pushButtonRowJump2_clicked()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用行跳转功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("不支持DBF文件使用此功能...");
        return;
    }
    //数据类型加入点
    if(currentOpenFileType==openFileType::OFDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD/类OFD文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
    }
    QString lineStr=ui->lineTextText_2->text();
    if(lineStr.isEmpty()){
        return;
    }
    bool ok=false;
    int  lineNumber=lineStr.toInt(&ok);
    if(!ok){
        statusBar_disPlayMessage(tr("请输入有效的行号,%1不是一个有效的整数").arg(lineStr));
        return;
    }
    if(lineNumber<1){
        statusBar_disPlayMessage(tr("行号不能小于1"));
    }
    else {
        //数据类型加入点
        int targetIndex=0;
        if(currentOpenFileType==openFileType::OFDFile){
            int headerCount=commonHeaderQStringList.count();
            if(lineNumber<=headerCount){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是文件头:[%1]").arg(commonHeaderQStringList.at(lineNumber-1)));
                return;
            }
            //重新计算数据行位置
            lineNumber-=headerCount;
            if(lineNumber==commonContentQByteArrayList.count()+1){
                statusBar_disPlayMessage(tr("你输入的行号所在行应该是OFD/类OFD文件结束标记[OFDCFEND]"));
                return;
            }
            else if(lineNumber>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==openFileType::CSVFile){
            int headerCount=commonHeaderQStringList.count();
            if(lineNumber==csv.getTitlerowindex()){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是标题行"));
                return;
            }
            //重新计算数据行位置
            lineNumber-=headerCount;
            if(lineNumber>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            int headerCount=commonHeaderQStringList.count();
            int contentCount=commonContentQByteArrayList.count();
            int footerCount=commonFooterQStringList.count();
            if(lineNumber<=headerCount){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是文件头:[%1]").arg(commonHeaderQStringList.at(lineNumber-1)));
                return;
            }
            //重新计算数据行位置
            lineNumber-=headerCount;
            //文件数据
            if(lineNumber>0&&lineNumber<=contentCount){
                targetIndex=lineNumber-1;
            }
            else if(lineNumber>contentCount&&lineNumber<=(contentCount+footerCount)){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是文件尾:[%1]").arg(commonFooterQStringList.at(lineNumber-contentCount-1)));
                return;
            }
            else if(lineNumber>(contentCount+footerCount)){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
        }
        //判断是否在当前页，是否需要跳行
        int targetPage=(targetIndex/pageRowSize)+1;
        int targetTableIndex=(targetIndex%pageRowSize);
        if(targetPage==currentPage){
            ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
            ptr_table->setFocus();
        }
        else{
            pageJump(targetPage,targetTableIndex);
            ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
            ptr_table->setFocus();
        }
    }
}

/**
 * @brief MainWindow::on_viewMode_triggered 界面模式切换
 */
void MainWindow::on_viewMode_triggered()
{
    bool isHidden=ui->frameInfo->isHidden();
    if(!isHidden){
        ui->frameInfo->setHidden(true);
        ui->viewMode->setText("标准视图");
        if(!isUpdateData){
            reCalculateTableBeginAndEnd();
        }
    }
    else{
        ui->frameInfo->setHidden(false);
        ui->viewMode->setText("精简视图");
    }
}

/**
 * @brief MainWindow::on_tableWidget_itemSelectionChanged 表格选择范围改变触发的事件，当表格选择范围变化时，进行数据统计！！！
 */
//需要分页改造
void MainWindow::on_tableWidget_itemSelectionChanged()
{
    if(dataBlocked){
        return;
    }
    firstNotNumberLine=0;
    //选择的范围
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    //范围和
    int rangeCount=itemsRange.count();
    //如果仅仅选择了一个单元格，先更新当前选择的单元格
    ///////////////////////////////////////////////////
    if(rangeCount==1&&itemsRange.at(0).leftColumn()==itemsRange.at(0).rightColumn()&&itemsRange.at(0).topRow()==itemsRange.at(0).bottomRow()){
        //记录当前所在行
        tableRowCurrent=itemsRange.at(itemsRange.count()-1).bottomRow();
        //当前所在列
        tableColCurrent=itemsRange.at(itemsRange.count()-1).rightColumn();
        //更新列跳转搜索开始列
        tableColSearch=tableColCurrent;
    }
    //如果未在更新数据，如果未执行导出等阻断任务
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据中...");
        return;
    }
    //当导出文件时的阻断，不往下执行并且显示阻断的原因，其余情况继续执行
    if(isExportFile&&dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    //判断是否要显示选择的单元格的信息
    //仅选择一个单元格-----仅仅选择了一个单元格，就显示这个单元格的信息
    if(rangeCount==1&&itemsRange.at(0).leftColumn()==itemsRange.at(0).rightColumn()&&itemsRange.at(0).topRow()==itemsRange.at(0).bottomRow()){
        //将表格中的行号转换成数据List内的行数
        //分页改造
        int dataRowCurrent=(currentPage-1)*pageRowSize+tableRowCurrent;
        //OFD索引文件
        if(currentOpenFileType==openFileType::OFDIndex){
            statusBar_display_rowsAndCol(dataRowCurrent+5,0,-1);
            //判断数据记录是否包含空格
            if(ptr_table->item(dataRowCurrent,0)->text().endsWith(" ")){
                statusBar_disPlayMessage("检测到存在空格填充,索引文件记录行一般不填充空格!");
            }
        }
        //OFD数据文件
        else if(currentOpenFileType==openFileType::OFDFile){
            int rowInFile=commonHeaderQStringList.count()+dataRowCurrent+1;
            int colInFile=ofd.getFieldList().at(tableColCurrent).getRowBeginIndex()+1;
            //显示当前行列
            statusBar_display_rowsAndCol(rowInFile,colInFile,ofd.getFieldList().at(tableColCurrent).getLength());
            if(ptr_table->item(tableRowCurrent,tableColCurrent)!=nullptr){
                QString text=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
                QString dic=ofdDictionary.getDictionary(ofd.getDictionary()+"_"+ofd.getFieldList().at(tableColCurrent).getFieldName(),text);
                if(text.isEmpty()){
                    statusBar_disPlayMessage(ofd.getFieldList().at(tableColCurrent).getFieldDescribe().append("/").append(ofd.getFieldList().at(tableColCurrent).getFieldName()).append("|").append(ofd.getFieldList().at(tableColCurrent).getFieldType()));
                }
                else{
                    //补充温馨提示
                    QString addinfo="";
                    int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
                    QString fieldOaiginal=Utils::getOriginalValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
                    if(ofd.getFieldList().at(tableColCurrent).getFieldType()=="N"){
                        //长度大于1存在左侧空格右对齐
                        if(fieldOaiginal.length()>1&&fieldOaiginal.startsWith(" ")&&fieldOaiginal.endsWith("0")&&text=="0"){
                            addinfo="警告:发现字段左填充空格,数值型字段值为0时不建议右对齐左填充空格,建议全填充0!!!";
                        }
                        //长度大于1存在左对齐右侧空格
                        else  if(fieldOaiginal.length()>1&&fieldOaiginal.startsWith("0")&&fieldOaiginal.endsWith(" ")&&text=="0"){
                            addinfo="警告:发现字段右填充空格,数值型字段值为0时不建议左对齐右填充空格,建议全填充0!!!";
                        }
                        //中间含有空格
                        else if((!text.isEmpty())&&fieldOaiginal.contains(" ")){
                            addinfo="警告:发现字段含有空格,数值型字段非空时不建议填充或者含有空格,极易造成解析兼容问题!!!";
                        }
                        //其余情况统一处理--不能转换为double数据
                        else if(!text.isEmpty()){
                            QString fieldValue=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
                            bool okFlag=true;
                            fieldValue.toDouble(&okFlag);
                            if(!okFlag){
                                addinfo="警告:这个字段的值好像不能解析为一个数值!!!";
                            }
                        }
                    }
                    statusBar_disPlayMessage(ofd.getFieldList().at(tableColCurrent).getFieldDescribe().append("/").append(ofd.getFieldList().at(tableColCurrent).getFieldName()).append("|").append(ofd.getFieldList().at(tableColCurrent).getFieldType()).append("|").append(text).append(dic.isEmpty()?"":("|"+dic)).append(addinfo.isEmpty()?"":("|"+addinfo)));
                }
            }
            else{
                statusBar_disPlayMessage(ofd.getFieldList().at(tableColCurrent).getFieldDescribe().append("/").append(ofd.getFieldList().at(tableColCurrent).getFieldName()).append("|").append(ofd.getFieldList().at(tableColCurrent).getFieldType()));
            }
        }
        //CSV文件
        else if(currentOpenFileType==openFileType::CSVFile){
            int rowInFile=commonHeaderQStringList.count()+dataRowCurrent+1;
            int colInFile=tableColCurrent+1;
            statusBar_display_rowsAndCol(rowInFile,colInFile,0);
            if(ptr_table->item(tableRowCurrent,tableColCurrent)!=nullptr){
                QString text=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
                QString dic=commonDictionary.value("CSV"+csv.getFileIni()).getDictionary(csv.getFieldList().at(tableColCurrent).getFieldDescribe(),text);
                if(!text.isEmpty()){
                    //如果文件执行过双引号边界符处理，则附带显示原始数据，方便用户阅览原始值
                    if(csv.getClearQuotes()||csv.getTrim()){
                        QStringList rowdata=Utils::getOriginalRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,dataRowCurrent);
                        statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)).append("|原始值:").append(rowdata.at(tableColCurrent)));
                    }
                    //如果探测到本列是数值,也附带显示原始值
                    else if(CsvFieldIsNumberOrNot.value(tableColCurrent).getIsNumber()){
                        QStringList rowdata=Utils::getOriginalRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,dataRowCurrent);
                        text=text.append(dic.isEmpty()?"":("|"+dic)).append("|原始值:").append(rowdata.at(tableColCurrent));
                        int numberShiftint=csv.getFieldList().count()<=tableColCurrent?0:csv.getFieldList().at(tableColCurrent).getDecimalPointShift();
                        if(numberShiftint>0){
                            switch (numberShiftint){
                            case 1:
                                text.append(QString("|缩小%1倍展示").arg(10));
                                break;
                            case 2:
                                text.append(QString("|缩小%1倍展示").arg(100));
                                break;
                            case 3:
                                text.append(QString("|缩小%1倍展示").arg(1000));
                                break;
                            case 4:
                                text.append(QString("|缩小%1倍展示").arg(10000));
                                break;
                            default:
                                break;
                            }
                        }
                        statusBar_disPlayMessage(text);
                    }
                    else{
                        statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)));
                    }
                }
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            int rowInFile=commonHeaderQStringList.count()+dataRowCurrent+1;
            int colInFile=tableColCurrent+1;
            statusBar_display_rowsAndCol(rowInFile,colInFile,fixed.getFieldList().at(tableColCurrent).getLength());
            if(ptr_table->item(tableRowCurrent,tableColCurrent)!=nullptr){
                QString text=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
                QString dic=commonDictionary.value("FIXED"+fixed.getFileIni()).getDictionary(fixed.getFieldList().at(tableColCurrent).getFieldDescribe(),text);
                if(!text.isEmpty()){
                    statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)));
                }
            }
        }
        else if(currentOpenFileType==openFileType::DBFFile){
            int colInFile=tableColCurrent+1;
            statusBar_display_rowsAndCol(dbfRowMap.value(dataRowCurrent)+1,colInFile,dbf.getFieldList().at(tableColCurrent).getLength());
            if(ptr_table->item(tableRowCurrent,tableColCurrent)!=nullptr){
                QString text=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
                QString dic=commonDictionary.value("DBF"+dbf.getFileIni()).getDictionary(dbf.getFieldList().at(tableColCurrent).getFieldName().toUpper(),text);
                QString fieldType="";
                switch (dbf.getFieldList().at(tableColCurrent).getFieldType()) {
                case QDbf::QDbfField::Character:
                    fieldType="Character-字符型";
                    break;
                case QDbf::QDbfField::Currency:
                    fieldType="Currency-货币型";
                    break;
                case QDbf::QDbfField::Date:
                    fieldType="Date-日期型";
                    break;
                case QDbf::QDbfField::DateTime:
                    fieldType="DateTime日期时间型";
                    break;
                case QDbf::QDbfField::FloatingPoint:
                    fieldType="FloatingPoint浮点型";
                    break;
                case QDbf::QDbfField::Integer:
                    fieldType="Integer整数型";
                    break;
                case QDbf::QDbfField::Logical:
                    fieldType="Logical逻辑型";
                    break;
                case QDbf::QDbfField::Memo:
                    fieldType="Memo备注型";
                    break;
                case QDbf::QDbfField::Number:
                    fieldType="Number数字型";
                    break;
                default:
                    fieldType=("Undefined-未定义类型");
                }
                if(text.isEmpty()){
                    statusBar_disPlayMessage(dbf.getFieldList().at(tableColCurrent).getFieldName().append("|").append(fieldType));
                }
                else{
                    statusBar_disPlayMessage(dbf.getFieldList().at(tableColCurrent).getFieldName().append("|").append(fieldType).append("|").append(text).append(dic.isEmpty()?"":("|"+dic)));
                }
            }
        }
        //***********未实现的数据类型插入点
    }
    //其余情况则走分析统计
    else {
        //选择的总单元格
        int selectedAllItemCount=0;
        //是否跨列的标记
        bool crossColumnFlag=false;
        //循环遍历选择的范围区间,进行计数和跨列判断
        for(int i=0;i<rangeCount;i++){
            selectedAllItemCount+=(itemsRange.at(i).rightColumn()-itemsRange.at(i).leftColumn()+1)*(itemsRange.at(i).bottomRow()-itemsRange.at(i).topRow()+1);
            //选择范围跨列,与上一个范围区间列不等这两种情况,不进行数据统计
            if(i==0){
                if((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())){
                    crossColumnFlag=true;
                }
            }else{
                //这里只更新一次flag,如果上次判断已经跨列，则不在更新
                if(((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())||(itemsRange.at(i).leftColumn()!=itemsRange.at(i-1).leftColumn()))&&!crossColumnFlag){
                    crossColumnFlag=true;
                }
            }
        }
        //如果多个选择器也没跨列，则允许进行统计
        if(!crossColumnFlag&&selectedAllItemCount>1){
            //如果选择了多个单元格但是没有跨列，更新当前列，以免导致右键菜单批量更新某列数据时定位列错误
            //当前所在列
            tableColCurrent=itemsRange.at(0).rightColumn();
            //需要统计的列
            int editCol=itemsRange.at(0).leftColumn();
            //总和
            double selectSum=0;
            //跳过的空
            int skipEmpty=0;
            //跳过的非数值
            int skipNotNumber=0;
            //成功统计的数量
            int numberCount=0;
            int editRowinFileContent=0;
            //显示类别
            int displayType=0;
            //用于触发窗口事件的计数器
            int itmCount=0;
            //本次统计任务的index
            int currentCalculateIndex=++calculateIndex;
            qDebug()<<"本次统计任务id"<<currentCalculateIndex;
            //当要进行数值统计的单元格较多时，运算往往需要一定的事件，温馨提示用户，呜呜呜
            if(selectedAllItemCount>10000){
                statusBar_disPlayMessage("统计中,请稍等...");
            }
            //判断当前打开的文件类型
            if(currentOpenFileType==openFileType::OFDFile){
                if(ofd.getFieldList().at(itemsRange.at(0).leftColumn()).getFieldType()=="N"){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QString vvv=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,editRowinFileContent,editCol);
                            if(vvv==nullptr){
                                skipEmpty++;
                            }
                            else{
                                bool covertOk=false;
                                double valueitem=vvv.toDouble(&covertOk);
                                if(covertOk){
                                    selectSum+=valueitem;
                                    numberCount++;
                                }
                                else{
                                    skipNotNumber++;
                                    if(skipNotNumber==1){
                                        firstNotNumberLine=editRowinFileContent+1;
                                    }
                                }
                            }
                            itmCount++;
                            if((itmCount%1000)==0||selectedAllItemCount==itmCount){
                                if(currentCalculateIndex==calculateIndex){
                                    qApp->processEvents();
                                }
                                else{
                                    qDebug()<<currentCalculateIndex<<"终止,不再展示统计结果";
                                    return;
                                }
                            }
                        }
                    }
                    displayType=0;
                }
                else{
                    displayType=1;
                }
            }
            else  if(currentOpenFileType==openFileType::CSVFile){
                if((CsvFieldIsNumberOrNot.contains(itemsRange.at(0).leftColumn())&&CsvFieldIsNumberOrNot.value(itemsRange.at(0).leftColumn()).getIsNumber())){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QStringList rowList=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,editRowinFileContent);
                            QString vvv="";
                            if(rowList.count()>editCol){
                                vvv=rowList.at(editCol);
                            }
                            if(vvv==nullptr){
                                skipEmpty++;
                            }
                            else{
                                bool covertOk=false;
                                double valueitem=vvv.toDouble(&covertOk);
                                if(covertOk){
                                    selectSum+=valueitem;
                                    numberCount++;
                                }
                                else{
                                    skipNotNumber++;
                                    if(skipNotNumber==1){
                                        firstNotNumberLine=editRowinFileContent+1;
                                    }
                                }
                            }
                            itmCount++;
                            if((itmCount%1000)==0||selectedAllItemCount==itmCount){
                                if(currentCalculateIndex==calculateIndex){
                                    qApp->processEvents();
                                }
                                else{
                                    qDebug()<<currentCalculateIndex<<"终止,不再展示统计结果";
                                    return;
                                }
                            }
                        }
                    }
                    displayType=0;
                }
                else{
                    displayType=1;
                }
            }
            if(currentOpenFileType==openFileType::FIXEDFile){
                if(fixed.getFieldList().at(itemsRange.at(0).leftColumn()).getFieldType()=="N"){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QString vvv=Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,editRowinFileContent,editCol);
                            if(vvv==nullptr){
                                skipEmpty++;
                            }
                            else{
                                bool covertOk=false;
                                double valueitem=vvv.toDouble(&covertOk);
                                if(covertOk){
                                    selectSum+=valueitem;
                                    numberCount++;
                                }
                                else{
                                    skipNotNumber++;
                                    if(skipNotNumber==1){
                                        firstNotNumberLine=editRowinFileContent+1;
                                    }
                                }
                            }
                            itmCount++;
                            if((itmCount%1000)==0||selectedAllItemCount==itmCount){
                                if(currentCalculateIndex==calculateIndex){
                                    qApp->processEvents();
                                }
                                else{
                                    qDebug()<<currentCalculateIndex<<"终止,不再展示统计结果";
                                    return;
                                }
                            }
                        }
                    }
                    displayType=0;
                }
                else{
                    displayType=1;
                }
            }
            if(currentOpenFileType==openFileType::DBFFile){
                if(dbfColIsNumber(itemsRange.at(0).leftColumn())){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QString vvv=Utils::getFormatValuesFromdbfTableFile(&dbftablefile,&dbf,editRowinFileContent,editCol,&dbfRowMap,dbfTrimType);
                            if(vvv==nullptr){
                                skipEmpty++;
                            }
                            else{
                                bool covertOk=false;
                                double valueitem=vvv.toDouble(&covertOk);
                                if(covertOk){
                                    selectSum+=valueitem;
                                    numberCount++;
                                }
                                else{
                                    skipNotNumber++;
                                    if(skipNotNumber==1){
                                        firstNotNumberLine=editRowinFileContent+1;
                                    }
                                }
                            }
                            itmCount++;
                            if((itmCount%1000)==0||selectedAllItemCount==itmCount){
                                if(currentCalculateIndex==calculateIndex){
                                    qApp->processEvents();
                                }
                                else{
                                    qDebug()<<currentCalculateIndex<<"终止,不再展示统计结果";
                                    return;
                                }
                            }
                        }
                    }
                    displayType=0;
                }
                else{
                    displayType=1;
                }
            }
            //结束--决定要干的事情
            if (displayType==0) {
                //统计
                ////////////小数长度
                int declength=0;
                /////////数据类型插入点/////////////////////////////////////////
                if(currentOpenFileType==openFileType::OFDFile){
                    declength=ofd.getFieldList().at(editCol).getDecLength();
                }
                else if(currentOpenFileType==openFileType::CSVFile){
                    if(CsvFieldIsNumberOrNot.contains(itemsRange.at(0).leftColumn())&&CsvFieldIsNumberOrNot.value(itemsRange.at(0).leftColumn()).getIsNumber()){
                        declength=CsvFieldIsNumberOrNot.value(itemsRange.at(0).leftColumn()).getDecimalLength();
                    }
                    else{
                        declength=2;
                    }
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    declength=fixed.getFieldList().at(editCol).getDecLength();
                }
                else if(currentOpenFileType==openFileType::DBFFile){
                    declength=dbf.getFieldList().at(editCol).getDecLength();
                }
                //开始显示统计结果
                if(skipNotNumber==0){
                    statusBar_disPlayMessage("[所有:"+QString::number(selectedAllItemCount)+"  计数:"+QString::number(numberCount)+"  平均值:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum/selectedAllItemCount,'f',declength))+"  求和:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum,'f',declength))+"]  [空值:"+QString::number(skipEmpty)+"  非法数值:"+QString::number(skipNotNumber)+"]");
                }
                else if(skipNotNumber==1){
                    statusBar_disPlayMessage("[所有:"+QString::number(selectedAllItemCount)+"  计数:"+QString::number(numberCount)+"  平均值:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum/selectedAllItemCount,'f',declength))+"  求和:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum,'f',declength))+"]  [空值:"+QString::number(skipEmpty)+"  非法数值"+QString::number(skipNotNumber)+"个,在第"+QString::number(firstNotNumberLine)+"行]");
                }else{
                    statusBar_disPlayMessage("[所有:"+QString::number(selectedAllItemCount)+"  计数:"+QString::number(numberCount)+"  平均值:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum/selectedAllItemCount,'f',declength))+"  求和:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum,'f',declength))+"]  [空值:"+QString::number(skipEmpty)+"  非法数值"+QString::number(skipNotNumber)+"个,第1个在第"+QString::number(firstNotNumberLine)+"行]");
                }
            }
            else if (displayType==1){
                //计数
                statusBar_disPlayMessage("计数:"+QString::number(selectedAllItemCount));
            }
        }
        else{
            statusBar_disPlayMessage("计数:"+QString::number(selectedAllItemCount));
        }
    }
}


/**
 * @brief MainWindow::on_actionpreference_triggered 设置
 */
//需要改造--需要新增设置项每页条目
void MainWindow::on_actionpreference_triggered()
{
    bool enableOrDisableCompress=false;
    //正在进行的阻断操作时，禁止修改设置
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再修改设置...");
        return;
    }
    //如果新增了配置项-记得修改配置读取--配置修改弹窗发起--弹窗类内修改--配置修改结果回写四处代码
    //获取当前的配置值，用于传递给设置界面
    QMap <QString,int>par;
    QMap <QString,QString> parstr;
    par.insert("compresslevel",dataCompressLevel);
    par.insert("defaultviewmode",defaultViewMode);
    par.insert("defaultnewfilemode",defaultNewFileMode);
    par.insert("defaultpagesizetype",defaultPageSizeType);
    par.insert("enablefieldcheck",checkFieldFlag?1:0);
    par.insert("standfontsize",standFontSize);
    parstr.insert("firstrarecharactersfont",firstRareCharactersFontGlobal);
    parstr.insert("firstuifont",firstUIFontGlobal);
    DialogPreference  dialog(&par,&parstr,this);
    dialog.setModal(true);
    dialog.exec();
    //获取结果
    if(dialog.getSaveFlag()){
        bool changeFlag=false;
        qDebug()<<par;
        qDebug()<<parstr;
        if(parstr.value("firstuifont")!=firstUIFontGlobal){
            firstUIFontGlobal=parstr.value("firstuifont");
            changeFlag=true;
        }
        if(parstr.value("firstrarecharactersfont")!=firstRareCharactersFontGlobal){
            firstRareCharactersFontGlobal=parstr.value("firstrarecharactersfont");
            changeFlag=true;
        }
        if(par.value("compresslevel")!=dataCompressLevel){
            if(dataCompressLevel==0&&par.value("compresslevel")!=0){
                enableOrDisableCompress=true;
            }
            if(dataCompressLevel!=0&&par.value("compresslevel")==0){
                enableOrDisableCompress=true;
            }
            changeFlag=true;
            this->dataCompressLevel=par.value("compresslevel");
        }
        if(par.value("enablefieldcheck")!=(checkFieldFlag?1:0)){
            if(par.value("enablefieldcheck")==0){
                checkFieldFlag=false;
            }
            if(par.value("enablefieldcheck")==1){
                checkFieldFlag=true;
            }
            changeFlag=true;
            //重新渲染界面元素
            if(currentOpenFileType!=openFileType::NotFileOrErr){
                //清空表格内容和加载记录
                clear_Table_Contents();
                rowHasloaded.clear();
                //数据类型插入点
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable();
                }
                else if(currentOpenFileType==openFileType::CSVFile){
                    display_CSVTable();
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable();
                }
                else if(currentOpenFileType==openFileType::DBFFile){
                    //DBF暂时不支持必填较验--无需进行任何操作
                }
            }
        }
        //字体大小
        if(par.value("standfontsize")!=standFontSize){
            standFontSize=par.value("standfontsize");
            changeFlag=true;
            setStandFontSize();
        }
        if(par.value("defaultviewmode")!=defaultViewMode){
            changeFlag=true;
            this->defaultViewMode=par.value("defaultviewmode");
            if(defaultViewMode==0){
                if(ui->frameInfo->isHidden()){
                    ui->frameInfo->setHidden(false);
                    ui->viewMode->setText("精简视图");
                }
            }
            if(defaultViewMode==1){
                if(!ui->frameInfo->isHidden()){
                    ui->frameInfo->setHidden(true);
                    ui->viewMode->setText("标准视图");
                }
            }
        }
        if(par.value("defaultnewfilemode")!=defaultNewFileMode){
            changeFlag=true;
            this->defaultNewFileMode=par.value("defaultnewfilemode");
        }
        //是否修改了分页每页页数
        if(par.value("defaultpagesizetype")!=defaultPageSizeType){
            changeFlag=true;
            this->defaultPageSizeType=par.value("defaultpagesizetype");
            //每页行数
            if(this->defaultPageSizeType==0){
                ui->labelPageSize->setText("(10万行/页)");
                this->pageRowSize=100000;
            }
            else if(this->defaultPageSizeType==1){
                ui->labelPageSize->setText("(20万行/页)");
                this->pageRowSize=200000;
            }
            else if(this->defaultPageSizeType==2){
                ui->labelPageSize->setText("(50万行/页)");
                this->pageRowSize=500000;
            }
            else if(this->defaultPageSizeType==3){
                ui->labelPageSize->setText("(100万行/页)");
                this->pageRowSize=1000000;
            }
            else if(this->defaultPageSizeType==4){
                ui->labelPageSize->setText("(200万行/页)");
                this->pageRowSize=2000000;
            }
            else{
                ui->labelPageSize->setText("(10万行/页)");
                this->pageRowSize=100000;
            }
            //如果每页条数变化了，并且当前打开了文件，需要跳到第一页-并且判断是否需要打开或者隐藏分页控件
            int rowCount=0;
            if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile){
                rowCount=commonContentQByteArrayList.count();
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowCount=dbfRowMap.count();
            }
            pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
            if(pageCount>1){
                ui->framePage->setVisible(true);
            }
            else{
                ui->framePage->setVisible(false);
            }
            statusBar_display_rowsCount(rowCount);
            currentPage=1;
            if(currentOpenFileType!=openFileType::NotFileOrErr){
                pageJump(currentPage,-2);
            }
        }
        //如果配置发生了改变,则写入配置
        if(changeFlag){
            Utils::setDefaultWindowFonts(this,true);
            Utils::setDefaultWindowFonts(statusLabel_ptr_showMessage);
            QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();
            for (auto widget = topLevelWidgets.begin(); widget != topLevelWidgets.end(); ++widget) {
                Utils::setDefaultWindowFonts(*widget,false);
            }
            QList<QWidget *> childWidgets = this->findChildren<QWidget *>();
            for (auto widget = childWidgets.begin(); widget != childWidgets.end(); ++widget) {
                Utils::setDefaultWindowFonts(*widget,false);
            }
            ptr_table->resizeColumnsToContents();
            QString Settingini=Utils::getConfigPath()+"Setting.ini";
            QSettings loadedSettingInfoIni(Settingini,QSettings::IniFormat,nullptr);
            loadedSettingInfoIni.setIniCodec("UTF-8");
            loadedSettingInfoIni.beginGroup("setting");
            loadedSettingInfoIni.setValue("compresslevel",dataCompressLevel);
            loadedSettingInfoIni.setValue("defaultviewmode",defaultViewMode);
            loadedSettingInfoIni.setValue("defaultnewfilemode",defaultNewFileMode);
            loadedSettingInfoIni.setValue("defaultpagesizetype",defaultPageSizeType);
            loadedSettingInfoIni.setValue("firstuifont",firstUIFontGlobal);
            loadedSettingInfoIni.setValue("firstrarecharactersfont",firstRareCharactersFontGlobal);
            loadedSettingInfoIni.setValue("enablefieldcheck",checkFieldFlag?"1":"0");
            loadedSettingInfoIni.setValue("standfontsize",standFontSize);
            loadedSettingInfoIni.endGroup();
            loadedSettingInfoIni.sync();
        }
        //从压缩切换到不压缩或者从不压缩切换到压缩，需要重新加载文件
        if(enableOrDisableCompress){
            if((currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::FIXEDFile)&&!currentOpenFilePath.isEmpty()){
                isUpdateData=true;
                initFile(currentOpenFilePath);
                isUpdateData=false;
                statusBar_disPlayMessage("打开或者关闭压缩功能,已重载文件...");
            }
        }
    }
}

/**
 * @brief MainWindow::on_actionnewWindow_triggered 打开新窗口的菜单槽函数
 */
void MainWindow::on_actionnewWindow_triggered()
{
    qDebug()<<"即将启动新的实例,从"<<QCoreApplication::applicationFilePath();
    QString program=QCoreApplication::applicationFilePath();
    QStringList arguments;
    arguments<<(QCoreApplication::applicationFilePath());
    QProcess::startDetached(program, arguments);
}

/**
 * @brief MainWindow::ignoreFileChangeAndOpenNewFile 决定是否忽略当前修改未保存并打开新的文件，如果允许打开或者文件压根没修改，直接返回true
 * @return
 */
//无需分页改造
bool MainWindow:: ignoreFileChangeAndOpenNewFile(){
    //判断文件是否被修改
    if(fileChanged){
        DialogMyTip dialog("当前修改的文件还未保存，要放弃保存并读取新的文件么？",this);
        dialog.setWindowTitle("警告！");
        dialog.setModal(true);
        dialog.exec();
        if(!dialog.getBoolFlag()){
            statusBar_disPlayMessage("放弃读取新文件,如果需要保存当前文件，可以使用Ctrl+S保存...");
            return false;
        }
        else{
            return true;
        }
    }
    //文件未修改--直接允许打开
    else{
        return true;
    }
}

/**
 * @brief MainWindow::closeEvent 窗口关闭事件----拦截窗口关闭，如果有未保存的文件则提醒
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event){
    if(isUpdateData||dataBlocked){
        DialogMyTip dialog("当前存在正在进行中的任务,确认要继续退出程序么？",this);
        dialog.setWindowTitle("警告！");
        dialog.setModal(true);
        dialog.exec();
        if(!dialog.getBoolFlag()){
            event->ignore();
            return;
        }
        else{
            //传递终止标志
            this->abortExit=true;
            if(dbftablefile.isOpen()){
                dbftablefile.close();
            }
            event->accept();
        }
    }
    if(fileChanged){
        DialogMyTip dialog("当前修改的文件还未保存,要放弃保存并退出工具么？",this);
        dialog.setWindowTitle("警告！");
        dialog.setModal(true);
        dialog.exec();
        if(!dialog.getBoolFlag()){
            statusBar_disPlayMessage("放弃退出工具,如果需要保存当前文件，可以使用Ctrl+S保存...");
            event->ignore();
            return;
        }
        else{
            if(dbftablefile.isOpen()){
                dbftablefile.close();
            }
            event->accept();
        }
    }
    //文件未修改--直接允许关闭程序
    else{
        if(dbftablefile.isOpen()){
            dbftablefile.close();
        }
        event->accept();
    }
}

/**
 * @brief MainWindow::on_pushButtonColumnJump_2_clicked 向右跳转比如打开的文件有100列，左右拖动很麻烦，输入这个列名字点击跳转即可
 */
void MainWindow::on_pushButtonColumnJump_2_clicked()
{
    //向右搜索
    columnJump(1);
}

/**
 * @brief MainWindow::columnJump 列左右跳转功能，供点击列左右跳转时使用
 * @param type
 */
void MainWindow::columnJump(int type){
    QString text=ui->lineTextText_2->text();
    int col=tableColSearch;
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用搜索功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //数据类型插入点
    if(currentOpenFileType==openFileType::OFDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD/类OFD文件没有数据记录...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                    ptr_table->setFocus();
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(ofd.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||ofd.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        ptr_table->setFocus();
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                    ptr_table->setFocus();
                }
                col-=1;
                for(;col>=0;col--){
                    if(ofd.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||ofd.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        ptr_table->setFocus();
                        return;
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        if(commonContentQByteArrayList.size()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                }
                col-=1;
                for(;col>=0;col--){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        return;
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(commonContentQByteArrayList.size()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(fixed.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||fixed.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                }
                col-=1;
                for(;col>=0;col--){
                    if(fixed.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||fixed.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        return;
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==0){
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的DBF文件没有数据记录...");
                return;
            }
        }
        else{
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的DBF文件在当前模式下没有数据记录...");
                return;
            }
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                }
                col-=1;
                for(;col>=0;col--){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        return;
                    }
                }
            }
        }
    }
    dataBlocked=false;
}

/**
 * @brief MainWindow::on_actionwebsite_triggered 打开我的网站
 */
void MainWindow::on_actionwebsite_triggered()
{
    QDesktopServices::openUrl(QUrl(WEBSITE_INFO));
}

/**
 * @brief MainWindow::on_actionmanual_triggered 打开在线手册
 */
void MainWindow::on_actionmanual_triggered()
{
    QDesktopServices::openUrl(QUrl(MANUAL_INFO));
}

/**
  在地址栏输入地址回车后的事件
*/
void MainWindow::on_currentOpenFilePathLineText_returnPressed()
{
    statusBar_disPlayMessage("");
    //获取当前文件路径
    QString  file = ui->currentOpenFilePathLineText->text();
    if(file.length()==0){
        statusBar_disPlayMessage("你可以复制文件路径到路径框后回车读取文件...");
        return;
    }
    //如果路径被引号包裹，处理一下
    if(file.startsWith("\"")&&file.endsWith("\"")){
        file=file.mid(1,file.length()-2);
    }
    if(file.startsWith("'")&&file.endsWith("'")){
        file=file.mid(1,file.length()-2);
    }
    //判断是否是有效的文件
    if(!Utils::isFileExist(file)){
        statusBar_disPlayMessage("["+file+"}不是一个有效的文件路径,你可以复制文件路径到路径框后回车读取文件...");
        return;
    }
    //如果没有忽略，则直接返回
    if(!ignoreFileChangeAndOpenNewFile()){
        return;
    }
    //是否正在加载数据中
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再打开新的文件...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    //标记阻断的任务和文件加载标志后开始加载文件
    dataBlocked=true;
    isUpdateData=true;
    initFile(file);
    isUpdateData=false;
    dataBlocked=false;
}

/**
 * @brief MainWindow::on_pushButtonPageFirst_clicked前往首页
 */
void MainWindow::on_pushButtonPageFirst_clicked()
{
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentPage==1){
        statusBar_disPlayMessage("当前已在第一页...");
    }
    else{
        currentPage=1;
        pageJump(currentPage,-2);
    }
}

/**
 * @brief MainWindow::on_pushButtonPagePrevious_clicked前往上一页
 */
void MainWindow::on_pushButtonPagePrevious_clicked()
{
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentPage>1){
        currentPage--;
        pageJump(currentPage,-2);
    }
    else{
        statusBar_disPlayMessage("当前已在第一页...");
    }
}

/**
 * @brief MainWindow::on_pushButtonPageNext_clicked前往下一页
 */
void MainWindow::on_pushButtonPageNext_clicked()
{
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    statusBar_disPlayMessage("");
    if(currentPage<pageCount){
        currentPage++;
        pageJump(currentPage,-2);
    }
    else{
        statusBar_disPlayMessage("当前已在最后一页...");
    }
}

/**
 * @brief MainWindow::on_pushButtonPageLast_clicked前往末页
 */
void MainWindow::on_pushButtonPageLast_clicked()
{
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentPage==pageCount){
        statusBar_disPlayMessage("当前已在最后一页...");
    }
    else{
        currentPage=pageCount;
        pageJump(currentPage,-2);
    }
}

/**
 * @brief MainWindow::on_pushButtonGo_clicked前往某一页
 */
void MainWindow::on_pushButtonGo_clicked()
{
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    statusBar_disPlayMessage("");
    bool ok=false;
    int pageNum=ui->pageText->text().toInt(&ok,10);
    if(ok){
        if(pageNum<1){
            statusBar_disPlayMessage("页码不能小于0...");
        }
        else if(pageNum>pageCount){
            statusBar_disPlayMessage(QString("当前打开的文件一共有%1页,输入的页码超过了最大值...").arg(QString::number(pageCount)));
        }
        else{
            currentPage=pageNum;
            pageJump(currentPage,-2);
        }
    }
    else{
        statusBar_disPlayMessage("请在页码输入框输入有效的页码数值后跳转...");
    }
}

/**
 * @brief MainWindow::pageJump 跳页函数
 * @param page
 * @param scrollIndex 滚动位置,-1滚动到页尾部,-2滚动到页首
 */

void MainWindow::
pageJump(int page,int scrollIndex){
    if(page>pageCount){
        statusBar_disPlayMessage("页面计算错误,请检查");
        return;
    }
    else{
        currentPage=page;
    }
    //按钮禁用逻辑
    if(page==1){
        ui->pushButtonPageFirst->setEnabled(false);
        ui->pushButtonPagePrevious->setEnabled(false);
        ui->pushButtonPageNext->setEnabled(true);
        ui->pushButtonPageLast->setEnabled(true);
    }
    else if (page==pageCount){
        ui->pushButtonPageFirst->setEnabled(true);
        ui->pushButtonPagePrevious->setEnabled(true);
        ui->pushButtonPageNext->setEnabled(false);
        ui->pushButtonPageLast->setEnabled(false);
    }
    else{
        ui->pushButtonPageFirst->setEnabled(true);
        ui->pushButtonPagePrevious->setEnabled(true);
        ui->pushButtonPageNext->setEnabled(true);
        ui->pushButtonPageLast->setEnabled(true);
    }
    //计算本页行数
    int currentPageRowSize=0;
    if(currentPage<pageCount){
        currentPageRowSize=pageRowSize;
    }
    else{
        //数据类型插入点
        if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile){
            currentPageRowSize=commonContentQByteArrayList.count()-(pageRowSize*(pageCount-1));
        }
        else if(currentOpenFileType==openFileType::DBFFile){
            currentPageRowSize=dbfRowMap.count()-(pageRowSize*(pageCount-1));
        }
    }
    //清空表格内容和加载记录
    clear_Table_Contents();
    rowHasloaded.clear();
    //重设表格行数
    ptr_table->setRowCount(currentPageRowSize);
    //显示行号
    ui->pageText->setText(QString::number(page));
    //显示描述
    statusBar_disPlayMessage(QString("第%1页,本页记录数%2行").arg(QString::number(page),QString::number(currentPageRowSize)));
    //加载本页数据
    //数据类型插入点
    if(currentOpenFileType==openFileType::OFDFile){
        display_OFDTable();
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        display_CSVTable();
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        display_FIXEDTable();
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        display_DBFTable();
    }
    //跳转位置
    if(scrollIndex==-1){
        ptr_table->scrollToBottom();
        //强制触发下刷新，避免显示数据不完整
        acceptVScrollValueChanged(0);
        tableRowCurrent=ptr_table->rowCount()-1;
        tableColCurrent=ptr_table->columnCount()-1;
    }
    else if(scrollIndex==-2){
        ptr_table->scrollToTop();
        acceptVScrollValueChanged(0);
        tableRowCurrent=0;
        tableColCurrent=0;
    }
    else if(scrollIndex<currentPageRowSize&&scrollIndex>=0){
        if(scrollIndex==currentPageRowSize-1){
            ptr_table->scrollToBottom();
        }
        else if(scrollIndex==0){
            ptr_table->scrollToTop();
        }
        ptr_table->setCurrentCell(scrollIndex,0);
        acceptVScrollValueChanged(0);
        tableRowCurrent=scrollIndex;
        tableColCurrent=0;
    }
    else if(scrollIndex>currentPageRowSize){
        ptr_table->scrollToBottom();
        ptr_table->setCurrentCell(currentPageRowSize-1,0);
        acceptVScrollValueChanged(0);
        tableRowCurrent=currentPageRowSize-1;
        tableColCurrent=0;
    }
}

bool MainWindow::dbfColIsNumber(int col){
    bool s=false;
    if(currentOpenFileType==openFileType::DBFFile){
        if(col<dbf.getFieldCount()){
            if(dbfIsNumberFieldType.contains(dbf.getFieldList().at(col).getFieldType())){
                s=true;
            }
        }
    }
    return s;
}

/**
 * @brief MainWindow::on_actiondifftools_triggered打开文件对比工具
 */
void MainWindow::on_actiondifftools_triggered()
{
    QString winmergepath=QApplication::applicationDirPath()+"/plugin/WinMerge/WinMergeU.exe";
    QStringList pars;
    if(Utils::isFileExist(winmergepath)){
        //Qt 5.6在xp系统下有个bug，调用不传参数的startDetached函数时，有可能无法启动程序,所以我们设定一个空参数调用带参数的函数
        QProcess::startDetached(winmergepath,pars);
    }else{
        statusBar_disPlayMessage("找不到WinMerge插件,请安装到plugin目录下！");
    }
}

void MainWindow::on_actionfileedit_triggered()
{
    if(currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("DBF文件不支持在文本编辑器中打开,DBF文件是二进制文件！");
        return;
    }
    QString notepad2path=QApplication::applicationDirPath()+"/plugin/Notepad2/Notepad2.exe";
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(Utils::isFileExist(notepad2path)){
        QProcess::startDetached(notepad2path,QStringList(currentOpenFilePath));
    }else{
        statusBar_disPlayMessage("找不到Notepad2插件,请安装到plugin目录下！");
    }
}

/**
 * @brief MainWindow::on_actioncreatenewofdfile_triggered 新建OFD文件
 */
void MainWindow::on_actioncreatenewofdfile_triggered()
{
    CreateOFDWindow * dialog = new CreateOFDWindow(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void MainWindow::on_actioncopy_triggered()
{
    copyToClipboard(false);
}

void MainWindow::on_actionedit_triggered()
{
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int rangeCount=itemsRange.count();
        int selectedRow=0;
        bool allSelectIsOneColumn=true;
        if(rangeCount>=1){
            selectedRow=itemsRange.at(0).bottomRow()-itemsRange.at(0).topRow()+1;
            if(itemsRange.at(0).leftColumn()!=itemsRange.at(0).rightColumn()){
                statusBar_disPlayMessage("不能同时编辑不同的列!!!");
                return;
            }
            for(int i=1;i<rangeCount;i++){
                selectedRow+=(itemsRange.at(i).bottomRow()-itemsRange.at(i).topRow()+1);
                if((itemsRange.at(i).leftColumn()!=itemsRange.at(i).rightColumn())||(itemsRange.at(i).leftColumn()!=itemsRange.at(i-1).leftColumn())){
                    allSelectIsOneColumn=false;
                    break;
                }
            }
            if(selectedRow<2){
                if(allSelectIsOneColumn){
                    showModifyCell();
                }
                else{
                    statusBar_disPlayMessage("如果要编辑整行数据，请使用编辑整行数据快捷键!!!");
                }
            }
            else {
                if(allSelectIsOneColumn){
                    showModifyCellBatch();
                }
                else{
                    statusBar_disPlayMessage("不能同时编辑多行不同的列!!!");
                }
            }
        }
    }
    else{
        statusBar_disPlayMessage("暂不支持编辑此类文件,敬请期待");
    }
}

void MainWindow::on_actionedit2_triggered()
{
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int selectRowNum=0;
        int rangeCount=itemsRange.count();
        bool allSelectIsOneRow=true;
        if(rangeCount>=1){
            if(itemsRange.at(0).topRow()!=itemsRange.at(0).bottomRow()){
                statusBar_disPlayMessage("不能同时编辑多行整行数据!!!");
                return;
            }
            selectRowNum=itemsRange.at(0).bottomRow();
            for(int i=1;i<rangeCount;i++){
                if(itemsRange.at(i).topRow()==selectRowNum&&itemsRange.at(i).bottomRow()==selectRowNum){
                    continue;
                }else{
                    allSelectIsOneRow=false;
                    break;
                }
            }
            if(allSelectIsOneRow){
                showMoaifyRow();
            }
            else {
                statusBar_disPlayMessage("不能同时编辑多行整行数据!!!");
            }
        }
    }
    else{
        statusBar_disPlayMessage("暂不支持编辑此类文件,敬请期待");
    }
}

void MainWindow::on_actionopeninexcel_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用数据导出功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    int row=0;
    if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile){
        row=commonContentQByteArrayList.count();
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        row=dbfRowMap.count();
    }
    else if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("暂不支持在excel中打开OFD/类OFD索引文件...");
        return;
    }
    else{
        return;
    }
    //弹窗提示
    DialogMyTip dialog2("确定导出当前文件为xlsx并在Excel中打开么?\r\n使用此功能将在文件原始目录生成一个同名xlsx文件\r\n在Excel中你可以进行更为细致的数据统计分析或者编辑文件后再导入本工具",this);
    dialog2.setWindowTitle("提示！");
    dialog2.setModal(true);
    dialog2.exec();
    if(dialog2.getBoolFlag()){
        //限制
        if(row>maxExcelRow){
            statusBar_disPlayMessage("记录数大于"+QString::number(maxExcelRow)+"行,无法直接使用导出到excel并打开功能,请使用数据导出功能导出当前页后再查看...");
            return;
        }
        else{
            //开始准备导出excel文件
            int first = currentOpenFilePath.lastIndexOf ("/");
            QString fileDir = currentOpenFilePath.left (first+1);
            QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
            QString fileNameNoneType=fileName;
            if(fileName.contains(".")){
                fileNameNoneType=fileNameNoneType.left(fileName.lastIndexOf("."));
            }
            QString exppath=fileDir+fileNameNoneType+".xlsx";
            //判断文件是否存在
            //覆盖导出先删除原来的文件
            if(Utils::isFileExist(exppath)){
                QFile file(exppath);
                bool r=file.remove();
                if(!r){
                    statusBar_disPlayMessage("覆盖文件失败,目标文件可能正在被占用...");
                    return;
                }
            }
            //执行excel导出，并标记导出结束时打开文件
            openXlsx=true;
            //标记阻断事件---正在导出数据
            dataBlocked=true;
            isExportFile=true;
            dataBlockedMessage="正在进行数据导出,请稍后再操作...";
            save2Xlsx(exppath,0);
        }
    }
    else{
        statusBar_disPlayMessage("放弃在Excel中打开当前接口文件...");
    }
}

/**
 * @brief MainWindow::on_actionscreen_triggered 对整个窗口截图
 */
void MainWindow::on_actionscreen_triggered()
{
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
        if(p.save(filePathName,"png"))
        {
            statusBar_disPlayMessage("截图保存在:"+filePathName);
        }
        else{
            statusBar_disPlayMessage("截图失败...");
        }
    }
    else{
        statusBar_disPlayMessage("放弃保存截图...");
    }
}

/**
 * @brief MainWindow::on_actiontipslist_triggered 查看温馨提示
 */
void MainWindow::on_actiontipslist_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再查看所有温馨提示...");
        return;
    }
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    setWindowTitle(appName);
    currentOpenFileType=openFileType::NotFileOrErr;
    currentOpenFilePath="";
    ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
    fileChanged=false;
    clear_OldData();
    clear_Display_Info();
    clear_Table_Info();
    int rowCount=tips.count();
    ptr_table->setColumnCount(1);
    ptr_table->setRowCount(rowCount);
    //设置表格列标题
    QStringList title;
    title.append("提示内容");
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ptr_table->setHorizontalHeaderLabels(title);
    //设置表格的选择方式
    ptr_table->setSelectionBehavior(tableSelectionBehavior);
    //设置编辑方式
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    //设置表格的内容
    for (int row = 0; row < tips.count(); ++row)
    {
        QTableWidgetItem *item= new QTableWidgetItem(tips.at(row));
        ptr_table->setItem(row, 0, item);
    }
    ptr_table->resizeColumnsToContents();
}

/**
 * @brief MainWindow::event 窗口事件处理
 * @param event
 * @return
 */
bool MainWindow::event(QEvent *event)
{
#ifdef Q_OS_MAC
    if(QEvent::WindowActivate == event->type()){
        qDebug()<<"获取焦点";
        ui->actionscreen->setEnabled(true);
        ui->actioncopy->setEnabled(true);
    }
    if(QEvent::WindowDeactivate == event->type()) {
        qDebug()<<"失去焦点";
        //在macOS下，弹出Dialog时快捷键还是作用在主窗口，所以这里设置主窗口失去焦点时，回收一些和弹窗冲突的快捷键
        //窗口回到焦点状态时,再启用这些快捷键
        ui->actionscreen->setEnabled(false);
        ui->actioncopy->setEnabled(false);
    }
#endif
    if(QEvent::WindowActivate == event->type()){
        if(configFileChange){
            qDebug()<<"配置已修改";
            loadConfig(true);
            configFileChange=false;
        }
    }
    return QWidget::event(event);
}


void MainWindow::loadConfig(bool isReload){
    configLoadCompleted=false;
    //清理之前加载的数据
    commonDictionary.clear();
    commonFieldTips.clear();

    ofdDictionary.clearDictionary();

    loadedOfdCodeInfo.clear();
    loadedOfdIndexFileInfo.clear();
    likeOFDIndexFilename.clear();
    likeOFDDataFilename.clear();
    ofdConfigList.clear();
    ofdQuickMatchIndex.clear();

    csvConfigList.clear();
    fixedConfigList.clear();
    dbfConfigList.clear();

    //加载基金销售商和TA代码信息
    Utils::load_OFDCodeInfo(loadedOfdCodeInfo);
    qApp->processEvents();
    //加载OFD字典
    Utils::load_OFDDictionary(ofdDictionary);
    qApp->processEvents();
    //加载OFD tips字典
    Utils::load_OFDTipDictionary(commonFieldTips);
    qApp->processEvents();
    //加载OFD文件类别信息
    Utils::load_OFDIndexFileDefinition(loadedOfdIndexFileInfo);
    qApp->processEvents();
    //加载类OFD体系的文件的文件名前缀信息
    Utils::load_OFDLikeFileDefinition(likeOFDIndexFilename,likeOFDDataFilename);
    qApp->processEvents();
    //加载各类OFD文件的定义
    Utils::load_OFDDefinition(ofdConfigList,ofdQuickMatchIndex);
    qApp->processEvents();
    //加载各类CSV文件的定义
    Utils::load_CSVDefinition(csvConfigList,commonDictionary,commonFieldTips);
    qApp->processEvents();
    //加载各类定长文件的定义
    Utils::load_FIXEDDefinition(fixedConfigList,commonDictionary,commonFieldTips);
    qApp->processEvents();
    //加载DBF配置
    Utils::load_DBFDefinition(dbfConfigList,commonDictionary,commonFieldTips);
    qApp->processEvents();
    //配置加载完毕
    configLoadCompleted=true;
    if(isReload){
        if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::OFDIndex||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
            qDebug()<<modifyConfigQStringList;
            //如果修改了当前正在打开的文件所在的ini
            if(modifyConfigQStringList.contains(ui->lineEditUseIni->text())){
                statusBar_disPlayMessage("配置文件发生了更新,F5刷新加载当前打开的文件,以使用最新的配置解析...");
                Toast::showMsg(QString("配置文件发生了更新,F5刷新加载当前打开的文件,以使用最新的配置解析..."), ToastTime::Time::ToastTime_long,ToastType::Type::ToastType_info,this);
            }
        }
    }
}

QString MainWindow::StringToHexStringWithEnCode(QString data,QTextCodec *codec,bool withSouceChar){
    QString sb;
    if(withSouceChar){
        for(int i=0;i<data.length();i++){
            //兼容4字节Unicode字符
            if(data.at(i).isHighSurrogate()&&i<data.count()-1&&data.at(i+1).isLowSurrogate())
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 99)
                sb.append("[").append(data.midRef(i,2)).append(":").append(codec->fromUnicode(data.midRef(i,2)).toHex().toUpper()).append("]");
#else
                sb.append("[").append(data.midRef(i,2)).append(":").append(codec->fromUnicode(data.mid(i,2)).toHex().toUpper()).append("]");
#endif
                i++;
                if(i<data.length()-1){
                    sb.append("-");
                }
            }
            else{
                sb.append("[").append(data.at(i)).append(":").append(codec->fromUnicode(data.at(i)).toHex().toUpper()).append("]");
                if(i<data.length()-1){
                    sb.append("-");
                }
            }
        }
    }
    else{
        sb=codec->fromUnicode(data).toHex().toUpper();
        int len = sb.length()/2;
        for(int i=1;i<len;i++)
        {
            sb.insert(2*i+i-1," ");
        }
    }
    return sb;
}

QString MainWindow::HexStringToBinaryString(QString HexString){
    QString string="";
    if(HexString.length()>0){
        for(int i=0;i<HexString.length();i++){
            QChar ch=HexString.at(i);
            if(ch=='0'){
                string.append("0000");
            }
            else if(ch=='1'){
                string.append("0001");
            }
            else if(ch=='2'){
                string.append("0010");
            }
            else if(ch=='3'){
                string.append("0011");
            }
            else if(ch=='4'){
                string.append("0100");
            }
            else if(ch=='5'){
                string.append("0101");
            }
            else if(ch=='6'){
                string.append("0110");
            }
            else if(ch=='7'){
                string.append("0111");
            }
            else if(ch=='8'){
                string.append("1000");
            }
            else if(ch=='9'){
                string.append("1001");
            }
            else if(ch=='A'){
                string.append("1010");
            }
            else if(ch=='B'){
                string.append("1011");
            }
            else if(ch=='C'){
                string.append("1100");
            }
            else if(ch=='D'){
                string.append("1101");
            }
            else if(ch=='E'){
                string.append("1110");
            }
            else if(ch=='F'){
                string.append("1111");
            }
            else{
                return "ERROR";
            }
        }
    }
    return string;
}

void MainWindow::on_actionmtime_triggered()
{
    //打开窗口
    DialogModifyMtime * dialog = new DialogModifyMtime(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

bool MainWindow::compareOFDData(const OFDFileDefinition &ofd1, const OFDFileDefinition &ofd2){
    if (ofd1.getConfigSegment() < ofd2.getConfigSegment())
    {
        return true;
    }
    else{
        return false;
    }
}

bool MainWindow::compareDBFData(const dbfMatchInfo &dbf1, const dbfMatchInfo &dbf2){
    if (dbf1.matchDegree > dbf2.matchDegree)
    {
        return true;
    }
    else{
        return false;
    }
}


void MainWindow::on_actioneditheaderfooter_triggered()
{
    if(currentOpenFileType==openFileType::OFDFile){
        QList<QStringList> header;
        QList<QStringList> footer;
        QStringList row1;
        row1.append("1");
        row1.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(0):"");
        row1.append("文件标识");
        header.append(row1);

        QStringList row2;
        row2.append("2");
        row2.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(1):"");
        row2.append("文件版本号");
        header.append(row2);

        QStringList row3;
        row3.append("3");
        row3.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(2):"");
        row3.append("文件创建人");
        header.append(row3);

        QStringList row4;
        row4.append("4");
        row4.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(3):"");
        row4.append("文件接收人");
        header.append(row4);

        QStringList row5;
        row5.append("5");
        row5.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(4):"");
        row5.append("日期");
        header.append(row5);

        QStringList row6;
        row6.append("6");
        row6.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(5):"");
        row6.append("汇总表号");
        header.append(row6);

        QStringList row7;
        row7.append("7");
        row7.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(6):"");
        row7.append("文件类型码");
        header.append(row7);

        QStringList row8;
        row8.append("8");
        row8.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(7):"");
        row8.append("发送人");
        header.append(row8);

        QStringList row9;
        row9.append("9");
        row9.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(8):"");
        row9.append("接收人");
        header.append(row9);

        QStringList row10;
        row10.append("10");
        row10.append(commonHeaderQStringList.count()>=10?commonHeaderQStringList.at(9):"");
        row10.append("字段数");
        header.append(row10);

        //字段信息和文件记录数信息
        if(commonHeaderQStringList.count()>10){
            for(int hn=10;hn<commonHeaderQStringList.count();hn++){
                QStringList rowdata;
                rowdata.append(QString::number(hn+1));
                rowdata.append(commonHeaderQStringList.at(hn));
                //文件记录数
                if(hn==commonHeaderQStringList.count()-1){
                    rowdata.append("文件记录数");
                }
                //字段名
                else if(hn<commonHeaderQStringList.count()-1&&ofd.getFieldList().count()>hn-10){
                    rowdata.append(ofd.getFieldList().at(hn-10).getFieldDescribe());
                }
                else {
                    rowdata.append("");
                }
                //添加到文件头
                header.append(rowdata);
            }
        }

        QStringList rowend;
        if(!ofdFooterQString.isEmpty()){
            rowend.append(QString::number(ofdFooterRowNumber));
            rowend.append(ofdFooterQString);
            rowend.append("文件结束标识");
        }
        else{
            rowend.append(QString::number(ofdFooterRowNumber));
            rowend.append("OFDCFEND");
            rowend.append("文件结束标识缺失,自动设置为OFDCFEND");
        }
        footer.append(rowend);

        //打开窗口
        DialogEditHeaderFooter * dialog = new DialogEditHeaderFooter(&ofd,header,footer,this);
        connect(dialog,&DialogEditHeaderFooter::sendNewHeaderAndFooter,this,&MainWindow::getNewHeaderAndFooter);
        dialog->setModal(true);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

void MainWindow::showQrcode(){
    //获取数据
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    QString text="";
    int rangeCount=itemsRange.count();
    if(rangeCount>0){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个-只取值
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
            }
        }
        //多个
        else{
            //先判断选择的单元格数据
            if((bottomRow-topRow+1)*(rigthColumn-leftColumn+1)>120){
                statusBar_disPlayMessage("数据太多了,生成二维码的数据限制最多120个单元格且不大于2300字节");
                return;
            }else{
                //判断是跨行单列还是跨列单行
                //同一行的一些列--行列交换展示
                if((bottomRow==topRow)&&(rigthColumn!=leftColumn)){
                    for(int i=leftColumn;i<=rigthColumn;i++){
                        if(ptr_table->item(tableRowCurrent,i)==nullptr){
                            text.append(ptr_table->horizontalHeaderItem(i)->text());
                        }
                        else{
                            text.append(ptr_table->horizontalHeaderItem(i)->text()).append(" ").append(ptr_table->item(topRow,i)->text());
                        }
                        if(i<rigthColumn){
                            text.append("\r\n");
                        }
                    }
                }
                //同一列的多行-不要标题直接一行有一行
                else if((bottomRow!=topRow)&&(rigthColumn==leftColumn)){
                    int rowRealInContent=0;
                    if(currentOpenFileType==openFileType::OFDFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QString value=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,leftColumn);
                            if(ofd.getFieldList().at(leftColumn).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                    else if(currentOpenFileType==openFileType::CSVFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QStringList line=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                            QString value=line.count()>leftColumn?line.at(leftColumn):"";
                            if(CsvFieldIsNumberOrNot.contains(leftColumn)&&CsvFieldIsNumberOrNot.value(leftColumn).getIsNumber()){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                    else if(currentOpenFileType==openFileType::FIXEDFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QString value=Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,leftColumn);
                            if(fixed.getFieldList().at(leftColumn).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                    else if(currentOpenFileType==openFileType::DBFFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QString value=Utils::getFormatValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,leftColumn,&dbfRowMap,dbfTrimType);
                            if(dbfColIsNumber(leftColumn)){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                }
                //多行多列
                else{
                    int rowRealInContent=0;
                    //标题行
                    for(int i=leftColumn;i<=rigthColumn;i++){
                        if(i<rigthColumn){
                            text.append(ptr_table->horizontalHeaderItem(i)->text()).append(" ");
                        }
                        else{
                            text.append(ptr_table->horizontalHeaderItem(i)->text()).append("\r\n");
                        }
                    }
                    //数据
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        QStringList line;
                        if(currentOpenFileType==openFileType::OFDFile){
                            line=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::CSVFile){
                            line=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            line=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::DBFFile){
                            line=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType);
                        }
                        for(int i=leftColumn;i<=rigthColumn&&i<line.count();i++){
                            QString value=line.at(i);
                            if (value.isEmpty()){
                                text.append("");
                            }
                            else if(currentOpenFileType==openFileType::OFDFile&&ofd.getFieldList().at(i).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else if(currentOpenFileType==openFileType::CSVFile&&CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }else if(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldList().at(i).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else if(currentOpenFileType==openFileType::DBFFile&&dbfColIsNumber(i)){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            //分割
                            if(i<rigthColumn&&(i<line.count()-1)){
                                text.append(" ");
                            }
                            //换行
                            else if((i==rigthColumn&&row<bottomRow)||((i==line.count()-1)&&row<bottomRow)){
                                text.append("\r\n");
                            }
                        }
                    }
                }
            }
        }
    }
    QString trimd=QString(text).replace("\r\n","").replace(" ","");
    if(!trimd.isEmpty()){
        if(text.toUtf8().count()>2300){
            statusBar_disPlayMessage(QString("数据太多了,生成二维码的数据限制最多120个单元格且不大于2300字节,当前%1字节").arg(QString::number(text.toUtf8().count())));
        }
        else{
            //打开窗口
            DialogShareQrCode * dialog = new DialogShareQrCode(text,this);
            dialog->setWindowTitle(QString("二维码分享数据-请扫一扫获得数据"));
            dialog->setModal(false);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
            dialog->raise();
            dialog->activateWindow();
        }
    }
    else{
        statusBar_disPlayMessage("没有有效数据可供生成二维码,数据为空或者全是空格!!!");
    }
}

void MainWindow::on_actionalldata_triggered()
{
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==0){
            return;
        }
        if(isUpdateData){
            statusBar_disPlayMessage("正在加载数据,请稍后再试...");
            return;
        }
        if(dataBlocked){
            statusBar_disPlayMessage(dataBlockedMessage);
            return;
        }
        dbfDisplayType=0;
        if(!currentOpenFilePath.isEmpty()){
            isUpdateData=true;
            initFile(currentOpenFilePath,true,true);
            isUpdateData=false;
        }
    }
}

void MainWindow::on_actionnotdelete_triggered()
{
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==1){
            return;
        }
        if(isUpdateData){
            statusBar_disPlayMessage("正在加载数据,请稍后再试...");
            return;
        }
        if(dataBlocked){
            statusBar_disPlayMessage(dataBlockedMessage);
            return;
        }
        dbfDisplayType=1;
        if(!currentOpenFilePath.isEmpty()){
            isUpdateData=true;
            initFile(currentOpenFilePath,true,true);
            isUpdateData=false;
        }
    }
}

void MainWindow::on_actionjustdelete_triggered()
{
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==2){
            return;
        }
        if(isUpdateData){
            statusBar_disPlayMessage("正在加载数据,请稍后再试...");
            return;
        }
        if(dataBlocked){
            statusBar_disPlayMessage(dataBlockedMessage);
            return;
        }
        dbfDisplayType=2;
        if(!currentOpenFilePath.isEmpty()){
            isUpdateData=true;
            initFile(currentOpenFilePath,true,true);
            isUpdateData=false;
        }
    }
}

void MainWindow::on_actionGB18030_GB2312_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::GB18030);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::GB18030);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionIBM437_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::IBM437);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::IBM437);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionIBM850_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::IBM850);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::IBM850);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionIBM866_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::IBM866);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::IBM866);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionWindows1250_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::Windows1250);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::Windows1250);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionWindows1251_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::Windows1251);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::Windows1251);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionWindows1252_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::Windows1252);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::Windows1252);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionBig5_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::Big5);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::Big5);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionShift_JIS_triggered()
{
    if(dbftablefile.isOpen()){
        dbftablefile.setCodepage(QDbf::QDbfTable::Shift_JIS);
        dbftablefile.setDefaultCodepage(QDbf::QDbfTable::Shift_JIS);
        //重新加载数据
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actioncloseFile_triggered()
{
    currentOpenFileType=openFileType::NotFileOrErr;
    currentOpenFilePath="";
    ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
    fileChanged=false;
    clear_OldData();
    clear_Display_Info();
    clear_Table_Info();
    setWindowTitle(appName);
    if(currentOpenFileType==openFileType::DBFFile){
        if (dbftablefile.isOpen()){
            dbftablefile.close();
        }
    }
}

void MainWindow::on_actionautcolwidth_triggered()
{
    ptr_table->resizeColumnsToContents();
}

void MainWindow::on_actionleftright_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再刷新...");
        return;
    }
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        ui->actionleftright->setEnabled(false);
        ui->actionleft->setEnabled(true);
        ui->actionright->setEnabled(true);
        dbfTrimType=0;
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionleft_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再刷新...");
        return;
    }
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        ui->actionleftright->setEnabled(true);
        ui->actionleft->setEnabled(false);
        ui->actionright->setEnabled(true);
        dbfTrimType=1;
        rowHasloaded.clear();
        display_DBFTable();
    }
}

void MainWindow::on_actionright_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再刷新...");
        return;
    }
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        ui->actionleftright->setEnabled(true);
        ui->actionleft->setEnabled(true);
        ui->actionright->setEnabled(false);
        dbfTrimType=2;
        rowHasloaded.clear();
        display_DBFTable();
    }
}


void MainWindow::on_actionOKTools_triggered()
{
    DialogOKTools * dialog = new DialogOKTools(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void MainWindow::on_actionimportfromexcel_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        DialogMyTip dialog2("请确认是否从Excel导入数据到当前打开的OFD/类OFD/字段定长文件，此功能将会覆盖当前文件的数据,请注意备份，导入的Excel第一行的标题必须和本文件一致，建议使用本程序导出xlsx文件编辑后再导入！\r\n数值型数据将会按照字段小数长度四舍五入截取,建议在Excel中按数值存储精确数据,以免造成数据错误,本程序不支持公式生成的数值字段!",this);
        dialog2.setWindowTitle("警告-从xlsx文件导入数据！");
        dialog2.setModal(true);
        dialog2.exec();
        if(dialog2.getBoolFlag()){
            QString openpath=QFileInfo(currentOpenFilePath).absoluteFilePath();
            if(!currentOpenFilePath.isEmpty()){
                if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
                    openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
                }
            }
            fromExcelImportFilePath = QFileDialog::getOpenFileName(this, tr("打开"),openpath , "Excel文件(*.xlsx)");
            if(fromExcelImportFilePath.length() != 0) {
                statusBar_disPlayMessage("正在分析校验Excel文件，请耐心等待...");
                auto future = QtConcurrent::run(this, &MainWindow::importFromExcel);
                watcherXlsxImportStatus_->setFuture(future);
            } else {
                return;
            }
        }
        else{
            return;
        }
    }
    else{
        statusBar_disPlayMessage("打开一个OFD/类OFD/字段定长文件,然后才可以导入此文件对应的Excel数据(字段中文名一样)...");
    }
}

int MainWindow::importFromExcel(){
    dataBlocked=true;
    importExcelErrorDetail.clear();
    fileContentQByteArrayListFromExcel.clear();
    dataBlockedMessage="正在装载Excel文件,请耐心等待...";
    if (Utils::isFileExist(fromExcelImportFilePath)){
        QXlsx::Document xlsxread(fromExcelImportFilePath);
        QTextCodec *codec=QTextCodec::codecForName("GB18030");
        QList<fieldType> fieldTypeList;
        int colCount=0;
        int rowLength=0;
        QList <int> fixedFileFieldCountList;
        if(currentOpenFileType==openFileType::OFDFile){
            colCount=ofd.getFieldCount();
            codec=codecOFD;
            rowLength=ofd.getRowLength();
            for(int i=0;i<ofd.getFieldList().count();i++){
                fieldType item;
                item.fieldType=ofd.getFieldList().at(i).getFieldType();
                item.fieldLength=ofd.getFieldList().at(i).getLength();
                item.fieldDecLength=ofd.getFieldList().at(i).getDecLength();
                item.fieldName=ofd.getFieldList().at(i).getFieldName();
                item.fieldDescribe=ofd.getFieldList().at(i).getFieldDescribe();
                item.fieldBeginIndex=ofd.getFieldList().at(i).getRowBeginIndex();
                fieldTypeList.append(item);
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            colCount=fixed.getFieldCountMax();
            codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            fixedFileFieldCountList=fixed.getRowLengthHash().values();
            for(int i=0;i<fixed.getFieldList().count();i++){
                fieldType item;
                item.fieldType=fixed.getFieldList().at(i).getFieldType();
                item.fieldLength=fixed.getFieldList().at(i).getLength();
                rowLength+=item.fieldLength;
                item.fieldDecLength=fixed.getFieldList().at(i).getDecLength();
                item.fieldName=fixed.getFieldList().at(i).getFieldName();
                item.fieldDescribe=fixed.getFieldList().at(i).getFieldDescribe();
                item.fieldBeginIndex=fixed.getFieldList().at(i).getRowBeginIndex();
                fieldTypeList.append(item);
            }
        }
        if(xlsxread.load()){
            for(int col=1;col<=colCount;col++){
                if(currentOpenFileType==openFileType::OFDFile){
                    if(xlsxread.read(1,col).toString().replace("（","(").replace("）",")")!=ofd.getFieldList().at(col-1).getFieldDescribe().replace("（","(").replace("）",")")){
                        importExcelErrorDetail.append("选择的文件第"+Utils::CovertInt2ExcelCol(col-1)+"列不是["+ofd.getFieldList().at(col-1).getFieldDescribe()+"],请确认你导入的文件第一行标题和当前打开的文件列名一致,你可以使用数据导出功能导出一份Excel文件后编辑导入！");
                        return 1;
                    }
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    if(xlsxread.read(1,col).toString().replace("（","(").replace("）",")")!=fixed.getFieldList().at(col-1).getFieldDescribe().replace("（","(").replace("）",")")){
                        importExcelErrorDetail.append("选择的文件第"+Utils::CovertInt2ExcelCol(col-1)+"列不是["+fixed.getFieldList().at(col-1).getFieldDescribe()+"],请确认你导入的文件第一行标题和当前打开的文件列名一致,你可以使用数据导出功能导出一份Excel文件后编辑导入！");
                        return 1;
                    }
                }
            }
            QByteArray byteArrayRowT;
            for(int charIndex=0;charIndex<rowLength;charIndex++){
                byteArrayRowT.append(QChar(' ').toLatin1());
            }
            importExcelRow=0;
            int row=2;
            bool allColIsEmpty=true;
            while (true){
                QStringList rowdata;
                allColIsEmpty=true;
                for(int col=1;col<=colCount;col++){
                    std::string filedDecLengthString=QString("%.%1f").arg(fieldTypeList.at(col-1).fieldDecLength).toStdString();
                    QVariant colvalQVariant=xlsxread.read(row,col);
                    QString colval="";
                    if(colvalQVariant.type() == QVariant::Double){
                        if(fieldTypeList.at(col-1).fieldType=="N"){
                            colval=QString::asprintf(filedDecLengthString.c_str(),colvalQVariant.toDouble());
                        }
                        else{
                            colval=QString::asprintf("%.0f",colvalQVariant.toDouble());
                        }
                    }
                    else if(colvalQVariant.type() == QVariant::Int){
                        colval=QString::asprintf("%d",colvalQVariant.toInt());
                    }
                    else if(colvalQVariant.type() == QVariant::UInt){
                        colval=QString::asprintf("%d",colvalQVariant.toUInt());
                    }
                    else if(colvalQVariant.type() == QVariant::LongLong){
                        colval=QString::asprintf("%lld",colvalQVariant.toLongLong());
                    }
                    else if(colvalQVariant.type() == QVariant::ULongLong){
                        colval=QString::asprintf("%llu",colvalQVariant.toULongLong());
                    }
                    else{
                        if(fieldTypeList.at(col-1).fieldType=="N"){
                            bool ok=false;
                            colvalQVariant.toDouble(&ok);
                            if(ok){
                                colval=QString::asprintf(filedDecLengthString.c_str(),colvalQVariant.toDouble());
                            }
                            else{
                                colval=colvalQVariant.toString();
                            }
                        }else{
                            colval=colvalQVariant.toString();
                        }
                    }
                    if(!colval.isEmpty()){
                        allColIsEmpty=false;
                    }
                    int textLength=codec->fromUnicode(colval).length();
                    if(fieldTypeList.at(col-1).fieldType=="C"||fieldTypeList.at(col-1).fieldType=="TEXT"){
                        if(textLength>fieldTypeList.at(col-1).fieldLength){
                            importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]长度不应当大于[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength)));
                        }
                    }
                    else if(fieldTypeList.at(col-1).fieldType=="A"){
                        if(textLength>fieldTypeList.at(col-1).fieldLength){
                            importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]长度不应当大于[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength)));
                        }
                    }
                    else if(fieldTypeList.at(col-1).fieldType=="N"){
                        if(!colval.isEmpty()){
                            bool ok=false;
                            colval.toDouble(&ok);
                            if(!ok){
                                importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]应当是一个数值或者为空,但是现在看起来并不是").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe));
                            }
                            else{
                                if(!colval.contains("."))
                                {
                                    if(textLength>(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)){
                                        importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]整数部分长度不能超过[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)));
                                    }
                                }
                                else{
                                    int index=colval.indexOf(".");
                                    QString intS=colval.mid(0,index);
                                    if(intS.length()>(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)){
                                        importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]的整数部分长度不能超过[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)));
                                    }
                                    else {
                                        QString intD=colval.mid(index+1,-1);
                                        if(intD.length()>fieldTypeList.at(col-1).fieldDecLength){
                                            colval=colval.mid(0,index+fieldTypeList.at(col-1).fieldDecLength+1);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else{
                        if(textLength>fieldTypeList.at(col-1).fieldLength){
                            importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]的长度不应当大于[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength)));
                        }
                    }
                    rowdata.append(colval);
                }
                if(allColIsEmpty){
                    break;
                }
                else if (importExcelErrorDetail.count()==0){
                    QByteArray byteArrayRow=byteArrayRowT;
                    if(rowdata.count()>0){
                        //多种长度情境下自适应行长度
                        if(currentOpenFileType==openFileType::FIXEDFile&&fixed.getRowLengthHash().count()>1){
                            int rowCountNew=rowdata.count();
                            for(int iii=rowdata.count()-1;iii>0;iii--){if(rowdata.at(iii).isEmpty()){rowCountNew--;}else{break;}}
                            if (rowCountNew<colCount&&!fixedFileFieldCountList.contains(rowCountNew)){while(true){rowCountNew++;if(fixedFileFieldCountList.contains(rowCountNew)){break;}}}
                            if(rowCountNew<colCount){rowdata=rowdata.mid(0,rowCountNew);byteArrayRow=byteArrayRow.mid(0,fixed.getFieldList().at(rowdata.count()).getRowBeginIndex());}
                        }
                        for(int editCol=0;editCol<rowdata.count()&&editCol<colCount;editCol++){
                            QString valueNew=rowdata.at(editCol);
                            bool flag=Utils::updateOFDOrFixedFieldValueFromRow(fieldTypeList.at(editCol).fieldType,fieldTypeList.at(editCol).fieldLength,fieldTypeList.at(editCol).fieldDecLength,fieldTypeList.at(editCol).fieldBeginIndex,0,codec,valueNew,byteArrayRow);
                            if(!flag){
                                importExcelErrorDetail.append(QString("Excel第[%1]行数据导入时生成行数据异常,请重试...").arg(QString::number(row)));
                                break;
                            }
                        }
                    }
                    if(dataCompressLevel==0){
                        fileContentQByteArrayListFromExcel.append(byteArrayRow);
                    }
                    else{
                        fileContentQByteArrayListFromExcel.append(qCompress(byteArrayRow,dataCompressLevel));
                    }
                    if (importExcelRow>0&&importExcelRow%100==0){
                        emit signals_update_import_excel_status();
                    }
                }
                row++;
                importExcelRow++;
                if(row>1000000){
                    fileContentQByteArrayListFromExcel.clear();
                    importExcelErrorDetail.clear();
                    importExcelErrorDetail.append(QString("最多可以导入1,000,000行的Excel,看起来目前Excel数据太多了..."));
                    return 1;
                }
            }
        }
        else{
            importExcelErrorDetail.append("加载Excel失败,请尝试重试或者确认文件是可读取的...");
        }
    }
    else{
        importExcelErrorDetail.append("加载Excel失败,请尝试重试或者确认文件是可读取的...");
    }
    return 1;
}

/**
 * @brief MainWindow::importFromXlsxFinished 导入excel结束
 */
void MainWindow::importFromXlsxFinished(){
    //显示错误
    if (importExcelErrorDetail.count()>0){
        if(importExcelErrorDetail.count()==1){
            statusBar_disPlayMessage("导入失败,请查看具体失败原因...");
            DialogMyTip dialog2(importExcelErrorDetail.at(0),this,true);
            dialog2.setWindowTitle("导入失败警告！");
            dialog2.setModal(true);
            dialog2.exec();
        }
        //多条-展示为表格
        else{
            statusBar_disPlayMessage("导入失败,请查看具体失败原因...");
            DialogShowImportExcelError dialog2(&importExcelErrorDetail,this);
            dialog2.setWindowTitle("Excel导入失败详情");
            dialog2.setModal(true);
            dialog2.exec();
        }
        dataBlocked=false;
    }
    //导入数据
    else{
        dataBlocked=false;
        int rowCount=fileContentQByteArrayListFromExcel.count();
        if(rowCount==0){
            statusBar_disPlayMessage("导入的Excel有效数据行数为0...");
        }
        else{
            commonContentQByteArrayList=fileContentQByteArrayListFromExcel;
            fileContentQByteArrayListFromExcel.clear();
            reCalculateFileCountLine();
            int rowCount=commonContentQByteArrayList.count();
            clear_Table_Contents();
            ptr_table->setRowCount(rowCount);
            //添加完数据没有引发分页-原来也就1页
            if(rowCount<=pageRowSize){
                //关闭分页组件
                ui->framePage->setVisible(false);
                pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
                currentPage=1;
                fileChanged=true;
                rowHasloaded.clear();
                compareData.clear();
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable();
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable();
                }
            }
            //添加完数据数据大于1页，或者原本数据就大于1页-需要判断跳转的位置
            else{
                //展示分页组件
                ui->framePage->setVisible(true);
                statusBar_display_rowsCount(rowCount);
                //统计新的页数
                pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
                fileChanged=true;
                rowHasloaded.clear();
                compareData.clear();
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable();
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable();
                }
            }
            acceptVScrollValueChanged(0);
            statusBar_display_rowsCount(rowCount);
            tableRowCurrent=0;
            tableColCurrent=0;
            this->setWindowTitle(appName+"-"+currentFileName+"-导入数据待保存");
            if(dialogDialogShowPrimaryCheck!=nullptr){
                needReCheckPrimary=true;
                statusBar_disPlayMessage(QString("导入完毕,从Excel导入数据%1行,请重新进行主键冲突检查...").arg(QString::number(rowCount)));
            }
            else{
                statusBar_disPlayMessage(QString("导入完毕,从Excel导入数据%1行").arg(QString::number(rowCount)));
            }
        }
    }
}

void MainWindow::extractZipFinished(){
    dataBlocked=false;
    if(zipExtractSucess){
        fileFromZip=true;
        initFile(zipOutPutFileDir+zipTargetFileInZip,false,false,true);
    }
    else{
        zipfilePath="";
        zipTargetFileInZip="";
        zipExtractAll=false;
        zipExtractSucess=false;
        zipOutPutFileDir="";
        statusBar_disPlayMessage(zipExtractMessage);
    }
}
/**
 * @brief MainWindow::update_import_excel_status 刷新UI
 */
void MainWindow::update_import_excel_status(){
    statusBar_disPlayMessage(QString("已分析数据%1行,请耐心等待...").arg(QString::number(importExcelRow+1)));
}

void MainWindow::update_zip_extract_status(){
    statusBar_disPlayMessage(QString("正在解压文件,共计%1个,已完成解压%2个,请耐心等待...").arg(QString::number(zipFileInfoList.count()),QString::number(zipFileCurrentUnzipIndex)));
}

void MainWindow::openPlugin(){
#ifdef Q_OS_WIN32
    QObject *object = QObject::sender();
    qDebug()<<"准备打开插件"<<object->objectName();
    QString pluginName=object->objectName().split("|").at(0);
    int pluginIndex=object->objectName().split("|").at(1).toInt();
    FormWebTools *web=new FormWebTools(pluginName,QApplication::applicationDirPath()+"/plugin-web/"+pluginpath.value(pluginIndex));
    web->setAttribute(Qt::WA_DeleteOnClose);
    web->show();
    web->raise();
    web->activateWindow();
#endif
}

/**
 * @brief MainWindow::getNewHeaderAndFooter 从弹窗中获取文件头文件尾编辑结果
 * @param header
 * @param footer
 */
void MainWindow::getNewHeaderAndFooter(QStringList header,QStringList footer){
    //    qDebug()<<header;
    //    qDebug()<<footer;
    if(currentOpenFileType==openFileType::OFDFile){
        if(header.count()!=commonHeaderQStringList.count()){
            statusBar_disPlayMessage("文件头读取异常,跳过更新文件头文件尾");
            return;
        }
        else{
            bool modify=false;
            if(header.count()>0){
                for (int i=0;i<header.count();i++){
                    if(0!=header.at(i).compare(commonHeaderQStringList.at(i),Qt::CaseSensitive)){
                        commonHeaderQStringList.replace(i,header.at(i));
                        modify=true;
                    }
                }
            }
            if(footer.count()>0){
                if(0!=footer.at(0).compare(ofdFooterQString,Qt::CaseSensitive)){
                    ofdFooterQString=footer.at(0);
                    modify=true;
                }
            }
            if(modify){
                statusBar_disPlayMessage("已修改文件头文件尾,请保存文件!");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                fileChanged=true;
            }
            else{
                statusBar_disPlayMessage("文件头文件尾未修改...");
            }
        }
    }
}

void MainWindow::on_actionexpcheckresult_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    //数据类型校验
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    else if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    else if(currentOpenFileType==openFileType::OFDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
        if(ofd.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
        if(csv.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
        if(fixed.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("不支持DBF文件使用此功能...");
        return;
    }
    //选择导出模式
    DialogChooseFieldCheckExportReport * dialog222 = new DialogChooseFieldCheckExportReport(this);
    connect(dialog222,&DialogChooseFieldCheckExportReport::sendFieldExportConfig,this,&MainWindow::getFieldExportConfig);
    dialog222->setModal(true);
    dialog222->setAttribute(Qt::WA_DeleteOnClose);
    dialog222->show();
    dialog222->raise();
    dialog222->activateWindow();
}

void MainWindow::on_actioncurrentfilechekrule_triggered()
{
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("此功能暂不支持dbf文件...");
        return;
    }
    QList<QStringList> data;
    if(currentOpenFileType==openFileType::OFDFile){
        data=Utils::parseCheckItemListtoChineseList(ofd);
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        data=Utils::parseCheckItemListtoChineseList(fixed);
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        data=Utils::parseCheckItemListtoChineseList(csv);
    }
    if(data.count()==0){
        statusBar_disPlayMessage("当前使用的解析规则下没有必填较验规则...");
    }else{
        statusBar_disPlayMessage(QString("加载%1个必填检查规则").arg(QString::number(data.count())));
        DialogShowFieldCheckList * dialog = new DialogShowFieldCheckList(&data,this);
        dialog->setModal(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

void MainWindow::checkRowFieldResult (QStringList & rowdata,QMap<int,QString> & checkresult,int splitFlag){
    switch (currentOpenFileType){
    case openFileType::OFDFile:
    {
        foreach (fieldcheckitem item, ofd.getFieldcheckList()) {
            //条件分组--或者关系
            foreach (auto cclist,item.getCheckConditionsListOverList()){
                //本分组内条件清单--和关系--必须全部满足
                QString cond="";
                bool needshi=true;
                bool thisconflag=true;
                /////////////////////////////条件检查//////////////////
                if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                    cond.append("本文件中");
                    needshi=false;
                }
                else{
                    needshi=true;
                    foreach(auto cc,cclist){
                        if(rowdata.count()>=cc.index+1&&rowdata.at(cc.index)==cc.value){
                            if(cond.isEmpty()){
                                cond.append(ofd.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            else{
                                cond.append(",并且").append(ofd.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            continue;
                        }
                        //一个条件不满足就不检查了
                        else{
                            thisconflag=false;
                            break;
                        }
                    }
                }
                /////////////////////////////条件检查//////////////////
                //命中本规则,开始构建规则检查结果
                if(thisconflag){
                    foreach(auto xxx,item.getRequiredFields()){
                        if(rowdata.at(xxx).isEmpty()){
                            if(checkresult.contains(xxx)){
                                if(needshi){
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append("时,").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append(",").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                            else{
                                if(needshi){
                                    checkresult.insert(xxx,QString(cond).append("时,").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(cond).append(",").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
        break;
    case openFileType::FIXEDFile:
    {
        QList<fieldcheckitem> fixedFileDefinition=fixed.getFieldcheckList();
        for (auto it = fixedFileDefinition.begin(); it != fixedFileDefinition.end(); ++it) {
            fieldcheckitem item=*it;
            //条件分组--或者关系
            foreach (auto cclist,item.getCheckConditionsListOverList()){
                //本分组内条件清单--和关系--必须全部满足
                QString cond="";
                bool needshi=true;
                bool thisconflag=true;
                if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                    cond.append("本文件中");
                    needshi=false;
                }
                else{
                    needshi=true;
                    foreach(auto cc,cclist){
                        if(rowdata.count()>=cc.index+1&&rowdata.at(cc.index)==cc.value){
                            if(cond.isEmpty()){
                                cond.append(fixed.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            else{
                                cond.append(",并且").append(fixed.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            continue;
                        }
                        //一个条件不满足就不检查了
                        else{
                            thisconflag=false;
                            break;
                        }
                    }
                }
                //命中本规则,开始构建规则检查结果
                if(thisconflag){
                    foreach(auto xxx,item.getRequiredFields()){
                        if(rowdata.at(xxx).isEmpty()){
                            if(checkresult.contains(xxx)){
                                if(needshi){
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append("时,").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append(",").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                            else{
                                if(needshi){
                                    checkresult.insert(xxx,QString(cond).append("时,").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(cond).append(",").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
        break;
    case openFileType::CSVFile:
    {
        QList<fieldcheckitem> csvFileDefinition=csv.getFieldcheckList();
        for (auto it = csvFileDefinition.begin(); it != csvFileDefinition.end(); ++it) {
            fieldcheckitem item=*it;
            //条件分组--或者关系
            foreach (auto cclist,item.getCheckConditionsListOverList()){
                //本分组内条件清单--和关系--必须全部满足
                QString cond="";
                bool needshi=true;
                bool thisconflag=true;
                if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                    cond.append("本文件中");
                    needshi=false;
                }
                else{
                    needshi=true;
                    foreach(auto cc,cclist){
                        if(rowdata.count()>=cc.index+1&&rowdata.at(cc.index)==cc.value){
                            if(cond.isEmpty()){
                                cond.append(csv.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            else{
                                cond.append(",并且").append(csv.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            continue;
                        }
                        //一个条件不满足就不检查了
                        else{
                            thisconflag=false;
                            break;
                        }
                    }
                }
                //命中本规则,开始构建规则检查结果
                if(thisconflag){
                    foreach(auto xxx,item.getRequiredFields()){
                        if(rowdata.at(xxx).isEmpty()){
                            if(checkresult.contains(xxx)){
                                if(needshi){
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append("时,").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append(",").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                            else{
                                if(needshi){
                                    checkresult.insert(xxx,QString(cond).append("时,").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(cond).append(",").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::on_actionprefieldcheck_triggered()
{
    checkRowFieldSearch(0);
}


void MainWindow::on_actionnextfieldcheck_triggered()
{
    checkRowFieldSearch(1);
}

void MainWindow::checkRowFieldSearch (int direction){
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    //数据类型校验
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    else if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    else if(currentOpenFileType==openFileType::OFDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
        if(ofd.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
        if(csv.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
        if(fixed.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("不支持DBF文件使用此功能...");
        return;
    }
    if (direction!=0&&direction!=1){
        return;
    }
    if(!checkFieldFlag){
        statusBar_disPlayMessage("必填检测功能已被禁用,请开启...");
        return;
    }
    //准备工作
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    dataBlockedMessage="正在搜索字段必填缺失,请稍后再操作...";
    int rowRealInContent=0;
    //焦点行从焦点单元格上一个开始搜索,非焦点行从最后一个
    int beginCol=tableColCurrent;
    int beginRow=tableRowCurrent;
    /////////////////////////////////////////////////
    if (direction==0){
        if(currentPage==1&&beginRow==0&&beginCol==0){
            statusBar_disPlayMessage("已经搜索到第一个单元格了,请向下搜索...");
            QApplication::restoreOverrideCursor();
            dataBlocked=false;
            return;
        }
        else {
            ui->actionprefieldcheck->setEnabled(false);
            ui->actionnextfieldcheck->setEnabled(false);
            //初始起点
            beginCol=tableColCurrent-1;
            //上一行
            if(beginCol<0&&beginRow>0){
                beginRow=beginRow-1;
                beginCol=ptr_table->columnCount()-1;
            }
            while(currentPage>0){
                QStringList rowdata;
                for(int row=beginRow;row>=0;row--){
                    QMap<int,QString> checkresult;
                    rowRealInContent=(currentPage-1)*pageRowSize+row;
                    if(currentOpenFileType==openFileType::OFDFile){
                        rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::CSVFile){
                        rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::FIXEDFile){
                        rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
                    }
                    if(row%100==0){
                        statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                    checkRowFieldResult (rowdata,checkresult);
                    if(checkresult.empty()){
                        //本行没有，准备搜下一行
                        beginCol=ptr_table->columnCount()-1;
                        continue;
                    }
                    else{
                        for (int colindex=beginCol;colindex>=0;colindex--) {
                            if(checkresult.contains(colindex)){
                                ptr_table->setCurrentCell(row,colindex);
                                ptr_table->setFocus();
                                ui->actionprefieldcheck->setEnabled(true);
                                ui->actionnextfieldcheck->setEnabled(true);
                                QApplication::restoreOverrideCursor();
                                dataBlocked=false;
                                reCalculateTableBeginAndEnd();
                                on_tableWidget_itemSelectionChanged();
                                QString message=QString("在第%1行\"%2\"查找到必填字段缺失...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(colindex)->text());
                                statusBar_disPlayMessage(message);
                                return;
                            }
                        }
                    }
                }
                if(currentPage>1){
                    currentPage--;
                    pageJump(currentPage,-1);
                    qApp->processEvents();
                    //重新设置从右下角开始继续搜索
                    beginCol=ptr_table->columnCount()-1;
                    beginRow=ptr_table->rowCount()-1;
                    tableColCurrent=ptr_table->columnCount()-1;
                    tableRowCurrent=ptr_table->rowCount()-1;
                }
                else{
                    statusBar_disPlayMessage("再往上没有命中必填缺失的单元格了...");
                    dataBlocked=false;
                    ui->actionprefieldcheck->setEnabled(true);
                    ui->actionnextfieldcheck->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    break;
                }
            }
        }
    }
    else if (direction==1){
        if(currentPage==pageCount&&beginRow==ptr_table->rowCount()-1&&beginCol==ptr_table->columnCount()-1){
            statusBar_disPlayMessage("已经搜索到最后一个单元格了,请向上搜索...");
            QApplication::restoreOverrideCursor();
            dataBlocked=false;
            return;
        }
        else{
            ui->actionprefieldcheck->setEnabled(false);
            ui->actionnextfieldcheck->setEnabled(false);
            //初始起点
            beginCol=tableColCurrent+1;
            //最后一列
            if(beginCol>ptr_table->columnCount()-1&&beginRow<ptr_table->rowCount()-1){
                beginRow=beginRow+1;
                beginCol=0;
            }
            while(currentPage<=pageCount){
                QStringList rowdata;
                for(int row=beginRow;row<ptr_table->rowCount();row++){
                    QMap<int,QString> checkresult;
                    rowRealInContent=(currentPage-1)*pageRowSize+row;
                    if(currentOpenFileType==openFileType::OFDFile){
                        rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::CSVFile){
                        rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::FIXEDFile){
                        rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
                    }
                    if(row%500==0){
                        statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                    checkRowFieldResult (rowdata,checkresult);
                    if(checkresult.empty()){
                        beginCol=0;
                        continue;
                    }
                    else{
                        for (int colindex=beginCol;colindex<ptr_table->columnCount();colindex++) {
                            if(checkresult.contains(colindex)){
                                ptr_table->setCurrentCell(row,colindex);
                                ptr_table->setFocus();
                                ui->actionprefieldcheck->setEnabled(true);
                                ui->actionnextfieldcheck->setEnabled(true);
                                QApplication::restoreOverrideCursor();
                                dataBlocked=false;
                                reCalculateTableBeginAndEnd();
                                on_tableWidget_itemSelectionChanged();
                                QString message=QString("在第%1行\"%2\"查找到必填字段缺失...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(colindex)->text());
                                statusBar_disPlayMessage(message);
                                return;
                            }
                        }
                    }
                }
                if(currentPage<pageCount){
                    currentPage++;
                    pageJump(currentPage,-2);
                    qApp->processEvents();
                    beginCol=0;
                    beginRow=0;
                    tableColCurrent=0;
                    tableRowCurrent=0;
                }
                else{
                    statusBar_disPlayMessage("再往下没有命中必填缺失的单元格了...");
                    dataBlocked=false;
                    ui->actionprefieldcheck->setEnabled(true);
                    ui->actionnextfieldcheck->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    break;
                }
            }
        }
    }
}

void MainWindow::getFieldExportConfig(QMap <QString,int> config){
    if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile){
        dataBlocked=true;
        isExportFile=true;
        dataBlockedMessage="正在进行字段必填检查报告导出,请稍后再操作...";
        QString openpath="./";
        if(!currentOpenFilePath.isEmpty()){
            if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
                openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
            }
        }
        QString filename="";
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            QString filenameall=currentOpenFilePath.right(currentOpenFilePath.length()-currentOpenFilePath.lastIndexOf("/"));
            int index=filenameall.lastIndexOf(".");
            if(index==-1){
                filename=filenameall;
            }else{
                filename=filenameall.left(index);
            }
        }
        else{
            int index=currentOpenFilePath.lastIndexOf(".");
            if(index==-1){
                filename=currentOpenFilePath;
            }else{
                filename=currentOpenFilePath.left(index);
            }
        }
        filename=filename.append("_字段必填检查报告");
        //文件过滤器,用于追踪选择的保存文件类别
        QString selectedFilter=Q_NULLPTR;
        //弹出保存框
        //数据类型插入点，csv文件不支持导出csv文件
        QString fileTypeSelect="";
        if(config.value("exptype")==0){
            fileTypeSelect="HTML文件(*.html)";
        }
        if(config.value("exptype")==1){
            fileTypeSelect="TXT文件(*.txt)";
        }
        //弹窗选择文件保存路径
        QString fileNameSave = QFileDialog::getSaveFileName(this,("选择数据导出保存路径"),openpath+filename,fileTypeSelect,&selectedFilter);
        if(!fileNameSave.isEmpty()){
            //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
            if(selectedFilter=="TXT文件(*.txt)"&&(!fileNameSave.endsWith(".txt"))){
                fileNameSave.append(".xlsx");
            }
            else if(selectedFilter=="HTML文件(*.html)"&&(!fileNameSave.endsWith(".html"))){
                fileNameSave.append(".html");
            }
            //开始写入数据
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QFile data(fileNameSave);
            if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
                QTextCodec *codec=QTextCodec::codecForName("UTF-8");
                QString sb;
                QString title="";
                if(filename.contains("/")){
                    title=filename.mid(filename.lastIndexOf("/")+1);
                    title=title.left(title.length()-0);
                }else{
                    title=title.left(title.length()-0);
                }
                if(config.value("exptype")==0){
                    //html文件头
                    sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
                    //内联的表格样式,内容太多,base64存储
                    sb.append(QByteArray::fromBase64("PHN0eWxlIHR5cGU9InRleHQvY3NzIj4KLnRhYmxlCnsKcGFkZGluZzogMDsKbWFyZ2luOiAwOwp9CnRoIHsKZm9udDogYm9sZCAxMnB4ICJUcmVidWNoZXQgTVMiLCBWZXJkYW5hLCBBcmlhbCwgSGVsdmV0aWNhLCBzYW5zLXNlcmlmOwpjb2xvcjogIzRmNmI3MjsKYm9yZGVyLXJpZ2h0OiAxcHggc29saWQgI0MxREFENzsKYm9yZGVyLWJvdHRvbTogMXB4IHNvbGlkICNDMURBRDc7CmJvcmRlci10b3A6IDFweCBzb2xpZCAjQzFEQUQ3OwpsZXR0ZXItc3BhY2luZzogMnB4Owp0ZXh0LXRyYW5zZm9ybTogdXBwZXJjYXNlOwp0ZXh0LWFsaWduOiBsZWZ0OwpwYWRkaW5nOiA2cHggNnB4IDZweCAxMnB4OwpiYWNrZ3JvdW5kOiAjQ0FFOEVBIG5vLXJlcGVhdDsKd29yZC1icmVhazoga2VlcC1hbGw7CndoaXRlLXNwYWNlOm5vd3JhcDsKfQp0ciB7CndvcmQtYnJlYWs6IGtlZXAtYWxsOwp3aGl0ZS1zcGFjZTpub3dyYXA7Cn0KdGQgewpib3JkZXItcmlnaHQ6IDFweCBzb2xpZCAjQzFEQUQ3Owpib3JkZXItYm90dG9tOiAxcHggc29saWQgI0MxREFENzsKZm9udC1zaXplOjE0cHg7CnBhZGRpbmc6IDJweCA2cHggMnB4IDZweDsKY29sb3I6ICM0ZjZiNzI7Cn0KLm51bWJlcnsKdGV4dC1hbGlnbjogcmlnaHQ7Cn0KPC9zdHlsZT4K"));
                    //标题表头
                    sb.append("<table>\r\n<tr><th>结果编号</th><th>数据行号</th><th>命中列号</th><th>校验结果</th></tr>\r\n");

                }
                else{
                    sb.append("结果编号\t数据行号\t命中列号\t校验结果\r\n");
                }
                data.write(codec->fromUnicode(sb));
                data.flush();
                sb.clear();
                ////开始获取数据
                int rowCount=commonContentQByteArrayList.count();
                int checkCount=0;
                if(rowCount>0){
                    for (int row=0;row<rowCount;row++){
                        QStringList rowdata;
                        if(currentOpenFileType==openFileType::OFDFile){
                            rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                        }
                        else if(currentOpenFileType==openFileType::CSVFile){
                            rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row);
                        }
                        QMap<int,QString> checkresult;
                        checkRowFieldResult (rowdata,checkresult,1);
                        if(!checkresult.empty()){
                            foreach(auto item,checkresult.toStdMap()){
                                checkCount++;
                                if(config.value("exptype")==0){
                                    sb.append("<tr><td class=\"number\">").append(QString::number(checkCount)).append("</td>");
                                    sb.append("<td class=\"number\">").append(QString::number(row+1)).append("</td>");
                                    sb.append("<td class=\"number\">").append(QString::number(item.first)).append("</td>");
                                    sb.append("<td>").append(item.second).append("</td></tr>\r\n");
                                }
                                else{
                                    sb.append(QString::number(checkCount));
                                    sb.append("\t").append(QString::number(row+1));
                                    sb.append("\t").append(QString::number(item.first));
                                    sb.append("\t").append(item.second).append("\r\n");
                                }
                            }
                        }
                        if((row%1000==0)||(row==rowCount-1)){
                            data.write(codec->fromUnicode(sb));
                            data.flush();
                            sb.clear();
                            statusBar_disPlayMessage(QString("字段必填检查报告导出中,请勿进行其他操作,已检查%1行").arg(QString::number(row+1)));
                            qApp->processEvents();
                            if(abortExit){
                                QApplication::restoreOverrideCursor();
                                qDebug()<<"强制立即终止任务";
                                return;
                            }
                        }
                    }
                }
                if(config.value("exptype")==0){
                    sb.append("</table>\r\n</body>\r\n</html>");
                }
                data.write(codec->fromUnicode(sb));
                data.flush();
                data.close();
                statusBar_disPlayMessage(tr("报告成功导出到%1").arg(fileNameSave));
                if(config.value("openatexported")==1){
                    bool is_open = QDesktopServices::openUrl(QUrl::fromLocalFile(fileNameSave));
                    if(!is_open)
                    {
                        statusBar_disPlayMessage("文件打开失败,请尝试手工打开~~~");
                        return;
                    }
                }
            }else{
                statusBar_disPlayMessage(tr("报告导出失败,请重试"));
            }
            dataBlocked=false;
            isExportFile=false;
            QApplication::restoreOverrideCursor();
        }
        else{
            dataBlocked=false;
            isExportFile=false;
            statusBar_disPlayMessage("取消导出...");
        }
    }
}

void MainWindow::getPrimaryExportConfig(QMap <QString,int> config){
    QString openpath="./";
    if(!currentOpenFilePath.isEmpty()){
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
        }
    }
    QString filename="";
    if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
        QString filenameall=currentOpenFilePath.right(currentOpenFilePath.length()-currentOpenFilePath.lastIndexOf("/"));
        int index=filenameall.lastIndexOf(".");
        if(index==-1){
            filename=filenameall;
        }else{
            filename=filenameall.left(index);
        }
    }
    else{
        int index=currentOpenFilePath.lastIndexOf(".");
        if(index==-1){
            filename=currentOpenFilePath;
        }else{
            filename=currentOpenFilePath.left(index);
        }
    }
    filename=filename.append("_主键冲突检查报告");
    //文件过滤器,用于追踪选择的保存文件类别
    QString selectedFilter=Q_NULLPTR;
    //弹出保存框
    //数据类型插入点，csv文件不支持导出csv文件
    QString fileTypeSelect="";
    if(config.value("exptype")==0){
        fileTypeSelect="HTML文件(*.html)";
    }
    if(config.value("exptype")==1){
        fileTypeSelect="TXT文件(*.txt)";
    }
    //弹窗选择文件保存路径
    QString fileNameSave = QFileDialog::getSaveFileName(this,("选择数据导出保存路径"),openpath+filename,fileTypeSelect,&selectedFilter);
    if(!fileNameSave.isEmpty()){
        //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
        if(selectedFilter=="TXT文件(*.txt)"&&(!fileNameSave.endsWith(".txt"))){
            fileNameSave.append(".xlsx");
        }
        else if(selectedFilter=="HTML文件(*.html)"&&(!fileNameSave.endsWith(".html"))){
            fileNameSave.append(".html");
        }
        primaryCheck(1,fileNameSave,config.value("exptype"),config.value("openatexported"));
    }
    else{
        statusBar_disPlayMessage("取消导出...");
    }
}

void MainWindow::on_actiontableSelectionBehaviorMenu_triggered()
{
    QTableWidgetSelectionRange a(0,0,ptr_table->rowCount()-1,ptr_table->columnCount()-1);
    if(tableSelectionBehavior==QAbstractItemView::SelectItems){
        tableSelectionBehavior=QAbstractItemView::SelectRows;
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        ui->actiontableSelectionBehaviorMenu->setText("表格高亮按单元格选择");
        ptr_table->setRangeSelected(a,false);
        statusBar_disPlayMessage("切换到按行选择...");
    }
    else if(tableSelectionBehavior==QAbstractItemView::SelectRows){
        tableSelectionBehavior=QAbstractItemView::SelectItems;
        ptr_table->setSelectionBehavior(tableSelectionBehavior);
        ui->actiontableSelectionBehaviorMenu->setText("表格高亮按行选择");
        ptr_table->setRangeSelected(a,false);
        statusBar_disPlayMessage("切换到按单元格选择...");
    }
}

void MainWindow::on_action_fieldcheckconfigtools_triggered()
{
    FormFieldCheckEditTools *edittools=new FormFieldCheckEditTools(ofdConfigList,csvConfigList,fixedConfigList);
    connect(edittools,&FormFieldCheckEditTools::sendConfigChange,this,&MainWindow::getConfigFileChange);
    edittools->setAttribute(Qt::WA_DeleteOnClose);
    edittools->show();
    edittools->raise();
    edittools->activateWindow();
}

void MainWindow::getConfigFileChange(bool flag,QStringList modifyConfigList){
    configFileChange=flag;
    modifyConfigQStringList.clear();
    modifyConfigQStringList=modifyConfigList;
}

void  MainWindow::getSignalsClosePrimaryCheck(){
    qDebug()<<"接收到信号,关闭主键冲突弹窗";
    dialogDialogShowPrimaryCheck->hide();
    dialogDialogShowPrimaryCheck->close();
    dialogDialogShowPrimaryCheck=nullptr;
}

void  MainWindow::getActionFromPrimaryCheckTools(int type,int row1,int row2){
    if(needReCheckPrimary){
        statusBar_disPlayMessage("数据可能有变化,需要重新执行主键冲突检查才能使用此功能...");
    }
    else{
        //跳到某行
        if(type==1){
            int  lineNumber=row1;
            if(lineNumber<1){
                statusBar_disPlayMessage(tr("行号不能小于1"));
            }
            else{
                //数据类型加入点
                int targetIndex=0;
                if(lineNumber>commonContentQByteArrayList.count()){
                    statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                    return;
                }
                else{
                    targetIndex=lineNumber-1;
                }
                //判断是否在当前页，是否需要跳行
                int targetPage=(targetIndex/pageRowSize)+1;
                int targetTableIndex=(targetIndex%pageRowSize);
                if(targetPage==currentPage){
                    ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
                    ptr_table->setFocus();
                }
                else{
                    pageJump(targetPage,targetTableIndex);
                    ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
                    ptr_table->setFocus();
                }
            }
        }
        //调出数据比对
        else if(type==2){
            if(row1>commonContentQByteArrayList.count()||row2>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("异常,行号超出了数据最大行数值,放弃比对"));
                return;
            }
            else{
                //打开比对器
                //设置表格列标题
                QStringList title;
                QStringList fieldType;
                title.append("数据行号");
                QMap<int,QStringList> primaryCompareData;
                //OFD
                if(currentOpenFileType==openFileType::OFDFile){
                    for(int i=0;i<ofd.getFieldList().count();i++){
                        //仅获取列的中文备注当作列标题
                        title.append(ofd.getFieldList().at(i).getFieldDescribe());
                        fieldType.append(ofd.getFieldList().at(i).getFieldType());
                    }
                    primaryCompareData.insert(row1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row1-1));
                    primaryCompareData.insert(row2,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row2-1));
                }
                //CSV
                else if(currentOpenFileType==openFileType::CSVFile){
                    for(int i=0;i<csv.getFieldList().count();i++){
                        //仅获取列的中文备注当作列标题
                        title.append(csv.getFieldList().at(i).getFieldDescribe());
                        if(CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                            fieldType.append("N");
                        }
                        else{
                            fieldType.append("S");
                        }
                    }
                    primaryCompareData.insert(row1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row1-1));
                    primaryCompareData.insert(row2,Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row2-1));
                }
                //FIXED
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    for(int i=0;i<fixed.getFieldList().count();i++){
                        //仅获取列的中文备注当作列标题
                        title.append(fixed.getFieldList().at(i).getFieldDescribe());
                        fieldType.append(fixed.getFieldList().at(i).getFieldType());
                    }
                    primaryCompareData.insert(row1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row1-1));
                    primaryCompareData.insert(row2,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row2-1));
                }
                //打开窗口
                if(title.count()>0&&primaryCompareData.count()>1){
                    DialogShowTableCompareView * dialog = new DialogShowTableCompareView(title,fieldType,&primaryCompareData,this);
                    dialog->setWindowTitle(QString("行比对器视图"));
                    dialog->setModal(false);
                    dialog->setAttribute(Qt::WA_DeleteOnClose);
                    dialog->show();
                    dialog->raise();
                    dialog->activateWindow();
                }
                else{
                    statusBar_disPlayMessage(tr("没有配置适用于该文件的字段必填检查规则,请配置后重试..."));
                }
            }
        }
    }
}

void MainWindow::on_actionprimaryKeyCheck_triggered()
{
    if(dialogDialogShowPrimaryCheck!=nullptr){
        statusBar_disPlayMessage("已打开一个主键冲突检查结果窗口,请关闭后再次重试...");
    }
    else{
        primaryCheck(0);
    }
}

void MainWindow::on_actionprimarkKeyCheckExport_triggered()
{
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    QList <uint> primaryKeyFieldList;
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    else if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    else if(currentOpenFileType==openFileType::DBFFile){statusBar_disPlayMessage("不支持DBF文件使用此功能...");return;}
    else{
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的文件没有数据记录...");
            return;
        }
        else{
            if(currentOpenFileType==openFileType::OFDFile){primaryKeyFieldList=ofd.getPrimaryKeyFieldList();}
            else if(currentOpenFileType==openFileType::CSVFile){primaryKeyFieldList=csv.getPrimaryKeyFieldList();}
            else if(currentOpenFileType==openFileType::FIXEDFile){primaryKeyFieldList=fixed.getPrimaryKeyFieldList();}
        }
        if(primaryKeyFieldList.count()>0){
            //选择导出模式
            DialogChooseFieldCheckExportReport * dialog222 = new DialogChooseFieldCheckExportReport(this);
            connect(dialog222,&DialogChooseFieldCheckExportReport::sendFieldExportConfig,this,&MainWindow::getPrimaryExportConfig);
            dialog222->setModal(true);
            dialog222->setAttribute(Qt::WA_DeleteOnClose);
            dialog222->show();
            dialog222->raise();
            dialog222->activateWindow();
        }
        else{
            statusBar_disPlayMessage(tr("没有配置适用于该文件的字段必填检查规则,请配置后重试..."));
        }
    }
}

void MainWindow::on_actioninvisibleCharactersCheck1_triggered()
{
    invisibleCharactersCheckUp(0);
}

void MainWindow::on_actioninvisibleCharactersCheck2_triggered()
{
    invisibleCharactersCheckDown(0);
}


void MainWindow::on_actionactionasciiCharactersCheck1_triggered()
{
    invisibleCharactersCheckUp(1);
}

void MainWindow::on_actionactionasciiCharactersCheck2_triggered()
{
    invisibleCharactersCheckDown(1);
}

void MainWindow::invisibleCharactersCheckUp(int rangeType){
    if(isUpdateData){statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");return;}
    if(dataBlocked){statusBar_disPlayMessage(dataBlockedMessage);return;}
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    //数据类型加入点
    if((currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile)&&commonContentQByteArrayList.count()<1){
        statusBar_disPlayMessage("打开的文件没有数据记录...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==0){
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的文件没有数据记录...");
                return;
            }
        }
        else{
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的文件在当前模式下没有数据记录...");
                return;
            }
        }
    }
    if(tableRowCurrent==0&&tableColCurrent==0){
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        statusBar_disPlayMessage("已经搜索到第一个单元格了,请向下搜索...");
        return;
    }
    dataBlocked=true;
    int oldCurrentPage=currentPage;
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->actioninvisibleCharactersCheck1->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    int beginCol=tableColCurrent-1;
    int beginRow=tableRowCurrent;
    int colCount=ptr_table->columnCount();
    int rowRealInContent=0;
    while(currentPage>0){
        for(int row=beginRow;row>=0;row--){
            rowRealInContent=(currentPage-1)*pageRowSize+row;
            QStringList rowdata;
            if(currentOpenFileType==openFileType::OFDFile){
                rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            if(rowdata.count()==0){
                beginCol=colCount-1;
                continue;
            }
            for(int col=rowdata.count()-1>=beginCol?beginCol:rowdata.count()-1;col>=0;col--){
                if(rangeType==0){
                    if(invisibleCharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck1->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("第%1行\"%2\"有不可见字符,右键使用\"十六进制字符编码透视(乱码分析)\"查看详情...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
                else{
                    if(invisibleCharactersCheck(rowdata.at(col))||invisibleASCIICharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck1->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("第%1行\"%2\"有不可见字符(含空格和制表符等ASCII码),右键使用\"十六进制字符编码透视(乱码分析)\"查看详情...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
            }
            beginCol=colCount-1;
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                qApp->processEvents();
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        if(currentPage>1){
            currentPage--;
            pageJump(currentPage,-1);
            beginCol=ptr_table->columnCount()-1;
            beginRow=ptr_table->rowCount()-1;
        }
        else{
            dataBlocked=false;
            ui->actioninvisibleCharactersCheck1->setEnabled(true);
            if(oldCurrentPage==currentPage){
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            else{
                ptr_table->scrollToTop();
                tableRowCurrent=0;
                tableColCurrent=0;
                ptr_table->setCurrentCell(0,0);
            }
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            if(rangeType==0){
                statusBar_disPlayMessage("再往上没有不可见字符了,请向下搜索...");
            }
            else{
                statusBar_disPlayMessage("再往上没有不可见字符(含空格和制表符等ASCII码)了,请向下搜索...");
            }
            break;
        }
    }
}

void MainWindow::invisibleCharactersCheckDown(int rangeType){
    if(isUpdateData){statusBar_disPlayMessage("正在加载数据,请稍后再使用此功能...");return;}
    if(dataBlocked){statusBar_disPlayMessage(dataBlockedMessage);return;}
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    int colCount=ptr_table->columnCount();
    int tableColCount=ptr_table->columnCount();
    //数据类型加入点
    if((currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile)&&commonContentQByteArrayList.count()<1){
        statusBar_disPlayMessage("打开的文件没有数据记录...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfRowMap.count()<1){
            statusBar_disPlayMessage("打开的文件没有数据记录...");
            return;
        }
    }
    if(tableRowCurrent==ptr_table->rowCount()-1&&tableColCurrent==colCount-1){
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        statusBar_disPlayMessage("已经搜索到最后一个单元格了,请向上搜索...");
        return;
    }
    dataBlocked=true;
    int oldCurrentPage=currentPage;
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->actioninvisibleCharactersCheck2->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    int currentPageRowCount=ptr_table->rowCount();
    int beginCol=tableColCurrent+1;
    int beginRow=tableRowCurrent;
    int rowRealInContent=0;
    while(currentPage<=pageCount){
        for(int row=beginRow;row<currentPageRowCount;row++){
            rowRealInContent=(currentPage-1)*pageRowSize+row;
            QStringList rowdata;
            if(currentOpenFileType==openFileType::OFDFile){
                rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            if(rowdata.count()==tableColCount){
                colCount=tableColCount;
            }
            else if(rowdata.count()==0){
                beginCol=0;
                continue;
            }
            else if(rowdata.count()<colCount){
                colCount=rowdata.count();
            }
            for(int col=beginCol;col<colCount;col++){
                if(rangeType==0){
                    if(invisibleCharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck2->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("在第%1行\"%2\"搜索到不可见字符,在单元格右键使用\"十六进制字符编码透视(乱码分析)\"进行详细分析...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
                else{
                    if(invisibleCharactersCheck(rowdata.at(col))||invisibleASCIICharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck2->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("在第%1行\"%2\"搜索到不可见字符(含空格和制表符等ASCII码),在单元格右键使用\"十六进制字符编码透视(乱码分析)\"进行详细分析...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
            }
            beginCol=0;
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                qApp->processEvents();
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        if(currentPage<pageCount){
            currentPage++;
            pageJump(currentPage,-2);
            beginCol=0;
            beginRow=0;
            currentPageRowCount=ptr_table->rowCount();
        }
        else{
            dataBlocked=false;
            ui->actioninvisibleCharactersCheck2->setEnabled(true);
            if(oldCurrentPage==currentPage){
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            else{
                ptr_table->scrollToBottom();
                tableRowCurrent=currentPageRowCount-1;
                tableColCurrent=colCount-1;
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            if(rangeType==0){
                statusBar_disPlayMessage("再往下没有不可见字符了,请向上搜索...");
            }
            else{
                statusBar_disPlayMessage("再往下没有不可见字符(含空格和制表符等ASCII码)了,请向上搜索...");
            }
            break;
        }
    }
}

void MainWindow::reCalculateFileCountLine(){
    if(currentOpenFileType==openFileType::OFDFile&&commonHeaderQStringList.count()>0){
        commonHeaderQStringList.replace(commonHeaderQStringList.count()-1,QString("%1").arg(commonContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
    }
    else if(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0){
        if(fixed.getRowcountcheckrow()>0&&fixed.getRowcountcheckrow()<=commonHeaderQStringList.count()){
            QString lineCount=commonHeaderQStringList.at(fixed.getRowcountcheckrow()-1);
            bool okFlag=true;
            lineCount.toDouble(&okFlag);
            if(okFlag){
                int lineLength=lineCount.length();
                if(lineCount.startsWith("0")&&lineLength>=8){
                    commonHeaderQStringList.replace(fixed.getRowcountcheckrow()-1,QString("%1").arg(commonContentQByteArrayList.count(),lineLength,10,QLatin1Char('0')));
                }
                else{
                    commonHeaderQStringList.replace(fixed.getRowcountcheckrow()-1,QString("%1").arg(QString::number(commonContentQByteArrayList.count())));
                }
            }
        }
    }
}

void MainWindow::setStandFontSize(){
    if(standFontSize==1){
#ifdef Q_OS_MAC
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(14));
#endif

#ifdef Q_OS_LINUX
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(14));
#endif

#ifdef Q_OS_WIN32
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(13));
#endif
    }
    else if(standFontSize==2){
#ifdef Q_OS_MAC
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(15));
#endif

#ifdef Q_OS_LINUX
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(15));
#endif

#ifdef Q_OS_WIN32
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(14));
#endif
    }
    else{
#ifdef Q_OS_MAC
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(13));
#endif

#ifdef Q_OS_LINUX
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(13));
#endif

#ifdef Q_OS_WIN32
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(12));
#endif
    }
}

void MainWindow::init_Others(){
    tips.append("导出数据到Excel,可以使用excel进行强大的筛选、统计、分析...");
    tips.append("导出数据到Csv,可以方便的进行数据交换,导入到别的系统...");
    tips.append("导出数据到Html,可以在浏览器中浏览数据,或者发送给其他人便捷浏览数据...");
    tips.append("输入你要查找的字段中文名或者英文名,点击列跳转可以快速跳到你要查找的字段,避免横着拖动滚动条寻找...");
    tips.append("输入你要直达的行号,点击行跳转,直接跳到你要查找的行...");
    tips.append("双击任意单元格,可以将本行数据以列模式显示,自带字典翻译,任意搜索...");
    tips.append("向上向下搜索,快速找到你要的数据...");
    tips.append("外部编辑文件后,使用重新读取文件功能,或者按F5键,极速重新加载数据...");
    tips.append("右键将任意行加入比对器,找到需要比对的行,再加入比对器,就能打开比对器以比对模式查看各行数据的差异了...");
    tips.append("比对器支持任意多行同时比对哟...");
    tips.append("右键菜单有不少小功能哟...");
    tips.append("数据修改后点击保存,程序会自动创建备份,避免找不回原文件...");
    tips.append("数据修改后点击另存保存,可以覆盖保存或者保存文件到其他位置...");
    tips.append("你可以在"+Utils::getConfigPath()+"目录下进行配置文件修改,可以让工具支持新增的各种OFD/类OFD文件...");
    tips.append("拖放文件到程序主窗口,即可打开文件,又快又省心...");
    tips.append("程序里有一个彩蛋哟,快去找一下...");
    tips.append("小心使用编辑功能,避免造成不可挽回的事故...");
    tips.append("源文件某行数据在解析器的第几行?,试试源文件行跳转功能,一键直达...");
    tips.append("选中某一列的多行数据(按住Crtl后单击需要选择的单元格),或者单击列标题选择本列单元格数据后,可以使用批量编辑功能...");
    tips.append("按Ctrl+G切换视图模式,可以隐藏或者显示文件头信息...");
    tips.append("尝试使用本工具解析各种固定分隔符文件吧，比excel更好的数据展示方式,且支持导出excel...");
    tips.append("创建一个本软件的桌面图标,直接拖放文件到图标上即可打开文件,只需一步即可直达");
    tips.append("选择一个区间的数据，右键复制,可以便捷粘贴到excel中");
    tips.append("对于全是数值的列，选择同一列多行数据时，状态栏会自动对选择的数据进行求和，求平均数");
    tips.append("尝试使用本工具解析各种固定字符/字节长度文件吧，高效解析,且支持导出excel...");
    tips.append("尝试进行设置本程序,可以实现默认拖进文件时打开一个新窗口解析,默认使用精简模式打开程序,你的爱好你做主...");
    tips.append("基金、银行理财领域的OFD/类OFD文件,本程序都支持解析哟...");
    tips.append("需要打开超大文件?,建议在设置里设置开启压缩模式和分页支持,支持的文件大小立即提升到GB级别...");
    tips.append("本程序是业余无偿开发的,如果程序帮助到了你,你可以选择小额捐赠给予支持,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("小额捐赠给予支持,FFReader将获得更好的发展,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("FFReader是一个免费的工具软件,如果程序帮助到了你,你可以选择小额捐赠给予支持,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("可以使用本程序新建OFD/类OFD文件,以及初始化自己的新建模板...");
    tips.append("导出功能可以分页导出或者导出全部数据,自由选择导出范围,是否使用UTF-8编码...");
    tips.append("在程序主窗口或者查看行详细信息界面,使用Ctrl+Alt+R(command+option+R)可以进行快速截图保存...");
    tips.append("固定分隔符文件可以在任意单元格右键对此列手工设置数据格式,调整为数值或者自定义小数长度哟,方便进行数值统计...");
    tips.append("在设置-首选项里设置默认精简视图,即可默认获得最大化的数据展示空间...");
    tips.append("本程序支持解析证券各类DBF文件,直接解析或者配置解析均可,使用配置获得更好的解析体验...");
    tips.append("需要编辑手工创造OFD/类OFD数据文件？尝试新建文件后导出到Excel编辑，编辑后可以立即导入本程序生成OFD/类OFD数据文件...");
    tips.append("字段必填检查工具可以方便的检查是否有异常的字段值缺失,尝试配置几个检测规则然后使用吧...");
    tips.append("不可见字符检查工具可以方便的检索文件中是否有不可见的零宽字符...");
    tips.append("担心文件存在重复冲突数据？尝试使用主键冲突检查工具检查你的文件吧...");
    tips.append("如果你的文件包含生僻字且显示异常,留意检查是否已安装相关字体补充文件...");
    tips.append("不喜欢默认字体?,你可以在设置-首选项中任意修改你喜欢的字体...");
    tips.append("你可以在设置-首选项中设定字体大小，找到适合你的字体大小吧...");

#ifdef Q_OS_WIN32
    tips.append("同时拖放两个文件到程序主窗口,将使用文件比对插件自动比对两个文件的差异...");
    tips.append("如果你要查看接口文件的原始数据,不妨在附加工具菜单下点击\"在文本编辑器中打开当前文件\"...");
    tips.append("想要比对同一个文件不同版本的差异?试试导出到csv固定分隔符文件后同时把两个文件拖向FFReader,它会问你是否要比对差异...");
    tips.append("禁用压缩功能,可以极大的提高文件加载速度,如果你的电脑内存足够,建议禁用压缩功能(打开2GB的文件大概需要2.5GB内存)...");
    tips.append("如果没有特殊情况,请使用64位FFReader,32位版本的FFReader难以打开超大的文件...");
#endif
    specialCharacter.insert(0X00,"NUL (null):空字符");
    specialCharacter.insert(0X01,"SOH (start of headling):标题开始");
    specialCharacter.insert(0X02,"STX (start of text):正文开始");
    specialCharacter.insert(0X03,"ETX (end of text):正文结束");
    specialCharacter.insert(0X04,"EOT (end of transmission):传输结束");
    specialCharacter.insert(0X05,"ENQ (enquiry):请求");
    specialCharacter.insert(0X06,"ACK (acknowledge):收到通知");
    specialCharacter.insert(0X07,"BEL (bell):响铃");
    specialCharacter.insert(0X08,"BS (backspace):退格");
    specialCharacter.insert(0X09,"HT (horizontal tab):水平制表符");
    specialCharacter.insert(0X0A,"LF (NL line feed new line):换行键");
    specialCharacter.insert(0X0B,"VT (vertical tab):垂直制表符");
    specialCharacter.insert(0X0C,"FF (NP form feed new page):换页键");
    specialCharacter.insert(0X0D,"CR (carriage return):回车键");
    specialCharacter.insert(0X0E,"SO (shift out):不用切换");
    specialCharacter.insert(0X0F,"SI (shift in):启用切换");
    specialCharacter.insert(0X10,"DLE (data link escape):数据链路转义");
    specialCharacter.insert(0X11,"DC1 (device control 1):设备控制1");
    specialCharacter.insert(0X12,"DC2 (device control 2):设备控制2");
    specialCharacter.insert(0X13,"DC3 (device control 3):设备控制3");
    specialCharacter.insert(0X14,"DC4 (device control 4):设备控制4");
    specialCharacter.insert(0X15,"NAK (negative acknowledge):拒绝接收");
    specialCharacter.insert(0X16,"SYN (synchronous idle):同步空闲");
    specialCharacter.insert(0X17,"ETB (end of trans block):传输块结束");
    specialCharacter.insert(0X18,"CAN (cancel):取消");
    specialCharacter.insert(0X19,"EM (end of medium):介质中断");
    specialCharacter.insert(0X1A,"SUB (substitute):替补");
    specialCharacter.insert(0X1B,"ESC (escape):溢出");
    specialCharacter.insert(0X1C,"FS (file separator):文件分割符");
    specialCharacter.insert(0X1D,"GS (group separator):分组符");
    specialCharacter.insert(0X1E,"RS (record separator):记录分离符");
    specialCharacter.insert(0X1F,"US (unit separator):单元分隔符");
    specialCharacter.insert(0X20,"SP (space):空格");
    specialCharacter.insert(0X7F,"DEL (delete):删除");

    //其他不可见或者神奇字符
    specialCharacter.insert(0X2000,"EN QUAD:en四元");
    specialCharacter.insert(0X2001,"EM QUAD");
    specialCharacter.insert(0X2002,"EN SPACE");
    specialCharacter.insert(0X2003,"EM SPACE:Em空间");
    specialCharacter.insert(0X2004,"THREE-PER-EM SPACE:每em三个空格");
    specialCharacter.insert(0X2025,"FOUR-PER-EM SPACE:每em四个空格");
    specialCharacter.insert(0X2026,"SIX-PER-EM SPACE:六个/em空间");
    specialCharacter.insert(0X2027,"FIGURE SPACE:数字空间");
    specialCharacter.insert(0X2028,"PUNCTUATION SPACE:标点符号空间");
    specialCharacter.insert(0X2029,"THIN SPACE:薄空间");
    specialCharacter.insert(0X202A,"HAIR SPACE:发际空间");
    specialCharacter.insert(0X200B,"ZERO WIDTH SPACE:零宽空格");
    specialCharacter.insert(0X200C,"ZERO WIDTH NON-JOINER:零宽不连字");
    specialCharacter.insert(0X200D,"ZERO WIDTH JOINER:零宽连字");
    specialCharacter.insert(0X200E,"LEFT-TO-RIGHT MARK:左至右符号");
    specialCharacter.insert(0X200F,"RIGHT-TO-LEFT MARK:右至左符号");
    specialCharacter.insert(0X2011,"NON-BREAKING HYPHEN:不间断连字符");
    specialCharacter.insert(0X2028,"LINE SEPARATOR:线路分离器");
    specialCharacter.insert(0X2029,"PARAGRAPH SEPARATOR:段落分隔符");
    specialCharacter.insert(0X202A,"LEFT-TO-RIGHT EMBEDDING:从左到右嵌入");
    specialCharacter.insert(0X202B,"RIGHT-TO-LEFT EMBEDDING:从右到左嵌入");
    specialCharacter.insert(0X202C,"POP DIRECTIONAL FORMATTING:弹出方向格式");
    specialCharacter.insert(0X202D,"LEFT-TO-RIGHT OVERRIDE:左右超控");
    specialCharacter.insert(0X202E,"RIGHT-TO-LEFT OVERRIDE:从右向左超控");
    specialCharacter.insert(0X202F,"NARROW NO-BREAK SPACE:窄的不间断空间");
    specialCharacter.insert(0X205F,"MEDIUM MATHEMATICAL SPACE:中等数学空间");
    specialCharacter.insert(0X2060,"WORD JOINER:字连接符");
    specialCharacter.insert(0X2061,"FUNCTION APPLICATION:功能应用");
    specialCharacter.insert(0X2062,"INVISIBLE TIMES:不可见时间");
    specialCharacter.insert(0X2063,"INVISIBLE SEPARATOR:不可见分隔符");
    specialCharacter.insert(0X2064,"INVISIBLE PLUS:不可见加号");
    specialCharacter.insert(0X2066,"LEFT-TO-RIGHT ISOLATE:从左到右隔离");
    specialCharacter.insert(0X2067,"RIGHT-TO-LEFT ISOLATE:从右到左隔离");
    specialCharacter.insert(0X2068,"FIRST STRONG ISOLATE:第一强分离物");
    specialCharacter.insert(0X2069,"POP DIRECTIONAL ISOLATE:流行方向隔离");
    specialCharacter.insert(0X206A,"INHIBIT SYMMETRIC SWAPPING:禁止对称交换");
    specialCharacter.insert(0X206B,"ACTIVATE SYMMETRIC SWAPPING:激活对称交换");
    specialCharacter.insert(0X206C,"INHIBIT ARABIC FORM SHAPING:抑制阿拉伯形式的形成");
    specialCharacter.insert(0X206D,"ACTIVATE ARABIC FORM SHAPING:激活阿拉伯语形式变形");
    specialCharacter.insert(0X206E,"NATIONAL DIGIT SHAPES:国家数字形状");
    specialCharacter.insert(0X206F,"NOMINAL DIGIT SHAPES:名义数字形状");

    /*52个PUA汉字*/
    puaOrDeletedGBKCharacter.insert(0xE82F,"PUA码字符,建议使用正式码【㳠,Unicode码3CE0,GB18030码FE6A】");
    puaOrDeletedGBKCharacter.insert(0xE81A,"PUA码字符,建议使用正式码【㑳,Unicode码3473,GB18030码FE55】");
    puaOrDeletedGBKCharacter.insert(0xE81B,"PUA码字符,建议使用正式码【㑇,Unicode码3447,GB18030码FE56】");
    puaOrDeletedGBKCharacter.insert(0xE81F,"PUA码字符,建议使用正式码【㖞,Unicode码359E,GB18030码FE5A】");
    puaOrDeletedGBKCharacter.insert(0xE821,"PUA码字符,建议使用正式码【㘎,Unicode码360E,GB18030码FE5C】");
    puaOrDeletedGBKCharacter.insert(0xE820,"PUA码字符,建议使用正式码【㘚,Unicode码361A,GB18030码FE5B】");
    puaOrDeletedGBKCharacter.insert(0xE824,"PUA码字符,建议使用正式码【㥮,Unicode码396E,GB18030码FE5F】");
    puaOrDeletedGBKCharacter.insert(0xE825,"PUA码字符,建议使用正式码【㤘,Unicode码3918,GB18030码FE60】");
    puaOrDeletedGBKCharacter.insert(0xE827,"PUA码字符,建议使用正式码【㧏,Unicode码39CF,GB18030码FE62】");
    puaOrDeletedGBKCharacter.insert(0xE828,"PUA码字符,建议使用正式码【㧟,Unicode码39DF,GB18030码FE63】");
    puaOrDeletedGBKCharacter.insert(0xE829,"PUA码字符,建议使用正式码【㩳,Unicode码3A73,GB18030码FE64】");
    puaOrDeletedGBKCharacter.insert(0xE82A,"PUA码字符,建议使用正式码【㧐,Unicode码39D0,GB18030码FE65】");
    puaOrDeletedGBKCharacter.insert(0xE82D,"PUA码字符,建议使用正式码【㭎,Unicode码3B4E,GB18030码FE68】");
    puaOrDeletedGBKCharacter.insert(0xE82E,"PUA码字符,建议使用正式码【㱮,Unicode码3C6E,GB18030码FE69】");
    puaOrDeletedGBKCharacter.insert(0xE834,"PUA码字符,建议使用正式码【䁖,Unicode码4056,GB18030码FE6F】");
    puaOrDeletedGBKCharacter.insert(0xE835,"PUA码字符,建议使用正式码【䅟,Unicode码415F,GB18030码FE70】");
    puaOrDeletedGBKCharacter.insert(0xE837,"PUA码字符,建议使用正式码【䌷,Unicode码4337,GB18030码FE72】");
    puaOrDeletedGBKCharacter.insert(0xE83C,"PUA码字符,建议使用正式码【䎱,Unicode码43B1,GB18030码FE77】");
    puaOrDeletedGBKCharacter.insert(0xE83D,"PUA码字符,建议使用正式码【䎬,Unicode码43AC,GB18030码FE78】");
    puaOrDeletedGBKCharacter.insert(0xE83F,"PUA码字符,建议使用正式码【䏝,Unicode码43DD,GB18030码FE7A】");
    puaOrDeletedGBKCharacter.insert(0xE840,"PUA码字符,建议使用正式码【䓖,Unicode码44D6,GB18030码FE7B】");
    puaOrDeletedGBKCharacter.insert(0xE841,"PUA码字符,建议使用正式码【䙡,Unicode码4661,GB18030码FE7C】");
    puaOrDeletedGBKCharacter.insert(0xE842,"PUA码字符,建议使用正式码【䙌,Unicode码464C,GB18030码FE7D】");
    puaOrDeletedGBKCharacter.insert(0xE844,"PUA码字符,建议使用正式码【䜣,Unicode码4723,GB18030码FE80】");
    puaOrDeletedGBKCharacter.insert(0xE845,"PUA码字符,建议使用正式码【䜩,Unicode码4729,GB18030码FE81】");
    puaOrDeletedGBKCharacter.insert(0xE846,"PUA码字符,建议使用正式码【䝼,Unicode码477C,GB18030码FE82】");
    puaOrDeletedGBKCharacter.insert(0xE847,"PUA码字符,建议使用正式码【䞍,Unicode码478D,GB18030码FE83】");
    puaOrDeletedGBKCharacter.insert(0xE849,"PUA码字符,建议使用正式码【䥇,Unicode码4947,GB18030码FE85】");
    puaOrDeletedGBKCharacter.insert(0xE84C,"PUA码字符,建议使用正式码【䦂,Unicode码4982,GB18030码FE88】");
    puaOrDeletedGBKCharacter.insert(0xE84A,"PUA码字符,建议使用正式码【䥺,Unicode码497A,GB18030码FE86】");
    puaOrDeletedGBKCharacter.insert(0xE84B,"PUA码字符,建议使用正式码【䥽,Unicode码497D,GB18030码FE87】");
    puaOrDeletedGBKCharacter.insert(0xE84D,"PUA码字符,建议使用正式码【䦃,Unicode码4983,GB18030码FE89】");
    puaOrDeletedGBKCharacter.insert(0xE84E,"PUA码字符,建议使用正式码【䦅,Unicode码4985,GB18030码FE8A】");
    puaOrDeletedGBKCharacter.insert(0xE84F,"PUA码字符,建议使用正式码【䦆,Unicode码4986,GB18030码FE8B】");
    puaOrDeletedGBKCharacter.insert(0xE853,"PUA码字符,建议使用正式码【䦶,Unicode码49B3,GB18030码8234A038】");
    puaOrDeletedGBKCharacter.insert(0xE850,"PUA码字符,建议使用正式码【䦟,Unicode码499F,GB18030码FE8C】");
    puaOrDeletedGBKCharacter.insert(0xE852,"PUA码字符,建议使用正式码【䦷,Unicode码49B7,GB18030码FE8E】");
    puaOrDeletedGBKCharacter.insert(0xE851,"PUA码字符,建议使用正式码【䦛,Unicode码499B,GB18030码FE8D】");
    puaOrDeletedGBKCharacter.insert(0xE85A,"PUA码字符,建议使用正式码【䱷,Unicode码4C77,GB18030码FE96】");
    puaOrDeletedGBKCharacter.insert(0xE857,"PUA码字符,建议使用正式码【䲟,Unicode码4C9F,GB18030码FE93】");
    puaOrDeletedGBKCharacter.insert(0xE858,"PUA码字符,建议使用正式码【䲠,Unicode码4CA0,GB18030码FE94】");
    puaOrDeletedGBKCharacter.insert(0xE859,"PUA码字符,建议使用正式码【䲡,Unicode码4CA1,GB18030码FE95】");
    puaOrDeletedGBKCharacter.insert(0xE856,"PUA码字符,建议使用正式码【䲣,Unicode码4CA3,GB18030码FE92】");
    puaOrDeletedGBKCharacter.insert(0xE85B,"PUA码字符,建议使用正式码【䲢,Unicode码4CA2,GB18030码FE97】");
    puaOrDeletedGBKCharacter.insert(0xE85C,"PUA码字符,建议使用正式码【䴓,Unicode码4D13,GB18030码FE98】");
    puaOrDeletedGBKCharacter.insert(0xE85D,"PUA码字符,建议使用正式码【䴔,Unicode码4D14,GB18030码FE99】");
    puaOrDeletedGBKCharacter.insert(0xE85E,"PUA码字符,建议使用正式码【䴕,Unicode码4D15,GB18030码FE9A】");
    puaOrDeletedGBKCharacter.insert(0xE85F,"PUA码字符,建议使用正式码【䴖,Unicode码4D16,GB18030码FE9B】");
    puaOrDeletedGBKCharacter.insert(0xE860,"PUA码字符,建议使用正式码【䴗,Unicode码4D17,GB18030码FE9C】");
    puaOrDeletedGBKCharacter.insert(0xE861,"PUA码字符,建议使用正式码【䴘,Unicode码4D18,GB18030码FE9D】");
    puaOrDeletedGBKCharacter.insert(0xE862,"PUA码字符,建议使用正式码【䴙,Unicode码4D19,GB18030码FE9E】");
    puaOrDeletedGBKCharacter.insert(0xE863,"PUA码字符,建议使用正式码【䶮,Unicode码4DAE,GB18030码FE9F】");
    //另外一批PUA双码字,这一批不怎么常用
    puaOrDeletedGBKCharacter.insert(0xE26E,"PUA码字符,建议使用正式码【𫢸,Unicode码2B8B8,GB18030码9839C534】");
    puaOrDeletedGBKCharacter.insert(0xE278,"PUA码字符,建议使用正式码【𪟝,Unicode码2A7DD,GB18030码98368F39】");
    puaOrDeletedGBKCharacter.insert(0xE284,"PUA码字符,建议使用正式码【𫭟,Unicode码2BB5F,GB18030码99308B33】");
    puaOrDeletedGBKCharacter.insert(0xE295,"PUA码字符,建议使用正式码【鿍,Unicode码9FCD,GB18030码82359332】");
    puaOrDeletedGBKCharacter.insert(0xE297,"PUA码字符,建议使用正式码【𫭼,Unicode码2BB7C,GB18030码99308E32】");
    puaOrDeletedGBKCharacter.insert(0xE2A5,"PUA码字符,建议使用正式码【𫮃,Unicode码2BB83,GB18030码99308E39】");
    puaOrDeletedGBKCharacter.insert(0xE2BA,"PUA码字符,建议使用正式码【𪤗,Unicode码2A917,GB18030码9836AF33】");
    puaOrDeletedGBKCharacter.insert(0xE2C8,"PUA码字符,建议使用正式码【𫰛,Unicode码2BC1B,GB18030码99309E31】");
    /*10个竖排标点和8个汉字构件*/
    puaOrDeletedGBKCharacter.insert(0xE7C7,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE78D,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE78E,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE78F,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE790,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE791,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE792,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE793,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE794,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE795,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE796,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE81E,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE826,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE82B,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE82C,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE832,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE843,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE854,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE864,"PUA码字符");

    /*6个汉字构件*/
    puaOrDeletedGBKCharacter.insert(0xE816,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE817,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE818,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE831,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE83B,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE855,"PUA码字符");
}
