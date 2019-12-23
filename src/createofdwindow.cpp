#include "src/createofdwindow.h"
#include "ui_createofdwindow.h"


CreateOFDWindow::CreateOFDWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateOFDWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    //初始化显示日期
    QDate date=ui->calendarWidget->selectedDate();
    setWindowTitle(windowTitle+"-[文件日期:"+date.toString("yyyy-MM-dd")+"]");
    //过滤器//过滤不需要的文件
    excludeFiles.append(".");
    excludeFiles.append("..");
    //程序启动的时候加载配置
    //检查并创建导出目录
    QDir config = QDir(configpath);
    bool exist = config.exists();
    //如果存在配置目录-则遍历配置目录
    if(exist)
    {
        //分别加载各类配置
        //交易申请类
        QDir configrequest = QDir(configpath+"request");
        bool existrequest = configrequest.exists();
        if(existrequest){
            QFileInfoList fileInfoList = configrequest.entryInfoList();
            foreach ( QFileInfo fileInfo, fileInfoList )
            {
                if ( fileInfo.fileName() == "." || fileInfo.fileName() == ".." )
                    continue;
                if ( fileInfo.isDir() )
                {
                    //加入配置
                    requestFileList.append(fileInfo.fileName());
                    ui->comboBox_2->addItem(fileInfo.fileName());
                }
            }
        }
        //基金行情类
        QDir confignav = QDir(configpath+"fundday");
        bool existnav = confignav.exists();
        //存在可用的行情配置
        if(existnav){
            QFileInfoList fileInfoList = confignav.entryInfoList();
            foreach ( QFileInfo fileInfo, fileInfoList )
            {
                if ( fileInfo.fileName() == "." || fileInfo.fileName() == ".." )
                    continue;
                if ( fileInfo.isDir() )
                {
                    //加入配置
                    navFileList.append(fileInfo.fileName());
                }
            }
        }
        //基金确认类
        QDir configconfirm = QDir(configpath+"confirm");
        bool existconfirm = configconfirm.exists();
        //存在可用的行情配置
        if(existconfirm){
            QFileInfoList fileInfoList = configconfirm.entryInfoList();
            foreach ( QFileInfo fileInfo, fileInfoList )
            {
                if ( fileInfo.fileName() == "." || fileInfo.fileName() == ".." )
                    continue;
                if ( fileInfo.isDir() )
                {
                    //加入配置
                    confirmFileList.append(fileInfo.fileName());
                }
            }
        }
    }
    //日历控件focus
    ui->calendarWidget->setFocus();
}

CreateOFDWindow::~CreateOFDWindow()
{
    delete ui;
}

