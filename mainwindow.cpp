#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle("基金文件阅读器-"+Utils::getVersion());
    //指向表格控件的指针
    ptr_table=ui->tableWidget;
    tableHeight=ptr_table->height();
    //监控表格进度条的变化
    connect (ptr_table->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(acceptVScrollValueChanged(int)));
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

    load_CodeInfo();
    load_FileType();
    load_OFDDefinition();
    //配置加载完毕
    loadCompleted=true;
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
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    if(!loadCompleted){
        return;
        statusBar_disPlayMessage("有正在加载的任务,稍后再拖入文件试试...");
    }
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()){
        return;
    }
    if(urls.count()>1){
        statusBar_disPlayMessage("拖进来一个文件试试~,文件太多啦");
        return;
    }
    QString fileName = urls.first().toLocalFile();
    //判断是否是文件夹
    QFileInfo fileinfo(fileName);
    if(fileinfo.isDir()){
        statusBar_disPlayMessage("拖进来一个文件试试~,不接受文件夹");
        return;
    }
    if (fileName.isEmpty())
    {        return;
    }
    else{
        currentOpenFilePath=fileName;
        ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
        statusBar_display_fileName(currentOpenFilePath);
        initFile(currentOpenFilePath);
    }
}

//监控窗口大小变化
void MainWindow:: resizeEvent (QResizeEvent * event ){
    event->ignore();
    if(tableHeight!=ptr_table->height()){
        //获取当前table的高度
        int higth=ptr_table->size().height();
        int hValueEnd=hValueBegin+(higth/22);
        display_OFDTable(hValueBegin,hValueEnd);
    }
}

//滚动条变化信息竖向
//通知显示指定行范围内的数据
void MainWindow::acceptVScrollValueChanged(int value)
{
    hValueBegin=value;
    //获取当前table的高度
    int higth=ptr_table->size().height();
    int hValueEnd=hValueBegin+(higth/22);
    display_OFDTable(hValueBegin,hValueEnd);
}

/**
  *清除状态栏的信息
 * @brief MainWindow::statusBar_clear_statusBar
 */
void MainWindow::statusBar_clear_statusBar(){
    statusLabel_ptr_showCount->setText(tr("记录数:"));
    statusLabel_ptr_showFileName->setText(tr("文件:"));
    statusLabel_ptr_showRowAndCol->setText(tr("文件内0行0列"));
    statusLabel_ptr_showMessage->setText(NULL);
}

/**
  *打开文件窗口
 * @brief MainWindow::open_file_Dialog
 */
void MainWindow::open_file_Dialog(){
    QString   file = QFileDialog::getOpenFileName(this, tr("打开"), ".", tr("所有文件(*.*)"));
    if(file.length() != 0) {
        currentOpenFilePath=file;
        ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
        statusBar_display_fileName(currentOpenFilePath);
        initFile(currentOpenFilePath);
    } else {
        //放弃了读取文件
    }
}
/**
  *显示加载的记录总数
 * @brief MainWindow::statusBar_display_rowsCount
 * @param rowsCount
 */
void MainWindow::statusBar_display_rowsCount(int rowsCount){

    statusLabel_ptr_showCount->setText(tr("记录数:")+QString::number(rowsCount, 10)+tr("行"));
}

void MainWindow::statusBar_display_rowsAndCol(int row,int col,int length){
    statusLabel_ptr_showRowAndCol->setText("文件内"+QString::number(row)+"行,"+QString::number(col)+"列,长度为"+QString::number(length));
}

void MainWindow::clear_oldData(){
    indexFileHeaderMap.clear();
    indexFileDataList.clear();
    ofdFileHeaderQStringList.clear();
    ofdFileContentQByteArrayList.clear();
}
void MainWindow::load_CodeInfo(){
    QString currentOpenFilePath="./config/CodeInfo.ini";
    if(Utils::isFileExist(currentOpenFilePath)){
        //加载ini文件
        QSettings loadedCodeInfoIni(currentOpenFilePath,QSettings::IniFormat,0);
        //目前仅接收UTF-8编码的配置文件
        loadedCodeInfoIni.setIniCodec("UTF-8");
        QStringList agencyInfo=loadedCodeInfoIni.childGroups();
        //获取所有代码和代码对应的机构名称
        for(int i=0;i<agencyInfo.count();i++){
            CodeInfo info;
            //qDebug()<<loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString();
            info.setCode(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString());
            info.setVersion(loadedCodeInfoIni.value(agencyInfo.at(i)+"/IVERSION").toString());
            info.setName(loadedCodeInfoIni.value(agencyInfo.at(i)+"/NAME").toString());
            loadedCodeInfo.insert(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString(),info);
        }
        //debug信息
        QString info=QString("共计加载代码数据条数:").append(QString::number(agencyInfo.count()));
        qDebug()<<info;
    }else{
        statusBar_disPlayMessage(tr("config/CodeInfo.ini配置丢失"));
    }
}

