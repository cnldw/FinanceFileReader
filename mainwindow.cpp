#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle("基金文件阅读器-"+Utils::getVersion());

    //第二个标签
    statusLabelTwo = new QLabel;
    statusLabelTwo->setMinimumSize(130, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelTwo);
    //设置标签内容
    statusLabelTwo->setText(tr("记录数:"));

    //第三个标签
    statusLabelThree = new QLabel;
    statusLabelThree->setMinimumSize(220, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelThree);
    //设置标签内容
    statusLabelThree->setText(tr("文件:"));

    //第四个标签
    statusLabelFour = new QLabel;
    statusLabelFour->setMinimumSize(150, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelFour);
    //设置标签内容
    statusLabelFour->setText(tr("文件内0行0列"));
    statusLabelFour->setToolTip(tr("此处显示当前选择的字段在原文件中的行和列"));

    //第五个标签
    statusLabelFive = new QLabel;
    statusLabelFive->setMinimumSize(335, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelFive);
    //设置标签内容
    statusLabelFive->setText(tr(""));

    load_CodeInfo();
    load_FileType();
    load_OFDDefinition();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete statusLabelTwo;
    statusLabelTwo=nullptr;
    delete statusLabelThree;
    statusLabelThree=nullptr;
    delete statusLabelFour;
    statusLabelFour=nullptr;
    delete statusLabelFive;
    statusLabelFive=nullptr;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()){
        return;
    }
    if(urls.count()>1){
        statusBar_disPlay("拖进来一个文件试试~,文件太多啦");
        return;
    }
    QString fileName = urls.first().toLocalFile();
    //判断是否是文件夹
    QFileInfo fileinfo(fileName);
    if(fileinfo.isDir()){
        statusBar_disPlay("拖进来一个文件试试~,不接受文件夹");
        return;
    }
    if (fileName.isEmpty())
    {        return;
    }
    else{
        filePath=fileName;
        ui->filePathLineText->setText(filePath);
        display_fileName(filePath);
        initFile(filePath);
    }
}

/**
  *清除状态栏的信息
 * @brief MainWindow::clear_statusBar
 */
void MainWindow::clear_statusBar(){
    statusLabelTwo->setText(tr("记录数:"));
    statusLabelThree->setText(tr("文件:"));
    statusLabelFour->setText(tr("文件内0行0列"));
    statusLabelFive->setText(NULL);
}

/**
  *打开文件窗口
 * @brief MainWindow::open_file_Dialog
 */
void MainWindow::open_file_Dialog(){
    QString   file = QFileDialog::getOpenFileName(this, tr("打开文件..."), ".", tr("所有文件(*.*)"));
    if(file.length() != 0) {
        filePath=file;
        ui->filePathLineText->setText(filePath);
        display_fileName(filePath);
        initFile(filePath);
    } else {
        //放弃了读取文件
    }
}
/**
  *显示加载的记录总数
 * @brief MainWindow::display_rowsCount
 * @param rowsCount
 */
void MainWindow::display_rowsCount(int rowsCount){

    statusLabelTwo->setText(tr("记录数:")+QString::number(rowsCount, 10)+tr("行"));
}

void MainWindow::load_CodeInfo(){
    QString filePath="./config/CodeInfo.ini";
    if(Utils::isFileExist(filePath)){
        //加载ini文件
        QSettings codeInfoIni(filePath,QSettings::IniFormat,0);
        //目前仅接收UTF-8编码的配置文件
        codeInfoIni.setIniCodec("UTF-8");
        QStringList agencyInfo=codeInfoIni.childGroups();
        //获取所有代码和代码对应的机构名称
        for(int i=0;i<agencyInfo.count();i++){
            CodeInfo info;
            //qDebug()<<codeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString();
            info.setCode(codeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString());
            info.setVersion(codeInfoIni.value(agencyInfo.at(i)+"/IVERSION").toString());
            info.setName(codeInfoIni.value(agencyInfo.at(i)+"/NAME").toString());
            codeInfo.insert(codeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString(),info);
        }
        //debug信息
        QString info=QString("共计加载代码数据条数:").append(QString::number(agencyInfo.count()));
        qDebug()<<info;
    }else{
        statusBar_disPlay(tr("config/CodeInfo.ini配置丢失"));
    }
}