void CreateOFDWindow::on_pushButton_clicked()
{
    //TA代码
    QString tacode=ui->lineEditTaCode->text();
    //销售商代码
    QString agentcode=ui->lineEditAgentCode->text();
    //日期
    QDate date=ui->calendarWidget->selectedDate();
    //组装等待被替换的模板变量
    QHash<QString,QString> data;
    data.insert("tacode",tacode);
    data.insert("agentcode",agentcode);
    data.insert("transferdate",date.toString("yyyyMMdd"));
    //开始做合法判断
    if(tacode.isEmpty()){
        ui->logOut->setText("TA代码不能为空");
        return;
    }
    if(tacode.length()!=2){
        ui->logOut->setText("TA代码目前应该是两位,请填写两位TA代码");
        return;
    }
    if(agentcode.isEmpty()){
        ui->logOut->setText("销售商代码不能为空");
        return;
    }
    if(agentcode.length()!=3){
        ui->logOut->setText("销售商代码目前应该是三位,请填写三位销售商代码");
        return;
    }
    //检查并创建导出目录
    QDir temp = QDir(exppath);
    if(!temp.exists())
    {
        bool ok = temp.mkdir(exppath);
        if( ok ){
            ui->logOut->setText("创建数据导出目录成功");
        }
        else{
            ui->logOut->setText("创建数据导出目录失败!");
            return;
        }
    }
    //开始生成模板路径和目标路径
    QString sourcePath="";
    QString targetPath="";
    //根据文件类型处理
    //申请类数据
    if(ui->comboBox->currentIndex()==0){
        if(ui->comboBox_2->count()<1){
            ui->logOut->setText("无可用的基金申请类数据文件模板!");
            return;
        }
        targetPath=exppath+"交易申请/"+agentcode+"/"+date.toString("yyyyMMdd")+"/"+tacode+"/";
        sourcePath=configpath+"request/"+ui->comboBox_2->currentText();
    }
    //行情类数据
    if(ui->comboBox->currentIndex()==1){
        if(ui->comboBox_2->count()<1){
            ui->logOut->setText("无可用的基金行情类数据文件模板!\r\n");
            return;
        }
        targetPath=exppath+"行情和确认/"+agentcode+"/"+date.toString("yyyyMMdd")+"/"+tacode+"/FundDay/";
        sourcePath=configpath+"fundday/"+ui->comboBox_2->currentText();
    }
    //交易确认类数据
    if(ui->comboBox->currentIndex()==2){
        if(ui->comboBox_2->count()<1){
            ui->logOut->setText("无可用的基金确认类数据文件模板!\r\n");
            return;
        }
        sourcePath=configpath+"confirm/"+ui->comboBox_2->currentText();
        targetPath=exppath+"行情和确认/"+agentcode+"/"+date.toString("yyyyMMdd")+"/"+tacode+"/Confirm/";
    }
    bool exist;
    bool ok=true;
    QDir targetDir = QDir(targetPath);
    exist = targetDir.exists();
    if(!exist)
    {
        ok=targetDir.mkpath(targetPath);
    }
    if(ok){
        QStringList  filesOld = targetDir.entryList(QStringList(), QDir::Files|QDir::Readable, QDir::Name);
        if(filesOld.count()>0){
            DialogMyTip * dialog2 = new DialogMyTip("文件生成路径["+targetPath+"]下存在文件,是否清空此目录下的文件生成新的文件？\r\n仅删除文件且不可恢复！",this);
            dialog2->setWindowTitle("请确认！");
            dialog2->setModal(true);
            dialog2->exec();
            if(!dialog2->getBoolFlag()){
                ui->logOut->setText("放弃覆盖生成!");
                return;
            }
            else{
                //清空目标目录下的文件，不清除目录
                QDir dir(targetPath);
                dir.setFilter(QDir::Files);
                int fileCount = dir.count();
                for (int i = 0; i < fileCount; i++){
                    dir.remove(dir[i]);
                }
            }
        }
        //将模板复制到目标目录
        copyFolder(sourcePath,targetPath,excludeFiles);
        //执行遍历更新
        updateFromtemplete(targetPath,data);
        QStringList  files = targetDir.entryList(QStringList(), QDir::Files|QDir::Readable, QDir::Name);
        QStringList  okFiles;
        if(files.count()>0){
            QString fileList;
            for(int ff=0;ff<files.count();ff++){
                fileList.append(files.at(ff)).append("\r\n");
                //判断是否是OK文件，OK文件稍后刷新下修改时间,确保要比数据文件的新,数据文件现在刷新
                if(files.at(ff).endsWith("OK")||files.at(ff).endsWith("ok")){
                    okFiles.append(files.at(ff));
                }
                else{
                    qDebug()<<targetPath+files.at(ff);
                    Utils::UpdateFileTime(targetPath+files.at(ff));
                }
            }
            //现在开始更新ok文件的修改时间
            //如果ok文件的更新时间早于数据文件，在某些系统环境下会认为数据没更新，所以OK文件在这里更新时间戳
            if(okFiles.count()>0){
                for(int okindex=0;okindex<okFiles.count();okindex++){
                    qDebug()<<targetPath+okFiles.at(okindex);
                    Utils::UpdateFileTime(targetPath+okFiles.at(okindex));
                }
            }
            //数据生成完毕,文件时间戳刷新完毕
            ui->logOut->setText("数据已生成在如下路径:\r\n"+targetPath+"\r\n包含如下文件:\r\n"+fileList);
        }
        else{
            ui->logOut->setText("生成数据失败!");
        }
    }
    else{
        ui->logOut->setText("创建文件导出目录"+targetPath+"失败,请检查路径权限");
    }
}