void MainWindow::load_FileType(){
    QString currentOpenFilePath="./config/FileType.ini";
    if(Utils::isFileExist(currentOpenFilePath)){
        //加载ini文件
        QSettings fileTypeIni(currentOpenFilePath,QSettings::IniFormat,0);
        //目前仅接收UTF-8编码的配置文件
        fileTypeIni.setIniCodec("UTF-8");
        //读取INDEXFILE
        fileTypeIni.beginGroup("INDEXFILE");
        QStringList info =fileTypeIni.childKeys();
        //开始加载可用的索引文件类别
        if(info.count()>0){
            for(int i=0;i<info.count();i++){
                loadedIndexFileInfo.insert(info.at(i),fileTypeIni.value(info.at(i)).toString());
                //  qDebug()<<info.at(i)<<fileTypeIni.value(info.at(i)).toString();
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
                // qDebug()<<info2.at(i)<<fileTypeIni.value(info2.at(i)).toString();
            }
        }
        fileTypeIni.endGroup();}
    else{
        statusBar_disPlayMessage(tr("config/FileType.ini配置丢失"));
    }
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
                    qDebug()<<"可用接口文件配置"<<fileName;
                    //加载ini文件
                    QString prefixName=nameList.at(1)+"_"+nameList.at(2);
                    qDebug()<<prefixName;
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
                            //qDebug()<<"待解析的文件记录------"+name;
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
        qDebug()<<"no file";
    }
}
/**
 * 用于加载文件后在状态栏显示加载的文件
 * @brief MainWindow::statusBar_display_fileName
 * @param currentOpenFilePath
 */
void MainWindow::statusBar_display_fileName(QString currentOpenFilePath){
    int first = currentOpenFilePath.lastIndexOf ("/");
    QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
    statusLabel_ptr_showFileName->setText(tr("文件：")+fileName);
}

void MainWindow::initFile(QString currentOpenFilePath){
    //开始加载文件时,设置加载状态为未完成,避免这个文件还没加载,就拖入了新的文件
    loadCompleted=false;
    //首先清除原来的显示信息
    clear_Display_Table_Info();
    //清除上次打开文件加载的数据信息
    clear_oldData();
    //首先获取完整的文件名
    int first = currentOpenFilePath.lastIndexOf ("/");
    QString fileName = currentOpenFilePath.right (currentOpenFilePath.length ()-first-1);
    //状态栏显示文件名
    statusBar_display_fileName(fileName);
    if(fileName.length()<10){
        statusBar_disPlayMessage("无法识别的文件类别,请检查");
        loadCompleted=true;
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
            //qDebug()<<fixName;
            //OFD文件名处理完毕后，开始拆解文件名
            QStringList nameList=fixName.split("_");
            //正常的OFD文件应该有5段信息组成
            if(nameList.count()!=5){
                statusBar_disPlayMessage("错误的文件名,参考:OFD_XX_XXX_YYYYMMDD_XX.TXT");
                loadCompleted=true;
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
                load_ofdFile(sendCode,fileTypeCode,currentOpenFilePath);
                loadCompleted=true;
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
                //qDebug()<<fixName;
                //索引文件名处理完毕后，开始拆解文件名
                QStringList nameList=fixName.split("_");
                //正常的OFD文件应该有5段信息组成
                if(nameList.count()!=4){
                    statusBar_disPlayMessage("无法识别的非OFD文件,请检查config/FileType.ini配置");
                    loadCompleted=true;
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
                    load_indexFile(currentOpenFilePath);
                    loadCompleted=true;
                    return;
                }
            }else{
                //没有检索到满足的文件头标识
                statusBar_disPlayMessage("无法识别的文件类别,请检查config/FileType.ini配置");
                loadCompleted=true;
                return;
            }
        }
    }
}

