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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <time.h>
#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QLabel>
#include <QSettings>
#include <QDebug>
#include <QHash>
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QClipboard>
#include <QColor>
#include <QDateTime>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QFuture>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include "src/utils.h"
#include "src/ofdfiledefinition.h"
#include "src/ofdfielddefinition.h"
#include "src/fixedfiledefinition.h"
#include "src/fixedfielddefinition.h"
#include "src/dialogshowtablerow.h"
#include "src/dialogchoosefiletype.h"
#include "src/dialogshowtablefieldcheck.h"
#include "src/dialogshowtablecompareview.h"
#include "src/dialogmytip.h"
#include "src/dictionary.h"
#include "src/ofdcodeinfo.h"
#include "src/xlsx/xlsxdocument.h"
#include "src/xlsx/xlsxcellrange.h"
#include "src/dialogmodifycell.h"
#include "src/dialogaboutauthor.h"
#include "src/dialogaboutthis.h"
#include "src/dialogmergetip.h"
#include "src/csvfiledefinition.h"
#include "src/faultcause.h"
#include "src/dialogopenfileadv.h"
#include "fieldisnumber.h"
#include "src/dialogpreference.h"
#include "src/dialogmodifyrow.h"
#include "src/publicdefine.h"
#include "src/ofdfaultcause.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc, char *argv[],QWidget *parent = nullptr);
    ~MainWindow();

protected:
    //文件拖拽支持
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    //窗口尺寸变化事件
    void resizeEvent (QResizeEvent * event );
    //槽函数
private slots:

    void on_fileOpen_triggered();

    void on_aboutOpen_triggered();

    void on_actionAboutQt_triggered();

    void on_pushButtonOpenFile_clicked();

    void on_pushButtonOpenFile_2_clicked();

    //接受滚动条事件
    void acceptVScrollValueChanged(int value);

    void on_pushButtonPreSearch_clicked();

    void on_pushButtonNextSearch_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void editCompareData();

    void deleteRowDataFromFileAndTable();

    void copyOFDRowData();

    void addOFDRowDataPreviousRow();

    void addOFDRowDataNextRow();

    void addOFDRowDataEndRow();

    void addOFDNewLineRowDataEndRow();

    void showMoaifyOFDRow();

    void copyToClipboard();

    void showRowDetails();

    void showOFDFiledAnalysis();

    void showModifyOFDCell();

    void save2XlsxFinished();

    void loadFileOnWindowisOpenOrDragEnter();

    void showModifyOFDCellBatch();

    void on_actionsOpenCompare_triggered();

    void on_actionClearCompare_triggered();

    void on_pushButtonExport_clicked();

    void on_pushButtonRowJump_clicked();

    void on_actionsss_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAS_triggered();

    void on_actionaboutAuthor_triggered();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButtonColumnJump_clicked();

    void on_pushButtonRowJump2_clicked();

    void on_viewMode_triggered();

    void on_tableWidget_itemSelectionChanged();

    void on_actionfileOpenAdv_triggered();

    void on_actionpreference_triggered();

    void on_actionnewWindow_triggered();

    void on_pushButtonColumnJump_2_clicked();

    void on_actionwebsite_triggered();

    void on_actionmanual_triggered();

    void on_currentOpenFilePathLineText_returnPressed();

    void on_pushButtonPageFirst_clicked();

    void on_pushButtonPagePrevious_clicked();

    void on_pushButtonPageNext_clicked();

    void on_pushButtonPageLast_clicked();

    void on_pushButtonGo_clicked();

