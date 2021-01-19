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
 *
 * @brief MainWindow::getConfigPath 针对不通平台设计获取配置目录的方法,windows和linux返回程序当前目录下的config目录,macOS返回程序包的Resources目录
 * @return
 */
QString Utils::getConfigPath(){
    //macOS 取程序包.app的Resources目录
#ifdef Q_OS_MAC
    return QApplication::applicationDirPath().remove(QApplication::applicationDirPath().lastIndexOf("MacOS"),6) + "Resources/";
#endif
    //linux 取程序文件所在目录下的config
#ifdef Q_OS_LINUX
    return QApplication::applicationDirPath()+"/config/";
#endif
    //windows 取程序文件所在目录下的config
#ifdef Q_OS_WIN32
    return QApplication::applicationDirPath()+"/config/";
#endif
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

/**
 * @brief Utils::qStringTrimRight 右侧trim
 * @param str
 * @return
 */

QString Utils::qStringTrimRight(const QString& str) {
    int n = str.size() - 1;
    for (; n >= 0; --n) {
        if (!str.at(n).isSpace()) {
            return str.left(n + 1);
        }
    }
    return "";
}

QString Utils::getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col)
{
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getFieldCount()){
        return "";
    }
    //本行数据
    QByteArray rowdata=qUncompress(ofdFileContentQByteArrayList->at(row));
    //字段类型
    QString fileType=ofd->getFieldList().at(col).getFieldType();
    //开始获取此字段的值
    QString field="";
    //字段长度
    int fieldlength=ofd->getFieldList().at(col).getLength();
    //小数长度
    int fieldDeclength=ofd->getFieldList().at(col).getDecLength();
    //获取此字段的值
    field=codecOFD->toUnicode(rowdata.mid(ofd->getFieldList().at(col).getRowBeginIndex(),fieldlength));
    //数据信息处理
    if(fileType=="C"){
        field=qStringTrimRight(field);
    }
    else if(fileType=="A"){
        field=qStringTrimRight(field);
    }
    else if(fileType=="N"){
        //如果字段为全部是空格，则认为字段为空
        if(field.trimmed().isEmpty()){
            field="";
        }
        //左0或者右0//或者中间0
        //这种是不合理的数据格式//除非这个字段原本长度就1位且值是0
        else if(field.trimmed()=="0"){
            field="0";
        }
        else{
            //N类型为双精度数字
            //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
            int needCheck=fieldlength-fieldDeclength-1;
            int needCutZero=0;
            int needCutZeroBegin=0;
            //判断负数标志位，如果是负数则从第二位开始切除不必要的0
            if(field.at(0)=='-'){
                needCutZeroBegin=1;
            }
            //循环切除0
            for(int s=needCutZeroBegin;s<needCheck;s++){
                if(field.at(s)=='0'){
                    needCutZero++;
                }
                else{
                    break;
                }
            }
            //获取整数
            QString left=field.left(fieldlength-fieldDeclength).remove(needCutZeroBegin,needCutZero);
            //获取小数--如果小数长度为0,就不必处理小数了
            if(fieldDeclength==0){
                field=left;
            }else{
                //获取小数
                QString right=field.right(fieldDeclength);
                //拼接整数部分和小数部分
                field=left.append(".").append(right);
            }
        }
    }
    else if(fileType=="TEXT"){
        field=qStringTrimRight(field);
    }
    return field;
}

