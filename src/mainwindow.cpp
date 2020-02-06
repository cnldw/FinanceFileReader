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
#include "src/mainwindow.h"
#include "ui_mainwindow.h"
#define UNUSED(x) (void)x

/**
 * @brief MainWindow::MainWindow 构造函数，主窗口入口----注意，本程序的主窗口初始化就是靠这个函数完成的，里面初始化了表格参数，表格右键菜单，加载了配置文件等等
 * @param argc
 * @param argv
 * @param parent
 */

MainWindow::MainWindow(int argc, char *argv[],QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle(appName);
    //调教字体大小差异,在mac/linux系统上字体默认显示很大，强制缩放下
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:12px");
    ui->statusBar->setStyleSheet("font-size:12px");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
    ui->statusBar->setStyleSheet("font-size:13px");
#endif
    //临时隐藏未开发完毕的功能--高级文件打开
    //////////////////////////////////插件功能/////////////////
    //仅Windows系统支持文件比对和在文本编辑器中打开
#ifdef Q_OS_LINUX
    ui->actiondifftools->setVisible(false);
    ui->actionfileedit->setVisible(false);
#endif
#ifdef Q_OS_MAC
    ui->actiondifftools->setVisible(false);
    ui->actionfileedit->setVisible(false);
#endif
    //////////////////////////////////////////////////////////
    //搜索框提示
    ui->lineTextText->setToolTip("输入你要搜索的内容,数值类搜索请不要输入千位分隔符~~");
    ui->actionfileOpenAdv->setVisible(false);
    //隐藏分页组件--只有当页数大于1的时候才显示
    ui->framePage->setVisible(false);
    //指向表格控件的指针
    ptr_table=ui->tableWidget;
    ptr_table->setAlternatingRowColors(true);
    tableHeight=ptr_table->height();
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    //强制让表格获取焦点,避免在mac上打开工具时第一个输入框获取焦点显得很难看,强迫症系列
    ptr_table->setFocus();
    //初始化表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowDetails = new QAction(tr("查看此行记录"),this);
    connect(action_ShowDetails, SIGNAL(triggered()), this, SLOT(showRowDetails()));
    action_ShowCopyColum = new QAction(tr("复制"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    action_ShowOFDAnalysis = new QAction(tr("OFD字段合法分析"),this);
    connect(action_ShowOFDAnalysis, SIGNAL(triggered()), this, SLOT(showOFDFiledAnalysis()));
    action_EditCompareData= new QAction(tr("将此行数据加入比对器"),this);
    connect(action_EditCompareData, SIGNAL(triggered()), this, SLOT(editCompareData()));
    action_EditCompareDataBatch= new QAction(tr("将选择的行批量加入比对器"),this);
    connect(action_EditCompareDataBatch, SIGNAL(triggered()), this, SLOT(editCompareData()));
    action_ModifyOFDCell= new QAction(tr("编辑光标所在单元格"),this);
    connect(action_ModifyOFDCell, SIGNAL(triggered()), this, SLOT(showModifyOFDCell()));
    action_ModifyOFDCellBatch= new QAction(tr("批量编辑选中单元格"),this);
    connect(action_ModifyOFDCellBatch, SIGNAL(triggered()), this, SLOT(showModifyOFDCellBatch()));
    action_DeleteOFDRowData= new QAction(tr("从文件中删除光标所在行的整行记录"),this);
    connect(action_DeleteOFDRowData, SIGNAL(triggered()), this, SLOT(deleteRowDataFromFileAndTable()));
    action_CopyOFDRowData= new QAction(tr("复制光标所在整行原始数据(专用于合并OFD数据)"),this);
    connect(action_CopyOFDRowData, SIGNAL(triggered()), this, SLOT(copyOFDRowData()));
    action_addCopyedOFDData2PreviousRow= new QAction(tr("将剪切板上的数据插入到此行上方"),this);
    connect(action_addCopyedOFDData2PreviousRow, SIGNAL(triggered()), this, SLOT(addOFDRowDataPreviousRow()));
    action_addCopyedOFDData2NextRow= new QAction(tr("将剪切板上的数据插入到此行下方"),this);
    connect(action_addCopyedOFDData2NextRow, SIGNAL(triggered()), this, SLOT(addOFDRowDataNextRow()));
    action_addCopyedOFDData2End= new QAction(tr("将剪切板上的数据插入到文件尾"),this);
    connect(action_addCopyedOFDData2End, SIGNAL(triggered()), this, SLOT(addOFDRowDataEndRow()));
    action_addNewLineOFDData2End= new QAction(tr("新增一行数据记录到文件尾"),this);
    connect(action_addNewLineOFDData2End, SIGNAL(triggered()), this, SLOT(addOFDNewLineRowDataEndRow()));
    action_ModifyOFDRow= new QAction(tr("编辑光标所在的整行数据"),this);
    connect(action_ModifyOFDRow, SIGNAL(triggered()), this, SLOT(showMoaifyOFDRow()));

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
    tips.append("你可以在"+Utils::getConfigPath()+"目录下进行配置文件修改,可以让工具支持新增的各种OFD文件...");
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
    tips.append("基金、银行理财领域的OFD文件,本程序都支持解析哟...");
    tips.append("需要打开超大文件?,建议在设置里设置开启压缩模式和分页支持,支持的文件大小立即提升到GB级别...");
    tips.append("本程序是业余无偿开发的,如果程序帮助到了你,你可以选择小额捐赠给予支持,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("可以使用本程序新建OFD文件,以及初始化自己的新建模板...");
#ifdef Q_OS_WIN32
    tips.append("同时拖放两个文件到程序主窗口,将使用文件比对插件自动比对两个文件的差异...");
    tips.append("如果你要查看接口文件的原始数据,不妨在附加工具菜单下点击\"在文本编辑器中打开当前文件\"...");
#endif
    //监控表格进度条的变化
    connect (ptr_table->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(acceptVScrollValueChanged(int)));
    //开始初始化状态栏
    initStatusBar();
    //开始进行配置加载
    //加载程序设置
    load_Setting();
    /*加载完设置后进行设置应用*/
    //默认页面视图模式
    if(this->defaultViewMode=="1"){
        ui->frameInfo->setHidden(true);
        ui->viewMode->setText("标准视图");
    }
    //每页行数
    if(this->defaultPageSizeType=="0"){
        ui->labelPageSize->setText("(10万行/页)");
        this->pageRowSize=100000;
    }
    else if(this->defaultPageSizeType=="1"){
        ui->labelPageSize->setText("(20万行/页)");
        this->pageRowSize=200000;
    }
    else if(this->defaultPageSizeType=="2"){
        ui->labelPageSize->setText("(50万行/页)");
        this->pageRowSize=500000;
    }
    else if(this->defaultPageSizeType=="3"){
        ui->labelPageSize->setText("(100万行/页)");
        this->pageRowSize=1000000;
    }
    else{
        ui->labelPageSize->setText("(10万行/页)");
        this->pageRowSize=100000;
    }
    //加载基金销售商和TA代码信息
    load_OFDCodeInfo();
    //加载OFD文件类别信息
    load_OFDIndexFile();
    //加载各类OFD文件的定义
    load_OFDDefinition();
    //加载OFD字典
    load_OFDDictionary();
    //加载各类CSV文件的定义
    load_CSVDefinition();
    //加载各类定长文件的定义
    load_FIXEDDefinition();
    //配置加载完毕
    configLoadCompleted=true;
    //随机提醒
    randomTips();
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
    ui->statusBar->setStyleSheet("font-family:Microsoft YaHei,Sans-serif;");
    //显示总记录数的标签
    statusLabel_ptr_showCount = new QLabel;
    statusLabel_ptr_showCount->setMinimumSize(210, 20); // 设置标签最小大小
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
    showMessagePopMenu->addAction(action_ShowCopy);
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
        DialogMyTip * dialog2 = new DialogMyTip("同时拖放了多个文件进来，你需要对比这些文件的差异么？,如果你不想使用对比工具对比文件而是要解析文件，请每次拖放一个文件进来！",this);
        dialog2->setWindowTitle("警告！");
        dialog2->setModal(true);
        dialog2->exec();
        if(dialog2->getBoolFlag()){
            //模态的使用完毕删除
            delete dialog2;
            dialog2=nullptr;
            //调用打开Winmerge然后return
            //拼接调用路径，包含文件路径
            QString winmergepath=QApplication::applicationDirPath()+"/plugin/WinMerge/WinMergeU.exe";
            if(Utils::isFileExist(winmergepath)){
                QProcess process;
                process.startDetached(winmergepath,pars);
            }else{
                statusBar_disPlayMessage("找不到WinMerge插件,请安装到plugin目录下！");
            }
            return;
        }
        else{
            //否,提示用户
            delete dialog2;
            dialog2=nullptr;
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
    if(defaultNewFileMode=="1"){
        //windows特殊处理
#ifdef Q_OS_WIN32
        fileNameFromDrag=fileNameFromDrag.replace("\\","/");
#endif
        //判断是否是同一个文件--不是同一个--则新实例打开
        if(fileNameFromDrag!=currentOpenFilePath&&!currentOpenFilePath.isEmpty()&&currentOpenFileType!=-1){
            qDebug()<<"即将启动新的实例,从"<<QCoreApplication::applicationFilePath();
            QString program=QCoreApplication::applicationFilePath();
            QStringList arguments;
            arguments<<(QCoreApplication::applicationFilePath())<<fileNameFromDrag;
            QProcess *Process = new QProcess(nullptr);
            Process->startDetached(program, arguments);
            return;
        }
    }
    //只要是新文件直接新实例打开
    else if (defaultNewFileMode=="2"&&!currentOpenFilePath.isEmpty()&&currentOpenFileType!=-1){
        qDebug()<<"即将启动新的实例,从"<<QCoreApplication::applicationFilePath();
        QString program=QCoreApplication::applicationFilePath();
        QStringList arguments;
        arguments<<(QCoreApplication::applicationFilePath())<<fileNameFromDrag;
        QProcess *Process = new QProcess(nullptr);
        Process->startDetached(program, arguments);
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
    clear_oldData();
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
 * @brief MainWindow::resizeEvent窗口缩放处理事件,当窗口缩放时，界面能显示的内容变化，需要重新调用display函数去显示数据
 * @param event
 */
void MainWindow:: resizeEvent (QResizeEvent * event ){
    event->ignore();
    if(tableHeight!=ptr_table->height()&&!isUpdateData){
        //获取当前table的高度
        int higth=ptr_table->size().height();
        tableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
        tableRowEnd=tableRowBegin+(higth/rowHight);
        //不同数据类型插入点
        if(currentOpenFileType==1){
            display_OFDTable();
        }
        if(currentOpenFileType==2){
            display_CSVTable();
        }
        if(currentOpenFileType==3){
            display_FIXEDTable();
        }
    }
}

/**
 * @brief MainWindow::acceptVScrollValueChanged主界面表格滚动事件，滚动时判断当前显示范围，实现异步按需加载
 * @param value
 */
void MainWindow::acceptVScrollValueChanged(int value)
{
    UNUSED (value);
    //正在更新表/数据源时不开启自动加载
    if(!isUpdateData){
        //新的起始位置
        tableRowBegin=ptr_table->rowAt(ptr_table->verticalScrollBar()->y());
        //获取当前table的高度
        int higth=ptr_table->size().height();
        //计算要结束的行
        tableRowEnd=tableRowBegin+(higth/rowHight);
        //不同数据类型插入点
        if(currentOpenFileType==1){
            display_OFDTable();
        }
        if(currentOpenFileType==2){
            display_CSVTable();
        }
        if(currentOpenFileType==3){
            display_FIXEDTable();
        }
    }
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
    QString openpath="./";
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
 * @brief MainWindow::statusBar_display_rowsCount状态栏显示总行数
 * @param rowsCount
 */
void MainWindow::statusBar_display_rowsCount(int rowsCount){
    int count=(rowsCount + pageRowSize - 1) / pageRowSize;
    statusLabel_ptr_showCount->setText(tr("记录数:%1行,共计%2页").arg(QString::number(rowsCount, 10)).arg(count));
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
 * @brief MainWindow::clear_oldData重新加载数据时，清除旧数据将会加载到内存的数据全部清除-重新加载文件前请务必调用此方法
 */
void MainWindow::clear_oldData(){
    //不同数据类型插入点
    ofdIndexFileHeaderMap.clear();
    indexFileDataList.clear();
    ofdFileHeaderQStringList.clear();
    ofdFileContentQByteArrayList.clear();
    csvFileHeaderQStringList.clear();
    csvFileContentQByteArrayList.clear();
    fixedFileHeaderQStringList.clear();
    fixedContenQByteArrayList.clear();
    fixedFooterQStringList.clear();
    rowHasloaded.clear();
    compareData.clear();
    columnWidth.clear();
    fieldIsNumberOrNot.clear();
    //记录当前所在行
    tableRowCurrent=0;
    //当前所在列
    tableColCurrent=0;
    //更新列跳转搜索开始列
    tableColSearch=0;
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
    /////////////////////////////////////////////
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
                        this->defaultViewMode="0";
                    }else{
                        this->defaultViewMode=value;
                    }
                }
                //默认新文件打开方式
                if(key=="defaultnewfilemode"){
                    if(value!="0"&&value!="1"&&value!="2"){
                        this->defaultNewFileMode="0";
                    }else{
                        this->defaultNewFileMode=value;
                    }
                }
                //默认每页行数类别
                if(key=="defaultpagesizetype"){
                    if(value!="0"&&value!="1"&&value!="2"&&value!="3"){
                        this->defaultPageSizeType="0";
                    }else{
                        this->defaultPageSizeType=value;
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
        loadedSettingInfoIni.endGroup();
        loadedSettingInfoIni.sync();
    }
}

/**
 * @brief MainWindow::load_OFDCodeInfo加载ofd的代码配置信息,包含销售商，TA信息-这个数据取自tools目录下的爬虫工具-数据来源为证券市场标准网
 */
void MainWindow::load_OFDCodeInfo(){
    QString codeInipath=Utils::getConfigPath()+"OFD_CodeInfo.ini";
    if(Utils::isFileExist(codeInipath)){
        //加载ini文件
        QSettings loadedCodeInfoIni(codeInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        loadedCodeInfoIni.setIniCodec("UTF-8");
        QStringList agencyInfo=loadedCodeInfoIni.childGroups();
        //获取所有代码和代码对应的机构名称
        for(int i=0;i<agencyInfo.count();i++){
            OFDCodeInfo infoItem;
            infoItem.setCode(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString());
            infoItem.setName(loadedCodeInfoIni.value(agencyInfo.at(i)+"/NAME").toString());
            loadedOfdCodeInfo.insert(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString(),infoItem);
        }
    }else{
        statusBar_disPlayMessage(Utils::getConfigPath()+tr("OFD_CodeInfo.ini配置丢失"));
    }
}

/**
 * @brief MainWindow::load_OFDIndexFile 加载OFD的索引文件类别信息
 */
void MainWindow::load_OFDIndexFile(){
    QString fileTypeInipath=Utils::getConfigPath()+"OFD_IndexFile.ini";
    if(Utils::isFileExist(fileTypeInipath)){
        //加载ini文件
        QSettings fileTypeIni(fileTypeInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        fileTypeIni.setIniCodec("UTF-8");
        //读取OFDINDEXFILE
        fileTypeIni.beginGroup("OFDINDEXFILE");
        QStringList infoList =fileTypeIni.childKeys();
        fileTypeIni.endGroup();
        //遍历组OFDINDEXFILE
        if(infoList.count()>0){
            for(int i=0;i<infoList.count();i++){
                loadedOfdIndexFileInfo.insert(infoList.at(i),fileTypeIni.value("OFDINDEXFILE/"+infoList.at(i)).toString());
            }
        }
        infoList.clear();
    }
    else{
        statusBar_disPlayMessage(Utils::getConfigPath()+tr("OFD_IndexFile.ini配置丢失"));
    }
}

/**
 * @brief MainWindow::load_Dictionary加载OFD的字典信息
 */
void MainWindow::load_OFDDictionary(){
    QString dictionaryInipath=Utils::getConfigPath()+"OFD_Dictionary.ini";
    if(Utils::isFileExist(dictionaryInipath)){
        //加载ini文件
        QSettings loadedDictionary(dictionaryInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        loadedDictionary.setIniCodec("UTF-8");
        //获取所有fixed文件的配置信息，循环存储
        QStringList dictionaryGroup=loadedDictionary.childGroups();
        //查不到任何段落
        if(dictionaryGroup.count()<1){
            return;
        }
        //循环各个字典组
        for(int i=0;i<dictionaryGroup.count();i++){
            loadedDictionary.beginGroup(dictionaryGroup.at(i));
            QStringList allkey=loadedDictionary.allKeys();
            if(allkey.count()>0){
                for (int j=0;j<allkey.count();j++){
                    //循环插入字典
                    //字典分组_key=a:xxxb:xxx
                    ofdDictionary.addDictionary(dictionaryGroup.at(i)+"_"+allkey.at(j)+"="+loadedDictionary.value(allkey.at(j)).toString());
                }
            }
            loadedDictionary.endGroup();
        }
    }
    else{
        statusBar_disPlayMessage(Utils::getConfigPath()+tr("OFD_Dictionary.ini配置丢失"));
    }
}

/**
 * @brief MainWindow::load_OFDDefinition加载OFD文件的定义
 */
void MainWindow::load_OFDDefinition(){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "OFD_*.ini";//设置过滤类型
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList list = dirPath.entryInfoList();
    if(list.length()!=0){
        //循环检索config目录下可用的接口定义配置
        //OFD开头ini结尾,形式类似于OFD_XXX.ini
        /*-------------------------------开启第一重循环,遍历config下的OFD配置文件-----------------------------------*/
        for (int f = 0; f < list.size(); f++)
        {
            QString fileName=list.at(f).fileName();
            QString fixName=fileName;
            //排除几个文件
            if(fileName=="OFD_CodeInfo.ini"||fileName=="OFD_Dictionary.ini"||fileName=="OFD_IndexFile.ini"){
                continue;
            }
            if(fileName.startsWith("OFD",Qt::CaseInsensitive)&&fileName.endsWith("ini",Qt::CaseInsensitive)){
                //编辑加载文件
                if(fixName.contains(".",Qt::CaseInsensitive)){
                    fixName=fixName.mid(0,fixName.indexOf("."));
                }
                QStringList nameList=fixName.split("_");
                if(nameList.count()==2){
                    //加载ini文件
                    QString prefixName=nameList.at(1);
                    QSettings ofdIni(Utils::getConfigPath()+fileName,QSettings::IniFormat,nullptr);
                    //目前仅接收UTF-8编码的配置文件
                    ofdIni.setIniCodec("UTF-8");
                    QStringList interfaceList=ofdIni.childGroups();
                    //获取所有文件类别定义
                    if(interfaceList.count()>0){
                        /*-----------------第二重循环，遍历OFD文件内的文件定义----------------------*/
                        /* /////////////////////////开始解析本文件可用接口文件定义////////// */
                        for(int i=0;i<interfaceList.count();i++){
                            //存入QHash使用的key
                            QString name=prefixName+"_"+interfaceList.at(i);
                            OFDFileDefinition ofd;
                            QList<OFDFieldDefinition> fieldList;
                            QString message;
                            bool okFlag=false;
                            //记录每行的长度，自动总数
                            int rowLength=0;
                            int fieldCount=0;
                            //文件说明
                            QString describe=ofdIni.value(interfaceList.at(i)+"/DESCRIBE").toString();
                            if (!describe.isEmpty()){
                                ofd.setDescribe(describe);
                            }
                            else{
                                ofd.setDescribe("未配置说明的文件");
                            }
                            //字典类型
                            QString dictionary=ofdIni.value(interfaceList.at(i)+"/DICTIONARY").toString();
                            //设置解析字典类型
                            ofd.setDictionary(dictionary.isEmpty()?"":dictionary);
                            //查找COUNT标记
                            QString countStr=ofdIni.value(interfaceList.at(i)+"/COUNT").toString();
                            if(countStr.isEmpty()){
                                message="找不到"+interfaceList.at(i)+"文件的字段总数配置";
                                okFlag=false;
                            }else{
                                bool ok;
                                int countInt=countStr.toInt(&ok,10);
                                if(ok&&countInt>0){
                                    fieldCount=countInt;
                                    /*------第三重循环--开始按行解析此接口文件的定义----------------*/
                                    //解析规则-一旦解析到一个失败的记录，就不再往下解析了
                                    for(int j=1;j<=countInt;j++){
                                        OFDFieldDefinition ofdfield;
                                        //获取这个文件定义的第i个字段的信息
                                        QStringList iniStrList=ofdIni.value(interfaceList.at(i)+"/"+QString::number(j)).toStringList();
                                        if (iniStrList.isEmpty()||iniStrList.length()!=5){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段定义不正确或者缺失";
                                            okFlag=false;
                                            break;
                                        }
                                        //获取字符类型参数
                                        QString fieldType=(QString(iniStrList.at(0)));
                                        if(fieldType.length()<1){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的类型定义缺失,请配置";
                                            okFlag=false;
                                            break;
                                        }
                                        //字段长度
                                        bool lengthOk;
                                        int length=(QString(iniStrList.at(1))).toInt(&lengthOk,10);
                                        if(!lengthOk){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的长度定义不是整数";
                                            okFlag=false;
                                            break;
                                        }
                                        //字段小数长度
                                        bool declengthOk;
                                        int declength=(QString(iniStrList.at(2))).toInt(&declengthOk,10);
                                        if(!declengthOk){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的小数长度定义不是整数";
                                            okFlag=false;
                                            break;
                                        }
                                        if(fieldType=="N"&&(declength>=length)){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的小数长度定义应该小于字段总长度";
                                            okFlag=false;
                                            break;
                                        }
                                        //历经千辛万苦经理校验无误后存入字段信息
                                        //每个字段开始的位置就是截至到上一个字段结束的长度
                                        //比如第一个字段长度为2,则第二个字段在此行记录中的起始坐标就是2（0,1这两个字节是第一个字段的索引）
                                        ofdfield.setRowBeginIndex(rowLength);
                                        rowLength+=length;
                                        ofdfield.setLength(length);
                                        ofdfield.setDecLength(declength);
                                        ofdfield.setFieldType(fieldType);
                                        ofdfield.setFieldName(QString(iniStrList.at(4)));
                                        ofdfield.setFieldDescribe(QString(iniStrList.at(3)));
                                        fieldList.append(ofdfield);
                                        //更新记录为成功
                                        okFlag=true;
                                    }
                                }
                                else{
                                    message=interfaceList.at(i)+"文件的字段总数配置COUNT标记不是有效的整数";
                                    okFlag=false;
                                }
                            }
                            //写入该文件的配置
                            //如果记录完整没错误则写入
                            if(okFlag){
                                //记录数的长度/////////20190723///////////
                                QString rowcountLengthStr=ofdIni.value(interfaceList.at(i)+"/ROWCOUNTLENGTH").toString();
                                if(rowcountLengthStr.isEmpty()){
                                    //找不到就默认设置为8
                                    ofd.setRowCountLength(8);
                                }else{
                                    bool ok;
                                    int rowcountLengthInt=rowcountLengthStr.toInt(&ok,10);
                                    if(ok&&rowcountLengthInt>0){
                                        ofd.setRowCountLength(rowcountLengthInt);
                                    }
                                    else{
                                        //读取配置错误也设置为8
                                        ofd.setRowCountLength(8);
                                    }
                                }
                                ///////////////////////////
                                ofd.setUseAble(okFlag);
                                ofd.setMessage(message);
                                ofd.setFieldCount(fieldCount);
                                ofd.setRowLength(rowLength);
                                ofd.setFieldList(fieldList);
                                loadedOfdDefinitionHash.insert(name,ofd);
                            }
                            //如果记录有错误,则仅写于不可用和错误原因
                            else{
                                ofd.setUseAble(okFlag);
                                ofd.setMessage(message);
                                loadedOfdDefinitionHash.insert(name,ofd);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief MainWindow::load_CSVDefinition加载CSV文件的定义
 */
void MainWindow::load_CSVDefinition(){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "CSV_*.ini";//设置过滤类型
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList listFile = dirPath.entryInfoList();
    if(listFile.length()!=0){
        //循环检索config目录下可用的CSV接口定义配置
        for (int f = 0; f < listFile.size(); f++)
        {
            QString fileName=listFile.at(f).fileName();
            QString codeInipath=Utils::getConfigPath()+fileName;
            if(Utils::isFileExist(codeInipath)){
                //加载ini文件
                QSettings loadedCsvInfoIni(codeInipath,QSettings::IniFormat,nullptr);
                //目前仅接收UTF-8编码的配置文件
                loadedCsvInfoIni.setIniCodec("UTF-8");
                QStringList csvInfo=loadedCsvInfoIni.childGroups();
                if(csvInfo.count()<1){
                    continue;
                }
                //获取所有csv文件的配置信息，循环存储
                for(int i=0;i<csvInfo.count();i++){
                    QString csvType=QString(csvInfo.at(i));
                    if(csvType.contains("|",Qt::CaseSensitive)){
                        //建立一个定义变量
                        CsvFileDefinition fileDef;
                        //本配置来自的配置文件
                        fileDef.setFileIni(fileName);
                        //分割文件名和字段数
                        QStringList list=csvType.split("|");
                        //1解析器可接受的文件名
                        fileDef.setFileName(QString (list.at(0)));
                        //2解析器配置全名
                        fileDef.setFileNameWithCount(csvType);
                        //3文件描述信息
                        QString fileDescribe=loadedCsvInfoIni.value(csvType+"/filedescribe").toString();
                        if(fileDescribe.isEmpty()){
                            fileDef.setFileDescribe("未说明的CSV文件");
                            loadedCsvFileInfo.insert(list.at(0),QString("未说明的CSV文件"));
                        }else{
                            fileDef.setFileDescribe(fileDescribe);
                            loadedCsvFileInfo.insert(list.at(0),fileDescribe);
                        }
                        //5分隔符
                        QString splitflag=loadedCsvInfoIni.value(csvType+"/splitflag").toString();
                        //注意不要乱删除这几行代码，qt有个bug，ini配置里某个字段的值是,(英文逗号)获取时会获取到空值，坑！
                        //如果列分割为配置符号为空，则分隔符配置设置为,
                        if(splitflag.isEmpty()){
                            splitflag=",";
                        }
                        fileDef.setSplit(splitflag);
                        //5是否以分隔符做结尾
                        QString endwithflag=loadedCsvInfoIni.value(csvType+"/endwithflag").toString();
                        fileDef.setEndwithflag(endwithflag);
                        //6数据起始行
                        bool beginflagok;
                        int  beginrowindex=loadedCsvInfoIni.value(csvType+"/datarowbeginindex").toInt(&beginflagok);
                        if(!beginflagok){
                            fileDef.setDatabeginrowindex(0);
                        }
                        //如果数据起始行小于1，则强制为1
                        else if(beginrowindex<1){
                            fileDef.setDatabeginrowindex(1);
                        }else{
                            fileDef.setDatabeginrowindex(beginrowindex);
                        }
                        //7标题行
                        bool titleflagok;
                        int  titlerowindex=loadedCsvInfoIni.value(csvType+"/titlerowindex").toInt(&titleflagok);
                        if(!titleflagok){
                            fileDef.setTitlerowindex(0);
                        }
                        //如果不存在标题栏目，则标题行强制设置为0
                        else if(titlerowindex<1){
                            fileDef.setTitlerowindex(0);
                        }else{
                            fileDef.setTitlerowindex(titlerowindex);
                        }
                        //8编码信息
                        QString encoding=loadedCsvInfoIni.value(csvType+"/encoding").toString();
                        if(encoding.isEmpty()){
                            fileDef.setEcoding("UTF-8");
                        }
                        else{
                            fileDef.setEcoding(encoding);
                        }
                        //9数据尾部忽略行
                        bool flag=false;
                        QString ignorerow=loadedCsvInfoIni.value(csvType+"/endignorerow").toString();
                        int endignoreRow;
                        if(ignorerow.isEmpty()){
                            //不配置就不忽略任何行
                            fileDef.setEndIgnoreRow(0);
                        }
                        else{
                            endignoreRow=ignorerow.toInt(&flag);
                            if(!flag){
                                fileDef.setUseAble(false);
                                fileDef.setMessage("文件结尾忽略行数不是一个可用的数值");
                                loadedCsvDefinitionList.append(fileDef);
                                continue;
                            }
                            else{
                                if(endignoreRow<0){
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage("文件结尾忽略行数应当是一个大于等于0的整数");
                                    loadedCsvDefinitionList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setEndIgnoreRow(endignoreRow);
                                }
                            }
                        }
                        //////////////////////////////////////
                        //10字段总数
                        QString fieldCcountStr=loadedCsvInfoIni.value(csvType+"/fieldcount").toString();
                        int fieldCount=fieldCcountStr.toInt(&flag,10);
                        //字段描述值正确
                        if(flag&&fieldCount>0){
                            //4字段总数
                            fileDef.setFieldCount(fieldCount);
                            QList <CsvFieldDefinition> fieldList;
                            //10开始循环加载本文件类型的字段信息///////////////
                            for(int r=0;r<fieldCount;r++){
                                CsvFieldDefinition fieldItem;
                                QString name=loadedCsvInfoIni.value(csvType+"/"+(QString::number(r+1,10))).toString();
                                if(name.isEmpty()){
                                    fieldItem.setFieldName("未定义的字段名");
                                }
                                else{
                                    fieldItem.setFieldName(name);
                                }
                                //添加此字段信息到文件定义
                                fieldList.append(fieldItem);
                            }
                            fileDef.setFieldList(fieldList);
                            fileDef.setUseAble(true);
                            loadedCsvDefinitionList.append(fileDef);
                        }
                        //如果字段数是AUTO，则字段总和设置为-1，在进行文件解析是自动分析标题数量来进行解析
                        //这种模式在带文件标题的情况下，自动解析标题，如果不带标题，则解析是标题全部描述为第XX列
                        else if(fieldCcountStr.toUpper()=="AUTO"){
                            fileDef.setFieldCount(-1);
                            fileDef.setUseAble(true);
                            loadedCsvDefinitionList.append(fileDef);
                        }
                        else{
                            fileDef.setUseAble(false);
                            fileDef.setMessage("字段总数描述不是正确可用的数值(字段数需为大于0的整数)或者为AUTO(自动解析列数形式)");
                            loadedCsvDefinitionList.append(fileDef);
                        }
                    }
                    else
                        //判断是否是字典配置，字典配置是csv配置文件里的一段特殊配置，使用Dictionary标注
                    {
                        if(csvType=="Dictionary"){
                            Dictionary dic;
                            //开始循环读取字典
                            //不要尝试使用begingroup后直接读取组内参数，否则会读取不到包含逗号的键值
                            loadedCsvInfoIni.beginGroup("Dictionary");
                            QStringList infoList =loadedCsvInfoIni.childKeys();
                            loadedCsvInfoIni.endGroup();
                            if(infoList.count()>0){
                                for(int aaa=0;aaa<infoList.count();aaa++){
                                    //ini配置中如果key存在中文会出现错误，所以我们使用1=key@value的方式去改造
                                    QString line=loadedCsvInfoIni.value("Dictionary/"+infoList.at(aaa)).toString();
                                    int index=line.indexOf('@');
                                    if(index>0){
                                        //只替换第一个出现的@，以免替换了字典里的其他@符号
                                        dic.addDictionary(line.replace(index,1,'='));
                                    }
                                    //不包含@的跳过
                                    else{
                                        continue;
                                    }
                                }
                                //将字典加入qhash/csv类的字典以csv开头
                                commonDictionary.insert("CSV"+fileName,dic);
                            }
                            infoList.clear();
                        }
                        else{
                            //无效配置段落，跳过
                            continue;
                        }
                    }
                }
            }
        }
        //对加载到信息进行按通配符号的数量进行排序
        //优先适配通配符少的更明确的文件
        int listcount=loadedCsvDefinitionList.count();
        if(listcount>1){
            for(int nn=0;nn<listcount-1;nn++){
                int lastsmallIndex=nn;
                int small=nn;
                for(int vv=nn;vv<listcount-1;vv++){
                    if(loadedCsvDefinitionList.at(vv+1).getFileName().count("*")<loadedCsvDefinitionList.at(lastsmallIndex).getFileName().count("*")){
                        lastsmallIndex=vv+1;
                        small=vv+1;
                    }
                }
                //如果发生了交换
                if(small!=nn){
                    CsvFileDefinition exchange=loadedCsvDefinitionList.at(nn);
                    loadedCsvDefinitionList.replace(nn,loadedCsvDefinitionList.at(small));
                    loadedCsvDefinitionList.replace(small,exchange);
                }
            }
        }
    }
}

/**
 * @brief MainWindow::load_FIXEDDefinition加载定长文件的定义
 */
void MainWindow::load_FIXEDDefinition(){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "FIXED_*.ini";//设置过滤类型,以FIXED_开头的配置
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList listFile = dirPath.entryInfoList();
    if(listFile.length()!=0){
        //循环检索config目录下可用的FIXED接口定义配置
        for (int f = 0; f < listFile.size(); f++)
        {
            //获取文件名
            QString fileName=listFile.at(f).fileName();
            //获取文件路径
            QString codeInipath=Utils::getConfigPath()+fileName;
            if(Utils::isFileExist(codeInipath)){
                //加载ini文件
                QSettings loadedFixedInfoIni(codeInipath,QSettings::IniFormat,nullptr);
                //目前仅接收UTF-8编码的配置文件
                loadedFixedInfoIni.setIniCodec("UTF-8");
                //获取所有fixed文件的配置信息，循环存储
                QStringList fixedInfo=loadedFixedInfoIni.childGroups();
                if(fixedInfo.count()<1){
                    continue;
                }
                //遍历所有组
                for(int i=0;i<fixedInfo.count();i++){
                    //配置验证失败标志
                    bool flag=false;
                    //配置验证失败原因
                    QString message;
                    //每行最大列数
                    int MaxCount=0;
                    //文件类别
                    QString fixedType=QString(fixedInfo.at(i));
                    if(fixedType.contains("|",Qt::CaseSensitive)){
                        //建立一个定义变量
                        FIXEDFileDefinition fileDef;
                        //本配置来自的配置文件
                        fileDef.setFileIni(fileName);
                        //分割文件名和版本号
                        QStringList list=fixedType.split("|");
                        //1解析器可接受的文件名
                        fileDef.setFileName(QString (list.at(0)));
                        //2解析器配置全名包含版本号
                        fileDef.setFileNameWithVersion(fixedType);
                        //3文件描述信息
                        QString fileDescribe=loadedFixedInfoIni.value(fixedType+"/filedescribe").toString();
                        if(fileDescribe.isEmpty()){
                            fileDef.setFileDescribe("未说明的定长文件");
                            loadedFixedFileInfo.insert(list.at(0),QString("未说明的定长文件"));
                        }else{
                            fileDef.setFileDescribe(fileDescribe);
                            loadedFixedFileInfo.insert(list.at(0),fileDescribe);
                        }
                        //4编码信息,获取不到则默认为GB18030编码
                        QString encoding=loadedFixedInfoIni.value(fixedType+"/encoding").toString();
                        if(encoding.isEmpty()){
                            fileDef.setEcoding("GB18030");
                        }
                        else{
                            fileDef.setEcoding(encoding);
                        }
                        //5定长分类，字节定长还是字符定长/获取不到定长分配时，则默认为字节定长
                        QString fieldLengthType=loadedFixedInfoIni.value(fixedType+"/fieldlengthtype").toString();
                        if(fieldLengthType.isEmpty()){
                            fileDef.setFieldlengthtype("0");
                        }
                        else{
                            fileDef.setFieldlengthtype(fieldLengthType);
                        }
                        //////////////////////////////////////
                        //6字段总数////存在多种长度的可能
                        QString fieldcount=loadedFixedInfoIni.value(fixedType+"/fieldcount").toString();
                        //找不到每行支持的列数，则无效配置
                        if(fieldcount.isEmpty()){
                            message="配置中找不到每行字段数的配置,无法界定每行的字段数!";
                            fileDef.setUseAble(false);
                            fileDef.setMessage(message);
                            loadedFixedDefinitionList.append(fileDef);
                            continue;
                        }
                        else{
                            //获取字段总数，有可能有多个
                            QStringList fieldCountListStr=fieldcount.split("|");
                            QList<int> fieldCountList;
                            //遍历支持的长度
                            for(int ll=0;ll<fieldCountListStr.count();ll++){
                                if(!QString(fieldCountListStr.at(ll)).isEmpty()){
                                    int fieldCount=(fieldCountListStr.at(ll)).toInt(&flag,10);
                                    //一旦遇到无法识别的字段和，则退出
                                    if(!flag){
                                        message="字段总数描述存在不可用的数值(字段数需为大于0的整数),如果有多种字段数则使用|分开维护";
                                        break;
                                    }
                                    else{
                                        if(fieldCount>MaxCount){
                                            MaxCount=fieldCount;
                                        }
                                        fieldCountList.append(fieldCount);
                                    }
                                }
                            }
                            if(!flag){
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                loadedFixedDefinitionList.append(fileDef);
                                continue;
                            }
                            else{
                                fileDef.setFieldCountMax(MaxCount);
                                fileDef.setFieldCountList(fieldCountList);
                            }
                        }
                        //7数据起始行
                        int dataRowBeginIndex=loadedFixedInfoIni.value(fixedType+"/datarowbeginindex").toInt(&flag);
                        if(!flag){
                            message="数据起始行标志不是一个可用的数值";
                            fileDef.setUseAble(false);
                            fileDef.setMessage(message);
                            loadedFixedDefinitionList.append(fileDef);
                            continue;
                        }
                        else{
                            if(dataRowBeginIndex<1){
                                message="数据起始行标志应当是一个大于等于1的整数";
                                flag=false;
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                loadedFixedDefinitionList.append(fileDef);
                                continue;
                            }else {
                                fileDef.setDataRowBeginIndex(dataRowBeginIndex);
                            }
                        }
                        //7数据尾部忽略行
                        QString ignorerow=loadedFixedInfoIni.value(fixedType+"/endignorerow").toString();
                        int endignoreRow;
                        if(ignorerow.isEmpty()){
                            //不配置就不忽略任何行
                            fileDef.setEndIgnoreRow(0);
                        }
                        else{
                            endignoreRow=ignorerow.toInt(&flag);
                            if(!flag){
                                message="文件结尾忽略行数不是一个可用的数值";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                loadedFixedDefinitionList.append(fileDef);
                                continue;
                            }
                            else{
                                if(endignoreRow<0){
                                    message="文件结尾忽略行数应当是一个大于等于0的整数";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    loadedFixedDefinitionList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setEndIgnoreRow(endignoreRow);
                                }
                            }
                        }
                        //字段描述值全部正确
                        if(flag){
                            int rowLength=0;
                            bool okFlag=true;
                            //循环遍历最长的字段数值
                            QList <FixedFieldDefinition> fieldList;
                            QHash<int,int> rowLengthHash;
                            //8开始循环加载本文件类型的字段信息///////////////
                            for(int r=0;r<MaxCount;r++){
                                FixedFieldDefinition fixedfield;
                                //获取这个文件定义的第i个字段的信息
                                QStringList iniStrList=loadedFixedInfoIni.value(fixedType+"/"+(QString::number(r+1,10))).toStringList();
                                if (iniStrList.isEmpty()||iniStrList.length()!=4){
                                    message="第"+QString::number(r+1,10)+"个字段定义不正确或者缺失";
                                    okFlag=false;
                                    break;
                                }
                                //获取字符类型参数
                                QString fieldType=(QString(iniStrList.at(0)));
                                if(fieldType.length()<1){
                                    message="第"+QString::number(r+1,10)+"个字段的类型定义缺失,请配置";
                                    okFlag=false;
                                    break;
                                }
                                //字段长度
                                bool lengthOk;
                                int length=(QString(iniStrList.at(1))).toInt(&lengthOk,10);
                                if(!lengthOk){
                                    message="第"+QString::number(r+1,10)+"个字段的长度定义不是整数";
                                    okFlag=false;
                                    break;
                                }
                                //字段小数长度
                                bool declengthOk;
                                int declength=(QString(iniStrList.at(2))).toInt(&declengthOk,10);
                                if(!declengthOk){
                                    message="第"+QString::number(r+1,10)+"个字段的小数长度定义不是整数";
                                    okFlag=false;
                                    break;
                                }
                                if(fieldType=="N"&&(declength>=length)){
                                    message="第"+QString::number(r+1,10)+"个字段的小数长度定义应该小于字段总长度";
                                    okFlag=false;
                                    break;
                                }
                                //历经千辛万苦经理校验无误后存入字段信息
                                //每个字段开始的位置就是截至到上一个字段结束的长度
                                //比如第一个字段长度为2,则第二个字段在此行记录中的起始坐标就是2（0,1这两个字节是第一个字段的索引）
                                fixedfield.setRowBeginIndex(rowLength);
                                rowLength+=length;
                                fixedfield.setLength(length);
                                fixedfield.setDecLength(declength);
                                fixedfield.setFieldType(fieldType);
                                fixedfield.setFieldName(QString(iniStrList.at(3)));
                                fixedfield.setFieldDescribe(QString(iniStrList.at(3)));
                                fieldList.append(fixedfield);
                                //如果存在此长度类型，则记录下行总长度,使用Qhash，行长度，字段数
                                if(fileDef.getFieldCountList().contains(r+1)){
                                    //记录长度和对应的字段数
                                    rowLengthHash.insert(rowLength,r+1);
                                }
                            }
                            //可用
                            if(okFlag){
                                fileDef.setRowLengthHash(rowLengthHash);
                                fileDef.setFieldList(fieldList);
                                fileDef.setUseAble(true);
                                loadedFixedDefinitionList.append(fileDef);
                            }
                            //不可用
                            else{
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                loadedFixedDefinitionList.append(fileDef);
                            }

                        }
                        //不可用
                        else{
                            fileDef.setUseAble(false);
                            fileDef.setMessage(message);
                            loadedFixedDefinitionList.append(fileDef);
                        }
                    }
                    //判断是否是字典配置，字典配置是csv配置文件里的一段特殊配置，使用Dictionary标注
                    else
                    {
                        if(fixedType=="Dictionary"){
                            Dictionary dic;
                            //开始循环读取字典
                            //不要尝试使用begingroup后直接读取组内参数，否则会读取不到包含逗号的键值
                            loadedFixedInfoIni.beginGroup("Dictionary");
                            QStringList infoList =loadedFixedInfoIni.childKeys();
                            loadedFixedInfoIni.endGroup();
                            if(infoList.count()>0){
                                for(int aaa=0;aaa<infoList.count();aaa++){
                                    //ini配置中如果key存在中文会出现错误，所以我们使用1=key@value的方式去改造
                                    QString line=loadedFixedInfoIni.value("Dictionary/"+infoList.at(aaa)).toString();
                                    int index=line.indexOf('@');
                                    if(index>0){
                                        //只替换第一个出现的@，以免替换了字典里的其他@符号
                                        dic.addDictionary(line.replace(index,1,'='));
                                    }
                                    //不包含@的跳过
                                    else{
                                        continue;
                                    }
                                }
                                //将字典加入qhash.FIXED文件的字典以FIXED开头
                                commonDictionary.insert("FIXED"+fileName,dic);
                            }
                            infoList.clear();
                        }
                        else{
                            //无效配置段落，跳过
                            continue;
                        }
                    }
                }
            }
        }
        //对加载到信息进行按通配符号的数量进行排序
        //优先适配通配符少的更明确的文件
        int listcount=loadedCsvDefinitionList.count();
        if(listcount>1){
            for(int nn=0;nn<listcount-1;nn++){
                int lastsmallIndex=nn;
                int small=nn;
                for(int vv=nn;vv<listcount-1;vv++){
                    if(loadedCsvDefinitionList.at(vv+1).getFileName().count("*")<loadedCsvDefinitionList.at(lastsmallIndex).getFileName().count("*")){
                        lastsmallIndex=vv+1;
                        small=vv+1;
                    }
                }
                //如果发生了交换
                if(small!=nn){
                    CsvFileDefinition exchange=loadedCsvDefinitionList.at(nn);
                    loadedCsvDefinitionList.replace(nn,loadedCsvDefinitionList.at(small));
                    loadedCsvDefinitionList.replace(small,exchange);
                }
            }
        }
    }
}


/**
 * @brief MainWindow::initFile 文件初始化方法 无论是拖拽进来的文件还是打开菜单打开的文件还是重新打开文件，最终都要从这个函数开始初始化加载文件，这个函数会对文件类型做个初步的判断，然后交付相对应的文件类别的加载方法
 * @param filePath
 */
void MainWindow::initFile(QString filePath){
    currentOpenFileType=-1;
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
    clear_oldData();
    clear_Display_Info();
    clear_Table_Info();
    //获取完整的文件名
    //将windows下的反斜杠的路径替换为标准的右斜杠
#ifdef Q_OS_WIN32
    currentOpenFilePath=currentOpenFilePath.replace("\\","/");
#endif
    int first = currentOpenFilePath.lastIndexOf ("/");
    QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
    currentFileName=fileName;
    this->setWindowTitle(appName+"-"+currentFileName);
    /*
        文件判断逻辑，先判断是不是OFD，再判断是不是索引，如果都校验失败，则进入非OF体系的判断
        在明确的确定下，则return完成判断，不明确的情况下，则goto到通用的csv和定长文件判断逻辑去
    */
    ////////////////OFD数据文件的判断////////////////////////////
    if(fileName.startsWith("OFD",Qt::CaseInsensitive)){
        //开始拆解文件名
        QString fixName=fileName;
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
            ui->labelFileTransferDate->setText("文件传递日期");
            ui->lineEditFileTransferDate->setText(dateInfo);
            ui->lineEditFileType->setText(fileTypeCode);
            ui->lineEditSenfInfo->setText(sendName);
            ui->lineEditRecInfo->setText(recName);
            //判断是否是ok文件
            if(currentOpenFilePath.toUpper().endsWith(".OK")){
                statusBar_disPlayMessage("这是一个OFD文件的OK文件,请解析原始数据文件...");
                return;
            }
            //判断是否是恒生系统sqllder导入时生成的bad文件
            if(currentOpenFilePath.toUpper().endsWith(".BAD")){
                statusBar_disPlayMessage("这是一个sqlldr导入生成的bad文件,请解析原始数据文件...");
                return;
            }
            //判断是否是恒生系统sqllder导入时的控制文件
            if(currentOpenFilePath.toUpper().endsWith(".CTL")){
                statusBar_disPlayMessage("是一个sqlldr导入使用的控制文件,请解析原始数据文件...");
                return;
            }
            //判断是否是快捷方式
            if(currentOpenFilePath.toUpper().endsWith(".LNK")){
                statusBar_disPlayMessage("我没猜错的话这是一个文件快捷方式,请解析原始数据文件...");
                return;
            }
            //记录从文件里读取的文件发送信息
            //此处开始加载OFD数据文件
            load_ofdFile(fileTypeCode);
            return;
        }
    }
    /////////////判断索引文件///////////
    if(fileName.length()>10&&fileName.startsWith("OF",Qt::CaseInsensitive))
    {
        //开始解析非OFD文件
        //检索到了文件头标识
        //开始拆解文件名
        QString fixName=fileName;
        if(fixName.contains(".",Qt::CaseInsensitive)){
            fixName=fixName.mid(0,fixName.indexOf("."));
        }
        //索引文件名处理完毕后，开始拆解文件名
        QStringList nameList=fixName.split("_");
        //正常的OFD索引文件应该有4段信息组成
        if(nameList.count()!=4){
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
                //此处开始加载索引文件
                load_indexFile();
                return;
            }
        }
        else{
            qDebug()<<"索引识别异常";
        }
    }
    //非OF体系的文件，既不是OFD数据文件，又不是索引文件，则不放弃，检测下是否是csv或者定长文件
NOT_OF_FILE:
    //开始判断是不是csv类别的文件
    ////////////////////////////////////////////
    QStringList resultCsvType;
    QHash<QString, QString>::iterator h;
    for(h=loadedCsvFileInfo.begin(); h!=loadedCsvFileInfo.end(); ++h){
        QString Name=h.key();
        Name.replace("*","\\S*");
        QString pattern(Name.toUpper());
        QRegExp rx(pattern);
        bool match = rx.exactMatch(fileName.toUpper());
        if(match){
            resultCsvType.append(h.key());
        }
    }
    //定长文件
    ///////////////////////////////////////////
    QStringList resultFixedType;
    QHash<QString, QString>::iterator h2;
    for(h2=loadedFixedFileInfo.begin(); h2!=loadedFixedFileInfo.end(); ++h2){
        QString Name=h2.key();
        Name.replace("*","\\S*");
        QString pattern(Name.toUpper());
        QRegExp rx(pattern);
        bool match = rx.exactMatch(fileName.toUpper());
        if(match){
            resultFixedType.append(h2.key());
        }
    }
    //判断是进入
    if(resultCsvType.count()>0&&resultFixedType.count()==0){
        load_csvFile(resultCsvType);
        return;
    }
    else if(resultCsvType.count()==0&&resultFixedType.count()>0){
        load_fixedFile(resultFixedType);
        return;
    }
    //同时两种类型的文件名字都匹配到了
    else if(resultCsvType.count()>0&&resultFixedType.count()>0){
        DialogChooseFileType * dialog2 = new DialogChooseFileType(this);
        dialog2->setWindowTitle("打开的文件文件名匹配到了两种文件大类型,请确认");
        dialog2->setModal(true);
        dialog2->exec();
        //从弹窗中获取结果
        int type=dialog2->getFileType();
        //模态的使用完毕删除
        delete dialog2;
        dialog2=nullptr;
        if(type==2){
            load_csvFile(resultCsvType);
            return;
        }
        else if(type==3){
            load_fixedFile(resultFixedType);
            return;
        }
        else{
            statusBar_disPlayMessage("放弃读取!");
            return;
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
void MainWindow::load_indexFile(){
    currentOpenFileType=0;
    ui->labelFileTransferDate->setText("文件传递日期");
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QString lineQString;
        int lineNumber=0;
        while (!dataFile.atEnd())
        {
            lineQString = codecOFD->toUnicode(dataFile.readLine());
            lineQString=lineQString.remove('\r').remove('\n');
            //文件体
            if(lineNumber>5){
                indexFileDataList.append(lineQString);
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
            currentOpenFileType=-1;
            statusBar_disPlayMessage("无效的文件,请检查文件是否满足索引文件规范...");
            return;
        }
        //处理最后一行
        QString lastLine=indexFileDataList.last();
        if(lastLine.startsWith("OFDCFEND")){
            ofdIndexFileHeaderMap.insert("fileend",lastLine);
            indexFileDataList.removeLast();
        }
        dataFile.close();
    }
    //读取完毕OFD文件的信息后，进行数据显示
    init_display_IndexTable();
}

/**
 * @brief MainWindow::load_ofdFile OFD文件的加载方法
 * @param fileType
 */
void MainWindow::load_ofdFile(QString fileType){
    qDebug()<<"开始加载的文件类别"<<fileType;
    //当前打开的文件类别为1，OFD文件
    currentOpenFileType=1;
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
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QString lineQString;
        int lineNumber=0;
        while (!dataFile.atEnd())
        {
            //根据OFD文件的编码解开成unicode字符串
            lineQString = codecOFD->toUnicode(dataFile.readLine());
            //第一行数据不trim
            if(lineNumber==lineEnd&&lineNumber!=9){
                //数据行不能trim
                lineQString=lineQString.remove('\r').remove('\n');
            }
            else{
                lineQString=lineQString.remove('\r').remove('\n').trimmed();
            }
            //文件的第二行是文件版本
            if(lineNumber==1){
                versionFromFile=lineQString.trimmed();
                //获取不到版本号，退出
                if(versionFromFile.isEmpty()){
                    currentOpenFileType=-1;
                    dataFile.close();
                    statusBar_disPlayMessage("解析失败,未从文件第2行读取到OFD文件的版本号信息");
                    return;
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
                    currentOpenFileType=-1;
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
                    currentOpenFileType=-1;
                    dataFile.close();
                    statusBar_disPlayMessage("解析失败,文件数据记录数["+lineQString+"]不是有效的数值,请检查文件是否合法");
                    return;
                }
            }
            //读取到了第一行记录，哈哈哈
            if(lineNumber>9&&lineNumber==lineEnd){
                //记录到了文件记录数
                firstLineDateLenght=codecOFD->fromUnicode(lineQString).length();
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
            currentOpenFileType=-1;
            statusBar_disPlayMessage("无效的文件,请检查文件是否满足OFD文件规范...");
            return;
        }
        QString useini="OFD_"+versionFromFile+".ini";
        ui->lineEditUseIni->setText(useini);
        ui->lineEditUseIni->setToolTip(nullptr);
        //判断对应的配置文件是否存在
        //从文件头部收取到了足够的信息后，我们要开始尝试用配置解析文件了
        //根据配置和从文件里采集到的文件字段总数，首行数据的长度综合分析，决定使用哪个配置文件解析文件
        QString path=Utils::getConfigPath()+useini;
        if(Utils::isFileExist(path)){
            //失败原因列表
            QList<OFDFaultCause> faultList;
            //这里开始根据文件类别筛查基本满足要求的配置
            //1.8.0改版后，同一个配置文件，比如OFD_22.ini,允许配置多个XX（比如07）文件的定义，所以需要二次确定要使用哪个配置
            QHash<QString, OFDFileDefinition>::const_iterator i;
            bool findConfigOk=false;
            for (i = loadedOfdDefinitionHash.constBegin(); i != loadedOfdDefinitionHash.constEnd(); ++i) {
                //判断是否以文件类型开头
                QString ofdHashKey=i.key();
                OFDFileDefinition ofdFileDefinition=i.value();
                QString needCom=versionFromFile+"_"+fileType;
                //标记使用的解析器
                QString config="["+QString(ofdHashKey).replace(versionFromFile+"_","")+"]";
                if(ofdHashKey.startsWith(needCom)){
                    //先判断配置文件是否可用，不可用的配置直接忽略
                    if(!ofdFileDefinition.getUseAble()){
                        //首行数据不满足
                        OFDFaultCause fault;
                        fault.setConfig(config);
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
                                //开始判断首行
                                if(firstLineDateLenght ==ofdFileDefinition.getRowLength()){
                                    //找到目标解析配置
                                    ofd=ofdFileDefinition;
                                    findConfigOk=true;
                                    break;
                                }
                                else{
                                    //首行数据不满足
                                    OFDFaultCause fault;
                                    fault.setConfig(config);
                                    fault.setCause("配置文件"+useini+"中的解析器"+config+"所代表的文件的文件每行数据长度是["+QString::number(ofdFileDefinition.getRowLength())+"],但是从文件第1行数据记录获取到的数据长度是["+QString::number(firstLineDateLenght)+"],请检查文件是否满足接口标准,或者配置是否有误");
                                    faultList.append(fault);
                                }
                            }
                            else{
                                //空数据无法校验首行记录，直接认定
                                ofd=ofdFileDefinition;
                                findConfigOk=true;
                                break;
                            }
                        }
                    }
                    else{
                        OFDFaultCause fault;
                        fault.setConfig(config);
                        fault.setCause(QString("打开的文件的字段数是[%1]和配置文件中的[%2]不一致,请检查文件是否满足接口标准,或者配置是否有误").arg(QString::number(countNumberFromFile)).arg(QString::number(ofdFileDefinition.getFieldCount())));
                        faultList.append(fault);
                    }
                }
            }
            if(findConfigOk){
                ui->lineEditFileDescribe->setText(ofd.getDescribe());
                QFile dataFile(currentOpenFilePath);
                //判断如果文件打开成功,则开始读取
                if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
                {
                    int lineNumber=0;
                    //数据行开始的位置,10行文件头,1行行记录标识+字段行数
                    int beginIndex=11+ofd.getFieldCount();
                    QString line;
                    bool sucessFlag=true;
                    bool mergeSucessFlag=false;
                    bool mergeFlag=false;
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    while (!dataFile.atEnd())
                    {
                        //如果此行记录小于数据开始行,则认为是文件头,存入文件头
                        if(lineNumber<beginIndex){
                            line = codecOFD->toUnicode(dataFile.readLine());
                            line=line.remove('\r').remove('\n');
                            ofdFileHeaderQStringList.append(line);
                        }
                        //数据行,进行数据行分析
                        else{
                            //转换为字符串
                            line = codecOFD->toUnicode(dataFile.readLine());
                            //去除换行符
                            line=line.remove('\r').remove('\n');
                            //还原会gb18030编码的字符串
                            QByteArray lineQByteArray=codecOFD->fromUnicode(line);
                            //分析是否读取到了OFDCFEND结束标记,是的话直接跳出循环
                            if(line.length()==8&&QString::compare(line,"OFDCFEND",Qt::CaseInsensitive)==0){
                                break;
                            }
                            //本行记录和接口约束的一致,存入
                            if(codecOFD->fromUnicode(line).length()==ofd.getRowLength()){
                                //数据压缩器
                                ofdFileContentQByteArrayList.append(qCompress(lineQByteArray,dataCompressLevel));
                            }
                            //如果本行数据长度和接口定义不一致,则尝试读取下一行,分析是否进行数据合并
                            else{
                                //读取下一行
                                QString nextLine = codecOFD->toUnicode(dataFile.readLine()).remove('\r').remove('\n');
                                //如果下一行是文件结束标志,则不再进行合并尝试,直接报错
                                if(nextLine.length()==8&&QString::compare(nextLine,"OFDCFEND",Qt::CaseInsensitive)==0){
                                    sucessFlag=false;
                                    currentOpenFileType=-1;
                                    statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                    QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getRowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"]行长度为["+QString::number(lineQByteArray.length())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                    break;
                                }
                                //如果下一行不是结束标志OFDCFEND,则尝试进行数据合并
                                else{
                                    //弹窗确认--只弹窗一次
                                    if(!mergeFlag){
                                        DialogMergeTip * dialog2 = new DialogMergeTip(this);
                                        dialog2->setWindowTitle("数据异常换行修复提示");
                                        dialog2->setModal(true);
                                        dialog2->exec();
                                        //从弹窗中获取结果
                                        mergeFlag=dialog2->getMergeFlag();
                                        //模态的使用完毕删除
                                        delete dialog2;
                                        dialog2=nullptr;
                                    }
                                    //如果确认需要合并
                                    //需要合并分三种情况上一行的长度+下一行的长度等于定义长度（人为插入的换行）
                                    //异常加入的换行符计入了长度,可能为/r /n /r/n三种情况，占用长度为1，2
                                    if (mergeFlag){
                                        if(codecOFD->fromUnicode(line+nextLine).length()==(ofd.getRowLength()-0)){
                                            ofdFileContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(line+nextLine),dataCompressLevel));
                                            mergeSucessFlag=true;
                                            fileChanged=true;
                                        }else if(codecOFD->fromUnicode(line+nextLine).length()==(ofd.getRowLength()-1)){
                                            ofdFileContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(line.append(" ").append(nextLine)),dataCompressLevel));
                                            mergeSucessFlag=true;
                                            fileChanged=true;
                                        }
                                        else if(codecOFD->fromUnicode(line+nextLine).length()==(ofd.getRowLength()-2)){
                                            ofdFileContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(line.append("  ").append(nextLine)),dataCompressLevel));
                                            mergeSucessFlag=true;
                                            fileChanged=true;
                                        }
                                        //合并
                                        else{
                                            sucessFlag=false;
                                            mergeSucessFlag=false;
                                            currentOpenFileType=-1;
                                            statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                            QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getRowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"]行长度为["+QString::number(lineQByteArray.length())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                            break;
                                        }
                                    }
                                    //如果放弃合并,则放弃读取
                                    else{
                                        sucessFlag=false;
                                        currentOpenFileType=-1;
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
                        if((lineNumber%1000)==0){
                            statusBar_disPlayMessage(QString("文件读取中,已读取数据记录%1行").arg(QString::number(lineNumber)));
                            qApp->processEvents();
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
                        statusBar_disPlayMessage("读取到数据行"+QString::number(ofdFileContentQByteArrayList.count())+"行");
                        init_OFDTable();
                    }
                    //如果失败了,则释放内存
                    //此处很重要,是否启用文件另存使用了ofdFileHeaderQStringList是否为空的判断
                    else{
                        ofdFileHeaderQStringList.clear();
                        ofdFileContentQByteArrayList.clear();
                    }
                }else{
                    currentOpenFileType=-1;
                    statusBar_disPlayMessage("解析失败:文件读取失败,请重试...");
                    return;
                }
            }
            else{
                currentOpenFileType=-1;
                //显示失败原因
                if(faultList.count()>0){
                    //调用展示OFD加载失败的函数
                    statusBar_disPlayMessage("配置文件["+useini+"]无法解析该文件,请参阅错误提示检查文件或修正配置...");
                    display_OFDFaultCause(useini,faultList);
                }
                //如果为解析成功，且失败原因为0，则说明配置文件里压根没合适的配置
                else{
                    statusBar_disPlayMessage("配置文件["+useini+"]无法解析该文件,请检查文件或在配置文件中新增["+fileType+"]配置段...");
                }
                return;
            }
        }
        else{
            statusBar_disPlayMessage("配置文件["+useini+"]不存在,无法解析版本号为["+versionFromFile+"]的文件,请新增配置...");
            return;
        }
    }
    else{
        currentOpenFileType=-1;
        statusBar_disPlayMessage("解析失败,文件打开失败...");
        return;
    }
}

/**
 * @brief MainWindow::load_csvFile加载CSV文件的函数
 * @param fileType
 */
void MainWindow::load_csvFile(QStringList fileType){
    currentOpenFileType=2;
    //由于csv文件有可能同一个文件名，但是实际文件版本不同，所以需要做版本校验
    QFile dataFile(currentOpenFilePath);
    //关于CSV文件类型可用的版本
    if(loadedCsvDefinitionList.count()<1){
        statusBar_disPlayMessage("配置目录中，无任何关于CSV文件的配置，请配置后再使用");
        return;
    }
    else{
        //在这里记录下各个配置文件失败无法使用的原因，如果全部匹配失败无法解析，则给用户一个交代！
        QList<FaultCause> faultList;
        int dd=0;
        for(;dd<loadedCsvDefinitionList.count();dd++){
            //如果文件名可用且配置可用，则加入
            if(fileType.contains(loadedCsvDefinitionList.at(dd).getFileName())){
                if(loadedCsvDefinitionList.at(dd).getUseAble()){
                    //打开并开始探测文件内容
                    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
                    {
                        int dataBeginRow=loadedCsvDefinitionList.at(dd).getDatabeginrowindex();
                        //查找可用配置结束，开始分析文件到底是哪个版本
                        //最大化获取文件内的样本数据
                        QTextCodec *codec=QTextCodec::codecForName(loadedCsvDefinitionList.at(dd).getEcoding().toLocal8Bit());
                        QList<QString>csvData;
                        QString line;
                        //当前读取到的行数
                        int row=0;
                        while (!dataFile.atEnd()&&row<dataBeginRow)
                        {
                            line = codec->toUnicode(dataFile.readLine());
                            line=line.remove('\r').remove('\n').trimmed();
                            csvData.append(line);
                            row++;
                        }
                        dataFile.close();
                        //如果一行数据都没读到，则提示是空文件，结束探测--不再做任何解析尝试
                        if(row==0){
                            dataFile.close();
                            statusBar_disPlayMessage("空的CSV文件,没有任何数据记录可工解析");
                            return;
                        }
                        else{
                            //存在标题行
                            if(loadedCsvDefinitionList.at(dd).getTitlerowindex()>0){
                                //存在标题行但是文件内的数据行数低于标题所在行，无内容，无法判断
                                if(csvData.count()<loadedCsvDefinitionList.at(dd).getTitlerowindex()){
                                    FaultCause item;
                                    item.setConfigIndex(dd);
                                    item.setCause("未在本配置描述的文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行找到有效标题行,无法认定文件属于此类型");
                                    faultList.append(item);
                                    continue;
                                }
                                //存在标题行并且数据文件内也有标题行数据，比对标题有多少列，和定义的一致否
                                else{
                                    QString titleRowString=csvData.at(loadedCsvDefinitionList.at(dd).getTitlerowindex()-1);
                                    if(titleRowString.length()<0){
                                        FaultCause item;
                                        item.setConfigIndex(dd);
                                        item.setCause("在本配置描述的文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行找到的标题内容为空，无法提取接口文件列名信息");
                                        faultList.append(item);
                                        continue;
                                    }
                                    //如果需要忽略最后的分隔符
                                    //注意不能只是减去最后一位，也许分隔符长度不是1呢
                                    if(loadedCsvDefinitionList.at(dd).getEndwithflag()=="1"){
                                        titleRowString= titleRowString.left(titleRowString.length()-loadedCsvDefinitionList.at(dd).getSplit().length());
                                    }
                                    QStringList fieldTitle=titleRowString.split(loadedCsvDefinitionList.at(dd).getSplit());
                                    //如果文件字段数是-1.则代表根据标题或者第一行数据自动分析列数和列标题，以标题行或者第一行数据自己造标题数据为准来更新列数
                                    if(loadedCsvDefinitionList.at(dd).getFieldCount()==-1){
                                        int countFromtitle=fieldTitle.count();
                                        if(countFromtitle<0){
                                            FaultCause item;
                                            item.setConfigIndex(dd);
                                            item.setCause("在本配置描述的文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行使用分隔符["+loadedCsvDefinitionList.at(dd).getSplit()+"]找到的标题列数为0，无法解析列数为0的文件");
                                            faultList.append(item);
                                            continue;
                                        }
                                        else{
                                            //开始加载配置文件
                                            csv=loadedCsvDefinitionList.at(dd);
                                            //开始更新文件列数到临时csv配置变量
                                            csv.setFieldCount(fieldTitle.count());
                                            //开始更新列标题到临时csv配置变量
                                            QList <CsvFieldDefinition> fieldList;
                                            //10开始循环加载本文件类型的字段信息///////////////
                                            for(int r=0;r<fieldTitle.count();r++){
                                                CsvFieldDefinition fieldItem;
                                                QString name=fieldTitle.at(r);
                                                if(name.isEmpty()){
                                                    fieldItem.setFieldName("未定义的字段名(第"+QString::number(r+1)+"列)");
                                                }
                                                else{
                                                    fieldItem.setFieldName(name);
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
                                    else if(fieldTitle.count()==loadedCsvDefinitionList.at(dd).getFieldCount()){
                                        //开始加载数据
                                        csv=loadedCsvDefinitionList.at(dd);
                                        load_csvFileData(fieldTitle);
                                        return;
                                    }
                                    else{
                                        FaultCause item;
                                        item.setConfigIndex(dd);
                                        item.setCause("在文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行找到的标题列数["+QString::number(fieldTitle.count())+"]和配置文件中的["+QString::number(loadedCsvDefinitionList.at(dd).getFieldCount())+"]不一致,可能不是本版本的文件,无法解析,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                                        faultList.append(item);
                                        continue;
                                    }
                                }
                            }
                            //如果不存在标题行
                            else{
                                //不存在标题并且文件内的数据行数低于第一行数据，无内容，无法判断
                                if(csvData.count()<loadedCsvDefinitionList.at(dd).getDatabeginrowindex()){
                                    FaultCause item;
                                    item.setConfigIndex(dd);
                                    item.setCause("配置文件中说文件从第"+QString::number(loadedCsvDefinitionList.at(dd).getDatabeginrowindex())+"行就是数据行了，但是打开的文件只有["+QString::number(csvData.count())+"]行哟，无有效数据,无法解析");
                                    faultList.append(item);
                                    continue;
                                }
                                //如果文件字段数是-1.则代表根据标题或者第一行数据自动分析列数和列标题，以标题行或者第一行数据自己造标题数据为准来更新列数
                                else if(loadedCsvDefinitionList.at(dd).getFieldCount()==-1){
                                    QString firstDataRowString=csvData.at(loadedCsvDefinitionList.at(dd).getDatabeginrowindex()-1);
                                    //如果需要忽略最后一个多余的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getEndwithflag()=="1"){
                                        firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                                    }
                                    int fieldCountFirstRow=firstDataRowString.split(loadedCsvDefinitionList.at(dd).getSplit()).count();
                                    if(fieldCountFirstRow<0){
                                        FaultCause item;
                                        item.setConfigIndex(dd);
                                        item.setCause("在本配置描述的文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行使用分隔符["+loadedCsvDefinitionList.at(dd).getSplit()+"]找到的数据列数为0，无法解析列数为0的文件");
                                        faultList.append(item);
                                        continue;
                                    }
                                    else{
                                        //开始加载配置文件
                                        csv=loadedCsvDefinitionList.at(dd);
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
                                            fieldItem.setFieldName(name);
                                            //添加此字段信息到文件定义
                                            fieldList.append(fieldItem);
                                        }
                                        csv.setFieldList(fieldList);
                                        load_csvFileData(fieldTitle);
                                        return;
                                    }
                                }
                                //存在数据,以第一行数据分析
                                else{
                                    QString firstDataRowString=csvData.at(loadedCsvDefinitionList.at(dd).getDatabeginrowindex()-1);
                                    //如果需要忽略最后一个多余的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getEndwithflag()=="1"){
                                        firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                                    }
                                    int fieldCount=firstDataRowString.split(loadedCsvDefinitionList.at(dd).getSplit()).count();
                                    //如果第一行数据的文件字段数和文件内的一致，则就是该版本的文件！
                                    if(fieldCount==loadedCsvDefinitionList.at(dd).getFieldCount()){
                                        //开始加载数据
                                        csv=loadedCsvDefinitionList.at(dd);
                                        //文件内没标题的从配置读取
                                        QStringList fieldTitle;
                                        for(int tc=0;tc<csv.getFieldList().count();tc++){
                                            fieldTitle.append(csv.getFieldList().at(tc).getFieldName());
                                        }
                                        load_csvFileData(fieldTitle);
                                        return;
                                    }
                                    else{
                                        FaultCause item;
                                        item.setConfigIndex(dd);
                                        item.setCause("在文件第"+QString::number(loadedCsvDefinitionList.at(dd).getDatabeginrowindex())+"行找到的数据有["+QString::number(fieldCount)+"]列数据,和配置文件中定义的列数["+QString::number(loadedCsvDefinitionList.at(dd).getFieldCount())+"]不一致，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                                        faultList.append(item);
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                    else{
                        int first = currentOpenFilePath.lastIndexOf ("/");
                        QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
                        statusBar_disPlayMessage("文件:"+fileName+"打开失败,请重试");
                        return;
                    }
                }
                else{
                    FaultCause item;
                    item.setConfigIndex(dd);
                    item.setCause("配置文件不可用");
                    faultList.append(item);
                }
            }
        }
        display_CSVFaultCause(faultList);
        //如果遍历完还没找到合适的配置，则提示
        currentOpenFileType=-1;
        statusBar_disPlayMessage("解析失败,请查看具体解析失败原因后修正配置或者确认是否文件格式错误");
        return;
    }
}

/**
 * @brief MainWindow::load_fixedFile加载定长文件的函数
 * @param fileType
 */
void MainWindow::load_fixedFile(QStringList fileType){
    //当前打开的文件类别3
    currentOpenFileType=3;
    //由于fixed文件有可能同一个文件名，但是实际文件版本不同，所以需要做版本校验
    QFile dataFile(currentOpenFilePath);
    //关于定长文件类型可用的版本
    if(loadedFixedDefinitionList.count()<1){
        statusBar_disPlayMessage("配置目录中，无任何关于定长文件的配置，请配置后再使用");
        return;
    }
    else{
        //在这里记录下各个配置文件失败无法使用的原因，如果全部匹配失败无法解析，则给用户一个交代！
        QList<FaultCause> faultList;
        int dd=0;
        for(;dd<loadedFixedDefinitionList.count();dd++){
            //如果文件名可用且配置可用，则加入
            if(fileType.contains(loadedFixedDefinitionList.at(dd).getFileName())){
                if(loadedFixedDefinitionList.at(dd).getUseAble()){
                    //打开并开始探测文件内容
                    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
                    {
                        int dataBeginRow=loadedFixedDefinitionList.at(dd).getDataRowBeginIndex();
                        //查找可用配置结束，开始分析文件到底是哪个版本
                        //最大化获取文件内的样本数据
                        QTextCodec *codec=QTextCodec::codecForName(loadedFixedDefinitionList.at(dd).getEcoding().toLocal8Bit());
                        QList<QString>fixedData;
                        QString line;
                        //当前读取到的行数
                        int row=0;
                        //读取到第一行数据为止
                        while (!dataFile.atEnd()&&row<dataBeginRow)
                        {
                            line =codec->toUnicode(dataFile.readLine());
                            line=line.remove('\r').remove('\n');
                            fixedData.append(line);
                            row++;
                        }
                        dataFile.close();
                        //如果一行数据都没读到，则提示是空文件，结束探测--不再做任何解析尝试
                        if(row==0){
                            dataFile.close();
                            statusBar_disPlayMessage("空的定长文件,没有任何数据记录可工解析!");
                            return;
                        }
                        else{
                            //文件内的数据行数低于第一行数据，无内容，无法判断
                            if(fixedData.count()<loadedFixedDefinitionList.at(dd).getDataRowBeginIndex()){
                                FaultCause item;
                                item.setConfigIndex(dd);
                                item.setCause("配置文件中说文件从第"+QString::number(loadedFixedDefinitionList.at(dd).getDataRowBeginIndex())+"行就是数据行了，但是打开的文件只有["+QString::number(fixedData.count())+"]行哟，无有效数据,无法解析");
                                faultList.append(item);
                                continue;
                            }
                            QString firstDataRowString=fixedData.at(loadedFixedDefinitionList.at(dd).getDataRowBeginIndex()-1);
                            //如果第一行数据的长度在可接受的长度内，则允许解析
                            //判断是字节定长还是字符定长
                            //字节定长
                            if(loadedFixedDefinitionList.at(dd).getFieldlengthtype()=="0"){
                                int rowLength=codec->fromUnicode(firstDataRowString).length();
                                //如果行长度包含这种长度
                                if(loadedFixedDefinitionList.at(dd).getRowLengthHash().contains(rowLength)){
                                    //开始加载数据
                                    fixed=loadedFixedDefinitionList.at(dd);
                                    load_fixedFileData();
                                    return;
                                }
                                else{
                                    FaultCause item;
                                    item.setConfigIndex(dd);
                                    item.setCause("在文件第"+QString::number(loadedFixedDefinitionList.at(dd).getDataRowBeginIndex())+"行找到的数据行长度为["+QString::number(rowLength)+"]字节,配置文件无法识别，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                                    faultList.append(item);
                                    continue;
                                }
                            }
                            //字符定长
                            else{
                                int rowLength=firstDataRowString.length();
                                //如果行长度包含这种长度
                                if(loadedFixedDefinitionList.at(dd).getRowLengthHash().contains(rowLength)){
                                    //开始加载数据
                                    fixed=loadedFixedDefinitionList.at(dd);
                                    load_fixedFileData();
                                    return;
                                }
                                else{
                                    FaultCause item;
                                    item.setConfigIndex(dd);
                                    item.setCause("在文件第"+QString::number(loadedFixedDefinitionList.at(dd).getDataRowBeginIndex())+"行找到的数据行长度为["+QString::number(rowLength)+"]字符,配置文件无法识别，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
                                    faultList.append(item);
                                    continue;
                                }
                            }
                        }
                    }
                    else{
                        int first = currentOpenFilePath.lastIndexOf ("/");
                        QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
                        statusBar_disPlayMessage("文件:"+fileName+"打开失败,请重试");
                        return;
                    }
                }
                else{
                    FaultCause item;
                    item.setConfigIndex(dd);
                    item.setCause("配置文件不可用");
                    faultList.append(item);
                }
            }
        }
        display_FIXEDFaultCause(faultList);
        //如果遍历完还没找到合适的配置，则提示
        currentOpenFileType=-1;
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
    //文件类型描述
    ui->lineEditFileDescribe->setText(fixed.getFileDescribe());
    //文件类型描述
    ui->lineEditFileType->setText("字段定长");
    //当加载的文件类别是fixed时，传递日期栏目改为文件正则匹配到的配置;
    ui->labelFileTransferDate->setText("解析器配置");
    ui->lineEditFileTransferDate->setText("["+fixed.getFileNameWithVersion()+"]");
    QList<int > rowLengthList;
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextCodec *codec=(QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()));
        QString line2;
        int lineNumber=0;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        while (!dataFile.atEnd())
        {
            line2 = codec->toUnicode(dataFile.readLine()).remove('\r').remove('\n');
            if(lineNumber<fixed.getDataRowBeginIndex()-1){
                fixedFileHeaderQStringList.append(line2);
            }
            else{
                //切记注意，在这里我们使用数据压缩算法对数据进行压缩---获取的时候需解压使用
                fixedContenQByteArrayList.append(qCompress(codec->fromUnicode(line2),dataCompressLevel));
                //记录每行长度
                if(fixed.getFieldlengthtype()=="0"){
                    rowLengthList.append(codec->fromUnicode(line2).length());
                }
                else{
                    rowLengthList.append(line2.length());
                }
            }
            lineNumber++;
            //为了防止UI卡死,进行循环读取文件时,考虑支持下窗口事件接收
            //每读取1000行更新下窗口事件
            if((lineNumber%1000)==0){
                statusBar_disPlayMessage(QString("文件读取中,已读取数据记录%1行").arg(QString::number(lineNumber)));
                qApp->processEvents();
            }
        }
        QApplication::restoreOverrideCursor();
        dataFile.close();
        //从文件读取到内存后，修正文件内容
        //文件尾部提取
        if(fixed.getEndIgnoreRow()>0&&fixedContenQByteArrayList.count()>fixed.getEndIgnoreRow()){
            for(int ic=1;ic<=fixed.getEndIgnoreRow();ic++){
                fixedFooterQStringList.append(codec->fromUnicode(qUncompress(fixedContenQByteArrayList.last())));
                fixedContenQByteArrayList.removeLast();
                rowLengthList.removeLast();
            }
        }
        //校验文件体的每一行的长度
        int count=rowLengthList.count();
        for(int row=0;row<count;row++){
            //字节判断法和字符判断法
            if(fixed.getFieldlengthtype()=="0"){
                //存在无法识别的行
                if(!fixed.getRowLengthHash().contains(rowLengthList.at(row))){
                    QString lengths="";
                    QList <int> length=fixed.getRowLengthHash().keys();
                    lengths.append(QString::number(length.at(0))).append("(").append(QString::number(fixed.getRowLengthHash().value(length.at(0)))).append("个字段)");
                    if(length.count()>1){
                        for(int ff=1;ff<length.count();ff++){
                            lengths.append("/").append(QString::number(length.at(ff))).append("(").append(QString::number(fixed.getRowLengthHash().value(length.at(ff)))).append("个字段)");
                        }
                    }
                    QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n["+fixed.getFileNameWithVersion()+"]中定义的记录长度和打开的文件中不一致,解析失败\r\n["+fixed.getFileNameWithVersion()+"]中文件定义的数据行长度可以为["+lengths+"]字节\r\n实际打开的文件中第["+QString::number(row+1+fixedFileHeaderQStringList.count())+"]行长度为["+QString::number(rowLengthList.at(row))+"]\r\n请检查是否是文件错误,或者配置定义错误,或者你打开的是别的版本的文件(如果是请先配置!)",QMessageBox::Ok,QMessageBox::Ok);
                    statusBar_disPlayMessage("文件解析失败!");
                    currentOpenFileType=-1;
                    fixedFileHeaderQStringList.clear();
                    fixedContenQByteArrayList.clear();
                    fixedFooterQStringList.clear();
                    return;
                }
            }
            else{
                //存在无法识别的行
                if(!fixed.getRowLengthHash().contains(rowLengthList.at(row))){
                    QString lengths="";
                    QList <int> length=fixed.getRowLengthHash().keys();
                    lengths.append(QString::number(length.at(0))).append("(").append(QString::number(fixed.getRowLengthHash().value(length.at(0)))).append("个字段)");
                    if(length.count()>1){
                        for(int ff=1;ff<length.count();ff++){
                            lengths.append("/").append(QString::number(length.at(ff))).append("(").append(QString::number(fixed.getRowLengthHash().value(length.at(ff)))).append("个字段)");
                        }
                    }
                    QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n["+fixed.getFileNameWithVersion()+"]中定义的记录长度和打开的文件中不一致,解析失败\r\n["+fixed.getFileNameWithVersion()+"]中文件定义的数据行长度可以为["+lengths+"]字符\r\n实际打开的文件中第["+QString::number(row+1+fixedFileHeaderQStringList.count())+"]行长度为["+QString::number(rowLengthList.at(row))+"]\r\n请检查是否是文件错误,或者配置定义错误,或者你打开的是别的版本的文件(如果是请先配置!)",QMessageBox::Ok,QMessageBox::Ok);
                    statusBar_disPlayMessage("文件解析失败!");
                    currentOpenFileType=-1;
                    fixedFileHeaderQStringList.clear();
                    fixedContenQByteArrayList.clear();
                    fixedFooterQStringList.clear();
                    return;
                }
            }
        }
        //如果文件没问题，开始初始化表格
        init_FIXEDTable();
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
        ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            CsvFileDefinition csvfileDefinition=loadedCsvDefinitionList.at(faultList.at(row).getConfigIndex());
            //配置文件
            if(!csvfileDefinition.getFileIni().isEmpty()){
                QTableWidgetItem *item0= new QTableWidgetItem(csvfileDefinition.getFileIni());
                ptr_table->setItem(row, 0, item0);
            }
            //解析器
            if(!csvfileDefinition.getFileNameWithCount().isEmpty()){
                QTableWidgetItem *item1= new QTableWidgetItem("["+csvfileDefinition.getFileNameWithCount()+"]");
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
        ptr_table->resizeColumnsToContents();
        statusBar_display_rowsCount(rowCount);
        currentOpenFileType=-1;
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
        ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            FIXEDFileDefinition csvfileDefinition=loadedFixedDefinitionList.at(faultList.at(row).getConfigIndex());
            //配置文件
            if(!csvfileDefinition.getFileIni().isEmpty()){
                QTableWidgetItem *item0= new QTableWidgetItem(csvfileDefinition.getFileIni());
                ptr_table->setItem(row, 0, item0);
            }
            //解析器
            if(!csvfileDefinition.getFileNameWithVersion().isEmpty()){
                QTableWidgetItem *item1= new QTableWidgetItem("["+csvfileDefinition.getFileNameWithVersion()+"]");
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
                //数据起始行
                QTableWidgetItem *item8= new QTableWidgetItem(QString::number(csvfileDefinition.getDataRowBeginIndex()));
                ptr_table->setItem(row, 5, item8);
                //文件编码
                if(!csvfileDefinition.getEcoding().isEmpty()){
                    QTableWidgetItem *item9= new QTableWidgetItem(csvfileDefinition.getEcoding());
                    ptr_table->setItem(row, 6, item9);
                }
                //字段长度类型
                if(csvfileDefinition.getFieldlengthtype()=="0"){
                    QTableWidgetItem *item10= new QTableWidgetItem("字节定长");
                    ptr_table->setItem(row, 7, item10);
                }
                else{
                    QTableWidgetItem *item10= new QTableWidgetItem("字符定长");
                    ptr_table->setItem(row, 7, item10);
                }
                //每行允许的长度
                QString lengths="";
                QList <int> length=csvfileDefinition.getRowLengthHash().keys();
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
        ptr_table->resizeColumnsToContents();
        statusBar_display_rowsCount(rowCount);
        currentOpenFileType=-1;
    }
}

/**
 * @brief MainWindow::display_OFDFaultCause OFD文件解析失败原因
 * @param useini
 * @param faultList
 */
void MainWindow::display_OFDFaultCause(QString useini,QList<OFDFaultCause> faultList){
    if(faultList.count()>0){
        int colCount=3;
        int rowCount=faultList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        title.append("使用的配置文件");
        title.append("文件匹配解析器");
        title.append("解析失败原因");
        ptr_table->setHorizontalHeaderLabels(title);
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            //配置文件
            QTableWidgetItem *item0= new QTableWidgetItem(useini);
            ptr_table->setItem(row, 0, item0);
            //解析器
            QTableWidgetItem *item1= new QTableWidgetItem(faultList.at(row).getConfig());
            ptr_table->setItem(row, 1, item1);
            //失败原因
            QTableWidgetItem *item2= new QTableWidgetItem(faultList.at(row).getCause());
            ptr_table->setItem(row, 2, item2);
        }
        ptr_table->resizeColumnsToContents();
        statusBar_display_rowsCount(rowCount);
        currentOpenFileType=-1;
    }
}

/**
 * @brief MainWindow::load_csvFileData当时识别出来csv文件类别后，加载csv文件数据的方法
 * @param fieldTitle
 */
void MainWindow::load_csvFileData(QStringList fieldTitle){
    //使用的ini配置
    ui->lineEditUseIni->setText(csv.getFileIni());
    //文件类型描述
    ui->lineEditFileDescribe->setText(csv.getFileDescribe());
    //文件类型描述
    ui->lineEditFileType->setText("CSV文件");
    //当加载的文件类别是csv时，传递日期栏目改为文件正则匹配到的配置;
    ui->labelFileTransferDate->setText("解析器配置");
    ui->lineEditFileTransferDate->setText("["+csv.getFileNameWithCount()+"]");
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextCodec *codec=QTextCodec::codecForName(csv.getEcoding().toLocal8Bit());
        QString line2;
        int lineNumber=0;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        while (!dataFile.atEnd())
        {
            line2 = codec->toUnicode(dataFile.readLine()).remove('\r').remove('\n');
            if(lineNumber<csv.getDatabeginrowindex()-1){
                csvFileHeaderQStringList.append(line2);
            }
            else{
                csvFileContentQByteArrayList.append(qCompress(codec->fromUnicode(line2),dataCompressLevel));
            }
            lineNumber++;
            //为了防止UI卡死,进行循环读取文件时,考虑支持下窗口事件接收
            //每读取1000行更新下窗口事件
            if((lineNumber%1000)==0){
                statusBar_disPlayMessage(QString("文件读取中,已读取数据记录%1行").arg(QString::number(lineNumber)));
                qApp->processEvents();
            }
        }
        QApplication::restoreOverrideCursor();
        dataFile.close();
        //从文件读取到内存后，修正文件内容
        //文件尾部提取
        if(csv.getEndIgnoreRow()>0&&csvFileContentQByteArrayList.count()>csv.getEndIgnoreRow()){
            for(int ic=1;ic<=csv.getEndIgnoreRow();ic++){
                csvFileContentQByteArrayList.removeLast();
            }
        }
    }
    //初始化表格
    init_CSVTable(fieldTitle);
}

/**
 * @brief MainWindow::init_display_IndexTable OFD索引数据的表格显示
 *
 */
void MainWindow::init_display_IndexTable(){
    if(!indexFileDataList.empty()){
        int colCount=1;
        int rowCount=indexFileDataList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        title.append("索引包含的文件信息");
        ptr_table->setHorizontalHeaderLabels(title);
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
        //设置编辑方式
        ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
        //设置表格的内容
        for (int row = 0; row < rowCount; ++row)
        {
            QTableWidgetItem *item= new QTableWidgetItem(indexFileDataList.at(row));
            ptr_table->setItem(row, 0, item);
        }
        ptr_table->resizeColumnsToContents();
        statusBar_display_rowsCount(rowCount);
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
        int rowCount=ofdFileContentQByteArrayList.count();
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
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
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
        statusBar_disPlayMessage("文件解析完毕!成功读取记录"+QString::number(rowCount)+"行");
    }
    else
    {
        statusBar_disPlayMessage(ofd.getMessage());
    }
}

/**
 * @brief MainWindow::init_FIXEDTable初始化定长文件函数，目前在分页支持下，默认只初始化第一页数据
 */
void MainWindow::init_FIXEDTable(){
    int colCount=fixed.getFieldCountMax();
    int rowCount=fixedContenQByteArrayList.count();
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
    //设置表格的选择方式
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
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
    statusBar_disPlayMessage("文件解析完毕!成功读取记录"+QString::number(rowCount)+"行");
}

/**
 * @brief MainWindow::init_CSVTable初始化csv文件函数，目前在分页支持下，默认只初始化第一页数据
 * @param title
 */
void MainWindow::init_CSVTable(QStringList title){
    if(csv.getUseAble()){
        ///////////////////////CSV数值类型判断///////////////////////////////////////////
        //判断哪些列是否是数值,判断10行数据或者低于10行判读所有行，全部是数值的才认为是数值、但是允许忽略空值
        //仅仅判断包含小数点的小数，整数暂时不管，大部分整数列都是字典值，不是数值，没必要
        //循环识别每列的数据类型
        QHash<int,bool> lastColIsEmpty;
        for(int rowIndex=0;rowIndex<10&&rowIndex<csvFileContentQByteArrayList.count();rowIndex++){
            //获取本行数据
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,rowIndex,csv.getEcoding());
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
                    if(convertOk){
                        //识别识别带小数点的小数
                        if(vav.contains(".",Qt::CaseSensitive)){
                            isnumber.setIsNumber(true);
                            isnumber.setDecimalLength(vav.split(".")[1].length());
                        }
                        //识别只有整数字符串暂不考虑
                        else{
                            isnumber.setIsNumber(false);
                        }
                        fieldIsNumberOrNot.insert(colIndex,isnumber);
                    }
                    else{
                        isnumber.setIsNumber(false);
                        fieldIsNumberOrNot.insert(colIndex,isnumber);
                        //如果本行某列为空则下一次判断本列是否为数值时，依然参与判断
                        if(vav.isEmpty()){
                            lastColIsEmpty.insert(colIndex,true);
                        }
                    }
                }
            }
            else{
                for(int colIndex=0;colIndex<title.count();colIndex++){
                    FieldIsNumber isnumber;
                    //上一行是数字或者上一行为空时进行判断
                    if(fieldIsNumberOrNot.value(colIndex).getIsNumber()||(lastColIsEmpty.contains(colIndex))){
                        bool convertOk=false;
                        QString vav="";
                        //注意不能去掉判断，csv文件如果列残缺会取不到所有列
                        if(colIndex<rowdata.count()){
                            vav=rowdata.at(colIndex);
                        }
                        vav.toDouble(&convertOk);
                        if(convertOk){
                            //识别小数长度
                            int len=0;
                            //不考虑整数
                            if(vav.contains(".",Qt::CaseSensitive)){
                                len=vav.split(".")[1].length();
                                //如果之前因为空行导致的不是小数
                                if(lastColIsEmpty.contains(colIndex)){
                                    isnumber.setIsNumber(true);
                                    isnumber.setDecimalLength(len);
                                    fieldIsNumberOrNot.insert(colIndex,isnumber);
                                }
                                //或者本行小数长度位数比上一行长
                                else if(len>fieldIsNumberOrNot.value(colIndex).getDecimalLength()){
                                    isnumber.setIsNumber(true);
                                    isnumber.setDecimalLength(len);
                                    fieldIsNumberOrNot.insert(colIndex,isnumber);
                                }
                            }
                        }
                        else{
                            //如果本行为空，但是之前的行判断是小数了，则这行不再改成非小数
                            if(vav.isEmpty()&&fieldIsNumberOrNot.contains(colIndex)&&fieldIsNumberOrNot.value(colIndex).getIsNumber()){
                                //如果之前行不空这一行空，则添加给下一次判断使用
                                if((!lastColIsEmpty.contains(colIndex))&&(vav.isEmpty())){
                                    lastColIsEmpty.insert(colIndex,true);
                                }
                                continue;
                            }
                            //如果这一行不空且不是小数，且之前判断过空，要剔空判断，既:之前有空行但是有过任何非小数非空的都不再进行小数判断
                            else if(fieldIsNumberOrNot.contains(colIndex)){
                                lastColIsEmpty.remove(colIndex);
                            }
                            isnumber.setIsNumber(false);
                            fieldIsNumberOrNot.insert(colIndex,isnumber);
                        }
                    }
                }
            }
        }
        int colCount=title.count();
        int rowCount=csvFileContentQByteArrayList.count();
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
        //设置标题
        ptr_table->setHorizontalHeaderLabels(title);
        for(int i=0;i<title.count();i++){
            //初始化列宽度设置为0，保障加载数据时，至少进行一次列宽度重设，解决一些坑坑的问题，请注意不要修改此处代码
            columnWidth.insert(i,0);
        }
        //设置表格的选择方式
        ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
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
        statusBar_display_rowsCount(rowCount);
        statusBar_disPlayMessage("文件解析完毕!成功读取记录"+QString::number(rowCount)+"行");
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

void MainWindow::display_OFDTable(){
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
            QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowInFileContent);
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
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
    }
}
/**
 * @brief MainWindow::display_FIXEDTable 显示定长文件内容到表格的方法，注意懒加载机制，依赖于窗口变化，滚动条滚动进行增量加载
 */
void MainWindow::display_FIXEDTable(){
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
            QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,rowInFileContent,fixed.getEcoding());
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
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
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
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,rowInFileContent,csv.getEcoding());
            //csv文件数据的某一行可能是不完整的，做忽略容忍！，允许正常解析
            //后续将不满足的数据记录在案
            for(int col=0;col<colCount&&col<rowdata.count();col++){
                QString values=rowdata.at(col);
                //仅对数据非空单元格赋值
                if(!values.isEmpty()){
                    bool isDouble=false;
                    if(fieldIsNumberOrNot.contains(col)&&fieldIsNumberOrNot.value(col).getIsNumber()){
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
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
    }
}


/**
 * @brief MainWindow::clear_Table_Info 清空表格内容--包含行列信息
 */
void MainWindow::clear_Table_Info(){
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    ptr_table->setColumnCount(0);
}

/**
 * @brief MainWindow::clear_Table_Contents清空表格信息-仅内容
 */
void MainWindow::clear_Table_Contents(){
    ptr_table->clearContents();
}

/**
 * @brief MainWindow::statusBar_disPlayMessage 状态栏显示消息的函数
 * @param text
 */
void MainWindow::statusBar_disPlayMessage(QString text){
    statusLabel_ptr_showMessage->setText(text);
}

/**
 * @brief MainWindow::clear_Display_Info清理文件信息栏的函数
 */
void MainWindow::clear_Display_Info(){
    ui->lineEditSendCode->setText(nullptr);
    ui->lineEditRecCode->setText(nullptr);
    ui->lineEditFileTransferDate->setText(nullptr);
    ui->lineEditFileType->setText(nullptr);
    ui->lineEditSenfInfo->setText(nullptr);
    ui->lineEditRecInfo->setText(nullptr);
    ui->lineEditFileDescribe->setText(nullptr);
    ui->lineEditUseIni->setText(nullptr);
    ui->lineEditUseIni->setToolTip(nullptr);
    statusBar_clear_statusBar();
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
    dialog->show();
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
    if(currentOpenFileType==0){
        if(!ofdIndexFileHeaderMap.isEmpty()){
            //组织要显示的内容
            QString info;
            info.append("文件解析情况如下:\r\n");
            info.append("文件发送者代码:").append(ofdIndexFileHeaderMap.value("sendcode")).append("\r\n");
            info.append("文件接收者代码:").append(ofdIndexFileHeaderMap.value("recivecode")).append("\r\n");
            info.append("文件传递日期:").append(ofdIndexFileHeaderMap.value("filedate")).append("\r\n");
            info.append("文件内标识的总记录数:").append(ofdIndexFileHeaderMap.value("count")).append("成功加载记录数:").append(QString::number(indexFileDataList.count())).append("\r\n");
            QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    else if(currentOpenFileType==1){
        if(!ofdFileHeaderQStringList.isEmpty()||ofdFileHeaderQStringList.count()>=10){
            //组织要显示的内容
            QString info;
            info.append("文件解析情况如下:\r\n");
            info.append("文件发送者代码:").append(ofdFileHeaderQStringList.at(2)).append("\r\n");
            info.append("文件接收者代码:").append(ofdFileHeaderQStringList.at(3)).append("\r\n");
            info.append("文件传递日期:").append(ofdFileHeaderQStringList.at(4)).append("\r\n");
            bool ok=false;
            int  fieldCount=ofdFileHeaderQStringList.at(9).toInt(&ok);
            if(ok){
                int countRow=10+fieldCount;
                if(countRow>ofdFileHeaderQStringList.count()-1){
                    info.append("从原文件读取记录数标志失败,").append("成功加载记录数:").append(QString::number(ofdFileContentQByteArrayList.count())).append("\r\n");
                }
                else{
                    info.append("文件内标识的总记录数:").append(ofdFileHeaderQStringList.at(countRow)).append("成功加载记录数:").append(QString::number(ofdFileContentQByteArrayList.count())).append("\r\n");
                }
            }else{
                info.append("读取文件字段数错误");
            }
            QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    else if(currentOpenFileType==2){
        if(csvFileContentQByteArrayList.count()>0){
            //组织要显示的内容
            QString info;
            info.append("文件解析情况如下:\r\n");
            info.append("使用的配置文件:").append(csv.getFileIni()).append("\r\n");
            info.append("使用的解析器配置:[").append(csv.getFileNameWithCount()).append("]\r\n");
            info.append("加载的文件头行数").append(QString::number(csvFileHeaderQStringList.count())).append("\r\n");
            info.append("加载的数据行数(含文件结尾行)").append(QString::number(csvFileContentQByteArrayList.count())).append("\r\n");
            QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    else if(currentOpenFileType==3){
        //组织要显示的内容
        QString info;
        info.append("文件解析情况如下:\r\n");
        info.append("使用的配置文件:").append(fixed.getFileIni()).append("\r\n");
        info.append("使用的解析器配置:[").append(fixed.getFileNameWithVersion()).append("]\r\n");
        info.append("加载的文件头行数").append(QString::number(fixedFileHeaderQStringList.count())).append("\r\n");
        info.append("加载的数据行数").append(QString::number(fixedContenQByteArrayList.count())).append("\r\n");
        info.append("加载的文件尾部行数").append(QString::number(fixedFooterQStringList.count())).append("\r\n");
        QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
    }
    //数据类型插入点
}

/**
 * @brief MainWindow::tableWidget_currentCellChanged 鼠标点击的表格当前单元格变化后触发的函数
 * @param currentRow
 * @param currentColumn
 * @param previousRow
 * @param previousColumn
 */
void MainWindow::tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    UNUSED(previousRow);
    UNUSED(previousColumn);
    ptr_table->setCurrentCell(currentRow,currentColumn);
}

/**
 * @brief MainWindow::copyToClipboard
 * 数据复制
 */
void MainWindow::copyToClipboard(){
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
            statusBar_disPlayMessage(QString("已复制数据:\"%1\"").arg(text));
        }
        //多个单元格复制-取原始数据
        else{
            int selectSum=(rigthColumn-leftColumn+1)*(bottomRow-topRow+1);
            if(selectSum>1000000){
                statusBar_disPlayMessage("警告:选择的单元格超过1,000,000,无法复制!");
            }
            else{
                QString value="";
                int rowRealInContent=0;
                //在错误展示和OFD索引文件模式下复制取表格数据
                if(currentOpenFileType==-1||currentOpenFileType==0){
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
                else if(currentOpenFileType==1){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            //数据不能取自表格，表格数据基于懒加载机制的情况下数据会不全
                            if(ofd.getFieldList().at(col).getFieldType()=="N"){
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,col)));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,col));
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
                else if(currentOpenFileType==2){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,rowRealInContent,csv.getEcoding());
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            //数据不能取自表格，表格数据基于懒加载机制的情况下数据会不全
                            //预防csv行不足的情况
                            if(col<rowdata.count()){
                                if(fieldIsNumberOrNot.contains(col)&&fieldIsNumberOrNot.value(col).getIsNumber()){
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
                else if(currentOpenFileType==3){
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        for(int col=leftColumn;col<=rigthColumn;col++){
                            //数据不能取自表格，表格数据基于懒加载机制的情况下数据会不全
                            if(fixed.getFieldList().at(col).getFieldType()=="N"){
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,rowRealInContent,col,fixed.getEcoding())));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,rowRealInContent,col,fixed.getEcoding()));
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
                statusBar_disPlayMessage("复制"+QString::number(selectSum)+"个单元格数据完毕～");
            }
        }
    }
    else if(rangeCount>1){
        statusBar_disPlayMessage("警告:无法对多重选择区域执行复制!");
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
                QStringList rowdata;
                for(int i=0;i<(ptr_table->columnCount());i++){
                    QString col="";
                    if(ptr_table->item(topRow,i)!=nullptr){
                        col=ptr_table->item(topRow,i)->text();
                    }
                    rowdata.append(col);
                }
                compareData.insert(rowRealInContent+1,rowdata);
                statusBar_disPlayMessage(QString("比对器内已加入%1行数据").arg(compareData.count()));
            }
        }
        else{
            //移除数据行
            compareData.remove(rowRealInContent+1);
            if(compareData.count()>0){
                statusBar_disPlayMessage(QString("比对器内还有%1行数据").arg(compareData.count()));
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
            if(currentOpenFileType==1){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent));
                        }
                    }
                }
            }
            //CSV
            else if(currentOpenFileType==2){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,rowRealInContent,csv.getEcoding()));
                        }
                    }
                }
            }
            //FIXED
            else if(currentOpenFileType==3){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,rowRealInContent,fixed.getEcoding()));
                        }
                    }
                }
            }
            statusBar_disPlayMessage(QString("比对器内已加入%1行数据").arg(compareData.count()));
        }
    }
}

/**
 * @brief MainWindow::deleteRowDataFromFileAndTable 删除选择的行，目前针对OFD文件支持
 *
 */
//待修改
void MainWindow::deleteRowDataFromFileAndTable(){
    //正在进行的阻断操作时，禁止删除数据
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    //此函数目前仅支持OFD数据
    if(currentOpenFileType==1){
        DialogMyTip * dialog2 = new DialogMyTip("真的要删除选择的数据行吗？",this);
        dialog2->setWindowTitle("警告！");
        dialog2->setModal(true);
        dialog2->exec();
        //从弹窗中获取结果//如果放弃删除数据
        if(!dialog2->getBoolFlag()){
            //模态的使用完毕删除
            delete dialog2;
            dialog2=nullptr;
            statusBar_disPlayMessage("放弃删除...");
            return;
        }
        else{
            delete dialog2;
            dialog2=nullptr;
        }
        dataBlocked=true;
        dataBlockedMessage="正在删除数据,请稍候...";
        //获取选择的范围
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        //先整理明确需要删除的行
        QList<int> deleteRowList;
        for(int i=0;i<itemsRange.count();i++){
            int top=itemsRange.at(i).topRow();
            int bottom=itemsRange.at(i).bottomRow();
            for(int j=top;j<=bottom;j++){
                //有可能两个选择范围是在一行的，不要重复加入行
                if(!deleteRowList.contains(j)){
                    deleteRowList.append(j);
                }
            }
        }
        //如果多选择器的选择顺序不是从上往下，存在可能deleteRowList里的顺序不是完全顺序的，这里进行重新排序
        //排序后从后一个开始删除，确保删除正确
        std::sort(deleteRowList.begin(), deleteRowList.end());
        int count=deleteRowList.count();
        //判断是不是要删除所有行--删除所有行要使用单独的方法
        //如果删除所有行-既全选能删除所有行，表明目前的文件未分页
        if(count==ofdFileContentQByteArrayList.count()){
            //清除所有内容
            ptr_table->clearContents();
            //设置表格行数为0
            ptr_table->setRowCount(0);
            //清空加载的原始数据
            ofdFileContentQByteArrayList.clear();
            //清空行加载记录
            rowHasloaded.clear();
            //更新OFD记录数标记--在文件头的最后一行
            ofdFileHeaderQStringList.replace(ofdFileHeaderQStringList.count()-1,QString("%1").arg(ofdFileContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
            //清空比对器内容
            compareData.clear();
            //更新提示-更新主窗口标题
            statusBar_disPlayMessage(QString("选择的%1行数据已删除,请记得保存文件哟...").arg(count));
            this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
            //更新文件修改标记
            fileChanged=true;
            //大功告成-处理下分页的问题
            currentPage=1;
            pageCount=1;
            ui->framePage->setVisible(false);
        }
        //非全部删除的情况下，逐行删除，注意需倒序删除，至于原因，请思考
        //需要考虑分页了的可能性
        else{
            if(count>0){
                //阻断标记，避免删除大量数据时误操作
                dataBlocked=true;
                dataBlockedMessage="正在删除数据中,请稍候...";
                int row=0;
                int rowRealInContent=0;
                //清空比对器内容
                compareData.clear();
                for(int w=count-1;w>=0;w--){
                    //需要被删除的行
                    row=deleteRowList.at(w);
                    //从表格移除该行
                    ptr_table->removeRow(row);
                    //从原始记录移除该行
                    //做行号转换
                    rowRealInContent=(currentPage-1)*pageRowSize+row;
                    ofdFileContentQByteArrayList.removeAt(rowRealInContent);
                }
                //删除所有行加载记录，因为删除某行后所有比删除的行大的行的加载记录都错位了,还是老实再加载一下吧，当然也可以重新校准哪些行已经加载了，但是会很麻烦
                rowHasloaded.clear();
                //更新OFD记录数标记--在文件头的最后一行
                ofdFileHeaderQStringList.replace(ofdFileHeaderQStringList.count()-1,QString("%1").arg(ofdFileContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
                //重新统计目前文件行数
                int rowCount=ofdFileContentQByteArrayList.count();
                //重新统计页数
                int tmpPageCount=(rowCount + pageRowSize - 1) / pageRowSize;
                //根据是否页数变化了来进行分开处理
                if(tmpPageCount==pageCount){
                    //总页数没变，则代表只是删除了本页部分，且删除后最后一页的数据量还存在部分条目，本页还在，直接重新加载显示下即可
                    //更新表格视图
                    display_OFDTable();
                }
                //总页数发生了变化，删除了某一页整页数据，或者删除了某页部分数据后最后一页不在了，需要处理跳页问题
                else{
                    //判断是否剩余页数大于1
                    if(tmpPageCount<2){
                        //重设页数
                        pageCount=1;
                        currentPage=1;
                        ui->framePage->setVisible(false);
                        pageJump(1,0);
                    }
                    //页数大于1并且删除了某一整页，再判断是不是当前在最后一页且删除的最后一页
                    else{
                        //最后一页被删除，则跳到新的最后页的尾部
                        if(currentPage==pageCount){
                            pageCount=tmpPageCount;
                            currentPage=pageCount;
                            ui->framePage->setVisible(true);
                            pageJump(pageCount,-1);
                        }
                        //删除了数据后页数发生了变化，但当前不是最后一页，则下一页数据会重新显示到本页，跳到页首
                        else{
                            pageCount=tmpPageCount;
                            ui->framePage->setVisible(true);
                            pageJump(currentPage,0);
                        }
                    }
                }
                //更新提示
                statusBar_disPlayMessage(QString("选择的%1行数据已删除,请记得保存文件哟...").arg(count));
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                //更新文件修改标记//////////////////////////////////////////////////////////
                fileChanged=true;
            }
        }
        dataBlocked=false;
        //更新总记录数信息
        statusBar_display_rowsCount(ofdFileContentQByteArrayList.count());
    }
}

/**
 * @brief MainWindow::copyOFDRowData 根据选取范围复制OFD数据
 */
void MainWindow::copyOFDRowData(){
    if(currentOpenFileType==1){
        QString version=ui->lineEditUseIni->text()+"|Author:"+QString(QByteArray::fromBase64(AUTHOR_EMAIL));
        QString type=ui->lineEditFileType->text();
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
            DialogMyTip * dialog2 = new DialogMyTip("即将复制大量数据到剪切板,数据有"+QString::number(count)+"行,可能会比较慢或者占用大量剪切板空间，确定继续吗？",this);
            dialog2->setWindowTitle("警告！");
            dialog2->setModal(true);
            dialog2->exec();
            if(!dialog2->getBoolFlag()){
                //模态的使用完毕删除
                delete dialog2;
                dialog2=nullptr;
                statusBar_disPlayMessage("放弃复制...");
                return;
            }
            else{
                delete dialog2;
                dialog2=nullptr;
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
            needCopyData.append(codecOFD->toUnicode(qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent))));
            if(j<count-1){
                needCopyData.append("\r\n");
            }
        }
        //数据准备完毕开始复制
        QClipboard *board = QApplication::clipboard();
        board->setText(needCopyData);
        dataBlocked=false;
        statusBar_disPlayMessage(QString("已复制%1行文件原始记录数据到剪切板").arg(count));
    }
}

/**
 * @brief MainWindow::addOFDRowDataPreviousRow 将剪切板上的数据插入到当前行的上一行
 */
void MainWindow::addOFDRowDataPreviousRow(){
    addOFDRowData(-1);
}

/**
 * @brief MainWindow::addOFDRowDataNextRow 将剪切板上的数据插入到当前行的下一行
 */
void MainWindow::addOFDRowDataNextRow(){
    addOFDRowData(1);
}

/**
 * @brief MainWindow::addOFDRowDataEndRow 插入到文件尾
 */
void MainWindow::addOFDRowDataEndRow(){
    addOFDRowData(0);
}

/**
 * @brief MainWindow::addOFDNewLineRowDataEndRow 插入到尾部一个空数据行
 */
void MainWindow::addOFDNewLineRowDataEndRow(){
    addOFDRowData(2);
}

/**
 * @brief MainWindow::addOFDRowData 插入数据到现有的OFD文件，-1上方，1下方，0文件末尾，2插入一行空数据记录到文件尾
 * @param location
 */
void MainWindow::addOFDRowData(int location){
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
            insertIndexInContent=ofdFileContentQByteArrayList.count();
        }
    }
    //////////////////////////////////////////////////////////
    //插入空数据行
    if(location==2){
        DialogMyTip * dialog2 = new DialogMyTip("真的要为文件新增一行数据么？如果确定，请新增后编辑数据内容！",this);
        dialog2->setWindowTitle("警告！");
        dialog2->setModal(true);
        dialog2->exec();
        if(!dialog2->getBoolFlag()){
            //模态的使用完毕删除
            delete dialog2;
            dialog2=nullptr;
            statusBar_disPlayMessage("放弃新增行...");
            return;
        }
        else{
            delete dialog2;
            dialog2=nullptr;
        }
        QByteArray newLine;
        //使用更为安全的方式新增空记录
        int Length=ofd.getRowLength();
        for(int charIndex=0;charIndex<Length;charIndex++){
            newLine.append(QChar(' '));
        }
        ofdFileContentQByteArrayList.insert(insertIndexInContent,qCompress(newLine,dataCompressLevel));
        //更新OFD记录数标记--在文件头的最后一行
        ofdFileHeaderQStringList.replace(ofdFileHeaderQStringList.count()-1,QString("%1").arg(ofdFileContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
        //新的总行数
        int rowCount=ofdFileContentQByteArrayList.count();
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
    }
    //插入剪切板数据
    else{
        //开始获取剪切板内容
        QClipboard *board = QApplication::clipboard();
        //对换行符切割
        //不同操作系统系统复制过来的数据，换行符号不同，我们这里做统一替换
        //一个坑爹的问题
        QStringList data=board->text().replace("\r\n","\n").replace("\r","\n").split("\n");
        //开始进行数据比对
        if(data.count()<3||!data.at(0).startsWith("OFD")){
            statusBar_disPlayMessage("剪切板上无有效的OFD专用数据，无法插入!");
            return;
        }
        else {
            QString version=data.at(0);
            QString fileType=data.at(1);
            if(fileType!=ui->lineEditFileType->text()){
                statusBar_disPlayMessage(QString("不能将[%1]文件的数据插入到[%2]文件,不匹配的文件类型!").arg(fileType).arg(ui->lineEditFileType->text()));
                return;
            }
            else  if(version!=ui->lineEditUseIni->text()+"|Author:"+QByteArray::fromBase64(AUTHOR_EMAIL)){
                statusBar_disPlayMessage("不同版本的文件,无法将数据插入!");
                return;
            }
            //文件版本和文件类型匹配开始准备数据转换---开始准备插入数据
            else{
                QList<QByteArray> dataQByteArrayList;
                //提取剪切板数据并校验长度
                //从最后一行倒序提取-边提取边删除QStringlist的最后一行，释放内存
                int count=data.count();
                bool dataok=true;
                for(int i=count-1;i>=2;i--){
                    QByteArray row=codecOFD->fromUnicode(data.at(i));
                    //匹配
                    if(row.length()==ofd.getRowLength()){
                        //字节数据插入一行数据，QStringlist删除一行数据
                        dataQByteArrayList.append(qCompress(row,dataCompressLevel));
                        data.removeAt(i);
                    }
                    else{
                        dataok=false;
                        statusBar_disPlayMessage(QString("剪切板上第%1行数据的长度不满足接口规范,放弃插入剪切板数据!").arg(i));
                        break;
                    }
                }
                //如果数据都OK。就插入数据到OFD原始记录数据
                if(dataok){
                    dataBlocked=true;
                    dataBlockedMessage="正在写入剪切板数据";
                    //清空比对器内容
                    compareData.clear();
                    int dataCount=dataQByteArrayList.count();
                    //倒序插入，边插入边删除dataQByteArrayList
                    //注意这里不要调整顺序，此处倒序和从剪切板拉取数据的倒序相结合，倒倒得正
                    int indexAdd=0;
                    for(int j=dataCount-1;j>=0;j--){
                        ofdFileContentQByteArrayList.insert(insertIndexInContent+indexAdd,dataQByteArrayList.at(j));
                        dataQByteArrayList.removeAt(j);
                        indexAdd++;
                    }
                    //更新OFD记录数标记--在文件头的最后一行
                    ofdFileHeaderQStringList.replace(ofdFileHeaderQStringList.count()-1,QString("%1").arg(ofdFileContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
                    //新的总行数
                    int rowCount=ofdFileContentQByteArrayList.count();
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
                        statusBar_disPlayMessage(QString("剪切板上的%1行数据插入到本文件完毕,记得保存文件哟...").arg(dataCount));
                        this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
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
                            fIndex=ofdFileContentQByteArrayList.count()-dataCount;
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
                            statusBar_disPlayMessage(QString("剪切板上的%1行数据插入到本文件完毕,记得保存文件哟...").arg(dataCount));
                        }
                        else{
                            statusBar_disPlayMessage(QString("剪切板上的%1行数据插入到本文件完毕,已选中插入的第一行数据,记得保存文件哟...").arg(dataCount));
                        }
                        //强制触发下刷新，避免显示数据不完整
                        acceptVScrollValueChanged(0);
                        this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    }
                    //////////////////////////////////////////////////////////
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
    statusBar_disPlayMessage(QString("查看第%1行数据").arg(dataRowCurrent+1));
    //定义一个Qlist存储此行的数据,将表格的列转换为行，共计四列
    //数据内容从表格取，从原始数据取还需要转换
    QList<QStringList> rowdata;
    //数据类型插入点
    if(currentOpenFileType==1){
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
            rowdata.append(colitem);
        }
    }
    if(currentOpenFileType==2){
        for(int i=0;i<colCount;i++){
            QStringList colitem;
            if(ptr_table->item(tableRowCurrent,i)==nullptr){
                //字段中文名
                colitem.append(ptr_table->horizontalHeaderItem(i)->text());
                //字段英文名
                colitem.append(nullptr);
                //字段值
                colitem.append(nullptr);
                //字典翻译
                colitem.append(nullptr);
            }
            else{
                QString colvalue=ptr_table->item(tableRowCurrent,i)->text();
                //字段名
                colitem.append(ptr_table->horizontalHeaderItem(i)->text());
                colitem.append(nullptr);
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(commonDictionary.value("CSV"+csv.getFileIni()).getDictionary(csv.getFieldList().at(i).getFieldName(),colvalue));
            }
            rowdata.append(colitem);
        }
    }
    if(currentOpenFileType==3){
        for(int i=0;i<colCount;i++){
            QStringList colitem;
            if(ptr_table->item(tableRowCurrent,i)==nullptr){
                //字段中文名
                colitem.append(fixed.getFieldList().at(i).getFieldDescribe());
                //字段英文名
                colitem.append(nullptr);
                //字段值
                colitem.append(nullptr);
                //字典翻译
                colitem.append(nullptr);
            }
            else{
                QString colvalue=ptr_table->item(tableRowCurrent,i)->text();
                //字段名
                colitem.append(fixed.getFieldList().at(i).getFieldDescribe());
                colitem.append(nullptr);
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(commonDictionary.value("FIXED"+fixed.getFileIni()).getDictionary(fixed.getFieldList().at(i).getFieldDescribe(),colvalue));
            }
            rowdata.append(colitem);
        }
    }
    //打开窗口
    DialogShowTableRow * dialog = new DialogShowTableRow(&rowdata,this);
    dialog->setWindowTitle(QString("查看表格行记录-第%1行").arg(dataRowCurrent+1));
    dialog->setModal(false);
    dialog->show();
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
 * @brief MainWindow::showOFDFiledAnalysis 用于分析OFD文件列合法性的工具
 */
void MainWindow:: showOFDFiledAnalysis(){
    //注意，这个函数不建议复用，如果需要进行别的文件类别合法性分析，建议设置单独的函数
    if(currentOpenFileType==1){
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        statusBar_disPlayMessage(QString("分析第%1行第%2列数据数据").arg(rowRealInContent+1).arg(tableColCurrent+1));
        //字段中文名
        QString fieldDes=ofd.getFieldList().at(tableColCurrent).getFieldDescribe();
        //字段英文名
        QString fieldName=ofd.getFieldList().at(tableColCurrent).getFieldName();
        //字段类型
        QString fieldType=ofd.getFieldList().at(tableColCurrent).getFieldType();
        //字段长度
        int fieldLength=ofd.getFieldList().at(tableColCurrent).getLength();
        //字段小数长度
        int fieldDecLength=ofd.getFieldList().at(tableColCurrent).getDecLength();
        //字段原始值
        QString fieldOaiginal=Utils::getOriginalValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,tableColCurrent);
        //字段翻译值
        QString fieldValues="";
        if(fieldType=="N"){
            fieldValues=Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,tableColCurrent));
        }
        else{
            fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,tableColCurrent);
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
        /////////////合法校验/////////
        QStringList rowfieldCheck;
        rowfieldCheck.append("字段值合法性");
        if(fieldType=="C"){
            //字符型,万能接收
            rowfieldCheck.append("字段值符合接口约束");
        }
        else if(fieldType=="A"){
            //数字字符型
            /*
        1:不能包含空格数字字母外的任何值
        2:不能以空格开头
        */
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
            //数字型
            /*
        1:不能包含数字外的任何值
        */
            //如果一个N类型的字段合法，那么肯定可以转换成一个数字
            if(fieldOaiginal.trimmed().isEmpty()){
                rowfieldCheck.append("数值型字段不建议使用空格填充,部分厂商系统不支持,空值建议填充0");
            }
            else if(fieldOaiginal.contains(" ")){
                rowfieldCheck.append("数值型字段不建议使用空格填充,部分厂商系统不支持,长度补位建议填充0");
            }else{
                bool ok;
                QString(fieldOaiginal).toLongLong(&ok);
                if(ok){
                    rowfieldCheck.append("字段值符合接口约束");
                }
                else{
                    rowfieldCheck.append("字段内容错误,本字段不应该包含0-9外的字符");
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
        //打开窗口
        DialogShowTableFieldCheck * dialog = new DialogShowTableFieldCheck(&data,this);
        dialog->setWindowTitle(QString("分析第%1行第%2列数据数据").arg(rowRealInContent+1).arg(tableColCurrent+1));
        dialog->setModal(false);
        dialog->show();
    }
}

/**
 * @brief MainWindow::showModifyOFDCell OFD专用修改工具函数
 */
void MainWindow::showModifyOFDCell(){
    if(currentOpenFileType==1){
        //标记要编辑的位置--注意做行转换
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        //表格中的行
        int editRow=tableRowCurrent;
        int editCol=tableColCurrent;
        //字段类型
        QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
        //字段长度
        int fieldLength=ofd.getFieldList().at(editCol).getLength();
        //字段小数长度
        int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
        //获取字段目前的值
        QString fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,editCol);
        //字段修改标记
        bool modifyFlag=false;
        //修改后的值
        QString valueNew="";
        //打开窗口
        DialogModifyCell * dialog = new DialogModifyCell(fieldType,fieldLength,fieldDecLength,fieldValues,this);
        dialog->setWindowTitle(QString("编辑第%1行第%2列-"+ofd.getFieldList().at(tableColCurrent).getFieldDescribe()).arg(rowRealInContent+1).arg(tableColCurrent+1));
        dialog->setModal(true);
        dialog->exec();
        //从弹窗中获取结果
        modifyFlag=dialog->getModifyFlag();
        valueNew=dialog->getValueNew();
        //模态的使用完毕删除
        delete dialog;
        dialog=nullptr;
        //开始处理是否需要更新
        if(modifyFlag){
            //检验新值是否和旧值相等,如果相等认为没有编辑
            if(valueNew==fieldValues){
                statusBar_disPlayMessage("单元格值没有变化,无需保存...");
            }
            else{
                //更新原始记录//////////////////////////////////////////////////////
                //要更新的范围
                int updateBegin=ofd.getFieldList().at(tableColCurrent).getRowBeginIndex();
                int updateEnd=updateBegin+fieldLength;
                //新的单元格值的字节数组
                QByteArray valueNewArray=codecOFD->fromUnicode(valueNew);
                //本行记录原始内容
                QByteArray valueNewArrayRow=qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent));
                //判断数据类型处理
                //数值字符型,字符型,长文本型对于长度不够的情况直接补充空格即可
                if(fieldType=="C"||fieldType=="TEXT"||fieldType=="A"){
                    int index=0;
                    for(int i=updateBegin;i<updateEnd;i++){
                        if(index<valueNewArray.length()){
                            valueNewArrayRow[i]=valueNewArray[index];
                            index++;
                        }
                        //超过填写的参数的部分使用空格补充
                        else{
                            valueNewArrayRow[i]=' ';
                        }
                    }
                }
                //数值型
                else if(fieldType=="N"){
                    //空数据自动补充为空格，0自动补充为全0，不再强制将空补充为0
                    if(valueNew.isEmpty()){
                        for(int i=updateBegin;i<updateEnd;i++){
                            valueNewArrayRow[i]=codecOFD->fromUnicode(QString(" ")).at(0);
                        }
                    }
                    //仅包含整数部分
                    else if(!valueNew.contains("."))
                    {
                        //合成整数和小数部分
                        int zLength=fieldLength-fieldDecLength;
                        //整数部分不够前补0,缺少多少补多少
                        QString intS=valueNew;
                        if(intS.length()<zLength){
                            int zeroAdd=zLength-intS.length();
                            for(int zz=0;zz<zeroAdd;zz++){
                                intS.insert(0,'0');
                            }
                        }
                        //小数部分直接全部补0
                        QString intD="";
                        if(intD.length()<fieldDecLength){
                            int zeroAdd=fieldDecLength-intD.length();
                            for(int zz=0;zz<zeroAdd;zz++){
                                intD.append('0');
                            }
                        }
                        //整数和小数补充缺少的0结束后开始填充数据
                        QString number=intS+intD;
                        int index=0;
                        for(int i=updateBegin;i<updateEnd;i++){
                            valueNewArrayRow[i]=codecOFD->fromUnicode(number).at(index);
                            index++;
                        }
                    }
                    //整数部分和小数部分都有值
                    else{
                        //分别获取整数和小数
                        int zLength=fieldLength-fieldDecLength;
                        QString intS=valueNew.mid(0,valueNew.indexOf("."));
                        if(intS.length()<zLength){
                            int zeroAdd=zLength-intS.length();
                            for(int zz=0;zz<zeroAdd;zz++){
                                intS.insert(0,'0');
                            }
                        }
                        //小数部分后补0
                        QString intD=valueNew.mid(valueNew.indexOf(".")+1,-1);
                        if(intD.length()<fieldDecLength){
                            int zeroAdd=fieldDecLength-intD.length();
                            for(int zz=0;zz<zeroAdd;zz++){
                                intD.append('0');
                            }
                        }
                        //整数和小数补充缺少的0结束后开始填充数据
                        QString number=intS+intD;
                        int index=0;
                        for(int i=updateBegin;i<updateEnd;i++){
                            valueNewArrayRow[i]=codecOFD->fromUnicode(number).at(index);
                            index++;
                        }
                    }
                }
                //其他未知类型
                else{
                    int index=0;
                    for(int i=updateBegin;i<updateEnd;i++){
                        if(index<valueNewArray.length()){
                            valueNewArrayRow[i]=valueNewArray[index];
                            index++;
                        }
                        //超过填写的参数的部分使用空格补充
                        else{
                            valueNewArrayRow[i]=' ';
                        }
                    }
                }
                //将新的行记录写入原ofdFileContentQByteArrayList/////////////////////////////
                ofdFileContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                //更新界面/////////////////////////////////////////////////////////////////
                QString valueForItem=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,editCol);
                if(ofd.getFieldList().at(editCol).getFieldType()=="N"){
                    valueForItem=Utils::CovertDoubleQStringWithThousandSplit(valueForItem);
                }
                if(ptr_table->item(editRow,tableColCurrent)!=nullptr){
                    ptr_table->item(editRow,tableColCurrent)->setText(valueForItem);
                }
                //如果这个单元格未填充过数据,则QTableWidgetItem不存在
                else if(!valueNew.isEmpty()){
                    QTableWidgetItem *item= new QTableWidgetItem();
                    ptr_table->setItem(editRow, tableColCurrent, item);
                    item->setText(valueForItem);
                }
                //重新校准列宽
                ptr_table->resizeColumnsToContents();
                //如果这行数据在比对器,需要更新////////////////////////////////////////////////
                if(compareData.contains(rowRealInContent+1)){
                    //移除原数据
                    compareData.remove(rowRealInContent+1);
                    //数据重新加入
                    QStringList rowdata;
                    for(int i=0;i<(ptr_table->columnCount());i++){
                        QString col="";
                        if(ptr_table->item(editRow,i)!=nullptr){
                            col=ptr_table->item(editRow,i)->text();
                        }
                        rowdata.append(col);
                    }
                    compareData.insert(rowRealInContent+1,rowdata);
                }
                //提示用户保存//////////////////////////////////////////////////////////////
                statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                //更新文件修改标记//////////////////////////////////////////////////////////
                fileChanged=true;
            }
        }
        else{
            statusBar_disPlayMessage("取消编辑...");
        }
    }
}

/**
 * @brief MainWindow::showModifyOFDCellBatch 批量修改选择的多行同列
 */
//待修改
void MainWindow::showModifyOFDCellBatch(){
    if(currentOpenFileType==1){
        //标记要编辑的列
        int editCol=tableColCurrent;
        //字段类型
        QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
        //字段长度
        int fieldLength=ofd.getFieldList().at(editCol).getLength();
        //字段小数长度
        int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
        //字段修改标记
        bool modifyFlag=false;
        //修改后的值
        //打开窗口
        DialogModifyCell * dialog = new DialogModifyCell(fieldType,fieldLength,fieldDecLength,"",this);
        dialog->setWindowTitle(QString("批量编辑第%1列多个单元格-"+ofd.getFieldList().at(tableColCurrent).getFieldDescribe()).arg(tableColCurrent+1));
        dialog->setModal(true);
        dialog->exec();
        //从弹窗中获取结果
        modifyFlag=dialog->getModifyFlag();
        QString valueNew=dialog->getValueNew();
        //模态的使用完毕删除
        delete dialog;
        dialog=nullptr;
        //开始处理是否需要更新
        if(modifyFlag){
            //新的单元格值的字节数组
            QByteArray valueNewArray=codecOFD->fromUnicode(valueNew);
            //新的单元格数据补齐/////////////
            //判断数据类型处理
            //数值字符型,字符型,长文本型对于长度不够的情况直接补充空格即可
            if(fieldType=="C"||fieldType=="TEXT"||fieldType=="A"){
                int addLength=fieldLength-valueNewArray.length();
                if(valueNewArray.length()<fieldLength){
                    for(int i=0;i<addLength;i++){
                        valueNewArray.append(' ');
                    }
                }
            }
            //数值型
            else if(fieldType=="N"){
                //全空数据自动补充0
                if(valueNew.isEmpty()){
                    for(int i=0;i<fieldLength;i++){
                        valueNewArray.append(codecOFD->fromUnicode(QString("0")).at(0));
                    }
                }
                //仅包含整数部分
                else if(!valueNew.contains("."))
                {
                    //合成整数和小数部分
                    int zLength=fieldLength-fieldDecLength;
                    //整数部分不够前补0,缺少多少补多少
                    QString intS=valueNew;
                    if(intS.length()<zLength){
                        int zeroAdd=zLength-intS.length();
                        for(int zz=0;zz<zeroAdd;zz++){
                            intS.insert(0,'0');
                        }
                    }
                    //小数部分直接全部补0
                    QString intD="";
                    for(int zz=0;zz<fieldDecLength;zz++){
                        intD.append('0');
                    }
                    //整数和小数补充缺少的0结束后合并
                    QString number=intS.append(intD);
                    valueNewArray=codecOFD->fromUnicode(number);
                }
                //整数部分和小数部分都有值
                else{
                    //分别获取整数和小数
                    int zLength=fieldLength-fieldDecLength;
                    QString intS=valueNew.mid(0,valueNew.indexOf("."));
                    if(intS.length()<zLength){
                        int zeroAdd=zLength-intS.length();
                        for(int zz=0;zz<zeroAdd;zz++){
                            intS.insert(0,'0');
                        }
                    }
                    //小数部分后补0
                    QString intD=valueNew.mid(valueNew.indexOf(".")+1,-1);
                    if(intD.length()<fieldDecLength){
                        int zeroAdd=fieldDecLength-intD.length();
                        for(int zz=0;zz<zeroAdd;zz++){
                            intD.append('0');
                        }
                    }
                    //整数和小数补充缺少的0结束后合并
                    QString number=intS.append(intD);
                    valueNewArray=codecOFD->fromUnicode(number);
                }
            }
            //其他未知类型，填充空格
            else{
                if(valueNewArray.length()<fieldLength){
                    int addLength=fieldLength-valueNewArray.length();
                    for(int i=0;i<addLength;i++){
                        valueNewArray.append(' ');
                    }
                }
            }
            int rowRealInContent=0;
            //RangeList
            QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
            if(itemsRange.count()>=1){
                //要更新的列范围
                int updateBegin=ofd.getFieldList().at(editCol).getRowBeginIndex();
                int updateEnd=updateBegin+fieldLength;
                //遍历多个range
                int updatedRow=0;
                QApplication::setOverrideCursor(Qt::WaitCursor);
                for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                    //遍历本range的多行,从上往下逐行修改
                    for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                        rowRealInContent=(currentPage-1)*pageRowSize+editRow;
                        //本行记录原始内容
                        QByteArray valueNewArrayRow=qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent));
                        //更新字段值
                        int index=0;
                        for(int i=updateBegin;i<updateEnd;i++){
                            valueNewArrayRow[i]=valueNewArray.at(index);
                            index++;
                        }
                        //qDebug()<<valueNewArrayRow;
                        //将新的行记录写入原ofdFileContentQByteArrayList/////////////////////////////
                        ofdFileContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                        //更新界面/////////////////////////////////////////////////////////////////
                        QString valueForItem=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,editCol);
                        if(ofd.getFieldList().at(editCol).getFieldType()=="N"){
                            valueForItem=Utils::CovertDoubleQStringWithThousandSplit(valueForItem);
                        }
                        //暂不考虑本行是否已加载
                        if(ptr_table->item(editRow,editCol)!=nullptr){
                            ptr_table->item(editRow,editCol)->setText(valueForItem);
                        }
                        //如果这个单元格未填充过数据,则QTableWidgetItem不存在
                        else if(!valueNew.isEmpty()){
                            QTableWidgetItem *item= new QTableWidgetItem();
                            ptr_table->setItem(editRow, editCol, item);
                            item->setText(valueForItem);
                        }
                        //如果这行数据在比对器,需要更新////////////////////////////////////////////////
                        if(compareData.contains(editRow+1)){
                            //移除原数据
                            compareData.remove(editRow+1);
                            //数据重新加入
                            QStringList rowdata;
                            for(int i=0;i<(ptr_table->columnCount());i++){
                                QString col="";
                                if(ptr_table->item(editRow,i)!=nullptr){
                                    col=ptr_table->item(editRow,i)->text();
                                }
                                rowdata.append(col);
                            }
                            compareData.insert(editRow+1,rowdata);
                        }
                        updatedRow++;
                        if((updatedRow%1000==0)){
                            statusBar_disPlayMessage(QString("批量更新中,请勿进行其他操作,已更新%1行").arg(QString::number(updatedRow)));
                            qApp->processEvents();
                        }
                    }
                }
                QApplication::restoreOverrideCursor();
                //提示用户保存//////////////////////////////////////////////////////////////
                statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                //更新文件修改标记//////////////////////////////////////////////////////////
                fileChanged=true;
            }
            //重新校准列宽
            ptr_table->resizeColumnsToContents();
        }
        else{
            statusBar_disPlayMessage("取消编辑...");
        }
    }
}

/**
 * @brief MainWindow::showMoaifyOFDRow 修改一行ODF文件
 */
//待修改
void MainWindow::showMoaifyOFDRow(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    if(itemsRange.count()>0){
        //获取行号-指向当前鼠标选择的行
        int editRow=itemsRange.at(0).topRow();
        int rowRealInContent=(currentPage-1)*pageRowSize+editRow;
        //获取目前行数据
        QStringList rowDataOld=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent);
        //创建弹窗，传递ofd定义的指针，行数据
        DialogModifyRow * dialog = new DialogModifyRow(&ofd,rowDataOld,this);
        dialog->setWindowTitle(QString("编辑第%1行数据").arg(rowRealInContent+1));
        dialog->setModal(true);
        dialog->exec();
        ///////////////////////////////开始进行数据编辑更新//////////////////////////////////
        //数据发生了改变
        if(dialog->getChange()&&dialog->getSaveFlag()){
            QStringList rowDataNew=dialog->getRowDataNew();
            delete dialog;
            dialog=nullptr;
            //开始更新数据，按列更新
            if(rowDataNew.count()>0){
                //本行记录原始内容
                QByteArray valueNewArrayRow=qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent));
                //列循环/////////////////////////
                //仅更新被修改的列
                for(int editCol=0;editCol<rowDataNew.count()&&editCol<ptr_table->columnCount();editCol++){
                    //字段类型
                    QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
                    //字段长度
                    int fieldLength=ofd.getFieldList().at(editCol).getLength();
                    //字段小数长度
                    int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
                    //获取字段目前的值
                    QString fieldValues=rowDataOld.at(editCol);
                    //修改后的值
                    QString valueNew=rowDataNew.at(editCol);
                    if(fieldValues!=valueNew){
                        //要更新的范围
                        int updateBegin=ofd.getFieldList().at(editCol).getRowBeginIndex();
                        int updateEnd=updateBegin+fieldLength;
                        //新的单元格值的字节数组
                        QByteArray valueNewArray=codecOFD->fromUnicode(valueNew);
                        //判断数据类型处理
                        //数值字符型,字符型,长文本型对于长度不够的情况直接补充空格即可
                        if(fieldType=="C"||fieldType=="TEXT"||fieldType=="A"){
                            int index=0;
                            for(int i=updateBegin;i<updateEnd;i++){
                                if(index<valueNewArray.length()){
                                    valueNewArrayRow[i]=valueNewArray[index];
                                    index++;
                                }
                                //超过填写的参数的部分使用空格补充
                                else{
                                    valueNewArrayRow[i]=' ';
                                }
                            }
                        }
                        //数值型
                        else if(fieldType=="N"){
                            //空数据自动补充为空格，0自动补充为全0，不再强制将空补充为0
                            if(valueNew.isEmpty()){
                                for(int i=updateBegin;i<updateEnd;i++){
                                    valueNewArrayRow[i]=codecOFD->fromUnicode(QString(" ")).at(0);
                                }
                            }
                            //仅包含整数部分
                            else if(!valueNew.contains("."))
                            {
                                //合成整数和小数部分
                                int zLength=fieldLength-fieldDecLength;
                                //整数部分不够前补0,缺少多少补多少
                                QString intS=valueNew;
                                if(intS.length()<zLength){
                                    int zeroAdd=zLength-intS.length();
                                    for(int zz=0;zz<zeroAdd;zz++){
                                        intS.insert(0,'0');
                                    }
                                }
                                //小数部分直接全部补0
                                QString intD="";
                                if(intD.length()<fieldDecLength){
                                    int zeroAdd=fieldDecLength-intD.length();
                                    for(int zz=0;zz<zeroAdd;zz++){
                                        intD.append('0');
                                    }
                                }
                                //整数和小数补充缺少的0结束后开始填充数据
                                QString number=intS+intD;
                                int index=0;
                                for(int i=updateBegin;i<updateEnd;i++){
                                    valueNewArrayRow[i]=codecOFD->fromUnicode(number).at(index);
                                    index++;
                                }
                            }
                            //整数部分和小数部分都有值
                            else{
                                //分别获取整数和小数
                                int zLength=fieldLength-fieldDecLength;
                                QString intS=valueNew.mid(0,valueNew.indexOf("."));
                                if(intS.length()<zLength){
                                    int zeroAdd=zLength-intS.length();
                                    for(int zz=0;zz<zeroAdd;zz++){
                                        intS.insert(0,'0');
                                    }
                                }
                                //小数部分后补0
                                QString intD=valueNew.mid(valueNew.indexOf(".")+1,-1);
                                if(intD.length()<fieldDecLength){
                                    int zeroAdd=fieldDecLength-intD.length();
                                    for(int zz=0;zz<zeroAdd;zz++){
                                        intD.append('0');
                                    }
                                }
                                //整数和小数补充缺少的0结束后开始填充数据
                                QString number=intS+intD;
                                int index=0;
                                for(int i=updateBegin;i<updateEnd;i++){
                                    valueNewArrayRow[i]=codecOFD->fromUnicode(number).at(index);
                                    index++;
                                }
                            }
                        }
                        //其他未知类型
                        else{
                            int index=0;
                            for(int i=updateBegin;i<updateEnd;i++){
                                if(index<valueNewArray.length()){
                                    valueNewArrayRow[i]=valueNewArray[index];
                                    index++;
                                }
                                //超过填写的参数的部分使用空格补充
                                else{
                                    valueNewArrayRow[i]=' ';
                                }
                            }
                        }
                        //将新的行记录写入原ofdFileContentQByteArrayList/////////////////////////////
                        ofdFileContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                        //更新界面/////////////////////////////////////////////////////////////////
                        QString itemValue=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,editCol);
                        if(fieldType=="N"){
                            itemValue=Utils::CovertDoubleQStringWithThousandSplit(itemValue);
                        }
                        if(ptr_table->item(editRow,editCol)!=nullptr){

                            ptr_table->item(editRow,editCol)->setText(itemValue);
                        }
                        //如果这个单元格未填充过数据,则QTableWidgetItem不存在
                        else if(!valueNew.isEmpty()){
                            QTableWidgetItem *item= new QTableWidgetItem();
                            ptr_table->setItem(editRow, editCol, item);
                            item->setText(itemValue);
                        }
                    }
                }
                //重新校准列宽
                ptr_table->resizeColumnsToContents();
                //如果这行数据在比对器,需要更新////////////////////////////////////////////////
                if(compareData.contains(rowRealInContent+1)){
                    //移除原数据
                    compareData.remove(rowRealInContent+1);
                    //数据重新加入
                    compareData.insert(rowRealInContent+1,rowDataNew);
                }
                //提示用户保存//////////////////////////////////////////////////////////////
                statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                //更新文件修改标记//////////////////////////////////////////////////////////
                fileChanged=true;
            }
        }
        //数据未发生改变
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
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用搜索功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==-1){
        return;
    }
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //数据类型加入点
    if(currentOpenFileType==1){
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==2){
        if(csvFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==3){
        if(fixedContenQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
    }
    //向上搜索
    if(ui->lineTextText->text().isEmpty()){
        statusBar_disPlayMessage("请填写你要搜索的内容...");
        return;
    }
    if(tableRowCurrent==0&&tableColCurrent==0){
        statusBar_disPlayMessage("再往上没有你要搜索的内容了...");
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        return;
    }
    dataBlocked=true;
    //开始搜索
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->pushButtonPreSearch->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    QString searchText=ui->lineTextText->text();
    //判断是否是当前搜索行,如果是则从焦点单元格前一个单元格搜索,否则从行记录的最后一个单元格搜索
    //搜索当前行时,不能从最后一列开始,要从焦点单元格前一个开始
    int beginCol=tableColCurrent-1;
    //查询总列数
    int colCount=0;
    if(currentOpenFileType==1){
        colCount=ofd.getFieldCount();
    }
    if(currentOpenFileType==2){
        colCount=csv.getFieldCount();
    }
    if(currentOpenFileType==3){
        colCount=fixed.getFieldCountMax();
    }
    //分页模式下的跨页搜索
    int rowRealInContent=0;
    while(currentPage>0){
        //数据类型插入点
        if(currentOpenFileType==1){
            for(int row=tableRowCurrent;row>=0;row--){
                for(int col=beginCol;col>=0;col--){
                    rowRealInContent=(currentPage-1)*pageRowSize+row;
                    if(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,col).contains(searchText,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->pushButtonPreSearch->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        return;
                    }
                }
                beginCol=colCount-1;
                if(row%500==0){
                    statusBar_disPlayMessage("正在搜索,请耐心等待...");
                    qApp->processEvents();
                }
            }
        }
        if(currentOpenFileType==2){
            for(int row=tableRowCurrent;row>=0;row--){
                //csv文件按行获取数据
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,rowRealInContent,csv.getEcoding());
                //csv文件的数据可能缺失列，因为读取的时候没有做强制判断，这里判断补充下,如果数据长度不够，补空数据
                while(rowdata.count()<colCount){
                    rowdata.append("");
                }
                for(int col=beginCol;col>=0;col--){
                    if(rowdata.at(col).contains(searchText,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->pushButtonPreSearch->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        return;
                    }
                }
                beginCol=colCount-1;
                if(row%500==0){
                    statusBar_disPlayMessage("正在搜索,请耐心等待...");
                    qApp->processEvents();
                }
            }
        }
        if(currentOpenFileType==3){
            for(int row=tableRowCurrent;row>=0;row--){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,rowRealInContent,fixed.getEcoding());
                for(int col=beginCol;col>=0;col--){
                    //无此列数据的值
                    if(col>rowdata.count()-1){
                        continue;
                    }
                    if(rowdata.at(col).contains(searchText,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->pushButtonPreSearch->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        return;
                    }
                }
                beginCol=colCount-1;
                if(row%500==0){
                    statusBar_disPlayMessage("正在搜索,请耐心等待...");
                    qApp->processEvents();
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
            tableRowCurrent=ptr_table->rowCount()-1;
        }
        else{
            statusBar_disPlayMessage("再往上没有你要搜索的内容了...");
            dataBlocked=false;
            ui->pushButtonPreSearch->setEnabled(true);
            ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
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
    if(isUpdateData){
        statusBar_disPlayMessage("正在加载数据,请稍后再使用搜索功能...");
        return;
    }
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(currentOpenFileType==-1){
        return;
    }
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //行列数
    int rowcount=0;
    int colCount=0;
    //数据类型加入点
    if(currentOpenFileType==1){
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }else{
            rowcount=ofdFileContentQByteArrayList.count();
            colCount=ofd.getFieldCount();
        }
    }
    if(currentOpenFileType==2){
        if(csvFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }else{
            rowcount=csvFileContentQByteArrayList.count();
            colCount=csv.getFieldCount();
        }
    }
    if(currentOpenFileType==3){
        if(fixedContenQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }else{
            rowcount=fixedContenQByteArrayList.count();
            colCount=fixed.getFieldCountMax();
        }
    }
    //向下搜索
    if(ui->lineTextText->text().isEmpty()){
        statusBar_disPlayMessage("请填写你要搜索的内容...");
        return;
    }
    if(tableRowCurrent==rowcount-1&&tableColCurrent==colCount-1){
        statusBar_disPlayMessage("再往下没有你要搜索的内容了...");
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        return;
    }
    dataBlocked=true;
    //开始搜索
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->pushButtonNextSearch->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    QString searchText=ui->lineTextText->text();
    //分页搜索
    //是否是当前搜索行,如果是则从焦点单元格后一个单元格搜索,否则从行记录的第一个单元格搜索
    int currentPageRowCount=ptr_table->rowCount();
    int beginCol=tableColCurrent+1;
    int rowRealInContent=0;
    while(currentPage<=pageCount){
        //数据类型加入点
        if(currentOpenFileType==1){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                for(int col=beginCol;col<colCount;col++){
                    if(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowRealInContent,col).contains(searchText,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->pushButtonNextSearch->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        return;
                    }
                }
                beginCol=0;
                if(row%500==0){
                    statusBar_disPlayMessage("正在搜索,请耐心等待...");
                    qApp->processEvents();
                }
            }
        }
        if(currentOpenFileType==2){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                //csv文件按行获取数据
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,rowRealInContent,csv.getEcoding());
                //csv文件的数据可能缺失列，因为读取的时候没有做强制判断，这里判断补充下,如果数据长度不够，补空数据
                while(rowdata.count()<colCount){
                    rowdata.append("");
                }
                for(int col=beginCol;col<colCount;col++){
                    if(rowdata.at(col).contains(searchText,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->pushButtonNextSearch->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        return;
                    }
                }
                beginCol=0;
                if(row%500==0){
                    statusBar_disPlayMessage("正在搜索,请耐心等待...");
                    qApp->processEvents();
                }
            }
        }
        if(currentOpenFileType==3){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,rowRealInContent,fixed.getEcoding());
                for(int col=beginCol;col<colCount;col++){
                    //超过本行最大列，直接break
                    if(col>=rowdata.count()){
                        break;
                    }
                    if(rowdata.at(col).contains(searchText,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->pushButtonNextSearch->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        return;
                    }
                }
                beginCol=0;
                if(row%500==0){
                    statusBar_disPlayMessage("正在搜索,请耐心等待...");
                    qApp->processEvents();
                }
            }
        }
        //不是最后一页
        //跳转到下一页
        if(currentPage<pageCount){
            currentPage++;
            //跳到上一页面
            pageJump(currentPage,0);
            //重新设置从左上角开始继续搜索
            beginCol=0;
            tableRowCurrent=0;
            currentPageRowCount=ptr_table->rowCount();
        }
        //最后一页
        else{
            statusBar_disPlayMessage("再往下没有你要搜索的内容了...");
            dataBlocked=false;
            ui->pushButtonNextSearch->setEnabled(true);
            ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            break;
        }
    }
}




void MainWindow::showMessage_customContextMenuRequested(const QPoint &pos)
{
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
    //特殊情况当前打开的文件是OFD文件，总行数是0的空文件允许插入内容--仅仅允许插入数据
    if(currentOpenFileType==1&&ptr_table->rowCount()<1&&ptr_table->rowAt(pos.y()) <0){
        tablePopMenu->addAction(action_addCopyedOFDData2End);
        tablePopMenu->addAction(action_addNewLineOFDData2End);
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
    if(currentOpenFileType==1){
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
                    tablePopMenu->addAction(action_ShowOFDAnalysis);
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_ModifyOFDCell);
                    tablePopMenu->addAction(action_ModifyOFDRow);
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
                    tablePopMenu->addSeparator();
                }
                //单行多列
                else{
                    tablePopMenu->addAction(action_ShowCopyColum);
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addSeparator();
                    tablePopMenu->addAction(action_ModifyOFDRow);
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
                    tablePopMenu->addSeparator();
                }
                //单行删除和复制
                tablePopMenu->addAction(action_DeleteOFDRowData);
                action_DeleteOFDRowData->setText("删除光标所在行的整行数据记录");
                tablePopMenu->addAction(action_addNewLineOFDData2End);
                tablePopMenu->addSeparator();
                tablePopMenu->addAction(action_CopyOFDRowData);
                action_CopyOFDRowData->setText("复制光标所在整行数据记录(专用于合并插入OFD数据)");
                //数据插入菜单
                tablePopMenu->addAction(action_addCopyedOFDData2PreviousRow);
                tablePopMenu->addAction(action_addCopyedOFDData2NextRow);
                tablePopMenu->addAction(action_addCopyedOFDData2End);

            }
            //多行
            else {
                //多行单列
                if(allSelectIsOneColumn){
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_ModifyOFDCellBatch);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addSeparator();
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误--跨选取复制
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_ModifyOFDCellBatch);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addSeparator();

                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addSeparator();
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误--跨选取复制
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addSeparator();
                    }
                }
                //多行情况下均展示删除选择的行
                tablePopMenu->addAction(action_DeleteOFDRowData);
                action_DeleteOFDRowData->setText("删除选择范围内的整行数据记录");
                //新增行
                tablePopMenu->addAction(action_addNewLineOFDData2End);
                tablePopMenu->addSeparator();
                tablePopMenu->addAction(action_CopyOFDRowData);
                action_CopyOFDRowData->setText("复制选择范围内的整行数据记录(专用于合并插入OFD数据)");
                //多行时展示插入到文件尾部
                tablePopMenu->addAction(action_addCopyedOFDData2End);
            }
        }
    }
    //csv类型的文件
    if(currentOpenFileType==2){
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
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addAction(action_ShowCopyColum);
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
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addAction(action_ShowCopyColum);
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
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);

                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);

                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_EditCompareDataBatch);

                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
            }
        }
    }
    if(currentOpenFileType==3){
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
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addAction(action_ShowCopyColum);
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
                    tablePopMenu->addAction(action_ShowDetails);
                    tablePopMenu->addAction(action_ShowCopyColum);
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
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        //tablePopMenu->addAction(action_ModifyCellBatch);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
                //多行多列
                else{
                    //单个选择器
                    if(rangeCount==1){
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                    }
                }
            }
        }
    }
    //索引模式也支持复制
    //为打开文件或者打开失败时，有可能表格显示了失败原因，开启复制功能
    if(currentOpenFileType==-1||currentOpenFileType==0){
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
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引文件不支持使用比对器...");
    }
    //数据类型插入点
    else if(currentOpenFileType==1||currentOpenFileType==2||currentOpenFileType==3){
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
            for(int i=0;i<ofd.getFieldList().count();i++){
                //仅获取列的中文备注当作列标题
                title.append(ofd.getFieldList().at(i).getFieldDescribe());
                fieldType.append(ofd.getFieldList().at(i).getFieldType());
            }
            //打开窗口
            if(title.count()>0){
                DialogShowTableCompareView * dialog = new DialogShowTableCompareView(title,fieldType,&compareData,this);
                dialog->setWindowTitle(QString("行比对器视图"));
                dialog->setModal(false);
                dialog->show();
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
//待分页改造,增加分页导出文件的提示-支持一页导出一个文件
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
    if(currentOpenFileType==-1){
        return;
    }
    //数据类型判断
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引数据不支持导出");
        return;
    }
    //预先数据判断
    if(ptr_table->rowCount()<1){
        statusBar_disPlayMessage("没有数据可供导出,目前表格有0行数据");
        return;
    }
    //数据类型插入点
    else if(currentOpenFileType==1||currentOpenFileType==2||currentOpenFileType==3){
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
        //文件过滤器,用于追踪选择的保存文件类别
        QString selectedFilter=Q_NULLPTR;
        //弹出保存框
        //数据类型插入点，csv文件不支持导出csv文件
        QString fileTypeSelect="";
        if(currentOpenFileType==1){
            fileTypeSelect=tr("Excel文件(*.xlsx);;Csv文件(*.csv);;Html文件(*.html)");
        }
        if(currentOpenFileType==2){
            fileTypeSelect=tr("Excel文件(*.xlsx);;Html文件(*.html)");
        }
        if(currentOpenFileType==3){
            fileTypeSelect=tr("Excel文件(*.xlsx);;Csv文件(*.csv);;Html文件(*.html)");
        }
        QString fileNameSave = QFileDialog::getSaveFileName(this,("文件数据导出"),openpath+filename,fileTypeSelect,&selectedFilter);
        if(!fileNameSave.isEmpty()){
            //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
            if(selectedFilter=="Excel文件(*.xlsx)"&&(!fileNameSave.endsWith(".xlsx"))){
                fileNameSave.append(".xlsx");
            }
            else if(selectedFilter=="Csv文件(*.csv)"&&(!fileNameSave.endsWith(".csv"))){
                fileNameSave.append(".csv");
            }
            else if(selectedFilter=="Html文件(*.html)"&&(!fileNameSave.endsWith(".html"))){
                fileNameSave.append(".html");
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
                save2Xlsx(fileNameSave);
                //xslx的保存，不在这里取消阻断，交后台存储线程
            }
            else if(fileNameSave.endsWith("csv",Qt::CaseSensitive)){
                save2Csv(fileNameSave);
                dataBlocked=false;
                isExportFile=false;
            }
            else if(fileNameSave.endsWith("html",Qt::CaseSensitive)){
                save2Html(fileNameSave);
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
void MainWindow::save2Csv(QString filename){
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile data(filename);
    //数据类型插入点
    if(currentOpenFileType==1){
        if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
            //开始准备待写入的数据
            QString sb;
            //标题
            for(int i=0;i<ofd.getFieldCount();i++){
                if(i<ofd.getFieldCount()-1){
                    sb.append(ofd.getFieldList().at(i).getFieldDescribe()).append("\t");
                }
                else{
                    sb.append(ofd.getFieldList().at(i).getFieldDescribe()).append("\r\n");
                }
            }
            data.write(codecOFD->fromUnicode(sb));
            data.flush();
            //文本内容
            sb.clear();
            for (int row=0;row<ofdFileContentQByteArrayList.count();row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row);
                for(int col=0;col<ofd.getFieldCount();col++){
                    if(col<ofd.getFieldCount()-1){
                        sb.append(rowdata.at(col)).append("\t");
                    }
                    else{
                        sb.append(rowdata.at(col)).append("\r\n");
                    }
                }
                //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                //仅1000行或者到最后一行时进行写入
                if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                    data.write(codecOFD->fromUnicode(sb));
                    data.flush();
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                }
            }
            data.close();
            statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
        }else{
            statusBar_disPlayMessage(tr("数据导出失败,请重试"));
        }
    }
    if(currentOpenFileType==3){
        if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
            QTextCodec *codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            //开始准备待写入的数据
            QString sb;
            //标题
            for(int i=0;i<fixed.getFieldCountMax();i++){
                if(i<fixed.getFieldCountMax()-1){
                    sb.append(fixed.getFieldList().at(i).getFieldDescribe()).append("\t");
                }
                else{
                    sb.append(fixed.getFieldList().at(i).getFieldDescribe()).append("\r\n");
                }
            }
            data.write(codec->fromUnicode(sb));
            data.flush();
            //文本内容
            sb.clear();
            for (int row=0;row<fixedContenQByteArrayList.count();row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,row,fixed.getEcoding());
                for(int col=0;col<fixed.getFieldCountMax();col++){
                    if(col<fixed.getFieldCountMax()-1){
                        sb.append(col<rowdata.count()?rowdata.at(col):"").append("\t");
                    }
                    else{
                        sb.append(col<rowdata.count()?rowdata.at(col):"").append("\r\n");
                    }
                }
                //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                //仅1000行或者到最后一行时进行写入
                if((row%1000==0)||(row==fixedContenQByteArrayList.count()-1)){
                    data.write(codec->fromUnicode(sb));
                    data.flush();
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
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
//待评估分页改造
void MainWindow::save2Html (QString filename){
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
        if(currentOpenFileType==1){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\"GB18030\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        }
        if(currentOpenFileType==2){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\""+csv.getEcoding()+"\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        }
        if(currentOpenFileType==3){
            sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\""+fixed.getEcoding()+"\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
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
        if(currentOpenFileType==1){
            colCount=ofd.getFieldCount();
            codec=codecOFD;
            for(int i=0;i<colCount;i++){
                if(i<colCount-1){
                    sb.append("<th>").append(ofd.getFieldList().at(i).getFieldDescribe()).append("</th>");
                }
                else{
                    sb.append("<th>").append(ofd.getFieldList().at(i).getFieldDescribe()).append("</th></tr>\r\n");
                }
            }
        }
        if(currentOpenFileType==2){
            colCount=csv.getFieldCount();
            codec=QTextCodec::codecForName(csv.getEcoding().toLocal8Bit());
            for(int i=0;i<ptr_table->columnCount();i++){
                if(i<colCount-1){
                    sb.append("<th>").append(ptr_table->horizontalHeaderItem(i)->text()).append("</th>");
                }
                else{
                    sb.append("<th>").append(ptr_table->horizontalHeaderItem(i)->text()).append("</th></tr>\r\n");
                }
            }
        }
        if(currentOpenFileType==3){
            colCount=fixed.getFieldCountMax();
            codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            for(int i=0;i<colCount;i++){
                if(i<colCount-1){
                    sb.append("<th>").append(fixed.getFieldList().at(i).getFieldDescribe()).append("</th>");
                }
                else{
                    sb.append("<th>").append(fixed.getFieldList().at(i).getFieldDescribe()).append("</th></tr>\r\n");
                }
            }
        }
        //输出表头
        data.write(codec->fromUnicode(sb));
        data.flush();
        //文本内容
        sb.clear();
        //数据类型插入点
        if(currentOpenFileType==1){
            for (int row=0;row<ofdFileContentQByteArrayList.count();row++){
                //数据写入--按行读取
                sb.append("<tr>");
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row);
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
                //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                //仅1000行或者到最后一行时进行写入
                if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                    data.write(codec->fromUnicode(sb));
                    data.flush();
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                }
            }
        }
        if(currentOpenFileType==2){
            for (int row=0;row<csvFileContentQByteArrayList.count();row++){
                //数据写入--按行读取
                //csv文件按行获取数据
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,row,csv.getEcoding());
                sb.append("<tr>");
                for(int col=0;col<colCount;col++){
                    if(col<rowdata.count()){
                        if(fieldIsNumberOrNot.contains(col)&&fieldIsNumberOrNot.value(col).getIsNumber()){
                            sb.append("<td class=\"number\">");
                        }
                        else{
                            sb.append("<td>");
                        }
                        sb.append((fieldIsNumberOrNot.contains(col)&&fieldIsNumberOrNot.value(col).getIsNumber())?Utils::CovertDoubleQStringWithThousandSplit(rowdata.at(col)):rowdata.at(col)).append("</td>");
                    }
                    else{
                        sb.append("<td></td>");
                    }
                }
                sb.append("</tr>\r\n");
                //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                //仅1000行或者到最后一行时进行写入
                if((row%1000==0)||(row==csvFileContentQByteArrayList.count()-1)){
                    data.write(codec->fromUnicode(sb));
                    data.flush();
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                }
            }
        }
        if(currentOpenFileType==3){
            int rowNumber=fixedContenQByteArrayList.count();
            for (int row=0;row<rowNumber;row++){
                //数据写入--按行读取
                sb.append("<tr>");
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,row,fixed.getEcoding());
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
                //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
                //仅1000行或者到最后一行时进行写入
                if((row%1000==0)||(row==rowNumber-1)){
                    data.write(codec->fromUnicode(sb));
                    data.flush();
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
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
//待分页改造
void MainWindow::save2Xlsx(QString filename){
    xlsxSaveName=filename;
    //禁止导出过大的excel文件
    //数据类型插入点
    int row=0;
    if(currentOpenFileType==1){
        row=ofdFileContentQByteArrayList.count();
    }
    else if(currentOpenFileType==2){
        row=csvFileContentQByteArrayList.count();
    }
    else if(currentOpenFileType==3){
        row=fixedContenQByteArrayList.count();
    }
    if(row>maxExcelRow){
        statusBar_disPlayMessage("记录数大于"+QString::number(maxExcelRow)+"行,无法使用导出到excel,请导出csv或者html(如有需求导出到excel请联系"+QByteArray::fromBase64(AUTHOR_EMAIL)+")");
        dataBlocked=false;
        isExportFile=false;
        return;
    }
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
    const int columnCount=ptr_table->columnCount();
    int colWidthArray[columnCount];
    //标题和是否是数值列,数值列需要设置列格式
    QHash<int,QXlsx::Format> numberFormatQhash;
    //标准文本样式
    QXlsx::Format formatBody;
    formatBody.setFont(QFont("SimSun"));
    formatBody.setNumberFormat("@");
    //导出OFD文件的内容
    if(currentOpenFileType==1){
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
                QString zj="#";
                QString z="0";
                QString xj="";
                QString x="";
                int lengthx=ofd.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    xj.append("#");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+","+xj+z+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj+z);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        for (int row=0;row<ofdFileContentQByteArrayList.count();row++){
            //数据写入--按行读取
            QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row);
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
                    if(okNb){
                        xlsx->write(row+2,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(row+2,col+1,value,numberFormatQhash.value(col));
                    }
                }else{
                    xlsx->write(row+2,col+1,value,formatBody);
                }
            }
            //每100行读取下事件循环
            //excel导出较慢
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(row)));
                qApp->processEvents();
            }
        }
    }
    //导出CSV文件的内容
    if(currentOpenFileType==2){
        //标题内容
        for(int i=0;i<columnCount;i++){
            xlsx->write(1,i+1,ptr_table->horizontalHeaderItem(i)->text(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(ptr_table->horizontalHeaderItem(i)->text().toLocal8Bit().length()+5);
            //插入列格式
            if(fieldIsNumberOrNot.contains(i)&&fieldIsNumberOrNot.value(i).getIsNumber()){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#";
                QString z="0";
                QString xj="";
                QString x="";
                int lengthx=fieldIsNumberOrNot.value(i).getDecimalLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    xj.append("#");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+","+xj+z+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj+z);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        for (int row=0;row<csvFileContentQByteArrayList.count();row++){
            //数据写入--按行读取
            //csv文件按行获取数据
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,row,csv.getEcoding().toLocal8Bit());
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
                if(fieldIsNumberOrNot.contains(col)&&numberFormatQhash.contains(col)){
                    bool okNb=false;
                    double number=value.toDouble(&okNb);
                    if(okNb){
                        xlsx->write(row+2,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(row+2,col+1,value,numberFormatQhash.value(col));
                    }
                }else{
                    xlsx->write(row+2,col+1,value,formatBody);
                }
            }
            //每100行读取下事件循环
            //excel导出较慢
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(row)));
                qApp->processEvents();
            }
        }
    }
    //fixed文件
    if(currentOpenFileType==3){
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
                QString zj="#";
                QString z="0";
                QString xj="";
                QString x="";
                int lengthx=fixed.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    xj.append("#");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+","+xj+z+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj+z);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int rowSum=fixedContenQByteArrayList.count();
        for (int row=0;row<rowSum;row++){
            //数据写入--按行读取
            QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,row,fixed.getEcoding().toLocal8Bit());
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
                    if(okNb){
                        xlsx->write(row+2,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(row+2,col+1,value,numberFormatQhash.value(col));
                    }
                }else{
                    xlsx->write(row+2,col+1,value,formatBody);
                }
            }
            //每100行读取下事件循环
            //excel导出较慢
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(row)));
                qApp->processEvents();
            }
        }
    }
    //根据每列最大的值设置本列的宽度
    for(int i=0;i<columnCount;i++){
        xlsx->setColumnWidth(i+1,i+1,colWidthArray[i]+0.0);
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

/**
 * @brief MainWindow::save2XlsxFinished 写入xlsx文件完毕的通知
 */
void MainWindow::save2XlsxFinished(){
    dataBlocked=false;
    statusBar_disPlayMessage(tr("数据成功导出到:%1").arg(xlsxSaveName));
    //如果需要打开文件
    if(openXlsx){
        statusBar_disPlayMessage(tr("使用系统默认程序打开文件:%1").arg(xlsxSaveName));
        QString  m_szHelpDoc = QString("file:///") + xlsxSaveName;
        bool is_open = QDesktopServices::openUrl(QUrl(m_szHelpDoc, QUrl::TolerantMode));
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
    if(currentOpenFileType==-1){
        return;
    }
    if(currentOpenFileType==0){
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
        if(currentOpenFileType==1){
            if(ofdFileContentQByteArrayList.count()<1){
                statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
                return;
            }
            else if(lineNumber>ofdFileContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==2){
            if(csvFileContentQByteArrayList.count()<1){
                statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
                return;
            }
            else if(lineNumber>csvFileContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==3){
            if(fixedContenQByteArrayList.count()<1){
                statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
                return;
            }
            else if(lineNumber>fixedContenQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
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
            pageJump(targetPage,1);
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
        initFile(currentOpenFilePath);
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
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
    }
    if(currentOpenFileType==1){
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
            saveOFDFile(currentOpenFilePath);
        }
        else{
            statusBar_disPlayMessage("文件没有被修改,无需保存");
        }
    }
    if(currentOpenFileType==2||currentOpenFileType==3){
        statusBar_disPlayMessage("暂不支持保存此类文件");
    }
}

/**
 * @brief MainWindow::saveOFDFile OFD文件保存功能
 * @param filepath
 */
void MainWindow::saveOFDFile(QString filepath)
{
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    dataBlocked=true;
    dataBlockedMessage="正在保存写入文件,请稍后再操作...";
    //检查文件是否存在,存在则覆盖
    if(Utils::isFileExist(filepath)){
        QFile oldfile(filepath);
        bool r=oldfile.remove();
        if(!r){
            statusBar_disPlayMessage("覆盖文件失败,请重试或检查文件是否被其他程序占用");
            dataBlocked=false;
            return;
        }
    }
    //开始执行文件写入
    QFile newfile(filepath);
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (newfile.open(QFile::WriteOnly | QIODevice::Truncate)) {
        //开始准备待写入的数据
        QString sb;
        //写入文件头///////////////////////////////////////////////////////////////////
        for(int i=0;i<ofdFileHeaderQStringList.length();i++){
            sb.append(ofdFileHeaderQStringList.at(i)).append("\r\n");
        }
        newfile.write(codecOFD->fromUnicode(sb));
        sb.clear();
        //写入文件体//////////////////////////////////////////////////////////////////
        int row=0;
        while(row<ofdFileContentQByteArrayList.length()){
            //按行写入数据
            sb.append(codecOFD->toUnicode(qUncompress(ofdFileContentQByteArrayList.at(row)))).append("\r\n");
            //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
            //仅1000行或者到最后一行时进行写入
            if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                newfile.write(codecOFD->fromUnicode(sb));
                sb.clear();
                statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                qApp->processEvents();
            }
            row++;
        }
        //写入文件结束标志位//////////////////////////////////////////////////////////////////
        newfile.write(codecOFD->fromUnicode("OFDCFEND\r\n"));
        newfile.flush();
        newfile.close();
        fileChanged=false;
        statusBar_disPlayMessage(tr("文件保存完毕,保存在%1").arg(filepath));
        this->setWindowTitle(appName+"-"+currentFileName);
        //当执行文件保存后更新上下文文件路径
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
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
        return;
    }
    if(currentOpenFileType==1){
        if(ofdFileHeaderQStringList.size()<1){
            statusBar_disPlayMessage("未打开有效的OFD文件,或者文件记录数为0,无法使用另存功能,请尝试打开一个有效非空的文件...");
            return;
        }
        //文件过滤器,用于追踪选择的保存文件类别
        QString selectedFilter=Q_NULLPTR;
        //弹出保存框
        QString fileNameSave = QFileDialog::getSaveFileName(this,("另存为"),currentOpenFilePath,tr("OFD文本文件(*.TXT);;自定义扩展名(*.*)"),&selectedFilter);
        if(!fileNameSave.isEmpty()){
            //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
            if(selectedFilter=="OFD文本文件(*.TXT)"&&(!fileNameSave.endsWith(".TXT"))){
                fileNameSave.append(".TXT");
            }
            //执行文件另存
            saveOFDFile(fileNameSave);
        }
    }
    if(currentOpenFileType==2||currentOpenFileType==3){
        statusBar_disPlayMessage("暂不支持另存此类文件");
    }
}

/**
 * @brief MainWindow::on_actionaboutAuthor_triggered 关于作者
 */
void MainWindow::on_actionaboutAuthor_triggered()
{
    DialogAboutAuthor * dialog = new DialogAboutAuthor(this);
    dialog->setModal(false);
    dialog->show();
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
    if(currentOpenFileType==1||currentOpenFileType==2||currentOpenFileType==3){
        showRowDetails();
    }
}

/**
 * @brief MainWindow::randomTips 随机在状态栏给出一个温馨提醒
 */
void MainWindow::randomTips(){
    srand((unsigned)time(nullptr));
    int index =rand()%tips.count();
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
    if(currentOpenFileType==-1){
        return;
    }
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //数据类型加入点
    if(currentOpenFileType==1){
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==2){
        if(csvFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==3){
        if(fixedContenQByteArrayList.count()<1){
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
        if(currentOpenFileType==1){
            int headerCount=ofdFileHeaderQStringList.count();
            if(lineNumber<=headerCount){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是文件头:[%1]").arg(ofdFileHeaderQStringList.at(lineNumber-1)));
                return;
            }
            //重新计算数据行位置
            lineNumber-=headerCount;
            if(lineNumber==ofdFileContentQByteArrayList.count()+1){
                statusBar_disPlayMessage(tr("你输入的行号所在行应该是OFD文件结束标记[OFDCFEND]"));
                return;
            }
            else if(lineNumber>ofdFileContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==2){
            int headerCount=csvFileHeaderQStringList.count();
            if(lineNumber==csv.getTitlerowindex()){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是标题行"));
                return;
            }
            //重新计算数据行位置
            lineNumber-=headerCount;
            if(lineNumber>csvFileContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                return;
            }
            else{
                targetIndex=lineNumber-1;
            }
        }
        if(currentOpenFileType==3){
            int headerCount=fixedFileHeaderQStringList.count();
            int contentCount=fixedContenQByteArrayList.count();
            int footerCount=fixedFooterQStringList.count();
            if(lineNumber<=headerCount){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是文件头:[%1]").arg(fixedFileHeaderQStringList.at(lineNumber-1)));
                return;
            }
            //重新计算数据行位置
            lineNumber-=headerCount;
            //文件数据
            if(lineNumber>0&&lineNumber<=contentCount){
                targetIndex=lineNumber-1;
            }
            else if(lineNumber>contentCount&&lineNumber<=(contentCount+footerCount)){
                statusBar_disPlayMessage(tr("你输入的行号所在行在源文件中记录的是文件尾:[%1]").arg(fixedFooterQStringList.at(lineNumber-contentCount-1)));
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
            pageJump(targetPage,1);
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
    int addHight=ui->frameInfo->height();
    bool isHidden=ui->frameInfo->isHidden();
    if(!isHidden){
        ui->frameInfo->setHidden(true);
        ui->viewMode->setText("标准视图");
        //切换到精简模式后，界面可视区域变大，要重新显示范围
        if(tableHeight!=ptr_table->height()&&!isUpdateData){
            //获取当前table的高度
            int higth=ptr_table->size().height()+addHight;
            //窗口变大不会影响起始行
            tableRowEnd=tableRowBegin+(higth/rowHight);
            //OFD文件
            if(currentOpenFileType==1){
                display_OFDTable();
            }
            //CSV文件
            else if(currentOpenFileType==2){
                display_CSVTable();
            }
            else if(currentOpenFileType==3){
                display_FIXEDTable();
            }
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
        if(currentOpenFileType==0){
            statusBar_display_rowsAndCol(dataRowCurrent+5,0,-1);
        }
        //OFD数据文件
        else if(currentOpenFileType==1){
            int rowInFile=ofdFileHeaderQStringList.count()+dataRowCurrent+1;
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
                    statusBar_disPlayMessage(ofd.getFieldList().at(tableColCurrent).getFieldDescribe().append("/").append(ofd.getFieldList().at(tableColCurrent).getFieldName()).append("|").append(ofd.getFieldList().at(tableColCurrent).getFieldType()).append("|").append(text).append(dic.isEmpty()?"":("|"+dic)));
                }
            }
            else{
                statusBar_disPlayMessage(ofd.getFieldList().at(tableColCurrent).getFieldDescribe().append("/").append(ofd.getFieldList().at(tableColCurrent).getFieldName()).append("|").append(ofd.getFieldList().at(tableColCurrent).getFieldType()));
            }
        }
        //CSV文件
        else if(currentOpenFileType==2){
            int rowInFile=csvFileHeaderQStringList.count()+dataRowCurrent+1;
            int colInFile=tableColCurrent+1;
            statusBar_display_rowsAndCol(rowInFile,colInFile,0);
            if(ptr_table->item(tableRowCurrent,tableColCurrent)!=nullptr){
                QString text=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
                QString dic=commonDictionary.value("CSV"+csv.getFileIni()).getDictionary(csv.getFieldList().at(tableColCurrent).getFieldName(),text);
                if(!text.isEmpty()){
                    statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)));
                }
            }
        }
        else if(currentOpenFileType==3){
            int rowInFile=fixedFileHeaderQStringList.count()+dataRowCurrent+1;
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
            //统计成功标记
            bool sucessFlag=true;
            //统计失败原因
            QString faultMessage;
            //跳过的空
            int skipEmpty=0;
            //跳过的非数值
            int skipNotNumber=0;
            //成功统计的数量
            int numberCount=0;
            int editRowinFileContent=0;
            //判断当前打开的文件类型
            if(currentOpenFileType==1){
                if(ofd.getFieldList().at(itemsRange.at(0).leftColumn()).getFieldType()=="N"){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QString vvv=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,editRowinFileContent,editCol);
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
                                }
                            }
                        }
                    }
                    statusBar_disPlayMessage("[所有:"+QString::number(selectedAllItemCount)+"  计数:"+QString::number(numberCount)+"  平均值:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum/selectedAllItemCount,'f',ofd.getFieldList().at(editCol).getDecLength()))+"  求和:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum,'f',ofd.getFieldList().at(editCol).getDecLength()))+"]  [空值:"+QString::number(skipEmpty)+"  非法数值:"+QString::number(skipNotNumber)+"]");
                }
                //非数值型字段，仅仅统计计数
                else{
                    statusBar_disPlayMessage("计数:"+QString::number(selectedAllItemCount));
                }
            }
            else  if(currentOpenFileType==2){
                if(fieldIsNumberOrNot.contains(itemsRange.at(0).leftColumn())&&fieldIsNumberOrNot.value(itemsRange.at(0).leftColumn()).getIsNumber()){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QStringList rowList=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,editRowinFileContent,csv.getEcoding());
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
                                }
                            }
                        }
                    }
                    statusBar_disPlayMessage("[所有:"+QString::number(selectedAllItemCount)+"  计数:"+QString::number(numberCount)+"  平均值:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum/selectedAllItemCount,'f',fieldIsNumberOrNot.value(itemsRange.at(0).leftColumn()).getDecimalLength()))+"  求和:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum,'f',fieldIsNumberOrNot.value(itemsRange.at(0).leftColumn()).getDecimalLength()))+"]  [空值:"+QString::number(skipEmpty)+"  非法数值:"+QString::number(skipNotNumber)+"]");
                }
                //非数值型字段，仅仅统计计数
                else{
                    statusBar_disPlayMessage("计数:"+QString::number(selectedAllItemCount));
                }
            }
            if(currentOpenFileType==3){
                if(fixed.getFieldList().at(itemsRange.at(0).leftColumn()).getFieldType()=="N"){
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            editRowinFileContent=(currentPage-1)*pageRowSize+editRow;
                            //开始取值
                            QString vvv=Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,editRowinFileContent,editCol,fixed.getEcoding());
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
                                }
                            }
                        }
                    }
                    statusBar_disPlayMessage("[所有:"+QString::number(selectedAllItemCount)+"  计数:"+QString::number(numberCount)+"  平均值:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum/selectedAllItemCount,'f',fixed.getFieldList().at(editCol).getDecLength()))+"  求和:"+Utils::CovertDoubleQStringWithThousandSplit(QString::number(selectSum,'f',fixed.getFieldList().at(editCol).getDecLength()))+"]  [空值:"+QString::number(skipEmpty)+"  非法数值:"+QString::number(skipNotNumber)+"]");
                }
                //非数值型字段，仅仅统计计数
                else{
                    statusBar_disPlayMessage("计数:"+QString::number(selectedAllItemCount));
                }
            }
        }
        else{
            statusBar_disPlayMessage("计数:"+QString::number(selectedAllItemCount));
        }
    }
}

