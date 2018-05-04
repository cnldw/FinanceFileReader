#include "mainwindow.h"
#include "ui_mainwindow.h"
#define UNUSED(x) (void)x

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
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
    //指向表格控件的指针
    ptr_table=ui->tableWidget;
    ptr_table->setAlternatingRowColors(true);
    tableHeight=ptr_table->height();
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    //初始化表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowDetails = new QAction(tr("查看此行记录"),this);
    connect(action_ShowDetails, SIGNAL(triggered()), this, SLOT(showRowDetails()));
    action_ShowCopyColum = new QAction(tr("复制此单元格"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    action_ShowAnalysis = new QAction(tr("字段合法分析"),this);
    connect(action_ShowAnalysis, SIGNAL(triggered()), this, SLOT(showFiledAnalysis()));
    action_EditCompareData= new QAction(tr("将此行数据加入比对器"),this);
    connect(action_EditCompareData, SIGNAL(triggered()), this, SLOT(editCompareData()));
    action_ModifyCell= new QAction(tr("编辑此单元格"),this);
    connect(action_ModifyCell, SIGNAL(triggered()), this, SLOT(showModifyCell()));
    //监控表格进度条的变化
    connect (ptr_table->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(acceptVScrollValueChanged(int)));
    //开始初始化状态栏
    initStatusBar();
    //列搜索信号连接,此处存在缺陷,如果搜索时表格获取了焦点，则回车无法连续搜索,暂不启用
    //connect(ui->lineTextText_2, SIGNAL(returnPressed()), this, SLOT(on_pushButtonNextSearch_2_clicked()));
    //开始进行配置加载
    load_CodeInfo();
    load_FileType();
    load_OFDDefinition();
    load_Dictionary();
    //配置加载完毕
    configLoadCompleted=true;
}

void MainWindow::initStatusBar(){
    ui->statusBar->setStyleSheet("font-family:Microsoft YaHei,Sans-serif;");
    statusLabel_ptr_showCount = new QLabel;
    statusLabel_ptr_showCount->setMinimumSize(140, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showCount);
    //设置标签内容
    statusLabel_ptr_showCount->setText(tr("记录数:"));

    statusLabel_ptr_showRowAndCol = new QLabel;
    statusLabel_ptr_showRowAndCol->setMinimumSize(200, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showRowAndCol);
    //设置标签内容
    statusLabel_ptr_showRowAndCol->setText(tr("文件内0行0列"));
    statusLabel_ptr_showRowAndCol->setToolTip(tr("此处显示当前选择的字段在原文件中的行和列"));

    statusLabel_ptr_showMessage = new QLabel;
    statusLabel_ptr_showMessage->setMinimumSize(495, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showMessage);
    //设置标签内容
    statusLabel_ptr_showMessage->setText(tr(""));
}

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
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
    }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    if(!configLoadCompleted){
        statusBar_disPlayMessage(tr("正在加载配置,稍后再拖入文件试试..."));
        return;
    }
    if(isUpdateData){
        statusBar_disPlayMessage(tr("有正在加载的任务,稍后再拖入文件试试..."));
        return;
    }
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.count()>1){
        statusBar_disPlayMessage(tr("拖进来一个文件试试~,文件太多啦"));
        return;
    }
    QString fileName = urls.first().toLocalFile();
    //判断是否是文件夹
    QFileInfo fileinfo(fileName);
    if(fileinfo.isDir()){
        statusBar_disPlayMessage(tr("拖进来一个文件试试~,不接受文件夹"));
        return;
    }
    if (!fileName.isEmpty())
    {
        currentOpenFilePath=fileName;
        ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
        isUpdateData=true;
        initFile();
        isUpdateData=false;
        return;
    }
}

void MainWindow:: resizeEvent (QResizeEvent * event ){
    event->ignore();
    if(tableHeight!=ptr_table->height()&&!isUpdateData&&currentOpenFileType!=0){
        //获取当前table的高度
        int higth=ptr_table->size().height();
        //窗口变大不会影响起始行
        hValueEnd=hValueBegin+(higth/rowHight);
        display_OFDTable();
    }
}

void MainWindow::acceptVScrollValueChanged(int value)
{
    //正在更新表/数据源时不开启自动加载
    if(!isUpdateData&&currentOpenFileType!=0){
        //新的起始位置
        hValueBegin=value;
        //获取当前table的高度
        int higth=ptr_table->size().height();
        //计算要结束的行
        hValueEnd=hValueBegin+(higth/rowHight);
        display_OFDTable();
    }
}

void MainWindow::statusBar_clear_statusBar(){
    statusLabel_ptr_showCount->setText(tr("记录数:0"));
    statusLabel_ptr_showRowAndCol->setText(tr("文件内0行0列"));
    statusLabel_ptr_showMessage->setText(NULL);
}

void MainWindow::open_file_Dialog(){
    QString openpath="./";
    if(!currentOpenFilePath.isEmpty()){
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
        }
    }
    QString   file = QFileDialog::getOpenFileName(this, tr("打开"),openpath , tr("所有文件(*.*)"));
    if(file.length() != 0) {
        currentOpenFilePath=file;
        ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
        isUpdateData=true;
        initFile();
        isUpdateData=false;
    } else {
        //放弃了读取文件
    }
}

void MainWindow::statusBar_display_rowsCount(int rowsCount){

    statusLabel_ptr_showCount->setText(tr("记录数:%1行").arg(QString::number(rowsCount, 10)));
}

void MainWindow::statusBar_display_rowsAndCol(int row,int col,int length){
    if(length==-1){
        statusLabel_ptr_showRowAndCol->setText(tr("源文件%1行").arg(QString::number(row)));
    }
    else{
        statusLabel_ptr_showRowAndCol->setText("源文件"+QString::number(row)+"行,"+QString::number(col)+"列,长度为"+QString::number(length));
    }
}

QString MainWindow::getConfigPath(){
    //为了更好的兼容各个操作系统,设立不同的配置文件目录规则
#ifdef Q_OS_MAC
    return QApplication::applicationDirPath() + "/../Resources/";
#endif

#ifdef Q_OS_LINUX
    return "./config/";
#endif

#ifdef Q_OS_WIN32
    return "./config/";
#endif
}

void MainWindow::clear_oldData(){
    indexFileHeaderMap.clear();
    indexFileDataList.clear();
    ofdFileHeaderQStringList.clear();
    ofdFileContentQByteArrayList.clear();
    rowHasloaded.clear();
    compareData.clear();
    //记录当前所在行
    rowcurrent=0;
    //当前所在列
    colcurrent=0;
    //更新列跳转搜索开始列
    colSearch=0;
}

void MainWindow::load_CodeInfo(){
    QString codeInipath=getConfigPath()+"CodeInfo.ini";
    if(Utils::isFileExist(codeInipath)){
        //加载ini文件
        QSettings loadedCodeInfoIni(codeInipath,QSettings::IniFormat,0);
        //目前仅接收UTF-8编码的配置文件
        loadedCodeInfoIni.setIniCodec("UTF-8");
        QStringList agencyInfo=loadedCodeInfoIni.childGroups();
        //获取所有代码和代码对应的机构名称
        for(int i=0;i<agencyInfo.count();i++){
            CodeInfo info;
            info.setCode(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString());
            info.setVersion(loadedCodeInfoIni.value(agencyInfo.at(i)+"/IVERSION").toString());
            info.setName(loadedCodeInfoIni.value(agencyInfo.at(i)+"/NAME").toString());
            loadedCodeInfo.insert(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString(),info);
        }
    }else{
        statusBar_disPlayMessage(getConfigPath()+tr("CodeInfo.ini配置丢失"));
    }
}

void MainWindow::load_FileType(){
    QString fileTypeInipath=getConfigPath()+"FileType.ini";
    if(Utils::isFileExist(fileTypeInipath)){
        //加载ini文件
        QSettings fileTypeIni(fileTypeInipath,QSettings::IniFormat,0);
        //目前仅接收UTF-8编码的配置文件
        fileTypeIni.setIniCodec("UTF-8");
        //读取INDEXFILE
        fileTypeIni.beginGroup("INDEXFILE");
        QStringList info =fileTypeIni.childKeys();
        //开始加载可用的索引文件类别
        if(info.count()>0){
            for(int i=0;i<info.count();i++){
                loadedIndexFileInfo.insert(info.at(i),fileTypeIni.value(info.at(i)).toString());
            }
        }
        fileTypeIni.endGroup();
        //读取DATAFILE
        fileTypeIni.beginGroup("DATAFILE");
        QStringList info2 =fileTypeIni.childKeys();
        //开始加载可用的索引文件类别
        if(info2.count()>0){
            for(int i=0;i<info2.count();i++){
                loadedOfdFileInfo.insert(info2.at(i),fileTypeIni.value(info2.at(i)).toString());
            }
        }
        fileTypeIni.endGroup();}
    else{
        statusBar_disPlayMessage(getConfigPath()+tr("FileType.ini配置丢失"));
    }
}

