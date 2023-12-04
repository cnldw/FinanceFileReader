/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "formfieldcheckedittools.h"
#include "ui_formfieldcheckedittools.h"

FormFieldCheckEditTools::FormFieldCheckEditTools(QList<ConfigFile<OFDFileDefinition>> ofdConfigListPar,QList<ConfigFile<CsvFileDefinition>> csvConfigListPar,QList<ConfigFile<FIXEDFileDefinition>> fixedConfigListPar,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormFieldCheckEditTools)
{
    ui->setupUi(this);
    this->setWindowTitle(windowName);
    Utils::setDefaultWindowFonts(this);

    ofdConfigList=ofdConfigListPar;
    csvConfigList=csvConfigListPar;
    fixedConfigList=fixedConfigListPar;

    ui->textBrowserVisualizationConfig->setReadOnly(true);


    //初始化字段列表表格标题
    ui->tableWidgetFieldList->setColumnCount(5);
    QStringList title;
    title.append("字段中文名");
    title.append("字段英文名");
    title.append("条件值");
    title.append("必填");
    title.append("主键");
    ui->tableWidgetFieldList->setHorizontalHeaderLabels(title);
    //设置表格的选择方式
    ui->tableWidgetFieldList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetFieldList->verticalHeader()->setDefaultSectionSize(22);
    ui->tableWidgetFieldList->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableWidgetFieldList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidgetFieldList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidgetFieldList->setColumnWidth(2,80);
    ui->tableWidgetFieldList->setColumnWidth(3,60);
    ui->tableWidgetFieldList->setColumnWidth(4,60);
    ui->tableWidgetFieldList->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableWidgetFieldList->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidgetFieldList->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableWidgetFieldList->setAlternatingRowColors(true);


    //字段必填列表
    ui->tableWidgetCheckList->setContextMenuPolicy (Qt::CustomContextMenu);
    ui->tableWidgetCheckList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetCheckList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetCheckList->setColumnCount(2);
    ui->tableWidgetCheckList->setAlternatingRowColors(true);
    ui->tableWidgetCheckList->verticalHeader()->setDefaultSectionSize(22);
    ui->tableWidgetCheckList->horizontalHeader()->setStretchLastSection(true);//关键
    ui->tableWidgetCheckList->setWordWrap(true);
    ui->tableWidgetCheckList->setAlternatingRowColors(true);

    QStringList title2;
    title2.append("条件描述");
    title2.append("必填字段");
    ui->tableWidgetCheckList->setHorizontalHeaderLabels(title2);

    //条件分组
    ui->tableWidgeConditionList->setContextMenuPolicy (Qt::CustomContextMenu);
    ui->tableWidgeConditionList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgeConditionList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgeConditionList->setColumnCount(1);
    ui->tableWidgeConditionList->setAlternatingRowColors(true);
    ui->tableWidgeConditionList->verticalHeader()->setDefaultSectionSize(22);
    ui->tableWidgeConditionList->horizontalHeader()->setStretchLastSection(true);//关键
    ui->tableWidgeConditionList->setWordWrap(true);
    ui->tableWidgeConditionList->setAlternatingRowColors(true);

    QStringList title3;
    title3.append("条件分组列表");
    ui->tableWidgeConditionList->setHorizontalHeaderLabels(title3);

    if(ofdConfigList.count()>0){
        for(int l1=0;l1<ofdConfigList.count();l1++){
            ui->comboBox_2->addItem(ofdConfigList.at(l1).getFileName());
        }
        //第二层初始化第一个配置文件
        if(!ofdConfigList.at(0).getConfigSegmentList().isEmpty()){
            for (int l2=0;l2<ofdConfigList.at(0).getConfigSegmentList().count();l2++){
                ui->comboBox_3->addItem(ofdConfigList.at(0).getConfigSegmentList().at(l2).getConfigSegment()+"-"+ofdConfigList.at(0).getConfigSegmentList().at(l2).getDescribe());
            }
            //初始化字段列表
            initCurrentItem(0,0,0);
        }
    }
    comboBox_1OK=true;
    comboBox_2OK=true;

    table_field_PopMenu = new QMenu(ui->tableWidgetFieldList);
    action_table_field_selectall=new QAction(tr("勾选所有字段为必填"),this);
    connect(action_table_field_selectall, SIGNAL(triggered()), this, SLOT(table_field_selectAll()));
    action_table_field_clear_all=new QAction(tr("清除所有字段必填勾选"),this);
    connect(action_table_field_clear_all, SIGNAL(triggered()), this, SLOT(table_field_clearAllSelet()));

    action_table_field_select_range=new QAction(tr("勾选选择的字段为必填"),this);
    connect(action_table_field_select_range, SIGNAL(triggered()), this, SLOT(table_field_selectRange()));
    action_table_field_clear_range=new QAction(tr("清除选择的字段必填勾选"),this);
    connect(action_table_field_clear_range, SIGNAL(triggered()), this, SLOT(table_field_clearRange()));


    action_table_field_clear_condition=new QAction(tr("清除所有已填写条件"),this);
    connect(action_table_field_clear_condition, SIGNAL(triggered()),this, SLOT(table_field_clearCondition()));

    table_check_PopMenu = new QMenu(ui->tableWidgetCheckList);
    action_table_check_add=new QAction(tr("新建一个字段必填规则并编辑"),this);
    connect(action_table_check_add, SIGNAL(triggered()), this, SLOT(checkAdd()));
    action_table_check_edit_select=new QAction(tr("编辑当前选中规则"),this);
    connect(action_table_check_edit_select, SIGNAL(triggered()), this, SLOT(fieldCheckEditSelect()));
    action_table_check_delete_select=new QAction(tr("删除当前选中规则"),this);
    connect(action_table_check_delete_select, SIGNAL(triggered()), this, SLOT(fieldCheckDeleteSelect()));

    table_condition_PopMenu= new QMenu(ui->tableWidgeConditionList);
    action_table_condition_add=new QAction(tr("新建一个条件规则并编辑"),this);
    connect(action_table_condition_add, SIGNAL(triggered()), this, SLOT(fieldConAdd()));
    action_table_condition_edit_select=new QAction(tr("编辑当前选中条件"),this);
    connect(action_table_condition_edit_select, SIGNAL(triggered()), this, SLOT(fieldConEditSelect()));
    action_table_condition_delete_select=new QAction(tr("删除当前选中条件"),this);
    connect(action_table_condition_delete_select, SIGNAL(triggered()), this, SLOT(fieldConDeleteSelect()));

    //初始化按钮状态
    ui->pushButton_ImportField->setEnabled(false);
    ui->pushButton_ImportCon->setEnabled(false);

    ui->pushButton_summitField->setEnabled(false);
    ui->pushButton_summitCon->setEnabled(false);

    ui->pushButton_abandonSummit->setEnabled(false);
    ui->pushButton_Summit->setEnabled(false);

    ui->pushButton_AbandonWrite->setEnabled(false);
    ui->pushButton_WriteToFile->setEnabled(false);

    ui->tableWidgeConditionList->setEnabled(false);
    ui->textBrowserVisualizationConfig->setEnabled(false);
}

FormFieldCheckEditTools::~FormFieldCheckEditTools()
{
    delete ui;
}

void FormFieldCheckEditTools::on_comboBox_1_currentIndexChanged(int index)
{
    comboBox_1OK=false;
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    switch (index){
    case 0:
        if(ofdConfigList.count()>0){
            for(int l1=0;l1<ofdConfigList.count();l1++){
                ui->comboBox_2->addItem(ofdConfigList.at(l1).getFileName());
            }
            //第二层初始化第一个配置文件
            if(!ofdConfigList.at(0).getConfigSegmentList().isEmpty()){
                for (int l2=0;l2<ofdConfigList.at(0).getConfigSegmentList().count();l2++){
                    ui->comboBox_3->addItem(ofdConfigList.at(0).getConfigSegmentList().at(l2).getConfigSegment()+"-"+ofdConfigList.at(0).getConfigSegmentList().at(l2).getDescribe());
                }
                //初始化字段列表
                initCurrentItem(0,0,0);
            }
        }
        else{
            clearOldData();
            statusBarDisplayMessage("没有有效的OFD/类OFD文件配置...");
        }
        break;
    case 1:
        if(csvConfigList.count()>0){
            for(int l1=0;l1<csvConfigList.count();l1++){
                ui->comboBox_2->addItem(csvConfigList.at(l1).getFileName());
            }
            //第二层初始化第一个配置文件
            if(!csvConfigList.at(0).getConfigSegmentList().isEmpty()){
                for (int l2=0;l2<csvConfigList.at(0).getConfigSegmentList().count();l2++){
                    ui->comboBox_3->addItem(csvConfigList.at(0).getConfigSegmentList().at(l2).getConfigSegment()+"-"+csvConfigList.at(0).getConfigSegmentList().at(l2).getFileDescribe());
                }
                //初始化字段列表
                initCurrentItem(1,0,0);
            }
        }
        else{
            clearOldData();
            statusBarDisplayMessage("没有有效的CSV固定分隔符文件配置...");
        }
        break;
    case 2:
        if(fixedConfigList.count()>0){
            for(int l1=0;l1<fixedConfigList.count();l1++){
                ui->comboBox_2->addItem(fixedConfigList.at(l1).getFileName());
            }
            //第二层初始化第一个配置文件
            if(!fixedConfigList.at(0).getConfigSegmentList().isEmpty()){
                for (int l2=0;l2<fixedConfigList.at(0).getConfigSegmentList().count();l2++){
                    ui->comboBox_3->addItem(fixedConfigList.at(0).getConfigSegmentList().at(l2).getConfigSegment()+"-"+fixedConfigList.at(0).getConfigSegmentList().at(l2).getFileDescribe());
                }
                //初始化字段列表
                initCurrentItem(2,0,0);
            }
        }
        else{
            clearOldData();
            statusBarDisplayMessage("没有有效的FIXED定长文件配置...");
        }
        break;
    }
    comboBox_1OK=true;
}


