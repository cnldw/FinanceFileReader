#include "dialogshowimportexcelerror.h"
#include "ui_dialogshowimportexcelerror.h"

DialogShowImportExcelError::DialogShowImportExcelError(QStringList * errinfo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowImportExcelError)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
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
    ptr_table->setColumnCount(1);
    ptr_table->setAlternatingRowColors(true);
    ptr_table->verticalHeader()->setDefaultSectionSize(22);
    //设定默认的规则，表格宽度自动适应窗口
    ptr_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置表格列标题
    QStringList title;
    title.append("错误信息描述");
    ptr_table->setHorizontalHeaderLabels(title);
    ptr_table->setRowCount(errinfo->count());
    for (int i=0;i<errinfo->count();i++){
        QTableWidgetItem *itemini= new QTableWidgetItem(errinfo->at(i));
        ptr_table->setItem(i, 0, itemini);
    }
}

DialogShowImportExcelError::~DialogShowImportExcelError()
{
    delete ui;
}
