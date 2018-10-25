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
#include "src/utils.h"
#include "src/ofdfiledefinition.h"
#include "src/fielddefinition.h"
#include "src/dialogshowtablerow.h"
#include "src/dialogshowtablefiledcheck.h"
#include "src/dialogshowtablecompareview.h"
#include "src/dictionary.h"
#include "src/codeinfo.h"
#include "src/xlsx/xlsxdocument.h"
#include "src/dialogmodifycell.h"
#include "src/dialogaboutauthor.h"
#include "src/dialogaboutthis.h"
#include "src/dialogmergetip.h"
#include "src/csvfiledefinition.h"

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

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButtonPreSearch_clicked();

    void on_pushButtonNextSearch_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void editCompareData();

    void copyToClipboard();

    void showRowDetails();

    void showFiledAnalysis();

    void showModifyCell();

    void loadFileOnWindowisOpen();

    void showModifyCellBatch();

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

private:
    Ui::MainWindow *ui;
    //应用程序名字
    QString appName=tr("基金文件阅读器-")+Utils::getVersion();
    //状态栏指针变量
    QLabel *statusLabel_ptr_showCount;
    QLabel *statusLabel_ptr_showRowAndCol;
    QLabel *statusLabel_ptr_showMessage;
    QTableWidget *ptr_table;
    QString currentOpenFilePath;
    QString getConfigPath();
    QString startUpfile="";
    QTimer *loadFiletimer;
    //已经加载的code信息,记录销售商和TA的代码信息
    QHash<QString, CodeInfo> loadedCodeInfo;
    //已经加载的索引文件信息,记录各种索引文件的文件名开头三个字符
    QHash<QString, QString> loadedIndexFileInfo;
    //已经加载的OFD文件的文件名结尾两个字符和文件名的对应关系,比如04,交易类确认
    QHash<QString, QString> loadedOfdFileInfo;
    //csv
    QHash<QString, QString> loadedCsvFileInfo;
    //已经加载的定长文件类别信息
    QHash<QString, QString> loadedFixedFileInfo;
    //已经加载的各种OFD文件的定义,比如400_21_01,代表V400,第21版本的01文件的定义
    QHash<QString,OFDFileDefinition>loadedOfdDefinitionMap;
    //已经加载的CSV文件类别信息,csv文件的文件名可能相同但是却是不同版本，所以使用List遍历匹配
    QList<CsvFileDefinition> loadedCsvDefinitionList;
    //用来记录文件头部内容的map,此信息用于文件检查
    QHash<QString,QString> indexFileHeaderMap;
    //用来记录文件标题和内容的list,解析索引类文件时使用
    QList<QStringList> indexFileDataList;
    //当前正在使用的ofd定义,打开哪个文件,就切换到改文件的ofd定义
    OFDFileDefinition ofd;
    //当前打开的csv文件使用的csv定义，打开哪个文件,就切换到改文件的csv定义
    CsvFileDefinition csv;
    //OFD文件头使用Qstring记录,作为原始记录,方便后续保存文件时直接提取文件头
    QList<QString> ofdFileHeaderQStringList;
    //OFD文件体,因为包含中英文,且要以GB18030方式记录文件内容,所以使用QByteArray
    QList<QByteArray> ofdFileContentQByteArrayList;
    //打开的CSV文件的文件头
    QList<QString> csvFileHeaderQStringList;
    //打开的CSV文件的数据体
    QList<QString> csvFileContentQStringList;
    //当前打开的文件类别,目前已支持的文件类型0索引,1 OFD数据,2CSV文件,-1未打开文件
    int currentOpenFileType=-1;

    //字典参数
    Dictionary dictionary;

    //表格的右键菜单
    QMenu *tablePopMenu;
    QAction *action_ShowDetails;
    QAction *action_ShowCopyColum;
    QAction *action_ShowAnalysis;
    QAction *action_EditCompareData;
    QAction *action_ModifyCell;
    QAction *action_ModifyCellBatch;

    //鼠标指针位置,当鼠标点击单元格时,记录鼠标所点位置
    QPoint posCurrentMenu;

    /*
      极其重要的表格相关参数
    */
    //记录table在当前界面显示的行范围,用于判断需要刷新显示到界面的元素
    int hValueBegin = 0;
    int hValueEnd = 0;
    //列跳转模式当前搜索进度
    int colSearch=0;
    //记录当前选中的行和列
    int rowcurrent=0;
    int colcurrent=0;
    //表格当前高度
    int tableHeight;
    //表格行高度,常量
    const int rowHight=22;
    //配置文件加载的状态
    bool configLoadCompleted=false;
    //数据更新状态,比如当前正在加载文件，正在重新刷新文件
    bool isUpdateData=false;

    //阻断的操作，比如当前正在导出文件，正在搜索，和isUpdatedata类似
    bool dataBlocked=false;

    //已经加载的行,用于懒加载是判断哪些行已经加载,避免重复加载
    //优化，使用QHash提高大文件下多行数据加载后的查找效率
    QHash <int,bool> rowHasloaded;

    //加入到比对器的数据
    QMap<int,QStringList> compareData;

    //文件变化标志，如果打开了一个文件并且编辑修改了，则此标志为真
    bool fileChanged=false;

    /**
     * @brief statusBar_clear_statusBar
     * 状态栏信息清除方法
     */
    void statusBar_clear_statusBar();

    /**
     * @brief statusBar_display_rowsCount
     * @param rowsCount
     * 状态栏显示目前打开的文件总记录数的方法
     */
    void statusBar_display_rowsCount(int rowsCount);

    /**
     * @brief statusBar_display_rowsAndCol
     * @param row
     * @param col
     * @param length
     * 状态栏显示目前打开的文件鼠标所在的行和列的方法
     */
    void statusBar_display_rowsAndCol(int row,int col,int length);

    /**
     * @brief statusBar_disPlayMessage
     * @param text
     * 状态栏显示消息的方法
     */
    void statusBar_disPlayMessage(QString text);

    /**
     * @brief clear_Table_Info
     * 清理表格的方法如果你要新打开一个文件/一个新的类型的文件，请先调用此方法清理上次打开的文件的数据
     */
    void clear_Table_Info();

    /**
     * @brief clear_Display_Info
     * 清理文件标示栏目的方法，诸如文件的发送方接收方的信息等，如果你要打开一个新的文件，先调用此方法
     */
    void clear_Display_Info();

    /**
     * @brief clear_oldData
     * 刷新文件时调用，清理上次的数据
     */
    void clear_oldData();

    /**
     * @brief initFile
     * 文件初始化方法
     */
    void initFile();

    /**
     * @brief initStatusBar
     * 无需关注的一个方法，用于程序启动时初始化状态栏
     */
    void initStatusBar();

    /**
     * @brief open_file_Dialog
     * 弹出选择文件的选择框
     */
    void open_file_Dialog();

    /**
     * @brief load_CodeInfo
     *针对OFD文件，用于加载文件发送方和接受方的代码和名称信息，读取OFD_CodeInfo.ini
     */
    void load_CodeInfo();

    /**
     * @brief load_FileType
     * 加载文件类别标示
     */
    void load_FileType();

    /**
     * @brief load_Dictionary
     * 加载OFD文件的字典信息
     */
    void load_Dictionary();

    /**
     * @brief load_OFDDefinition
     * 加载所有的OFD文件的配置，各个版本的各个文件
     */
    void load_OFDDefinition();

    /**
     * @brief load_CSVDefinition
     * 加载所有csv类文件的定义
     */
    void load_CSVDefinition();

    /**
     * @brief load_indexFile
     * 索引文件的加载
     */
    void load_indexFile();

    /**
     * @brief load_ofdFile
     * @param sendCode
     * @param fileType
     * OFD文件的加载
     */
    void load_ofdFile(QString sendCode,QString fileType);

    /**
     * @brief load_csvFile
     * @param fileType
     * CSV文件的加载
     */
    void load_csvFile(QString fileType);

    void load_csvFileData(QStringList title);
    /**
     * @brief save2Csv
     * @param filename
     * 保存文件到csv
     */
    void save2Csv(QString filename);

    /**
     * @brief save2Html
     * @param filename
     * 保存文件到html
     */
    void save2Html(QString filename);

    /**
     * @brief save2Xlsx
     * @param filename
     * 保存文件到csv
     */
    void save2Xlsx(QString filename);

    //初始化且显示索引文件数据
    //索引文件数据较小,不再启用懒加载
    void init_display_IndexTable();

    //初始化OFD用的表格
    void init_OFDTable();
    void init_CSVTable(QStringList title);
    //仅仅渲染显示当前指定区域
    //算法原理,当table试图发生滚动或者table控件大小发生变化时
    //探视当前屏幕显示的区间范围,从QTableWidgetItem池中获取已经不再显示的item复用，大大降低内存开销
    void display_OFDTable();

    void display_CSVTable();
    /**
     * @brief saveOFDFile
     * @param filepath
     * OFD文件的保存方法
     */
    void saveOFDFile(QString filepath);

    /**
     * @brief randomTips
     * 启动程序时候的随机提醒信息
     */
    void randomTips();

};

#endif // MAINWINDOW_H