void FormFieldCheckEditTools::on_comboBox_2_currentIndexChanged(int index)
{
    //一级菜单改变时2级菜单不响应
    if(!comboBox_1OK){
        return;
    }
    comboBox_2OK=false;
    ui->comboBox_3->clear();
    switch (ui->comboBox_1->currentIndex()){
    case 0:
        if(!ofdConfigList.at(index).getConfigSegmentList().isEmpty()){
            for (int l2=0;l2<ofdConfigList.at(index).getConfigSegmentList().count();l2++){
                ui->comboBox_3->addItem(ofdConfigList.at(index).getConfigSegmentList().at(l2).getConfigSegment()+"-"+ofdConfigList.at(index).getConfigSegmentList().at(l2).getDescribe());
            }
            //初始化字段列表
            initCurrentItem(0,index,0);
        }
        else{
            clearOldData();
            statusBarDisplayMessage("此配置文件没有有效的配置段...");
        }
        break;
    case 1:
        //第二层初始化第一个配置文件
        if(!csvConfigList.at(index).getConfigSegmentList().isEmpty()){
            for (int l2=0;l2<csvConfigList.at(index).getConfigSegmentList().count();l2++){
                ui->comboBox_3->addItem(csvConfigList.at(index).getConfigSegmentList().at(l2).getConfigSegment()+"-"+csvConfigList.at(index).getConfigSegmentList().at(l2).getFileDescribe());
            }
            //初始化字段列表
            initCurrentItem(1,index,0);
        }
        else{
            clearOldData();
            statusBarDisplayMessage("此配置文件没有有效的配置段...");
        }
        break;
    case 2:
        if(!fixedConfigList.at(index).getConfigSegmentList().isEmpty()){
            for (int l2=0;l2<fixedConfigList.at(index).getConfigSegmentList().count();l2++){
                ui->comboBox_3->addItem(fixedConfigList.at(index).getConfigSegmentList().at(l2).getConfigSegment()+"-"+fixedConfigList.at(index).getConfigSegmentList().at(l2).getFileDescribe());
            }
            //初始化字段列表
            initCurrentItem(2,index,0);
        }
        else{
            clearOldData();
            statusBarDisplayMessage("此配置文件没有有效的配置段...");
        }
        break;

    }
    comboBox_2OK=true;
}


void FormFieldCheckEditTools::on_comboBox_3_currentIndexChanged(int index)
{
    if(!comboBox_2OK||!comboBox_1OK){
        return;
    }
    int index1=ui->comboBox_1->currentIndex();
    int index2=ui->comboBox_2->currentIndex();
    int index3=index;
    initCurrentItem(index1,index2,index3);
}

void FormFieldCheckEditTools::initCurrentItem(int index1,int index2,int index3){
    clearOldData();
    searchRow =0;
    searchColumn=0;
    currentEditConfigSegmentCheckItemList.clear();
    currentEditCheckIndex=-1;
    currentEditRequiredFields.clear();
    currentEditConListOverList.clear();
    currentEditConditionIndex=-1;
    currentEditConfigSegmentCheckItemList.clear();
    currentConfigSegment="";
    currentIni="";
    newAddCheckIndex=-1;
    newAddCheckInited=true;
    newAddConditionIndex=-1;
    newAddConditionInited=true;
    //OFD
    QList<QStringList> fieldListdata;
    QList<QStringList> checkListdata;
    if(index1==0&&ofdConfigList.count()>index2&&ofdConfigList.at(index2).getConfigSegmentList().count()>index3){
        if(!ofdConfigList.at(index2).getConfigSegmentList().at(index3).getUseAble()){
            ui->frameButton->setEnabled(false);
            statusBarDisplayMessage("此配置无法使用,原因为:"+ofdConfigList.at(index2).getConfigSegmentList().at(index3).getMessage());
        }
        else{
            currentIni=ofdConfigList.at(index2).getFileName();
            currentConfigSegment=ofdConfigList.at(index2).getConfigSegmentList().at(index3).getConfigSegment();
            currentEditConfigSegmentprimaryKeyFieldList=ofdConfigList.at(index2).getConfigSegmentList().at(index3).getPrimaryKeyFieldList();
            if(ofdConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().count()>0){
                for(int i=0;i<ofdConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().count();i++){
                    QStringList line;
                    line.append(ofdConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().at(i).getFieldDescribe());
                    line.append(ofdConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().at(i).getFieldName());
                    fieldListdata.append(line);
                }
            }
            if(ofdConfigList.at(index2).getConfigSegmentList().at(index3).getFieldcheckList().count()>0){
                OFDFileDefinition ofd=ofdConfigList.at(index2).getConfigSegmentList().at(index3);
                currentEditConfigSegmentCheckItemList=ofd.getFieldcheckList();
                checkListdata=Utils::parseCheckItemListtoChineseList(ofd);
            }
        }
    }
    //CSV
    else if(index1==1&&csvConfigList.count()>index2&&csvConfigList.at(index2).getConfigSegmentList().count()>index3){
        if(!csvConfigList.at(index2).getConfigSegmentList().at(index3).getUseAble()){
            ui->frameButton->setEnabled(false);
            statusBarDisplayMessage("此配置无法使用,原因为:"+csvConfigList.at(index2).getConfigSegmentList().at(index3).getMessage());
        }
        else {
            currentIni=csvConfigList.at(index2).getFileName();
            currentConfigSegment=csvConfigList.at(index2).getConfigSegmentList().at(index3).getConfigSegment();
            currentEditConfigSegmentprimaryKeyFieldList=csvConfigList.at(index2).getConfigSegmentList().at(index3).getPrimaryKeyFieldList();
            if(csvConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().count()>0){
                for(int i=0;i<csvConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().count();i++){
                    QStringList line;
                    line.append(csvConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().at(i).getFieldDescribe());
                    line.append(csvConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().at(i).getFieldName());
                    fieldListdata.append(line);
                }
            }
            if(csvConfigList.at(index2).getConfigSegmentList().at(index3).getFieldcheckList().count()>0){
                CsvFileDefinition csv=csvConfigList.at(index2).getConfigSegmentList().at(index3);
                currentEditConfigSegmentCheckItemList=csv.getFieldcheckList();
                checkListdata=Utils::parseCheckItemListtoChineseList(csv);
            }
        }
    }
    //FIXED
    else if(index1==2&&fixedConfigList.count()>index2&&fixedConfigList.at(index2).getConfigSegmentList().count()>index3){
        if(!fixedConfigList.at(index2).getConfigSegmentList().at(index3).getUseAble()){
            ui->frameButton->setEnabled(false);
            statusBarDisplayMessage("此配置无法使用,原因为:"+fixedConfigList.at(index2).getConfigSegmentList().at(index3).getMessage());
        }
        else {
            currentIni=fixedConfigList.at(index2).getFileName();
            currentConfigSegment=fixedConfigList.at(index2).getConfigSegmentList().at(index3).getConfigSegment();
            currentEditConfigSegmentprimaryKeyFieldList=fixedConfigList.at(index2).getConfigSegmentList().at(index3).getPrimaryKeyFieldList();
            if(fixedConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().count()>0){
                for(int i=0;i<fixedConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().count();i++){
                    QStringList line;
                    line.append(fixedConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().at(i).getFieldDescribe());
                    line.append(fixedConfigList.at(index2).getConfigSegmentList().at(index3).getFieldList().at(i).getFieldName());
                    fieldListdata.append(line);
                }
            }
            if(fixedConfigList.at(index2).getConfigSegmentList().at(index3).getFieldcheckList().count()>0){
                FIXEDFileDefinition fixed=fixedConfigList.at(index2).getConfigSegmentList().at(index3);
                currentEditConfigSegmentCheckItemList=fixed.getFieldcheckList();
                checkListdata=Utils::parseCheckItemListtoChineseList(fixed);
            }
        }
    }
    //初始化字段列表
    if(fieldListdata.count()>0){
        ui->frameButton->setEnabled(true);
        ui->tableWidgetFieldList->setRowCount(fieldListdata.count());
        for(int i=0;i<fieldListdata.count();i++){
            QString eng=fieldListdata.at(i).at(0);
            eng=eng.replace(",","");
            QString ch=fieldListdata.at(i).at(1);
            ch=ch.replace(",","");
            QTableWidgetItem *item0= new QTableWidgetItem(eng);
            item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
            QTableWidgetItem *item1= new QTableWidgetItem(ch);
            item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
            ui->tableWidgetFieldList->setItem(i,0,item0);
            ui->tableWidgetFieldList->setItem(i,1,item1);
            //字段值
            QTableWidgetItem *item2= new QTableWidgetItem();
            item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
            ui->tableWidgetFieldList->setItem(i,2,item2);

            QCheckBox * checkB = new QCheckBox(this); //创建checkbox
            checkB->setEnabled(false);
            checkB->setCheckState(Qt::Unchecked); //设置状态
            QWidget *w = new QWidget(this); //创建一个widget
            QHBoxLayout *hLayout = new QHBoxLayout(); //创建布局
            hLayout->addWidget(checkB); //添加checkbox
            hLayout->setMargin(0); //设置边缘距离 否则会很难看
            hLayout->setAlignment(checkB, Qt::AlignCenter); //居中
            w->setLayout(hLayout); //设置widget的布局
            ui->tableWidgetFieldList->setCellWidget(i,3,w);

            //主键标识
            QCheckBox * checkC = new QCheckBox(this); //创建checkbox
            if(currentEditConfigSegmentprimaryKeyFieldList.contains(i)){
                checkC->setCheckState(Qt::Checked); //设置状态
            }
            else{
                checkC->setCheckState(Qt::Unchecked); //设置状态
            }
            checkC->setEnabled(false);
            QHBoxLayout *hLayout2 = new QHBoxLayout(); //创建布局
            hLayout2->addWidget(checkC); //添加checkbox
            hLayout2->setMargin(0); //设置边缘距离 否则会很难看
            hLayout2->setAlignment(checkC, Qt::AlignCenter); //居中
            QWidget *w2 = new QWidget(this); //创建一个widget
            w2->setLayout(hLayout2); //设置widget的布局
            ui->tableWidgetFieldList->setCellWidget(i,4,w2);
        }
    }
    //初始化当前的必填字段检查
    if(checkListdata.count()>0){
        ui->tableWidgetCheckList->setRowCount(checkListdata.count());
        for (int row = 0; row < checkListdata.count(); ++row)
        {
            for(int col=0;col<2;col++){
                if(col<checkListdata.at(row).count()){
                    QString value=checkListdata.at(row).at(col);
                    QTableWidgetItem *item= new QTableWidgetItem(value);
                    ui->tableWidgetCheckList->setItem(row, col, item);
                }
            }
            ui->tableWidgetCheckList->resizeRowsToContents();
        }
    }
}