void MainWindow::load_Dictionary(){
    //读取配置使用UTF-8
    //字典解析直接按行解析即可,不再使用QSettings
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QFile dataFile(getConfigPath()+"Dictionary.ini");
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream data(&dataFile);
        QString line;
        while (!data.atEnd())
        {   QStringList lineList;
            line = data.readLine();
            line=line.remove('\r').remove('\n');
            dictionary.addDictionary(line);
        }
        dataFile.close();
    }
    return;
}

void MainWindow::load_OFDDefinition(){
    QDir dirPath(getConfigPath());
    QStringList filters;
    filters << "OFD_*.ini";//设置过滤类型
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList list = dirPath.entryInfoList();
    if(list.length()!=0){
        //循环检索config目录下可用的接口定义配置
        //OFD开头ini结尾,形式类似于OFD_XXX_XXX.ini
        /*-------------------------------开启第一重循环,遍历config下的OFD配置文件-----------------------------------*/
        for (int f = 0; f < list.size(); f++)
        {
            QString fileName=list.at(f).fileName();
            QString fixName=fileName;
            if(fileName.startsWith("OFD",Qt::CaseInsensitive)&&fileName.endsWith("ini",Qt::CaseInsensitive)){
                //编辑加载文件
                if(fixName.contains(".",Qt::CaseInsensitive)){
                    fixName=fixName.mid(0,fixName.indexOf("."));
                }
                QStringList nameList=fixName.split("_");
                if(nameList.count()==3){
                    //加载ini文件
                    QString prefixName=nameList.at(1)+"_"+nameList.at(2);
                    QSettings ofdIni(getConfigPath()+fileName,QSettings::IniFormat,0);
                    //目前仅接收UTF-8编码的配置文件
                    ofdIni.setIniCodec("UTF-8");
                    QStringList interfaceList=ofdIni.childGroups();
                    //获取所有文件类别定义
                    if(interfaceList.count()>0){
                        /*-----------------第二重循环，遍历OFD文件内的文件定义----------------------*/
                        /* /////////////////////////开始解析本文件可用接口文件定义////////// */
                        for(int i=0;i<interfaceList.count();i++){
                            //存入Qmap使用的key
                            QString name=prefixName+"_"+interfaceList.at(i);
                            OFDFileDefinition ofd;
                            QList<FieldDefinition> fieldList;
                            QString message;
                            bool okFlag=false;
                            //记录每行的长度，自动总数
                            int rowLength=0;
                            int filedCount=0;
                            //查找COUNT标记
                            QString countStr=ofdIni.value(interfaceList.at(i)+"/COUNT").toString();
                            if(countStr.isEmpty()){
                                message="找不到"+interfaceList.at(i)+"文件的字段总数配置";
                                okFlag=false;
                            }else{
                                bool ok;
                                int countInt=countStr.toInt(&ok,10);
                                if(ok&&countInt>0){
                                    filedCount=countInt;
                                    /*------第三重循环--开始按行解析此接口文件的定义----------------*/
                                    //解析规则-一旦解析到一个失败的记录，就不再往下解析了
                                    for(int j=1;j<=countInt;j++){
                                        FieldDefinition ofdfiled;
                                        //获取这个文件定义的第i个字段的信息
                                        QStringList iniStrList=ofdIni.value(interfaceList.at(i)+"/"+QString::number(j)).toStringList();
                                        if (iniStrList.isEmpty()||iniStrList.length()!=5){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段定义不正确或者缺失";
                                            okFlag=false;
                                            break;
                                        }
                                        //获取字符类型参数
                                        QString filedType=((QString)iniStrList.at(0));
                                        if(filedType.length()<1){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的类型定义缺失,请配置";
                                            okFlag=false;
                                            break;
                                        }
                                        //字段长度
                                        bool lengthOk;
                                        int length=((QString)iniStrList.at(1)).toInt(&lengthOk,10);
                                        if(!lengthOk){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的长度定义竟然不是整数";
                                            okFlag=false;
                                            break;
                                        }
                                        //字段小数长度
                                        bool declengthOk;
                                        int declength=((QString)iniStrList.at(2)).toInt(&declengthOk,10);
                                        if(!declengthOk){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的小数长度定义竟然不是整数";
                                            okFlag=false;
                                            break;
                                        }
                                        if(filedType=="N"&&(declength>=length)){
                                            message=interfaceList.at(i)+"文件的第"+QString::number(j)+"个字段的小数长度定义应该小于字段总长度";
                                            okFlag=false;
                                            break;
                                        }
                                        //历经千辛万苦经理校验无误后存入字段信息
                                        //每个字段开始的位置就是截至到上一个字段结束的长度
                                        //比如第一个字段长度为2,则第二个字段在此行记录中的起始坐标就是2（0,1这两个字节是第一个字段的索引）
                                        ofdfiled.setRowBeginIndex(rowLength);
                                        rowLength+=length;
                                        ofdfiled.setLength(length);
                                        ofdfiled.setDecLength(declength);
                                        ofdfiled.setFiledType(filedType);
                                        ofdfiled.setFiledName((QString)iniStrList.at(4));
                                        ofdfiled.setFiledDescribe((QString)iniStrList.at(3));
                                        fieldList.append(ofdfiled);
                                        //更新记录为成功
                                        okFlag=true;
                                    }
                                    //读取结束要关闭组
                                }
                                else{
                                    message=interfaceList.at(i)+"文件的字段总数配置不是有效的整数";
                                    okFlag=false;
                                }
                            }
                            //写入该文件的配置
                            //如果记录完整没错误则写入
                            if(okFlag){
                                ofd.setuseAble(okFlag);
                                ofd.setMessage(message);
                                ofd.setfieldCount(filedCount);
                                ofd.setrowLength(rowLength);
                                ofd.setfieldList(fieldList);
                                loadedOfdDefinitionMap.insert(name,ofd);
                            }
                            //如果记录有错误,则仅写于不可用和错误原因
                            else{
                                ofd.setuseAble(okFlag);
                                ofd.setMessage(message);
                                loadedOfdDefinitionMap.insert(name,ofd);
                            }
                        }
                    }
                }
            }}

    } else
    {
    }
}

