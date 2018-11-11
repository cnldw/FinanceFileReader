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
#include "src/ofdfiledefinition.h"
#include "src/csvfiledefinition.h"

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
     /**
      * @brief getRowCsvValuesFromcsvFileContentQStringList
      * @param csvFileContentQStringList
      * @param csv
      * @param row
      * @return
      * csv文件的数据行分隔方法
      */
     static QStringList getRowCsvValuesFromcsvFileContentQStringList(QList<QString> * csvFileContentQStringList,CsvFileDefinition * csv,int row);
     static QString CovertInt2ExcelCol(int number);
     /**
      * @brief CovertDoubleQStringWithThousandSplit
      * @param doubleString
      * @return
      */
     static QString CovertDoubleQStringWithThousandSplit(QString doubleString);
};

#endif // UTILS_H