void FormFieldCheckEditTools::statusBarDisplayMessage(QString message){
    ui->statusbar->showMessage(message);
}

void FormFieldCheckEditTools::clearOldData(){
    ui->tableWidgetFieldList->clearContents();
    ui->tableWidgetFieldList->setRowCount(0);
    ui->tableWidgetCheckList->clearContents();
    ui->tableWidgetCheckList->setRowCount(0);
    ui->tableWidgeConditionList->clearContents();
    ui->tableWidgeConditionList->setRowCount(0);
    ui->textBrowserVisualizationConfig->clear();
}

void FormFieldCheckEditTools::on_searchPushButton_clicked()
{
    QString text=ui->searchLineEdit->text();
    if(!text.isEmpty()){
        int rowcount=ui->tableWidgetFieldList->rowCount();
        int colcount=ui->tableWidgetFieldList->columnCount();
        //开始搜索
        //逻辑--搜到了就允许继续往下搜，搜不到就搜一圈停止
        int beginrow=searchRow;
        int begincol=searchColumn;
        //先从开始搜索的位置搜索到结尾
        for(int i=beginrow;i<rowcount;i++){
            int col=0;
            //搜索开始行时，列从searchCol开始
            if(i==beginrow){
                col=begincol;
            }
            else{
                col=0;
            }
            for(int j=col;j<colcount;j++){
                if(j==2||j==3){
                    continue;
                }
                //搜索到了
                if(ui->tableWidgetFieldList->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ui->tableWidgetFieldList->setCurrentCell(i,j);
                    ui->tableWidgetFieldList->setFocus();
                    return;
                }
            }
        }
        //定义从头搜索的结束单元格
        int againendrow=0;
        int againendcol=0;
        //没搜到，从开头搜到开始搜索的列的上一个单元格
        //从某一行的非第一列，重新搜索到上一个单元格
        if(begincol>0){
            againendrow=beginrow;
            againendcol=begincol-1;
        }
        //非第一行的第一列-重新搜索到上一行的最后一列
        else if(beginrow>0&&begincol==0){
            againendrow=beginrow-1;
            againendcol=colcount-1;
        }
        //如果是从首行首列开始搜索的，直接结束
        else if(beginrow==0&&begincol==0){
            statusBarDisplayMessage("搜索了一圈,没找到你要搜索的内容...");
            return;
        }
        for(int i=0;i<=againendrow;i++){
            int colend=colcount-1;
            if(i==againendrow){
                colend=againendcol;
            }
            else{
                colend=colcount-1;
            }
            for(int j=0;j<=colend;j++){
                if(j==2||j==3){
                    continue;
                }
                //搜索到了
                if(ui->tableWidgetFieldList->item(i,j)->text().contains(text,Qt::CaseInsensitive)){
                    ui->tableWidgetFieldList->setCurrentCell(i,j);
                    ui->tableWidgetFieldList->setFocus();
                    return;
                }
            }
        }
        statusBarDisplayMessage("搜索了一圈,没找到你要搜索的内容...");
    }
}


void FormFieldCheckEditTools::on_tableWidgetFieldList_itemSelectionChanged()
{
    //选择的范围
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgetFieldList->selectedRanges();
    //范围和
    int rangeCount=itemsRange.count();
    if(rangeCount==1&&itemsRange.at(0).leftColumn()==itemsRange.at(0).rightColumn()&&itemsRange.at(0).topRow()==itemsRange.at(0).bottomRow()){
        int rowcount=ui->tableWidgetFieldList->rowCount();
        int colcount=ui->tableWidgetFieldList->columnCount();
        //记录当前所在行
        this->searchRow=itemsRange.at(itemsRange.count()-1).bottomRow();
        //当前所在列
        this->searchColumn=itemsRange.at(itemsRange.count()-1).rightColumn();
        //焦点在最后一个单元格时，转移到第一行第一列
        if(searchRow==(rowcount-1)&&searchColumn==(colcount-1)){
            this->searchRow=0;
            this->searchColumn=0;
        }
        //焦点在非最后一行的最后一个单元格,转移到下一行的第一个单元格
        else if(searchRow<(rowcount-1)&&searchColumn==(colcount-1)){
            this->searchRow+=1;
            this->searchColumn=0;
        }
        else{
            this->searchColumn+=1;
        }
    }
}


/**
 *编辑规则
 **/
void FormFieldCheckEditTools::fieldCheckEditSelect()
{
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgetCheckList->selectedRanges();
    if(itemsRange.count()==1&&itemsRange.at(0).bottomRow()==itemsRange.at(0).topRow()){
        //进入编辑模式,屏蔽无关按钮
        editFieldCheckMode=true;
        editPrimaryMode=false;
        inEditMode();
        currentEditCheckIndex=itemsRange.at(0).topRow();
        currentEditConListOverList=currentEditConfigSegmentCheckItemList.at(itemsRange.at(0).topRow()).getCheckConditionsListOverList();
        ui->tableWidgeConditionList->clearContents();
        //这个是新建的空规则
        if(newAddCheckIndex==currentEditCheckIndex){
            //清除上次新建条件规则的标志,因为上次新建后并未提交
            newAddConditionIndex=-1;
            if(ui->tableWidgetFieldList->rowCount()>0){
                for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
                    ui->tableWidgetFieldList->item(i,2)->setText("");
                    QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
                    checkBox3->setCheckState(Qt::Unchecked);
                }
            }
            fieldConAdd();
        }
        else{
            QList<int> fieldList=currentEditConfigSegmentCheckItemList.at(itemsRange.at(0).topRow()).getRequiredFields();
            currentEditRequiredFields=fieldList;
            QString fieldListString;
            //必填字段勾选和生成文本格式内容
            if(ui->tableWidgetFieldList->rowCount()>0){
                for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
                    if (fieldList.contains(i)){
                        if(fieldListString.isEmpty()){
                            fieldListString.append(ui->tableWidgetFieldList->item(i,0)->text());
                        }
                        else{
                            fieldListString.append(",").append(ui->tableWidgetFieldList->item(i,0)->text());
                        }
                        QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
                        checkBox3->setCheckState(Qt::Checked);
                    }
                    else{
                        QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
                        checkBox3->setCheckState(Qt::Unchecked);
                    }
                }
            }
            ui->textBrowserVisualizationConfig->setText(fieldListString);
            //条件列表
            ui->tableWidgeConditionList->clearContents();
            if(currentEditConListOverList.count()>0){
                ui->tableWidgeConditionList->setRowCount(currentEditConListOverList.count());
                //循环每一个分组的每条记录
                for(int j=0;j<currentEditConListOverList.count();j++){
                    QString con;
                    QList<CheckCondition> currenGroupconList=currentEditConListOverList.at(j);
                    if(currenGroupconList.count()==1&&currenGroupconList.at(0).index==ALL_CHECK_INDEX){
                        con="本文件中所有记录";
                        QTableWidgetItem *item=new QTableWidgetItem;
                        item->setText(con);
                        ui->tableWidgeConditionList->setItem(j,0,item);
                        currentEditConditionIndex=0;
                    }
                    else if(currenGroupconList.count()>0)
                    {
                        //字段必填转化成map
                        QMap<int,QString> map;
                        for(int aa=0;aa<currenGroupconList.count();aa++){
                            map.insert(currenGroupconList.at(aa).index,currenGroupconList.at(aa).value);
                        }
                        for(int bb=0;bb<ui->tableWidgetFieldList->rowCount();bb++){
                            if(map.contains(bb)){
                                if(con.isEmpty()){
                                    con.append(ui->tableWidgetFieldList->item(bb,0)->text()).append("=").append(map.value(bb));
                                }
                                else{
                                    con.append(",").append(ui->tableWidgetFieldList->item(bb,0)->text()).append("=").append(map.value(bb));
                                }
                                if(j==0){
                                    QTableWidgetItem *item=new QTableWidgetItem;
                                    item->setText(map.value(bb));
                                    ui->tableWidgetFieldList->setItem(bb,2,item);
                                }
                            }
                            else{
                                if(j==0){
                                    QTableWidgetItem *item=new QTableWidgetItem;
                                    item->setText(map.value(bb));
                                    ui->tableWidgetFieldList->setItem(bb,2,item);
                                }
                            }
                        }
                        QTableWidgetItem *item=new QTableWidgetItem;
                        item->setText(con);
                        ui->tableWidgeConditionList->setItem(j,0,item);
                        QTableWidgetSelectionRange range=QTableWidgetSelectionRange(0,0,0,0);
                        ui->tableWidgeConditionList->setRangeSelected(range,true);
                        currentEditConditionIndex=0;
                    }
                }
            }
        }
        this->setWindowTitle(windowName+"-规则编辑中");
    }
    else{
        statusBarDisplayMessage("选择一行规则进行编辑...");
    }
}


