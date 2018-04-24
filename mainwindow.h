#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include <QMap>
#include <QStringList>
#include <utils.h>
#include <ofdfiledefinition.h>
#include <fielddefinition.h>
#include <dialogshowtablerow.h>
#include <dialogshowtablefiledcheck.h>
#include "dialogshowtablecompareview.h"
#include <dictionary.h>
#include <codeinfo.h>
#include <QByteArray>
#include <QTextCodec>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QClipboard>
#include <QColor>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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

    void on_pushButtonNextSearch_2_clicked();

    void on_actionsOpenCompare_triggered();

    void on_actionClearCompare_triggered();

private:
    Ui::MainWindow *ui;
    //状态栏指针变量
    QLabel *statusLabel_ptr_showCount;
    QLabel *statusLabel_ptr_showFileName;
    QLabel *statusLabel_ptr_showRowAndCol;
    QLabel *statusLabel_ptr_showMessage;
    QTableWidget *ptr_table;
    QString currentOpenFilePath;
    //已经加载的code信息,记录销售商和TA的代码信息
    QMap<QString, CodeInfo> loadedCodeInfo;
    //已经加载的索引文件信息,记录各种索引文件的文件名开头三个字符
    QMap<QString, QString> loadedIndexFileInfo;
    //已经加载的OFD文件的文件名结尾两个字符和文件名的对应关系,比如04,交易类确认
    QMap<QString, QString> loadedOfdFileInfo;
    //已经加载的各种OFD文件的定义,比如400_21_01,代表V400,第21版本的01文件的定义
    QMap<QString,OFDFileDefinition>loadedOfdDefinitionMap;
    //用来记录文件头部内容的map,此信息用于文件检查
    QMap<QString,QString> indexFileHeaderMap;
    //用来记录文件标题和内容的list,解析索引类文件时使用
    QList<QStringList> indexFileDataList;
    //当前正在使用的ofd定义,打开哪个文件,就切换到改文件的ofd定义
    OFDFileDefinition ofd;
    //OFD文件头使用Qstring记录,作为原始记录,方便后续保存文件时直接提取文件头
    QList<QString> ofdFileHeaderQStringList;
    //OFD文件体,因为包含中英文,且要以GB18030方式记录文件内容,所以使用QByteArray
    QList<QByteArray> ofdFileContentQByteArrayList;
    //当前打开的文件类别,目前已支持的文件类型0索引,1OFD数据(销售商和TA交互版本),2OFD数据(TA和管理人版本,暂未开发)
    int currentOpenFileType=0;

    //字典参数
    Dictionary dictionary;

    //表格的右键菜单
    QMenu *tablePopMenu;
    QAction *action_ShowDetails;
    QAction *action_ShowCopyColum;
    QAction *action_ShowAnalysis;
    QAction *action_EditCompareData;

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
    //数据更新状态
    bool isUpdateData=false;

    //已经加载的行,用于懒加载是判断哪些行已经加载,避免重复加载
    QList<int> rowHasloaded;

    //加入到比对器的数据
    QMap<int,QStringList> compareData;

    void statusBar_clear_statusBar();

    void statusBar_display_rowsCount(int rowsCount);

    void statusBar_display_rowsAndCol(int row,int col,int length);

    void statusBar_display_fileName();

    void statusBar_disPlayMessage(QString text);

    void clear_Table_Info();

    void clear_Display_Info();

    void clear_oldData();

    void initFile();

    void initStatusBar();

    void open_file_Dialog();

    void load_CodeInfo();

    void load_FileType();

    void load_Dictionary();

    void load_OFDDefinition();

    void load_indexFile();

    void load_ofdFile(QString sendCode,QString fileType);

    //初始化且显示索引文件数据
    //索引文件数据较小,不再启用懒加载
    void init_display_IndexTable();

    //获取指定行列的解析后的数据
    QString getFormatValuesFromofdFileContentQByteArrayList(int row ,int col);
    //获取指定行列的原始数据
    QString getOriginalValuesFromofdFileContentQByteArrayList(int row ,int col);

    //初始化OFD用的表格
    void init_OFDTable();
    //仅仅渲染显示当前指定区域
    //算法原理,当table试图发生滚动或者table控件大小发生变化时
    //探视当前屏幕显示的区间范围,从QTableWidgetItem池中获取已经不再显示的item复用，大大降低内存开销
    void display_OFDTable();
};

#endif // MAINWINDOW_H
