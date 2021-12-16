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
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:13px;font-family:PingFangSC-Regular,sans-serif;");
    ui->tableWidget->setStyleSheet("font-size:13px;font-family:PingFangSC-Light,sans-serif;");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
#endif
    /*临时隐藏未开发完毕的功能***************************************/

    /*插件功能*****************************************************/
    //仅Windows系统支持文件比对和在文本编辑器中打开
#ifdef Q_OS_LINUX
    ui->actiondifftools->setVisible(false);
    ui->actionfileedit->setVisible(false);
#endif
#ifdef Q_OS_MAC
    ui->actiondifftools->setVisible(false);
    ui->actionfileedit->setVisible(false);
#endif
    /**************************************************************/

    /*默认隐藏***************************************************/
    //默认隐藏dbf工具箱
    ui->menudbftools->menuAction()->setVisible(false);
    /**************************************************************/
    //搜索框提示
    ui->lineTextText->setToolTip("输入你要搜索的内容,数值类搜索请不要输入千位分隔符~~");
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
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboardWithoutTitle()));
    action_ShowCopyColumWithTitle = new QAction(tr("复制(带字段标题)"),this);
    connect(action_ShowCopyColumWithTitle, SIGNAL(triggered()), this, SLOT(copyToClipboardWithTitle()));
    action_Magnify = new QAction(tr("放大镜"),this);
    connect(action_Magnify, SIGNAL(triggered()), this, SLOT(showMagnify()));
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
    action_ShowCharacter = new QAction(tr("十六进制和二进制编码透视(乱码分析)"),this);
    connect(action_ShowCharacter, SIGNAL(triggered()), this, SLOT(showCharacter()));
    action_CsvForceNumber= new QAction(tr("对此列调整数据格式"),this);
    connect(action_CsvForceNumber, SIGNAL(triggered()), this, SLOT(forceNumber()));
    //qrcode
    action_ShareUseQrCode = new QAction(tr("使用二维码分享此单元格"),this);
    connect(action_ShareUseQrCode, SIGNAL(triggered()), this, SLOT(showQrcode()));

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
    tips.append("小额捐赠给予支持,FFReader将获得更好的发展,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("FFReader是一个免费的工具软件,如果程序帮助到了你,你可以选择小额捐赠给予支持,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("可以使用本程序新建OFD文件,以及初始化自己的新建模板...");
    tips.append("导出功能可以分页导出或者导出全部数据,自由选择导出范围,是否使用UTF-8编码...");
    tips.append("在程序主窗口或者查看行详细信息界面,使用Ctrl+Alt+R(command+option+R)可以进行快速截图保存...");
    tips.append("固定分隔符文件可以在任意单元格右键对此列手工设置数据格式,调整为数值或者自定义小数长度哟,方便进行数值统计...");
    tips.append("在设置-首选项里设置默认精简视图,即可默认获得最大化的数据展示空间...");
    tips.append("本程序支持解析证券各类DBF文件,直接解析或者配置解析均可,使用配置获得更好的解析体验...");
    tips.append("需要编辑手工创造OFD数据文件？尝试新建文件后导出到Excel编辑，编辑后可以立即导入本程序生成OFD数据文件...");
#ifdef Q_OS_WIN32
    tips.append("同时拖放两个文件到程序主窗口,将使用文件比对插件自动比对两个文件的差异...");
    tips.append("如果你要查看接口文件的原始数据,不妨在附加工具菜单下点击\"在文本编辑器中打开当前文件\"...");
    tips.append("想要比对同一个文件不同版本的差异?试试导出到csv固定分隔符文件后同时把两个文件拖向FFReader,它会问你是否要比对差异...");
    tips.append("禁用压缩功能,可以极大的提高文件加载速度,如果你的电脑内存足够,建议禁用压缩功能(打开2GB的文件大概需要2.5GB内存)...");
    tips.append("如果没有特殊情况,请使用64位FFReader,32位版本的FFReader难以打开超大的文件...");
#endif
    specialCharacter.insert("00","NUL (null):空字符");
    specialCharacter.insert("01","SOH (start of headling):标题开始");
    specialCharacter.insert("02","STX (start of text):正文开始");
    specialCharacter.insert("03","ETX (end of text):正文结束");
    specialCharacter.insert("04","EOT (end of transmission):传输结束");
    specialCharacter.insert("05","ENQ (enquiry):请求");
    specialCharacter.insert("06","ACK (acknowledge):收到通知");
    specialCharacter.insert("07","BEL (bell):响铃");
    specialCharacter.insert("08","BS (backspace):退格");
    specialCharacter.insert("09","HT (horizontal tab):水平制表符");
    specialCharacter.insert("0A","LF (NL line feed new line):换行键");
    specialCharacter.insert("0B","VT (vertical tab):垂直制表符");
    specialCharacter.insert("0C","FF (NP form feed new page):换页键");
    specialCharacter.insert("0D","CR (carriage return):回车键");
    specialCharacter.insert("0E","SO (shift out):不用切换");
    specialCharacter.insert("0F","SI (shift in):启用切换");
    specialCharacter.insert("10","DLE (data link escape):数据链路转义");
    specialCharacter.insert("11","DC1 (device control 1):设备控制1");
    specialCharacter.insert("12","DC2 (device control 2):设备控制2");
    specialCharacter.insert("13","DC3 (device control 3):设备控制3");
    specialCharacter.insert("14","DC4 (device control 4):设备控制4");
    specialCharacter.insert("15","NAK (negative acknowledge):拒绝接收");
    specialCharacter.insert("16","SYN (synchronous idle):同步空闲");
    specialCharacter.insert("17","ETB (end of trans block):传输块结束");
    specialCharacter.insert("18","CAN (cancel):取消");
    specialCharacter.insert("19","EM (end of medium):介质中断");
    specialCharacter.insert("1A","SUB (substitute):替补");
    specialCharacter.insert("1B","ESC (escape):溢出");
    specialCharacter.insert("1C","FS (file separator):文件分割符");
    specialCharacter.insert("1D","GS (group separator):分组符");
    specialCharacter.insert("1E","RS (record separator):记录分离符");
    specialCharacter.insert("1F","US (unit separator):单元分隔符");
    specialCharacter.insert("20","SP (space):空格");
    specialCharacter.insert("7F","DEL (delete):删除");

    //哪些数据类型当作数字处理//////
    dbfIsNumberFieldType.append(QDbf::QDbfField::FloatingPoint);
    dbfIsNumberFieldType.append(QDbf::QDbfField::Number);
    dbfIsNumberFieldType.append(QDbf::QDbfField::Integer);
    dbfIsNumberFieldType.append(QDbf::QDbfField::Currency);

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
    else if(this->defaultPageSizeType=="4"){
        ui->labelPageSize->setText("(200万行/页)");
        this->pageRowSize=2000000;
    }
    else{
        ui->labelPageSize->setText("(10万行/页)");
        this->pageRowSize=100000;
    }
    //加载web平台插件
    load_PluginList();
    //加载基金销售商和TA代码信息
    load_OFDCodeInfo();
    //加载OFD文件类别信息
    load_OFDIndexFile();
    //加载各类OFD文件的定义
    load_OFDDefinition();
    //加载OFD字典
    load_OFDDictionary();
    //加载OFD tips字典
    load_OFDTipDictionary();
    //加载各类CSV文件的定义
    load_CSVDefinition();
    //加载各类定长文件的定义
    load_FIXEDDefinition();
    //加载DBF配置
    load_DBFDefinition();
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
    watcherXlsxImportStatus_ = new QFutureWatcher<int>;
    connect(watcherXlsxImportStatus_, &QFutureWatcher<int>::finished,this, &MainWindow::importFromXlsxFinished);
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
                QProcess process;
                process.startDetached(winmergepath,pars);
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
    if(defaultNewFileMode=="1"){
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
            QProcess *Process = new QProcess(nullptr);
            Process->startDetached(program, arguments);
            return;
        }
    }
    //只要是新文件直接新实例打开
    else if (defaultNewFileMode=="2"&&!currentOpenFilePath.isEmpty()&&currentOpenFileType!=openFileType::NotFileOrErr){
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
 * @brief MainWindow::statusBar_display_rowsCount状态栏显示总行数
 * @param rowsCount
 */
void MainWindow::statusBar_display_rowsCount(int rowsCount){
    int count=(rowsCount + pageRowSize - 1) / pageRowSize;
    statusLabel_ptr_showCount->setText(tr("记录数:%1行,共计%2页").arg(QString::number(rowsCount, 10)).arg(count));
}

void MainWindow::statusBar_display_rowsCount(int rowsCount, QString charset){
    int count=(rowsCount + pageRowSize - 1) / pageRowSize;
    statusLabel_ptr_showCount->setText(tr("记录数:%1行,共计%2页-%3").arg(QString::number(rowsCount, 10)).arg(count).arg(charset));
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
                    if(value!="0"&&value!="1"&&value!="2"&&value!="3"&&value!="4"){
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

void MainWindow::load_OFDTipDictionary(){
    QString TipDictionaryInipath=Utils::getConfigPath()+"OFD_TipDictionary.ini";
    if(Utils::isFileExist(TipDictionaryInipath)){
        //加载ini文件
        QSettings loadedDictionary(TipDictionaryInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        loadedDictionary.setIniCodec("UTF-8");
        //获取所有fixed文件的配置信息，循环存储
        QStringList dictionaryGroup=loadedDictionary.childGroups();
        //查不到任何段落
        if(dictionaryGroup.count()<1){
            return;
        }
        //循环各个字典组
        //tip字典和字段枚举字典一样分组
        for(int i=0;i<dictionaryGroup.count();i++){
            loadedDictionary.beginGroup(dictionaryGroup.at(i));
            QStringList allkey=loadedDictionary.allKeys();
            QMap<QString,QString> tips;
            if(allkey.count()>0){
                for (int j=0;j<allkey.count();j++){
                    //如果值不空,则认为是一条有效的tips
                    if(!loadedDictionary.value(allkey.at(j)).toString().isEmpty()){
                        tips.insert(allkey.at(j),loadedDictionary.value(allkey.at(j)).toString());
                    }
                }
                //OFD的tips存储结构为OFD0001这样的OFD开头的文件类别
                fieldTips.insert("OFD"+dictionaryGroup.at(i),tips);
            }
            loadedDictionary.endGroup();
        }
    }
    else{
        statusBar_disPlayMessage(Utils::getConfigPath()+tr("OFD_TipDictionary.ini配置丢失"));
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
            if(fileName=="OFD_CodeInfo.ini"||fileName=="OFD_Dictionary.ini"||fileName=="OFD_IndexFile.ini"||fileName=="OFD_TipDictionary.ini"){
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
                            ofd.setConfigSegment("["+interfaceList.at(i)+"]");
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
                        //10是否清除双引号
                        QString clearQuotesStr=loadedCsvInfoIni.value(csvType+"/clearquotes").toString();
                        if(clearQuotesStr=="1") {
                            fileDef.setClearQuotes(true);
                        }
                        else{
                            fileDef.setClearQuotes(false);
                        }
                        //11是否进行trim
                        QString TrimStr=loadedCsvInfoIni.value(csvType+"/trim").toString();
                        if(TrimStr=="1") {
                            fileDef.setTrim(true);
                        }
                        else{
                            fileDef.setTrim(false);
                        }
                        //////////////////////////////////////
                        //最后字段总数和字段内容
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
                        //字段Tips加载
                        else if(csvType=="TipDictionary"){
                            //开始循环读取字典
                            loadedCsvInfoIni.beginGroup("TipDictionary");
                            QStringList infoList =loadedCsvInfoIni.childKeys();
                            loadedCsvInfoIni.endGroup();
                            if(infoList.count()>0){
                                QMap<QString,QString> tips;
                                for(int aaa=0;aaa<infoList.count();aaa++){
                                    //ini配置中如果key存在中文会出现错误，所以我们使用1=key:value的方式去改造
                                    QString line=loadedCsvInfoIni.value("TipDictionary/"+infoList.at(aaa)).toString();
                                    //从第一个：拆分这样做到优雅的tips中可以存在：
                                    int index=line.indexOf(':');
                                    if(index>0){
                                        tips.insert(line.mid(0,index),line.mid(index+1));
                                    }
                                    //不包含:的跳过
                                    else{
                                        continue;
                                    }
                                }
                                //以文件类别，配置文件名存入
                                fieldTips.insert("CSV"+fileName,tips);
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
                                //至少要四段配置
                                if (iniStrList.isEmpty()||iniStrList.length()<4){
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
                                fixedfield.setFieldDescribe(QString(iniStrList.at(3)));
                                //如果存在英文名赋值
                                if(iniStrList.count()>4){
                                    fixedfield.setFieldName(QString(iniStrList.at(4)));
                                }
                                //否则赋值中文名
                                else{
                                    fixedfield.setFieldName(QString(iniStrList.at(3)));
                                }
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
                    //判断是否是字典配置，字典配置是fixed配置文件里的一段特殊配置，使用Dictionary标注
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
                        //字段Tips加载
                        else if(fixedType=="TipDictionary"){
                            //开始循环读取字典
                            loadedFixedInfoIni.beginGroup("TipDictionary");
                            QStringList infoList =loadedFixedInfoIni.childKeys();
                            loadedFixedInfoIni.endGroup();
                            if(infoList.count()>0){
                                QMap<QString,QString> tips;
                                for(int aaa=0;aaa<infoList.count();aaa++){
                                    //ini配置中如果key存在中文会出现错误，所以我们使用1=key@value的方式去改造
                                    QString line=loadedFixedInfoIni.value("TipDictionary/"+infoList.at(aaa)).toString();
                                    int index=line.indexOf(':');
                                    if(index>0){
                                        tips.insert(line.mid(0,index),line.mid(index+1));
                                    }
                                    //不包含:的跳过
                                    else{
                                        continue;
                                    }
                                }
                                //以文件类别，配置文件名存入
                                fieldTips.insert("FIXED"+fileName,tips);
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
        int listcount=loadedFixedDefinitionList.count();
        if(listcount>1){
            for(int nn=0;nn<listcount-1;nn++){
                int lastsmallIndex=nn;
                int small=nn;
                for(int vv=nn;vv<listcount-1;vv++){
                    if(loadedFixedDefinitionList.at(vv+1).getFileName().count("*")<loadedFixedDefinitionList.at(lastsmallIndex).getFileName().count("*")){
                        lastsmallIndex=vv+1;
                        small=vv+1;
                    }
                }
                //如果发生了交换
                if(small!=nn){
                    FIXEDFileDefinition exchange=loadedFixedDefinitionList.at(nn);
                    loadedFixedDefinitionList.replace(nn,loadedFixedDefinitionList.at(small));
                    loadedFixedDefinitionList.replace(small,exchange);
                }
            }
        }
    }
}

void MainWindow::load_DBFDefinition(){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "DBF_*.ini";
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
                QSettings loadedDBFInfoIni(codeInipath,QSettings::IniFormat,nullptr);
                DbfFileConfig dbfconfig;
                dbfconfig.setFileIniName(fileName);
                //目前仅接收UTF-8编码的配置文件
                loadedDBFInfoIni.setIniCodec("UTF-8");
                QStringList configInfo=loadedDBFInfoIni.childGroups();
                if(configInfo.count()<1){
                    continue;
                }
                //不包含基础信息和字段信息的配置文件不要
                if(!configInfo.contains("Global")||!configInfo.contains("Field")){
                    continue;
                }
                //遍历global/////////////////////////////////////////////////////////////////
                QList<FileNameAndDescribe> list;
                loadedDBFInfoIni.beginGroup("Global");
                QStringList infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.contains("matchfilename",Qt::CaseInsensitive)){
                    QString line=loadedDBFInfoIni.value("Global/matchfilename").toString();
                    QStringList names=line.split("|");
                    if(names.count()>0){
                        for(int k=0;k<names.count();k++){
                            QStringList nameanddescribe=names.at(k).split(":");
                            if(nameanddescribe.count()==2){
                                FileNameAndDescribe a;
                                a.fileName=nameanddescribe.at(0);
                                a.fileDescribe=nameanddescribe.at(1);
                                list.append(a);
                            }
                            else{
                                continue;
                            }
                        }
                    }
                    else{
                        continue;
                    }
                }
                else{
                    continue;
                }
                infoList.clear();
                //汇聚结果
                if(list.count()>0){
                    dbfconfig.setSupportFileList(list);
                }
                else{
                    //没有识别到任何有效的文件名支持情况
                    continue;
                }
                //遍历字段配置///////////////////////////////////////////////////////////////////
                loadedDBFInfoIni.beginGroup("Field");
                infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.count()>0){
                    QMap<QString,QString> fieldDescribe;
                    for(int aaa=0;aaa<infoList.count();aaa++){
                        QString line=loadedDBFInfoIni.value("Field/"+infoList.at(aaa)).toString();
                        fieldDescribe.insert(infoList.at(aaa).toUpper(),line);
                    }
                    dbfconfig.setFieldDescribe(fieldDescribe);
                }
                infoList.clear();
                //遍历字典////////////////////////////////////////////////////////////////////////
                Dictionary dic;
                //开始循环读取字典
                loadedDBFInfoIni.beginGroup("Dictionary");
                infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.count()>0){
                    for(int aaa=0;aaa<infoList.count();aaa++){
                        QString line=loadedDBFInfoIni.value("Dictionary/"+infoList.at(aaa)).toString();
                        dic.addDictionary(infoList.at(aaa).toUpper()+"="+line);
                    }
                    commonDictionary.insert("DBF"+fileName,dic);
                }
                infoList.clear();
                //字段Tips加载/////////////////////////////////////////////////////////////////
                loadedDBFInfoIni.beginGroup("TipDictionary");
                infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.count()>0){
                    QMap<QString,QString> tips;
                    for(int aaa=0;aaa<infoList.count();aaa++){
                        QString line=loadedDBFInfoIni.value("TipDictionary/"+infoList.at(aaa)).toString();
                        tips.insert(infoList.at(aaa).toUpper(),line);
                    }
                    //以文件类别，配置文件名存入
                    fieldTips.insert("DBF"+fileName,tips);
                }
                infoList.clear();
                //加入配置
                loadedDbfDefinitionHash.insert(fileName,dbfconfig);
            }
        }
    }
}


/**
 * @brief MainWindow::initFile 文件初始化方法 无论是拖拽进来的文件还是打开菜单打开的文件还是重新打开文件，最终都要从这个函数开始初始化加载文件，这个函数会对文件类型做个初步的判断，然后交付相对应的文件类别的加载方法
 * @param filePath
 */
void MainWindow::initFile(QString filePath, bool keepdbfDisplayType, bool keepdbfTrimType){
    //重设开始时间
    time_Start = (double)clock();
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
    currentFileName=QFileInfo(currentOpenFilePath).fileName();
    this->setWindowTitle(appName+"-"+currentFileName);
    //前置拦击-常用压缩包
    if(currentFileName.endsWith(".zip",Qt::CaseInsensitive)||currentFileName.endsWith(".zipx",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".rar",Qt::CaseInsensitive)||currentFileName.endsWith(".7z",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".tar",Qt::CaseInsensitive)||currentFileName.endsWith(".gz",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".tar.gz",Qt::CaseInsensitive)||currentFileName.endsWith(".tgz",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".bz",Qt::CaseInsensitive)||currentFileName.endsWith(".bz2",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".tbz",Qt::CaseInsensitive)||currentFileName.endsWith(".tbz2",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".xz",Qt::CaseInsensitive)||currentFileName.endsWith(".txz",Qt::CaseInsensitive)){
        statusBar_disPlayMessage("压缩文件请解压后解析!");
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
            ||currentFileName.endsWith(".html",Qt::CaseInsensitive)||currentFileName.endsWith(".zip",Qt::CaseInsensitive)
            ||currentFileName.endsWith(".sql",Qt::CaseInsensitive)||currentFileName.endsWith(".pdf",Qt::CaseInsensitive)){
        statusBar_disPlayMessage("不支持解析此类文件!");
        currentOpenFileType=openFileType::NotFileOrErr;
        return;
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
                load_indexFile();
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
    //开始判断是不是csv类别的文件
    ////////////////////////////////////////////
    QStringList resultCsvType;
    QHash<QString, QString>::iterator h;
    for(h=loadedCsvFileInfo.begin(); h!=loadedCsvFileInfo.end(); ++h){
        QString Name=h.key();
        Name.replace("*","\\S*");
        QString pattern(Name.toUpper());
        QRegExp rx(pattern);
        bool match = rx.exactMatch(currentFileName.toUpper());
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
        bool match = rx.exactMatch(currentFileName.toUpper());
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
        DialogChooseFileType  dialog2(this);
        dialog2.setWindowTitle("打开的文件文件名匹配到了两种文件大类型,请确认");
        dialog2.setModal(true);
        dialog2.exec();
        //从弹窗中获取结果
        int type=dialog2.getFileType();
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
    //自动化csv文件分析
    //此部分构造一个自动化识别csv文件的方案，尝试免配置自动解析csv文件，支持带标题行和不带标题行的竖线分割，tab分割，逗号分割三种方式
    //支持自动识别的文件编码方式
    QString filecharset="UTF-8";
    QFile dataFile(currentOpenFilePath);
    //判断如果文件打开成功,则开始读取
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
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
            filecharset=m_libucd.CharsetDetect(line50ByteArray);
            qDebug()<<"自动识别出来的原始编码格式"<<filecharset;
            //不在编码白名单的编码方式暂不支持，统一使用UTF-8解析
            if(!allowCharsetList.contains(filecharset.toUpper())){
                if(filecharset.toUpper()=="WINDOWS-1252"||filecharset.toUpper()=="WINDOWS-1251"||filecharset.toUpper()=="KOI8-R"){
                    filecharset="GBK";
                }
                else{
                    filecharset="UTF-8";
                }
            }
            QTextCodec *codec=QTextCodec::codecForName(filecharset.toLocal8Bit());
            if(autoFlagList.count()>0){
                QString useflag="";
                int lastCount=1;
                int titleRow=0;
                int dataBeginRow=1;
                QString fileini="自动解析引擎";
                QString fileName="";
                QString fileDescribe="";
                QString endwithflag="0";
                //-1自动识别
                int ignorerow=-1;
                bool clearQuotesStr=true;
                int fieldCount=-1;
                QList <CsvFieldDefinition> fieldTitleList;
                QStringList fieldTitleStrList;
                for(int in=0;in<autoFlagList.count();in++){
                    QString thisflag=autoFlagList.at(in);
                    QRegExp rx("\\"+thisflag+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                    //只有一行数据
                    //如果文件只有一行数据，则对该行数据进行使用不同的分隔符拆分，拆分出来的字段数最多的则获得解析权，拆分出来的列不得低于2
                    //如果该行数据高ascii位占比超过设定值，则认为该行是仅有的一行标题行
                    if(line50Qlist.count()==1){
                        QString line = codec->toUnicode(line50Qlist.at(0)).replace("\r","").replace("\n","");
                        QStringList fieldList=line.split(rx);
                        if(fieldList.count()>lastCount){
                            lastCount=fieldList.count();
                            useflag=thisflag;
                            //优化描述展示,毕竟直接展示制表符不是很优雅
                            QString flagd="";
                            if(thisflag=="\t"){
                                flagd="制表符";
                            }
                            else {
                                flagd=thisflag;
                            }
                            fileName="基于 "+flagd+" 分割列的文件";
                            //统计中英文占比--笼统的算法-直接统计高ansii占比
                            float ct=0;
                            float all=0;
                            for(int xx=0;xx<fieldList.count();xx++){
                                int fiedleLength=fieldList.at(xx).length();
                                int in=0;
                                while (in<fiedleLength) {
                                    if(fieldList.at(xx).at(in)>=QChar('A')){
                                        ct++;
                                    }
                                    all++;
                                    in++;
                                }
                            }
                            //满足标题要求
                            if((ct/all)>titlecheck){
                                titleRow=1;
                                dataBeginRow=2;
                                fieldCount=-1;
                                fieldTitleList.clear();
                                for(int xx=0;xx<fieldList.count();xx++){
                                    CsvFieldDefinition fieldItem;
                                    QString name=fieldList.at(xx);
                                    if(name.isEmpty()){
                                        name="未定义的字段名";
                                    }
                                    fieldItem.setFieldName(name);
                                    fieldTitleStrList.append(name);
                                    fieldTitleList.append(fieldItem);
                                }
                            }
                            //不满足，则认定该行数据为仅有的一行数据
                            else{
                                titleRow=0;
                                dataBeginRow=1;
                                fieldCount=fieldList.count();
                                fieldTitleList.clear();
                                for(int xx=0;xx<fieldList.count();xx++){
                                    CsvFieldDefinition fieldItem;
                                    QString name="第"+QString::number(xx+1)+"列";
                                    fieldItem.setFieldName(name);
                                    //添加此字段信息到文件定义
                                    fieldTitleList.append(fieldItem);
                                    fieldTitleStrList.append(name);
                                    fieldTitleList.append(fieldItem);
                                }
                            }
                        }
                    }
                    //文件内容行数大于1，开始分析多行数据
                    else{
                        QList<QStringList > lineQListQStringList;
                        /////////////////step1////////////////////////////////////////
                        //对前50行数据进行分隔符拆分
                        for(int lineN=0;lineN<line50Qlist.count();lineN++){
                            QStringList line=codec->toUnicode(line50Qlist.at(lineN)).replace("\r","").replace("\n","").split(rx);
                            lineQListQStringList.append(line);
                        }
                        /////////////////step2////////////////////////////////////////
                        //开始循环分析列数
                        int previousRowFieldCount=1;
                        //疑似数据起始行（含标题）--从这一行开始往后最大的列
                        int contextBgein=-1;
                        for(int lineN=0;lineN<lineQListQStringList.count();lineN++){
                            //首次遇到大于1列的-认为该行数据就是起始行
                            if(previousRowFieldCount==1&&lineQListQStringList.at(lineN).count()>1&&lineN>contextBgein){
                                previousRowFieldCount=lineQListQStringList.at(lineN).count();
                                //如果这个分割符分出来的比上一个分隔符的多，则更新
                                if(previousRowFieldCount>lastCount){
                                    useflag=thisflag;
                                    lastCount=previousRowFieldCount;
                                    QString flagd="";
                                    if(thisflag=="\t"){
                                        flagd="制表符";
                                    }
                                    else {
                                        flagd=thisflag;
                                    }
                                    fileName="基于 "+flagd+" 分割列的文件";
                                }
                                contextBgein=lineN;
                            }
                            //非首次遇到-并且解析到了更多的列-则更新起始行
                            else if(lineQListQStringList.at(lineN).count()>previousRowFieldCount&&lineN>contextBgein){
                                previousRowFieldCount=lineQListQStringList.at(lineN).count();
                                if(previousRowFieldCount>lastCount){
                                    lastCount=previousRowFieldCount;
                                }
                                contextBgein=lineN;
                            }
                        }
                        /////////////////step3////////////////////////////////////////
                        //进阶分析标题和其他情况
                        qDebug()<<thisflag;
                        qDebug()<<previousRowFieldCount;
                        qDebug()<<contextBgein+1;
                        if(previousRowFieldCount>1){
                            bool firstRowIsTitle=false;
                            //从疑似第一行开始，做进阶分析
                            //先判断该行数据是否是标题行------取第一行
                            float firstCheck=1;
                            float ct=0;
                            float all=0;
                            QStringList firstRowfieldList=lineQListQStringList.at(contextBgein);
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
                                if(contextBgein+1<lineQListQStringList.count()){
                                    if(lineQListQStringList.at(contextBgein+1).count()==firstRowfieldList.count()){
                                        float secondCheck=1;
                                        float c2t=0;
                                        float all2=0;
                                        QStringList secondRowfieldList=lineQListQStringList.at(contextBgein+1);
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
                                    fieldItem.setFieldName(name);
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
                                    fieldItem.setFieldName(name);
                                    //添加此字段信息到文件定义
                                    fieldTitleList.append(fieldItem);
                                    fieldTitleStrList.append(name);
                                    fieldTitleList.append(fieldItem);
                                }
                            }
                        }
                    }
                }
                //////////////检查识别结果开始解析/////////////////////////
                //检查识别结果
                if(lastCount>1){
                    //构建一个可用的解析配置
                    CsvFileDefinition fileDef;
                    fileDef.setFfAuto(true);
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
void MainWindow::load_indexFile(){
    currentOpenFileType=openFileType::OFDIndex;
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
            currentOpenFileType=openFileType::NotFileOrErr;
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
void MainWindow::load_ofdFile(QString fileTypeFromFileName){
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
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QString lineQString;
        int lineNumber=0;
        bool atLastRow=false;
        while (!dataFile.atEnd())
        {
            //根据OFD文件的编码解开成unicode字符串
            lineQString = codecOFD->toUnicode(dataFile.readLine());
            atLastRow=dataFile.atEnd();
            //记录需要截取的换行符长度
            if(lineNumber==0){
                if(lineQString.endsWith("\r\n")){
                    newLineCharsetLength=2;
                }
                else if(lineQString.endsWith("\r")){
                    newLineCharsetLength=1;
                }
                else if(lineQString.endsWith("\n")){
                    newLineCharsetLength=1;
                }
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
                    //最后一行可能没有换行符
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
                        //没有换行符
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
                    statusBar_disPlayMessage("解析失败,未从文件第2行读取到OFD文件的版本号信息");
                    return;
                }
            }
            if(lineNumber==6){
                QString fileTypeFromFile=lineQString.trimmed();
                if(fileTypeFromFile!=fileType){
                    qDebug()<<"文件内的文件类型和文件名里的不一致,取文件里的";
                    fileType=fileTypeFromFile;
                    ui->lineEditFileType->setText(fileTypeFromFile);
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
                    firstLineDateLenght=codecOFD->fromUnicode(lineQString).length();
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
            //1.8.9版本开始，新增多配置匹配手工选择解析类别
            //比如一个文件既满足开放式基金配置，又满足银行理财子配置，则弹窗让你选择使用哪一个
            QList<OFDFileDefinition> matchOFD;
            for (i = loadedOfdDefinitionHash.constBegin(); i != loadedOfdDefinitionHash.constEnd(); ++i) {
                //判断是否以文件类型开头
                QString ofdHashKey=i.key();
                OFDFileDefinition ofdFileDefinition=i.value();
                QString needCom=versionFromFile+"_"+fileType;
                //标记使用的解析器
                QString config="["+QString(ofdHashKey).replace(versionFromFile+"_","")+"]";
                if(ofdHashKey.startsWith(needCom+"_")||ofdHashKey==needCom){
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
                                //开始判断首行---满足
                                if(firstLineDateLenght ==ofdFileDefinition.getRowLength()){
                                    matchOFD.append(ofdFileDefinition);
                                    continue;
                                }
                                else{
                                    //首行数据不满足
                                    OFDFaultCause fault;
                                    fault.setConfig(config);
                                    fault.setCause("配置文件"+useini+"中的解析器"+config+"所代表的文件的文件每行数据长度是["+QString::number(ofdFileDefinition.getRowLength())+"],但是从文件第1行数据记录获取到的数据长度是["+QString::number(firstLineDateLenght)+"],请检查文件是否满足该配置对应的接口标准,或者配置是否有误");
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
                        fault.setCause(QString("打开的文件的字段数是[%1]和配置文件中的[%2]不一致,请检查文件是否满足该配置对应的接口标准,或者配置是否有误").arg(QString::number(countNumberFromFile)).arg(QString::number(ofdFileDefinition.getFieldCount())));
                        faultList.append(fault);
                    }
                }
            }
            //如果至少一条满足，则准备进入解析步骤
            if(matchOFD.count()>0){
                //只有一条满足的，直接干
                if(matchOFD.count()<2){
                    ofd=matchOFD.at(0);
                }
                else{
                    qDebug()<<"多个配置满足解析";
                    //排序
                    std::sort(matchOFD.begin(), matchOFD.end(),compareOFDData);
                    DialogChooseOFDConfig  dialog2(useini,&matchOFD,this);
                    dialog2.setWindowTitle("打开的文件匹配到了多个解析配置,请选择使用哪一个配置解析文件");
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
                ui->lineEditFileDescribe->setText(ofd.getDescribe());
                ui->lineEditFileDescribe->setToolTip(ofd.getDescribe());
                QFile dataFile(currentOpenFilePath);
                //判断如果文件打开成功,则开始读取
                //革命尚未成功,同志还需努力,要不惜一切办法,提高读取速度
                if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
                {
                    int lineNumber=0;
                    //数据行开始的位置,10行文件头,1行行记录标识+字段行数
                    int beginIndex=11+ofd.getFieldCount();
                    QString line;
                    bool sucessFlag=true;
                    bool mergeSucessFlag=false;
                    bool mergeFlag=false;
                    int ofdrowLength=ofd.getRowLength();
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    bool lastRowIsOFDCFEND=false;
                    bool readToLastRow=true;
                    bool atLastRow=false;
                    while (!dataFile.atEnd())
                    {
                        //如果此行记录小于数据开始行,则认为是文件头,存入文件头
                        if(lineNumber<beginIndex){
                            line = codecOFD->toUnicode(dataFile.readLine());
                            line=line.left(line.length()-newLineCharsetLength);
                            ofdFileHeaderQStringList.append(line);
                        }
                        //数据行,进行数据行分析
                        else{
                            /*******标准数据行*******/
                            //gb18030编码的字节数组-不包含换行符
                            QByteArray lineQByteArray=dataFile.readLine();
                            atLastRow=dataFile.atEnd();
                            //移除换行符
                            if(!atLastRow){
                                lineQByteArray=lineQByteArray.left(lineQByteArray.length()-newLineCharsetLength);
                            }
                            //最后一行，要判断最后一行数据结尾是否有换行符
                            else{
                                //最后一行可能没有换行符
                                if(codecOFD->toUnicode(lineQByteArray).endsWith("\r\n")){
                                    lineQByteArray=lineQByteArray.left(lineQByteArray.length()-2);
                                }
                                else if(codecOFD->toUnicode(lineQByteArray).endsWith("\r")){
                                    lineQByteArray=lineQByteArray.left(lineQByteArray.length()-1);
                                }
                                else if(codecOFD->toUnicode(lineQByteArray).endsWith("\n")){
                                    lineQByteArray=lineQByteArray.left(lineQByteArray.length()-1);
                                }
                            }
                            //转换为unicode字符串
                            line = codecOFD->toUnicode(lineQByteArray);
                            //分析是否读取到了OFDCFEND结束标记,是的话直接跳出循环
                            if(line.length()==8){
                                if(QString::compare(line,"OFDCFEND",Qt::CaseInsensitive)==0){
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
                            if(lineQByteArray.length()==ofdrowLength&&!line.contains("\r")&&!line.contains("\n")){
                                if(dataCompressLevel==0){
                                    ofdFileContentQByteArrayList.append(lineQByteArray);
                                }
                                else{
                                    ofdFileContentQByteArrayList.append(qCompress(lineQByteArray,dataCompressLevel));
                                }
                            }
                            /*******其他情况*******/
                            //如果本行数据长度和接口定义不一致,则尝试读取下一行,分析是否进行数据合并
                            else{
                                //如果已经是最后一行了,且内容不是结束标志--这个问题比较严肃,要提醒用户
                                if(dataFile.atEnd()){
                                    lastRowIsOFDCFEND=false;
                                    ofdFooterQString=line;
                                    ofdFooterRowNumber=lineNumber+1;
                                    break;
                                }
                                QString nextLine = "";
                                //情况1Qt没换行但是含换行符号
                                if(line.contains("\r")){
                                    int index=line.indexOf("\r");
                                    nextLine=line.mid(index+1);
                                    line=line.mid(0,index);
                                }
                                else if(line.contains("\n")){
                                    int index=line.indexOf("\r");
                                    nextLine=line.mid(index+1);
                                    line=line.mid(0,index);
                                }
                                else{
                                    //读取下一行--QT读取换行
                                    nextLine = codecOFD->toUnicode(dataFile.readLine());
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
                                        if(codecOFD->fromUnicode(line+nextLine).length()==(ofdrowLength-0)){
                                            if(dataCompressLevel==0){
                                                ofdFileContentQByteArrayList.append(codecOFD->fromUnicode(line+nextLine));
                                            }
                                            else{
                                                ofdFileContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(line+nextLine),dataCompressLevel));
                                            }
                                            mergeSucessFlag=true;
                                            fileChanged=true;
                                        }
                                        //替换了一个字符
                                        else if(codecOFD->fromUnicode(line+nextLine).length()==(ofdrowLength-1)){
                                            //20210405算法优化，补充空格补充到字段的末尾
                                            for(int i=0;i<ofd.getFieldCount();i++){
                                                //这一行的长度正好是第i个字段的结束位置
                                                //发生换行的位置是第i个字段和第i+1个字段之间--这种情况对下一个字段进行补长度
                                                if(i<ofd.getFieldCount()-1){
                                                    if(ofd.getFieldList().at(i+1).getRowBeginIndex()==codecOFD->fromUnicode(line).length()){
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        //对下一个字段按缺失的长度位置进行插入空格修补
                                                        nextLineByteArray=nextLineByteArray.insert(ofd.getFieldList().at(i+1).getLength()-1," ");
                                                        if(dataCompressLevel==0){
                                                            ofdFileContentQByteArrayList.append(codecOFD->fromUnicode(line).append(nextLineByteArray));
                                                        }
                                                        else{
                                                            ofdFileContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(line).append(nextLineByteArray),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        break;
                                                    }
                                                    //发生换行的位置是第i个字段,对第i个字段最后面补空格
                                                    else if(ofd.getFieldList().at(i+1).getRowBeginIndex()>codecOFD->fromUnicode(line).length()){
                                                        QByteArray lineByteArray=codecOFD->fromUnicode(line);
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        if(dataCompressLevel==0){
                                                            ofdFileContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-1," "));
                                                        }
                                                        else{
                                                            ofdFileContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-1," "),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        break;
                                                    }
                                                }
                                                //如果是最后一个字段中间有毛病
                                                else{
                                                    QByteArray lineByteArray=codecOFD->fromUnicode(line);
                                                    QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                    if(dataCompressLevel==0){
                                                        ofdFileContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).append(" "));
                                                    }
                                                    else{
                                                        ofdFileContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).append(" "),dataCompressLevel));
                                                    }
                                                    mergeSucessFlag=true;
                                                    fileChanged=true;
                                                    break;
                                                }
                                            }
                                        }
                                        //替换了两个字符
                                        else if(codecOFD->fromUnicode(line+nextLine).length()==(ofdrowLength-2)){
                                            for(int i=0;i<ofd.getFieldCount();i++){
                                                //这一行的长度正好是第i个字段的结束位置
                                                //发生换行的位置是第i个字段和第i+1个字段之间--这种情况对下一个字段进行补长度
                                                if(i<ofd.getFieldCount()-1){
                                                    if(ofd.getFieldList().at(i+1).getRowBeginIndex()==codecOFD->fromUnicode(line).length()){
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        //对下一个字段按缺失的长度位置进行插入空格修补
                                                        nextLineByteArray=nextLineByteArray.insert(ofd.getFieldList().at(i+1).getLength()-2,"  ");
                                                        if(dataCompressLevel==0){
                                                            ofdFileContentQByteArrayList.append(codecOFD->fromUnicode(line).append(nextLineByteArray));
                                                        }
                                                        else{
                                                            ofdFileContentQByteArrayList.append(qCompress(codecOFD->fromUnicode(line).append(nextLineByteArray),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        break;
                                                    }
                                                    //发生换行的位置是第i个字段,对第i个字段最后面补空格
                                                    else if(ofd.getFieldList().at(i+1).getRowBeginIndex()>codecOFD->fromUnicode(line).length()){
                                                        QByteArray lineByteArray=codecOFD->fromUnicode(line);
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        if(dataCompressLevel==0){
                                                            ofdFileContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-2,"  "));
                                                        }
                                                        else{
                                                            ofdFileContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).insert(ofd.getFieldList().at(i+1).getRowBeginIndex()-2,"  "),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
                                                        break;
                                                    }
                                                    //如果是最后一个字段中间有毛病
                                                    else{
                                                        QByteArray lineByteArray=codecOFD->fromUnicode(line);
                                                        QByteArray nextLineByteArray=codecOFD->fromUnicode(nextLine);
                                                        if(dataCompressLevel==0){
                                                            ofdFileContentQByteArrayList.append(lineByteArray.append(nextLineByteArray).append("  "));
                                                        }
                                                        else{
                                                            ofdFileContentQByteArrayList.append(qCompress(lineByteArray.append(nextLineByteArray).append("  "),dataCompressLevel));
                                                        }
                                                        mergeSucessFlag=true;
                                                        fileChanged=true;
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
                        statusBar_disPlayMessage("读取到数据行"+QString::number(ofdFileContentQByteArrayList.count())+"行");
                        init_OFDTable();
                        //没读取到最后一行就遇到了结束标志
                        if(!readToLastRow){
                            QMessageBox::warning(this,tr("提示"),QString("\r\n\r\n当前打开的OFD文件还没读取到最后一行就遇到了\"OFDCFEND\"行,所以提前结束了数据读取,\"OFDCFEND\"正常应该出现在最后一行作为文件结束行!!!\r\n\r\n"),QMessageBox::Ok,QMessageBox::Ok);
                        }
                        //读取到了最后一行,但是不是结束标志,提示最后一行是不是OFDCFEND
                        else if(!lastRowIsOFDCFEND){
                            QMessageBox::warning(this,tr("提示"),QString("\r\n\r\n当前打开的OFD文件最后一行当前内容是\"%1\"不是\"OFDCFEND\",正常来说OFD文件应该以\"OFDCFEND\"为结束行!!!\r\n\r\n").arg(ofdFooterQString),QMessageBox::Ok,QMessageBox::Ok);
                        }
                    }
                    //如果失败了,则释放内存
                    //此处很重要,是否启用文件另存使用了ofdFileHeaderQStringList是否为空的判断
                    else{
                        ofdFileHeaderQStringList.clear();
                        ofdFileContentQByteArrayList.clear();
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
                currentOpenFileType=openFileType::NotFileOrErr;
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
        currentOpenFileType=openFileType::NotFileOrErr;
        statusBar_disPlayMessage("解析失败,文件打开失败...");
        return;
    }
}

/**
 * @brief MainWindow::load_csvFile加载CSV文件的函数
 * @param fileType
 */
void MainWindow::load_csvFile(QStringList fileType){
    currentOpenFileType=openFileType::CSVFile;
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
                        int endIgnore=loadedCsvDefinitionList.at(dd).getEndIgnoreRow();
                        //查找可用配置结束，开始分析文件到底是哪个版本
                        //最大化获取文件内的样本数据
                        QTextCodec *codec=QTextCodec::codecForName(loadedCsvDefinitionList.at(dd).getEcoding().toLocal8Bit());
                        QList<QString>csvData;
                        QString line;
                        //当前读取到的行数
                        int row=0;
                        while (!dataFile.atEnd()&&row<(dataBeginRow+endIgnore+1))
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
                        //当文件内存在标题行时，允许解析空文件
                        else{
                            //存在标题行----需要从文件内提取标题行
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
                                    if(titleRowString.isEmpty()){
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
                                    QStringList fieldTitle;
                                    //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getClearQuotes()&&loadedCsvDefinitionList.at(dd).getSplit().length()==1){
                                        QRegExp rx("\\"+loadedCsvDefinitionList.at(dd).getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                                        fieldTitle=titleRowString.split(rx);
                                    }
                                    else{
                                        fieldTitle=titleRowString.split(loadedCsvDefinitionList.at(dd).getSplit());
                                    }
                                    //如果文件字段数是-1.则代表根据标题或者第一行数据自动分析列数和列标题，以标题行或者第一行数据自己造标题数据为准来更新列数
                                    if(loadedCsvDefinitionList.at(dd).getFieldCount()==-1){
                                        int countFromtitle=fieldTitle.count();
                                        if(countFromtitle<2){
                                            FaultCause item;
                                            item.setConfigIndex(dd);
                                            item.setCause("在本配置描述的文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行使用分隔符["+loadedCsvDefinitionList.at(dd).getSplit()+"]找到的标题列数为小于2，可能不是该类型的文件");
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
                            //如果不存在标题行---标题定义在配置文件
                            else{
                                //如果文件内没标题行,则至少要有一行数据用于判断
                                //至少存在一行数据的时候我们才开始进行正常的解析行为
                                //文件内的行数<文件头+文件尾部忽略行【既文件无实质性数据内容】
                                if (csvData.count()<(dataBeginRow+endIgnore)){
                                    FaultCause item;
                                    item.setConfigIndex(dd);
                                    item.setCause("基于该配置文件解析当前文件得出的结论是:当前文件应该是一个不含数据记录的空文件,请确认是否1行数据记录都没有(CSV文件解析对于文件内无标题行的文件,工具需要根据首行数据内容识别文件类别)....");
                                    faultList.append(item);
                                    continue;
                                }
                                //配置不带标题行，但是字段数又是auto的场景
                                if(loadedCsvDefinitionList.at(dd).getFieldCount()==-1){
                                    QString firstDataRowString=csvData.at(loadedCsvDefinitionList.at(dd).getDatabeginrowindex()-1);
                                    //如果需要忽略最后一个多余的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getEndwithflag()=="1"){
                                        firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                                    }
                                    int fieldCountFirstRow=0;
                                    //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getClearQuotes()&&loadedCsvDefinitionList.at(dd).getSplit().length()==1){
                                        QRegExp rx("\\"+loadedCsvDefinitionList.at(dd).getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                                        fieldCountFirstRow=firstDataRowString.split(rx).count();
                                    }
                                    else{
                                        fieldCountFirstRow=firstDataRowString.split(loadedCsvDefinitionList.at(dd).getSplit()).count();
                                    }
                                    if(fieldCountFirstRow<2){
                                        FaultCause item;
                                        item.setConfigIndex(dd);
                                        item.setCause("在本配置描述的文件第"+QString::number(loadedCsvDefinitionList.at(dd).getTitlerowindex())+"行使用分隔符["+loadedCsvDefinitionList.at(dd).getSplit()+"]找到的数据列数小于2，无法断定为该类型的文件");
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
                                //存在明确的字段数定义在配置文件中时
                                else{
                                    QString firstDataRowString=csvData.at(loadedCsvDefinitionList.at(dd).getDatabeginrowindex()-1);
                                    //如果需要忽略最后一个多余的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getEndwithflag()=="1"){
                                        firstDataRowString= firstDataRowString.left(firstDataRowString.length()-1);
                                    }
                                    int fieldCount=0;
                                    //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
                                    if(loadedCsvDefinitionList.at(dd).getClearQuotes()&&loadedCsvDefinitionList.at(dd).getSplit().length()==1){
                                        QRegExp rx("\\"+loadedCsvDefinitionList.at(dd).getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                                        fieldCount=firstDataRowString.split(rx).count();
                                    }
                                    else{
                                        fieldCount=firstDataRowString.split(loadedCsvDefinitionList.at(dd).getSplit()).count();
                                    }
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
                                        item.setCause("在文件第"+QString::number(loadedCsvDefinitionList.at(dd).getDatabeginrowindex())+"行(基于该配置的第一行有效数据)找到的数据有["+QString::number(fieldCount)+"]列数据,和配置文件中定义的列数["+QString::number(loadedCsvDefinitionList.at(dd).getFieldCount())+"]不一致，可能不是本版本的文件,无法解析,如果这是一个新增字段的新版本,请先配置后再解析");
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
        currentOpenFileType=openFileType::NotFileOrErr;
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
    currentOpenFileType=openFileType::FIXEDFile;
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
                        int endIgnore=loadedFixedDefinitionList.at(dd).getEndIgnoreRow();
                        //查找可用配置结束，开始分析文件到底是哪个版本
                        //最大化获取文件内的样本数据
                        QTextCodec *codec=QTextCodec::codecForName(loadedFixedDefinitionList.at(dd).getEcoding().toLocal8Bit());
                        QList<QString>fixedData;
                        QString line;
                        //当前读取到的行数
                        int row=0;
                        //读取到第一行数据为止(1.8.6开始多读取几行尾部忽略行+1，用于判断是否为空数据文件)
                        while (!dataFile.atEnd()&&row<(dataBeginRow+endIgnore+1))
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
                            statusBar_disPlayMessage("空的定长文件,没有任何数据记录可供解析!");
                            return;
                        }
                        else{
                            //文件内的数据行数低于第一行数据，无内容，无法判断
                            if(fixedData.count()<loadedFixedDefinitionList.at(dd).getDataRowBeginIndex()){
                                FaultCause item;
                                item.setConfigIndex(dd);
                                item.setCause("配置文件中标注文件第"+QString::number(loadedFixedDefinitionList.at(dd).getDataRowBeginIndex())+"行开始是数据行了，但是打开的文件只有["+QString::number(fixedData.count())+"]行哟，无有效数据,无法确认该文件符合本解析规则");
                                faultList.append(item);
                                continue;
                            }
                            //文件内的行数<文件头+文件尾部忽略行【既文件无实质性数据内容】
                            else if(fixedData.count()<(dataBeginRow+endIgnore)){
                                FaultCause item;
                                item.setConfigIndex(dd);
                                item.setCause("基于该配置文件解析当前文件得出的结论是:当前文件应该是一个不含数据记录的空文件,请确认是否1行数据记录都没有(定长文件解析不支持空文件解析,工具需要根据首行数据内容识别文件类别)....");
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
    //文件类型描述
    ui->lineEditFileDescribe->setText(fixed.getFileDescribe());
    ui->lineEditFileDescribe->setToolTip(fixed.getFileDescribe());
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
                if(dataCompressLevel==0){
                    fixedContenQByteArrayList.append(codec->fromUnicode(line2));
                }
                else{
                    fixedContenQByteArrayList.append(qCompress(codec->fromUnicode(line2),dataCompressLevel));
                }
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
        //文件尾部提取
        if(fixed.getEndIgnoreRow()>0&&fixedContenQByteArrayList.count()>fixed.getEndIgnoreRow()){
            for(int ic=1;ic<=fixed.getEndIgnoreRow();ic++){
                if(dataCompressLevel==0){
                    fixedFooterQStringList.append(codec->fromUnicode(fixedContenQByteArrayList.last()));
                }
                else{
                    fixedFooterQStringList.append(codec->fromUnicode(qUncompress(fixedContenQByteArrayList.last())));
                }
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
                    currentOpenFileType=openFileType::NotFileOrErr;
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
                    currentOpenFileType=openFileType::NotFileOrErr;
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
    //文件类型描述
    ui->lineEditFileDescribe->setText(csv.getFileDescribe());
    ui->lineEditFileDescribe->setToolTip(csv.getFileDescribe());
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
                if(dataCompressLevel==0){
                    csvFileContentQByteArrayList.append(codec->fromUnicode(line2));
                }
                else{
                    csvFileContentQByteArrayList.append(qCompress(codec->fromUnicode(line2),dataCompressLevel));
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
        if(csv.getEndIgnoreRow()>0&&csvFileContentQByteArrayList.count()>csv.getEndIgnoreRow()){
            for(int ic=1;ic<=csv.getEndIgnoreRow();ic++){
                csvFileContentQByteArrayList.removeLast();
            }
        }
        //文件尾部提取//基于自动分析
        //自动分析csv时，配置文件结尾忽略为-1，自动检查
        else if(csv.getEndIgnoreRow()==-1){
            int ignore=0;
            for(int i=csvFileContentQByteArrayList.count()-1;i>=0;i--){
                QString lastline;
                if(dataCompressLevel==0){
                    lastline=codec->fromUnicode(csvFileContentQByteArrayList.last());
                }
                else{
                    lastline=codec->fromUnicode(qUncompress(csvFileContentQByteArrayList.last()));
                }
                //字段拆分
                QRegExp rx("\\"+csv.getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
                QStringList fieldList=lastline.split(rx);
                qDebug()<<fieldList;
                qDebug()<<csv.getFieldCount();
                //列数小于自动识别出来的列
                if(fieldList.count()<csv.getFieldCount()){
                    csvFileContentQByteArrayList.removeLast();
                    ignore++;
                }
                //列数等于配置，终止
                else{
                    break;
                }
            }
            //回填csv配置
            csv.setEndIgnoreRow(ignore);
        }
    }
    //初始化表格
    init_CSVTable(fieldTitle);
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
    //设置默认GB18030编码--国情
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
    QHash<QString, DbfFileConfig>::const_iterator ii;
    //遍历DBF配置
    for (ii = loadedDbfDefinitionHash.constBegin(); ii != loadedDbfDefinitionHash.constEnd(); ++ii) {
        QString dbfHashKey=ii.key();
        DbfFileConfig dbfFileConfig=ii.value();
        if(dbfFileConfig.getSupportFileList().count()>0){
            for(int kk=0;kk<dbfFileConfig.getSupportFileList().count();kk++){
                bool matchOk=false;
                QString filename=dbfFileConfig.getSupportFileList().at(kk).fileName;
                QString filedescribe=dbfFileConfig.getSupportFileList().at(kk).fileDescribe;
                filename.replace("*","\\S*");
                QString pattern(filename.toUpper());
                QRegExp rx(pattern);
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
    bool configisok=false;
    if(matchListInfo.count()==1){
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
            dbf.setFileIni(matchListInfo.at(0).ini);
            dbf.setFileDescribe(matchListInfo.at(0).matchfileDescribe);
            dbf.setFileType(matchListInfo.at(0).matchFileType);
            configisok=true;
        }
        else{
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
            field.setFieldDescribe(loadedDbfDefinitionHash.value(dbf.getFileIni()).getFieldDescribe().value(field.getFieldName().toUpper()));
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
    ui->lineEditFileDescribe->setText(dbf.getFileDescribe());
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
        //设置Tips
        if(fieldTips.contains("OFD"+ofd.getDictionary())){
            //设置tips
            for(int tipindex=0;tipindex<colCount;tipindex++){
                QString tips=fieldTips.value("OFD"+ofd.getDictionary()).value(ofd.getFieldList().at(tipindex).getFieldName());
                if(!tips.isEmpty()){
                    ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
                }
            }
        }
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
        statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒").arg(QString::number(rowCount)).arg(QString::number(((double)clock() - time_Start)/1000.0)));
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
    if(fieldTips.contains("DBF"+dbf.getFileIni())){
        //设置tips
        for(int tipindex=0;tipindex<colCount;tipindex++){
            QString tips=fieldTips.value("DBF"+dbf.getFileIni()).value(dbf.getFieldList().at(tipindex).getFieldName().toUpper());
            if(!tips.isEmpty()){
                ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
            }
        }
    }
    //设置表格的选择方式
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
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
    statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-%2-耗时%3秒").arg(QString::number(rowCount)).arg(add).arg(QString::number(((double)clock() - time_Start)/1000.0)));
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
    if(fieldTips.contains("FIXED"+fixed.getFileIni())){
        //设置tips
        for(int tipindex=0;tipindex<colCount;tipindex++){
            QString tips=fieldTips.value("FIXED"+fixed.getFileIni()).value(fixed.getFieldList().at(tipindex).getFieldDescribe());
            if(!tips.isEmpty()){
                ptr_table->horizontalHeaderItem(tipindex)->setToolTip(tips);
            }
        }
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
        display_FIXEDTable();
        ptr_table->resizeColumnsToContents();
    }
    else{
        //如果没有数据,也执行下自动设置列宽,增加空数据的显示美感
        ptr_table->resizeColumnsToContents();
    }
    statusBar_display_rowsCount(rowCount);
    statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒").arg(QString::number(rowCount)).arg(QString::number(((double)clock() - time_Start)/1000.0)));
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
        for(int rowIndex=0;rowIndex<20&&rowIndex<csvFileContentQByteArrayList.count();rowIndex++){
            //获取本行数据
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowIndex);
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
                                    isnumber.setDecimalLength(0);
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
        if(fieldTips.contains("CSV"+csv.getFileIni())){
            //设置tips
            for(int tipindex=0;tipindex<colCount;tipindex++){
                QString tips=fieldTips.value("CSV"+csv.getFileIni()).value(csv.getFieldList().at(tipindex).getFieldName());
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
        statusBar_display_rowsCount(rowCount,csv.getEcoding());
        if(csv.getFfAuto()){
            statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-[使用自动解析引擎解析,建议配置解析规则获得更好体验]-耗时%2秒").arg(QString::number(rowCount)).arg(QString::number(((double)clock() - time_Start)/1000.0)));
        }
        else{
            statusBar_disPlayMessage(QString("文件解析完毕!成功读取记录%1行-耗时%2秒").arg(QString::number(rowCount)).arg(QString::number(((double)clock() - time_Start)/1000.0)));
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
            QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowInFileContent);
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
            QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowInFileContent);
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
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowInFileContent);
            //csv文件数据的某一行可能是不完整的，做忽略容忍！，允许正常解析
            //后续将不满足的数据记录在案
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
            }
        }
    }
    //分析判断是否需要重新设置列宽
    int count=needRestwitdh.count();
    for(int cc=0;cc<count;cc++){
        ptr_table->resizeColumnToContents(needRestwitdh.at(cc));
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
                itemVerticalHeaderItem->setTextColor(Qt::red);
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
                        item->setTextColor(Qt::red);
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
    }
}

/**
 * @brief MainWindow::clear_Table_Info 清空表格内容--包含包含数据信息等
 */
void MainWindow::clear_Table_Info(){
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    ptr_table->setColumnCount(0);
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
    //不同数据类型插入点
    ofdIndexFileHeaderMap.clear();
    indexFileDataList.clear();
    ofdFileHeaderQStringList.clear();
    ofdFooterQString="";
    ofdFileContentQByteArrayList.clear();
    ofdFileContentQByteArrayListFromExcel.clear();
    csvFileHeaderQStringList.clear();
    csvFileContentQByteArrayList.clear();
    fixedFileHeaderQStringList.clear();
    fixedContenQByteArrayList.clear();
    fixedFooterQStringList.clear();
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
    /////////////////////////////////////////////
}

/**
 * @brief MainWindow::statusBar_disPlayMessage 状态栏显示消息的函数
 * @param text
 */
void MainWindow::statusBar_disPlayMessage(QString text){
    statusLabel_ptr_showMessage->setText(text);
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
            info.append("文件内标识的总记录数:").append(ofdIndexFileHeaderMap.value("count")).append("成功加载记录数:").append(QString::number(indexFileDataList.count())).append("\r\n");
            QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    else if(currentOpenFileType==openFileType::OFDFile){
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
    else if(currentOpenFileType==openFileType::CSVFile){
        //组织要显示的内容
        QString info;
        info.append("文件解析情况如下:\r\n");
        info.append("使用的配置文件:").append(csv.getFileIni()).append("\r\n");
        info.append("使用的解析器配置:[").append(csv.getFileNameWithCount()).append("]\r\n");
        info.append("加载的文件头行数:").append(QString::number(csvFileHeaderQStringList.count())).append("\r\n");
        info.append("加载的数据行数:").append(QString::number(csvFileContentQByteArrayList.count())).append("\r\n");
        info.append("加载的文件尾行数:").append(QString::number(csv.getEndIgnoreRow())).append("\r\n");
        QMessageBox::information(this,tr("文件检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        //组织要显示的内容
        QString info;
        info.append("文件解析情况如下:\r\n");
        info.append("使用的配置文件:").append(fixed.getFileIni()).append("\r\n");
        info.append("使用的解析器配置:[").append(fixed.getFileNameWithVersion()).append("]\r\n");
        info.append("加载的文件头行数:").append(QString::number(fixedFileHeaderQStringList.count())).append("\r\n");
        info.append("加载的数据行数:").append(QString::number(fixedContenQByteArrayList.count())).append("\r\n");
        info.append("加载的文件尾行数:").append(QString::number(fixedFooterQStringList.count())).append("\r\n");
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

void MainWindow::copyToClipboardWithoutTitle(){
    copyToClipboard(false);
}

void MainWindow::copyToClipboardWithTitle(){
    copyToClipboard(true);
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
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,col)));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,col));
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
                        QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
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
                                value.append(Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,col)));
                            }
                            else{
                                value.append(Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,col));
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
                //OFD
                if(currentOpenFileType==openFileType::OFDFile){
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                }
                //CSV
                else if(currentOpenFileType==openFileType::CSVFile){
                    compareData.insert(rowRealInContent+1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent));
                }
                //FIXED
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                }
                //DBF
                else if(currentOpenFileType==openFileType::DBFFile){
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType));
                }
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
            if(currentOpenFileType==openFileType::OFDFile){
                for(int l1=0;l1<rangeCount;l1++){
                    for(int l2=itemsRange.at(l1).topRow();l2<=itemsRange.at(l1).bottomRow();l2++){
                        rowRealInContent=(currentPage-1)*pageRowSize+l2;
                        if(compareData.contains(rowRealInContent+1)){
                            continue;
                        }
                        else{
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
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
                            compareData.insert(rowRealInContent+1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent));
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
                            compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
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
    if(currentOpenFileType==openFileType::OFDFile){
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
    if(currentOpenFileType==openFileType::OFDFile){
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
                needCopyData.append(codecOFD->toUnicode(ofdFileContentQByteArrayList.at(rowRealInContent)));
            }
            else{
                needCopyData.append(codecOFD->toUnicode(qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent))));
            }
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
        int Length=ofd.getRowLength();
        for(int charIndex=0;charIndex<Length;charIndex++){
            newLine.append(QChar(' ').toLatin1());
        }
        if(dataCompressLevel==0){
            ofdFileContentQByteArrayList.insert(insertIndexInContent,newLine);
        }
        else{
            ofdFileContentQByteArrayList.insert(insertIndexInContent,qCompress(newLine,dataCompressLevel));
        }
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
                colitem.append(csv.getFieldList().at(i).getFieldName());
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
                colitem.append(csv.getFieldList().at(i).getFieldName());
                colitem.append(nullptr);
                //字段值
                colitem.append(colvalue);
                //字典翻译
                colitem.append(commonDictionary.value("CSV"+csv.getFileIni()).getDictionary(csv.getFieldList().at(i).getFieldName(),colvalue));
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
    dialog->setWindowTitle(QString("查看表格行记录-第%1行").arg(dataRowCurrent+1));
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
        dialog->setWindowTitle(QString("数据放大镜-第%1行第%2列").arg(rowRealInContent+1).arg(tableColCurrent+1));
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
            QString hex=codec->fromUnicode(data.at(i)).toHex().toUpper();
            //字符
            colitem.append(data.at(i));
            //字节数
            colitem.append(QString::number(hex.length()/2));
            //16进制
            colitem.append(hex);
            //2进制
            colitem.append(HexStringToBinaryString(hex));
            //特殊字符说明
            if(specialCharacter.contains(hex)){
                colitem.append(specialCharacter.value(hex));
            }
            else{
                colitem.append(nullptr);
            }
            rowdata.append(colitem);
        }
    }
    //打开窗口
    DialogShowCharset * dialog = new DialogShowCharset(&rowdata,this);
    dialog->setWindowTitle(QString("查看十六进制和二进制编码(%1)").arg(charset));
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
    dialog.setWindowTitle(QString("对此列调整数据格式-%1").arg(csv.getFieldList().at(tableColCurrent).getFieldName()));
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

/**
 * @brief MainWindow::showOFDFiledAnalysis 用于分析OFD文件列合法性的工具
 */
void MainWindow:: showOFDFiledAnalysis(){
    //注意，这个函数不建议复用，如果需要进行别的文件类别合法性分析，建议设置单独的函数
    if(currentOpenFileType==openFileType::OFDFile){
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
        QString fieldOaiginal=Utils::getOriginalValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
        //字段翻译值
        QString fieldValues="";
        if(fieldType=="N"){
            fieldValues=Utils::CovertDoubleQStringWithThousandSplit(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent));
        }
        else{
            fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
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
        rowfieldHexValues.append("逐字符16进制(GB18030编码)");
        rowfieldHexValues.append(StringToHexStringWithEnCode(fieldValues,codecOFD,true));
        data.append(rowfieldHexValues);
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
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

/**
 * @brief MainWindow::showModifyOFDCell OFD专用修改工具函数
 */
void MainWindow::showModifyOFDCell(){
    if(currentOpenFileType==openFileType::OFDFile){
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
        QString fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,editCol);
        //字段修改标记
        bool modifyFlag=false;
        //修改后的值
        QString valueNew="";
        //打开窗口
        DialogModifyCell  dialog(fieldType,fieldLength,fieldDecLength,fieldValues,this);
        dialog.setWindowTitle(QString("编辑第%1行第%2列-"+ofd.getFieldList().at(tableColCurrent).getFieldDescribe()).arg(rowRealInContent+1).arg(tableColCurrent+1));
        dialog.setModal(true);
        dialog.exec();
        //从弹窗中获取结果
        modifyFlag=dialog.getModifyFlag();
        valueNew=dialog.getValueNew();
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
                QByteArray valueNewArrayRow;
                if(dataCompressLevel==0){
                    valueNewArrayRow=ofdFileContentQByteArrayList.at(rowRealInContent);
                }
                else{
                    valueNewArrayRow=qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent));
                }
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
                            valueNewArrayRow[i]=' ';
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
                if(dataCompressLevel==0){
                    ofdFileContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);
                }
                else{
                    ofdFileContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                }
                //更新界面/////////////////////////////////////////////////////////////////
                QString valueForItem=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,editCol);
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
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
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
    if(currentOpenFileType==openFileType::OFDFile){
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
        DialogModifyCell dialog(fieldType,fieldLength,fieldDecLength,"",this);
        dialog.setWindowTitle(QString("批量编辑第%1列多个单元格-"+ofd.getFieldList().at(tableColCurrent).getFieldDescribe()).arg(tableColCurrent+1));
        dialog.setModal(true);
        dialog.exec();
        //从弹窗中获取结果
        modifyFlag=dialog.getModifyFlag();
        QString valueNew=dialog.getValueNew();
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
                //全空数据自动补充空格
                if(valueNew.isEmpty()){
                    for(int i=0;i<fieldLength;i++){
                        valueNewArray.append(' ');
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
                        QByteArray valueNewArrayRow;
                        if(dataCompressLevel==0){
                            valueNewArrayRow=ofdFileContentQByteArrayList.at(rowRealInContent);
                        }
                        else{
                            valueNewArrayRow=qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent));
                        }
                        //更新字段值
                        int index=0;
                        for(int i=updateBegin;i<updateEnd;i++){
                            valueNewArrayRow[i]=valueNewArray.at(index);
                            index++;
                        }
                        //qDebug()<<valueNewArrayRow;
                        //将新的行记录写入原ofdFileContentQByteArrayList/////////////////////////////
                        if(dataCompressLevel==0){
                            ofdFileContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);
                        }
                        else{
                            ofdFileContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                        }
                        //更新界面/////////////////////////////////////////////////////////////////
                        QString valueForItem=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,editCol);
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
                            compareData.insert(editRow+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,editRow));
                        }
                        updatedRow++;
                        if((updatedRow%1000==0)){
                            statusBar_disPlayMessage(QString("批量更新中,请勿进行其他操作,已更新%1行").arg(QString::number(updatedRow)));
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
        QStringList rowDataOld=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
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
                QByteArray valueNewArrayRow;
                if(dataCompressLevel==0){
                    valueNewArrayRow=ofdFileContentQByteArrayList.at(rowRealInContent);
                }
                else{
                    valueNewArrayRow=qUncompress(ofdFileContentQByteArrayList.at(rowRealInContent));
                }
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
                                    valueNewArrayRow[i]=' ';
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
                        if(dataCompressLevel==0){
                            ofdFileContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);
                        }
                        else{
                            ofdFileContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                        }
                        //更新界面/////////////////////////////////////////////////////////////////
                        QString itemValue=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,editCol);
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
                    compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
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
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //数据类型加入点
    if(currentOpenFileType==openFileType::OFDFile){
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        if(csvFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(fixedContenQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
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
    if(currentOpenFileType==openFileType::OFDFile){
        colCount=ofd.getFieldCount();
    }
    if(currentOpenFileType==openFileType::CSVFile){
        colCount=csv.getFieldCount();
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        colCount=fixed.getFieldCountMax();
    }
    if(currentOpenFileType==openFileType::DBFFile){
        colCount=dbf.getFieldCount();
    }
    //分页模式下的跨页搜索
    int rowRealInContent=0;
    while(currentPage>0){
        //数据类型插入点
        if(currentOpenFileType==openFileType::OFDFile){
            for(int row=tableRowCurrent;row>=0;row--){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::CSVFile){
            for(int row=tableRowCurrent;row>=0;row--){
                //csv文件按行获取数据
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            for(int row=tableRowCurrent;row>=0;row--){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::DBFFile){
            for(int row=tableRowCurrent;row>=0;row--){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
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
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //行列数
    int rowcount=0;
    int colCount=0;
    //数据类型加入点
    if(currentOpenFileType==openFileType::OFDFile){
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }else{
            rowcount=ofdFileContentQByteArrayList.count();
            colCount=ofd.getFieldCount();
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        if(csvFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }else{
            rowcount=csvFileContentQByteArrayList.count();
            colCount=csv.getFieldCount();
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(fixedContenQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }else{
            rowcount=fixedContenQByteArrayList.count();
            colCount=fixed.getFieldCountMax();
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfRowMap.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }else{
            rowcount=dbfRowMap.count();
            colCount=dbf.getFieldCount();
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
        if(currentOpenFileType==openFileType::OFDFile){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::CSVFile){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                //csv文件按行获取数据
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::FIXEDFile){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
            }
        }
        if(currentOpenFileType==openFileType::DBFFile){
            for(int row=tableRowCurrent;row<currentPageRowCount;row++){
                rowRealInContent=(currentPage-1)*pageRowSize+row;
                QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType);
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
                    //强制终止事件-立即return退出
                    if(abortExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
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
    //特殊情况当前打开的文件是OFD文件，总行数是0的空文件允许插入内容--仅仅允许插入数据
    if(currentOpenFileType==openFileType::OFDFile&&ptr_table->rowCount()<1&&ptr_table->rowAt(pos.y()) <0){
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
                    tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                    tablePopMenu->addAction(action_ShowDetails);
                    action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
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
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
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
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
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
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        action_ShareUseQrCode->setText("使用二维码分享选中的单元格数据");
                        if(ENABLE_QRCODE){
                            tablePopMenu->addAction(action_ShareUseQrCode);
                        }
                        tablePopMenu->addSeparator();
                        tablePopMenu->addAction(action_EditCompareDataBatch);
                        tablePopMenu->addSeparator();
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误--跨选取复制
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
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
                    action_ShareUseQrCode->setText("使用二维码分享此单元格数据");
                    if(ENABLE_QRCODE){
                        tablePopMenu->addAction(action_ShareUseQrCode);
                    }
                    tablePopMenu->addAction(action_Magnify);
                    tablePopMenu->addAction(action_ShowCharacter);
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
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        //tablePopMenu->addAction(action_ModifyCellBatch);
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
                    }
                    //多个选择器
                    else{
                        //将会弹出复制错误
                        tablePopMenu->addAction(action_ShowCopyColum);
                        tablePopMenu->addAction(action_ShowCopyColumWithTitle);
                        //tablePopMenu->addAction(action_ModifyCellBatch);
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
                    title.append(csv.getFieldList().at(i).getFieldName());
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
    //预先数据判断
    if(ptr_table->rowCount()<1){
        statusBar_disPlayMessage("没有数据可供导出,目前表格有0行数据");
        return;
    }
    /////////////////导出方案选择///////////////////////////
    bool moreThan=false;
    bool useUTF8=false;
    //超出限制无法导出整个excel
    if(ofdFileContentQByteArrayList.count()>maxExcelRow||csvFileContentQByteArrayList.count()>maxExcelRow||fixedContenQByteArrayList.count()>maxExcelRow||dbfRowMap.count()>maxExcelRow){
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
        rowEnd=ofdFileContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=csvFileContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=fixedContenQByteArrayList.count();
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
        else{
            codec=QTextCodec::codecForName("UTF-8");
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
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,row);
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
            data.close();
            statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
        }else{
            statusBar_disPlayMessage(tr("数据导出失败,请重试"));
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(useUTF8){
            codec=QTextCodec::codecForName("UTF-8");
        }
        else{
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
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,row);
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
            data.close();
            statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
        }else{
            statusBar_disPlayMessage(tr("数据导出失败,请重试"));
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(useUTF8){
            codec=QTextCodec::codecForName("UTF-8");
        }
        else{
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
        rowEnd=ofdFileContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=csvFileContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=fixedContenQByteArrayList.count();
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
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                sb.append("<tr>");
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,row);
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
        if(currentOpenFileType==openFileType::CSVFile){
            int exprow=0;
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                //csv文件按行获取数据
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,row);
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
        if(currentOpenFileType==openFileType::FIXEDFile){
            int exprow=0;
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                sb.append("<tr>");
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,row);
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
        if(currentOpenFileType==openFileType::DBFFile){
            int exprow=0;
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
        rowEnd=ofdFileContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=csvFileContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=fixedContenQByteArrayList.count();
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
    const int columnCount=ptr_table->columnCount();
    int colWidthArray[columnCount];
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
        for (int row=rowBegin;row<rowEnd;row++){
            //数据写入--按行读取
            QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,row);
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
                        xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(excelRowNumber,col+1,value,numberFormatQhash.value(col));
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
        for (int row=rowBegin;row<rowEnd;row++){
            //数据写入--按行读取
            //csv文件按行获取数据
            QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,row);
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
                    if(okNb){
                        xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(excelRowNumber,col+1,value,numberFormatQhash.value(col));
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
        for (int row=rowBegin;row<rowEnd;row++){
            //数据写入--按行读取
            QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,row);
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
                        xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(excelRowNumber,col+1,value,numberFormatQhash.value(col));
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
                    if(okNb){
                        xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                    }
                    else{
                        xlsx->write(excelRowNumber,col+1,value,numberFormatQhash.value(col));
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
        if(currentOpenFileType==openFileType::CSVFile){
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
        if(currentOpenFileType==openFileType::FIXEDFile){
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
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
    }
    if(currentOpenFileType==openFileType::OFDFile){
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
    if(currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
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
            if(dataCompressLevel==0){
                sb.append(codecOFD->toUnicode(ofdFileContentQByteArrayList.at(row))).append("\r\n");
            }
            else{
                sb.append(codecOFD->toUnicode(qUncompress(ofdFileContentQByteArrayList.at(row)))).append("\r\n");
            }
            //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
            //仅1000行或者到最后一行时进行写入
            if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                newfile.write(codecOFD->fromUnicode(sb));
                sb.clear();
                statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                qApp->processEvents();
                //强制终止事件-立即return退出
                if(abortExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
            row++;
        }
        //写入文件结束标志位//////////////////////////////////////////////////////////////////
        newfile.write(codecOFD->fromUnicode(ofdFooterQString+"\r\n"));
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
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
        return;
    }
    if(currentOpenFileType==openFileType::OFDFile){
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
    if(currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
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
        if(ofdFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        if(csvFileContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
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
        if(currentOpenFileType==openFileType::OFDFile){
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
        if(currentOpenFileType==openFileType::CSVFile){
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
        if(currentOpenFileType==openFileType::FIXEDFile){
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
            if(currentOpenFileType==openFileType::OFDFile){
                display_OFDTable();
            }
            //CSV文件
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
                    //补充温馨提示
                    QString addinfo="";
                    int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
                    QString fieldOaiginal=Utils::getOriginalValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,tableColCurrent);
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
                        //其余情况统一处理
                        else if((!text.isEmpty())&&(!fieldOaiginal.contains(QRegExp("^\\d+$")))){
                            addinfo="警告:数值型字段混入了0-9外的字符,极易造成兼容问题!!!";
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
            int rowInFile=csvFileHeaderQStringList.count()+dataRowCurrent+1;
            int colInFile=tableColCurrent+1;
            statusBar_display_rowsAndCol(rowInFile,colInFile,0);
            if(ptr_table->item(tableRowCurrent,tableColCurrent)!=nullptr){
                QString text=ptr_table->item(tableRowCurrent,tableColCurrent)->text();
                QString dic=commonDictionary.value("CSV"+csv.getFileIni()).getDictionary(csv.getFieldList().at(tableColCurrent).getFieldName(),text);
                if(!text.isEmpty()){
                    //如果文件执行过双引号边界符处理，则附带显示原始数据，方便用户阅览原始值
                    if(csv.getClearQuotes()||csv.getTrim()){
                        QStringList rowdata=Utils::getOriginalRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,dataRowCurrent);
                        statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)).append("|原始值:").append(rowdata.at(tableColCurrent)));
                    }
                    //如果探测到本列是数值,也附带显示原始值
                    else if(CsvFieldIsNumberOrNot.value(tableColCurrent).getIsNumber()){
                        QStringList rowdata=Utils::getOriginalRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,dataRowCurrent);
                        statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)).append("|原始值:").append(rowdata.at(tableColCurrent)));
                    }
                    else{
                        statusBar_disPlayMessage(text.append(dic.isEmpty()?"":("|"+dic)));
                    }
                }
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
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
                            QString vvv=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,editRowinFileContent,editCol);
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
                            QStringList rowList=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,editRowinFileContent);
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
                            QString vvv=Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,editRowinFileContent,editCol);
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
    QMap <QString,QString>par;
    par.insert("compresslevel",QString::number(dataCompressLevel));
    par.insert("defaultviewmode",defaultViewMode);
    par.insert("defaultnewfilemode",defaultNewFileMode);
    par.insert("defaultpagesizetype",defaultPageSizeType);
    DialogPreference  dialog(par,this);
    dialog.setModal(true);
    dialog.exec();
    //获取结果
    if(dialog.getSaveFlag()){
        bool changeFlag=false;
        //设置项压缩等级
        if(dialog.getDataCompressLevel()!=dataCompressLevel){
            if(dataCompressLevel==0&&dialog.getDataCompressLevel()!=0){
                enableOrDisableCompress=true;
            }
            if(dataCompressLevel!=0&&dialog.getDataCompressLevel()==0){
                enableOrDisableCompress=true;
            }
            changeFlag=true;
            this->dataCompressLevel=dialog.getDataCompressLevel();
        }
        if(dialog.getDefaultViewMode()!=defaultViewMode){
            changeFlag=true;
            this->defaultViewMode=dialog.getDefaultViewMode();
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
        if(dialog.getDefaultNewFileMode()!=defaultNewFileMode){
            changeFlag=true;
            this->defaultNewFileMode=dialog.getDefaultNewFileMode();
        }
        //是否修改了分页每页页数
        if(dialog.getDefaultPageSizeType()!=defaultPageSizeType){
            changeFlag=true;
            this->defaultPageSizeType=dialog.getDefaultPageSizeType();
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
            else if(this->defaultPageSizeType=="4"){
                ui->labelPageSize->setText("(200万行/页)");
                this->pageRowSize=2000000;
            }
            else{
                ui->labelPageSize->setText("(10万行/页)");
                this->pageRowSize=100000;
            }
            //如果每页条数变化了，并且当前打开了文件，需要跳到第一页-并且判断是否需要打开或者隐藏分页控件
            int rowCount=0;
            if(currentOpenFileType==openFileType::OFDFile){
                rowCount=ofdFileContentQByteArrayList.count();
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowCount=csvFileContentQByteArrayList.count();
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowCount=fixedContenQByteArrayList.count();
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
    if(currentOpenFileType==openFileType::CSVFile){
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
    if(currentOpenFileType==openFileType::FIXEDFile){
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
        pageJump(currentPage,0);
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
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
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
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
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
        if(currentOpenFileType==openFileType::OFDFile){
            currentPageRowSize=ofdFileContentQByteArrayList.count()-(pageRowSize*(pageCount-1));
        }
        else if(currentOpenFileType==openFileType::CSVFile){
            currentPageRowSize=csvFileContentQByteArrayList.count()-(pageRowSize*(pageCount-1));
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            currentPageRowSize=fixedContenQByteArrayList.count()-(pageRowSize*(pageCount-1));
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
    statusBar_disPlayMessage(QString("第%1页,本页记录数%2行").arg(page).arg(currentPageRowSize));
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
    }
    else if(scrollIndex==0){
        ptr_table->scrollToTop();
        //强制触发下刷新，避免显示数据不完整
        acceptVScrollValueChanged(0);
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
        QProcess process;
        //Qt 5.6在xp系统下有个bug，调用不传参数的startDetached函数时，有可能无法启动程序,所以我们设定一个空参数调用带参数的函数
        process.startDetached(winmergepath,pars);
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
    if(currentOpenFileType==openFileType::OFDFile){
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
    else{
        statusBar_disPlayMessage("暂不支持编辑此类文件,敬请期待");
    }
}

void MainWindow::on_actionedit2_triggered()
{
    if(currentOpenFileType==openFileType::OFDFile){
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
    if(currentOpenFileType==openFileType::OFDFile){
        row=ofdFileContentQByteArrayList.count();
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        row=csvFileContentQByteArrayList.count();
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        row=fixedContenQByteArrayList.count();
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        row=dbfRowMap.count();
    }
    else if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持在excel中打开OFD索引文件...");
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
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    //设置编辑方式
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->verticalHeader()->setDefaultSectionSize(rowHight);
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
    return QWidget::event(event);
}

QString MainWindow::StringToHexStringWithEnCode(QString data,QTextCodec *codec,bool withSouceChar){
    QString sb;
    if(withSouceChar){

        for(int i=0;i<data.length();i++){
            sb.append("[").append(data.at(i)).append(":").append(codec->fromUnicode(data.at(i)).toHex().toUpper()).append("]");
            if(i<data.length()-1){
                sb.append("-");
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
        //////
        QStringList row1;
        row1.append("1");
        row1.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(0):"");
        row1.append("文件标识");
        header.append(row1);

        QStringList row2;
        row2.append("2");
        row2.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(1):"");
        row2.append("文件版本号");
        header.append(row2);

        QStringList row3;
        row3.append("3");
        row3.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(2):"");
        row3.append("文件创建人");
        header.append(row3);

        QStringList row4;
        row4.append("4");
        row4.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(3):"");
        row4.append("文件接收人");
        header.append(row4);

        QStringList row5;
        row5.append("5");
        row5.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(4):"");
        row5.append("日期");
        header.append(row5);

        QStringList row6;
        row6.append("6");
        row6.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(5):"");
        row6.append("汇总表号");
        header.append(row6);

        QStringList row7;
        row7.append("7");
        row7.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(6):"");
        row7.append("文件类型码");
        header.append(row7);

        QStringList row8;
        row8.append("8");
        row8.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(7):"");
        row8.append("发送人");
        header.append(row8);

        QStringList row9;
        row9.append("9");
        row9.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(8):"");
        row9.append("接收人");
        header.append(row9);

        QStringList row10;
        row10.append("10");
        row10.append(ofdFileHeaderQStringList.count()>=10?ofdFileHeaderQStringList.at(9):"");
        row10.append("字段数");
        header.append(row10);

        //字段信息和文件记录数信息
        if(ofdFileHeaderQStringList.count()>10){
            for(int hn=10;hn<ofdFileHeaderQStringList.count();hn++){
                QStringList rowdata;
                rowdata.append(QString::number(hn+1));
                rowdata.append(ofdFileHeaderQStringList.at(hn));
                //文件记录数
                if(hn==ofdFileHeaderQStringList.count()-1){
                    rowdata.append("文件记录数");
                }
                //字段名
                else if(hn<ofdFileHeaderQStringList.count()-1&&ofd.getFieldList().count()>hn-10){
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
                            QString value=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,leftColumn);
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
                            QStringList line=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
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
                            QString value=Utils::getFormatValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,leftColumn);
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
                            line=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::CSVFile){
                            line=Utils::getRowCsvValuesFromcsvFileContentQStringList(&csvFileContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            line=Utils::getFormatRowValuesFromfixedFileContentQStringList(&fixedContenQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
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
            statusBar_disPlayMessage(QString("数据太多了,生成二维码的数据限制最多120个单元格且不大于2300字节,当前%1字节").arg(text.toUtf8().count()));
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
    if(currentOpenFileType==openFileType::OFDFile){
        DialogMyTip dialog2("请确认是否从Excel导入数据到当前打开的OFD文件，此功能将会覆盖当前文件的数据,请注意备份，导入的Excel第一行的标题必须和本文件一致，建议使用本程序导出xlsx文件编辑后再导入！\r\n数值型数据将会按照字段小数长度四舍五入截取,建议在Excel中按数值存储精确数据,以免造成数据错误,本程序不支持公式生成的数值字段!",this);
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
                //导入文件比较耗时，放到新的线程
                statusBar_disPlayMessage("正在分析校验Excel文件，请耐心等待...");
                auto future = QtConcurrent::run(this, &MainWindow::importFromExcel);
                watcherXlsxImportStatus_->setFuture(future);
            } else {
                //放弃了读取文件
                return;
            }
        }
        else{
            return;
        }
    }
    else{
        statusBar_disPlayMessage("打开一个OFD数据文件,然后才可以导入此文件对应的Excel数据(字段中文名一样)...");
    }
}

/**
 * @brief MainWindow::importFromExcel 从Excel导入文件到OFD的主体函数
 * @return
 */

int MainWindow::importFromExcel(){
    dataBlocked=true;
    importExcelErrorDetail.clear();
    ofdFileContentQByteArrayListFromExcel.clear();
    dataBlockedMessage="正在装载Excel文件,请稍候...";
    if (Utils::isFileExist(fromExcelImportFilePath)){
        QXlsx::Document xlsxread(fromExcelImportFilePath);
        int colCount=ofd.getFieldCount();
        if(xlsxread.load()){
            //读取第一行标题
            for(int col=1;col<=colCount;col++){
                //全角半角标点统一转换为半角
                if(xlsxread.read(1,col).toString().replace("（","(").replace("）",")")!=ofd.getFieldList().at(col-1).getFieldDescribe().replace("（","(").replace("）",")")){
                    importExcelErrorDetail.append("选择的文件第"+Utils::CovertInt2ExcelCol(col-1)+"列不是["+ofd.getFieldList().at(col-1).getFieldDescribe()+"],请确认你导入的文件第一行标题和当前打开的文件列名一致,你可以使用数据导出功能导出一份Excel文件后编辑导入！");
                    return 1;
                }
            }
            //开始读取数据
            importExcelRow=0;
            int row=2;
            //如果某一行数据都是空，则结束导入
            bool allColIsEmpty=true;
            while (true){
                QStringList rowdata;
                allColIsEmpty=true;
                //遍历列
                for(int col=1;col<=colCount;col++){
                    //数据类型
                    QString filedType=this->ofd.getFieldList().at(col-1).getFieldType();
                    QString fieldDesc=this->ofd.getFieldList().at(col-1).getFieldDescribe();
                    int filedLength=this->ofd.getFieldList().at(col-1).getLength();
                    int filedDecLength=this->ofd.getFieldList().at(col-1).getDecLength();
                    std::string filedDecLengthString=QString("%.%1f").arg(filedDecLength).toStdString();
                    //提取数据
                    QVariant colvalQVariant=xlsxread.read(row,col);
                    QString colval="";
                    qDebug()<<colvalQVariant;
                    //数值///////////////
                    if(colvalQVariant.type() == QVariant::Double){
                        if(filedType=="N"){
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
                    //////////////////
                    //excel记录非数值
                    else{
                        //但是本列是数值
                        if(filedType=="N"){
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
                    //只要有一列非空就非空
                    if(!colval.isEmpty()){
                        allColIsEmpty=false;
                    }
                    //数据校验
                    int textLength=codecOFD->fromUnicode(colval).length();
                    //字符类和文本类的只需要判断长度
                    if(filedType=="C"||filedType=="TEXT"){
                        //长度校验
                        if(textLength>filedLength){
                            importExcelErrorDetail.append(QString("第[%1]行,字段[%2]长度不应当大于[%3]").arg(row).arg(fieldDesc).arg(filedLength));
                        }
                    }
                    //数字字符型
                    else if(filedType=="A"){
                        //长度校验
                        if(textLength>filedLength){
                            importExcelErrorDetail.append(QString("第[%1]行,字段[%2]长度不应当大于[%3]").arg(row).arg(fieldDesc).arg(filedLength));
                        }
                    }
                    //数字型
                    else if(filedType=="N"){
                        if(!colval.isEmpty()){
                            bool ok=false;
                            colval.toDouble(&ok);
                            //如果无法转化为Double直接认为不合规//兼容负数
                            if(!ok){
                                importExcelErrorDetail.append(QString("第[%1]行,字段[%2]应当是一个数值或者为空,但是现在看起来并不是").arg(row).arg(fieldDesc));
                            }
                            else{
                                //无小数部分
                                if(!colval.contains("."))
                                {
                                    if(textLength>(filedLength-filedDecLength)){
                                        importExcelErrorDetail.append(QString("第[%1]行,字段[%2]整数部分长度不能超过[%3]").arg(row).arg(fieldDesc).arg(filedLength-filedDecLength));
                                    }
                                }
                                //如果包含小数点
                                else{
                                    //拆分出来整数和小数
                                    int index=colval.indexOf(".");
                                    //整数部分
                                    QString intS=colval.mid(0,index);
                                    //整数部分有问题
                                    if(intS.length()>(filedLength-filedDecLength)){
                                        importExcelErrorDetail.append(QString("第[%1]行,字段[%2]的整数部分长度不能超过[%3]").arg(row).arg(fieldDesc).arg(filedLength-filedDecLength));
                                    }
                                    //整数部分没问题开始检查小数部分
                                    else {
                                        //小数部分
                                        QString intD=colval.mid(index+1,-1);
                                        //小数部分超长则直接截取
                                        if(intD.length()>filedDecLength){
                                            colval=colval.mid(0,index+filedDecLength+1);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //其他未知类型,仅校验长度
                    else{
                        //长度校验
                        if(textLength>filedLength){
                            importExcelErrorDetail.append(QString("第[%1]行,字段[%2]的长度不应当大于[%3]").arg(row).arg(fieldDesc).arg(filedLength));
                        }
                    }
                    //插入数据
                    rowdata.append(colval);
                }
                row++;
                importExcelRow++;
                //全空就代表结束了，不再往下读取
                if(allColIsEmpty){
                    break;
                }
                //没错误的情况下插入数据，只要有一条错误就不插入
                else if (importExcelErrorDetail.count()==0){
                    QByteArray byteArrayRow;
                    int length=ofd.getRowLength();
                    //先做一个空行
                    for(int charIndex=0;charIndex<length;charIndex++){
                        byteArrayRow.append(QChar(' ').toLatin1());
                    }
                    //开始填充更新数据，按列更新
                    if(rowdata.count()>0){
                        //列循环/////////////////////////
                        for(int editCol=0;editCol<rowdata.count()&&editCol<colCount;editCol++){
                            //字段类型
                            QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
                            //字段长度
                            int fieldLength=ofd.getFieldList().at(editCol).getLength();
                            //字段小数长度
                            int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
                            //导入的值
                            QString valueNew=rowdata.at(editCol);
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
                                        byteArrayRow[i]=valueNewArray[index];
                                        index++;
                                    }
                                    //超过填写的参数的部分使用空格补充
                                    else{
                                        byteArrayRow[i]=' ';
                                    }
                                }
                            }
                            //数值型
                            else if(fieldType=="N"){
                                //空数据自动补充为空格，0自动补充为全0，不再强制将空补充为0
                                if(valueNew.isEmpty()){
                                    for(int i=updateBegin;i<updateEnd;i++){
                                        byteArrayRow[i]=codecOFD->fromUnicode(QString(" ")).at(0);
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
                                        byteArrayRow[i]=codecOFD->fromUnicode(number).at(index);
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
                                        byteArrayRow[i]=codecOFD->fromUnicode(number).at(index);
                                        index++;
                                    }
                                }
                            }
                            //其他未知类型
                            else{
                                int index=0;
                                for(int i=updateBegin;i<updateEnd;i++){
                                    if(index<valueNewArray.length()){
                                        byteArrayRow[i]=valueNewArray[index];
                                        index++;
                                    }
                                    //超过填写的参数的部分使用空格补充
                                    else{
                                        byteArrayRow[i]=' ';
                                    }
                                }
                            }
                        }
                    }
                    if(dataCompressLevel==0){
                        ofdFileContentQByteArrayListFromExcel.append(byteArrayRow);
                    }
                    else{
                        ofdFileContentQByteArrayListFromExcel.append(qCompress(byteArrayRow,dataCompressLevel));
                    }
                    //刷新UI
                    if (importExcelRow%100==0){
                        emit update_import_excel_status();
                    }
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
            DialogMyTip dialog2(importExcelErrorDetail.at(0),this,true);
            dialog2.setWindowTitle("导入失败警告！");
            dialog2.setModal(true);
            dialog2.exec();
            statusBar_disPlayMessage("导入失败,Excel列不符和当前文件规范...");
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
        int rowCount=ofdFileContentQByteArrayListFromExcel.count();
        if(rowCount==0){
            statusBar_disPlayMessage("导入的Excel有效数据行数为0...");
        }
        else{
            ofdFileContentQByteArrayList=ofdFileContentQByteArrayListFromExcel;
            //更新OFD记录数标记--在文件头的最后一行
            ofdFileHeaderQStringList.replace(ofdFileHeaderQStringList.count()-1,QString("%1").arg(ofdFileContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
            //新的总行数
            int rowCount=ofdFileContentQByteArrayList.count();
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
                display_OFDTable();
                acceptVScrollValueChanged(0);
                statusBar_display_rowsCount(rowCount);
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
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
                display_OFDTable();
                acceptVScrollValueChanged(0);
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
            }
            statusBar_disPlayMessage(QString("导入完毕,从Excel导入数据%1行").arg(rowCount));
        }
    }
}

/**
 * @brief MainWindow::update_import_excel_status 刷新UI
 */
void MainWindow::update_import_excel_status(){
    statusBar_disPlayMessage(QString("已分析数据%1行,请耐心等待...").arg(importExcelRow));
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
    qDebug()<<header;
    qDebug()<<footer;
    if(currentOpenFileType==openFileType::OFDFile){
        if(header.count()!=ofdFileHeaderQStringList.count()){
            statusBar_disPlayMessage("文件头读取异常,跳过更新文件头文件尾");
            return;
        }
        else{
            bool modify=false;
            if(header.count()>0){
                for (int i=0;i<header.count();i++){
                    if(0!=header.at(i).compare(ofdFileHeaderQStringList.at(i),Qt::CaseSensitive)){
                        ofdFileHeaderQStringList.replace(i,header.at(i));
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