private:
    Ui::MainWindow *ui;
    //应用程序名字
    QString appName=tr("金融文件阅读器-").append(VERSION_V);
    //状态栏指针变量
    QLabel *statusLabel_ptr_showCount;
    QLabel *statusLabel_ptr_showRowAndCol;
    QLabel *statusLabel_ptr_showMessage;
    //表格指针
    QTableWidget *ptr_table;
    //当前打开的文件的路径
    QString currentOpenFilePath="";
    //当前打开的文件名字
    QString currentFileName="";
    //程序启动时或者拖拽进来需要加载的文件,当次变量不为空时，timer定时任务会扫描到文件后加载
    QString startUpOrDragfile="";
    //程序启动时读取是否需要加载文件的定时器，以及定时扫描是否拖拽了文件进来
    QTimer *loadFiletimer;
    //已经加载的OFDcode信息,记录销售商和TA的代码信息
    QHash<QString, OFDCodeInfo> loadedOfdCodeInfo;
    //已经加载的OFD索引文件信息,记录各种索引文件的文件名开头三个字符
    QHash<QString, QString> loadedOfdIndexFileInfo;
    //已经加载的CSV类的文件名的正则匹配器--CSV文件判断类别，使用简单的正则匹配方法
    QHash<QString, QString> loadedCsvFileInfo;
    //已经加载的定长文件文件名的正则匹配器--FIXED文件判断类别，使用简单的正则匹配方法
    QHash<QString, QString> loadedFixedFileInfo;
    //已经加载的各种OFD文件的定义,比如21_01,代表第21版本的01文件的定义，使用Hash记录<_21_01,21版本下的01文件的定义>
    QHash<QString,OFDFileDefinition>loadedOfdDefinitionHash;
    //已经加载的CSV文件类别信息,csv文件的文件名可能相同但是却是不同版本，所以使用List遍历匹配
    QList<CsvFileDefinition> loadedCsvDefinitionList;
    //已经加载的FIXED文件类别信息,定长文件的文件名可能相同但是却是不同版本，所以使用List遍历匹配
    QList<FIXEDFileDefinition> loadedFixedDefinitionList;
    //用来记录文件头部内容的map,此信息用于文件检查
    QHash<QString,QString> ofdIndexFileHeaderMap;
    //用来记录文件标题和内容的list,解析索引类文件时使用
    QList<QStringList> indexFileDataList;
    //当前正在使用的ofd定义,打开哪个文件,就切换到改文件的ofd定义
    OFDFileDefinition ofd;
    //当前打开的csv文件使用的csv定义，打开哪个文件,就切换到改文件的csv定义
    CsvFileDefinition csv;
    //用于记录csv等类别文件哪些列是数值的变量，注意，这个是否是数值是猜出来的，根据前几行的数据，进猜取小数，不猜整数
    QHash<int,FieldIsNumber> fieldIsNumberOrNot;
    //当前打开的fixed文件使用的fixed定义，打开哪个文件,就切换到改文件的fixed定义
    FIXEDFileDefinition fixed;
    //打开的OFD文件的换行符
    QString ofdFileNewLine;
    //OFD文件头使用Qstring记录,作为原始记录,方便后续保存文件时直接提取文件头
    QList<QString> ofdFileHeaderQStringList;
    //OFD文件体,因为包含中英文,且要以GB18030方式记录文件内容,所以使用QByteArray
    QList<QByteArray> ofdFileContentQByteArrayList;
    //打开的CSV文件的文件头
    QList<QString> csvFileHeaderQStringList;
    //打开的CSV文件的数据体
    QList<QByteArray> csvFileContentQByteArrayList;
    //打开的Fixed文件的文件头
    QList<QString> fixedFileHeaderQStringList;
    //打开的Fixed文件的数据体
    QList<QByteArray> fixedContenQByteArrayList;
    //打开的Fixed文件的文件尾
    QList<QString> fixedFooterQStringList;
    //当前打开的文件类别,目前已支持的文件类型0索引,1:OFD数据,2:CSV文件,3:FIXED定长文件，-1未打开文件或者显示了错误信息
    int currentOpenFileType=-1;

    //OFD字典参数，专用于OFD文件，打开文件类别为1时使用
    Dictionary ofdDictionary;

    //通用字典配置-用于csv和定长文件
    QHash<QString,Dictionary> commonDictionary;
    //监控xlsx文件导出是否完成
    QFutureWatcher<int>* watcherXlsxSaveStatus_;
    //创建成员xlsx变量//xlsx文件数据存放到堆内存,注意使用完毕释放
    QXlsx::Document *xlsx=new QXlsx::Document();
    //xlsx文件保存文件名
    QString xlsxSaveName;
    //全局blocked消息,当主动block时,填充此消息,用于告知其他场景程序正在做什么
    QString dataBlockedMessage;

    //表格的右键菜单
    QMenu *tablePopMenu;
    QAction *action_ShowDetails;
    QAction *action_ShowCopyColum;
    QAction *action_ShowOFDAnalysis;
    QAction *action_EditCompareData;
    QAction *action_EditCompareDataBatch;
    QAction *action_ModifyOFDCell;
    QAction *action_ModifyOFDCellBatch;
    QAction *action_DeleteOFDRowData;
    QAction *action_CopyOFDRowData;
    QAction *action_addCopyedOFDData2PreviousRow;
    QAction *action_addCopyedOFDData2NextRow;
    QAction *action_addCopyedOFDData2End;
    QAction *action_addNewLineOFDData2End;
    QAction *action_ModifyOFDRow;

    /*
      极其重要的表格相关参数
    */
    //记录table在当前界面显示的行范围,用于判断需要刷新显示到界面的元素
    int tableRowBegin = 0;
    int tableRowEnd = 0;
    //列跳转模式当前搜索进度
    int tableColSearch=0;
    //记录当前选中的行和列
    int tableRowCurrent=0;
    int tableColCurrent=0;
    //表格当前高度
    int tableHeight;
    //表格行高度,常量-用于数据展示-如果你觉得现在的表格比较拥挤，可以增加每行高度
    const int rowHight=22;
    //配置文件加载的状态，在启动时使用
    bool configLoadCompleted=false;
    //数据更新状态,比如当前正在加载文件，正在重新刷新文件
    bool isUpdateData=false;

    //阻断的操作，比如当前正在导出文件，正在搜索，和isUpdatedata类似
    bool dataBlocked=false;
    //是否是在导出文件
    bool isExportFile=false;
    //已经加载的行,用于懒加载是判断哪些行已经加载,避免重复加载
    //优化，使用QHash提高大文件下多行数据加载后的查找效率
    QHash <int,bool> rowHasloaded;

    //优化器，记录每列表格最宽记录，当异步下次加载数据时，根据是否发生了变化来决定是否需要更新列宽度
    QHash <int,int> columnWidth;

    //加入到比对器的数据 <行号,数据>
    QMap<int,QStringList> compareData;

    //文件变化标志，如果打开了一个文件并且编辑修改了，则此标志为真
    bool fileChanged=false;

    //设置选项
    //数据内存压缩等级--默认禁用
    int dataCompressLevel=0;
    //设置选项
    //默认视图模式-0当拖进来文件时在当前窗口打开，1不同的文件在新窗口打开，2所有拖到窗口的文件在新窗口打开
    QString defaultViewMode="0";
    //设置选项
    //新文件打开方式
    QString defaultNewFileMode="0";
    //设置选项
    //每页行数类型 0-10万行,1-20万行,2-50万行,3-100万行
    QString defaultPageSizeType="0";

    //程序所有的tips，用于程序启动后随机显示一条提示
    QList<QString> tips;

    //每页行数-默认10万
    int pageRowSize=100000;
    //总页数
    int pageCount=1;
    //目前在第几页
    int currentPage=1;

    QString getConfigPath();
    void tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void statusBar_clear_statusBar();
    void statusBar_display_rowsCount(int rowsCount);
    void statusBar_display_rowsAndCol(int row,int col,int length);
    void statusBar_disPlayMessage(QString text);
    void clear_Table_Info();
    void clear_Table_Contents();
    void clear_Display_Info();
    void clear_oldData();
    void initFile(QString filePath);
    void initStatusBar();
    void open_file_Dialog();
    void load_OFDCodeInfo();
    void load_Setting();
    void load_OFDIndexFile();
    void load_OFDDictionary();
    void load_OFDDefinition();
    void load_CSVDefinition();
    void load_FIXEDDefinition();
    void load_indexFile();
    void load_ofdFile(QString fileType);
    void load_csvFile(QStringList fileType);
    void load_fixedFile(QStringList fileType);
    void load_fixedFileData();
    void load_csvFileData(QStringList title);
    void save2Csv(QString filename);
    void save2Html(QString filename);
    void save2Xlsx(QString filename);
    int save2XlxsFile();
    void init_display_IndexTable();
    void init_OFDTable();
    void init_CSVTable(QStringList title);
    void init_FIXEDTable();
    void display_OFDTable();
    void display_CSVTable();
    void display_FIXEDTable();
    void display_CSVFaultCause(QList<FaultCause> faultList);
    void display_FIXEDFaultCause(QList<FaultCause> faultList);
    void display_OFDFaultCause(QString useini,QList<OFDFaultCause> faultList);
    void saveOFDFile(QString filepath);
    void randomTips();
    void addOFDRowData(int location);
    bool ignoreFileChangeAndOpenNewFile();
    void closeEvent(QCloseEvent *event);
    void columnJump(int type);
    void pageJump(int page,int scrollIndex=0);
};

#endif // MAINWINDOW_H