void MainWindow::initFile(){
    //初始化文件时,检查配置是否加载完毕
    if(!configLoadCompleted){
        statusBar_disPlayMessage(tr("正在加载配置,稍后再拖入文件试试..."));
        return;
    }
    fileChanged=false;
    this->setWindowTitle(appName);
    //首先清除原来的显示信息
    clear_oldData();
    clear_Display_Info();
    clear_Table_Info();
    //获取完整的文件名
    int first = currentOpenFilePath.lastIndexOf ("/");
    QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
    if(fileName.length()<10){
        statusBar_disPlayMessage("无法识别的文件类别,请检查");
        return;
    }
    else{
        //数据文件
        if(fileName.startsWith("OFD",Qt::CaseInsensitive)){
            //开始拆解文件名
            QString fixName=fileName;
            if(fixName.contains(".",Qt::CaseInsensitive)){
                fixName=fixName.mid(0,fixName.indexOf("."));
            }
            //OFD文件名处理完毕后，开始拆解文件名
            QStringList nameList=fixName.split("_");
            //正常的OFD文件应该至少有5段信息组成,中登TA和管理人交互的文件还有批次号
            if(nameList.count()<5){
                statusBar_disPlayMessage("错误的文件名,参考:OFD_XX_XXX_YYYYMMDD_XX.TXT");
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
                //con从配置文件获取名称信息
                QString sendName=(loadedCodeInfo.value(sendCode)).getName();
                QString recName=(loadedCodeInfo.value(recCode)).getName();
                QString fileTypeName=loadedOfdFileInfo.value(fileTypeCode);
                //刷新UI
                ui->lineEditSendCode->setText(sendCode);
                ui->lineEditRecCode->setText(recCode);
                ui->lineEditFileTransferDate->setText(dateInfo);
                ui->lineEditFileType->setText(fileTypeCode);
                ui->lineEditSenfInfo->setText(sendName);
                ui->lineEditRecInfo->setText(recName);
                ui->lineEditFileDescribe->setText(fileTypeName);
                //记录从文件里读取的文件发送信息
                //此处开始加载OFD数据文件
                load_ofdFile(sendCode,fileTypeCode);
                return;
            }
        }else{
            //开始解析非OFD文件
            QString nameBegin=fileName.left(3);
            if(!loadedIndexFileInfo.value(nameBegin).isEmpty()){
                //检索到了文件头标识
                //开始拆解文件名
                QString fixName=fileName;
                if(fixName.contains(".",Qt::CaseInsensitive)){
                    fixName=fixName.mid(0,fixName.indexOf("."));
                }
                //索引文件名处理完毕后，开始拆解文件名
                QStringList nameList=fixName.split("_");
                //正常的OFD索引文件应该有5段信息组成
                if(nameList.count()!=4){
                    statusBar_disPlayMessage("无法识别的非OFD文件,请检查"+getConfigPath()+"FileType.ini配置");
                    return;
                }
                else{
                    /*开始初步分析文件*/
                    //从文件头获取各种信息
                    //Code
                    QString sendCode=nameList.at(1);
                    QString recCode=nameList.at(2);
                    QString dateInfo=nameList.at(3);
                    QString fileIndexTypeCode=nameBegin;
                    //名称信息
                    QString sendName=loadedCodeInfo.value(sendCode).getName();
                    QString recName=loadedCodeInfo.value(recCode).getName();
                    QString fileIndexTypeName=loadedIndexFileInfo.value(fileIndexTypeCode);
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
            }else{
                //没有检索到满足的文件头标识
                statusBar_disPlayMessage("无法识别的文件类别,请检查"+getConfigPath()+"FileType.ini配置");
                return;
            }
        }
    }
}

void MainWindow::load_indexFile(){
    currentOpenFileType=0;
    QFile dataFile(currentOpenFilePath);
    //开放式基金交换协议使用GB18030编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream data(&dataFile);
        QString line;
        int lineNumber=0;
        while (!data.atEnd())
        {   QStringList lineList;
            line = data.readLine();
            line=line.remove('\r').remove('\n');
            //文件体
            if(lineNumber>5){
                lineList.append(line);
                indexFileDataList.append(lineList);
            }
            //文件头部
            else{
                switch (lineNumber) {
                case 0:
                    //文件第一行OF标记
                    indexFileHeaderMap.insert("filebegin",line);
                    break;
                case 1:
                    //文件第二行版本
                    indexFileHeaderMap.insert("version",line);
                    break;
                case 2:
                    //文件第三行发送方代码
                    indexFileHeaderMap.insert("sendcode",line);
                    break;
                case 3:
                    //文件第四行接收方代码
                    indexFileHeaderMap.insert("recivecode",line);
                    break;
                case 4:
                    //文件第五行文件传递日期
                    indexFileHeaderMap.insert("filedate",line);
                    break;
                case 5:
                    //文件第6行文件记录数
                    indexFileHeaderMap.insert("count",line);
                    break;
                default:
                    break;
                }
            }
            lineNumber++;
        }
        //处理最后一行
        QString lastLine=indexFileDataList.last().at(0);
        if(lastLine.startsWith("OFDCFEND")){
            indexFileHeaderMap.insert("fileend",lastLine);
            indexFileDataList.removeLast();
        }
        dataFile.close();
    }
    init_display_IndexTable();
}

void MainWindow::load_ofdFile(QString sendCode,QString fileType){
    currentOpenFileType=1;
    QString defineMapName;
    QString versionName;
    CodeInfo info=loadedCodeInfo.value(sendCode);
    if(info.getCode().isEmpty()){
        versionName="400";
    }else{
        versionName=(info.getVersion().isEmpty()?"400":info.getVersion());
    }
    //读取文件结束位置
    int lineEnd=9;
    int countNumberFromFile=0;
    QString versionFromFile="";
    QStringList filedNameFromFile;
    QFile dataFile(currentOpenFilePath);
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream data(&dataFile);
        QString line;
        int lineNumber=0;
        while (!data.atEnd())
        {   QStringList lineList;
            line = data.readLine();
            line=line.remove('\r').remove('\n').trimmed();
            //文件体
            lineList.append(line);
            //文件的第二行是文件版本
            if(lineNumber==1){
                versionFromFile=line.trimmed();
                //获取不到版本号，退出
                if(versionFromFile.isEmpty()){
                    statusBar_disPlayMessage("解析失败,未从文件第2行读取到OFD文件的版本号信息");
                    return;
                }
            }
            //文件的第10行记录了该文件有多少个字段
            if(lineNumber==9){
                QString count=line;
                bool flag=false;
                countNumberFromFile=count.toInt(&flag);
                if(!flag)
                {
                    statusBar_disPlayMessage("解析失败,从文件第10行读取接口字段总数失败,请检查文件");
                    return;
                }else{
                    //如果读取记录数成功,则延长结束行,读取文件的字段记录
                    lineEnd+=countNumberFromFile;
                }
            }
            //从第11行开始,为字段名字
            if(lineNumber>9){
                filedNameFromFile.append(line);
            }
            lineNumber++;
            //如果到达了终止点就跳出
            if(lineNumber>lineEnd){
                break;
            }
        }
        dataFile.close();
        defineMapName=versionName+"_"+versionFromFile+"_"+fileType;
        QString useini="OFD_"+versionName+"_"+versionFromFile+".ini";
        ui->lineEditUseIni->setText(useini);
        ui->lineEditUseIni->setToolTip(NULL);
        //判断对应的配置文件是否存在
        QString path=getConfigPath()+useini;
        if(Utils::isFileExist(path)){
            ofd=loadedOfdDefinitionMap.value(defineMapName);
            if(loadedOfdDefinitionMap.contains(defineMapName)) {
                if(ofd.getuseAble()){
                    if(filedNameFromFile.count()!=countNumberFromFile){
                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n从解析的文件第10行获取到的文件字段数为["+QString::number( countNumberFromFile)+"],但是读取字段时，确只读取到了["+QString::number(filedNameFromFile.count())+"]个,请检查文件是否完整",QMessageBox::Ok,QMessageBox::Ok);
                        return;
                    }
                    //判断文件字段数是否和定义的一致
                    if(ofd.getfieldCount()!=countNumberFromFile){
                        //中登TA21版本04文件兼容20版本
                        //特殊逻辑
                        ///////////////////////////////////////////////////////////////
                        if(fileType=="04" && versionFromFile=="21"){
                            int filedCount21=ofd.getfieldCount();
                            defineMapName=versionName+"_20_"+fileType;
                            useini="OFD_"+versionName+"_20.ini(20兼容)";
                            ofd=loadedOfdDefinitionMap.value(defineMapName);
                            if(ofd.getuseAble()){
                                //兼容模式也无法解析
                                if(filedNameFromFile.count()!=countNumberFromFile){
                                    QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n无法解析这个04文件,请检查文件是否完整正确",QMessageBox::Ok,QMessageBox::Ok);
                                    return;
                                }
                                else{
                                    //检查字段总数是否和定义一致
                                    if(ofd.getfieldCount()!=countNumberFromFile){
                                        QMessageBox::information(this,tr("提示"),QString("重要提示\r\n\r\n无法解析这个04文件,请检查文件是否符合接口定义,此文件目前即不满足20版本的%1个字段的要求,也不满足21版本的%2个字段的要求").arg(ofd.getfieldCount()).arg(filedCount21),QMessageBox::Ok,QMessageBox::Ok);
                                        return;
                                    }
                                    else{
                                        //确认使用兼容解析
                                        ui->lineEditUseIni->setText(useini);
                                        ui->lineEditUseIni->setToolTip("20兼容表示这个文件里的版本号不是20版本\r\n但是文件内容实际和20版本字段一致");
                                    }
                                }
                            }
                            else{
                                QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n缺失20版本的04文件p配置,无法尝试使用兼容模式解析04,请检查配置",QMessageBox::Ok,QMessageBox::Ok);
                                return;
                            }
                        }
                        /////////////////////////////////////////////////////////////////
                        //通用提示
                        else{
                            QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n解析文件失败,配置文件"+useini+"中记录的"+fileType+"文件有["+QString::number(ofd.getfieldCount())+"]个字段，但是从文件第10行获取到的文件字段数为["+QString::number( countNumberFromFile)+"],请检查文件是否满足接口标准,或者配置是否有误",QMessageBox::Ok,QMessageBox::Ok);
                            return;
                        }
                    }
                    //遍历比对字段是否一致
                    for(int ff=0;ff<filedNameFromFile.count();ff++){
                        if(((QString)filedNameFromFile.at(ff)).toUpper()!=ofd.getfieldList().at(ff).getFiledName().toUpper()){
                            QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n解析文件失败,配置文件"+useini+"中记录的"+fileType+"文件第["+QString::number(ff)+"]个字段是["+ofd.getfieldList().at(ff).getFiledName().toUpper()+"],但是从文件第["+QString::number(11+ff)+"]行获取到的字段是["+((QString)filedNameFromFile.at(ff)).toUpper()+"] 字段名(忽略大小写)不一致,请检查文件是否满足接口标准,或者配置是否有误",QMessageBox::Ok,QMessageBox::Ok);
                            return;
                        }
                    }
                    //关键,此句强制将toLocal8Bit()函数转换为GB18030编码的字符数组
                    //如果不加次定义,默认取系统编码，因此在英文系统下读取可能会有问题
                    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
                    QFile dataFile(currentOpenFilePath);
                    //判断如果文件打开成功,则开始读取
                    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
                    {
                        QTextStream data(&dataFile);
                        int lineNumber=0;
                        //数据行开始的位置
                        int beginIndex=11+ofd.getfieldCount();
                        QString line;
                        bool sucessFalg=true;
                        QApplication::setOverrideCursor(Qt::WaitCursor);
                        while (!data.atEnd())
                        {
                            //如果此行记录小于数据开始行,则认为是文件头
                            if(lineNumber<beginIndex){
                                line = data.readLine();
                                line=line.remove('\r').remove('\n');
                                ofdFileHeaderQStringList.append(line);
                            }
                            //数据行
                            else{
                                line = data.readLine();
                                line=line.remove('\r').remove('\n');
                                //获取本行的QByteArray
                                QByteArray qbyteArrayRow=line.toLocal8Bit();
                                //本行记录和接口约束的一致,存入
                                if(qbyteArrayRow.size()==ofd.getrowLength()){
                                    ofdFileContentQByteArrayList.append(qbyteArrayRow);
                                }else{
                                    //如果行长度为8且内容为OFDCFEND跳过
                                    if(line.length()==8&&QString::compare(line,"OFDCFEND",Qt::CaseInsensitive)==0){
                                        break;
                                    }else{
                                        sucessFalg=false;
                                        statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getrowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"]行长度为["+QString::number(qbyteArrayRow.size())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                        break;
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
                        if(sucessFalg){
                            statusBar_disPlayMessage("读取到数据行"+QString::number(ofdFileContentQByteArrayList.count())+"行");
                            init_OFDTable();
                        }
                    }else{
                        statusBar_disPlayMessage("解析失败:文件读取失败,请重试...");
                        return;
                    }
                }else{
                    statusBar_disPlayMessage("解析失败:配置文件"+path+"中"+fileType+"文件的定义存在错误"+ofd.getMessage());
                    return;
                }
            }else{
                statusBar_disPlayMessage("解析失败:未在"+path+"配置中找到"+fileType+"文件的定义,请配置...");
                return;
            }
        }else{
            statusBar_disPlayMessage("解析失败,配置文件"+path+"不存在...");
            return;
        }
    }
}

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
            for(int col=0;col<colCount;col++){
                QTableWidgetItem *item= new QTableWidgetItem(indexFileDataList.at(row).at(col));
                ptr_table->setItem(row, col, item);
            }
        }
        ptr_table->resizeColumnsToContents();
        statusBar_display_rowsCount(rowCount);
    }
    else
    {
        statusBar_disPlayMessage(tr("没有数据可供显示~"));
    }
}

