#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include<QString>
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
#include<QDateTime>
#include<utils.h>
#include<ofdfiledefinition.h>
#include <fielddefinition.h>
#include<codeinfo.h>
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
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_fileOpen_triggered();

    void on_aboutOpen_triggered();

    void on_actionAboutQt_triggered();

    void on_pushButtonOpenFile_clicked();

    void on_pushButtonOpenFile_2_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *statusLabelTwo;
    QLabel *statusLabelThree;
    QLabel *statusLabelFour;
    QLabel *statusLabelFive;
    QTableWidget *table;
    QString filePath;
    QMap<QString, CodeInfo> codeInfo;
    QMap<QString, QString> indexFileInfo;
    QMap<QString, QString> ofdFileInfo;
    QMap<QString,OFDFileDefinition>ofdDefinitionMap;
    QList<QStringList> fileDataList;
    QMap<QString,QString> fileHeaderMap;

    /**
     * @brief clear_statusBar
     */
    void clear_statusBar();
    /**
     * @brief display_rowsCount
     * @param rowsCount
     */
    void display_rowsCount(int rowsCount);
    /**
     * @brief display_fileName
     * @param filePath
     */
    void display_fileName(QString filePath);
    void initFile(QString filePath);
    /**
     * @brief open_file_Dialog
     */
    void open_file_Dialog();

    void clear_Display();

    void statusBar_disPlay(QString text);

    void load_CodeInfo();

    void load_FileType();

    void load_OFDDefinition();

    void load_indexFile(QString filePath);

    void load_ofdFile(QString sendCode,QString fileType,QString filePath);

    void displayIndexTable(QList<int> colwidth,QList <QStringList> data);

    void clearTable();
};

#endif // MAINWINDOW_H
