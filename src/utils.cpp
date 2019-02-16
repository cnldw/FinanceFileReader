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
#include "src/utils.h"

Utils::Utils()
{

}

/**
 * @brief Utils::isDirExist 判断目录是否存在
 * @param fullPath
 * @return
 */
bool Utils::isDirExist(QString fullPath)
{
    if(QDir(fullPath).exists())
    {
        return true;
    }
    return false;
}

/**
 * @brief Utils::isFileExist 判断文件是否存在
 * @param fullFileName
 * @return
 */
bool Utils::isFileExist(QString fullFileName)
{
    if(QFileInfo(fullFileName).isFile())
    {
        return true;
    }
    return false;
}

/**
 * @brief Utils::getCompileDate 使用c++内置宏自动获取程序编译日期，这个宏可能在msvc上无法使用，未验证
 * @return
 */
QString Utils::getCompileDate(){
    return QLocale(QLocale::English).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy").toString("yyyy-MM-dd");
}

QString Utils::getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col)
{
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getFieldCount()){
        return "";
    }
    //本行数据
    QByteArray rowdata=qUncompress(ofdFileContentQByteArrayList->at(row));
    //字段类型
    QString fileType=ofd->getFieldList().at(col).getFieldType();
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd->getFieldList().at(col).getLength();
    //小数长度
    int filedDeclength=ofd->getFieldList().at(col).getDecLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(rowdata.mid(ofd->getFieldList().at(col).getRowBeginIndex(),filedlength));
    //数据信息处理
    if(fileType=="C"){
        //C类型从右去除多余空格
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    else if(fileType=="A"){
        //A类型从右移除多余空格,移除空格后剩余的应该是0-9的数值
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    else if(fileType=="N"){
        //如果字段为全部是空格，则认为字段为空
        if(filed.trimmed().isEmpty()){
            filed="";
        }
        else{
            //N类型为双精度数字
            //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
            int needCheck=filedlength-filedDeclength-1;
            int needCutZero=0;
            int needCutZeroBegin=0;
            //判断负数标志位，如果是负数则从第二位开始切除不必要的0
            if(filed.at(0)=='-'){
                needCutZeroBegin=1;
            }
            //循环切除0
            for(int s=needCutZeroBegin;s<needCheck;s++){
                if(filed.at(s)=='0'){
                    needCutZero++;
                }
                else{
                    break;
                }
            }
            //获取整数
            QString left=filed.left(filedlength-filedDeclength).remove(needCutZeroBegin,needCutZero);
            //获取小数--如果小数长度为0,就不必处理小数了
            if(filedDeclength==0){
                filed=left;
            }else{
                //获取小数
                QString right=filed.right(filedDeclength);
                //拼接整数部分和小数部分
                filed=left.append(".").append(right);
            }
        }
    }
    else if(fileType=="TEXT"){
        //TEXT类型从右去除多余空格
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            //遇到第一个不是空格的字段就立即退出循环
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    return filed;
}

QStringList Utils::getFormatRowValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row){
    QStringList rowList;
    //判断越界
    if(row<ofdFileContentQByteArrayList->count()){
        //获取本行数据
        QByteArray rowdata=qUncompress(ofdFileContentQByteArrayList->at(row));
        //遍历获取各个字段
        int colCount=ofd->getFieldCount();
        for(int col=0;col<colCount;col++){
            //字段类型
            QString fileType=ofd->getFieldList().at(col).getFieldType();
            //开始获取此字段的值
            QString filed="";
            //字段长度
            int filedlength=ofd->getFieldList().at(col).getLength();
            //小数长度
            int filedDeclength=ofd->getFieldList().at(col).getDecLength();
            //获取此字段的值
            if(rowdata.count()>=ofd->getFieldList().at(col).getRowBeginIndex()+filedlength){
                //对字节mid后返回字符串
                filed=QString::fromLocal8Bit(rowdata.mid(ofd->getFieldList().at(col).getRowBeginIndex(),filedlength));
            }
            //越界表示本行后续不再有内容，终止
            else{
                break;
            }
            //数据信息处理
            if(fileType=="C"){
                //C类型从右去除多余空格
                int spaceLength=0;
                for(int i=filed.length()-1;i>=0;i--){
                    if(filed.at(i)==' '){
                        spaceLength++;
                    }
                    else{
                        break;
                    }
                }
                if(spaceLength>0){
                    filed=filed.left(filed.length()-spaceLength);
                }
            }
            else if(fileType=="A"){
                //A类型从右移除多余空格,移除空格后剩余的应该是0-9的数值
                int spaceLength=0;
                for(int i=filed.length()-1;i>=0;i--){
                    if(filed.at(i)==' '){
                        spaceLength++;
                    }
                    else{
                        break;
                    }
                }
                if(spaceLength>0){
                    filed=filed.left(filed.length()-spaceLength);
                }
            }
            else if(fileType=="N"){
                //N类型为双精度数字
                //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                //如果字段为全部是空格，则认为字段为空
                if(filed.trimmed().isEmpty()){
                    filed="";
                }
                else{
                    //N类型为双精度数字
                    //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                    int needCheck=filedlength-filedDeclength-1;
                    int needCutZero=0;
                    int needCutZeroBegin=0;
                    //判断负数标志位，如果是负数则从第二位开始切除不必要的0
                    if(filed.at(0)=='-'){
                        needCutZeroBegin=1;
                    }
                    //循环切除0
                    for(int s=needCutZeroBegin;s<needCheck;s++){
                        if(filed.at(s)=='0'){
                            needCutZero++;
                        }
                        else{
                            break;
                        }
                    }
                    //获取整数
                    QString left=filed.left(filedlength-filedDeclength).remove(needCutZeroBegin,needCutZero);
                    //获取小数--如果小数长度为0,就不必处理小数了
                    if(filedDeclength==0){
                        filed=left;
                    }else{
                        //获取小数
                        QString right=filed.right(filedDeclength);
                        //拼接整数部分和小数部分
                        filed=left.append(".").append(right);
                    }
                }
            }
            else if(fileType=="TEXT"){
                //TEXT类型从右去除多余空格
                int spaceLength=0;
                for(int i=filed.length()-1;i>=0;i--){
                    if(filed.at(i)==' '){
                        spaceLength++;
                    }
                    //遇到第一个不是空格的字段就立即退出循环
                    else{
                        break;
                    }
                }
                if(spaceLength>0){
                    filed=filed.left(filed.length()-spaceLength);
                }
            }
            //其他类型直接原地返回
            rowList.append(filed);
        }
    }
    return rowList;
}

QString Utils::getFormatValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int row ,int col)
{
    //判断越界
    if(row>=fixedContentQByteArrayList->count()||col>=fixed->getFieldCountMax()){
        return "";
    }
    //字段类型
    QString fileType=fixed->getFieldList().at(col).getFieldType();
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=fixed->getFieldList().at(col).getLength();
    //小数长度
    int filedDeclength=fixed->getFieldList().at(col).getDecLength();
    //获取本行记录
    QByteArray rowdata=qUncompress(fixedContentQByteArrayList->at(row));
    //获取此字段的值
    //字符定长和字节定长判断
    if(fixed->getFieldlengthtype()=="0"){
        //字节截取
        //定长文件兼容多种行长度，判断越界
        if(rowdata.count()>=fixed->getFieldList().at(col).getRowBeginIndex()+filedlength){
            //对字节mid后返回字符串
            filed=QString::fromLocal8Bit(rowdata.mid(fixed->getFieldList().at(col).getRowBeginIndex(),filedlength));
        }
        else{
            return "";
        }
    }
    //字符截取
    else{
        //判断越界
        if(QString::fromLocal8Bit(qUncompress(fixedContentQByteArrayList->at(row))).length()>=(fixed->getFieldList().at(col).getRowBeginIndex()+filedlength)){
            //转成字符串后mid
            filed=QString::fromLocal8Bit(rowdata).mid(fixed->getFieldList().at(col).getRowBeginIndex(),filedlength);
        }
        else{
            return "";
        }
    }
    //数据信息处理
    if(fileType=="C"){
        //C类型从右去除多余空格
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    else if(fileType=="A"){
        //A类型从右移除多余空格,移除空格后剩余的应该是0-9的数值
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    else if(fileType=="N"){
        //N类型为双精度数字
        //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
        //如果字段为全部是空格，则认为字段为空
        if(filed.trimmed().isEmpty()){
            filed="";
        }
        else{
            //N类型为双精度数字
            //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
            int needCheck=filedlength-filedDeclength-1;
            int needCutZero=0;
            int needCutZeroBegin=0;
            //判断负数标志位，如果是负数则从第二位开始切除不必要的0
            if(filed.at(0)=='-'){
                needCutZeroBegin=1;
            }
            //循环切除0
            for(int s=needCutZeroBegin;s<needCheck;s++){
                if(filed.at(s)=='0'){
                    needCutZero++;
                }
                else{
                    break;
                }
            }
            //获取整数
            QString left=filed.left(filedlength-filedDeclength).remove(needCutZeroBegin,needCutZero);
            //获取小数--如果小数长度为0,就不必处理小数了
            if(filedDeclength==0){
                filed=left;
            }else{
                //获取小数
                QString right=filed.right(filedDeclength);
                //拼接整数部分和小数部分
                filed=left.append(".").append(right);
            }
        }
    }
    else if(fileType=="TEXT"){
        //TEXT类型从右去除多余空格
        int spaceLength=0;
        for(int i=filed.length()-1;i>=0;i--){
            if(filed.at(i)==' '){
                spaceLength++;
            }
            //遇到第一个不是空格的字段就立即退出循环
            else{
                break;
            }
        }
        if(spaceLength>0){
            filed=filed.left(filed.length()-spaceLength);
        }
    }
    //其他类型直接原地返回
    return filed;
}

QStringList Utils::getFormatRowValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int row){
    QStringList rowList;
    //判断越界
    if(row<fixedContentQByteArrayList->count()){
        //获取本行数据
        QByteArray rowdata=qUncompress(fixedContentQByteArrayList->at(row));
        //遍历获取各个字段
        int colCount=fixed->getFieldCountMax();
        for(int col=0;col<colCount;col++){
            //字段类型
            QString fileType=fixed->getFieldList().at(col).getFieldType();
            //开始获取此字段的值
            QString filed="";
            //字段长度
            int filedlength=fixed->getFieldList().at(col).getLength();
            //小数长度
            int filedDeclength=fixed->getFieldList().at(col).getDecLength();
            //获取此字段的值
            //字符定长和字节定长判断
            if(fixed->getFieldlengthtype()=="0"){
                //字节截取
                //定长文件兼容多种行长度，判断越界
                if(rowdata.count()>=fixed->getFieldList().at(col).getRowBeginIndex()+filedlength){
                    //对字节mid后返回字符串
                    filed=QString::fromLocal8Bit(rowdata.mid(fixed->getFieldList().at(col).getRowBeginIndex(),filedlength));
                }
                //越界表示本行后续不再有内容，终止
                else{
                    break;
                }
            }
            //字符截取
            else{
                //判断越界
                if(QString::fromLocal8Bit(qUncompress(fixedContentQByteArrayList->at(row))).length()>=(fixed->getFieldList().at(col).getRowBeginIndex()+filedlength)){
                    //转成字符串后mid
                    filed=QString::fromLocal8Bit(rowdata).mid(fixed->getFieldList().at(col).getRowBeginIndex(),filedlength);
                }
                //越界表示本行后续不再有内容，终止
                else{
                    break;
                }
            }
            //数据信息处理
            if(fileType=="C"){
                //C类型从右去除多余空格
                int spaceLength=0;
                for(int i=filed.length()-1;i>=0;i--){
                    if(filed.at(i)==' '){
                        spaceLength++;
                    }
                    else{
                        break;
                    }
                }
                if(spaceLength>0){
                    filed=filed.left(filed.length()-spaceLength);
                }
            }
            else if(fileType=="A"){
                //A类型从右移除多余空格,移除空格后剩余的应该是0-9的数值
                int spaceLength=0;
                for(int i=filed.length()-1;i>=0;i--){
                    if(filed.at(i)==' '){
                        spaceLength++;
                    }
                    else{
                        break;
                    }
                }
                if(spaceLength>0){
                    filed=filed.left(filed.length()-spaceLength);
                }
            }
            else if(fileType=="N"){
                //N类型为双精度数字
                //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                //如果字段为全部是空格，则认为字段为空
                if(filed.trimmed().isEmpty()){
                    filed="";
                }
                else{
                    //N类型为双精度数字
                    //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                    int needCheck=filedlength-filedDeclength-1;
                    int needCutZero=0;
                    int needCutZeroBegin=0;
                    //判断负数标志位，如果是负数则从第二位开始切除不必要的0
                    if(filed.at(0)=='-'){
                        needCutZeroBegin=1;
                    }
                    //循环切除0
                    for(int s=needCutZeroBegin;s<needCheck;s++){
                        if(filed.at(s)=='0'){
                            needCutZero++;
                        }
                        else{
                            break;
                        }
                    }
                    //获取整数
                    QString left=filed.left(filedlength-filedDeclength).remove(needCutZeroBegin,needCutZero);
                    //获取小数--如果小数长度为0,就不必处理小数了
                    if(filedDeclength==0){
                        filed=left;
                    }else{
                        //获取小数
                        QString right=filed.right(filedDeclength);
                        //拼接整数部分和小数部分
                        filed=left.append(".").append(right);
                    }
                }
            }
            else if(fileType=="TEXT"){
                //TEXT类型从右去除多余空格
                int spaceLength=0;
                for(int i=filed.length()-1;i>=0;i--){
                    if(filed.at(i)==' '){
                        spaceLength++;
                    }
                    //遇到第一个不是空格的字段就立即退出循环
                    else{
                        break;
                    }
                }
                if(spaceLength>0){
                    filed=filed.left(filed.length()-spaceLength);
                }
            }
            //其他类型直接原地返回
            rowList.append(filed);
        }
    }
    return rowList;
}


QString Utils::getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col)
{
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getFieldCount()){
        return "";
    }
    //本行数据
    QByteArray rowdata=qUncompress(ofdFileContentQByteArrayList->at(row));
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd->getFieldList().at(col).getLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(rowdata.mid(ofd->getFieldList().at(col).getRowBeginIndex(),filedlength));
    return filed;
}