void MainWindow::init_OFDTable(){
    if(ofd.getuseAble()){
        int colCount=ofd.getfieldCount();
        int rowCount=ofdFileContentQByteArrayList.count();
        ptr_table->setColumnCount(colCount);
        ptr_table->setRowCount(rowCount);
        //设置表格列标题
        QStringList title;
        for(int i=0;i<colCount;i++){
            //仅获取列的中文备注当作列标题
            title.append(ofd.getfieldList().at(i).getFiledDescribe());
        }
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
            hValueBegin=0;
            hValueEnd=hValueBegin+(higth/rowHight);
            display_OFDTable();
        }
        statusBar_display_rowsCount(rowCount);
        statusBar_disPlayMessage("文件解析完毕!成功读取记录"+QString::number(rowCount)+"行");
    }
    else
    {
        statusBar_disPlayMessage(tr("空的数据记录,没有数据可供显示..."));
    }
}

void MainWindow::display_OFDTable(){
    int rowCount=ptr_table->rowCount();
    int colCount=ptr_table->columnCount();
    //防止渲染边界超过表总行数
    if(hValueBegin<0){
        hValueBegin=0;
    }
    if(hValueEnd>=rowCount){
        hValueEnd=rowCount-1;
    }
    for (int row = hValueBegin; row <= hValueEnd; ++row)
    {
        //如果此行已经加载过了,则不再加载
        if(rowHasloaded.contains(row)){
            continue;
        }else{
            rowHasloaded.append(row);
            for(int col=0;col<colCount;col++){
                QString values=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col);
                //仅对数据非空单元格赋值
                if(!values.isEmpty()){
                    QTableWidgetItem *item= new QTableWidgetItem();
                    ptr_table->setItem(row, col, item);
                    item->setText(values);
                }
            }
        }
    }
    ptr_table->resizeColumnsToContents();
}

void MainWindow::clear_Table_Info(){
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    ptr_table->setColumnCount(0);
}

void MainWindow::statusBar_disPlayMessage(QString text){
    statusLabel_ptr_showMessage->setText(text);
}

void MainWindow::clear_Display_Info(){
    ui->lineEditSendCode->setText(NULL);
    ui->lineEditRecCode->setText(NULL);
    ui->lineEditFileTransferDate->setText(NULL);
    ui->lineEditFileType->setText(NULL);
    ui->lineEditSenfInfo->setText(NULL);
    ui->lineEditRecInfo->setText(NULL);
    ui->lineEditFileDescribe->setText(NULL);
    ui->lineEditUseIni->setText(NULL);
    ui->lineEditUseIni->setToolTip(NULL);
    statusBar_clear_statusBar();
}

void MainWindow::on_fileOpen_triggered()
{
    open_file_Dialog();
}

void MainWindow::on_aboutOpen_triggered()
{
    QMessageBox::about(this,tr("关于本程序"),tr("本程序是一个可以解析格式化显示开放式基金数据交换协议文件的工具\r\n可以用于解析如上所述协议所交换的各种文件\r\n作      者:793554262@qq.com(刘德位)\r\n商业支持:793554262@qq.com(刘德位)\r\n友情赞助:15238872101(支付宝)\r\n\r\n版本:")+Utils::getVersion());
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,tr("关于QT"));
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    open_file_Dialog();
}

