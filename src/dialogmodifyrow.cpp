/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dialogmodifyrow.h"
#include "ui_dialogmodifyrow.h"
#include "src/msgtoast.h"
#include "src/utils.h"

DialogModifyRow::DialogModifyRow(QList<fieldType> fieldList,QTextCodec *codec,QStringList rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyRow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    Utils::setDefaultWindowFonts(this);
    this->rowDataOld=rowdata;
    this->rowDataNew=rowdata;
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label->setPalette(pe);
    this->fieldList=fieldList;
    this->codec=codec;
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(5);
    ptr_table->setRowCount(fieldList.count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setStretchLastSection(true);//关键
    ptr_table->setFocus();
    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    //设置表格列标题
    QStringList title;
    title.append("字段中文名");
    title.append("字段类型");
    title.append("字段最大长度");
    title.append("字段小数长度");
    title.append("字段值");
    ptr_table->setHorizontalHeaderLabels(title);
    ptr_table->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //设置表格的内容
    if(fieldList.count()){
        for (int row = 0; row < fieldList.count(); ++row)
        {
            //第一列--字段中文名
            QTableWidgetItem *item= new QTableWidgetItem(fieldList.at(row).fieldDescribe);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            ptr_table->setItem(row, 0, item);
            //第二列--字段类型
            QString filedType=fieldList.at(row).fieldType;
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
            QTableWidgetItem *item2= new QTableWidgetItem(type);
            item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
            ptr_table->setItem(row, 1, item2);
            //第三列-字段长度
            QTableWidgetItem *item3= new QTableWidgetItem(QString::number(fieldList.at(row).fieldLength));
            item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
            ptr_table->setItem(row, 2, item3);
            //第四列-字段小数长度
            QTableWidgetItem *item4= new QTableWidgetItem(QString::number(fieldList.at(row).fieldDecLength));
            item4->setFlags(item4->flags() & (~Qt::ItemIsEditable));
            ptr_table->setItem(row, 3, item4);
            //第五列-字段值
            QTableWidgetItem *item5= new QTableWidgetItem(rowdata.count()>row?rowdata.at(row):"");
            ptr_table->setItem(row, 4, item5);
        }
        ptr_table->resizeColumnsToContents();
    }
}

void DialogModifyRow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    posCurrentMenu=pos;
    //判断当前鼠标位置是不是在表格单元格位置内
    //空表
    if(ptr_table->rowCount()<1){
        return;
    }
    //如果鼠标点击的是表格的空白位置取到得行y是-1
    if( ptr_table->rowAt(pos.y()) <0){
        return;
    }
    tablePopMenu->clear();
    tablePopMenu->addAction(action_ShowCopyColum);
    tablePopMenu->exec(QCursor::pos());
}

void DialogModifyRow::copyToClipboard(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount==1){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个单元格复制
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            QString text="";
            QClipboard *board = QApplication::clipboard();
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
            }
            board->setText(text);
        }
        //多个单元格复制
        else{
            QString value="";
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
            QClipboard *board = QApplication::clipboard();
            board->setText(value);
        }
        Toast::showMsg(QString("已复制数据到剪切板~"), ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
    else if(rangeCount>1){
        Toast::showMsg(QString("无法对多重选择区域执行复制！"), ToastTime::Time::ToastTime_normal,ToastType::Type::ToastType_warn,this);
    }
}

QStringList DialogModifyRow::getRowDataNew() const
{
    return rowDataNew;
}


DialogModifyRow::~DialogModifyRow()
{
    delete ui;
}

void DialogModifyRow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    int rowcount=ptr_table->rowCount();
    int colcount=ptr_table->columnCount();
    this->searchRow=currentRow;
    this->searchColumn=currentColumn;
    //焦点在最后一个单元格时，转移到第一行第一列，其余情况起始列+1
    if(searchRow==(rowcount-1)&&searchColumn==(colcount-1)){
        this->searchRow=0;
        this->searchColumn=0;
    }
    else{
        this->searchColumn+=1;
        //溢出到下一行
        if(searchColumn>colcount-1){
            if(searchRow<rowcount-1){
                searchRow+=1;
                searchColumn=0;
            }
            else{
                searchRow=0;
                searchColumn=0;
            }
        }
    }
}

/**
 * @brief DialogModifyRow::on_pushButton_clicked 搜索功能
 */
void DialogModifyRow::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        int colcount=ptr_table->columnCount();
        //逻辑--搜到了就允许继续往下搜，搜不到就搜一圈停止
        int beginrow=searchRow;
        int begincol=searchColumn;
        int col=0;
        for(int i=beginrow;i<rowcount;i++){
            if(i==beginrow){
                col=begincol;
            }
            else{
                col=0;
            }
            for(int j=col;j<colcount;j++){
                if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(i,j);
                    ptr_table->setFocus();
                    return;
                }
            }
        }
        int againendrow=0;
        int againendcol=0;
        if(begincol>0){
                againendrow=beginrow;
                againendcol=begincol-1;
        }
        else if(beginrow>0&&begincol==0){
            againendrow=beginrow-1;
            againendcol=colcount-1;
        }
        else if(beginrow==0&&begincol==0){
            Toast::showMsg("搜索了一圈,没找到你要搜索的内容...", ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
            return;
        }
        int colend;
        for(int i=0;i<=againendrow;i++){
            if(i==againendrow){
                colend=againendcol;
            }
            else{
                colend=colcount-1;
            }
            for(int j=0;j<=colend;j++){
                if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ptr_table->setCurrentCell(i,j);
                    ptr_table->setFocus();
                    return;
                }
            }
        }
        Toast::showMsg("搜索了一圈,没找到你要搜索的内容...", ToastTime::Time::ToastTime_short,ToastType::Type::ToastType_info,this);
    }
}

