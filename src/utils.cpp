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
bool Utils::isDirExist(QString fullPath)
{
    if(QDir(fullPath).exists())
    {
        return true;
    }
    return false;
}

bool Utils::isFileExist(QString fullFileName)
{
    if(QFileInfo(fullFileName).isFile())
    {
        return true;
    }
    return false;
}

QString Utils::getVersion()
{
    return "V1.5.6";
}

QString Utils::getCompileDate(){
    return "2018-07-15";
}

QString Utils::getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col)
{
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getfieldCount()){
        return "";
    }
    //字段类型
    QString fileType=ofd->getfieldList().at(col).getFiledType();
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd->getfieldList().at(col).getLength();
    //小数长度
    int filedDeclength=ofd->getfieldList().at(col).getDecLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(ofdFileContentQByteArrayList->at(row).mid(ofd->getfieldList().at(col).getRowBeginIndex(),filedlength));
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
        int needCheck=filedlength-filedDeclength-1;
        int needCutZero=0;
        for(int s=0;s<needCheck;s++){
            if(filed.at(s)=='0'){
                needCutZero++;
            }
            else{
                break;
            }
        }
        //获取整数
        QString left=filed.left(filedlength-filedDeclength).remove(0,needCutZero);
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

QString Utils::getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col)
{
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getfieldCount()){
        return "";
    }
    //开始获取此字段的值
    QString filed="";
    //字段长度
    int filedlength=ofd->getfieldList().at(col).getLength();
    //获取此字段的值
    filed=QString::fromLocal8Bit(ofdFileContentQByteArrayList->at(row).mid(ofd->getfieldList().at(col).getRowBeginIndex(),filedlength));
    return filed;
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