void MainWindow::load_indexFile(QString currentOpenFilePath){
    currentOpenFileType=0;
    QFile dataFile(currentOpenFilePath);
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

void MainWindow::load_ofdFile(QString sendCode,QString fileType,QString currentOpenFilePath){

    //清除原来的数据信息
    currentOpenFileType=1;
    QString defineMapName;
    QString versionName;
    CodeInfo info=loadedCodeInfo.value(sendCode);
    if(info.getCode().isEmpty()){
        qDebug()<<"未找到该代码的版本信息，默认使用400接口";
        versionName="400";
    }else{
        versionName=(info.getVersion().isEmpty()?"400":info.getVersion());
    }
    qDebug()<<"用于解析本文件的大版本"<<versionName;
    QString versionFromFile="";
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
            //仅读取两行,用户获取文件版本
            if(lineNumber==1){
                versionFromFile=line;
                break;
            }
            lineNumber++;
        }
        dataFile.close();
        if(versionFromFile.isEmpty()){
            statusBar_disPlayMessage("解析失败,未从文件第二行读取到版本号信息");
            return;
        }else{
            defineMapName=versionName+"_"+versionFromFile+"_"+fileType;
            //配置完毕,输出使用的配置
            ui->lineEditUseIni->setText("OFD_"+versionName+"_"+versionFromFile+".ini");
        }
        //判断对应的配置文件是否存在
        QString path="config/OFD_"+versionName+"_"+versionFromFile+".ini";
        if(Utils::isFileExist(path)){
            ofd=loadedOfdDefinitionMap.value(defineMapName);
            if(loadedOfdDefinitionMap.contains(defineMapName)) {
                if(ofd.getuseAble()){
                    //关键,此句强制将toLocal8Bit()函数转换为GB18030编码的字符数组
                    //如果不加次定义,默认取系统编码，因此在英文系统下读取可能会有问题
                    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
                    //找到配置,开始解析配置,此处待优化,需改为后台读取文件
                    qDebug()<<"接口文档记录长度"<<ofd.getrowLength();
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
                                    //如果行长度为9且内容为OFDCFEND,我们就认为文件结束
                                    if(line.length()==8&&QString::compare(line,"OFDCFEND",Qt::CaseInsensitive)==0){
                                        break;
                                    }else{
                                        sucessFalg=false;
                                        statusBar_disPlayMessage(path+"文件中第["+QString::number(lineNumber+1)+"]行数据解析失败...");
                                        QMessageBox::information(this,tr("提示"),"重要提示\r\n\r\n"+path+"中定义的记录长度和文件中不一致,解析失败\r\n"+path+"中"+fileType+"文件定义的数据行长度为["+QString::number(ofd.getrowLength())+"]\r\n实际打开的文件中第["+QString::number(lineNumber+1)+"[行长度为["+QString::number(qbyteArrayRow.size())+"]\r\n请检查是否是文件错误,或者定义错误",QMessageBox::Ok,QMessageBox::Ok);
                                        break;
                                    }
                                }
                            }
                            lineNumber++;
                        }
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
            statusBar_disPlayMessage("解析失败,配置"+path+"不存在...");
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
        ptr_table->verticalHeader()->setDefaultSectionSize(22);
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
        ptr_table->verticalHeader()->setDefaultSectionSize(22);
        //设置表格的内容
        //按行读取ofdFileContentQByteArrayList,边读取边解析
        if(rowCount>0){
            //获取当前table的高度
            int higth=ptr_table->size().height();
            int hValueEnd=hValueBegin+(higth/22);
            display_OFDTable(hValueBegin,hValueEnd);
        }
        statusBar_display_rowsCount(rowCount);
        statusBar_disPlayMessage("文件解析完毕!");
    }
    else
    {
        statusBar_disPlayMessage(tr("空的数据记录,没有数据可供显示..."));
    }
}

//仅仅渲染显示当前指定区域
//算法原理,当table试图发生滚动或者table控件大小发生变化时
//探视当前屏幕显示的区间范围,从QTableWidgetItem池中获取已经不再显示的item复用，大大降低内存开销
void MainWindow::display_OFDTable(int begin,int end){
    int count=ptr_table->rowCount();
    int colCount=ptr_table->columnCount();
    //防止渲染边界超过表总行数
    if(begin<0){
        begin=0;
    }
    if(end>=count){
        end=count-1;
    }
    for (int row = begin; row <= end; ++row)
    {
        for(int col=0;col<colCount;col++){
            QTableWidgetItem *item= new QTableWidgetItem();
            ptr_table->setItem(row, col, item);
            //其他类型,原样输出，不再调整
            item->setText(getValuesFromofdFileContentQByteArrayList(row,col));
        }
    }
    ptr_table->resizeColumnsToContents();
}

