#include "mainwindow.h"
#include "ui_mainwindow.h"
#define UNUSED(x) (void)x

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle(tr("基金文件阅读器-")+Utils::getVersion());
    //调教字体大小差异,在linux系统上字体默认显示很大
#ifdef Q_OS_MAC
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
    statusLabel_ptr_showCount->setMinimumSize(130, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showCount);
    //设置标签内容
    statusLabel_ptr_showCount->setText(tr("记录数:"));

    statusLabel_ptr_showFileName = new QLabel;
    statusLabel_ptr_showFileName->setMinimumSize(220, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showFileName);
    //设置标签内容
    statusLabel_ptr_showFileName->setText(tr("文件:"));

    statusLabel_ptr_showRowAndCol = new QLabel;
    statusLabel_ptr_showRowAndCol->setMinimumSize(170, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showRowAndCol);
    //设置标签内容
    statusLabel_ptr_showRowAndCol->setText(tr("文件内0行0列"));
    statusLabel_ptr_showRowAndCol->setToolTip(tr("此处显示当前选择的字段在原文件中的行和列"));

    statusLabel_ptr_showMessage = new QLabel;
    statusLabel_ptr_showMessage->setMinimumSize(305, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabel_ptr_showMessage);
    //设置标签内容
    statusLabel_ptr_showMessage->setText(tr(""));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete statusLabel_ptr_showCount;
    statusLabel_ptr_showCount=nullptr;
    delete statusLabel_ptr_showFileName;
    statusLabel_ptr_showFileName=nullptr;
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
    statusLabel_ptr_showFileName->setText(tr("文件:"));
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

    statusLabel_ptr_showCount->setText(tr("记录数:")+QString::number(rowsCount, 10)+tr("行"));
}

void MainWindow::statusBar_display_rowsAndCol(int row,int col,int length){
    if(length==-1){
        statusLabel_ptr_showRowAndCol->setText("源文件"+QString::number(row)+"行");
    }
    else{
        statusLabel_ptr_showRowAndCol->setText("源文件"+QString::number(row)+"行,"+QString::number(col)+"列,长度为"+QString::number(length));
    }
}

void MainWindow::clear_oldData(){
    indexFileHeaderMap.clear();
    indexFileDataList.clear();
    ofdFileHeaderQStringList.clear();
    ofdFileContentQByteArrayList.clear();
    rowHasloaded.clear();
    //记录当前所在行
    rowcurrent=0;
    //当前所在列
    colcurrent=0;
    //更新列跳转搜索开始列
    colSearch=0;
}

void MainWindow::load_CodeInfo(){
    QString codeInipath="./config/CodeInfo.ini";
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
        statusBar_disPlayMessage(tr("config/CodeInfo.ini配置丢失"));
    }
}

void MainWindow::load_FileType(){
    QString fileTypeInipath="./config/FileType.ini";
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
        statusBar_disPlayMessage(tr("config/FileType.ini配置丢失"));
    }
}