void MainWindow::on_pushButtonOpenFile_2_clicked()
{
    if(currentOpenFileType==0){
        if(!indexFileHeaderMap.isEmpty()){
            //组织要显示的内容
            QString info;
            info.append("文件解析情况如下:\r\n");
            info.append("文件发送者代码:").append(indexFileHeaderMap.value("sendcode")).append("\r\n");
            info.append("文件接收者代码:").append(indexFileHeaderMap.value("recivecode")).append("\r\n");
            info.append("文件传递日期:").append(indexFileHeaderMap.value("filedate")).append("\r\n");
            info.append("文件内标识的总记录数:").append(indexFileHeaderMap.value("count")).append("成功加载记录数:").append(QString::number(indexFileDataList.count())).append("\r\n");
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
            int  filedCount=ofdFileHeaderQStringList.at(9).toInt(&ok);
            if(ok){
                int countRow=10+filedCount;
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
    //***********未实现的数据类型插入点
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    UNUSED(previousRow);
    UNUSED(previousColumn);
    if(!isUpdateData){
        if(currentOpenFileType==0){
            statusBar_display_rowsAndCol(currentRow+5,0,-1);
        }
        else if(currentOpenFileType==1){
            //记录当前所在行
            rowcurrent=currentRow;
            //当前所在列
            colcurrent=currentColumn;
            //更新列跳转搜索开始列
            colSearch=currentColumn;
            int rowInFile=12+ofd.getfieldCount()+currentRow;
            int colInFile=ofd.getfieldList().at(currentColumn).getRowBeginIndex()+1;
            statusBar_display_rowsAndCol(rowInFile,colInFile,ofd.getfieldList().at(currentColumn).getLength());
            if(ptr_table->item(currentRow,currentColumn)!=nullptr){
                QString text=ptr_table->item(currentRow,currentColumn)->text();
                QString dic=dictionary.getDictionary(ofd.getfieldList().at(currentColumn).getFiledName(),text);
                if(text.isEmpty()){
                    statusBar_disPlayMessage(ofd.getfieldList().at(currentColumn).getFiledDescribe().append("/").append(ofd.getfieldList().at(currentColumn).getFiledName()).append("|").append(ofd.getfieldList().at(currentColumn).getFiledType()));
                }
                else{
                    statusBar_disPlayMessage(ofd.getfieldList().at(currentColumn).getFiledDescribe().append("/").append(ofd.getfieldList().at(currentColumn).getFiledName()).append("|").append(ofd.getfieldList().at(currentColumn).getFiledType()).append("|").append(text).append(dic.isEmpty()?"":("|"+dic)));
                }
            }
            else{
                statusBar_disPlayMessage(ofd.getfieldList().at(currentColumn).getFiledDescribe().append("/").append(ofd.getfieldList().at(currentColumn).getFiledName()).append("|").append(ofd.getfieldList().at(currentColumn).getFiledType()));
            }
        }
        //***********未实现的数据类型插入点
    }
}

void MainWindow::copyToClipboard(){
    if(ptr_table->itemAt(posCurrentMenu)!=nullptr){
        QString text= ptr_table->itemAt(posCurrentMenu)->text();
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
        statusBar_disPlayMessage(QString("复制数据:\"%1\"").arg(text));
    }
}
void MainWindow::editCompareData(){
    if(compareData.value(rowcurrent+1).isEmpty()){
        //数据加入
        QStringList rowdata;
        for(int i=0;i<(ptr_table->columnCount());i++){
            QString col="";
            if(ptr_table->item(rowcurrent,i)!=nullptr){
                col=ptr_table->item(rowcurrent,i)->text();
            }
            rowdata.append(col);
        }
        compareData.insert(rowcurrent+1,rowdata);
        statusBar_disPlayMessage(QString("比对列表已加入%1行数据").arg(compareData.count()));
    }
    else{
        //移除数据行
        compareData.remove(rowcurrent+1);
        if(compareData.count()>0){
            statusBar_disPlayMessage(QString("比对列表还有%1行数据").arg(compareData.count()));
        }
        else{
            statusBar_disPlayMessage(QString("比对列表已清空"));
        }
    }
}

void MainWindow::showRowDetails(){
    //行
    int row=ptr_table->rowAt(posCurrentMenu.y());
    int colCount=ptr_table->columnCount();
    statusBar_disPlayMessage(QString("查看第%1行数据").arg(row+1));
    //定义一个Qlist存储此行的数据,将表格的列转换为行，共计四列
    //数据内容从表格取，从原始数据取还需要转换
    QList<QStringList> rowdata;
    for(int i=0;i<colCount;i++){
        QStringList colitem;
        if(ptr_table->item(row,i)==nullptr){
            //字段中文名
            colitem.append(ofd.getfieldList().at(i).getFiledDescribe());
            //字段英文名
            colitem.append(ofd.getfieldList().at(i).getFiledName());
            //字段值
            colitem.append(NULL);
            //字典翻译
            colitem.append(NULL);
        }
        else{
            QString colvalue=ptr_table->item(row,i)->text();
            //字段名
            colitem.append(ofd.getfieldList().at(i).getFiledDescribe());
            colitem.append(ofd.getfieldList().at(i).getFiledName());
            //字段值
            colitem.append(colvalue);
            //字典翻译
            colitem.append(dictionary.getDictionary(ofd.getfieldList().at(i).getFiledName(),colvalue));
        }
        rowdata.append(colitem);
    }
    //打开窗口
    DialogShowTableRow * dialog = new DialogShowTableRow(&rowdata,this);
    dialog->setWindowTitle(QString("查看表格行记录-第%1行").arg(row+1));
    dialog->setModal(false);
    dialog->show();
}

void MainWindow:: showFiledAnalysis(){
    if(currentOpenFileType==1){
        statusBar_disPlayMessage(QString("分析第%1行第%2列数据数据").arg(rowcurrent+1).arg(colcurrent+1));
        //字段中文名
        QString filedDes=ofd.getfieldList().at(colcurrent).getFiledDescribe();
        //字段英文名
        QString filedName=ofd.getfieldList().at(colcurrent).getFiledName();
        //字段类型
        QString filedType=ofd.getfieldList().at(colcurrent).getFiledType();
        //字段长度
        int filedLength=ofd.getfieldList().at(colcurrent).getLength();
        //字段小数长度
        int filedDecLength=ofd.getfieldList().at(colcurrent).getDecLength();
        //字段原始值
        QString filedOaiginal=Utils::getOriginalValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowcurrent,colcurrent);
        //字段翻译值
        QString filedValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,rowcurrent,colcurrent);
        QList<QStringList> data;
        //开始存储数据
        /////////////////////////////
        QStringList rowfiledDes;
        rowfiledDes.append("字段定义中文名");
        rowfiledDes.append(filedDes);
        data.append(rowfiledDes);
        /////////////////////////////
        QStringList rowfiledName;
        rowfiledName.append("字段定义英文名");
        rowfiledName.append(filedName);
        data.append(rowfiledName);
        /////////////////////////////
        QStringList rowfiledType;
        rowfiledType.append("字段定义类型");
        QString type="";
        if(filedType=="C"){
            type=filedType+"(字符型)";
        }
        else if(filedType=="A"){
            type=filedType+"(数字字符型)";
        }
        else if(filedType=="N"){
            type=filedType+"(数值型)";
        }
        else if(filedType=="TEXT"){
            type=filedType+"(不定长文本)";
        }
        rowfiledType.append(type);
        data.append(rowfiledType);
        /////////////////////////////
        QStringList rowfiledLength;
        rowfiledLength.append("字段定义总长度");
        rowfiledLength.append(QString::number(filedLength));
        data.append(rowfiledLength);
        /////////////////////////////
        QStringList rowfiledDecLength;
        rowfiledDecLength.append("字段定义小数长度");
        rowfiledDecLength.append(QString::number(filedDecLength));
        data.append(rowfiledDecLength);
        /////////////////////////////
        QStringList rowfiledOaiginal;
        rowfiledOaiginal.append("字段原始值");
        rowfiledOaiginal.append(filedOaiginal);
        data.append(rowfiledOaiginal);
        /////////////////////////////
        QStringList rowfiledValues;
        rowfiledValues.append("字段解析值");
        rowfiledValues.append(filedValues);
        data.append(rowfiledValues);
        /////////////合法校验/////////
        QStringList rowfiledCheck;
        rowfiledCheck.append("字段值合法性");
        if(filedType=="C"){
            //字符型,万能接收
            rowfiledCheck.append("字段值符合接口约束");
        }
        else if(filedType=="A"){
            //数字字符型
            /*
        1:不能包含空格数字字母外的任何值
        2:不能以空格开头
        */
            if(filedOaiginal.trimmed().isEmpty()){
                rowfiledCheck.append("字段值符合接口约束");
            }
            else if(filedOaiginal.at(0)==' '){
                rowfiledCheck.append("数字字符型字段不建议以空格开头");
            }
            else{
                QString textTrimed=filedOaiginal.trimmed();
                bool ok=true;
                for(int i=0;i<textTrimed.length();i++){
                    if(((int)textTrimed.at(i).toLatin1())<48||(((int)textTrimed.at(i).toLatin1())>57&&((int)textTrimed.at(i).toLatin1())<65)||(((int)textTrimed.at(i).toLatin1())>90&&((int)textTrimed.at(i).toLatin1())<97)||((int)textTrimed.at(i).toLatin1())>122){
                        ok=false;
                        break;
                    }
                }
                if(ok){
                    rowfiledCheck.append("字段值符合接口约束");
                }
                else{
                    rowfiledCheck.append("数字字符型字段不建议包含0-9,a-z,A-Z外的其他字符");
                }
            }
        }
        else if(filedType=="N"){
            //数字型
            /*
        1:不能包含数字外的任何值
        */
            //如果一个N类型的字段合法，那么肯定可以转换成一个数字
            if(filedOaiginal.trimmed().isEmpty()){
                rowfiledCheck.append("数值型字段不建议使用空格填充,部分厂商系统不支持,空值建议填充0");
            }
            else if(filedOaiginal.contains(" ")){
                rowfiledCheck.append("数值型字段不建议使用空格填充,部分厂商系统不支持,长度补位建议填充0");
            }else{
                bool ok;
                QString(filedOaiginal).toLongLong(&ok);
                if(ok){
                    rowfiledCheck.append("字段值符合接口约束");
                }
                else{
                    rowfiledCheck.append("字段内容错误,本字段不应该包含0-9外的字符");
                }
            }
        }
        else if(filedType=="TEXT"){
            rowfiledCheck.append("字段值符合接口约束");
        }
        else{
            rowfiledCheck.append("未知类型字段,无法分析");
        }
        data.append(rowfiledCheck);
        //打开窗口
        DialogShowTableFiledCheck * dialog = new DialogShowTableFiledCheck(&data,this);
        dialog->setWindowTitle(QString("分析第%1行第%2列数据数据").arg(rowcurrent+1).arg(colcurrent+1));
        dialog->setModal(false);
        dialog->show();
    }
    //***********未实现的数据类型插入点
}

void MainWindow::showModifyCell(){
    //标记要编辑的位置
    int editRow=rowcurrent;
    int editCol=colcurrent;
    //字段类型
    QString filedType=ofd.getfieldList().at(editCol).getFiledType();
    //字段长度
    int filedLength=ofd.getfieldList().at(editCol).getLength();
    //字段小数长度
    int filedDecLength=ofd.getfieldList().at(editCol).getDecLength();
    //获取字段目前的值
    QString filedValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,editRow,editCol);
    //字段修改标记
    bool modifyFlag=false;
    //修改后的值
    QString valueNew="";
    //打开窗口
    DialogModifyCell * dialog = new DialogModifyCell(filedType,filedLength,filedDecLength,filedValues,this);
    dialog->setWindowTitle(QString("编辑第%1行第%2列-"+ofd.getfieldList().at(colcurrent).getFiledDescribe()).arg(rowcurrent+1).arg(colcurrent+1));
    dialog->setModal(true);
    dialog->exec();
    //从弹窗中获取结果
    modifyFlag=dialog->getModifyFlag();
    valueNew=dialog->getValueNew();
    //开始处理是否需要更新
    if(modifyFlag){
        //检验新值是否和旧值相等,如果相等认为没有编辑
        if(valueNew==filedValues){
            statusBar_disPlayMessage("单元格值没有变化,无需保存...");
        }
        else{
            //更新原始记录//////////////////////////////////////////////////////
            //要更新的范围
            int updateBegin=ofd.getfieldList().at(colcurrent).getRowBeginIndex();
            int updateEnd=updateBegin+filedLength;
            //新的单元格值的字节数组
            QByteArray valueNewArray=valueNew.toLocal8Bit();
            //本行记录原始内容
            QByteArray valueNewArrayRow=ofdFileContentQByteArrayList.at(rowcurrent);
            //判断数据类型处理
            //数值字符型,字符型,长文本型对于长度不够的情况直接补充空格即可
            if(filedType=="C"||filedType=="TEXT"||filedType=="A"){
                int index=0;
                for(int i=updateBegin;i<updateEnd;i++){
                    if(index<valueNewArray.length()){
                        valueNewArrayRow[i]=valueNewArray[index];
                        index++;
                    }
                    //超过填写的参数的部分使用空格补充
                    else{
                        valueNewArrayRow[i]=QString(" ").toLocal8Bit().at(0);
                    }
                }
            }
            //数值型
            else if(filedType=="N"){
                //全空数据自动补充0
                if(valueNew.isEmpty()){
                    for(int i=updateBegin;i<updateEnd;i++){
                        valueNewArrayRow[i]=QString("0").toLocal8Bit().at(0);
                    }
                }
                //仅包含整数部分
                else if(!valueNew.contains("."))
                {
                    //合成整数和小数部分
                    int zLength=filedLength-filedDecLength;
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
                    if(intD.length()<filedDecLength){
                        int zeroAdd=filedDecLength-intD.length();
                        for(int zz=0;zz<zeroAdd;zz++){
                            intD.append('0');
                        }
                    }
                    //整数和小数补充缺少的0结束后开始填充数据
                    QString number=intS+intD;
                    int index=0;
                    for(int i=updateBegin;i<updateEnd;i++){
                        valueNewArrayRow[i]=number.toLocal8Bit().at(index);
                        index++;
                    }
                }
                //整数部分和小数部分都有值
                else{
                    //分别获取整数和小数
                    int zLength=filedLength-filedDecLength;
                    QString intS=valueNew.mid(0,valueNew.indexOf("."));
                    if(intS.length()<zLength){
                        int zeroAdd=zLength-intS.length();
                        for(int zz=0;zz<zeroAdd;zz++){
                            intS.insert(0,'0');
                        }
                    }
                    //小数部分后补0
                    QString intD=valueNew.mid(valueNew.indexOf(".")+1,-1);
                    if(intD.length()<filedDecLength){
                        int zeroAdd=filedDecLength-intD.length();
                        for(int zz=0;zz<zeroAdd;zz++){
                            intD.append('0');
                        }
                    }
                    //整数和小数补充缺少的0结束后开始填充数据
                    QString number=intS+intD;
                    int index=0;
                    for(int i=updateBegin;i<updateEnd;i++){
                        valueNewArrayRow[i]=number.toLocal8Bit().at(index);
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
                        valueNewArrayRow[i]=QString(" ").toLocal8Bit().at(0);
                    }
                }
            }
            //将新的行记录写入原ofdFileContentQByteArrayList/////////////////////////////
            ofdFileContentQByteArrayList.replace(rowcurrent,valueNewArrayRow);
            //更新界面/////////////////////////////////////////////////////////////////
            if(ptr_table->item(rowcurrent,colcurrent)!=nullptr){
                ptr_table->item(rowcurrent,colcurrent)->setText(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,editRow,editCol));
            }
            //如果这个单元格未填充过数据,则QTableWidgetItem不存在
            else if(!valueNew.isEmpty()){
                QTableWidgetItem *item= new QTableWidgetItem();
                ptr_table->setItem(rowcurrent, colcurrent, item);
                item->setText(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,editRow,editCol));
            }
            //重新校准列宽
            ptr_table->resizeColumnsToContents();
            //提示用户保存//////////////////////////////////////////////////////////////
            statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
            this->setWindowTitle(appName+"-修改待保存");
            //更新文件修改标记//////////////////////////////////////////////////////////
            fileChanged=true;
        }
    }
    else{
        statusBar_disPlayMessage("取消编辑...");
    }
}