void FormFieldCheckEditTools::fieldCheckDeleteSelect()
{
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgetCheckList->selectedRanges();
    int minnewAddCheckIndexCount=0;
    if(itemsRange.count()>0){
        //特殊情况只选择了新增的那条，且没有其他修改
        if(!changeNotSaveToFile&&itemsRange.count()==1&&itemsRange.at(0).bottomRow()==itemsRange.at(0).topRow()&&itemsRange.at(0).topRow()==newAddCheckIndex){
            newAddCheckIndex=-1;
            newAddCheckInited=true;
            ui->tableWidgetCheckList->removeRow(itemsRange.at(0).topRow());
            currentEditConfigSegmentCheckItemList.removeAt(itemsRange.at(0).topRow());
            this->setWindowTitle(windowName);
            ui->pushButton_AbandonWrite->setEnabled(false);
            ui->pushButton_WriteToFile->setEnabled(false);
            ui->comboBox_1->setEnabled(true);
            ui->comboBox_2->setEnabled(true);
            ui->comboBox_3->setEnabled(true);
            statusBarDisplayMessage("已删除新增待编辑的规则...");
        }
        else{
            for (int i=itemsRange.count()-1;i>=0;i--){
                QTableWidgetSelectionRange range=itemsRange.at(i);
                if (range.rowCount()>0){
                    for (int row = range.bottomRow(); row>=range.topRow(); --row) {
                        ui->tableWidgetCheckList->removeRow(row);
                        currentEditConfigSegmentCheckItemList.removeAt(row);
                        if(row<minnewAddCheckIndexCount){
                            minnewAddCheckIndexCount+=1;
                        }
                        if(row==newAddCheckIndex){
                            newAddCheckIndex=-1;
                            newAddCheckInited=true;
                        }
                    }
                }
            }
            if(newAddCheckIndex>0){
                newAddCheckIndex=newAddCheckIndex-minnewAddCheckIndexCount;
                //兜底异常
                if(newAddCheckIndex<0){
                    newAddCheckIndex=-1;
                }
            }
            changeNotSaveToFile=true;
            outEditMode();
            this->setWindowTitle(windowName+"-有修改待写入配置文件");
        }
    }
}


void FormFieldCheckEditTools::on_tableWidgetFieldList_customContextMenuRequested(const QPoint &pos)
{
    table_field_PopMenu->clear();
    //判断当前鼠标位置是不是在表格单元格位置内
    //空表
    if(ui->tableWidgetFieldList->rowCount()<1){
        return;
    }
    //如果鼠标点击的是表格的空白位置取到得行y是-1
    if( ui->tableWidgetFieldList->rowAt(pos.y()) <0){
        return;
    }
    if(editFieldCheckMode&&!editPrimaryMode){
        action_table_field_select_range->setText(tr("勾选选择的字段为必填"));
        table_field_PopMenu->addAction(action_table_field_select_range);
        action_table_field_clear_range->setText(tr("清除选择的字段必填勾选"));
        table_field_PopMenu->addAction(action_table_field_clear_range);

        action_table_field_selectall->setText(tr("勾选所有字段为必填"));
        table_field_PopMenu->addAction(action_table_field_selectall);
        action_table_field_clear_all->setText(tr("清除所有字段必填勾选"));
        table_field_PopMenu->addAction(action_table_field_clear_all);

        table_field_PopMenu->addAction(action_table_field_clear_condition);
    }
    if(!editFieldCheckMode&&editPrimaryMode){
        action_table_field_select_range->setText(tr("勾选选择的字段为主键字段"));
        table_field_PopMenu->addAction(action_table_field_select_range);
        action_table_field_clear_range->setText(tr("清除选择的字段为主键字段"));
        table_field_PopMenu->addAction(action_table_field_clear_range);

        action_table_field_selectall->setText(tr("勾选所有字段为主键字段"));
        table_field_PopMenu->addAction(action_table_field_selectall);
        action_table_field_clear_all->setText(tr("清除所有字段为主键字段"));
        table_field_PopMenu->addAction(action_table_field_clear_all);
    }
    table_field_PopMenu->exec(QCursor::pos());
}

void FormFieldCheckEditTools::table_field_selectAll(){
    if(ui->tableWidgetFieldList->rowCount()>0){
        for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            if(editFieldCheckMode&!editPrimaryMode){
                QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
                checkBox3->setCheckState(Qt::Checked);
            }else if(!editFieldCheckMode&editPrimaryMode){
                QCheckBox * checkBox4 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 4)->children().at(1));
                checkBox4->setCheckState(Qt::Checked);
            }
        }
    }
}

void FormFieldCheckEditTools::table_field_clearAllSelet(){
    if(ui->tableWidgetFieldList->rowCount()>0){
        for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            if(editFieldCheckMode&!editPrimaryMode){
                QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
                checkBox3->setCheckState(Qt::Unchecked);
            }else if(!editFieldCheckMode&editPrimaryMode){
                QCheckBox * checkBox4 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 4)->children().at(1));
                checkBox4->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void FormFieldCheckEditTools::table_field_selectRange(){
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgetFieldList->selectedRanges();
    if(itemsRange.count()>0){
        for(int aa=0;aa<itemsRange.count();aa++){
            int first=itemsRange.at(aa).topRow();
            int last=itemsRange.at(aa).bottomRow();
            for(int bb=first;bb<=last;bb++){
                if(editFieldCheckMode&!editPrimaryMode){
                    QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 3)->children().at(1));
                    checkBox3->setCheckState(Qt::Checked);
                }else if(!editFieldCheckMode&editPrimaryMode){
                    QCheckBox * checkBox4 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 4)->children().at(1));
                    checkBox4->setCheckState(Qt::Checked);
                }
            }
        }
    }
}

void FormFieldCheckEditTools::table_field_clearRange(){
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgetFieldList->selectedRanges();
    if(itemsRange.count()>0){
        for(int aa=0;aa<itemsRange.count();aa++){
            int first=itemsRange.at(aa).topRow();
            int last=itemsRange.at(aa).bottomRow();
            for(int bb=first;bb<=last;bb++){
                if(editFieldCheckMode&!editPrimaryMode){
                    QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 3)->children().at(1));
                    checkBox3->setCheckState(Qt::Unchecked);
                }else if(!editFieldCheckMode&editPrimaryMode){
                    QCheckBox * checkBox4 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 4)->children().at(1));
                    checkBox4->setCheckState(Qt::Unchecked);
                }
            }
        }
    }
}

void FormFieldCheckEditTools::table_field_clearCondition(){
    if(ui->tableWidgetFieldList->rowCount()>0){
        for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            QTableWidgetItem *item2= new QTableWidgetItem();
            ui->tableWidgetFieldList->setItem(i,2,item2);
        }
    }
}

void FormFieldCheckEditTools::on_tableWidgetCheckList_customContextMenuRequested(const QPoint &pos)
{
    table_check_PopMenu->clear();
    //编辑中不准动
    if(editFieldCheckMode){return;}
    //判断当前鼠标位置是不是在表格单元格位置内
    //空表--仅仅允许新增
    if(ui->tableWidgetCheckList->rowCount()<1||ui->tableWidgetCheckList->rowAt(pos.y()) <0){
        table_check_PopMenu->addAction(action_table_check_add);
    }
    else{
        table_check_PopMenu->addAction(action_table_check_add);
        QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgetCheckList->selectedRanges();
        if(itemsRange.count()==1&&itemsRange.at(0).bottomRow()==itemsRange.at(0).topRow()){
            table_check_PopMenu->addAction(action_table_check_edit_select);
        }
        table_check_PopMenu->addAction(action_table_check_delete_select);
    }
    table_check_PopMenu->exec(QCursor::pos());
}

void FormFieldCheckEditTools::inEditMode(){
    ui->comboBox_1->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->tableWidgetCheckList->setEnabled(false);
    ui->pushButton_AbandonWrite->setEnabled(false);
    ui->pushButton_WriteToFile->setEnabled(false);

    //字段必填
    if(editFieldCheckMode&&!editPrimaryMode){
        ui->pushButton_PrimaryKey->setEnabled(false);

        ui->pushButton_ImportField->setEnabled(true);
        ui->pushButton_ImportCon->setEnabled(true);

        ui->pushButton_summitField->setEnabled(true);
        ui->pushButton_summitCon->setEnabled(true);

        ui->tableWidgeConditionList->setEnabled(true);
        ui->textBrowserVisualizationConfig->setEnabled(true);

        for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            ui->tableWidgetFieldList->item(i,2)->setText("");
            QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
            checkBox3->setEnabled(true);
        }
    }
    //主键
    if(!editFieldCheckMode&&editPrimaryMode){
        ui->pushButton_ImportField->setEnabled(true);

        ui->pushButton_PrimaryKey->setEnabled(false);

        ui->tableWidgeConditionList->setEnabled(false);
        ui->textBrowserVisualizationConfig->setEnabled(false);

        for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            QCheckBox * checkBox = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 4)->children().at(1));
            checkBox->setEnabled(true);
        }
    }

    ui->pushButton_abandonSummit->setEnabled(true);
    ui->pushButton_Summit->setEnabled(true);

    ui->pushButton_AbandonWrite->setEnabled(false);
    ui->pushButton_WriteToFile->setEnabled(false);
}