QString MainWindow::getValuesFromofdFileContentQByteArrayList(int row ,int col){
    //判断越界
    if(row>=ofdFileContentQByteArrayList.count()||col>=ofd.getfieldCount()){
        return "";
    }
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd.getfieldList().at(col).getLength();
    //小数长度
    int filedDeclength=ofd.getfieldList().at(col).getDecLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(ofdFileContentQByteArrayList.at(row).mid(ofd.getfieldList().at(col).getRowBeginIndex(),filedlength));
    //数据信息处理
    //字符型--trim处理
    if(ofd.getfieldList().at(col).getFiledType()=="C"){
        filed=filed.trimmed();
    }
    //数字字符型，限于0—9--trim处理
    else if(ofd.getfieldList().at(col).getFiledType()=="A"){
        filed=filed.trimmed();
    }
    //数值型，其长度不包含小数点，可参与数值计算
    //去除左侧的0,但是如果整数部分全是0，则至少保留一个0然后插入一个小数点
    else  if(ofd.getfieldList().at(col).getFiledType()=="N"){
        int needCheck=filedlength-filedDeclength-1;
        int needCutZero=0;
        for(int s=0;s<needCheck;s++){
            if(filed.at(s)=='0'){
                needCutZero++;
            }
            //如果不是0了，则跳出循环
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
    //不定长文本
    else  if(ofd.getfieldList().at(col).getFiledType()=="TEXT"){
        filed=filed.trimmed();
    }
    return filed;
}

void MainWindow::clearTable(){
    ptr_table->clearContents();
    ptr_table->setRowCount(0);
    ptr_table->setColumnCount(0);
}

void MainWindow::statusBar_disPlayMessage(QString text){
    statusLabel_ptr_showMessage->setText(text);
}

void MainWindow::clear_Display_Table_Info(){
    ui->lineEditSendCode->setText(NULL);
    ui->lineEditRecCode->setText(NULL);
    ui->lineEditFileTransferDate->setText(NULL);
    ui->lineEditFileType->setText(NULL);
    ui->lineEditSenfInfo->setText(NULL);
    ui->lineEditRecInfo->setText(NULL);
    ui->lineEditFileDescribe->setText(NULL);
    ui->lineEditUseIni->setText(NULL);
    statusBar_clear_statusBar();
    clearTable();
}

void MainWindow::on_fileOpen_triggered()
{
    open_file_Dialog();
}

void MainWindow::on_aboutOpen_triggered()
{
    QMessageBox::about(this,tr("关于本程序"),tr("本程序是一个可以解析格式化显示开放式基金数据交换协议文件的工具\r\n可以用于解析如上所述协议所交换的各种文件\r\n作      者:793554262@qq.com(刘德位)\r\n商业支持:793554262@qq.com(刘德位)\r\n友情赞助:15238872101(支付宝转账)\r\n\r\n版本:")+Utils::getVersion());
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
            info.append("文件总记录数:").append(indexFileHeaderMap.value("count")).append("成功加载记录数:").append(QString::number(indexFileDataList.count())).append("\r\n");
            info.append("文件起始行:").append(indexFileHeaderMap.value("filebegin")).append("\r\n");
            info.append("文件结束行:").append(indexFileHeaderMap.value("fileend")).append("\r\n");
            QMessageBox::information(this,tr("检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Yes,QMessageBox::Yes);
        }
    }
    else{
        //OFD文件检查器
    }
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    qDebug()<<index.row();
}
void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentOpenFileType==1){
        //记录当前所在行
        rowcurrent=currentRow;
        //当前所在列
        colcurrent=currentColumn;
        // qDebug()<<"row"<<QString::number(rowcurrent)<<",col"<<QString::number(colcurrent);
        int rowInFile=12+ofd.getfieldCount()+currentRow;
        int colInFile=ofd.getfieldList().at(currentColumn).getRowBeginIndex()+1;
        statusBar_display_rowsAndCol(rowInFile,colInFile,ofd.getfieldList().at(currentColumn).getLength());
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