void MainWindow::on_pushButtonPreSearch_clicked()
{
    //向上搜索
    if(ui->lineTextText->text().isEmpty()){
        statusBar_disPlayMessage("请填写你要搜索的内容...");
        return;
    }
    if(rowcurrent==0&&colcurrent==0){
        statusBar_disPlayMessage("再往上没有你要搜索的内容了...");
        return;
    }
    if(ofdFileContentQByteArrayList.count()<1){
        return;
    }
    //开始搜索
    statusBar_disPlayMessage("正在搜索,请耐心等待...");
    QString searchText=ui->lineTextText->text();
    //判断是否是当前搜索行,如果是则从焦点单元格前一个单元格搜索,否则从行记录的最后一个单元格搜索
    int colCount=ofd.getfieldCount();
    //搜索当前行时,不能从最后一列开始,要从焦点单元格前一个开始
    int beginCol=colcurrent-1;
    for(int row=rowcurrent;row>=0;row--){
        for(int col=beginCol;col>=0;col--){
            if(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col).contains(searchText,Qt::CaseInsensitive)){
                statusBar_disPlayMessage("在"+QString::number(row+1)+"行,"+QString::number(col+1)+"列找到你搜索的内容");
                ptr_table->setCurrentCell(row,col);
                ptr_table->setFocus();
                return;
            }
        }
        beginCol=colCount-1;
    }
    statusBar_disPlayMessage("再往上没有你要搜索的内容了...");
}

