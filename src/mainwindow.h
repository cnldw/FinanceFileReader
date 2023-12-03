/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
#include <QSet>
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
#include <QPixmap>
#include <QKeySequence>
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
#include "src/QXlsx/header/xlsxdocument.h"
#include "src/QXlsx/header/xlsxcellrange.h"
#include "src/dialogmodifycell.h"
#include "src/dialogaboutauthor.h"
#include "src/dialogaboutthis.h"
#include "src/dialogmergetip.h"
#include "src/csvfiledefinition.h"
#include "src/faultcause.h"
#include "fieldisnumber.h"
#include "src/dialogpreference.h"
#include "src/dialogmodifyrow.h"
#include "src/publicdefine.h"
#include "src/ofdfaultcause.h"
#include "src/createofdwindow.h"
#include "src/dialogchooseexporttype.h"
#include "src/dialogshowcharset.h"
#include "src/dialogmodifymtime.h"
#include "src/dialogmagnify.h"
#include "src/dialogchooseofdconfig.h"
#include "src/dialogchoosedbfconfig.h"
#include "src/dialogeditheaderfooter.h"
#include "src/dialogforcenumber.h"
#include "src/ucdutils.h"
#include "src/dialogshareqrcode.h"
#include "src/qdbf/qdbftable.h"
#include "src/qdbf/qdbfrecord.h"
#include "src/qdbf/qdbffield.h"
#include "src/dbffielddefinition.h"
#include "src/dbffiledefinition.h"
#include "src/dbffileconfig.h"
#include "src/dialogoktools.h"
#include "src/msgtoast.h"
#include "src/dialogshowimportexcelerror.h"
#include "src/dialogshowfieldchecklist.h"
#include "src/fieldcheckitem.h"
#include "src/dialogshowfieldchecklist.h"
#include "src/dialogchoosefieldcheckexportreport.h"
#include "src/configfile.h"
#include "src/formfieldcheckedittools.h"

#ifdef Q_OS_WIN32
#include "src/formwebtools.h"
#endif

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief The dbfMatchInfo struct 存储匹配到的DBF配置的结构体
 */

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

    bool event(QEvent *event);

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

    void showMessage_customContextMenuRequested(const QPoint &pos);

    void editCompareData();

    void deleteRowDataFromFileAndTable();

    void copyOFDRowData();

    void addOFDRowDataPreviousRow();

    void addOFDRowDataNextRow();

    void addOFDRowDataEndRow();

    void addOFDNewLineRowDataEndRow();

    void showMoaifyOFDRow();

    void copyToClipboardWithoutTitle();

    void copyToClipboardWithTitle();

    void showRowDetails();

    void openPlugin();

    void showMagnify();

    void copyMessage();

    int importFromExcel();

    void gotoFirstNotNumber();

    void showOFDFiledAnalysis();

    void showModifyOFDCell();

    void save2XlsxFinished();

    void importFromXlsxFinished();

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

    void on_actiondifftools_triggered();

    void on_actionfileedit_triggered();

    void on_actioncreatenewofdfile_triggered();

    void on_actioncopy_triggered();

    void on_actionedit_triggered();

    void on_actionedit2_triggered();

    void on_actionopeninexcel_triggered();

    void on_actionscreen_triggered();

    void on_actiontipslist_triggered();

    void showCharacter();

    void forceNumber();

    void on_actionmtime_triggered();

    void on_actioneditheaderfooter_triggered();

    void showQrcode();

    void on_actionalldata_triggered();

    void on_actionnotdelete_triggered();

    void on_actionjustdelete_triggered();

    void on_actionGB18030_GB2312_triggered();

    void on_actionIBM437_triggered();

    void on_actionIBM850_triggered();

    void on_actionIBM866_triggered();

    void on_actionWindows1250_triggered();

    void on_actionWindows1251_triggered();

    void on_actionWindows1252_triggered();

    void on_actionBig5_triggered();

    void on_actionShift_JIS_triggered();

    void on_actioncloseFile_triggered();

    void on_actionautcolwidth_triggered();

    void on_actionleftright_triggered();

    void on_actionleft_triggered();

    void on_actionright_triggered();

    void on_actionOKTools_triggered();

    void on_actionimportfromexcel_triggered();

    void update_import_excel_status();

    void on_actionexpcheckresult_triggered();

    void on_actioncurrentfilechekrule_triggered();

    void on_actionprefieldcheck_triggered();

    void on_actionnextfieldcheck_triggered();

    void on_actiontableSelectionBehaviorMenu_triggered();


    void on_action_fieldcheckconfigtools_triggered();