void MainWindow::load_FileType(){
    QString filePath="./config/FileType.ini";
    if(Utils::isFileExist(filePath)){
        //加载ini文件
        QSettings fileTypeIni(filePath,QSettings::IniFormat,0);
        //目前仅接收UTF-8编码的配置文件
        fileTypeIni.setIniCodec("UTF-8");
        //读取INDEXFILE
        fileTypeIni.beginGroup("INDEXFILE");
        QStringList info =fileTypeIni.childKeys();
        //开始加载可用的索引文件类别
        if(info.count()>0){
            for(int i=0;i<info.count();i++){
                indexFileInfo.insert(info.at(i),fileTypeIni.value(info.at(i)).toString());
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
                ofdFileInfo.insert(info2.at(i),fileTypeIni.value(info2.at(i)).toString());
                // qDebug()<<info2.at(i)<<fileTypeIni.value(info2.at(i)).toString();
            }
        }
        fileTypeIni.endGroup();}
    else{
        statusBar_disPlay(tr("config/FileType.ini配置丢失"));
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
                                ofdDefinitionMap.insert(name,ofd);
                            }
                            //如果记录有错误,则仅写于不可用和错误原因
                            else{
                                ofd.setuseAble(okFlag);
                                ofd.setMessage(message);
                                ofdDefinitionMap.insert(name,ofd);
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
 * @brief MainWindow::display_fileName
 * @param filePath
 */
void MainWindow::display_fileName(QString filePath){
    int first = filePath.lastIndexOf ("/");
    QString fileName = filePath.right (filePath.length ()-first-1);
    statusLabelThree->setText(tr("文件：")+fileName);
}

void MainWindow::initFile(QString filePath){
    //首先清除原来的显示信息
    clear_Display();
    //首先获取完整的文件名
    int first = filePath.lastIndexOf ("/");
    QString fileName = filePath.right (filePath.length ()-first-1);
    //状态栏显示文件名
    display_fileName(fileName);
    if(fileName.length()<10){
        statusBar_disPlay("无法识别的文件类别,请检查");
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
                statusBar_disPlay("错误的文件名,参考:OFD_XX_XXX_YYYYMMDD_XX.TXT");
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
                QString sendName=(codeInfo.value(sendCode)).getName();
                QString recName=(codeInfo.value(recCode)).getName();
                QString fileTypeName=ofdFileInfo.value(fileTypeCode);
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
                load_ofdFile(sendCode,fileTypeCode,filePath);
                return;
            }
        }else{
            //开始解析非OFD文件
            QString nameBegin=fileName.left(3);
            if(!indexFileInfo.value(nameBegin).isEmpty()){
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
                    statusBar_disPlay("无法识别的非OFD文件,请检查config/FileType.ini配置");
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
                    QString sendName=codeInfo.value(sendCode).getName();
                    QString recName=codeInfo.value(recCode).getName();
                    QString fileIndexTypeName=indexFileInfo.value(fileIndexTypeCode);
                    //刷新UI
                    ui->lineEditSendCode->setText(sendCode);
                    ui->lineEditRecCode->setText(recCode);
                    ui->lineEditFileTransferDate->setText(dateInfo);
                    ui->lineEditFileType->setText(fileIndexTypeCode);
                    ui->lineEditSenfInfo->setText(sendName);
                    ui->lineEditRecInfo->setText(recName);
                    ui->lineEditFileDescribe->setText(fileIndexTypeName);
                    //此处开始加载索引文件
                    load_indexFile(filePath);
                    return;
                }
            }else{
                //没有检索到满足的文件头标识
                statusBar_disPlay("无法识别的文件类别,请检查config/FileType.ini配置");
                return;
            }
        }
    }
}

/**
  *加载索引文件的实现,注意数据加载放到后台线程实现,避免加载数据时UI卡顿
 * @brief MainWindow::load_indexFile
 * @param filePath
 */

void MainWindow::load_indexFile(QString filePath){

    //清除原来的数据信息
    fileHeaderMap.clear();
    fileDataList.clear();
    /////////////////////////////
    QList<int> width;
    width.append(210);
    QStringList header;
    header.append("索引包含的文件信息");
    fileDataList.append(header);
    QFile dataFile(filePath);
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
                fileDataList.append(lineList);
            }
            //文件头部
            else{
                switch (lineNumber) {
                case 0:
                    //文件第一行OF标记
                    fileHeaderMap.insert("filebegin",line);
                    break;
                case 1:
                    //文件第二行版本
                    fileHeaderMap.insert("version",line);
                    break;
                case 2:
                    //文件第三行发送方代码
                    fileHeaderMap.insert("sendcode",line);
                    break;
                case 3:
                    //文件第四行接收方代码
                    fileHeaderMap.insert("recivecode",line);
                    break;
                case 4:
                    //文件第五行文件传递日期
                    fileHeaderMap.insert("filedate",line);
                    break;
                case 5:
                    //文件第6行文件记录数
                    fileHeaderMap.insert("count",line);
                    break;
                default:
                    break;
                }
            }
            lineNumber++;
        }
        //处理最后一行
        QString lastLine=fileDataList.last().at(0);
        if(lastLine.startsWith("OFDCFEND")){
            fileHeaderMap.insert("fileend",lastLine);
            fileDataList.removeLast();
        }
        dataFile.close();
    }
    displayIndexTable(width,fileDataList);
}

