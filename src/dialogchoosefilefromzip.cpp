/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogchoosefilefromzip.h"
#include "ui_dialogchoosefilefromzip.h"
#include "src/utils.h"
#include "src/msgtoast.h"
#include <QTextCodec>

DialogChooseFileFromZip::DialogChooseFileFromZip(QString zipPathPar,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseFileFromZip)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    Utils::setDefaultWindowFonts(this);
    ui->label->setText(messageText);
    zipPath=zipPathPar;
    ptr_table =ui->tableWidget;
    ui->pushButton->setEnabled(false);
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptr_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ptr_table->setColumnCount(3);
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ptr_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ptr_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ptr_table->setColumnWidth(0, 60);
    ptr_table->setColumnWidth(2, 100);
    QStringList title;
    title.append("编号");
    title.append("文件路径");
    title.append("文件大小");
    ptr_table->setHorizontalHeaderLabels(title);
    initZipTable();
}

DialogChooseFileFromZip::~DialogChooseFileFromZip()
{
    delete ui;
}

void DialogChooseFileFromZip::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    chooeseIndex=currentRow;
    if(zipFileInfoList.at(chooeseIndex).size>1073741824){
        ui->label->setText("暂不支持在工具中直接解压超过1GB的文件,请尝试使用外部解压工具解压后解析");
    }
    else{
        if(ui->checkBox->isChecked()){
            extractAll=true;
            ui->label->setText(messageTextextractAll);
        }
        else{
            extractAll=false;
            ui->label->setText(messageText);
        }
    }
}

int DialogChooseFileFromZip::getChooeseIndex() const
{
    return chooeseIndex;
}

void DialogChooseFileFromZip::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    chooeseIndex=item->row();
    if(zipFileInfoList.at(chooeseIndex).size>1073741824){
        ui->label->setText("暂不支持在工具中直接解压超过1GB的文件,请尝试使用外部解压工具解压后解析");
    }
    else{
        confirm=true;
        this->close();
    }
}

bool DialogChooseFileFromZip::getConfirm() const
{
    return confirm;
}

void DialogChooseFileFromZip::on_pushButton_clicked()
{
    if(zipFileInfoList.at(chooeseIndex).size>1073741824){
        ui->label->setText("暂不支持在工具中直接解压超过1GB的文件,请尝试使用外部解压工具解压后解析");
    }
    else{
        confirm=true;
        this->close();
    }
}

bool DialogChooseFileFromZip::getExtractAll() const
{
    return extractAll;
}

void DialogChooseFileFromZip::on_checkBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if(ui->checkBox->isChecked()){
        extractAll=true;
        ui->label->setText(messageTextextractAll);
    }
    else{
        extractAll=false;
        ui->label->setText(messageText);
    }
}

void DialogChooseFileFromZip::on_pushButton_2_clicked()
{
    confirm=false;
    this->close();
}

void DialogChooseFileFromZip::on_pushButton_3_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        int beginrow=chooeseIndex;
        if(chooeseIndex<rowcount-1){
            beginrow+=1;
        }
        for(int i=beginrow;i<rowcount;i++){
            if(ptr_table->item(i,1)->text().contains(text,Qt::CaseInsensitive)){
                ptr_table->setCurrentCell(i,1);
                ptr_table->setFocus();
                return;
            }
        }
        int againendrow=0;
        if(beginrow>0){
            againendrow=beginrow-1;
        }
        for(int i=0;i<=againendrow;i++){
            if(ptr_table->item(i,1)->text().contains(text,Qt::CaseInsensitive)){
                ptr_table->setCurrentCell(i,1);
                ptr_table->setFocus();
                return;
            }
        }
        Toast::showMsg("搜索了一圈,没找到你要搜索的内容...", ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
}

const QMap<QString, QDateTime> &DialogChooseFileFromZip::getZipFileListMdTime() const
{
    return zipFileListMdTime;
}

const QList<QZipReader::FileInfo> &DialogChooseFileFromZip::getZipFileInfoList() const
{
    return zipFileInfoList;
}