/**
 * @brief CreateOFDWindow::copyFolder目录复制
 * @param srcFolder
 * @param dstFolder
 * @param excludeFiles
 */
void  CreateOFDWindow::copyFolder(const QString &srcFolder,const QString & dstFolder,const QStringList &excludeFiles)
{
    QDir dir( srcFolder );
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    int count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if( excludeFiles.indexOf( fileName ) != -1 )
            continue;
        if( !dir.exists(dstFolder) )
            dir.mkpath(dstFolder);
        QString newSrcFileName = srcFolder + "/" + fileInfo.fileName();
        QString newDstFileName = dstFolder + "/" + fileInfo.fileName();
        QFile::copy(newSrcFileName, newDstFileName);
    }

    dir.setFilter(QDir::Dirs);
    list = dir.entryInfoList();
    count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if( fileInfo.isDir() )
        {
            if( excludeFiles.indexOf( fileName ) != -1 )
                continue;
            QString newSrcFolder = srcFolder + "/" + fileName;
            QString newDstFolder = dstFolder + "/" + fileName;
            copyFolder( newSrcFolder , newDstFolder ,excludeFiles);
        }
    }
}

/**
 * @brief CreateOFDWindow::updateFromtemplete 基于模板文件加工成目标文件
 * @param path
 * @param data
 */
void CreateOFDWindow::updateFromtemplete(QString path,QHash<QString,QString> data){
    QDir dir(path);
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            QFile currentfile(mfi.absoluteFilePath());
            QStringList lineList;
            //开放式基金交换协议使用GB18030编码
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
            //获取文件内容
            if (currentfile.open(QFile::ReadOnly|QIODevice::Text))
            {
                QTextStream dataLine(&currentfile);
                QString line;
                int lineNumber=0;
                while (!dataLine.atEnd())
                {
                    line = dataLine.readLine().replace("agentcode",data.value("agentcode")).replace("tacode",data.value("tacode")).replace("transferdate",data.value("transferdate"));
                    lineList.append(line);
                }
                lineNumber++;
            }
            //关闭文件
            currentfile.close();
            //删除原文件后重建
            currentfile.remove();
            QFile newFile(mfi.absoluteFilePath().replace("agentcode",data.value("agentcode")).replace("tacode",data.value("tacode")).replace("transferdate",data.value("transferdate")));
            if (newFile.open(QFile::WriteOnly | QIODevice::Truncate)) {
                QTextStream out(&newFile);
                //开始准备待写入的数据
                QString sb;
                //文本内容
                sb.clear();
                if(lineList.count()>0){
                    for (int row=0;row<lineList.count();row++){
                        //数据写入--按行读取
                        sb.append(lineList.at(row)).append("\r\n");
                    }
                }
                out<<sb;
                sb.clear();
                newFile.close();
            }
        }
        else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
            updateFromtemplete(mfi.absoluteFilePath(),data);
        }
    }
}

void CreateOFDWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    //基金交易申请
    case 0:
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(requestFileList);
        break;
        //基金行情
    case 1:
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(navFileList);
        break;
        //交易确认
    case 2:
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(confirmFileList);
        break;
    default:
        break;
    }
}

/**
 * @brief CreateOFDWindow::on_calendarWidget_selectionChanged 日期变更时展示文件交互日期
 */
void CreateOFDWindow::on_calendarWidget_selectionChanged()
{
    //初始化显示日期
    QDate date=ui->calendarWidget->selectedDate();
    setWindowTitle(windowTitle+"-[文件日期:"+date.toString("yyyy-MM-dd")+"]");
}
