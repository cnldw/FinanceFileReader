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
#include "src/dialogshowtablecompareview.h"
#include "ui_dialogshowtablecompareview.h"
#define UNUSED(x) (void)x

DialogShowTableCompareView::DialogShowTableCompareView(QStringList title,QStringList fieldType,QMap<int,QStringList> * compareData,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableCompareView)
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
    //初始化背景色
    this->backcolor=QColor (241,226,173);
    //初始化表格
    ptr_table =ui->tableWidget;
    ptr_table->setContextMenuPolicy (Qt::CustomContextMenu);
    ptr_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ptr_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    ptr_table->setColumnCount(title.count());
    ptr_table->setRowCount(compareData->count());
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);

    //表格右键菜单
    tablePopMenu = new QMenu(ptr_table);
    action_ShowCopyColum = new QAction(tr("复制"),this);
    QString text="对当前窗口进行截图保存(Ctrl+Alt+R)";
#ifdef Q_OS_MAC
    text="对当前窗口进行截图保存(command+Option+R)";
#endif
    action_ShowSaveScreen= new QAction(text,this);
    tablePopMenu->addAction(action_ShowCopyColum);
    tablePopMenu->addAction(action_ShowSaveScreen);
    connect(action_ShowCopyColum, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    connect(action_ShowSaveScreen, SIGNAL(triggered()), this, SLOT(saveScreen()));
    //截图快捷键
    QShortcut *_shortcut;
    _shortcut = new QShortcut(QKeySequence("Ctrl+Alt+R"), this);
    connect(_shortcut, SIGNAL(activated()),this,SLOT(saveScreen()));
    //复制快捷键-拦截系统的复制函数，使用我们写的
    QShortcut *_shortcut2;
    _shortcut2 = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(_shortcut2, SIGNAL(activated()),this,SLOT(copyToClipboard()));
    //设置表格标题
    ptr_table->setHorizontalHeaderLabels(title);
    //获取数据行
    QList<int> keys=compareData->keys();
    //排序
    std::sort(keys.begin(), keys.end());
    //数据显示
    if(keys.count()>0){
        for(int row=0;row<keys.count();row++){
            //第一列显示数据的原始行号
            QTableWidgetItem *item= new QTableWidgetItem(QString::number(keys.at(row)));
            ptr_table->setItem(row, 0, item);
            //从第二列开始为数据内容
            for(int col=1;col<ptr_table->columnCount();col++){
                QString value="";
                //col是从1开始
                if(col<=compareData->value(keys.at(row)).count()){
                    value=compareData->value(keys.at(row)).at(col-1);
                }
                //比对器这里自己进行小数千位分隔符处理
                if(fieldType.at(col-1)=="N"){
                    value=Utils::CovertDoubleQStringWithThousandSplit(value);
                }
                QTableWidgetItem *item2= new QTableWidgetItem(value);
                //判断是否是数字，数字右对齐
                if(fieldType.at(col-1)=="N"){
                    item2->setTextAlignment(Qt::AlignRight);
                }
                ptr_table->setItem(row, col, item2);
                //从第二行开始，开始判断是否不一样，一旦有不一样的直接就标记此列有不一样的
                if(row==1){
                    //如果第一行不等于第二行
                    if(ptr_table->item(row,col)->text()!=ptr_table->item(row-1,col)->text()){
                        //插入不等标记
                        colNoEqual.append(col);
                        for(int i2=0;i2<=row;i2++){
                            ptr_table->item(i2,col)->setBackgroundColor(backcolor);
                        }
                    }
                }else if(row>1){
                    //先判断前几行是否不相等,如果不相等直接更新本行颜色即可
                    if(colNoEqual.contains(col)){
                        ptr_table->item(row,col)->setBackgroundColor(backcolor);
                    }
                    else{
                        //前N行相等,则拿本行和上一行对比,并且如果检测到不一致,需从第一行开始更新背景色
                        if(ptr_table->item(row,col)->text()!=ptr_table->item(row-1,col)->text()){
                            //插入不等标记
                            colNoEqual.append(col);
                            for(int i2=0;i2<=row;i2++){
                                ptr_table->item(i2,col)->setBackgroundColor(backcolor);
                            }
                        }
                    }
                }
            }
        }
        ptr_table->resizeColumnsToContents();
    }
}

DialogShowTableCompareView::~DialogShowTableCompareView()
{
    delete action_ShowCopyColum;
    delete tablePopMenu;
    delete ui;
}

void DialogShowTableCompareView::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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
    tablePopMenu->exec(QCursor::pos());
}

void DialogShowTableCompareView::copyToClipboard(){
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

void DialogShowTableCompareView::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    UNUSED(previousRow);
    UNUSED(previousColumn);
    //记录当前所在行
    rowcurrent=currentRow;
    //当前所在列
    colcurrent=currentColumn;
    //更新列跳转搜索开始列
    colSearch=currentColumn;
}

void DialogShowTableCompareView::on_pushButton_clicked()
{
    QString text=ui->lineEdit->text();
    if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
        bool returnSearch=false;
        int begin=colSearch;
        if((colSearch+1)>=(ptr_table->columnCount()-1)){
            colSearch=-1;
        }
        colSearch+=1;
        for(;colSearch<ptr_table->columnCount();colSearch++){
            if((ptr_table->horizontalHeaderItem(colSearch)->text()).contains(text,Qt::CaseInsensitive)){
                ptr_table->setCurrentCell(rowcurrent,colSearch);
                ptr_table->setFocus();
                if(colSearch==begin){
                    //如果搜了一圈在开始搜索的列找到了目标列，刷新下提示
                    on_tableWidget_currentCellChanged(rowcurrent,colcurrent,0,0);
                }
                return;
            }
            //如果是第二圈搜索且搜索到了开始位置,则终止搜索
            if(returnSearch&&colSearch>=begin){
                return;
            }
            //搜到最后一列还没找到,则跳到第1列继续,保证搜索形成一个圆环
            else if(colSearch>=(ptr_table->columnCount()-1)){
                colSearch=-1;
                returnSearch=true;
            }
        }
    }
}

void DialogShowTableCompareView::on_pushButton_2_clicked()
{
    if(colNoEqual.isEmpty()){
        return;
    }else{
        if(colcurrent>0){
            for(int col=colcurrent-1;col>=0;col--){
                if(colNoEqual.contains(col)){
                    ptr_table->setCurrentCell(rowcurrent,col);
                    ptr_table->setFocus();
                    break;
                }
            }
        }
    }
}

void DialogShowTableCompareView::on_pushButton_3_clicked()
{
    if(colNoEqual.isEmpty()){
        return;
    }else{
        if(colcurrent<ptr_table->columnCount()){
            for(int col=colcurrent+1;col<ptr_table->columnCount();col++){
                if(colNoEqual.contains(col)){
                    ptr_table->setCurrentCell(rowcurrent,col);
                    ptr_table->setFocus();
                    break;
                }
            }
        }

    }
}

void DialogShowTableCompareView::saveScreen(){
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