QStringList Utils::getFormatRowValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row){
    QStringList rowList;
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
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
            QString field="";
            //字段长度
            int fieldlength=ofd->getFieldList().at(col).getLength();
            //小数长度
            int fieldDeclength=ofd->getFieldList().at(col).getDecLength();
            //获取此字段的值
            if(rowdata.count()>=ofd->getFieldList().at(col).getRowBeginIndex()+fieldlength){
                //对字节mid后返回字符串
                field=codecOFD->toUnicode(rowdata.mid(ofd->getFieldList().at(col).getRowBeginIndex(),fieldlength));
            }
            //越界表示本行后续不再有内容，终止
            else{
                break;
            }
            //数据信息处理
            if(fileType=="C"){
                field=qStringTrimRight(field);
            }
            else if(fileType=="A"){
                field=qStringTrimRight(field);
            }
            else if(fileType=="N"){
                //N类型为双精度数字
                //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                //如果字段为全部是空格，则认为字段为空
                if(field.trimmed().isEmpty()){
                    field="";
                }
                else if(field.trimmed()=="0"){
                    field="0";
                }
                else{
                    //N类型为双精度数字
                    //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                    int needCheck=fieldlength-fieldDeclength-1;
                    int needCutZero=0;
                    int needCutZeroBegin=0;
                    //判断负数标志位，如果是负数则从第二位开始切除不必要的0
                    if(field.at(0)=='-'){
                        needCutZeroBegin=1;
                    }
                    //循环切除0
                    for(int s=needCutZeroBegin;s<needCheck;s++){
                        if(field.at(s)=='0'){
                            needCutZero++;
                        }
                        else{
                            break;
                        }
                    }
                    //获取整数
                    QString left=field.left(fieldlength-fieldDeclength).remove(needCutZeroBegin,needCutZero);
                    //获取小数--如果小数长度为0,就不必处理小数了
                    if(fieldDeclength==0){
                        field=left;
                    }else{
                        //获取小数
                        QString right=field.right(fieldDeclength);
                        //拼接整数部分和小数部分
                        field=left.append(".").append(right);
                    }
                }
            }
            else if(fileType=="TEXT"){
                field=qStringTrimRight(field);
            }
            //其他类型直接原地返回
            rowList.append(field);
        }
    }
    return rowList;
}

QString Utils::getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int row ,int col)
{
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getFieldCount()){
        return "";
    }
    else{
        int fieldlength=ofd->getFieldList().at(col).getLength();
        return codecOFD->toUnicode(qUncompress(ofdFileContentQByteArrayList->at(row)).mid(ofd->getFieldList().at(col).getRowBeginIndex(),fieldlength));
    }
}

QString Utils::getFormatValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int row ,int col,QString charset)
{
    QTextCodec *codec=QTextCodec::codecForName(charset.toLocal8Bit());
    //判断越界
    if(row>=fixedContentQByteArrayList->count()||col>=fixed->getFieldCountMax()){
        return "";
    }
    //字段类型
    QString fileType=fixed->getFieldList().at(col).getFieldType();
    //开始获取此字段的值
    QString field="";
    //字段长度
    int fieldlength=fixed->getFieldList().at(col).getLength();
    //小数长度
    int fieldDeclength=fixed->getFieldList().at(col).getDecLength();
    //获取本行记录
    QByteArray rowdata=qUncompress(fixedContentQByteArrayList->at(row));
    //获取此字段的值
    //字符定长和字节定长判断
    if(fixed->getFieldlengthtype()=="0"){
        //字节截取
        //定长文件兼容多种行长度，判断越界
        if(rowdata.count()>=fixed->getFieldList().at(col).getRowBeginIndex()+fieldlength){
            //对字节mid后返回字符串
            field=codec->toUnicode(rowdata.mid(fixed->getFieldList().at(col).getRowBeginIndex(),fieldlength));
        }
        else{
            return "";
        }
    }
    //字符截取
    else{
        //判断越界
        if(rowdata.length()>=(fixed->getFieldList().at(col).getRowBeginIndex()+fieldlength)){
            //转成字符串后mid
            field=codec->toUnicode(rowdata.mid(fixed->getFieldList().at(col).getRowBeginIndex(),fieldlength));
        }
        else{
            return "";
        }
    }
    //数据信息处理
    if(fileType=="C"){
        field=qStringTrimRight(field);
    }
    else if(fileType=="A"){
        field=qStringTrimRight(field);
    }
    else if(fileType=="N"){
        //N类型为双精度数字
        //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
        //如果字段为全部是空格，则认为字段为空
        if(field.trimmed().isEmpty()){
            field="";
        }
        else if(field.trimmed()=="0"){
            field="0";
        }
        else{
            //N类型为双精度数字
            //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
            int needCheck=fieldlength-fieldDeclength-1;
            int needCutZero=0;
            int needCutZeroBegin=0;
            //判断负数标志位，如果是负数则从第二位开始切除不必要的0
            if(field.at(0)=='-'){
                needCutZeroBegin=1;
            }
            //循环切除0
            for(int s=needCutZeroBegin;s<needCheck;s++){
                if(field.at(s)=='0'){
                    needCutZero++;
                }
                else{
                    break;
                }
            }
            //获取整数
            QString left=field.left(fieldlength-fieldDeclength).remove(needCutZeroBegin,needCutZero);
            //获取小数--如果小数长度为0,就不必处理小数了
            if(fieldDeclength==0){
                field=left;
            }else{
                //获取小数
                QString right=field.right(fieldDeclength);
                //拼接整数部分和小数部分
                field=left.append(".").append(right);
            }
        }
    }
    else if(fileType=="TEXT"){
        field=qStringTrimRight(field);
    }
    //其他类型直接原地返回
    return field;
}

