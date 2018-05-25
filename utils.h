#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <ofdfiledefinition.h>
#include <fielddefinition.h>

namespace Ui {
class Utils;
}

class Utils
{
public:
    Utils();
     static bool isDirExist(QString fullPath);
     static bool isFileExist(QString fullFileName);
     static QString getVersion();
     static QString getCompileDate();
     /**
      * 获取指定行列的解析后的数据
      * 由于ofd文件原始记录可能非常大,需要使用指针
     */
     static QString getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col);
     /**
      * 获取指定行列的原始数据
      * 由于ofd文件原始记录可能非常大,需要使用指针
      */
     static QString getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col);
     static QString CovertInt2ExcelCol(int number);
};

#endif // UTILS_H