/**
  高级打开文件，将弹出一个选择文件类型的框框
 * @brief MainWindow::on_actionfileOpenAdv_triggered
 */
void MainWindow::on_actionfileOpenAdv_triggered()
{
    DialogOpenFileAdv * dialog = new DialogOpenFileAdv(this);
    dialog->setWindowTitle(QString("自定义配置打开文件"));
    dialog->setModal(true);
    dialog->show();
    //模态的使用完毕删除
    delete dialog;
    dialog=nullptr;
}


/**
 * @brief MainWindow::on_actionpreference_triggered 设置
 */
//需要改造--需要新增设置项每页条目
void MainWindow::on_actionpreference_triggered()
{
    //如果新增了配置项-记得修改配置读取--配置修改弹窗发起--弹窗类内修改--配置修改结果回写四处代码
    //获取当前的配置值，用于传递给设置界面
    QMap <QString,QString>par;
    par.insert("compresslevel",QString::number(dataCompressLevel));
    par.insert("defaultviewmode",defaultViewMode);
    par.insert("defaultnewfilemode",defaultNewFileMode);
    par.insert("defaultpagesizetype",defaultPageSizeType);
    DialogPreference * dialog = new DialogPreference(par,this);
    dialog->setModal(true);
    dialog->exec();
    //获取结果
    if(dialog->getSaveFlag()){
        bool changeFlag=false;
        //设置项压缩等级
        if(dialog->getDataCompressLevel()!=dataCompressLevel){
            changeFlag=true;
            this->dataCompressLevel=dialog->getDataCompressLevel();
        }
        if(dialog->getDefaultViewMode()!=defaultViewMode){
            changeFlag=true;
            this->defaultViewMode=dialog->getDefaultViewMode();
            if(defaultViewMode=="0"){
                if(ui->frameInfo->isHidden()){
                    ui->frameInfo->setHidden(false);
                    ui->viewMode->setText("精简视图");
                }
            }
            if(defaultViewMode=="1"){
                if(!ui->frameInfo->isHidden()){
                    ui->frameInfo->setHidden(true);
                    ui->viewMode->setText("标准视图");
                }
            }
        }
        if(dialog->getDefaultNewFileMode()!=defaultNewFileMode){
            changeFlag=true;
            this->defaultNewFileMode=dialog->getDefaultNewFileMode();
        }
        //是否修改了分页每页页数
        if(dialog->getDefaultPageSizeType()!=defaultPageSizeType){
            changeFlag=true;
            this->defaultPageSizeType=dialog->getDefaultPageSizeType();
            //每页行数
            if(this->defaultPageSizeType=="0"){
                ui->labelPageSize->setText("(10万行/页)");
                this->pageRowSize=100000;
            }
            else if(this->defaultPageSizeType=="1"){
                ui->labelPageSize->setText("(20万行/页)");
                this->pageRowSize=200000;
            }
            else if(this->defaultPageSizeType=="2"){
                ui->labelPageSize->setText("(50万行/页)");
                this->pageRowSize=500000;
            }
            else if(this->defaultPageSizeType=="3"){
                ui->labelPageSize->setText("(100万行/页)");
                this->pageRowSize=1000000;
            }
            else{
                ui->labelPageSize->setText("(10万行/页)");
                this->pageRowSize=100000;
            }
            //如果每页条数变化了，并且当前打开了文件，需要跳到第一页-并且判断是否需要打开或者隐藏分页控件
            int rowCount=0;
            if(currentOpenFileType==1){
                rowCount=ofdFileContentQByteArrayList.count();
            }
            else if(currentOpenFileType==2){
                rowCount=csvFileContentQByteArrayList.count();
            }
            else if(currentOpenFileType==3){
                rowCount=fixedContenQByteArrayList.count();
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
            if(currentOpenFileType!=-1){
                pageJump(currentPage,0);
            }
        }
        //如果配置发生了改变,则写入配置
        if(changeFlag){
            QString Settingini=Utils::getConfigPath()+"Setting.ini";
            QSettings loadedSettingInfoIni(Settingini,QSettings::IniFormat,nullptr);
            loadedSettingInfoIni.setIniCodec("UTF-8");
            loadedSettingInfoIni.beginGroup("setting");
            loadedSettingInfoIni.setValue("compresslevel",dataCompressLevel);
            loadedSettingInfoIni.setValue("defaultviewmode",defaultViewMode);
            loadedSettingInfoIni.setValue("defaultnewfilemode",defaultNewFileMode);
            loadedSettingInfoIni.setValue("defaultpagesizetype",defaultPageSizeType);
            loadedSettingInfoIni.endGroup();
            loadedSettingInfoIni.sync();
        }
    }
    //模态弹窗删除
    delete dialog;
    dialog=nullptr;
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
    QProcess *Process = new QProcess(nullptr);
    Process->startDetached(program, arguments);
}

/**
 * @brief MainWindow::ignoreFileChangeAndOpenNewFile 决定是否忽略当前修改未保存并打开新的文件，如果允许打开或者文件压根没修改，直接返回true
 * @return
 */
//无需分页改造
bool MainWindow:: ignoreFileChangeAndOpenNewFile(){
    //判断文件是否被修改
    if(fileChanged){
        DialogMyTip * dialog = new DialogMyTip("当前修改的文件还未保存，要放弃保存并读取新的文件么？",this);
        dialog->setWindowTitle("警告！");
        dialog->setModal(true);
        dialog->exec();
        if(!dialog->getBoolFlag()){
            delete dialog;
            dialog=nullptr;
            statusBar_disPlayMessage("放弃读取新文件,如果需要保存当前文件，可以使用Ctrl+S保存...");
            return false;
        }
        else{
            delete dialog;
            dialog=nullptr;
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
    if(fileChanged){
        DialogMyTip * dialog = new DialogMyTip("当前修改的文件还未保存,要放弃保存并退出工具么？",this);
        dialog->setWindowTitle("警告！");
        dialog->setModal(true);
        dialog->exec();
        if(!dialog->getBoolFlag()){
            delete dialog;
            dialog=nullptr;
            statusBar_disPlayMessage("放弃退出工具,如果需要保存当前文件，可以使用Ctrl+S保存...");
            event->ignore();
        }
        else{
            delete dialog;
            dialog=nullptr;
            event->accept();
        }
    }
    //文件未修改--直接允许关闭程序
    else{
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
    if(currentOpenFileType==-1){
        return;
    }
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //数据类型插入点
    if(currentOpenFileType==1){
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
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
    if(currentOpenFileType==2){
        if(csvFileContentQByteArrayList.size()<1){
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
    if(currentOpenFileType==3){
        if(fixedContenQByteArrayList.size()<1){
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
    if(currentPage==1){
        statusBar_disPlayMessage("当前已在第一页...");
    }
    else{
        currentPage=1;
        pageJump(currentPage,0);
    }
}

/**
 * @brief MainWindow::on_pushButtonPagePrevious_clicked前往上一页
 */
void MainWindow::on_pushButtonPagePrevious_clicked()
{
    if(currentPage>1){
        currentPage--;
        pageJump(currentPage,0);
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
    statusBar_disPlayMessage("");
    if(currentPage<pageCount){
        currentPage++;
        pageJump(currentPage,0);
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
    if(currentPage==pageCount){
        statusBar_disPlayMessage("当前已在最后一页...");
    }
    else{
        currentPage=pageCount;
        pageJump(currentPage,0);
    }
}

/**
 * @brief MainWindow::on_pushButtonGo_clicked前往某一页
 */
void MainWindow::on_pushButtonGo_clicked()
{
    statusBar_disPlayMessage("");
    bool ok=false;
    int pageNum=ui->pageText->text().toInt(&ok,10);
    if(ok){
        if(pageNum<1){
            statusBar_disPlayMessage("页码不能小于0...");
        }
        else if(pageNum>pageCount){
            statusBar_disPlayMessage(QString("当前打开的文件一共有%1页,输入的页码超过了最大值...").arg(pageCount));
        }
        else{
            currentPage=pageNum;
            pageJump(currentPage,0);
        }
    }
    else{
        statusBar_disPlayMessage("请在页码输入框输入有效的页码数值后跳转...");
    }
}

/**
 * @brief MainWindow::pageJump 跳页函数
 * @param page
 * @param scrollIndex 滚动位置,-1滚动到页尾部,0滚动到页首
 */

void MainWindow:: pageJump(int page,int scrollIndex){
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
        if(currentOpenFileType==1){
            currentPageRowSize=ofdFileContentQByteArrayList.count()-(pageRowSize*(pageCount-1));
        }
        else if(currentOpenFileType==2){
            currentPageRowSize=csvFileContentQByteArrayList.count()-(pageRowSize*(pageCount-1));
        }
        else if(currentOpenFileType==3){
            currentPageRowSize=fixedContenQByteArrayList.count()-(pageRowSize*(pageCount-1));
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
    statusBar_disPlayMessage(QString("第%1页,本页记录数%2行").arg(page).arg(currentPageRowSize));
    //加载本页数据
    //数据类型插入点
    if(currentOpenFileType==1){
        display_OFDTable();
    }
    else if(currentOpenFileType==2){
        display_CSVTable();
    }
    else if(currentOpenFileType==3){
        display_FIXEDTable();
    }
    //跳转位置
    if(scrollIndex==-1){
        ptr_table->scrollToBottom();
        //强制触发下刷新，避免显示数据不完整
        acceptVScrollValueChanged(0);
    }
    else if(scrollIndex==0){
        ptr_table->scrollToTop();
        //强制触发下刷新，避免显示数据不完整
        acceptVScrollValueChanged(0);
    }
}

/**
 * @brief MainWindow::on_actiondifftools_triggered打开文件对比工具
 */
void MainWindow::on_actiondifftools_triggered()
{
    QString winmergepath=QApplication::applicationDirPath()+"/plugin/WinMerge/WinMergeU.exe";
    QStringList pars;
    if(Utils::isFileExist(winmergepath)){
        QProcess process;
        //Qt 5.6在xp系统下有个bug，调用不传参数的startDetached函数时，有可能无法启动程序,所以我们设定一个空参数调用带参数的函数
        process.startDetached(winmergepath,pars);
    }else{
        statusBar_disPlayMessage("找不到WinMerge插件,请安装到plugin目录下！");
    }
}

void MainWindow::on_actionfileedit_triggered()
{
    QString notepad2path=QApplication::applicationDirPath()+"/plugin/Notepad2/Notepad2.exe";
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(Utils::isFileExist(notepad2path)){
        QProcess process;
        process.startDetached(notepad2path,QStringList(currentOpenFilePath));
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
    dialog->show();
}

void MainWindow::on_actioncopy_triggered()
{
    copyToClipboard();
}

void MainWindow::on_actionedit_triggered()
{
    if(currentOpenFileType==1){
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int rangeCount=itemsRange.count();
        int selectedRow=0;
        bool allSelectIsOneColumn=true;
        if(rangeCount>=1){
            selectedRow=itemsRange.at(0).bottomRow()-itemsRange.at(0).topRow()+1;
            //第一个选择范围就跨列
            if(itemsRange.at(0).leftColumn()!=itemsRange.at(0).rightColumn()){
                statusBar_disPlayMessage("不能同时编辑不同的列!!!");
                return;
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
                    showModifyOFDCell();
                }
                //单行多列
                else{
                    statusBar_disPlayMessage("如果要编辑整行数据，请使用编辑整行数据快捷键!!!");
                }
            }
            //多行
            else {
                //多行单列
                if(allSelectIsOneColumn){
                    showModifyOFDCellBatch();
                }
                //多行多列
                else{
                    statusBar_disPlayMessage("不能同时编辑多行不同的列!!!");
                }
            }
        }
    }
}

void MainWindow::on_actionedit2_triggered()
{
    if(currentOpenFileType==1){
        QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
        int selectRowNum=0;
        int rangeCount=itemsRange.count();
        bool allSelectIsOneRow=true;
        if(rangeCount>=1){
            //第一个选择范围就跨行
            if(itemsRange.at(0).topRow()!=itemsRange.at(0).bottomRow()){
                statusBar_disPlayMessage("不能同时编辑多行整行数据!!!");
                return;
            }
            //待编辑的行
            selectRowNum=itemsRange.at(0).bottomRow();
            //如果存在第二个选择区间
            //循环从第第二个遍历选择的范围区间--所有选择区间都是同一行才可以
            for(int i=1;i<rangeCount;i++){
                if(itemsRange.at(i).topRow()==selectRowNum&&itemsRange.at(i).bottomRow()==selectRowNum){
                    continue;
                }else{
                    allSelectIsOneRow=false;
                    break;
                }
            }
            //单行
            if(allSelectIsOneRow){
                showMoaifyOFDRow();
            }
            //多行
            else {
                statusBar_disPlayMessage("不能同时编辑多行整行数据!!!");
            }
        }
    }
}

void MainWindow::on_actionopeninexcel_triggered()
{
    int row=0;
    if(currentOpenFileType==1){
        row=ofdFileContentQByteArrayList.count();
    }
    else if(currentOpenFileType==2){
        row=csvFileContentQByteArrayList.count();
    }
    else if(currentOpenFileType==3){
        row=fixedContenQByteArrayList.count();
    }
    else if(currentOpenFileType==0){
        statusBar_disPlayMessage("不支持在excel中打开OFD索引文件...");
        return;
    }
    else{
        return;
    }
    //弹窗提示
    DialogMyTip * dialog2 = new DialogMyTip("确定导出当前文件为xlsx并在Excel中打开么?\r\n使用此功能将在文件原始目录生成一个同名xlsx文件\r\n在Excel中你可以进行更为细致的数据统计分析",this);
    dialog2->setWindowTitle("提示！");
    dialog2->setModal(true);
    dialog2->exec();
    if(dialog2->getBoolFlag()){
        //模态的使用完毕删除
        delete dialog2;
        dialog2=nullptr;
        //限制
        if(row>maxExcelRow){
            statusBar_disPlayMessage("记录数大于"+QString::number(maxExcelRow)+"行,无法使用导出到excel,(如有需求导出到excel请联系"+QByteArray::fromBase64(AUTHOR_EMAIL)+")");
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
            save2Xlsx(exppath);
        }
    }
    else{
        statusBar_disPlayMessage("放弃在Excel中打开当前接口文件...");
    }
}
