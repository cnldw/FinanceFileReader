/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGCHOOSEFILEFROMZIP_H
#define DIALOGCHOOSEFILEFROMZIP_H

#include <QDialog>
#include <QTableWidget>
#include <private/qzipreader_p.h>

namespace Ui {
class DialogChooseFileFromZip;
}

class DialogChooseFileFromZip : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseFileFromZip(QString zipPathPar,QWidget *parent = nullptr);
    ~DialogChooseFileFromZip();
    int getChooeseIndex() const;
    bool getConfirm() const;

    bool getExtractAll() const;

    const QList<QZipReader::FileInfo> &getZipFileInfoList() const;

    const QMap<QString, QDateTime> &getZipFileListMdTime() const;

    QList<QString> getZipFileNameSystemCodecList() const;

    QString getZipFileUseCodec() const;

private slots:
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void on_pushButton_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DialogChooseFileFromZip *ui;
    QString messageText="在压缩包所在目录自动创建文件夹解压你选择的文件然后开始解析";
    QString messageTextextractAll="在压缩包所在目录自动创建文件夹解压列出的所有文件然后开始解析你选择的文件";
    int chooeseIndex=0;
    bool  confirm=false;
    bool extractAll=false;
    QString zipFileUseCodec="System";
    qint64 fileSizeSum=0;
    QString zipPath="";
    QList<QZipReader::FileInfo> zipFileInfoList;
    QList<QString> zipFileNameSystemCodecList;
    QMap<QString,QDateTime> zipFileListMdTime;
    QTableWidget * ptr_table;
    void initZipTable();
};

#endif // DIALOGCHOOSEFILEFROMZIP_H