void MainWindow::load_ofdFile(QString sendCode,QString fileType,QString filePath){

    //清除原来的数据信息
    fileHeaderMap.clear();
    fileDataList.clear();
    //首先获取发送方Code用于检索V版本
    QString defineMapName;
    QString versionName;
    CodeInfo info=codeInfo.value(sendCode);
    if(info.getCode().isEmpty()){
        qDebug()<<"未找到该代码的版本信息，默认使用400接口";
        versionName="400";
    }else{
        versionName=info.getVersion();
    }
    qDebug()<<"用于解析本文件的大版本"<<versionName;
    QString versionFromFile="";
    QFile dataFile(filePath);
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
            statusBar_disPlay("解析失败,未从文件第二行读取到版本号信息");
            return;
        }else{
            defineMapName=versionName+"_"+versionFromFile+"_"+fileType;
            //配置完毕,输出使用的配置
            ui->lineEditUseIni->setText("OFD_"+versionName+"_"+versionFromFile+".ini");
        }
        //判断对应的配置文件是否存在
        QString path="config/OFD_"+versionName+"_"+versionFromFile+".ini";
        if(Utils::isFileExist(path)){
            OFDFileDefinition ofd=ofdDefinitionMap.value(defineMapName);
            if(ofdDefinitionMap.contains(defineMapName)) {
                if(ofd.getuseAble()){
                    //找到配置,开始解析配置,此处待优化,需改为后台读取文件
                    statusBar_disPlay("开始解析文件内容");
                }else{
                    statusBar_disPlay("解析失败:配置文件"+path+"中"+ofd.getMessage());
                    return;
                }
            }else{
                statusBar_disPlay("解析失败:未在"+path+"配置中找到"+fileType+"文件的定义,请配置");
                return;
            }

        }else{
            statusBar_disPlay("解析失败,配置"+path+"不存在");
            return;
        }
    }
}

void MainWindow::displayIndexTable(QList<int> colwidth,QList <QStringList> data){
    //data分解，第一行记录是表头，从第二行开始为数据
    if(!data.empty()){
        int colCount=data.at(0).count();
        int rowCount=data.count()-1;
        QTableWidget *table=ui->tableWidget;
        table->setColumnCount(colCount);
        table->setRowCount(rowCount);
        //设置表格行标题
        table->setHorizontalHeaderLabels(data.at(0));
        for(int i=0;i<colCount;i++){
            table->setColumnWidth(i,colwidth.at(i));
        }
        //设置表格的选择方式
        table->setSelectionBehavior(QAbstractItemView::SelectItems);
        //设置编辑方式
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //设置表格的内容
        if(rowCount>0){
            for (int row = 1; row <= rowCount; ++row)
            {
                table->setRowHeight(row-1,22);
                QStringList rowdata=data.at(row);
                for(int col=0;col<colCount;col++){
                    QTableWidgetItem *item= new QTableWidgetItem(rowdata.at(col));
                    table->setItem(row-1, col, item);
                }
            }
        }
        //table->resizeColumnsToContents();
        display_rowsCount(rowCount);
    }
    else
    {
        statusBar_disPlay(tr("没有数据可供显示~"));
    }
}

void MainWindow::clearTable(){
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
}

void MainWindow::statusBar_disPlay(QString text){
    statusLabelFive->setText(text);
}

void MainWindow::clear_Display(){
    ui->lineEditSendCode->setText(NULL);
    ui->lineEditRecCode->setText(NULL);
    ui->lineEditFileTransferDate->setText(NULL);
    ui->lineEditFileType->setText(NULL);
    ui->lineEditSenfInfo->setText(NULL);
    ui->lineEditRecInfo->setText(NULL);
    ui->lineEditFileDescribe->setText(NULL);
    clear_statusBar();
    clearTable();
}

void MainWindow::on_fileOpen_triggered()
{
    open_file_Dialog();
}

void MainWindow::on_aboutOpen_triggered()
{
    QMessageBox::about(this,tr("关于本程序"),tr("本程序是一个可以解析格式化显示开放式基金数据交换协议的工具，用于解析如上所述协议所交换的各种文件\r\n作者:793554262@qq.com(刘德位)\r\n\r\n版本:")+Utils::getVersion());
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
    if(!fileHeaderMap.isEmpty()){
        //组织要显示的内容
        QString info;
        info.append("文件解析情况如下:\r\n");
        info.append("文件发送者代码:").append(fileHeaderMap.value("sendcode")).append("\r\n");
        info.append("文件接收者代码:").append(fileHeaderMap.value("recivecode")).append("\r\n");
        info.append("文件传递日期:").append(fileHeaderMap.value("filedate")).append("\r\n");
        info.append("文件总记录数:").append(fileHeaderMap.value("count")).append("成功加载记录数:").append(QString::number(fileDataList.count()-1)).append("\r\n");
        info.append("文件起始行:").append(fileHeaderMap.value("filebegin")).append("\r\n");
        info.append("文件结束行:").append(fileHeaderMap.value("fileend")).append("\r\n");
        QMessageBox::information(this,tr("检查结果"),info,QMessageBox::Ok,QMessageBox::Ok);
    }else{
        QMessageBox::information(this,tr("提示"),tr("目前未打开任何有效的接口文件"),QMessageBox::Yes,QMessageBox::Yes);
    }
}