void DialogModifyRow::checkField(int row,int column,bool updateValue,bool displayErrorMessage){
    if(column==4&&row<(ptr_table->rowCount())){
        //OFD的文件编码
        //内容
        QString text=ptr_table->item(row,column)->text();
        //获取字段类别等信息
        QString fieldDesc=ptr_table->item(row,0)->text();
        QString filedType=this->fieldList.at(row).fieldType;
        int filedLength=this->fieldList.at(row).fieldLength;
        int filedDecLength=this->fieldList.at(row).fieldDecLength;
        int textLength=codec->fromUnicode(text).length();
        //字符类和文本类的只需要判断长度
        if(filedType=="C"||filedType=="TEXT"){
            //长度校验
            if(textLength>filedLength){
                this->errorHash.insert(row,QString("字段[%1]长度不应当大于[%2],请修正").arg(fieldDesc).arg(filedLength));
            }
            //字段可以被接受
            else if(this->errorHash.contains(row)){
                this->errorHash.remove(row);
            }
        }
        //数字字符型
        else if(filedType=="A"){
            //长度校验
            if(textLength>filedLength){
                this->errorHash.insert(row,QString("字段[%1]长度不应当大于[%2],请修正").arg(fieldDesc).arg(filedLength));
            }
            //字段可以被接受
            else if(this->errorHash.contains(row)){
                this->errorHash.remove(row);
            }
        }
        //数字型
        else if(filedType=="N"){
            //如果是千位分隔符
            if(text.contains(",")){
                text=text.replace(",","");
            }
            if(!text.isEmpty()){
                bool ok=false;
                text.toDouble(&ok);
                //如果无法转化为Double直接认为不合规//兼容负数
                if(!ok){
                    this->errorHash.insert(row,QString("字段[%1]应当是一个数值或者为空,但是现在不是,请修正").arg(fieldDesc));
                }
                else{
                    //无小数部分
                    if(!text.contains("."))
                    {
                        if(textLength>(filedLength-filedDecLength)){
                            this->errorHash.insert(row,QString("字段[%1]整数部分长度不能超过%2").arg(fieldDesc).arg(filedLength-filedDecLength));
                        }
                        //字段可以被接受
                        else if(this->errorHash.contains(row)){
                            this->errorHash.remove(row);
                        }
                    }
                    //如果包含小数点
                    else{
                        //拆分出来整数和小数
                        int index=text.indexOf(".");
                        //整数部分
                        QString intS=text.mid(0,index);
                        //整数部分有问题
                        if(intS.length()>(filedLength-filedDecLength)){
                            this->errorHash.insert(row,QString("字段[%1]的整数部分长度不能超过%2").arg(fieldDesc).arg(filedLength-filedDecLength));
                        }
                        //整数部分没问题开始检查小数部分
                        else {
                            //小数部分
                            QString intD=text.mid(index+1,-1);
                            if(intD.length()>filedDecLength){
                                this->errorHash.insert(row,QString("字段[%1]的小数部分长度不能超过%2").arg(fieldDesc).arg(QString::number(filedDecLength)));
                            }
                            //都没问题
                            //字段可以被接受
                            else if(this->errorHash.contains(row)){
                                this->errorHash.remove(row);
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
                this->errorHash.insert(row,QString("字段[%1]的长度不应当大于[%2],请修正").arg(fieldDesc).arg(filedLength));
            }
            //字段可以被接受
            else if(this->errorHash.contains(row)){
                this->errorHash.remove(row);
            }
        }
        if(displayErrorMessage){
            //如果有毛病，则显示毛病
            if(this->errorHash.contains(row)){
                ui->label->setText(this->errorHash.value(row));
            }
            else{
                ui->label->setText(nullptr);
                this->change=true;
            }
        }
        //更新记录值
        if(updateValue){
            this->rowDataNew.replace(row,text);
        }
    }
}

void DialogModifyRow::on_tableWidget_cellChanged(int row, int column)
{
    //在保存退出前不更新记录值
    checkField(row,column,false,true);
}

bool DialogModifyRow::getSaveFlag() const
{
    return saveFlag;
}

bool DialogModifyRow::getChange() const
{
    return change;
}

void DialogModifyRow::on_pushButtonCancal_clicked()
{
    this->saveFlag=false;
    this->close();
}

void DialogModifyRow::on_pushButtonSave_clicked()
{
    qDebug()<<rowDataNew;
    //保存前做前校验
    int count=ptr_table->rowCount();
    //不要删除这个代码，否则将会引发bug，比如你打开编辑功能后修改了一个单元格后没切换到其他单元格直接点击保存，这个修改的单元格还未提交数据，会有问题，我们强制在保存时切换到0，0单元格
    ptr_table->setCurrentCell(0,0);
    for(int row=0;row<count;row++){
        checkField(row,4,true,false);
    }
    //如果有待修改的错误，则不准保存
    if(this->errorHash.count()>0){
        QList<int> keys=this->errorHash.keys();
        //排序
        std::sort(keys.begin(), keys.end());
        ui->label->setText(this->errorHash.value(keys.first()));
        ptr_table->setCurrentCell(keys.first(),4);
        ptr_table->setFocus();
        return;
    }
    //没错误
    else{
        //这里做个check，看看是不是真的修改了数据
        bool existDifferent=false;
        for(int i=0;i<rowDataOld.count();i++){
            if(rowDataOld.at(i)!=rowDataNew.at(i)){
                existDifferent=true;
                break;
            }
        }
        if(existDifferent){
            this->change=true;
        }
        else{
            this->change=false;
        }
        this->saveFlag=true;
        //退出
        this->close();
    }
}
