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
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QLocale>
#include <QDateTime>
#include <QApplication>
#include <QTextCodec>
#include "src/ofdfiledefinition.h"
#include "src/csvfiledefinition.h"
#include "src/fixedfiledefinition.h"
#include "time.h"
#ifdef Q_OS_WIN32
#include "sys/utime.h"
#else
#include "utime.h"
#endif
namespace Ui {
class Utils;
}

class Utils
{
public:
    Utils();
     static bool isDirExist(QString fullPath);
     static bool isFileExist(QString fullFileName);
     static QString getConfigPath();
     static QString getCompileDate();
     static QString getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col);
     static QStringList getFormatRowValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row);
     static QString getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col);
     static QString getFormatValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int row ,int col,QString charset);
     static QStringList getFormatRowValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int row,QString charset);
     static QStringList getRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> * csvFileContentQByteArrayList,CsvFileDefinition * csv,int row,QString charset);
     static QString CovertInt2ExcelCol(int number);
     static QString CovertDoubleQStringWithThousandSplit(QString doubleString);
     static void UpdateFileTime(QString file,QDateTime lastModifyTime=QDateTime::currentDateTime());
     static void getFileListFromDir(QString dirpath,QStringList *filelist);
};

#endif // UTILS_H
