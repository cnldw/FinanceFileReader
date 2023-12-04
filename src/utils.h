/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QLocale>
#include <QDateTime>
#include <QApplication>
#include <QTime>
#include <QTextCodec>
#include <QSettings>

#include <QtCore/qmath.h>
#include "src/ofdfiledefinition.h"
#include "src/csvfiledefinition.h"
#include "src/fixedfiledefinition.h"
#include "src/qdbf/qdbftable.h"
#include "src/qdbf/qdbfrecord.h"
#include "src/dbffileconfig.h"
#include "src/dbffiledefinition.h"
#include "src/ofdcodeinfo.h"
#include "src/dictionary.h"
#include "src/configfile.h"
#include "src/publicdefine.h"

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
     static QString qStringTrimRight(const QString& str);
     static QString qStringTrimLeft(const QString& str);
     static QString getConfigPath();
     static QString getCompileDate();
     static void load_OFDCodeInfo(QHash<QString, OFDCodeInfo> &loadedOfdCodeInfo);
     static void load_OFDDictionary(Dictionary &ofdDictionary);
     static void load_OFDTipDictionary(QMap<QString,QMap<QString,QString>> &fieldTips);
     static void load_OFDLikeFileDefinition(QHash<QString, QString>  &likeOFDIndexFilename,QHash<QString, QString>  &likeOFDDataFilename);
     static void load_OFDIndexFileDefinition(QHash<QString, QString> &loadedOfdIndexFileInfo);
     static void  load_OFDDefinition(QList<ConfigFile<OFDFileDefinition>> &ofdConfigList,QHash<QString,int> &ofdQuickMatchIndex);
     static void  load_CSVDefinition(QList<ConfigFile<CsvFileDefinition>> &csvConfigList,QHash<QString,Dictionary> &commonDictionary,QMap<QString,QMap<QString,QString>> &commonFieldTips);
     static void  load_FIXEDDefinition(QList<ConfigFile<FIXEDFileDefinition>> &fixedConfigList,QHash<QString,Dictionary> &commonDictionary,QMap<QString,QMap<QString,QString>> &commonFieldTips);
     static void  load_DBFDefinition(QList<DbfFileConfig> &dbfConfigList,QHash<QString,Dictionary> &commonDictionary,QMap<QString,QMap<QString,QString>> &commonFieldTips);

     static QString getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int dataCompressLevel,int row ,int col);
     static QStringList getFormatRowValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int dataCompressLevel,int row);
     static QString getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int dataCompressLevel,int row ,int col);
     static QString getFormatValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int dataCompressLevel,int row ,int col);
     static QStringList getFormatRowValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int dataCompressLevel,int row);
     static QStringList getRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> * csvFileContentQByteArrayList,CsvFileDefinition * csv,int dataCompressLevel,int row);
     static QStringList getFormatRowValuesFromdbfTableFile(QDbf::QDbfTable * dbftablefile,DbfFileDefinition * dbf,int row,QHash<int,int> * rowMap,bool adddeletedFlag=false,int trimType=0);
     static QString getFormatValuesFromdbfTableFile(QDbf::QDbfTable * dbftablefile,DbfFileDefinition * dbf,int row,int col,QHash<int,int> * rowMap,int trimType=0);
     static QStringList getOriginalRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> * csvFileContentQByteArrayList,CsvFileDefinition * csv,int dataCompressLevel,int row);
     static QString CovertInt2ExcelCol(int number);
     static QString CovertDoubleQStringWithThousandSplit(QString doubleString);
     static QString clearQuotes(QString stringS);
     static void UpdateFileTime(QString file,QDateTime lastModifyTime=QDateTime::currentDateTime());
     static void getFileListFromDir(QString dirpath,QStringList *filelist);
     static void  sleep(unsigned int msec);
     static void getFileListFromDirSkipOkfile(QString dirpath,QStringList *filelist);
     static fieldcheckitem parseStringtofieldcheckitem(QString text,int countInt,bool * okFlag);
     static QList<QStringList> parseCheckItemListtoChineseList(OFDFileDefinition &ofd);
     static QList<QStringList> parseCheckItemListtoChineseList(CsvFileDefinition &csv);
     static QList<QStringList> parseCheckItemListtoChineseList(FIXEDFileDefinition &fixed);
     static double CVCcal (QList<QStringList > data);
     static void setDefaultWindowFonts(QWidget *w);

};

#endif // UTILS_H
