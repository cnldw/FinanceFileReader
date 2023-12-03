/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogmodifyrow.h"
#include "ui_dialogmodifyrow.h"
#include "src/publicdefine.h"

DialogModifyRow::DialogModifyRow(OFDFileDefinition *ofd,QStringList rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyRow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsMacOS));
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsLinux));
#endif
#ifdef Q_OS_WIN32
    this->setStyleSheet(UIFontsWindows);
#endif
    /**************************************************************/
    this->rowDataOld=rowdata;
    this->rowDataNew=rowdata;
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label->setPalette(pe);
    this->ofd=*ofd;
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(5);
    ptr_table->setRowCount(ofd->getFieldCount());
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
    if(ofd->getFieldCount()){
        for (int row = 0; row < ofd->getFieldCount(); ++row)
        {
            //第一列--字段中文名
            QTableWidgetItem *item= new QTableWidgetItem(ofd->getFieldList().at(row).getFieldDescribe());
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            ptr_table->setItem(row, 0, item);
            //第二列--字段类型
            QString filedType=ofd->getFieldList().at(row).getFieldType();
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
            QTableWidgetItem *item3= new QTableWidgetItem(QString::number(ofd->getFieldList().at(row).getLength()));
            item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
            ptr_table->setItem(row, 2, item3);
            //第四列-字段小数长度
            QTableWidgetItem *item4= new QTableWidgetItem(QString::number(ofd->getFieldList().at(row).getDecLength()));
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
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    this->searchRow=currentRow+1;
}

/**
 * @brief DialogModifyRow::on_pushButton_clicked 搜索功能
 */
void DialogModifyRow::on_pushButton_clicked()
{
    if(searchRow>=ptr_table->rowCount()){
        this->searchRow=0;
    }
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        //开始搜索
        //行
        for(int i=searchRow;i<rowcount;i++){
            //搜索到了
            if(ptr_table->item(i,0)->text().contains(text,Qt::CaseInsensitive)){
                QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(i,0,i,ptr_table->columnCount()-1);
                ptr_table->clearSelection();
                ptr_table->setRangeSelected(addRange,true);
                ptr_table->setFocus();
                this->searchRow=i+1;
                return;
            }
            //如果搜索到了最后一行都没搜到则将搜索起始点转移到第一行
            if(i==(rowcount-1)){
                this->searchRow=0;
            }
        }
    }
}
void DialogModifyRow::checkField(int row,int column,bool updateValue,bool displayErrorMessage){
    if(column==4&&row<(ptr_table->rowCount())){
        //OFD的文件编码
        //内容
        QString text=ptr_table->item(row,column)->text();
        //获取字段类别等信息
        QString fieldDesc=ptr_table->item(row,0)->text();
        QString filedType=this->ofd.getFieldList().at(row).getFieldType();
        int filedLength=this->ofd.getFieldList().at(row).getLength();
        int filedDecLength=this->ofd.getFieldList().at(row).getDecLength();
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