void DialogChooseFileFromZip::initZipTable(){
    QFile file(zipPath);
    if (!file.open(QIODevice::ReadOnly)){
        ui->label->setText(QString("无法打开zip文件[%1],请重试...").arg(zipPath));
        ptr_table->setRowCount(0);
        return;
    }
    QTextCodec *systemCodec=QTextCodec::codecForName("System");
    QTextCodec *zipCodec=QTextCodec::codecForName(zipFileUseCodec.toLocal8Bit());
    QTextCodec::setCodecForLocale(zipCodec);
    QZipReader reader(&file);
    if (!reader.isReadable()){
        ui->label->setText(QString("无法读取zip文件[%1],请重试...").arg(zipPath));
        ptr_table->setRowCount(0);
        QTextCodec::setCodecForLocale(systemCodec);
        return;
    }
    const auto fileList = reader.fileInfoList();
    zipFileListMdTime.clear();
    zipFileInfoList.clear();
    fileSizeSum=0;
    for (auto it = fileList.begin(); it != fileList.end(); ++it){
        if (it->isFile && !it->isSymLink&&!it->filePath.contains(".DS_Store")&&!it->filePath.startsWith("__MACOSX")){
            zipFileInfoList.append(*it);
            zipFileListMdTime.insert(it->filePath,it->lastModified);
            fileSizeSum+=it->size;
        }
    }
    reader.close();
    file.close();
    //重新获取一份原始编码的文件路径
    QTextCodec::setCodecForLocale(systemCodec);
    if (!file.open(QIODevice::ReadOnly)){
        ui->label->setText(QString("无法打开zip文件[%1],请重试...").arg(zipPath));
        ptr_table->setRowCount(0);
        return;
    }
    QZipReader readerSystemCodec(&file);
    if (!readerSystemCodec.isReadable()){
        ui->label->setText(QString("无法读取zip文件[%1],请重试...").arg(zipPath));
        ptr_table->setRowCount(0);
        return;
    }
    const auto fileListSystemCodec = readerSystemCodec.fileInfoList();
    zipFileNameSystemCodecList.clear();
    //这个顺序要和上面的一模一样，否则文件读取会错位
    for (auto it = fileListSystemCodec.begin(); it != fileListSystemCodec.end(); ++it){
        if (it->isFile && !it->isSymLink&&!it->filePath.contains(".DS_Store")&&!it->filePath.startsWith("__MACOSX")){
            zipFileNameSystemCodecList.append(it->filePath);
        }
    }
    readerSystemCodec.close();
    file.close();
    if(zipFileInfoList.count()>0){
        ptr_table->setRowCount(zipFileInfoList.count());
        for (int i=0;i<zipFileInfoList.count();i++){
            QTableWidgetItem *itemno= new QTableWidgetItem(QString::number(i+1));
            itemno->setTextAlignment(Qt::AlignCenter);
            ptr_table->setItem(i, 0, itemno);
            QTableWidgetItem *item1= new QTableWidgetItem(zipFileInfoList.at(i).filePath);
            ptr_table->setItem(i, 1, item1);
            QTableWidgetItem *item2= new QTableWidgetItem(Utils::formatFileSize(zipFileInfoList.at(i).size));
            ptr_table->setItem(i, 2, item2);
        }
        ptr_table->selectRow(0);
        QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(0,0,0,ptr_table->columnCount()-1);
        ptr_table->setRangeSelected(addRange,true);
        ptr_table->setFocus();
        ui->pushButton->setEnabled(true);
        if(fileSizeSum>1073741824){
            ui->checkBox->setEnabled(false);
            ui->checkBox->setToolTip("压缩包中的文件体积太大了(超过了1GB),暂不支持在工具内直接提取全部文件");
        }
    }
    else{
        ui->label->setText(QString("没有从zip文件[%1]中读取到任何有效的文件,请重试...").arg(zipPath));
        ui->pushButton->setEnabled(false);
        return;
    }
}

void DialogChooseFileFromZip::on_comboBox_currentIndexChanged(int index)
{   switch (index) {
    case 0:
        zipFileUseCodec="System";
        initZipTable();
        break;
    case 1:
        zipFileUseCodec="UTF-8";
        initZipTable();
        break;
    case 2:
        zipFileUseCodec="GB18030";
        initZipTable();
        break;
    default:
        zipFileUseCodec="System";
        initZipTable();
        break;
    }
}

QString DialogChooseFileFromZip::getZipFileUseCodec() const
{
    return zipFileUseCodec;
}

QList<QString> DialogChooseFileFromZip::getZipFileNameSystemCodecList() const
{
    return zipFileNameSystemCodecList;
}