void MainWindow::load_Dictionary(){
    //读取配置使用UTF-8
    //字典解析直接按行解析即可,不再使用QSettings
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QFile dataFile("./config/Dictionary.ini");
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
    QDir dirPath("./config/");
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
                    QSettings ofdIni("./config/"+fileName,QSettings::IniFormat,0);
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

void MainWindow::statusBar_display_fileName(){
    int first = currentOpenFilePath.lastIndexOf ("/");
    QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
    statusLabel_ptr_showFileName->setText(tr("文件：")+fileName);
}

void MainWindow::initFile(){
    //初始化文件时,检查配置是否加载完毕
    if(!configLoadCompleted){
        statusBar_disPlayMessage(tr("正在加载配置,稍后再拖入文件试试..."));
        return;
    }
    //首先清除原来的显示信息
    clear_oldData();
    clear_Display_Info();
    clear_Table_Info();
    //获取完整的文件名
    int first = currentOpenFilePath.lastIndexOf ("/");
    QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
    //状态栏显示文件名
    statusBar_display_fileName();
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
            //正常的OFD文件应该有5段信息组成
            if(nameList.count()!=5){
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
                //正常的OFD文件应该有5段信息组成
                if(nameList.count()!=4){
                    statusBar_disPlayMessage("无法识别的非OFD文件,请检查config/FileType.ini配置");
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
                statusBar_disPlayMessage("无法识别的文件类别,请检查config/FileType.ini配置");
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
                versionFromFile=line;
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
        //判断对应的配置文件是否存在
        QString path="config/"+useini;
        if(Utils::isFileExist(path)){
            ofd=loadedOfdDefinitionMap.value(defineMapName);
            if(loadedOfdDefinitionMap.contains(defineMapName)) {
                if(ofd.getuseAble()){
                    if(filedNameFromFile.count()!=countNumberFromFile){
                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n从解析的文件第10行获取到的文件字段数为["+QString::number( countNumberFromFile)+"],但是读取字段时，确只读取到了["+QString::number(filedNameFromFile.count())+"]个,请检查文件是否完整",QMessageBox::Ok,QMessageBox::Ok);
                        return;
                    }
                    if(ofd.getfieldCount()!=countNumberFromFile){
                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n解析文件失败,配置文件"+useini+"中记录的"+fileType+"文件有["+QString::number(ofd.getfieldCount())+"]个字段，但是从文件第10行获取到的文件字段数为["+QString::number( countNumberFromFile)+"],请检查文件是否满足接口标准,或者配置是否有误",QMessageBox::Ok,QMessageBox::Ok);
                        return;
                    }
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
                    statusBar_disPlayMessage("解析失败:配置文件"+path+"中"+ofd.getMessage());
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
                QString values=getValuesFromofdFileContentQByteArrayList(row,col);
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

QString MainWindow::getValuesFromofdFileContentQByteArrayList(int row ,int col){
    //判断越界
    if(row>=ofdFileContentQByteArrayList.count()||col>=ofd.getfieldCount()){
        return "";
    }
    //字段类型
    QString fileType=ofd.getfieldList().at(col).getFiledType();
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd.getfieldList().at(col).getLength();
    //小数长度
    int filedDeclength=ofd.getfieldList().at(col).getDecLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(ofdFileContentQByteArrayList.at(row).mid(ofd.getfieldList().at(col).getRowBeginIndex(),filedlength));
    //数据信息处理
    if(fileType=="C"){
        //C类型从右去除多余空格
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    else if(fileType=="A"){
        //A类型从右移除多余空格,移除空格后剩余的应该是0-9的数值
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    else if(fileType=="N"){
        //N类型为双精度数字
        int needCheck=filedlength-filedDeclength-1;
        int needCutZero=0;
        for(int s=0;s<needCheck;s++){
            if(filed.at(s)=='0'){
                needCutZero++;
            }
            else{
                break;
            }
        }
        //获取整数
        QString left=filed.left(filedlength-filedDeclength).remove(0,needCutZero);
        //获取小数--如果小数长度为0,就不必处理小数了
        if(filedDeclength==0){
            filed=left;
        }else{
            //获取小数
            QString right=filed.right(filedDeclength);
            //拼接整数部分和小数部分
            filed=left.append(".").append(right);
        }
    }
    else if(fileType=="TEXT"){
        //TEXT类型从右去除多余空格
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            //遇到第一个不是空格的字段就立即退出循环
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    return filed;
}

QString MainWindow::getOriginalValuesFromofdFileContentQByteArrayList(int row, int col){
    //判断越界
    if(row>=ofdFileContentQByteArrayList.count()||col>=ofd.getfieldCount()){
        return "";
    }
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd.getfieldList().at(col).getLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(ofdFileContentQByteArrayList.at(row).mid(ofd.getfieldList().at(col).getRowBeginIndex(),filedlength));
    return filed;
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
    else{
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
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    UNUSED(previousRow);
    UNUSED(previousColumn);
    if(currentOpenFileType==1&&!isUpdateData){
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
                statusBar_disPlayMessage(ofd.getfieldList().at(currentColumn).getFiledDescribe().append("|").append(ofd.getfieldList().at(currentColumn).getFiledType()));
            }
            else{
                statusBar_disPlayMessage(ofd.getfieldList().at(currentColumn).getFiledDescribe().append("|").append(ofd.getfieldList().at(currentColumn).getFiledType()).append("|").append(text).append(dic.isEmpty()?"":("|"+dic)));
            }
        }
        else{
            statusBar_disPlayMessage(ofd.getfieldList().at(currentColumn).getFiledDescribe().append("|").append(ofd.getfieldList().at(currentColumn).getFiledType()));
        }
    }
    if(currentOpenFileType==0&&!isUpdateData){
        statusBar_display_rowsAndCol(currentRow+5,0,-1);
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
    QString filedOaiginal=getOriginalValuesFromofdFileContentQByteArrayList(rowcurrent,colcurrent);
    //字段翻译值
    QString filedValues=getValuesFromofdFileContentQByteArrayList(rowcurrent,colcurrent);
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
    rowfiledValues.append("字段解释值");
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
        1:不能包含空格数字外的任何值
        2:不能以数字外的类型开头
        */
        if(filedOaiginal.trimmed().isEmpty()){
            rowfiledCheck.append("字段值符合接口约束");
        }
        else if(filedOaiginal.at(0)==' '){
            rowfiledCheck.append("数字字符型字段不建议以空格开头");
        }
        else{
            bool ok;
            QString(filedOaiginal.trimmed()).toLongLong(&ok);
            if(ok){
                rowfiledCheck.append("字段值符合接口约束");
            }
            else{
                rowfiledCheck.append("数字字符型字段不建议包含0-9外的其他字符(空格结尾除外)");
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
            if(getValuesFromofdFileContentQByteArrayList(row,col).contains(searchText,Qt::CaseInsensitive)){
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
            if(getValuesFromofdFileContentQByteArrayList(row,col).contains(searchText,Qt::CaseInsensitive)){
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
    if(currentOpenFileType==1){
        tablePopMenu->addAction(action_ShowDetails);
        tablePopMenu->addAction(action_ShowAnalysis);
    }
    tablePopMenu->addAction(action_ShowCopyColum);
    tablePopMenu->exec(QCursor::pos());
}

void MainWindow::on_pushButtonNextSearch_2_clicked()
{
    QString text=ui->lineTextText_2->text();
    if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
        if((colSearch+1)>=(ptr_table->columnCount()-1)){
            colSearch=-1;
        }
        colSearch+=1;
        for(;colSearch<ptr_table->columnCount();colSearch++){
            if((ptr_table->horizontalHeaderItem(colSearch)->text()).contains(text,Qt::CaseInsensitive)){
                ptr_table->setCurrentCell(rowcurrent,colSearch);
                ptr_table->setFocus();
                return;
            }
        }
    }
}