void FormFieldCheckEditTools::outEditMode(){
    editFieldCheckMode=false;
    editPrimaryMode=false;
    currentEditCheckIndex=-1;
    currentEditRequiredFields.clear();
    currentEditConditionIndex=-1;
    currentEditConListOverList.clear();
    if(changeNotSaveToFile||newAddCheckIndex!=-1){
        if(changeNotSaveToFile){
            this->setWindowTitle(windowName+"-有修改待写入配置文件");
        }
        else if(newAddCheckIndex!=-1){
            this->setWindowTitle(windowName+"-有新增规则待编辑初始化");
        }
        ui->comboBox_1->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);

        ui->pushButton_PrimaryKey->setEnabled(true);

        ui->tableWidgetCheckList->setEnabled(true);
        ui->pushButton_AbandonWrite->setEnabled(true);
        ui->pushButton_WriteToFile->setEnabled(true);

        ui->tableWidgeConditionList->setRowCount(0);
        ui->tableWidgeConditionList->clearContents();
        ui->textBrowserVisualizationConfig->clear();

        ui->pushButton_ImportField->setEnabled(false);
        ui->pushButton_ImportCon->setEnabled(false);

        ui->pushButton_summitField->setEnabled(false);
        ui->pushButton_summitCon->setEnabled(false);

        ui->pushButton_abandonSummit->setEnabled(false);
        ui->pushButton_Summit->setEnabled(false);

        ui->pushButton_AbandonWrite->setEnabled(true);
        ui->pushButton_WriteToFile->setEnabled(true);

        ui->tableWidgeConditionList->setEnabled(false);
        ui->textBrowserVisualizationConfig->setEnabled(false);
    }
    else{
        this->setWindowTitle(windowName);
        ui->comboBox_1->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);

        ui->pushButton_PrimaryKey->setEnabled(true);

        ui->tableWidgetCheckList->setEnabled(true);
        ui->pushButton_AbandonWrite->setEnabled(true);
        ui->pushButton_WriteToFile->setEnabled(true);

        ui->tableWidgeConditionList->setRowCount(0);
        ui->tableWidgeConditionList->clearContents();
        ui->textBrowserVisualizationConfig->clear();

        ui->pushButton_ImportField->setEnabled(false);
        ui->pushButton_ImportCon->setEnabled(false);

        ui->pushButton_summitField->setEnabled(false);
        ui->pushButton_summitCon->setEnabled(false);

        ui->pushButton_abandonSummit->setEnabled(false);
        ui->pushButton_Summit->setEnabled(false);

        ui->pushButton_AbandonWrite->setEnabled(false);
        ui->pushButton_WriteToFile->setEnabled(false);

        ui->tableWidgeConditionList->setEnabled(false);
        ui->textBrowserVisualizationConfig->setEnabled(false);
    }
    //还原
    for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
        QTableWidgetItem *item2= ui->tableWidgetFieldList->item(i,2);
        item2->setText("");
        item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
        QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
        checkBox3->setEnabled(false);
        checkBox3->setCheckState(Qt::Unchecked);
        QCheckBox * checkBox4 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 4)->children().at(1));
        if(currentEditConfigSegmentprimaryKeyFieldList.contains(i)){
            checkBox4->setCheckState(Qt::Checked);
        }
        else{
            checkBox4->setCheckState(Qt::Unchecked);
        }
        checkBox4->setEnabled(false);
    }
}

void FormFieldCheckEditTools::on_pushButton_abandonSummit_clicked()
{
    if(editFieldCheckMode||editPrimaryMode){
        outEditMode();
        statusBarDisplayMessage("放弃并退出编辑...");
    }
}


void FormFieldCheckEditTools::on_pushButton_summitCon_clicked()
{
    //macos下点提交时，最后一次编辑的表格取不到数据，先调用下禁用再开启就可以了
    ui->tableWidgetFieldList->setEnabled(false);
    ui->tableWidgetFieldList->setEnabled(true);
    if(editFieldCheckMode&&currentEditConditionIndex>=0&&currentEditConListOverList.count()>currentEditConditionIndex){
        //查找是否存在一个All规则
        int allCheckIndex=-1;
        if (currentEditConListOverList.count()>0){
            for(int i=0;i<currentEditConListOverList.count();i++){
                QList<CheckCondition> itemList=currentEditConListOverList.at(i);
                if(itemList.count()>0){
                    for(int j=0;j<itemList.count();j++){
                        //只寻找一次
                        if(allCheckIndex==-1&&j==0){
                            if(itemList.at(j).index==ALL_CHECK_INDEX){
                                allCheckIndex=i;
                            }
                        }
                    }
                }
            }
        }
        qDebug()<<"ALl规则"<<allCheckIndex;
        //构建当前新的必填字段以及评估是否存在修改
        QList<CheckCondition> newCon;
        bool changeflage=false;
        QString newConString;
        int emptyCount=0;
        QList<CheckCondition> oldCon=currentEditConListOverList.at(currentEditConditionIndex);
        //字段必填转化成map
        QMap<int,QString> map;
        for(int aa=0;aa<oldCon.count();aa++){
            map.insert(oldCon.at(aa).index,oldCon.at(aa).value);
        }
        for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            QString varz=ui->tableWidgetFieldList->item(i,2)->text();
            if(!varz.isEmpty()){
                if(varz.contains(",")||varz.contains("|")||varz.contains("&")){
                    statusBarDisplayMessage("条件值不能是逗号(,)竖线(|)and符号(&)中的任何一个");
                    break;
                }
                CheckCondition c;
                c.index=i;
                c.value=varz;
                newCon.append(c);
                if(!map.contains(i)||varz!=map.value(i)){
                    changeflage=true;
                }
                if(newConString.isEmpty()){
                    newConString.append(ui->tableWidgetFieldList->item(i,0)->text()).append("=").append(varz);
                }
                else{
                    newConString.append(",").append(ui->tableWidgetFieldList->item(i,0)->text()).append("=").append(varz);
                }
            }
            else{
                emptyCount++;
                if(map.contains(i)){
                    changeflage=true;
                }
            }
        }
        //全空,则是All
        if(emptyCount==ui->tableWidgetFieldList->rowCount()){
            //All规则不能和其他规则以及其他All规则共存
            if(allCheckIndex!=-1&&currentEditConditionIndex!=allCheckIndex){
                statusBarDisplayMessage(QString("你当前试图提交一个适用于所有记录的规则,但是条件分组中编号为%1已经存在一个适用于所有记录的规则了...").arg(allCheckIndex+1));
                return;
            }
            if(currentEditConListOverList.count()>1){
                statusBarDisplayMessage("你当前试图提交一个适用于所有记录的规则,这种规则不能和其他规则共存,如确实需要,请先删除其他条件分组...");
                return;
            }
            CheckCondition c;
            c.index=ALL_CHECK_INDEX;
            c.value="";
            newCon.append(c);
            newConString="本文件中所有记录";
        }
        //目前正在编辑的规则是新条件--放过--认为是修改了的
        if(currentEditConditionIndex==newAddConditionIndex){
            changeflage=true;
        }
        if(changeflage){
            //检查冲突
            if (currentEditConListOverList.count()>0){
                for(int i=0;i<currentEditConListOverList.count();i++){
                    QList<CheckCondition> newItemList=currentEditConListOverList.at(i);
                    if(newCon.count()!=newItemList.count()){
                        continue;
                    }
                    else if(newCon.count()>0&&newItemList.count()>0){
                        int eqCount=0;
                        for(int j=0;j<newCon.count();j++){
                            if(newCon.at(j).index==newItemList.at(j).index&&newCon.at(j).value==newItemList.at(j).value){
                                eqCount++;
                            }
                        }
                        //全部相等
                        if(eqCount==newCon.count()){
                            statusBarDisplayMessage(QString("提交的规则和条件分组列表中的第%1条规则完全一样,无需重复构建相同的条件...").arg(i+1));
                            return;
                        }
                    }
                }
            }
            if(allCheckIndex!=-1&&currentEditConditionIndex!=allCheckIndex){
                statusBarDisplayMessage(QString("条件分组中编号为%1的是一个适用于所有记录的规则,这种规则不能和其他规则共存,请二选一删除适用于所有记录的规则或者放弃提交当前正在编辑的规则...").arg(allCheckIndex+1));
                return;
            }
            currentEditConListOverList.replace(currentEditConditionIndex,newCon);
            ui->tableWidgeConditionList->item(currentEditConditionIndex,0)->setText(newConString);
            ui->tableWidgeConditionList->resizeRowsToContents();
            if(currentEditConditionIndex==newAddConditionIndex){
                //提交的是新建的规则
                newAddConditionIndex=-1;
                newAddConditionInited=true;
            }
            statusBarDisplayMessage("当前条件分组编辑已提交,你可以继续新建条件规则或者编辑必填字段列表...");
        }
        else{
            statusBarDisplayMessage("条件信息未发生改变,无需提交...");
        }
    }
    else if(currentEditConditionIndex==-1){
        statusBarDisplayMessage("请选择你要编辑的条件分组...");
    }
}


void FormFieldCheckEditTools::on_pushButton_summitField_clicked()
{
    if(ui->tableWidgetFieldList->rowCount()>0){
        QString fieldString;
        QList<int> currentEditRequiredFieldsNew;
        for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            QWidget *w = ui->tableWidgetFieldList->cellWidget(i, 3);
            QCheckBox * checkBox = qobject_cast<QCheckBox*>(w->children().at(1));
            if(checkBox->checkState() == Qt::Checked){
                currentEditRequiredFieldsNew.append(i);
                if(fieldString.isEmpty()){
                    fieldString.append(ui->tableWidgetFieldList->item(i,0)->text());
                }
                else{
                    fieldString.append(",").append(ui->tableWidgetFieldList->item(i,0)->text());
                }
            }
        }
        if(currentEditRequiredFieldsNew.count()>0){
            bool changeFlag=false;
            for(int item:currentEditRequiredFieldsNew){
                if (!currentEditRequiredFields.contains(item)){
                    changeFlag=true;
                    break;
                }
            }
            if(!changeFlag){
                for(int item:currentEditRequiredFields){
                    if (!currentEditRequiredFieldsNew.contains(item)){
                        changeFlag=true;
                        break;
                    }
                }
            }
            if(changeFlag){
                currentEditRequiredFields=currentEditRequiredFieldsNew;
                ui->textBrowserVisualizationConfig->setText(fieldString);
                statusBarDisplayMessage("当前必填字段列表已提交,你可以继续编辑必填字段列表或者编辑条件规则...");
            }
            else{
                statusBarDisplayMessage("勾选的必填字段未发生变化,无需提交...");
            }
        }
        else{
            statusBarDisplayMessage("没有勾选任何字段为必填字段,这可不太行哟...");
        }
    }
}