public slots:
    void getNewHeaderAndFooter(QStringList header,QStringList footer);
    void getFieldExportConfig(QMap <QString,int> config);
    void getConfigFileChange(bool flag,QStringList modifyConfigList);


private:

    Ui::MainWindow *ui;
    //应用程序名字
    QString appName=tr("金融文件阅读器-").append(VERSION_V);
    //状态栏指针变量
    QLabel *statusLabel_ptr_showCount;
    QLabel *statusLabel_ptr_showRowAndCol;
    QLabel *statusLabel_ptr_showMessage;

    //当前打开的文件类别,目前已支持的文件类型0:OFD索引,1:OFD数据,2:CSV文件,3:FIXED定长文件，4:DBF文件，-1未打开文件或者显示了错误信息
    enum openFileType {NotFileOrErr=-1, OFDIndex=0, OFDFile=1, CSVFile=2, FIXEDFile=3, DBFFile=4};
    int currentOpenFileType=openFileType::NotFileOrErr;

    //哪些配置被更新了
    QStringList modifyConfigQStringList;

    bool configFileChange=false;
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
    /*****配置类****/
    //已经加载的OFDcode信息,记录销售商和TA的代码信息
    QHash<QString, OFDCodeInfo> loadedOfdCodeInfo;
    //已经加载的OFD索引文件信息,记录各种索引文件的文件名开头三个字符
    QHash<QString, QString> loadedOfdIndexFileInfo;
    //OFD配置文件列表
    QList<ConfigFile<OFDFileDefinition>> ofdConfigList;
    //类似key=21这样的Hash存储，方便打开文件的时候快速定位是否存在该文件的解析配置
    QHash<QString,int> ofdQuickMatchIndex;
    //CSV配置文件列表
    QList<ConfigFile<CsvFileDefinition>> csvConfigList;
    //FIXED配置文件列表
    QList<ConfigFile<FIXEDFileDefinition>> fixedConfigList;
    //DBF配置文件列表--dbf配置一个文件内就一段,无需嵌套ConfigFile
    QList<DbfFileConfig> dbfConfigList;
    /*****当前加载使用的配置****/
    //当前正在使用的ofd定义,打开哪个文件,就切换到该文件的ofd定义
    OFDFileDefinition ofd;
    //当前打开的csv文件使用的csv定义，打开哪个文件,就切换到该文件的csv定义
    CsvFileDefinition csv;
    //当前打开的dbf文件使用的定义,和其他类型文件不同，这个变量的内容基于打开的dbf文件里的元信息生成而不是取自配置文件
    DbfFileDefinition dbf;
    //当前打开的fixed文件使用的fixed定义，打开哪个文件,就切换到改文件的fixed定义
    FIXEDFileDefinition fixed;
    //用于记录csv文件哪些列是数值的变量，注意，这个是否是数值是猜出来的，根据前几行的数据，仅猜取小数，不猜整数
    QHash<int,FieldIsNumber> CsvFieldIsNumberOrNot;
    //各个插件的地址
    QHash<int,QString> pluginpath;
    /*****文件头文件体文件尾数据承载****/
    //通用文件头记录
    QList<QString> commonHeaderQStringList;
    //通用文件体记录
    QList<QByteArray> commonContentQByteArrayList;
    //通用文件尾记录
    QList<QString> commonFooterQStringList;
    //打开的ofd文件的文件尾
    QString ofdFooterQString;
    //用来记录文件头部内容的map,此信息用于文件检查
    QHash<QString,QString> ofdIndexFileHeaderMap;
    //用来记录文件标题和内容的list,解析索引类文件时使用
    QList<QString> ofdindexFileDataList;
    int ofdFooterRowNumber;

    //当前打开的dbf文件
    QDbf::QDbfTable dbftablefile;
    //0显示所有记录-1仅显示未删除的记录-2仅显示已删除的记录
    int  dbfDisplayType=0;
    //字段修剪方式 0 trim 1 lefttrim 2 righttrim
    int  dbfTrimType=0;
    //哪些数据类型的列会被认为是数字处理//针对dbf文件
    QList<int> dbfIsNumberFieldType;
    //行映射关系-用于在全浏览和非删除数据和仅删除数据的浏览模式切换
    QHash<int,int> dbfRowMap;

    //OFD字典参数，专用于OFD文件，打开文件类别为1时使用
    Dictionary ofdDictionary;
    //通用字典配置-用于csv和定长文件
    QHash<QString,Dictionary> commonDictionary;
    //监控xlsx文件导出是否完成
    QFutureWatcher<int>* watcherXlsxSaveStatus_;
    //导入Excel分析进度
    QFutureWatcher<int>* watcherXlsxImportStatus_;
    QString fromExcelImportFilePath="";
    //导入excel过程中遇到的致命错误
    QStringList importExcelErrorDetail;
    //导入excel行数
    int importExcelRow=0;
    //从Exccel中导入的数据记录
    QList<QByteArray> ofdFileContentQByteArrayListFromExcel;
    //创建成员xlsx变量//xlsx文件数据存放到堆内存,注意使用完毕释放
    QXlsx::Document *xlsx=new QXlsx::Document();
    //xlsx文件保存文件名
    QString xlsxSaveName;
    //导出excel结束后是否打开excel
    bool openXlsx=false;
    //全局blocked消息,当主动block时,填充此消息,用于告知其他场景程序正在做什么
    QString dataBlockedMessage;
    //OFD文件专用编码
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
    //编码识别
    LibUcd m_libucd;
    //允许解析的编码白名单
    QStringList allowCharsetList={"GBK","GB2312","GB18030","ISO-8859-1","UTF-8","UTF-16","UTF-16BE","UTF-16LE","BIG5","EUC-JP","EUC-KR","X-EUC-TW","SHIFT_JIS"};
    //编码识别尝试识别的分隔符-如考虑新增分隔符则写到这里
    //QChar(1)=SOH=标题开始
    //QChar(31)=1FH=单元分隔符
    QList<QString> autoFlagList={"|",",","\t",";","#",QChar(1),QChar(31)};
    //尝试自动检测标题时，中英文字符占比（我们姑且认为如果第一行数据中英文占比大于此值，识别第一行数据行为标题）
    float titlecheck=0.7;
    //当第一行中英文占比低于上述设定值时，通过校验第一行和第2到11行数据中英文占比平均值的差值，如果差异大于下属设定值，则依然识别第一行数据行为标题
    float titleandcontextcheck=0.2;
    //第一个非法数值的行号
    int firstNotNumberLine=0;
    //统计index,当用户选中一列或者数值区域进行数值统计时对此变量自增并记录
    //如果统计途中发现此变量变了，则代表有新的统计任务，此任务立即return废弃
    int calculateIndex=0;
    //类OFD索引和数据文件名规则
    QHash<QString, QString>  likeOFDDataFilename;
    QHash<QString, QString>  likeOFDIndexFilename;
    QString likeOFDFileType;
    //表格的右键菜单
    QMenu *tablePopMenu;
    QAction *action_ShowDetails;
    QAction *action_ShowCharacter;
    QAction *action_ShowCopyColum;
    QAction *action_ShowCopyColumWithTitle;
    QAction *action_Magnify;
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
    QAction *action_CsvForceNumber;
    QAction *action_ShareUseQrCode;

    //详情增加可复制功能
    QMenu *showMessagePopMenu;
    QAction *action_ShowCopy;
    QAction *action_GoToFirstNotNumberLine;
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

    //表格选择模式
    QAbstractItemView::SelectionBehavior tableSelectionBehavior=QAbstractItemView::SelectItems;

    //阻断的操作，比如当前正在导出文件，正在搜索，和isUpdatedata类似
    bool dataBlocked=false;
    //是否是在导出文件
    bool isExportFile=false;
    //已经加载的行,用于懒加载是判断哪些行已经加载,避免重复加载
    //优化，使用QHash提高大文件下多行数据加载后的查找效率
    QHash <int,bool> rowHasloaded;
    //
    QHash <QString,QString> specialCharacter;
    //PUA码提示，或者一字双码提示
    QHash <uint,QString> puaOrDeletedGBKCharacter;
    //优化器，记录每列表格最宽记录，当异步下次加载数据时，根据是否发生了变化来决定是否需要更新列宽度
    QHash <int,int> columnWidth;

    //加入到比对器的数据 <行号,数据>
    QMap<int,QStringList> compareData;

    //字段Tips
    QMap<QString,QMap<QString,QString>> commonFieldTips;

    //文件变化标志，如果打开了一个文件并且编辑修改了，则此标志为真
    bool fileChanged=false;

    //设置选项///////////////////////
    //字段必填检查
    bool checkFieldFlag=true;
    //数据内存压缩等级--默认禁用
    int dataCompressLevel=0;
    //设置选项
    //默认视图模式-0当拖进来文件时在当前窗口打开，1不同的文件在新窗口打开，2所有拖到窗口的文件在新窗口打开
    int defaultViewMode=0;
    //设置选项
    //新文件打开方式
    int defaultNewFileMode=0;
    //设置选项
    //每页行数类型 0-10万行,1-20万行,2-50万行,3-100万行
    int defaultPageSizeType=0;
    //设置选项///////////////////////

    //程序所有的tips，用于程序启动后随机显示一条提示
    QList<QString> tips;

    //每页行数-默认10万
    int pageRowSize=100000;
    //总页数
    int pageCount=1;
    //目前在第几页
    int currentPage=1;
    //支持导出excel文件的最大行数
    //导出超大excel将占用很大的内存，这里初步限制60万行，需要再高的需要定制该功能
    //1.8.6开始支持导出100万行
    int maxExcelRow=1000000;

    //换行符长度,正确的来说OFD的换行符都是\r\n两个字节
    int newLineCharsetLength=2;

    //忽略正在进行的任务强制退出
    //如果程序正在进行诸如文件读取,文件导出,搜索任务时,程序强制退出时使用,主动告知进行中的任务进行退出函数，方式程序在后台驻留
    //只允许退出程序时使用此标志,遇到此标志一切进行中的耗时任务都会终止并退出
    bool abortExit=false;

    //统计读取文件耗时
    QDateTime time_Start = QDateTime::currentDateTime();

    bool checkCSVVersion(CsvFileDefinition  csv,QString versionRowData);
    void copyToClipboard(bool withTitle=false);

    void tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void statusBar_clear_statusBar();
    void statusBar_display_rowsCount(int rowsCount);
    void statusBar_display_rowsCount(int rowsCount,QString charset);
    void statusBar_display_rowsAndCol(int row,int col,int length);
    void statusBar_disPlayMessage(QString text);
    void clear_Table_Info();
    void clear_Table_Contents();
    void clear_Display_Info();
    void clear_OldData( bool keepdbfDisplayType=false, bool keepdbfTrimType=false);
    void initFile(QString filePath, bool keepdbfDisplayType=false, bool keepdbfTrimType=false);
    void initStatusBar();
    void open_file_Dialog();

    void loadConfig(bool isReload=false);

    //设置和插件加载
    void load_Setting();
    void load_PluginList();

    //数据load
    void load_ofdIndexFile();
    void load_ofdDataFile(QString fileTypeFromFileName);
    void load_csvFile(QList<matchIndex> csvMatchList);
    void load_fixedFile(QList<matchIndex> fixedMatchList);
    void load_fixedFileData();
    void load_csvFileData(QStringList title);
    void load_dbfFile();

    //数据存储
    void save2Csv(QString filename,int pageNum,int splitBy, bool useUTF8=false);
    void save2Html(QString filename,int pageNum, bool useUTF8=false);
    void save2Xlsx(QString filename,int pageNum);
    int save2XlxsFile();

    //表格初始化
    void init_display_OFDIndexTable();
    void init_OFDTable();
    void init_DBFTable();
    void init_CSVTable(QStringList title);
    void init_FIXEDTable();

    //数据渲染
    void display_OFDTable(bool clearDirtyData=false);
    void display_CSVTable();
    void display_FIXEDTable();
    void display_DBFTable();
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
    QString StringToHexStringWithEnCode(QString data,QTextCodec *codec,bool withSouceChar=true);
    QString HexStringToBinaryString(QString HexString);
    static bool compareOFDData(const OFDFileDefinition &ofd1, const OFDFileDefinition &ofd2);
    static bool compareDBFData(const dbfMatchInfo &dbf1, const dbfMatchInfo &dbf2);
    bool dbfColIsNumber(int col);
    void reCalculateTableBeginAndEnd();
    void checkRowFieldResult (QStringList & rowdata,QMap<int,QString> & checkresult,int splitFlag=0);
    void checkRowFieldSearch (int direction);

};

#endif // MAINWINDOW_H