QStringList Utils::getRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> *  csvFileContentQByteArrayList,CsvFileDefinition * csv,int row){
    //判断越界
    QStringList rowData;
    if(row>=csvFileContentQByteArrayList->count()){
        return rowData;
    }else{
        return QString::fromLocal8Bit(qUncompress(csvFileContentQByteArrayList->at(row))).split(csv->getSplit());
    }
}

//转换数字列为excel列
QString Utils::CovertInt2ExcelCol(int col){
    QString tmp = "";
    int tmpInt = col % 26;
    tmp.append((QChar)(tmpInt + 65));
    col = col / 26;
    while (col > 0)
    {
        tmpInt = col % 26;
        col = col / 26;
        if (tmpInt != 0)
        {
            tmp.insert(0, (QChar)(tmpInt + 64));
        }
        else
        {
            if(col>0)
            {
                tmp.insert(0, 'Z');
                col--;
            }
        }
    }
    return tmp;
}

QString Utils::CovertDoubleQStringWithThousandSplit(QString doubleString){
    int strLength=doubleString.length();
    int BeginIndex=strLength-1;
    if(doubleString.contains(".")){
        BeginIndex=doubleString.indexOf(".")-1;
    }
    int z=1;
    for(int index=BeginIndex;index>0;index--){
        if(z%3==0){
            doubleString.insert(index,QChar(','));
        }
        z++;
    }
    return doubleString;
}
