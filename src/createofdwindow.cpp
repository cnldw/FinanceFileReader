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
    //程序启动的时候加载配置
    //检查并创建导出目录
    //如果存在配置目录-则遍历配置目录
    if(QDir(configpath).exists())
    {
        //分别加载各类配置
        //交易申请类
        QDir configrequest = QDir(configpath+"request");
        if(configrequest.exists()){
            QFileInfoList fileInfoList = configrequest.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
            foreach ( QFileInfo fileInfo, fileInfoList )
            {
                //加入配置
                requestFileList.append(fileInfo.fileName());
                ui->comboBox_2->addItem(fileInfo.fileName());
            }
        }
        //基金行情类
        QDir confignav = QDir(configpath+"fundday");
        //存在可用的行情配置
        if(confignav.exists()){
            QFileInfoList fileInfoList = confignav.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
            foreach ( QFileInfo fileInfo, fileInfoList )
            {
                //加入配置
                navFileList.append(fileInfo.fileName());
            }
        }
        //基金确认类
        QDir configconfirm = QDir(configpath+"confirm");
        //存在可用的行情配置
        if(configconfirm.exists()){
            QFileInfoList fileInfoList = configconfirm.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot );
            foreach ( QFileInfo fileInfo, fileInfoList )
            {
                //加入配置
                confirmFileList.append(fileInfo.fileName());
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
    //将模板变量打包到QHash里
    QHash<QString,QString> data;
    data.insert("tacode",tacode);
    data.insert("agentcode",agentcode);
    data.insert("transferdate",date.toString("yyyyMMdd"));
    //正则校验器
    QString pattern("^[A-Za-z0-9]+$");
    QRegExp rx(pattern);
    //开始做合法判断
    if(tacode.isEmpty()){
        ui->logOut->setText("TA代码不能为空");
        return;
    }
    if(!rx.exactMatch(tacode)){
        ui->logOut->setText("TA代码只允许包含数字和字母！,请勿随意输入");
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
    if(!rx.exactMatch(agentcode)){
        ui->logOut->setText("销售商代码只允许包含数字和字母！,请勿随意输入");
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
        if( !ok ){
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
        sourcePath=configpath+"request/"+ui->comboBox_2->currentText()+"/";
    }
    //行情类数据
    if(ui->comboBox->currentIndex()==1){
        if(ui->comboBox_2->count()<1){
            ui->logOut->setText("无可用的基金行情类数据文件模板!\r\n");
            return;
        }
        targetPath=exppath+"行情和确认/"+agentcode+"/"+date.toString("yyyyMMdd")+"/"+tacode+"/FundDay/";
        sourcePath=configpath+"fundday/"+ui->comboBox_2->currentText()+"/";
    }
    //交易确认类数据
    if(ui->comboBox->currentIndex()==2){
        if(ui->comboBox_2->count()<1){
            ui->logOut->setText("无可用的基金确认类数据文件模板!\r\n");
            return;
        }
        sourcePath=configpath+"confirm/"+ui->comboBox_2->currentText()+"/";
        targetPath=exppath+"行情和确认/"+agentcode+"/"+date.toString("yyyyMMdd")+"/"+tacode+"/Confirm/";
    }
    //目录创建结果
    bool ok=true;
    QDir targetDir = QDir(targetPath);
    if(!targetDir.exists())
    {
        ok=targetDir.mkpath(targetPath);
    }
    //如果导出路径OK
    if(ok){
        targetDir.refresh();
        QStringList  filesOld = targetDir.entryList(QStringList(), QDir::Files, QDir::Name);
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
                targetDir.setFilter(QDir::Files);
                int fileCount = targetDir.count();
                for (int i = 0; i < fileCount; i++){
                    targetDir.remove(targetDir[i]);
                }
            }
        }
        //将模板目录里的文件复制到目标目录
        QDir sourceDir=QDir(sourcePath);
        QStringList  sourcefiles = sourceDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        //复制到目标目录的所有文件
        QString  targetdataandokfiles;
        //源目录的ok模板文件
        QStringList sourceokfiles;
        //复制到目标目录的OK文件
        QStringList  targetokfiles;
        //循环拷贝文件使用的变量
        QString sourceFile;
        QString targetFileName;
        QString targetFile;
        //遍历源目录文件--忽略目录
        if(sourcefiles.count()>0){
            for(int ff=0;ff<sourcefiles.count();ff++){
                //源模板文件
                sourceFile=sourcePath+sourcefiles.at(ff);
                //拷贝过去的文件名--注意这里基于模板变量替换文件名
                targetFileName=QString(sourcefiles.at(ff)).replace("agentcode",data.value("agentcode")).replace("tacode",data.value("tacode")).replace("transferdate",data.value("transferdate"));
                targetFile=targetPath+targetFileName;
                targetdataandokfiles.append("\r\n").append(targetFileName);
                //ok文件延后拷贝-确保ok文件在数据文件生成后生成
                if(targetFile.endsWith("ok")||targetFile.endsWith("OK")){
                    sourceokfiles.append(sourceFile);
                    targetokfiles.append(targetFileName);
                    continue;
                }
                //数据文件现在拷贝
                else{
                    //如果拷贝失败则终止
                    if(!copyFile(sourceFile,targetFile,true,true)){
                        ui->logOut->setText("写入如下模板文件失败,生成失败!!!:\r\n"+sourceFile);
                        return;
                    }
                    //非OK文件更新文件内容
                    //模板文件拷贝过去后，读取文件内容基于模板变量替换生成新的文件
                    else {
                        QFile currentfile(targetFile);
                        QStringList lineList;
                        //获取文件内容
                        if (currentfile.open(QFile::ReadOnly|QIODevice::Text))
                        {
                            QTextStream dataLine(&currentfile);
                            QString line;
                            while (!dataLine.atEnd())
                            {
                                line = dataLine.readLine().replace("agentcode",data.value("agentcode")).replace("tacode",data.value("tacode")).replace("transferdate",data.value("transferdate"));
                                lineList.append(line);
                            }
                        }
                        //关闭文件
                        currentfile.close();
                        //删除原文件后重建
                        currentfile.remove();
                        QFile newFile(targetFile);
                        if (newFile.open(QFile::WriteOnly | QIODevice::Truncate)) {
                            //开始准备待写入的数据
                            QString sb="";
                            if(lineList.count()>0){
                                for (int row=0;row<lineList.count();row++){
                                    //数据写入--按行读取
                                    sb.append(lineList.at(row)).append("\r\n");
                                }
                            }
                            //按照GB18030还原字节数据写入文件
                            newFile.write(codecGb18030->fromUnicode(sb));
                            newFile.flush();
                            newFile.close();
                        }
                    }
                }
            }
        }
        //如果ok文件的更新时间早于数据文件，在某些系统环境下会认为数据没更新，所以OK文件在这里拷贝并更新时间戳
        if(targetokfiles.count()>0){
            for(int okindex=0;okindex<targetokfiles.count();okindex++){
                //源模板文件
                sourceFile=sourceokfiles.at(okindex);
                //目标写入文件
                targetFile=targetPath+targetokfiles.at(okindex);
                //如果拷贝失败则终止
                if(!copyFile(sourceFile,targetFile,true,true)){
                    ui->logOut->setText("写入如下模板文件失败,生成失败!!!:\r\n"+sourceFile);
                    return;
                }
                //注意,ok文件我们认为是空文件，不再打开进行文件内的模板变量替换
            }
        }
        //数据生成完毕
        ui->logOut->setText("数据已生成在如下路径:\r\n"+targetPath+"\r\n包含如下文件:"+targetdataandokfiles);
    }
    else{
        ui->logOut->setText("创建文件导出目录"+targetPath+"失败,请检查路径权限");
    }
}

bool CreateOFDWindow::copyFile(QString srcFile ,QString dstFile, bool coverFileIfExist,bool upDateTime)
{
    dstFile.replace("\\","/");
    if (srcFile == dstFile){
        return true;
    }
    if (!QFile::exists(srcFile)){
        return false;
    }
    QFile *createfile  = new QFile;
    bool exist = createfile->exists(dstFile);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(dstFile);
        }
    }
    if(!QFile::copy(srcFile, dstFile))
    {
        return false;
    }
    //更新文件的创建和修改时间，理论上来说我们应当在搬移文件时更新文件的时间戳，避免各种操蛋的问题出现
    if(upDateTime){
        Utils::UpdateFileTime(dstFile);
    }
    return true;
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