QStringList Utils::getFormatRowValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int row,QString charset){
    QStringList rowList;
    //判断越界
    if(row<fixedContentQByteArrayList->count()){
        QTextCodec *codec=QTextCodec::codecForName(charset.toLocal8Bit());
        //获取本行数据
        QByteArray rowdata=qUncompress(fixedContentQByteArrayList->at(row));
        //遍历获取各个字段
        int colCount=fixed->getFieldCountMax();
        for(int col=0;col<colCount;col++){
            //字段类型
            QString fileType=fixed->getFieldList().at(col).getFieldType();
            //开始获取此字段的值
            QString field="";
            //字段长度
            int fieldlength=fixed->getFieldList().at(col).getLength();
            //小数长度
            int fieldDeclength=fixed->getFieldList().at(col).getDecLength();
            //获取此字段的值
            //字符定长和字节定长判断
            if(fixed->getFieldlengthtype()=="0"){
                //字节截取
                //定长文件兼容多种行长度，判断越界
                if(rowdata.count()>=fixed->getFieldList().at(col).getRowBeginIndex()+fieldlength){
                    //对字节mid后返回字符串
                    field=codec->toUnicode(rowdata.mid(fixed->getFieldList().at(col).getRowBeginIndex(),fieldlength));
                }
                //越界表示本行后续不再有内容，终止
                else{
                    break;
                }
            }
            //字符截取
            else{
                //判断越界
                if(rowdata.length()>=(fixed->getFieldList().at(col).getRowBeginIndex()+fieldlength)){
                    //转成字符串后mid
                    field=codec->toUnicode(rowdata.mid(fixed->getFieldList().at(col).getRowBeginIndex(),fieldlength));
                }
                //越界表示本行后续不再有内容，终止
                else{
                    break;
                }
            }
            //数据信息处理
            if(fileType=="C"){
                field=qStringTrimRight(field);
            }
            else if(fileType=="A"){
                field=qStringTrimRight(field);
            }
            else if(fileType=="N"){
                //N类型为双精度数字
                //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                //如果字段为全部是空格，则认为字段为空
                if(field.trimmed().isEmpty()){
                    field="";
                }
                else if(field.trimmed()=="0"){
                    field="0";
                }
                else{
                    //N类型为双精度数字
                    //需要检查整数位需要切除的0，如果整数位为全0，则至少也要保留一位
                    int needCheck=fieldlength-fieldDeclength-1;
                    int needCutZero=0;
                    int needCutZeroBegin=0;
                    //判断负数标志位，如果是负数则从第二位开始切除不必要的0
                    if(field.at(0)=='-'){
                        needCutZeroBegin=1;
                    }
                    //循环切除0
                    for(int s=needCutZeroBegin;s<needCheck;s++){
                        if(field.at(s)=='0'){
                            needCutZero++;
                        }
                        else{
                            break;
                        }
                    }
                    //获取整数
                    QString left=field.left(fieldlength-fieldDeclength).remove(needCutZeroBegin,needCutZero);
                    //获取小数--如果小数长度为0,就不必处理小数了
                    if(fieldDeclength==0){
                        field=left;
                    }else{
                        //获取小数
                        QString right=field.right(fieldDeclength);
                        //拼接整数部分和小数部分
                        field=left.append(".").append(right);
                    }
                }
            }
            else if(fileType=="TEXT"){
                field=qStringTrimRight(field);
            }
            //其他类型直接原地返回
            rowList.append(field);
        }
    }
    return rowList;
}