void FormFieldCheckEditTools::on_tableWidgeConditionList_customContextMenuRequested(const QPoint &pos)
{
    table_condition_PopMenu->clear();
    if(!editFieldCheckMode){return;}
    //判断当前鼠标位置是不是在表格单元格位置内
    //空表
    if(ui->tableWidgeConditionList->rowCount()<1||ui->tableWidgeConditionList->rowAt(pos.y()) <0){
        table_condition_PopMenu->addAction(action_table_condition_add);
    }
    else{
        table_condition_PopMenu->addAction(action_table_condition_add);
        QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgeConditionList->selectedRanges();
        if(itemsRange.count()==1&&itemsRange.at(0).topRow()==itemsRange.at(0).bottomRow()){
            table_condition_PopMenu->addAction(action_table_condition_edit_select);
        }
        table_condition_PopMenu->addAction(action_table_condition_delete_select);
    }
    table_condition_PopMenu->exec(QCursor::pos());
}

void FormFieldCheckEditTools::fieldConEditSelect(){
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgeConditionList->selectedRanges();
    if(itemsRange.count()==1&&itemsRange.at(0).topRow()==itemsRange.at(0).bottomRow()){
        currentEditConditionIndex=itemsRange.at(0).topRow();
        QList<CheckCondition> currenGroupconList=currentEditConListOverList.at(currentEditConditionIndex);
        //字段必填转化成map
        QMap<int,QString> map;
        for(int aa=0;aa<currenGroupconList.count();aa++){
            map.insert(currenGroupconList.at(aa).index,currenGroupconList.at(aa).value);
        }
        for(int bb=0;bb<ui->tableWidgetFieldList->rowCount();bb++){
            if(map.contains(bb)){
                QTableWidgetItem *item=new QTableWidgetItem;
                item->setText(map.value(bb));
                ui->tableWidgetFieldList->setItem(bb,2,item);
            }
            else{
                QTableWidgetItem *item=new QTableWidgetItem;
                item->setText("");
                ui->tableWidgetFieldList->setItem(bb,2,item);
            }
        }
    }else{
        statusBarDisplayMessage("选择一行条件进行编辑...");
    }
}

void FormFieldCheckEditTools::fieldConAdd(){
    if(newAddConditionIndex!=-1){
        statusBarDisplayMessage(QString("序号为%1新建的条件规则还没提交,请编辑提交后再新建新的条件规则...").arg(newAddConditionIndex+1));
    }
    else{
        QList<CheckCondition> newCon;
        //新增一个空规则编辑
        currentEditConListOverList.append(newCon);
        ui->tableWidgeConditionList->setRowCount(currentEditConListOverList.count());
        QTableWidgetItem *item= new QTableWidgetItem();
        item->setText("新增条件编辑中...");
        ui->tableWidgeConditionList->setItem(currentEditConListOverList.count()-1,0,item);
        QTableWidgetSelectionRange rangeSelect(currentEditConListOverList.count()-1,0,currentEditConListOverList.count()-1,0);
        QTableWidgetSelectionRange rangeAll(0,0,currentEditConListOverList.count()-1,0);
        ui->tableWidgeConditionList->setRangeSelected(rangeAll,false);
        ui->tableWidgeConditionList->setRangeSelected(rangeSelect,true);
        for(int bb=0;bb<ui->tableWidgetFieldList->rowCount();bb++){
            QTableWidgetItem *item=new QTableWidgetItem;
            item->setText("");
            ui->tableWidgetFieldList->setItem(bb,2,item);
        }
        currentEditConditionIndex=currentEditConListOverList.count()-1;
        newAddConditionIndex=currentEditConListOverList.count()-1;
        newAddConditionInited=false;
    }
}

void FormFieldCheckEditTools::checkAdd(){
    if(!editFieldCheckMode){
        if(newAddCheckIndex!=-1){
            statusBarDisplayMessage(QString("序号为%1新建的必填规则还没提交,请编辑提交后再新建新的必填规则...").arg(newAddCheckIndex+1));
        }
        else{
            //新建一个空的检查项目
            fieldcheckitem checkItem;
            currentEditConfigSegmentCheckItemList.append(checkItem);
            ui->tableWidgetCheckList->setRowCount(currentEditConfigSegmentCheckItemList.count());
            QTableWidgetItem *item0=new QTableWidgetItem;
            item0->setText("新增规则编辑中...");
            ui->tableWidgetCheckList->setItem(currentEditConfigSegmentCheckItemList.count()-1,0,item0);
            QTableWidgetItem *item1=new QTableWidgetItem;
            item1->setText("新增规则编辑中...");
            ui->tableWidgetCheckList->setItem(currentEditConfigSegmentCheckItemList.count()-1,1,item1);
            //选中新增的行
            QTableWidgetSelectionRange addRange=QTableWidgetSelectionRange(currentEditConfigSegmentCheckItemList.count()-1,0,currentEditConfigSegmentCheckItemList.count()-1,ui->tableWidgetCheckList->columnCount()-1);
            ui->tableWidgetCheckList->clearSelection();
            ui->tableWidgetCheckList->scrollToBottom();
            ui->tableWidgetCheckList->setRangeSelected(addRange,true);
            //进入编辑模式
            editFieldCheckMode=true;
            editPrimaryMode=false;
            inEditMode();
            this->setWindowTitle(windowName+"-规则编辑中");
            //设置正在编辑的坐标
            currentEditCheckIndex=currentEditConfigSegmentCheckItemList.count()-1;
            newAddCheckIndex=currentEditConfigSegmentCheckItemList.count()-1;
            newAddCheckInited=false;
            if(ui->tableWidgetFieldList->rowCount()>0){
                for (int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
                    ui->tableWidgetFieldList->item(i,2)->setText("");
                    QCheckBox * checkBox3 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i, 3)->children().at(1));
                    checkBox3->setCheckState(Qt::Unchecked);
                }
            }
            //新建一个条件分组
            fieldConAdd();
        }
    }
}

void FormFieldCheckEditTools::fieldConDeleteSelect(){
    QList<QTableWidgetSelectionRange> itemsRange=ui->tableWidgeConditionList->selectedRanges();
    //用于评估删除后新建的规则编号被减少了多少--既新建的规则上面被删除了多少条
    int minnewAddConditionIndexCount=0;
    if(itemsRange.count()>0){
        for (int i=itemsRange.count()-1;i>=0;i--){
            QTableWidgetSelectionRange range=itemsRange.at(i);
            if (range.rowCount()>0){
                for (int row = range.bottomRow(); row>=range.topRow(); --row) {
                    ui->tableWidgeConditionList->removeRow(row);
                    currentEditConListOverList.removeAt(row);
                    if(row<newAddConditionIndex){
                        minnewAddConditionIndexCount+=1;
                    }
                    //删除正在编辑的这行条件时清除编辑区
                    if(row==currentEditConditionIndex){
                        for(int aaa=0;aaa<ui->tableWidgetFieldList->rowCount();aaa++){
                            ui->tableWidgetFieldList->item(aaa,2)->setText("");
                        }
                    }
                    if(row==newAddConditionIndex){
                        newAddConditionIndex=-1;
                        newAddConditionInited=true;
                    }
                }
                //设置当前编辑的为-1
                currentEditConditionIndex=-1;
            }
        }
        if(newAddConditionIndex>0){
            newAddConditionIndex=newAddConditionIndex-minnewAddConditionIndexCount;
            //兜底异常
            if(newAddConditionIndex<0){
                newAddConditionIndex=-1;
            }
        }
    }
}

