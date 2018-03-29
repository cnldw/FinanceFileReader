#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    //第一个标签
    statusLabelOne = new QLabel;
    statusLabelOne->setMinimumSize(50, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelOne);
    statusLabelOne->setText(tr("耗时:"));

    //第二个标签
    statusLabelTwo = new QLabel;
    statusLabelTwo->setMinimumSize(100, 20); // 设置标签最小大小
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
    statusLabelFive->setMinimumSize(300, 20); // 设置标签最小大小
    ui->statusBar->addWidget(statusLabelFive);
    //设置标签内容
    statusLabelFive->setText(tr(""));

    load_CodeInfo();
    load_FileType();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete statusLabelOne;
    statusLabelOne=NULL;
    delete statusLabelTwo;
    statusLabelTwo=NULL;
    delete statusLabelThree;
    statusLabelThree=NULL;
    delete statusLabelFour;
    statusLabelFour=NULL;
    delete statusLabelFive;
    statusLabelFive=NULL;
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
    statusLabelOne->setText(tr("耗时:"));
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
            codeInfo.insert(codeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString(),codeInfoIni.value(agencyInfo.at(i)+"/NAME").toString());
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
                //名称信息
                QString sendInfo=codeInfo.value(sendCode);
                QString recInfo=codeInfo.value(recCode);
                QString fileTypeInfo=ofdFileInfo.value(fileTypeCode);
                //刷新UI
                ui->lineEditSendCode->setText(sendCode);
                ui->lineEditRecCode->setText(recCode);
                ui->lineEditFileTransferDate->setText(dateInfo);
                ui->lineEditFileType->setText(fileTypeCode);
                ui->lineEditSenfInfo->setText(sendInfo);
                ui->lineEditRecInfo->setText(recInfo);
                ui->lineEditFileDescribe->setText(fileTypeInfo);
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
                    QString sendInfo=codeInfo.value(sendCode);
                    QString recInfo=codeInfo.value(recCode);
                    QString fileIndexTypeInfo=indexFileInfo.value(fileIndexTypeCode);
                    //刷新UI
                    ui->lineEditSendCode->setText(sendCode);
                    ui->lineEditRecCode->setText(recCode);
                    ui->lineEditFileTransferDate->setText(dateInfo);
                    ui->lineEditFileType->setText(fileIndexTypeCode);
                    ui->lineEditSenfInfo->setText(sendInfo);
                    ui->lineEditRecInfo->setText(recInfo);
                    ui->lineEditFileDescribe->setText(fileIndexTypeInfo);
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
}

void MainWindow::on_fileOpen_triggered()
{
    open_file_Dialog();
}

void MainWindow::on_aboutOpen_triggered()
{
    QMessageBox::about(this,tr("关于本程序"),tr("本程序是一个可以解析格式化显示开放式基金数据交换协议的工具，用于解析如上所述协议所交换的各种文件\r\n作者:793554262@qq.com(刘德位)"));
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,tr("关于QT"));
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    open_file_Dialog();
}