QStringList Utils::getRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> *  csvFileContentQByteArrayList,CsvFileDefinition * csv,int row,QString charset){
    //判断越界
    QStringList rowData;
    if(row>=csvFileContentQByteArrayList->count()){
        return rowData;
    }else{
        QTextCodec *codec=QTextCodec::codecForName(charset.toLocal8Bit());
        //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
        if(csv->getClearQuotes()&&csv->getSplit().length()==1){
            QRegExp rx("\\"+csv->getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
            rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(rx);
        }
        else{
            rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(csv->getSplit());
        }
        //需要处理分隔符
        if(csv->getClearQuotes()&&rowData.count()>0){
            QStringList rowData2;
            for(int i=0;i<rowData.count();i++){
                rowData2.append(clearQuotes(rowData.at(i)));
            }
            return rowData2;
        }
        else{
            return rowData;
        }
    }
}

QStringList Utils::getOriginalRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> *  csvFileContentQByteArrayList,CsvFileDefinition * csv,int row,QString charset){
    //判断越界
    QStringList rowData;
    if(row>=csvFileContentQByteArrayList->count()){
        return rowData;
    }else{
        QTextCodec *codec=QTextCodec::codecForName(charset.toLocal8Bit());
        if(csv->getClearQuotes()&&csv->getSplit().length()==1){
            QRegExp rx("\\"+csv->getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
            rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(rx);
        }
        else{
            rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(csv->getSplit());
        }
        return rowData;
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
    doubleString=doubleString.trimmed();
    int strLength=doubleString.length();
    int BeginIndex=strLength-1;
    if(doubleString.contains(".")){
        BeginIndex=doubleString.indexOf(".")-1;
    }
    int z=1;
    for(int index=BeginIndex;index>0;index--){
        if(z%3==0){
            if(!(index==1&&doubleString.at(0)=='-')){
                doubleString.insert(index,QChar(','));
            }
        }
        z++;
    }
    return doubleString;
}

/**
 * @brief Utils::UpdateFileTime 更新文件修改时间为最新
 * @param file
 */
void Utils::UpdateFileTime(QString file,QDateTime lastModifyTime){
    int aYear = lastModifyTime.date().year()-1900;
    int aMonth = lastModifyTime.date().month()-1;
    int aDay = lastModifyTime.date().day();
    int aHour = lastModifyTime.time().hour();
    int aMinute = lastModifyTime.time().minute();
    int aSec = lastModifyTime.time().second();
    struct tm tma = {0};
    tma.tm_year = aYear;
    tma.tm_mon = aMonth;
    tma.tm_mday = aDay;
    tma.tm_hour = aHour;
    tma.tm_min = aMinute;
    tma.tm_sec = aSec;
    tma.tm_isdst = 0;
#ifdef Q_OS_WIN32
    struct _utimbuf ut;
#else
    struct utimbuf ut;
#endif
    //二者得同时修改，否则修改不成功
    ut.modtime = mktime(&tma);// 最后修改时间
    ut.actime=mktime(&tma);//最后访问时间
    QByteArray ary=file.toLocal8Bit();
    char *fileName = ary.data();
#ifdef Q_OS_WIN32
    _utime(fileName, &ut );
#else
    utime(fileName, &ut );
#endif
}

/**
 * @brief Utils::getFileListFromDir 遍历文件夹获得文件列表
 * @param dirpath
 * @param filelist
 */
void Utils::getFileListFromDir(QString dirpath,QStringList *filelist){
    QDir dir(dirpath);
    if (!dir.exists()) {
        return;
    }
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return;
    }
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            QString path=fileInfo.filePath();
            Utils::getFileListFromDir(path,filelist);
        }
        else{
            filelist->append(fileInfo.filePath());
        }
        i++;
    } while(i < list.size());
    return;
}

/**
 * @brief Utils::clearQuotes 清除csv字段的双引号
 * @param stringS
 * @return
 */
QString Utils::clearQuotes(QString stringS){

    if(!stringS.isEmpty() && stringS.length()>=2) {
        if(stringS.indexOf("\"")==0) {
            stringS = stringS.mid(1,-1);
        }
        if(stringS.lastIndexOf("\"")==(stringS.length()-1)) {
            stringS = stringS.mid(0,stringS.length()-1);
        }
        stringS = stringS.replace("\"\"","\"");
    }
    return stringS;
}