void FormFieldCheckEditTools::on_pushButton_Summit_clicked()
{
    if(editFieldCheckMode&&!editPrimaryMode){
        if(!newAddConditionInited){
            statusBarDisplayMessage(QString("条件分组列表中,编号为%1的新增的条件分组还未提交,请先编辑并提交该条件配置或删除该条件配置...").arg(currentEditConditionIndex+1));
            return;
        }
        if(currentEditConListOverList.empty()){
            statusBarDisplayMessage("还没有任何有效的条件,请先配置并提交条件...");
            return;
        }
        if(currentEditRequiredFields.empty()){
            statusBarDisplayMessage("还没有任何有效的必填字段,请先配置并提交必填字段...");
            return;
        }
        if(editFieldCheckMode&&currentEditConditionIndex!=-1){
            bool fieldChange=false;
            bool conChange=false;
            //检查字段是发生了变化
            QList<int> currentEditOldRequiredFields=currentEditConfigSegmentCheckItemList.at(currentEditCheckIndex).getRequiredFields();
            if(currentEditRequiredFields.count()!=currentEditOldRequiredFields.count()){
                fieldChange=true;
            }
            else if (currentEditRequiredFields.count()>0&&currentEditOldRequiredFields.count()>0){
                for(int i=0;i<currentEditOldRequiredFields.count();i++){
                    if(currentEditRequiredFields.at(i)!=currentEditOldRequiredFields.at(i)){
                        fieldChange=true;
                        break;
                    }
                }
            }
            QList<QList<CheckCondition>> conListOverListOld=currentEditConfigSegmentCheckItemList.at(currentEditCheckIndex).getCheckConditionsListOverList();
            if(!fieldChange){
                if(conListOverListOld.count()!=currentEditConListOverList.count()){
                    conChange=true;
                }else if (conListOverListOld.count()>0&&currentEditConListOverList.count()>0){
                    for(int i=0;i<conListOverListOld.count();i++){
                        if(conChange){
                            break;
                        }
                        QList<CheckCondition> oldItemList=conListOverListOld.at(i);
                        QList<CheckCondition> newItemList=currentEditConListOverList.at(i);
                        if(oldItemList.count()!=newItemList.count()){
                            conChange=true;
                            break;
                        }
                        else if(oldItemList.count()>0&&newItemList.count()>0){
                            for(int i=0;i<oldItemList.count();i++){
                                if(oldItemList.at(i).index!=newItemList.at(i).index||oldItemList.at(i).value!=newItemList.at(i).value){
                                    conChange=true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            //检测字段列表是否发生了变化
            if(!fieldChange&&!conChange){
                outEditMode();
                statusBarDisplayMessage("配置没有发生任何修改,忽略提交...");
            }
            else{
                changeNotSaveToFile=true;
                //将编辑的字段和条件写入待保存的必填清单
                fieldcheckitem newCheck=currentEditConfigSegmentCheckItemList.at(currentEditCheckIndex);
                newCheck.setRequiredFields(currentEditRequiredFields);
                newCheck.setCheckConditions(currentEditConListOverList);
                //写入新的
                currentEditConfigSegmentCheckItemList.replace(currentEditCheckIndex,newCheck);
                //将编辑的描述信息写入到列表
                QString con;
                for(int i=0;i<ui->tableWidgeConditionList->rowCount();i++){
                    if(con.isEmpty()){
                        con.append(ui->tableWidgeConditionList->item(i,0)->text());
                    }else{
                        con.append(";").append(ui->tableWidgeConditionList->item(i,0)->text());
                    }
                }
                ui->tableWidgetCheckList->item(currentEditCheckIndex,0)->setText(con);
                ui->tableWidgetCheckList->item(currentEditCheckIndex,1)->setText(ui->textBrowserVisualizationConfig->toPlainText());
                if(currentEditCheckIndex==newAddCheckIndex){
                    newAddCheckIndex=-1;
                    newAddCheckInited=true;
                }
                outEditMode();
                statusBarDisplayMessage("字段必填编辑已成功提交,待写入配置文件...");
            }
        }
    }
    if(!editFieldCheckMode&&editPrimaryMode){
        QList <uint> newPrimaryKeyFieldList;
        bool flag=false;
        for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            QCheckBox * checkBox4 = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(i,4)->children().at(1));
            checkBox4->setEnabled(false);
            if(checkBox4->checkState()==Qt::Checked){
                if(!flag&&!currentEditConfigSegmentprimaryKeyFieldList.contains(i)){
                    flag=true;
                }
                newPrimaryKeyFieldList.append(i);
            }
            else{
                if(!flag&&currentEditConfigSegmentprimaryKeyFieldList.contains(i)){
                    flag=true;
                }
            }
        }
        if(!flag){
            outEditMode();
            statusBarDisplayMessage("配置没有发生任何修改,忽略提交...");
        }
        else{
            changeNotSaveToFile=true;
            currentEditConfigSegmentprimaryKeyFieldList=newPrimaryKeyFieldList;
            outEditMode();
            statusBarDisplayMessage("主键字段编辑已成功提交,待写入配置文件...");
        }
    }
}


void FormFieldCheckEditTools::on_pushButton_AbandonWrite_clicked()
{
    //修改标记改为未修改
    changeNotSaveToFile=false;
    newAddCheckIndex=-1;
    //模拟退出编辑,刷新各个按钮状态
    outEditMode();
    //重新加载数据
    int index1=ui->comboBox_1->currentIndex();
    int index2=ui->comboBox_2->currentIndex();
    int index3=ui->comboBox_3->currentIndex();
    initCurrentItem(index1,index2,index3);
    statusBarDisplayMessage("放弃写入配置,已还原配置...");
}


void FormFieldCheckEditTools::on_pushButton_WriteToFile_clicked()
{

    if(newAddCheckIndex!=-1){
        statusBarDisplayMessage(QString("必填检查列表中,编号为%1的新增的必填检查还未编辑初始化,请先编辑并提交该配置或删除该配置...").arg(newAddCheckIndex+1));
        return;
    }
    else{
        if(Utils::isFileExist(Utils::getConfigPath()+currentIni)){
            QString loadedIni=Utils::getConfigPath()+currentIni;
            //使用QSetting编辑ini会失去注释,所以这里干脆还是自己读写ini吧，呜呜呜
            QFile dataFile(loadedIni);
            QList<QString> iniData;
            bool findConfig=false;
            int configbeginIndex=-1;
            int insertIndex=-1;
            int oldStartIndex=-1;
            int oldEndIndex=-1;
            QString parimaryKeyListStr="";
            if(!currentEditConfigSegmentprimaryKeyFieldList.empty()){
                for(int i=0;i<currentEditConfigSegmentprimaryKeyFieldList.count();i++){
                    if(parimaryKeyListStr.isEmpty()){
                        parimaryKeyListStr.append(QString::number(currentEditConfigSegmentprimaryKeyFieldList.at(i)));
                    }
                    else{
                        parimaryKeyListStr.append(",").append(QString::number(currentEditConfigSegmentprimaryKeyFieldList.at(i)));
                    }
                }
            }
            if (dataFile.open(QFile::ReadOnly))
            {
                QTextCodec *codec=QTextCodec::codecForName("UTF-8");
                QString line;
                int lineNumber=0;
                while (!dataFile.atEnd())
                {
                    line = codec->toUnicode(dataFile.readLine()).remove('\r').remove('\n');
                    iniData.append(line);
                    //找到了该段配置
                    if(!findConfig&&line==currentConfigSegment){
                        findConfig=true;
                        configbeginIndex=lineNumber;
                    }
                    lineNumber++;
                }
                dataFile.close();
                if(findConfig){
                    int priamyKeyIndex=-1;
                    if(iniData.count()>configbeginIndex+1){
                        for(int i=configbeginIndex+1;i<iniData.count();i++){
                            //主键位置
                            if(priamyKeyIndex<0&&iniData.at(i).startsWith("fieldprimarylist=")){
                                priamyKeyIndex=i;
                            }
                            //找到第一个必填字段检查或者空行或者下一个配置段的开始
                            if(insertIndex==-1&&((iniData.at(i).startsWith("[")&&iniData.at(i).endsWith("]"))||iniData.at(i).trimmed().isEmpty()||iniData.at(i).startsWith("fieldcheck_"))){
                                insertIndex=i;
                            }
                            //老的必填字段配置开始位置
                            if(oldStartIndex==-1&&iniData.at(i).startsWith("fieldcheck_")){
                                oldStartIndex=i;
                            }
                            //老的必填字段配置结束位置-找到最后一个必填
                            if(oldStartIndex!=-1&&iniData.at(i).startsWith("fieldcheck_")){
                                oldEndIndex=i;
                                if(i==iniData.count()-1||(i<iniData.count()-1&&iniData.at(i+1).startsWith("[")&&iniData.at(i+1).endsWith("]"))||iniData.at(i+1).trimmed().isEmpty()){
                                    break;
                                }
                            }
                            //文件尾特殊处理
                            if(oldStartIndex>0&&oldEndIndex==-1&&i==iniData.count()-1){
                                oldEndIndex=i;
                                break;
                            }
                        }
                        //主键配置
                        //新的为空，当前非空-替换
                        if(parimaryKeyListStr.isEmpty()&&priamyKeyIndex>0){
                            iniData.replace(priamyKeyIndex,"fieldprimarylist=\"\"");
                        }
                        //新的非空，当前非空，替换
                        else if(!parimaryKeyListStr.isEmpty()&&priamyKeyIndex>0){
                            iniData.replace(priamyKeyIndex,"fieldprimarylist=\""+parimaryKeyListStr+"\"");
                        }
                        //先清除老的配置
                        if(oldEndIndex>=oldStartIndex&&oldEndIndex>=0&&oldStartIndex>=0){
                            for(int i=oldEndIndex;i>=oldStartIndex;i--){
                                iniData.removeAt(i);
                            }
                        }
                        //其余场景放到后面插入
                        //这里开始插入新的配置
                        if(insertIndex>0){
                            //生成规则并写入配置文件
                            QList<QString> newcheckList;
                            if(currentEditConfigSegmentCheckItemList.count()>0){
                                int chekIndex=0;
                                for(fieldcheckitem item:currentEditConfigSegmentCheckItemList){
                                    QString StringConfigLine;
                                    if(item.getCheckConditionsListOverList().count()>0){
                                        //构造条件
                                        for(QList<CheckCondition> current:item.getCheckConditionsListOverList()){
                                            if(!StringConfigLine.isEmpty()){
                                                StringConfigLine.append(";");
                                            }
                                            if(current.count()>0){
                                                if(current.at(0).index==ALL_CHECK_INDEX){
                                                    StringConfigLine.append("ALL");
                                                }
                                                else{
                                                    for(int i=0;i<current.count();i++){
                                                        if(!StringConfigLine.isEmpty()){
                                                            StringConfigLine.append("&");
                                                        }
                                                        StringConfigLine.append(QString::number(current.at(i).index+1)).append("=").append(current.at(i).value);
                                                    }
                                                }
                                            }
                                        }
                                        //然后构造必填字段
                                        if(!StringConfigLine.isEmpty()){
                                            StringConfigLine.append("|");
                                            QList<int> must=item.getRequiredFields();
                                            if(must.count()>0){
                                                for(int ii=0;ii<must.count();ii++){
                                                    if (ii!=must.count()-1){
                                                        StringConfigLine.append(QString::number(must.at(ii)+1)).append(",");
                                                    }
                                                    else{
                                                        StringConfigLine.append(QString::number(must.at(ii)+1));
                                                    }
                                                }
                                            }
                                            else{
                                                statusBarDisplayMessage("解析规则异常,存在空的必填字段列表,请重试...");
                                                return;
                                            }
                                        }
                                        //构造参数和必填列表结束
                                        chekIndex++;
                                        newcheckList.append("fieldcheck_"+QString("%1").arg(chekIndex,4,10,QLatin1Char('0'))+"=\""+StringConfigLine+"\"");
                                    }
                                    else{
                                        statusBarDisplayMessage("解析规则异常,找不到有效的条件分组,请重试...");
                                        return;
                                    }
                                }
                            }
                            if(newcheckList.count()>0){
                                for(int aaa=newcheckList.count()-1;aaa>=0;aaa--){
                                    iniData.insert(insertIndex,newcheckList.at(aaa));
                                }
                            }
                            //主键-新的非空,当前空，插入，在必填字段之前
                            if(!parimaryKeyListStr.isEmpty()&&priamyKeyIndex<0){
                                iniData.insert(insertIndex,"fieldprimarylist=\""+parimaryKeyListStr+"\"");
                            }
                            //写入文件
                            if (dataFile.open(QFile::WriteOnly | QIODevice::Truncate)) {
                                QString fileData;
                                for(QString line:iniData){
                                    if(!fileData.isEmpty()){
                                        fileData.append("\r\n").append(line);
                                    }
                                    else{
                                        fileData.append(line);
                                    }
                                }
                                //写入并进行一系列的更新
                                dataFile.write(codec->fromUnicode(fileData));
                                dataFile.flush();
                                changeNotSaveToFile=false;
                                outEditMode();
                                int index1=ui->comboBox_1->currentIndex();
                                int index2=ui->comboBox_2->currentIndex();
                                int index3=ui->comboBox_3->currentIndex();
                                if(index1==0){
                                    int l1=0;
                                    for(ConfigFile<OFDFileDefinition> configFile:ofdConfigList){
                                        if(l1==index2){
                                            int l2=0;
                                            QList<OFDFileDefinition> newConfigSegmentList;
                                            for(OFDFileDefinition configSegment:configFile.getConfigSegmentList()){
                                                if(l2==index3){
                                                    qDebug()<<index1<<index2<<index3;
                                                    configSegment.setFieldcheckItemList(currentEditConfigSegmentCheckItemList);
                                                    configSegment.setPrimaryKeyFieldList(currentEditConfigSegmentprimaryKeyFieldList);
                                                }
                                                newConfigSegmentList.append(configSegment);
                                                l2++;
                                            }
                                            configFile.setConfigSegmentList(newConfigSegmentList);
                                            ofdConfigList.replace(index2,configFile);
                                        }
                                        l1++;
                                    }
                                }
                                else if(index1==1){
                                    int l1=0;
                                    for(ConfigFile<CsvFileDefinition> configFile:csvConfigList){
                                        if(l1==index2){
                                            int l2=0;
                                            QList<CsvFileDefinition> newConfigSegmentList;
                                            for(CsvFileDefinition configSegment:configFile.getConfigSegmentList()){
                                                if(l2==index3){
                                                    qDebug()<<index1<<index2<<index3;
                                                    configSegment.setFieldcheckItemList(currentEditConfigSegmentCheckItemList);
                                                    configSegment.setPrimaryKeyFieldList(currentEditConfigSegmentprimaryKeyFieldList);
                                                }
                                                newConfigSegmentList.append(configSegment);
                                                l2++;
                                            }
                                            configFile.setConfigSegmentList(newConfigSegmentList);
                                            csvConfigList.replace(index2,configFile);
                                        }
                                        l1++;
                                    }
                                }
                                else if (index1==3){
                                    int l1=0;
                                    for(ConfigFile<FIXEDFileDefinition> configFile:fixedConfigList){
                                        if(l1==index2){
                                            int l2=0;
                                            QList<FIXEDFileDefinition> newConfigSegmentList;
                                            for(FIXEDFileDefinition configSegment:configFile.getConfigSegmentList()){
                                                if(l2==index3){
                                                    qDebug()<<index1<<index2<<index3;
                                                    configSegment.setFieldcheckItemList(currentEditConfigSegmentCheckItemList);
                                                    configSegment.setPrimaryKeyFieldList(currentEditConfigSegmentprimaryKeyFieldList);
                                                }
                                                newConfigSegmentList.append(configSegment);
                                                l2++;
                                            }
                                            configFile.setConfigSegmentList(newConfigSegmentList);
                                            fixedConfigList.replace(index2,configFile);
                                        }
                                        l1++;
                                    }
                                }
                                statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]更新完毕...");
                                if(!modifyConfigList.contains(currentIni)){
                                    modifyConfigList.append(currentIni);
                                }
                                emit sendConfigChange(true,modifyConfigList);
                                this->setWindowTitle(windowName);
                            }
                            else{
                                statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]写入失败,请重试...");
                            }
                        }
                        else{
                            statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]中更新必填字段配置异常,找不到合适的插入点,请重试...");
                        }
                    }
                    else{
                        statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]中更新必填字段配置异常,请重试...");
                    }
                }
                else{
                    statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]中未找到该配置的定义,请重试...");
                }
            }
            else{
                statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]读取失败,请重试...");
            }
        }
        else{
            statusBarDisplayMessage("配置文件["+Utils::getConfigPath()+currentIni+"]不存在,请重试...");
        }
    }
}