void MainWindow::on_pushButtonNextSearch_clicked()
{

    int rowcount=ofdFileContentQByteArrayList.count();
    int colCount=ofd.getfieldCount();
    //向下搜索
    if(ui->lineTextText->text().isEmpty()){
        statusBar_disPlayMessage("请填写你要搜索的内容...");
        return;
    }
    if(rowcurrent==rowcount-1&&colcurrent==colCount-1){
        statusBar_disPlayMessage("再往下没有你要搜索的内容了...");
        return;
    }
    if(ofdFileContentQByteArrayList.count()<1){
        return;
    }
    //开始搜索
    statusBar_disPlayMessage("正在搜索,请耐心等待...");
    QString searchText=ui->lineTextText->text();
    //是否是当前搜索行,如果是则从焦点单元格后一个单元格搜索,否则从行记录的第一个单元格搜索
    int beginCol=colcurrent+1;
    for(int row=rowcurrent;row<rowcount;row++){
        for(int col=beginCol;col<colCount;col++){
            if(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col).contains(searchText,Qt::CaseInsensitive)){
                statusBar_disPlayMessage("在"+QString::number(row+1)+"行,"+QString::number(col+1)+"列找到你搜索的内容");
                ptr_table->setCurrentCell(row,col);
                ptr_table->setFocus();
                return;
            }
        }
        beginCol=0;
    }
    statusBar_disPlayMessage("再往下没有你要搜索的内容了...");
}

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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
    //OFD数据可以打开行详情
    tablePopMenu->clear();
    //当打开的是OFD文件时,菜单里添加行预览和显示行详情和编辑的菜单
    if(currentOpenFileType==1){
        tablePopMenu->addAction(action_ShowDetails);
        tablePopMenu->addAction(action_ShowAnalysis);
        tablePopMenu->addAction(action_ModifyCell);
    }
    //***********未实现的数据类型插入点
    tablePopMenu->addAction(action_ShowCopyColum);
    //当打开的是OFD文件时,菜单里添加比对信息功能
    if(currentOpenFileType==1){
        if(compareData.value(ptr_table->rowAt(pos.y())+1).isEmpty()){
            action_EditCompareData->setText("将此行数据加入比对器");
        }
        else{
            action_EditCompareData->setText("将此行数据从比对器移除");
        }
        tablePopMenu->addAction(action_EditCompareData);
    }
    //***********未实现的数据类型插入点
    tablePopMenu->exec(QCursor::pos());
}

void MainWindow::on_pushButtonNextSearch_2_clicked()
{
    QString text=ui->lineTextText_2->text();
    if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
        bool returnSearch=false;
        int begin=colSearch;
        if((colSearch+1)>=(ptr_table->columnCount()-1)){
            colSearch=-1;
        }
        colSearch+=1;
        for(;colSearch<ptr_table->columnCount();colSearch++){
            //兼容字段中英文名的搜索
            if(ofd.getfieldList().at(colSearch).getFiledDescribe().contains(text,Qt::CaseInsensitive)||ofd.getfieldList().at(colSearch).getFiledName().contains(text,Qt::CaseInsensitive)){
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
                statusBar_disPlayMessage("臣妾找不到你要搜索的列...");
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

void MainWindow::on_actionsOpenCompare_triggered()
{
    if(currentOpenFileType==0){
        if(ptr_table->rowCount()<1){
            statusBar_disPlayMessage("没有需要比对的数据行...");
        }
        else{
            statusBar_disPlayMessage("索引文件不支持使用比对器...");
        }
    }
    else if(currentOpenFileType==1){
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
            title.append("数据行号");
            for(int i=0;i<ptr_table->columnCount();i++){
                //仅获取列的中文备注当作列标题
                title.append(ofd.getfieldList().at(i).getFiledDescribe());
            }
            //打开窗口
            if(title.count()>0){
                DialogShowTableCompareView * dialog = new DialogShowTableCompareView(title,&compareData,this);
                dialog->setWindowTitle(QString("行比对器视图"));
                dialog->setModal(false);
                dialog->show();
            }
        }
    }
    //***********未实现的数据类型插入点
}

void MainWindow::on_actionClearCompare_triggered()
{
    compareData.clear();
    statusBar_disPlayMessage("比对器内容清除完毕...");
}

void MainWindow::on_pushButtonNextSearch_3_clicked()
{
    //预先数据判断
    if(ptr_table->rowCount()<1){
        statusBar_disPlayMessage("没有数据可供导出,目前表格有0行数据");
        return;
    }
    //数据类型判断
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引数据不支持导出");
    }
    else if(currentOpenFileType==1){
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
        QString fileNameSave = QFileDialog::getSaveFileName(this,("文件数据导出"),openpath+filename,tr("Excel文件(*.xlsx);;Csv文件(*.csv);;Html文件(*.html)"),&selectedFilter);
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
                    statusBar_disPlayMessage("覆盖文件失败,源文件可能正在被占用...");
                    return;
                }
            }
            //根据文件类型来判断
            if(fileNameSave.endsWith("xlsx",Qt::CaseSensitive)){
                save2Xlsx(fileNameSave);
            }
            else if(fileNameSave.endsWith("csv",Qt::CaseSensitive)){
                save2Csv(fileNameSave);
            }
            else if(fileNameSave.endsWith("html",Qt::CaseSensitive)){
                save2Html(fileNameSave);
            }else{
                statusBar_disPlayMessage("暂不支持的导出类型");
            }
        }
    }
}

//保存为csv
void MainWindow::save2Csv(QString filename){
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile data(filename);
    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&data);
        //开始准备待写入的数据
        QString sb;
        //标题
        for(int i=0;i<ofd.getfieldCount();i++){
            if(i<ofd.getfieldCount()-1){
                sb.append(ofd.getfieldList().at(i).getFiledDescribe()).append("\t");
            }
            else{
                sb.append(ofd.getfieldList().at(i).getFiledDescribe()).append("\r\n");
            }
        }
        out<<sb;
        //文本内容
        sb.clear();
        for (int row=0;row<ofdFileContentQByteArrayList.count();row++){
            //数据写入--按行读取
            for(int col=0;col<ofd.getfieldCount();col++){
                if(col<ofd.getfieldCount()-1){
                    sb.append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col)).append("\t");
                }
                else{
                    sb.append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col)).append("\r\n");
                }
            }
            //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
            //仅1000行或者到最后一行时进行写入
            if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                out<<sb;
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
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
}

//保存为html
void MainWindow::save2Html (QString filename){
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile data(filename);
    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&data);
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
        sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei(793554262@qq.com)-->\r\n<head>\r\n<meta charset=\"GB18030\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
        //内联的表格样式,内容太多,base64存储
        sb.append(QByteArray::fromBase64("PHN0eWxlIHR5cGU9InRleHQvY3NzIj4KLnRhYmxlCnsKcGFkZGluZzogMDsKbWFyZ2luOiAwOwp9CnRoIHsKZm9udDogYm9sZCAxMnB4ICJUcmVidWNoZXQgTVMiLCBWZXJkYW5hLCBBcmlhbCwgSGVsdmV0aWNhLCBzYW5zLXNlcmlmOwpjb2xvcjogIzRmNmI3MjsKYm9yZGVyLXJpZ2h0OiAxcHggc29saWQgI0MxREFENzsKYm9yZGVyLWJvdHRvbTogMXB4IHNvbGlkICNDMURBRDc7CmJvcmRlci10b3A6IDFweCBzb2xpZCAjQzFEQUQ3OwpsZXR0ZXItc3BhY2luZzogMnB4Owp0ZXh0LXRyYW5zZm9ybTogdXBwZXJjYXNlOwp0ZXh0LWFsaWduOiBsZWZ0OwpwYWRkaW5nOiA2cHggNnB4IDZweCAxMnB4OwpiYWNrZ3JvdW5kOiAjQ0FFOEVBIG5vLXJlcGVhdDsKd29yZC1icmVhazoga2VlcC1hbGw7CndoaXRlLXNwYWNlOm5vd3JhcDsKfQp0ciB7CndvcmQtYnJlYWs6IGtlZXAtYWxsOwp3aGl0ZS1zcGFjZTpub3dyYXA7Cn0KdGQgewpib3JkZXItcmlnaHQ6IDFweCBzb2xpZCAjQzFEQUQ3Owpib3JkZXItYm90dG9tOiAxcHggc29saWQgI0MxREFENzsKZm9udC1zaXplOjE0cHg7CnBhZGRpbmc6IDJweCA2cHggMnB4IDZweDsKY29sb3I6ICM0ZjZiNzI7Cn0KPC9zdHlsZT4K"));
        //标题表头
        sb.append("<table>\r\n<tr>");
        for(int i=0;i<ofd.getfieldCount();i++){
            if(i<ofd.getfieldCount()-1){
                sb.append("<th>").append(ofd.getfieldList().at(i).getFiledDescribe()).append("</th>");
            }
            else{
                sb.append("<th>").append(ofd.getfieldList().at(i).getFiledDescribe()).append("</th></tr>\r\n");
            }
        }
        //输出表头
        out<<sb;
        //文本内容
        sb.clear();
        for (int row=0;row<ofdFileContentQByteArrayList.count();row++){
            //数据写入--按行读取
            sb.append("<tr>");
            for(int col=0;col<ofd.getfieldCount();col++){
                if(col<ofd.getfieldCount()-1){
                    sb.append("<td>").append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col)).append("</td>");
                }
                else{
                    sb.append("<td>").append(Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col)).append("</td></tr>\r\n");
                }
            }
            //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
            //仅1000行或者到最后一行时进行写入
            if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                out<<sb;
                sb.clear();
                statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                qApp->processEvents();
            }
        }
        //补充html剩余部分
        sb.append("</table>\r\n</body>\r\n</html>");
        out<<sb;
        data.close();
        statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
    }else{
        statusBar_disPlayMessage(tr("数据导出失败,请重试"));
    }
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
}

