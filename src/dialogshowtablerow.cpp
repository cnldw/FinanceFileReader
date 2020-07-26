/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/
#include "src/dialogshowtablerow.h"
#include "ui_dialogshowtablerow.h"
#define UNUSED(x) (void)x

DialogShowTableRow::DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableRow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:13px;font-family:PingFangSC-Regular,sans-serif;");
    ui->tableWidget->setStyleSheet("font-size:13px;font-family:PingFangSC-Light,sans-serif;");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
#endif
    /**************************************************************/
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(4);
    ptr_table->setRowCount(rowdata->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    ptr_table->horizontalHeader()->setStretchLastSection(true);//关键

    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制"),this);
    QString text="对当前窗口进行截图保存(Ctrl+Alt+R)";
#ifdef Q_OS_MAC
    text="对当前窗口进行截图保存(command+Option+R)";
#endif
    action_ShowSaveScreen= new QAction(text,this);
    action_Magnify = new QAction(tr("放大镜"),this);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    connect(action_ShowSaveScreen, SIGNAL(triggered()), this, SLOT(saveScreen()));
    connect(action_Magnify, SIGNAL(triggered()), this, SLOT(showMagnify()));
    //截图快捷键
    QShortcut *_shortcut;
    _shortcut = new QShortcut(QKeySequence("Ctrl+Alt+R"), this);
    connect(_shortcut, SIGNAL(activated()),this,SLOT(saveScreen()));
    //复制快捷键-拦截系统的复制函数，使用我们写的
    QShortcut *_shortcut2;
    _shortcut2 = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(_shortcut2, SIGNAL(activated()),this,SLOT(copyToClipboard()));
    //设置表格列标题
    QStringList title;
    title.append("字段中文名");
    title.append("字段英文名");
    title.append("字段值");
    title.append("字典解释");
    ptr_table->setHorizontalHeaderLabels(title);

    //设置表格的内容
    if(rowdata->count()>0){
        for (int row = 0; row < rowdata->count(); ++row)
        {
            for(int col=0;col<4;col++){
                if(col==0){
                    //第5个字段是tips
                    QString tip=((QString)rowdata->at(row).at(4));
                    QTableWidgetItem *item= new QTableWidgetItem(rowdata->at(row).at(col));
                    if(!tip.isEmpty()){
                        item->setToolTip(tip);
                    }
                    ptr_table->setItem(row, col, item);
                }
                else{
                    QTableWidgetItem *item= new QTableWidgetItem(rowdata->at(row).at(col));
                    ptr_table->setItem(row, col, item);
                }
            }
        }
        ptr_table->resizeColumnsToContents();
        //表格初始化完毕后，搜索默认以00开始
        searchColumn=0;
        searchRow=0;
    }
}

DialogShowTableRow::~DialogShowTableRow()
{
    delete action_ShowCopyColum;
    delete action_ShowSaveScreen;
    delete action_Magnify;
    delete tablePopMenu;
    delete ui;
}

void DialogShowTableRow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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
    //复制
    tablePopMenu->addAction(action_ShowCopyColum);
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    int rangeCount=itemsRange.count();
    if(rangeCount==1){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个单元格--允许使用放大镜
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            tablePopMenu->addAction(action_Magnify);
        }
    }
    //截屏
    tablePopMenu->addAction(action_ShowSaveScreen);
    tablePopMenu->exec(QCursor::pos());
}

void DialogShowTableRow::copyToClipboard(){
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
    }
    else if(rangeCount>1){
        QMessageBox::warning(this,tr("警告"),"无法对多重选择区域执行复制!",QMessageBox::Ok,QMessageBox::Ok);
    }
}

void DialogShowTableRow::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ptr_table->rowCount();
        int colcount=ptr_table->columnCount();
        //开始搜索
        //行
        for(int i=searchRow;i<rowcount;i++){
            //搜索开始行时，列从searchCol开始
            if(i==searchRow){
                for(int j=searchColumn;j<colcount;j++){
                    //搜索到了
                    if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(i,j);
                        ptr_table->setFocus();
                        return;
                    }
                    //如果搜索到了最后一行最后一列都没搜到则将搜索起始点转移到第一行第一列
                    if(i==(rowcount-1)&&j==(colcount-1)){
                        this->searchRow=0;
                        this->searchColumn=0;
                    }
                }
            }
            //非搜索起始行，则从第0列搜索
            else{
                for(int j=0;j<colcount;j++){
                    //搜索到了
                    if(ptr_table->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(i,j);
                        ptr_table->setFocus();
                        return;
                    }
                    //如果搜索到了最后一行最后一列都没搜到则将搜索起始点转移到第一行第一列
                    if(i==(rowcount-1)&&j==(colcount-1)){
                        this->searchRow=0;
                        this->searchColumn=0;
                    }
                }
            }
        }
    }
}

void DialogShowTableRow::on_tableWidget_itemSelectionChanged()
{
    //选择的范围
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    //范围和
    int rangeCount=itemsRange.count();
    //如果仅仅选择了一个单元格，先更新当前选择的单元格
    ///////////////////////////////////////////////////
    if(rangeCount==1&&itemsRange.at(0).leftColumn()==itemsRange.at(0).rightColumn()&&itemsRange.at(0).topRow()==itemsRange.at(0).bottomRow()){
        int rowcount=ptr_table->rowCount();
        int colcount=ptr_table->columnCount();
        //记录当前所在行
        this->searchRow=itemsRange.at(itemsRange.count()-1).bottomRow();
        //当前所在列
        this->searchColumn=itemsRange.at(itemsRange.count()-1).rightColumn();
        //焦点在最后一个单元格时，转移到第一行第一列，其余情况起始列+1
        if(searchRow==(rowcount-1)&&searchColumn==(colcount-1)){
            this->searchRow=0;
            this->searchColumn=0;
        }
        else{
            this->searchColumn+=1;
        }
    }
}

void DialogShowTableRow::saveScreen(){
    QGuiApplication::primaryScreen();
    //获取截图
    QPixmap p = this->grab();
    //获取桌面路径拼接文件路径
    QString filePathName=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/FFReader截图";
    filePathName += QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    filePathName += ".png";
    //弹出保存框
    QString selectedFilter=Q_NULLPTR;
    filePathName = QFileDialog::getSaveFileName(this,("截图另存为"),filePathName,tr("PNG(*.png)"),&selectedFilter);
    if(!filePathName.isEmpty()){
        if(selectedFilter=="PNG(*.png)"&&(!filePathName.endsWith(".png"))){
            filePathName.append(".png");
        }
        //开始进行截图
        p.save(filePathName,"png");
    }
}

/**
 * @brief DialogShowTableRow::showMagnify 打开放大镜
 */
void DialogShowTableRow::showMagnify(){
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
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
                DialogMagnify * dialog = new DialogMagnify(text,this);
                dialog->setWindowTitle(QString("数据放大镜"));
                dialog->setModal(false);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                dialog->show();
                dialog->raise();
                dialog->activateWindow();
            }
        }
    }
}