void FormFieldCheckEditTools::closeEvent(QCloseEvent *event){
    if(changeNotSaveToFile){
        DialogMyTip dialog("还有配置修改没写入到配置文件,放弃保存并退出？",this);
        dialog.setWindowTitle("警告！");
        dialog.setModal(true);
        dialog.exec();
        if(!dialog.getBoolFlag()){
            event->ignore();
            return;
        }
        else{
            event->accept();
        }
    }
    else{
        event->accept();
    }
}

void FormFieldCheckEditTools::on_pushButton_ImportCon_clicked()
{
    if(ui->tableWidgetFieldList->rowCount()<1){
        return;
    }
    else{
        QStringList fieldList;
        for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            fieldList.append(ui->tableWidgetFieldList->item(i,0)->text());
        }
        DialogEditText *dialog=new DialogEditText(QString("导入条件规则覆盖或者合并当前编辑中的条件"),fieldList,0,this);
        connect(dialog,&DialogEditText::sendImportString,this,&FormFieldCheckEditTools::getImportString);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setModal(true);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}


void FormFieldCheckEditTools::on_pushButton_ImportField_clicked()
{
    if(ui->tableWidgetFieldList->rowCount()<1){
        return;
    }
    else{
        QStringList fieldList;
        for(int i=0;i<ui->tableWidgetFieldList->rowCount();i++){
            fieldList.append(ui->tableWidgetFieldList->item(i,0)->text());
        }
        QString title="";
        int type=0;
        if(editFieldCheckMode&&!editPrimaryMode){
            title="导入必填字段列表覆盖或者合并当前编辑中的字段列表";
            type=1;
        }
        else if(!editFieldCheckMode&&editPrimaryMode){
            title="导入主键字段列表覆盖或者合并当前编辑中的字段列表";
            type=2;
        }
        DialogEditText *dialog=new DialogEditText(title,fieldList,type,this);
        connect(dialog,&DialogEditText::sendImportString,this,&FormFieldCheckEditTools::getImportString);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setModal(true);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

void FormFieldCheckEditTools::getImportString(QString data,int type,bool clearOldData){
    if(data.isEmpty()){
        return;
    }
    //导入条件规则
    if(type==0){
        QStringList checkList=data.split("&");
        QMap<QString,QString> valueMap;
        for(int i=0;i<checkList.count();i++){
            QStringList item=checkList.at(i).split("=");
            if(item.count()!=2){
                statusBarDisplayMessage("导入条件列表失败,数据不规范...");
                return;
            }
            else{
                valueMap.insert(item.at(0),item.at(1));
            }
        }
        int count=0;
        if(valueMap.count()>0){
            for(int bb=0;bb<ui->tableWidgetFieldList->rowCount();bb++){
                if(valueMap.contains(ui->tableWidgetFieldList->item(bb,0)->text())){
                    QTableWidgetItem *item=new QTableWidgetItem;
                    item->setText(valueMap.value(ui->tableWidgetFieldList->item(bb,0)->text()));
                    ui->tableWidgetFieldList->setItem(bb,2,item);
                    count++;
                }
                else if(clearOldData){
                    QTableWidgetItem *item=new QTableWidgetItem;
                    item->setText("");
                    ui->tableWidgetFieldList->setItem(bb,2,item);
                }
            }
            statusBarDisplayMessage(QString("成功导入%1个条件...").arg(count));
        }
    }
    //导入必填字段列表
    else if (type==1){
        QStringList fieldCheckList=data.split(",");
        int count=0;
        if(fieldCheckList.count()>0){
            for(int bb=0;bb<ui->tableWidgetFieldList->rowCount();bb++){
                if(fieldCheckList.contains(ui->tableWidgetFieldList->item(bb,0)->text())){
                    count++;
                    QCheckBox * checkBox = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 3)->children().at(1));
                    checkBox->setCheckState(Qt::Checked);
                }
                else if(clearOldData){
                    QCheckBox * checkBox = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 3)->children().at(1));
                    checkBox->setCheckState(Qt::Unchecked);
                }
            }
            statusBarDisplayMessage(QString("成功导入%1个字段...").arg(count));
        }
    }
    //导入主键字段列表
    else if (type==2){
        QStringList fieldCheckList=data.split(",");
        int count=0;
        if(fieldCheckList.count()>0){
            for(int bb=0;bb<ui->tableWidgetFieldList->rowCount();bb++){
                if(fieldCheckList.contains(ui->tableWidgetFieldList->item(bb,0)->text())){
                    count++;
                    QCheckBox * checkBox = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 4)->children().at(1));
                    checkBox->setCheckState(Qt::Checked);
                }
                else if(clearOldData){
                    QCheckBox * checkBox = qobject_cast<QCheckBox*>(ui->tableWidgetFieldList->cellWidget(bb, 4)->children().at(1));
                    checkBox->setCheckState(Qt::Unchecked);
                }
            }
            statusBarDisplayMessage(QString("成功导入%1个字段...").arg(count));
        }
    }
}


void FormFieldCheckEditTools::on_pushButton_PrimaryKey_clicked()
{
    editFieldCheckMode=false;
    editPrimaryMode=true;
    //进入编辑模式
    inEditMode();
}