//保存为xlsx
void MainWindow::save2Xlsx(QString filename){

    //禁止导出过大的excel文件
    if(ofdFileContentQByteArrayList.count()>200000){
        statusBar_disPlayMessage("记录数大于20万行,无法使用导出到excel,请导出csv或者html(如有需求导出到excel请联系793554262@qq.com)");
        return;
    }
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QXlsx::Document xlsx;
    //标题的样式
    QXlsx::Format formatTitle;
    formatTitle.setFont(QFont("SimSun"));
    formatTitle.setFontBold(true);
    formatTitle.setFontColor(QColor(Qt::black));
    formatTitle.setPatternBackgroundColor(QColor(0,176,80));
    formatTitle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    //用来记录列最宽的list
    int colWidthArray[ofd.getfieldCount()];
    //标题和是否是数值列,数值列需要设置列格式
    QMap<int,QXlsx::Format> numberFormat;
    for(int i=0;i<ofd.getfieldCount();i++){
        xlsx.write(1,i+1,ofd.getfieldList().at(i).getFiledDescribe(),formatTitle);
        //记录每列的宽度
        colWidthArray[i]=(ofd.getfieldList().at(i).getFiledDescribe().toLocal8Bit().length()+4);
        if(ofd.getfieldList().at(i).getFiledType()=="N"){
            QXlsx::Format formatNumber;
            formatNumber.setFont(QFont("SimSun"));
            formatNumber.setFontBold(false);
            formatNumber.setFontColor(QColor(Qt::black));
            //构造数值长度
            QString zj="#";
            QString z="0";
            QString xj="";
            QString x="";

            int lengthx=ofd.getfieldList().at(i).getDecLength();
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
            numberFormat.insert(i,formatNumber);
        }
    }
    //文本样式
    QXlsx::Format formatBody;
    formatBody.setFont(QFont("SimSun"));
    formatBody.setFontBold(false);
    formatBody.setFontColor(QColor(Qt::black));
    //文本内容
    for (int row=0;row<ofdFileContentQByteArrayList.count();row++){
        //数据写入--按行读取
        for(int col=0;col<ofd.getfieldCount();col++){
            QString value=Utils::getFormatValuesFromofdFileContentQByteArrayList(&ofdFileContentQByteArrayList,&ofd,row,col);
            //空的单元格直接忽略
            if(value.isEmpty()){
                continue;
            }
            //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
            int widthNew=value.toLocal8Bit().length()+4;
            if(widthNew>colWidthArray[col]){
                colWidthArray[col]=widthNew;
            }
            //判断数据类型
            if(ofd.getfieldList().at(col).getFiledType()=="N"){
                //对于数值型的数据如果接口文档里给的确实是数值型,则填充到excel时也用数值型
                if(numberFormat.contains(col)){
                    bool okNb=false;
                    double number=value.toDouble(&okNb);
                    if(okNb){
                        xlsx.write(row+2,col+1,number,numberFormat.value(col));
                    }
                    else{
                        xlsx.write(row+2,col+1,value,numberFormat.value(col));
                    }
                }
                else{
                    xlsx.write(row+2,col+1,value,formatBody);
                }
            }else{
                xlsx.write(row+2,col+1,value,formatBody);
            }
        }
        //每100行读取下事件循环
        //excel导出较慢
        if(row%100==0){
            statusBar_disPlayMessage(QString("文件导出中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
            qApp->processEvents();
        }
        if(row==ofdFileContentQByteArrayList.count()-1){
            statusBar_disPlayMessage(QString("数据产生完毕,正在写入文件,请勿进行其他操作"));
            qApp->processEvents();
        }
    }
    //根据每列最大的值设置本列的宽度
    for(int i=0;i<ofd.getfieldCount();i++){
        xlsx.setColumnWidth(i+1,i+1,colWidthArray[i]+0.0);
    }
    //保存文件
    xlsx.saveAs(filename);
    statusBar_disPlayMessage(tr("数据成功导出到%1").arg(filename));
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_pushButtonNextSearch_4_clicked()
{
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
    else if(lineNumber>ptr_table->rowCount()){
        statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
    }
    else{
        ptr_table->setCurrentCell(lineNumber-1,colcurrent);
        ptr_table->setFocus();
    }
}

void MainWindow::on_actionsss_triggered()
{
    //加载数据中时忽略刷新
    if(isUpdateData){
        return;
    }
    if(!currentOpenFilePath.isEmpty()){
        isUpdateData=true;
        initFile();
        isUpdateData=false;
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
    }
    else if(currentOpenFileType==1){
        if(fileChanged){
            //先备份原文件
            if(Utils::isFileExist(currentOpenFilePath)){
                QFile oldfile(currentOpenFilePath);
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyyMMdd-hhmmss");
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
}

void MainWindow::saveOFDFile(QString filepath)
{
    //检查文件是否存在,存在则覆盖
    if(Utils::isFileExist(filepath)){
        QFile oldfile(filepath);
        bool r=oldfile.remove();
        if(!r){
            statusBar_disPlayMessage("覆盖文件失败,请重试或检查文件是否被其他程序占用");
            return;
        }
    }
    //开始执行文件写入
    QFile newfile(filepath);
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (newfile.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&newfile);
        //开始准备待写入的数据
        QString sb;
        //写入文件头///////////////////////////////////////////////////////////////////
        for(int i=0;i<ofdFileHeaderQStringList.length();i++){
            sb.append(ofdFileHeaderQStringList.at(i)).append("\r\n");
        }
        out<<sb;
        sb.clear();
        //写入文件体//////////////////////////////////////////////////////////////////
        int row=0;
        while(row<ofdFileContentQByteArrayList.length()){
            //按行写入数据
            sb.append(QString::fromLocal8Bit(ofdFileContentQByteArrayList.at(row))).append("\r\n");
            //为了降低磁盘写入频率,每1000行写入一次,写入后清空sb
            //仅1000行或者到最后一行时进行写入
            if((row%1000==0)||(row==ofdFileContentQByteArrayList.count()-1)){
                out<<sb;
                sb.clear();
                statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                qApp->processEvents();
            }
            row++;
        }
        //写入文件结束标志位//////////////////////////////////////////////////////////////////
        out<<"OFDCFEND\r\n";
        newfile.close();
        fileChanged=false;
        statusBar_disPlayMessage(tr("文件保存完毕,保存在%1").arg(filepath));
        this->setWindowTitle(tr("基金文件阅读器-")+Utils::getVersion());
    }else{
        statusBar_disPlayMessage(tr("数据保存失败,请重试"));
    }
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
}


void MainWindow::on_actionSaveAS_triggered()
{
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(currentOpenFileType==0){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
        return;
    }
    //文件过滤器,用于追踪选择的保存文件类别
    QString selectedFilter=Q_NULLPTR;
    //弹出保存框
    QString fileNameSave = QFileDialog::getSaveFileName(this,("另存为"),currentOpenFilePath,tr("OFD文本文件(*.TXT)"),&selectedFilter);
    if(!fileNameSave.isEmpty()){
        //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
        if(selectedFilter=="OFD文本文件(*.TXT)"&&(!fileNameSave.endsWith(".TXT"))){
            fileNameSave.append(".TXT");
        }
        //执行文件另存
        saveOFDFile(fileNameSave);
    }
}

void MainWindow::on_actionaboutAuthor_triggered()
{
    DialogAboutAuthor * dialog = new DialogAboutAuthor(this);
    dialog->setModal(false);
    dialog->show();
}
