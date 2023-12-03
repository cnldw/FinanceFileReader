/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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

/**
 * @brief Utils::load_OFDCodeInfo加载ofd的代码配置信息,包含销售商，TA信息-这个数据取自tools目录下的爬虫工具-数据来源为证券市场标准网
 * @param loadedOfdCodeInfo
 */
void Utils::load_OFDCodeInfo(QHash<QString, OFDCodeInfo> &loadedOfdCodeInfo){
    QString codeInipath=Utils::getConfigPath()+"OFD_CodeInfo.ini";
    if(Utils::isFileExist(codeInipath)){
        //加载ini文件
        QSettings loadedCodeInfoIni(codeInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        loadedCodeInfoIni.setIniCodec("UTF-8");
        QStringList agencyInfo=loadedCodeInfoIni.childGroups();
        //获取所有代码和代码对应的机构名称
        for(int i=0;i<agencyInfo.count();i++){
            OFDCodeInfo infoItem;
            infoItem.setCode(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString());
            infoItem.setName(loadedCodeInfoIni.value(agencyInfo.at(i)+"/NAME").toString());
            loadedOfdCodeInfo.insert(loadedCodeInfoIni.value(agencyInfo.at(i)+"/AGENCYNO").toString(),infoItem);
        }
    }
}

/**
 * @brief Utils::load_OFDDictionary 加载OFD的字典信息
 * @param ofdDictionary
 */
void Utils::load_OFDDictionary(Dictionary &ofdDictionary){
    ofdDictionary.clearDictionary();
    QString dictionaryInipath=Utils::getConfigPath()+"OFD_Dictionary.ini";
    if(Utils::isFileExist(dictionaryInipath)){
        //加载ini文件
        QSettings loadedDictionary(dictionaryInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        loadedDictionary.setIniCodec("UTF-8");
        //获取所有fixed文件的配置信息，循环存储
        QStringList dictionaryGroup=loadedDictionary.childGroups();
        //查不到任何段落
        if(dictionaryGroup.count()<1){
            return;
        }
        //循环各个字典组
        for(int i=0;i<dictionaryGroup.count();i++){
            loadedDictionary.beginGroup(dictionaryGroup.at(i));
            QStringList allkey=loadedDictionary.allKeys();
            if(allkey.count()>0){
                for (int j=0;j<allkey.count();j++){
                    //循环插入字典
                    //字典分组_key=a:xxxb:xxx
                    ofdDictionary.addDictionary(dictionaryGroup.at(i)+"_"+allkey.at(j)+"="+loadedDictionary.value(allkey.at(j)).toString());
                }
            }
            loadedDictionary.endGroup();
        }
    }
}

void Utils::load_OFDTipDictionary(QMap<QString,QMap<QString,QString>> &fieldTips){
    fieldTips.clear();
    QString TipDictionaryInipath=Utils::getConfigPath()+"OFD_TipDictionary.ini";
    if(Utils::isFileExist(TipDictionaryInipath)){
        //加载ini文件
        QSettings loadedDictionary(TipDictionaryInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        loadedDictionary.setIniCodec("UTF-8");
        //获取所有fixed文件的配置信息，循环存储
        QStringList dictionaryGroup=loadedDictionary.childGroups();
        //查不到任何段落
        if(dictionaryGroup.count()<1){
            return;
        }
        //循环各个字典组
        //tip字典和字段枚举字典一样分组
        for(int i=0;i<dictionaryGroup.count();i++){
            loadedDictionary.beginGroup(dictionaryGroup.at(i));
            QStringList allkey=loadedDictionary.allKeys();
            QMap<QString,QString> tips;
            if(allkey.count()>0){
                for (int j=0;j<allkey.count();j++){
                    //如果值不空,则认为是一条有效的tips
                    if(!loadedDictionary.value(allkey.at(j)).toString().isEmpty()){
                        tips.insert(allkey.at(j),loadedDictionary.value(allkey.at(j)).toString());
                    }
                }
                //OFD的tips存储结构为OFD001这样的OFD开头的文件类别
                fieldTips.insert("OFD"+dictionaryGroup.at(i),tips);
            }
            loadedDictionary.endGroup();
        }
    }
}
void Utils::load_OFDLikeFileDefinition(QHash<QString, QString> &likeOFDIndexFilename,QHash<QString, QString>  &likeOFDDataFilename){
    QString fileTypeInipath=Utils::getConfigPath()+"OFD_Like.ini";
    if(Utils::isFileExist(fileTypeInipath)){
        //加载ini文件
        QSettings fileTypeIni(fileTypeInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        fileTypeIni.setIniCodec("UTF-8");
        QStringList infoList;
        //读取INDEXFILEPREFIX
        fileTypeIni.beginGroup("INDEXFILEPREFIX");
        infoList =fileTypeIni.childKeys();
        fileTypeIni.endGroup();
        likeOFDIndexFilename.clear();
        if(infoList.count()>0){
            for(int i=0;i<infoList.count();i++){
                likeOFDIndexFilename.insert(infoList.at(i),fileTypeIni.value("INDEXFILEPREFIX/"+infoList.at(i)).toString());
            }
        }
        infoList.clear();
        ////////////////////////////////////////////////////////
        //读取DATAFILEPREFIX
        fileTypeIni.beginGroup("DATAFILEPREFIX");
        infoList =fileTypeIni.childKeys();
        fileTypeIni.endGroup();
        likeOFDDataFilename.clear();
        if(infoList.count()>0){
            for(int i=0;i<infoList.count();i++){
                likeOFDDataFilename.insert(infoList.at(i),fileTypeIni.value("DATAFILEPREFIX/"+infoList.at(i)).toString());
            }
        }
        infoList.clear();
    }
}

void Utils::load_OFDIndexFileDefinition(QHash<QString, QString> &loadedOfdIndexFileInfo){
    loadedOfdIndexFileInfo.clear();
    QString fileTypeInipath=Utils::getConfigPath()+"OFD_IndexFile.ini";
    if(Utils::isFileExist(fileTypeInipath)){
        //加载ini文件
        QSettings fileTypeIni(fileTypeInipath,QSettings::IniFormat,nullptr);
        //目前仅接收UTF-8编码的配置文件
        fileTypeIni.setIniCodec("UTF-8");
        //读取OFDINDEXFILE
        fileTypeIni.beginGroup("OFDINDEXFILE");
        QStringList infoList =fileTypeIni.childKeys();
        fileTypeIni.endGroup();
        //遍历组OFDINDEXFILE
        if(infoList.count()>0){
            for(int i=0;i<infoList.count();i++){
                loadedOfdIndexFileInfo.insert(infoList.at(i),fileTypeIni.value("OFDINDEXFILE/"+infoList.at(i)).toString());
            }
        }
        infoList.clear();
    }
}

void Utils::load_OFDDefinition(QList<ConfigFile<OFDFileDefinition>> &ofdConfigList,QHash<QString,int> &ofdQuickMatchIndex){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "OFD_*.ini";//设置过滤类型
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList list = dirPath.entryInfoList();
    if(list.length()!=0){
        //循环检索config目录下可用的接口定义配置
        //OFD开头ini结尾,形式类似于OFD_XXX.ini
        /*-------------------------------开启第一重循环,遍历config下的OFD配置文件-----------------------------------*/
        for (int f = 0; f < list.size(); f++)
        {

            QString fileName=list.at(f).fileName();
            QString fixName=fileName;

            //排除几个文件
            if(fileName=="OFD_CodeInfo.ini"||fileName=="OFD_Dictionary.ini"||fileName=="OFD_IndexFile.ini"||fileName=="OFD_TipDictionary.ini"||fileName=="OFD_Like.ini"){
                continue;
            }
            if(fileName.startsWith("OFD",Qt::CaseInsensitive)&&fileName.endsWith("ini",Qt::CaseInsensitive)){
                //编辑加载文件
                if(fixName.contains(".",Qt::CaseInsensitive)){
                    fixName=fixName.mid(0,fixName.indexOf("."));
                }
                QStringList nameList=fixName.split("_");
                if(nameList.count()==2){
                    //本文件的定义
                    ConfigFile<OFDFileDefinition> ofdconfigfile;
                    ofdconfigfile.setFileName(fileName);
                    QList<OFDFileDefinition> configSegmentList;

                    //加载ini文件
                    QString configFileVersion=nameList.at(1);
                    QSettings ofdIni(Utils::getConfigPath()+fileName,QSettings::IniFormat,nullptr);
                    //目前仅接收UTF-8编码的配置文件
                    ofdIni.setIniCodec("UTF-8");
                    QStringList interfaceList=ofdIni.childGroups();
                    //获取所有文件类别定义
                    if(interfaceList.count()>0){
                        /*-----------------第二重循环，遍历OFD文件内的文件定义----------------------*/
                        /* /////////////////////////开始解析本文件可用接口文件定义////////// */
                        for(int i=0;i<interfaceList.count();i++){
                            OFDFileDefinition ofd;
                            ofd.setConfigSegment("["+interfaceList.at(i)+"]");
                            //包含版本和文件类别的字符串比如21_07
                            ofd.setUseForVersionAndType(configFileVersion+"_"+interfaceList.at(i).split("_").first());
                            QList<OFDFieldDefinition> fieldList;
                            QString message;
                            bool okFlag=false;
                            //记录每行的长度，自动总数
                            int rowLength=0;
                            int fieldCount=0;
                            //文件说明
                            QString describe=ofdIni.value(interfaceList.at(i)+"/DESCRIBE").toString();
                            if (!describe.isEmpty()){
                                ofd.setDescribe(describe);
                            }
                            else{
                                ofd.setDescribe("未配置说明的文件");
                            }
                            //字典类型
                            QString dictionary=ofdIni.value(interfaceList.at(i)+"/DICTIONARY").toString();
                            //设置解析字典类型
                            ofd.setDictionary(dictionary.isEmpty()?"":dictionary);
                            //查找COUNT标记
                            QString countStr=ofdIni.value(interfaceList.at(i)+"/COUNT").toString();
                            if(countStr.isEmpty()){
                                message="找不到"+interfaceList.at(i)+"文件的字段总数配置";
                                okFlag=false;
                            }else{
                                bool ok;
                                int countInt=countStr.toInt(&ok,10);
                                if(ok&&countInt>0){
                                    fieldCount=countInt;
                                    /*------第三重循环--开始按行解析此接口文件的定义----------------*/
                                    //解析规则-一旦解析到一个失败的记录，就不再往下解析了
                                    for(int j=1;j<=countInt;j++){
                                        OFDFieldDefinition ofdfield;
                                        //获取这个文件定义的第i个字段的信息
                                        QStringList iniStrList=ofdIni.value(interfaceList.at(i)+"/"+QString::number(j)).toStringList();
                                        if (iniStrList.isEmpty()||iniStrList.length()!=5){
                                            message=ofd.getConfigSegment()+"文件的第"+QString::number(j)+"个字段定义不正确或者缺失";
                                            okFlag=false;
                                            break;
                                        }
                                        //获取字符类型参数
                                        QString fieldType=(QString(iniStrList.at(0)));
                                        if(fieldType.length()<1){
                                            message=ofd.getConfigSegment()+"文件的第"+QString::number(j)+"个字段的类型定义缺失,请配置";
                                            okFlag=false;
                                            break;
                                        }
                                        //字段长度
                                        bool lengthOk;
                                        int length=(QString(iniStrList.at(1))).toInt(&lengthOk,10);
                                        if(!lengthOk){
                                            message=ofd.getConfigSegment()+"文件的第"+QString::number(j)+"个字段的长度定义不是整数";
                                            okFlag=false;
                                            break;
                                        }
                                        //字段小数长度
                                        bool declengthOk;
                                        int declength=(QString(iniStrList.at(2))).toInt(&declengthOk,10);
                                        if(!declengthOk){
                                            message=ofd.getConfigSegment()+"文件的第"+QString::number(j)+"个字段的小数长度定义不是整数";
                                            okFlag=false;
                                            break;
                                        }
                                        if(fieldType=="N"&&(declength>=length)){
                                            message=ofd.getConfigSegment()+"文件的第"+QString::number(j)+"个字段的小数长度定义应该小于字段总长度";
                                            okFlag=false;
                                            break;
                                        }
                                        //历经千辛万苦经理校验无误后存入字段信息
                                        //每个字段开始的位置就是截至到上一个字段结束的长度
                                        //比如第一个字段长度为2,则第二个字段在此行记录中的起始坐标就是2（0,1这两个字节是第一个字段的索引）
                                        ofdfield.setRowBeginIndex(rowLength);
                                        rowLength+=length;
                                        ofdfield.setLength(length);
                                        ofdfield.setDecLength(declength);
                                        ofdfield.setFieldType(fieldType);
                                        ofdfield.setFieldName(QString(iniStrList.at(4)));
                                        ofdfield.setFieldDescribe(QString(iniStrList.at(3)));
                                        fieldList.append(ofdfield);
                                        //更新记录为成功
                                        okFlag=true;
                                    }
                                    //字段配置解析正确的情况下，才考虑解析必填规则
                                    if(okFlag){
                                        ofdIni.beginGroup(interfaceList.at(i));
                                        QStringList infoList =ofdIni.childKeys();
                                        ofdIni.endGroup();
                                        if(infoList.count()>0){
                                            for (int checkindex=0;checkindex<infoList.count();checkindex++){
                                                //字段必填配置
                                                if(infoList.at(checkindex).startsWith("fieldcheck_")){
                                                    bool okFlag=false;
                                                    fieldcheckitem ckitem=Utils::parseStringtofieldcheckitem(ofdIni.value(interfaceList.at(i)+"/"+infoList.at(checkindex)).toString(),countInt,&okFlag);
                                                    if(okFlag){
                                                        ofd.addFieldcheckItem(ckitem);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else{
                                    message=ofd.getConfigSegment()+"文件的字段总数配置COUNT标记不是有效的整数";
                                    okFlag=false;
                                }
                            }
                            //写入该文件的配置
                            //如果记录完整没错误则写入
                            if(okFlag){
                                //记录数的长度/////////20190723///////////
                                QString rowcountLengthStr=ofdIni.value(interfaceList.at(i)+"/ROWCOUNTLENGTH").toString();
                                if(rowcountLengthStr.isEmpty()){
                                    //找不到就默认设置为8
                                    ofd.setRowCountLength(8);
                                }else{
                                    bool ok;
                                    int rowcountLengthInt=rowcountLengthStr.toInt(&ok,10);
                                    if(ok&&rowcountLengthInt>0){
                                        ofd.setRowCountLength(rowcountLengthInt);
                                    }
                                    else{
                                        //读取配置错误也设置为8
                                        ofd.setRowCountLength(8);
                                    }
                                }
                                ///////////////////////////
                                ofd.setUseAble(okFlag);
                                ofd.setMessage(message);
                                ofd.setFieldCount(fieldCount);
                                ofd.setRowLength(rowLength);
                                ofd.setFieldList(fieldList);
                                configSegmentList.append(ofd);
                            }
                            //如果记录有错误,则仅写于不可用和错误原因
                            else{
                                ofd.setUseAble(okFlag);
                                ofd.setMessage(message);
                                configSegmentList.append(ofd);
                            }
                        }
                    }
                    ofdconfigfile.setConfigSegmentList(configSegmentList);
                    ofdConfigList.append(ofdconfigfile);
                    //记录版本和config的映射关系,方便快速找到某个版本的配置是否存在
                    ofdQuickMatchIndex.insert(configFileVersion,ofdConfigList.count()-1);
                }
            }
        }
    }
}

void Utils::load_CSVDefinition(QList<ConfigFile<CsvFileDefinition>> &csvConfigList,QHash<QString,Dictionary> &commonDictionary,QMap<QString,QMap<QString,QString>> &commonFieldTips){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "CSV_*.ini";//设置过滤类型
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList listFile = dirPath.entryInfoList();
    if(listFile.length()!=0){
        //循环检索config目录下可用的CSV接口定义配置
        for (int f = 0; f < listFile.size(); f++)
        {
            QString fileName=listFile.at(f).fileName();
            QString inipath=Utils::getConfigPath()+fileName;
            if(Utils::isFileExist(inipath)){
                //本文件的定义
                ConfigFile<CsvFileDefinition> csvconfigfile;
                csvconfigfile.setFileName(fileName);
                QList<CsvFileDefinition> configSegmentList;
                //加载ini文件
                QSettings loadedCsvInfoIni(inipath,QSettings::IniFormat,nullptr);
                //目前仅接收UTF-8编码的配置文件
                loadedCsvInfoIni.setIniCodec("UTF-8");
                QStringList csvInfo=loadedCsvInfoIni.childGroups();
                if(csvInfo.count()>0){
                    //获取所有csv文件的配置信息，循环存储
                    for(int i=0;i<csvInfo.count();i++){
                        QString csvType=QString(csvInfo.at(i));
                        if(csvType.contains("|",Qt::CaseSensitive)){
                            //建立一个定义变量
                            CsvFileDefinition fileDef;
                            //本配置来自的配置文件
                            fileDef.setFileIni(fileName);
                            //分割文件名和字段数
                            QStringList list=csvType.split("|");
                            //1解析器可接受的文件名
                            fileDef.setFileName(QString (list.at(0)));
                            //2解析器配置全名
                            fileDef.setFileNameWithCount(csvType);
                            fileDef.setConfigSegment("["+csvType+"]");
                            //3文件描述信息
                            QString fileDescribe=loadedCsvInfoIni.value(csvType+"/filedescribe").toString();
                            if(fileDescribe.isEmpty()){
                                fileDef.setFileDescribe("未说明的CSV文件");
                            }else{
                                fileDef.setFileDescribe(fileDescribe);
                            }
                            //5分隔符
                            QString splitflag=loadedCsvInfoIni.value(csvType+"/splitflag").toString();
                            //注意不要乱删除这几行代码，qt有个bug，ini配置里某个字段的值是,(英文逗号)获取时会获取到空值，坑！
                            //如果列分割为配置符号为空，则分隔符配置设置为,
                            if(splitflag.isEmpty()){
                                splitflag=",";
                            }
                            fileDef.setSplit(splitflag);
                            //5是否以分隔符做结尾
                            QString endwithflag=loadedCsvInfoIni.value(csvType+"/endwithflag").toString();
                            fileDef.setEndwithflag(endwithflag);
                            //6数据起始行
                            bool beginflagok;
                            int  beginrowindex=loadedCsvInfoIni.value(csvType+"/datarowbeginindex").toInt(&beginflagok);
                            if(!beginflagok){
                                fileDef.setDatabeginrowindex(0);
                            }
                            //如果数据起始行小于1，则强制为1
                            else if(beginrowindex<1){
                                fileDef.setDatabeginrowindex(1);
                            }else{
                                fileDef.setDatabeginrowindex(beginrowindex);
                            }
                            //7标题行
                            bool titleflagok;
                            int  titlerowindex=loadedCsvInfoIni.value(csvType+"/titlerowindex").toInt(&titleflagok);
                            if(!titleflagok){
                                fileDef.setTitlerowindex(0);
                            }
                            //如果不存在标题栏目，则标题行强制设置为0
                            else if(titlerowindex<1){
                                fileDef.setTitlerowindex(0);
                            }else{
                                fileDef.setTitlerowindex(titlerowindex);
                            }
                            //8编码信息
                            QString encoding=loadedCsvInfoIni.value(csvType+"/encoding").toString();
                            if(encoding.toUpper()=="AUTO"){
                                fileDef.setAutoecoding(true);
                            }
                            if(encoding.isEmpty()){
                                fileDef.setEcoding("UTF-8");
                            }
                            else{
                                fileDef.setEcoding(encoding);
                            }
                            //9数据尾部忽略行
                            bool flag=false;
                            QString ignorerow=loadedCsvInfoIni.value(csvType+"/endignorerow").toString();
                            int endignoreRow;
                            if(ignorerow.isEmpty()){
                                //不配置就不忽略任何行
                                fileDef.setEndIgnoreRow(0);
                            }
                            else{
                                endignoreRow=ignorerow.toInt(&flag);
                                if(!flag){
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage("文件结尾忽略行数不是一个可用的数值");
                                    configSegmentList.append(fileDef);
                                    continue;
                                }
                                else{
                                    if(endignoreRow<0){
                                        flag=false;
                                        fileDef.setUseAble(false);
                                        fileDef.setMessage("文件结尾忽略行数应当是一个大于等于0的整数");
                                        configSegmentList.append(fileDef);
                                        continue;
                                    }else {
                                        fileDef.setEndIgnoreRow(endignoreRow);
                                    }
                                }
                            }
                            //10是否清除双引号
                            QString clearQuotesStr=loadedCsvInfoIni.value(csvType+"/clearquotes").toString();
                            if(clearQuotesStr=="1") {
                                fileDef.setClearQuotes(true);
                            }
                            else{
                                fileDef.setClearQuotes(false);
                            }
                            //11是否进行trim
                            QString TrimStr=loadedCsvInfoIni.value(csvType+"/trim").toString();
                            if(TrimStr=="1") {
                                fileDef.setTrim(true);
                            }
                            else{
                                fileDef.setTrim(false);
                            }
                            //12-版本记录所在行
                            QString versioncheckrow=loadedCsvInfoIni.value(csvType+"/versioncheckrow").toString();
                            int versioncheckrowint;
                            if(versioncheckrow.isEmpty()){
                                //不配置就是不检验版本
                                fileDef.setVersioncheckrow(0);
                            }
                            else{
                                versioncheckrowint=versioncheckrow.toInt(&flag);
                                if(!flag){
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage("版本检查行配置versioncheckrow不是一个有效数据");
                                    configSegmentList.append(fileDef);
                                    continue;
                                }
                                else{
                                    if(versioncheckrowint<0){
                                        flag=false;
                                        fileDef.setUseAble(false);
                                        fileDef.setMessage("版本检查行配置versioncheckrow应当是一个大于等于0的整数,0为不进行版本检查,大于0则代表进行版本检查");
                                        configSegmentList.append(fileDef);
                                        continue;
                                    }else {
                                        fileDef.setVersioncheckrow(versioncheckrowint);
                                    }
                                }
                            }
                            //13版本标志行
                            QString version=loadedCsvInfoIni.value(csvType+"/version").toString();
                            if(version.isEmpty()){
                                //不配置就不校验
                                fileDef.setVersion("");
                            }
                            else{
                                fileDef.setVersion(version);
                            }
                            //14-版本号检测模式
                            QString versioncheckmode=loadedCsvInfoIni.value(csvType+"/versioncheckmode").toString();
                            int versioncheckmodeint;
                            if(versioncheckmode.isEmpty()){
                                fileDef.setVersioncheckmode(0);
                            }
                            else{
                                versioncheckmodeint=versioncheckmode.toInt(&flag);
                                if(!flag){
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage("版本号检查模式配置versioncheckmode不是一个有效数据");
                                    configSegmentList.append(fileDef);
                                    continue;
                                }
                                else{
                                    if(versioncheckmodeint<0){
                                        flag=false;
                                        fileDef.setUseAble(false);
                                        fileDef.setMessage("版本检查行配置versioncheckmode应当是一个大于等于0的整数,0为精确匹配,非0则代表模糊匹配");
                                        configSegmentList.append(fileDef);
                                        continue;
                                    }else {
                                        fileDef.setVersioncheckmode(versioncheckmodeint);
                                    }
                                }
                            }
                            //15首行检测
                            QString firstrowcheck=loadedCsvInfoIni.value(csvType+"/firstrowcheck").toString();
                            if(firstrowcheck.isEmpty()){
                                //不配置就不校验
                                fileDef.setFirstrowcheck("");
                            }
                            else{
                                fileDef.setFirstrowcheck(firstrowcheck);
                            }
                            //16尾行检测
                            QString lastrowcheck=loadedCsvInfoIni.value(csvType+"/lastrowcheck").toString();
                            if(lastrowcheck.isEmpty()){
                                //不配置就不校验
                                fileDef.setLastrowcheck("");
                            }
                            else{
                                fileDef.setLastrowcheck(lastrowcheck);
                            }
                            //////////////////////////////////////
                            //最后字段总数和字段内容
                            QString fieldCcountStr=loadedCsvInfoIni.value(csvType+"/fieldcount").toString();
                            int fieldCount=fieldCcountStr.toInt(&flag,10);
                            //字段描述值正确
                            if(flag&&fieldCount>0){
                                //4字段总数
                                fileDef.setFieldCount(fieldCount);
                                QList <CsvFieldDefinition> fieldList;
                                //10开始循环加载本文件类型的字段信息///////////////
                                bool fieldListIsOK=true;
                                for(int r=0;r<fieldCount;r++){
                                    CsvFieldDefinition fieldItem;
                                    QStringList iniStrList=loadedCsvInfoIni.value(csvType+"/"+(QString::number(r+1,10))).toStringList();
                                    if(iniStrList.isEmpty()){
                                        fieldItem.setFieldDescribe("未定义的字段名");
                                    }
                                    else{
                                        //中文字段名
                                        QString desc=iniStrList.at(0);
                                        if(desc.isEmpty()){
                                            fieldItem.setFieldDescribe("未定义的字段名");
                                        }
                                        else{
                                            fieldItem.setFieldDescribe(desc);
                                        }
                                        if(iniStrList.count()>=2){
                                            QString name=iniStrList.at(1);
                                            fieldItem.setFieldName(name);
                                        }
                                        if(iniStrList.count()>=3){
                                            QString numbercheck=iniStrList.at(2);
                                            if(numbercheck.isEmpty()){
                                                fieldItem.setIsNumber(-1);
                                            }
                                            else if(numbercheck=="-1"){
                                                fieldItem.setIsNumber(-1);
                                            }
                                            else if(numbercheck=="0"){
                                                fieldItem.setIsNumber(0);
                                            }
                                            else if(numbercheck=="1"){
                                                fieldItem.setIsNumber(1);
                                            }
                                            else{
                                                fieldListIsOK=false;
                                                fileDef.setMessage(QString("第%1个字段是否是数值的标志位存在非法值,-1和留空代表程序自动判断,0为非数值,1为数值").arg(QString::number(r+1,10)));
                                                break;
                                            }
                                        }
                                        if(iniStrList.count()>=4){
                                            QString numberShift=iniStrList.at(3);
                                            if(numberShift.isEmpty()){
                                                fieldItem.setDecimalPointShift(0);
                                            }
                                            else{
                                                bool flag=false;
                                                int numberShiftint=numberShift.toInt(&flag,10);
                                                if(flag){
                                                    if(numberShiftint<0||numberShiftint>4){
                                                        fieldListIsOK=false;
                                                        fileDef.setMessage(QString("第%1个字段数值缩放位数存在非法值,留空和0代表数值不缩放,1-4代表缩放10倍-1万倍").arg(QString::number(r+1,10)));
                                                        break;
                                                    }
                                                    else{
                                                        fieldItem.setDecimalPointShift(numberShiftint);
                                                    }
                                                }
                                                else{
                                                    fieldListIsOK=false;
                                                    fileDef.setMessage(QString("第%1个字段数值缩放位数存在非法值,留空和0代表数值不缩放,1-4代表缩放10倍-1万倍").arg(QString::number(r+1,10)));
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    //添加此字段信息到文件定义
                                    fieldList.append(fieldItem);
                                }
                                if(fieldListIsOK){
                                    //必填加工
                                    loadedCsvInfoIni.beginGroup(csvType);
                                    QStringList infoList =loadedCsvInfoIni.childKeys();
                                    loadedCsvInfoIni.endGroup();
                                    if(infoList.count()>0){
                                        for (int checkindex=0;checkindex<infoList.count();checkindex++){
                                            //字段必填配置
                                            if(infoList.at(checkindex).startsWith("fieldcheck_")){
                                                bool okFlag=false;
                                                fieldcheckitem ckitem=Utils::parseStringtofieldcheckitem(loadedCsvInfoIni.value(csvType+"/"+infoList.at(checkindex)).toString(),fieldCount,&okFlag);
                                                if(okFlag){
                                                    fileDef.addFieldcheckItem(ckitem);
                                                }
                                            }
                                        }
                                    }
                                    fileDef.setFieldList(fieldList);
                                    fileDef.setUseAble(true);
                                    configSegmentList.append(fileDef);
                                }
                                else{
                                    fileDef.setFieldList(fieldList);
                                    fileDef.setUseAble(false);
                                    configSegmentList.append(fileDef);
                                }
                            }
                            //如果字段数是AUTO，则字段总和设置为-1，在进行文件解析是自动分析标题数量来进行解析
                            //这种模式在带文件标题的情况下，自动解析标题，如果不带标题，则解析是标题全部描述为第XX列
                            else if(fieldCcountStr.toUpper()=="AUTO"){
                                //必填加工
                                loadedCsvInfoIni.beginGroup(csvType);
                                QStringList infoList =loadedCsvInfoIni.childKeys();
                                loadedCsvInfoIni.endGroup();
                                if(infoList.count()>0){
                                    for (int checkindex=0;checkindex<infoList.count();checkindex++){
                                        //字段必填配置
                                        if(infoList.at(checkindex).startsWith("fieldcheck_")){
                                            bool okFlag=false;
                                            fieldcheckitem ckitem=Utils::parseStringtofieldcheckitem(loadedCsvInfoIni.value(csvType+"/"+infoList.at(checkindex)).toString(),-1,&okFlag);
                                            if(okFlag){
                                                fileDef.addFieldcheckItem(ckitem);
                                            }
                                        }
                                    }
                                }
                                fileDef.setFieldCount(-1);
                                fileDef.setUseAble(true);
                                configSegmentList.append(fileDef);
                            }
                            else{
                                fileDef.setUseAble(false);
                                fileDef.setMessage("字段总数描述不是正确可用的数值(字段数需为大于0的整数)或者为AUTO(自动解析列数形式)");
                                configSegmentList.append(fileDef);
                            }
                        }
                        else
                            //判断是否是字典配置，字典配置是csv配置文件里的一段特殊配置，使用Dictionary标注
                        {
                            if(csvType=="Dictionary"){
                                Dictionary dic;
                                //开始循环读取字典
                                //不要尝试使用begingroup后直接读取组内参数，否则会读取不到包含逗号的键值
                                loadedCsvInfoIni.beginGroup("Dictionary");
                                QStringList infoList =loadedCsvInfoIni.childKeys();
                                loadedCsvInfoIni.endGroup();
                                if(infoList.count()>0){
                                    for(int aaa=0;aaa<infoList.count();aaa++){
                                        //ini配置中如果key存在中文会出现错误，所以我们使用1=key@value的方式去改造
                                        QString line=loadedCsvInfoIni.value("Dictionary/"+infoList.at(aaa)).toString();
                                        int index=line.indexOf('@');
                                        if(index>0){
                                            //只替换第一个出现的@，以免替换了字典里的其他@符号
                                            dic.addDictionary(line.replace(index,1,'='));
                                        }
                                        //不包含@的跳过
                                        else{
                                            continue;
                                        }
                                    }
                                    //将字典加入qhash/csv类的字典以csv开头
                                    commonDictionary.insert("CSV"+fileName,dic);
                                }
                                infoList.clear();
                            }
                            //字段Tips加载
                            else if(csvType=="TipDictionary"){
                                //开始循环读取字典
                                loadedCsvInfoIni.beginGroup("TipDictionary");
                                QStringList infoList =loadedCsvInfoIni.childKeys();
                                loadedCsvInfoIni.endGroup();
                                if(infoList.count()>0){
                                    QMap<QString,QString> tips;
                                    for(int aaa=0;aaa<infoList.count();aaa++){
                                        //ini配置中如果key存在中文会出现错误，所以我们使用1=key:value的方式去改造
                                        QString line=loadedCsvInfoIni.value("TipDictionary/"+infoList.at(aaa)).toString();
                                        //从第一个：拆分这样做到优雅的tips中可以存在：
                                        int index=line.indexOf(':');
                                        if(index>0){
                                            tips.insert(line.mid(0,index),line.mid(index+1));
                                        }
                                        //不包含:的跳过
                                        else{
                                            continue;
                                        }
                                    }
                                    //以文件类别，配置文件名存入
                                    commonFieldTips.insert("CSV"+fileName,tips);
                                }
                                infoList.clear();
                            }
                            else{
                                //无效配置段落，跳过
                                continue;
                            }
                        }
                    }
                }
                csvconfigfile.setConfigSegmentList(configSegmentList);
                csvConfigList.append(csvconfigfile);
            }
        }
    }
}


void Utils::load_FIXEDDefinition(QList<ConfigFile<FIXEDFileDefinition>> &fixedConfigList,QHash<QString,Dictionary> &commonDictionary,QMap<QString,QMap<QString,QString>> &commonFieldTips){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "FIXED_*.ini";//设置过滤类型,以FIXED_开头的配置
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList listFile = dirPath.entryInfoList();
    if(listFile.length()!=0){
        //循环检索config目录下可用的FIXED接口定义配置
        for (int f = 0; f < listFile.size(); f++)
        {
            //获取文件名
            QString fileName=listFile.at(f).fileName();
            //获取文件路径
            QString codeInipath=Utils::getConfigPath()+fileName;
            if(Utils::isFileExist(codeInipath)){
                //本文件的定义
                ConfigFile<FIXEDFileDefinition> fixedconfigfile;
                fixedconfigfile.setFileName(fileName);
                QList<FIXEDFileDefinition> configSegmentList;
                //加载ini文件
                QSettings loadedFixedInfoIni(codeInipath,QSettings::IniFormat,nullptr);
                //目前仅接收UTF-8编码的配置文件
                loadedFixedInfoIni.setIniCodec("UTF-8");
                //获取所有fixed文件的配置信息，循环存储
                QStringList fixedInfo=loadedFixedInfoIni.childGroups();
                if(fixedInfo.count()<1){
                    continue;
                }
                //遍历所有组
                for(int i=0;i<fixedInfo.count();i++){
                    //配置验证失败标志
                    bool flag=false;
                    //配置验证失败原因
                    QString message;
                    //每行最大列数
                    int MaxCount=0;
                    //文件类别
                    QString fixedType=QString(fixedInfo.at(i));
                    if(fixedType.contains("|",Qt::CaseSensitive)){
                        //建立一个定义变量
                        FIXEDFileDefinition fileDef;
                        //本配置来自的配置文件
                        fileDef.setFileIni(fileName);
                        //分割文件名和版本号
                        QStringList list=fixedType.split("|");
                        //1解析器可接受的文件名
                        fileDef.setFileName(QString (list.at(0)));
                        //2解析器配置全名包含版本号
                        fileDef.setFileNameWithVersion(fixedType);
                        fileDef.setConfigSegment("["+fixedType+"]");
                        //3文件描述信息
                        QString fileDescribe=loadedFixedInfoIni.value(fixedType+"/filedescribe").toString();
                        if(fileDescribe.isEmpty()){
                            fileDef.setFileDescribe("未说明的定长文件");
                        }else{
                            fileDef.setFileDescribe(fileDescribe);
                        }
                        //4编码信息,获取不到则默认为GB18030编码
                        QString encoding=loadedFixedInfoIni.value(fixedType+"/encoding").toString();
                        if(encoding.isEmpty()){
                            fileDef.setEcoding("GB18030");
                        }
                        else{
                            fileDef.setEcoding(encoding);
                        }
                        //5定长分类，字节定长还是字符定长/获取不到定长分配时，则默认为字节定长
                        QString fieldLengthType=loadedFixedInfoIni.value(fixedType+"/fieldlengthtype").toString();
                        if(fieldLengthType.isEmpty()){
                            fileDef.setFieldlengthtype("0");
                        }
                        else{
                            fileDef.setFieldlengthtype(fieldLengthType);
                        }
                        //////////////////////////////////////
                        //6字段总数////存在多种长度的可能
                        QString fieldcount=loadedFixedInfoIni.value(fixedType+"/fieldcount").toString();
                        //找不到每行支持的列数，则无效配置
                        if(fieldcount.isEmpty()){
                            message="配置中找不到每行字段数的配置,无法界定每行的字段数!";
                            fileDef.setUseAble(false);
                            fileDef.setMessage(message);
                            configSegmentList.append(fileDef);
                            continue;
                        }
                        else{
                            //获取字段总数，有可能有多个
                            QStringList fieldCountListStr=fieldcount.split("|");
                            QList<int> fieldCountList;
                            //遍历支持的长度
                            for(int ll=0;ll<fieldCountListStr.count();ll++){
                                if(!QString(fieldCountListStr.at(ll)).isEmpty()){
                                    int fieldCount=(fieldCountListStr.at(ll)).toInt(&flag,10);
                                    //一旦遇到无法识别的字段和，则退出
                                    if(!flag){
                                        message="字段总数描述存在不可用的数值(字段数需为大于0的整数),如果有多种字段数则使用|分开维护";
                                        break;
                                    }
                                    else{
                                        if(fieldCount>MaxCount){
                                            MaxCount=fieldCount;
                                        }
                                        fieldCountList.append(fieldCount);
                                    }
                                }
                            }
                            if(!flag){
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                fileDef.setFieldCountMax(MaxCount);
                                fileDef.setFieldCountList(fieldCountList);
                            }
                        }
                        //7数据起始行
                        int dataRowBeginIndex=loadedFixedInfoIni.value(fixedType+"/datarowbeginindex").toInt(&flag);
                        if(!flag){
                            message="数据起始行标志不是一个可用的数值,数据起始行标志应当是一个大于等于1的整数,代表文件从此行开始就是数据记录了";
                            fileDef.setUseAble(false);
                            fileDef.setMessage(message);
                            configSegmentList.append(fileDef);
                            continue;
                        }
                        else{
                            if(dataRowBeginIndex<1){
                                message="数据起始行标志应当是一个大于等于1的整数,代表文件从此行开始就是数据记录了";
                                flag=false;
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }else {
                                fileDef.setDataRowBeginIndex(dataRowBeginIndex);
                            }
                        }
                        //8数据尾部忽略行
                        QString ignorerow=loadedFixedInfoIni.value(fixedType+"/endignorerow").toString();
                        int endignoreRow;
                        if(ignorerow.isEmpty()){
                            //不配置就不忽略任何行
                            fileDef.setEndIgnoreRow(0);
                        }
                        else{
                            endignoreRow=ignorerow.toInt(&flag);
                            if(!flag){
                                message="文件结尾忽略行数不是一个可用的数值";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                if(endignoreRow<0){
                                    message="文件结尾忽略行数应当是一个大于等于0的整数";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    configSegmentList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setEndIgnoreRow(endignoreRow);
                                }
                            }
                        }
                        //9首行校验
                        QString firstrowcheck=loadedFixedInfoIni.value(fixedType+"/firstrowcheck").toString();
                        if(firstrowcheck.isEmpty()){
                            //不配置就不校验
                            fileDef.setFirstrowcheck("");
                        }
                        else{
                            fileDef.setFirstrowcheck(firstrowcheck);
                        }
                        //10尾行校验
                        QString lastrowcheck=loadedFixedInfoIni.value(fixedType+"/lastrowcheck").toString();
                        if(lastrowcheck.isEmpty()){
                            //不配置就不校验
                            fileDef.setLastrowcheck("");
                        }
                        else{
                            fileDef.setLastrowcheck(lastrowcheck);
                        }
                        //11-版本记录所在行
                        QString versioncheckrow=loadedFixedInfoIni.value(fixedType+"/versioncheckrow").toString();
                        int versioncheckrowint;
                        if(versioncheckrow.isEmpty()){
                            //不配置就是不检验版本
                            fileDef.setVersioncheckrow(0);
                        }
                        else{
                            versioncheckrowint=versioncheckrow.toInt(&flag);
                            if(!flag){
                                message="版本检查行配置versioncheckrow不是一个有效数据";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                if(versioncheckrowint<0){
                                    message="版本检查行配置versioncheckrow应当是一个大于等于0的整数,0为不进行版本检查,大于0则代表进行版本检查";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    configSegmentList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setVersioncheckrow(versioncheckrowint);
                                }
                            }
                        }
                        //12版本标志行
                        QString version=loadedFixedInfoIni.value(fixedType+"/version").toString();
                        if(version.isEmpty()){
                            //不配置就不校验
                            fileDef.setVersion("");
                        }
                        else{
                            fileDef.setVersion(version);
                        }
                        //13-文件记录数所在行
                        QString fieldcountcheckrow=loadedFixedInfoIni.value(fixedType+"/fieldcountcheckrow").toString();
                        int fieldcountcheckrowint;
                        if(fieldcountcheckrow.isEmpty()){
                            //不配置就是不进行字段数检查
                            fileDef.setFieldcountcheckrow(0);
                        }
                        else{
                            fieldcountcheckrowint=fieldcountcheckrow.toInt(&flag);
                            if(!flag){
                                message="字段数检查配置fieldcountcheckrow不是一个有效数据";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                if(fieldcountcheckrowint<0){
                                    message="字段数检查配置fieldcountcheckrow应当是一个大于等于0的整数,0为不进行字段数检查,大于0则代表进行字段数检查";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    configSegmentList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setFieldcountcheckrow(fieldcountcheckrowint);
                                }
                            }
                        }
                        //14-文件内字段列表开始行
                        QString fielddetailcheckbeginrow=loadedFixedInfoIni.value(fixedType+"/fielddetailcheckbeginrow").toString();
                        int fielddetailcheckbeginrowint;
                        if(fielddetailcheckbeginrow.isEmpty()){
                            //不配置就是不检验
                            fileDef.setFielddetailcheckbeginrow(0);
                        }
                        else{
                            fielddetailcheckbeginrowint=fielddetailcheckbeginrow.toInt(&flag);
                            if(!flag){
                                message="文件内字段明细开始行检查配置fielddetailcheckbeginrow不是一个有效数据";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                if(fielddetailcheckbeginrowint<0){
                                    message="文件内字段明细开始行检查配置fielddetailcheckbeginrow应当是一个大于等于0的整数,0为不进行字段检查,大于0则代表进行字段检查";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    configSegmentList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setFielddetailcheckbeginrow(fielddetailcheckbeginrowint);
                                }
                            }
                        }
                        //15-文件内记录数
                        QString rowcountcheckrow=loadedFixedInfoIni.value(fixedType+"/rowcountcheckrow").toString();
                        int rowcountcheckrowint;
                        if(rowcountcheckrow.isEmpty()){
                            //不配置就是不检验
                            fileDef.setRowcountcheckrow(0);
                        }
                        else{
                            rowcountcheckrowint=rowcountcheckrow.toInt(&flag);
                            if(!flag){
                                message="文件内记录数检查配置rowcountcheckrow不是一个有效数据";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                if(rowcountcheckrowint<0){
                                    message="文件内记录数检查配置rowcountcheckrow应当是一个大于等于0的整数,0为不支持记录数,大于0则代表支持字段数检查";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    configSegmentList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setRowcountcheckrow(rowcountcheckrowint);
                                }
                            }
                        }
                        //16-版本号检测模式
                        QString versioncheckmode=loadedFixedInfoIni.value(fixedType+"/versioncheckmode").toString();
                        int versioncheckmodeint;
                        if(versioncheckmode.isEmpty()){
                            fileDef.setVersioncheckmode(0);
                        }
                        else{
                            versioncheckmodeint=versioncheckmode.toInt(&flag);
                            if(!flag){
                                message="版本号检查模式配置versioncheckmode不是一个有效数据";
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                                continue;
                            }
                            else{
                                if(versioncheckmodeint<0){
                                    message="版本检查行配置versioncheckmode应当是一个大于等于0的整数,0为精确匹配,非0则代表模糊匹配";
                                    flag=false;
                                    fileDef.setUseAble(false);
                                    fileDef.setMessage(message);
                                    configSegmentList.append(fileDef);
                                    continue;
                                }else {
                                    fileDef.setVersioncheckmode(versioncheckmodeint);
                                }
                            }
                        }
                        //字段描述值全部正确
                        if(flag){
                            int rowLength=0;
                            bool okFlag=true;
                            //循环遍历最长的字段数值
                            QList <FixedFieldDefinition> fieldList;
                            QHash<int,int> rowLengthHash;
                            //8开始循环加载本文件类型的字段信息///////////////
                            for(int r=0;r<MaxCount;r++){
                                FixedFieldDefinition fixedfield;
                                //获取这个文件定义的第i个字段的信息
                                QStringList iniStrList=loadedFixedInfoIni.value(fixedType+"/"+(QString::number(r+1,10))).toStringList();
                                //至少要四段配置
                                if (iniStrList.isEmpty()||iniStrList.length()<4){
                                    message="第"+QString::number(r+1,10)+"个字段定义不正确或者缺失";
                                    okFlag=false;
                                    break;
                                }
                                //获取字符类型参数
                                QString fieldType=(QString(iniStrList.at(0)));
                                if(fieldType.length()<1){
                                    message="第"+QString::number(r+1,10)+"个字段的类型定义缺失,请配置";
                                    okFlag=false;
                                    break;
                                }
                                //字段长度
                                bool lengthOk;
                                int length=(QString(iniStrList.at(1))).toInt(&lengthOk,10);
                                if(!lengthOk){
                                    message="第"+QString::number(r+1,10)+"个字段的长度定义不是整数";
                                    okFlag=false;
                                    break;
                                }
                                //字段小数长度
                                bool declengthOk;
                                int declength=(QString(iniStrList.at(2))).toInt(&declengthOk,10);
                                if(!declengthOk){
                                    message="第"+QString::number(r+1,10)+"个字段的小数长度定义不是整数";
                                    okFlag=false;
                                    break;
                                }
                                if(fieldType=="N"&&(declength>=length)){
                                    message="第"+QString::number(r+1,10)+"个字段的小数长度定义应该小于字段总长度";
                                    okFlag=false;
                                    break;
                                }
                                //历经千辛万苦经理校验无误后存入字段信息
                                //每个字段开始的位置就是截至到上一个字段结束的长度
                                //比如第一个字段长度为2,则第二个字段在此行记录中的起始坐标就是2（0,1这两个字节是第一个字段的索引）
                                fixedfield.setRowBeginIndex(rowLength);
                                rowLength+=length;
                                fixedfield.setLength(length);
                                fixedfield.setDecLength(declength);
                                fixedfield.setFieldType(fieldType);
                                fixedfield.setFieldDescribe(QString(iniStrList.at(3)));
                                //如果存在英文名赋值
                                if(iniStrList.count()>4){
                                    fixedfield.setFieldName(QString(iniStrList.at(4)));
                                }
                                //否则赋值空字符串
                                else{
                                    fixedfield.setFieldName("");
                                }
                                fieldList.append(fixedfield);
                                //如果存在此长度类型，则记录下行总长度,使用Qhash，行长度，字段数
                                if(fileDef.getFieldCountList().contains(r+1)){
                                    //记录长度和对应的字段数
                                    rowLengthHash.insert(rowLength,r+1);
                                }
                            }
                            //可用
                            if(okFlag){
                                //必填加工
                                loadedFixedInfoIni.beginGroup(fixedType);
                                QStringList infoList =loadedFixedInfoIni.childKeys();
                                loadedFixedInfoIni.endGroup();
                                if(infoList.count()>0){
                                    for (int checkindex=0;checkindex<infoList.count();checkindex++){
                                        //字段必填配置
                                        if(infoList.at(checkindex).startsWith("fieldcheck_")){
                                            bool okFlag=false;
                                            fieldcheckitem ckitem=Utils::parseStringtofieldcheckitem(loadedFixedInfoIni.value(fixedType+"/"+infoList.at(checkindex)).toString(),-1,&okFlag);
                                            if(okFlag){
                                                fileDef.addFieldcheckItem(ckitem);
                                            }
                                        }
                                    }
                                }
                                fileDef.setRowLengthHash(rowLengthHash);
                                fileDef.setFieldList(fieldList);
                                fileDef.setUseAble(true);
                                configSegmentList.append(fileDef);
                            }
                            //不可用
                            else{
                                fileDef.setUseAble(false);
                                fileDef.setMessage(message);
                                configSegmentList.append(fileDef);
                            }
                        }
                        //不可用
                        else{
                            fileDef.setUseAble(false);
                            fileDef.setMessage(message);
                            configSegmentList.append(fileDef);
                        }
                    }
                    //判断是否是字典配置，字典配置是fixed配置文件里的一段特殊配置，使用Dictionary标注
                    else
                    {
                        if(fixedType=="Dictionary"){
                            Dictionary dic;
                            //开始循环读取字典
                            //不要尝试使用begingroup后直接读取组内参数，否则会读取不到包含逗号的键值
                            loadedFixedInfoIni.beginGroup("Dictionary");
                            QStringList infoList =loadedFixedInfoIni.childKeys();
                            loadedFixedInfoIni.endGroup();
                            if(infoList.count()>0){
                                for(int aaa=0;aaa<infoList.count();aaa++){
                                    //ini配置中如果key存在中文会出现错误，所以我们使用1=key@value的方式去改造
                                    QString line=loadedFixedInfoIni.value("Dictionary/"+infoList.at(aaa)).toString();
                                    int index=line.indexOf('@');
                                    if(index>0){
                                        //只替换第一个出现的@，以免替换了字典里的其他@符号
                                        dic.addDictionary(line.replace(index,1,'='));
                                    }
                                    //不包含@的跳过
                                    else{
                                        continue;
                                    }
                                }
                                //将字典加入qhash.FIXED文件的字典以FIXED开头
                                commonDictionary.insert("FIXED"+fileName,dic);
                            }
                            infoList.clear();
                        }
                        //字段Tips加载
                        else if(fixedType=="TipDictionary"){
                            //开始循环读取字典
                            loadedFixedInfoIni.beginGroup("TipDictionary");
                            QStringList infoList =loadedFixedInfoIni.childKeys();
                            loadedFixedInfoIni.endGroup();
                            if(infoList.count()>0){
                                QMap<QString,QString> tips;
                                for(int aaa=0;aaa<infoList.count();aaa++){
                                    //ini配置中如果key存在中文会出现错误，所以我们使用1=key@value的方式去改造
                                    QString line=loadedFixedInfoIni.value("TipDictionary/"+infoList.at(aaa)).toString();
                                    int index=line.indexOf(':');
                                    if(index>0){
                                        tips.insert(line.mid(0,index),line.mid(index+1));
                                    }
                                    //不包含:的跳过
                                    else{
                                        continue;
                                    }
                                }
                                //以文件类别，配置文件名存入
                                commonFieldTips.insert("FIXED"+fileName,tips);
                            }
                            infoList.clear();
                        }
                        else{
                            //无效配置段落，跳过
                            continue;
                        }
                    }
                }
                fixedconfigfile.setConfigSegmentList(configSegmentList);
                fixedConfigList.append(fixedconfigfile);
            }
        }
    }
}


void Utils::load_DBFDefinition(QList<DbfFileConfig> &dbfConfigList,QHash<QString,Dictionary> &commonDictionary,QMap<QString,QMap<QString,QString>> &commonFieldTips){
    QDir dirPath(Utils::getConfigPath());
    QStringList filters;
    filters << "DBF_*.ini";
    dirPath.setNameFilters(filters);//设置文件名的过滤
    QFileInfoList listFile = dirPath.entryInfoList();
    if(listFile.length()!=0){
        //循环检索config目录下可用的FIXED接口定义配置
        for (int f = 0; f < listFile.size(); f++)
        {
            //获取文件名
            QString fileName=listFile.at(f).fileName();
            //获取文件路径
            QString codeInipath=Utils::getConfigPath()+fileName;
            if(Utils::isFileExist(codeInipath)){
                //加载ini文件
                QSettings loadedDBFInfoIni(codeInipath,QSettings::IniFormat,nullptr);
                DbfFileConfig dbfconfig;
                dbfconfig.setFileIniName(fileName);
                //目前仅接收UTF-8编码的配置文件
                loadedDBFInfoIni.setIniCodec("UTF-8");
                QStringList configInfo=loadedDBFInfoIni.childGroups();
                //不包含基础信息和字段信息的配置文件不要
                if(configInfo.count()<1||!configInfo.contains("Global")||!configInfo.contains("Field")){
                    continue;
                }
                //遍历global/////////////////////////////////////////////////////////////////
                QList<FileNameAndDescribe> list;
                loadedDBFInfoIni.beginGroup("Global");
                QStringList infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.contains("matchfilename",Qt::CaseInsensitive)){
                    QString line=loadedDBFInfoIni.value("Global/matchfilename").toString();
                    QStringList names=line.split("|");
                    if(names.count()>0){
                        for(int k=0;k<names.count();k++){
                            QStringList nameanddescribe=names.at(k).split(":");
                            if(nameanddescribe.count()==2){
                                FileNameAndDescribe a;
                                a.fileName=nameanddescribe.at(0);
                                a.fileDescribe=nameanddescribe.at(1);
                                list.append(a);
                            }
                            else{
                                continue;
                            }
                        }
                    }
                    else{
                        continue;
                    }
                }
                else{
                    continue;
                }
                infoList.clear();
                //汇聚结果
                if(list.count()>0){
                    dbfconfig.setSupportFileList(list);
                }
                else{
                    //没有识别到任何有效的文件名支持情况
                    continue;
                }
                //遍历字段配置///////////////////////////////////////////////////////////////////
                loadedDBFInfoIni.beginGroup("Field");
                infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.count()>0){
                    QMap<QString,QString> fieldDescribe;
                    for(int aaa=0;aaa<infoList.count();aaa++){
                        QString line=loadedDBFInfoIni.value("Field/"+infoList.at(aaa)).toString();
                        fieldDescribe.insert(infoList.at(aaa).toUpper(),line);
                    }
                    dbfconfig.setFieldDescribe(fieldDescribe);
                }
                infoList.clear();
                //遍历字典////////////////////////////////////////////////////////////////////////
                Dictionary dic;
                //开始循环读取字典
                loadedDBFInfoIni.beginGroup("Dictionary");
                infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.count()>0){
                    for(int aaa=0;aaa<infoList.count();aaa++){
                        QString line=loadedDBFInfoIni.value("Dictionary/"+infoList.at(aaa)).toString();
                        dic.addDictionary(infoList.at(aaa).toUpper()+"="+line);
                    }
                    commonDictionary.insert("DBF"+fileName,dic);
                }
                infoList.clear();
                //字段Tips加载/////////////////////////////////////////////////////////////////
                loadedDBFInfoIni.beginGroup("TipDictionary");
                infoList =loadedDBFInfoIni.childKeys();
                loadedDBFInfoIni.endGroup();
                if(infoList.count()>0){
                    QMap<QString,QString> tips;
                    for(int aaa=0;aaa<infoList.count();aaa++){
                        QString line=loadedDBFInfoIni.value("TipDictionary/"+infoList.at(aaa)).toString();
                        tips.insert(infoList.at(aaa).toUpper(),line);
                    }
                    //以文件类别，配置文件名存入
                    commonFieldTips.insert("DBF"+fileName,tips);
                }
                infoList.clear();
                //加入配置
                dbfConfigList.append(dbfconfig);
            }
        }
    }
}

QString Utils::qStringTrimLeft(const QString& str) {
    int n = str.size() - 1;
    int i=0;
    for (; i<=n; i++) {
        if (!str.at(i).isSpace()) {
            return str.mid(i);
        }
    }
    return "";
}


QString Utils::getFormatValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int dataCompressLevel,int row ,int col)
{
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getFieldCount()){
        return "";
    }
    //本行数据
    QByteArray rowdata;
    if(dataCompressLevel==0){
        rowdata=ofdFileContentQByteArrayList->at(row);
    }
    else{
        rowdata=qUncompress(ofdFileContentQByteArrayList->at(row));
    }
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

QStringList Utils::getFormatRowValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int dataCompressLevel,int row){
    QStringList rowList;
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
    //判断越界
    if(row<ofdFileContentQByteArrayList->count()){
        //本行数据
        QByteArray rowdata;
        if(dataCompressLevel==0){
            rowdata=ofdFileContentQByteArrayList->at(row);
        }
        else{
            rowdata=qUncompress(ofdFileContentQByteArrayList->at(row));
        }
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

QString Utils::getOriginalValuesFromofdFileContentQByteArrayList(QList<QByteArray> * ofdFileContentQByteArrayList,OFDFileDefinition * ofd,int dataCompressLevel,int row ,int col)
{
    QTextCodec *codecOFD = QTextCodec::codecForName("GB18030");
    //判断越界
    if(row>=ofdFileContentQByteArrayList->count()||col>=ofd->getFieldCount()){
        return "";
    }
    else{
        int fieldlength=ofd->getFieldList().at(col).getLength();
        if(dataCompressLevel==0){
            return codecOFD->toUnicode(ofdFileContentQByteArrayList->at(row).mid(ofd->getFieldList().at(col).getRowBeginIndex(),fieldlength));
        }
        else{
            return codecOFD->toUnicode(qUncompress(ofdFileContentQByteArrayList->at(row)).mid(ofd->getFieldList().at(col).getRowBeginIndex(),fieldlength));
        }
    }
}

QString Utils::getFormatValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int dataCompressLevel,int row ,int col)
{
    QTextCodec *codec=QTextCodec::codecForName(fixed->getEcoding().toLocal8Bit());
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
    QByteArray rowdata;
    if(dataCompressLevel==0){
        rowdata=fixedContentQByteArrayList->at(row);
    }
    else{
        rowdata=qUncompress(fixedContentQByteArrayList->at(row));
    }
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

QStringList Utils::getFormatRowValuesFromfixedFileContentQStringList(QList<QByteArray>  * fixedContentQByteArrayList,FIXEDFileDefinition * fixed,int dataCompressLevel,int row){
    QStringList rowList;
    //判断越界
    if(row<fixedContentQByteArrayList->count()){
        QTextCodec *codec=QTextCodec::codecForName(fixed->getEcoding().toLocal8Bit());
        //获取本行数据
        QByteArray rowdata;
        if(dataCompressLevel==0){
            rowdata=fixedContentQByteArrayList->at(row);
        }
        else{
            rowdata=qUncompress(fixedContentQByteArrayList->at(row));
        }
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

QStringList Utils::getRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> *  csvFileContentQByteArrayList,CsvFileDefinition * csv,int dataCompressLevel,int row){
    //判断越界
    QStringList rowData;
    if(row>=csvFileContentQByteArrayList->count()){
        return rowData;
    }else{
        QTextCodec *codec=QTextCodec::codecForName(csv->getEcoding().toLocal8Bit());
        //带双引号的解析模式下，严格按照双引号内的分隔符不解析的原则,但是不支持长度大于1的分隔符
        if(csv->getClearQuotes()&&csv->getSplit().length()==1){
            QRegExp rx("\\"+csv->getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
            if(dataCompressLevel==0){
                rowData=codec->toUnicode(csvFileContentQByteArrayList->at(row)).split(rx);
            }
            else{
                rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(rx);
            }
        }
        else{
            if(dataCompressLevel==0){
                rowData=codec->toUnicode(csvFileContentQByteArrayList->at(row)).split(csv->getSplit());
            }
            else{
                rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(csv->getSplit());
            }
        }
        //需要处理分隔符
        if(csv->getClearQuotes()&&rowData.count()>0){
            QStringList rowData2;
            for(int i=0;i<rowData.count();i++){
                rowData2.append(clearQuotes(rowData.at(i)));
            }
            rowData=rowData2;
        }
        if(csv->getTrim()&&rowData.count()>0){
            QStringList rowData2;
            for(int i=0;i<rowData.count();i++){
                rowData2.append(rowData.at(i).trimmed());
            }
            rowData=rowData2;
        }
        //缩放处理
        for(int i=0;i<rowData.count()&&i<csv->getFieldList().count();i++){
            //对本字段执行缩放
            if(csv->getFieldList().at(i).getIsNumber()==1&&csv->getFieldList().at(i).getDecimalPointShift()>0){
                QString value=rowData.at(i);
                if(value.isEmpty()){
                    continue;
                }
                bool convertOk=false;
                double valued=value.toDouble(&convertOk);
                //非双精度数值
                if(!convertOk){
                    continue;
                }
                else{
                    int pointShift=csv->getFieldList().at(i).getDecimalPointShift();
                    int decLenght=0;
                    if(value.contains(".")){
                        decLenght=value.split(".").at(1).length();
                    }
                    //计算出新的小数长度
                    decLenght=decLenght+pointShift;
                    switch (pointShift){
                    case 1:
                        valued=valued/10;
                        break;
                    case 2:
                        valued=valued/100;
                        break;
                    case 3:
                        valued=valued/1000;
                        break;
                    case 4:
                        valued=valued/10000;
                        break;
                    default:
                        break;
                    }
                    rowData.replace(i,QString::number(valued,'f', decLenght));
                }
            }
        }
        return rowData;
    }
}

QStringList Utils::getFormatRowValuesFromdbfTableFile(QDbf::QDbfTable * dbftablefile,DbfFileDefinition * dbf,int row,QHash<int,int> * rowMap,bool adddeletedFlag,int trimType){
    int rowintablefile=rowMap->value(row);
    int colCount=dbf->getFieldCount();
    dbftablefile->seek(rowintablefile);
    QStringList list;
    for (int col=0;col<colCount;col++){
        int type=dbf->getFieldList().at(col).getFieldType();
        //NULL值不考虑数据类型直接填充空
        if(dbftablefile->record().field(col).isNull()){
            list.append(nullptr);
        }
        //字符串特殊处理
        else if(type==0){
            switch (trimType){
            case 0:
                list.append(dbftablefile->record().field(col).value().toString().trimmed());
                break;
            case 1:
                list.append(Utils::qStringTrimLeft(dbftablefile->record().field(col).value().toString()));
                break;
            case 2:
                list.append(Utils::qStringTrimRight(dbftablefile->record().field(col).value().toString()));
                break;
            default:
                list.append(dbftablefile->record().field(col).value().toString());
            }
        }
        else if(type==-1||type==4){
            list.append(dbftablefile->record().field(col).value().toString().trimmed());
        }
        //浮点2，数值5，货币型8
        else if(type==2||type==5||type==8){
            int declength=dbf->getFieldList().at(col).getDecLength();
            list.append(QString::number(dbftablefile->record().field(col).value().toDouble(), 'f', declength).trimmed());
        }
        //6整数
        else if(type==6){
            list.append(dbftablefile->record().field(col).value().toString().trimmed());
        }
        //
        else if(type==1||type==7){
            list.append(dbftablefile->record().field(col).value().toString().trimmed());
        }
        //bool型3
        else if(type==3){
            bool vale=dbftablefile->record().field(col).value().toBool();
            if(vale){
                list.append("True");
            }
            else{
                list.append("False");
            }
        }
        //其他类型
        else{
            list.append(dbftablefile->record().field(col).value().toString().trimmed());
        }
    }
    //插入删除标记
    if(adddeletedFlag){
        if(dbftablefile->record().isDeleted()){
            list.append("1");
        }
        else{
            list.append("0");
        }
    }
    return list;
}

QString Utils::getFormatValuesFromdbfTableFile(QDbf::QDbfTable * dbftablefile,DbfFileDefinition * dbf,int row,int col,QHash<int,int> * rowMap,int trimType){
    int rowintablefile=rowMap->value(row);
    int colCount=dbf->getFieldCount();
    dbftablefile->seek(rowintablefile);
    QString value=NULL;
    if(col<colCount){
        int type=dbf->getFieldList().at(col).getFieldType();
        //NULL值不考虑数据类型直接填充空
        if(dbftablefile->record().field(col).isNull()){
            value="";
        }
        //字符串特殊处理
        else if(type==0){
            switch (trimType){
            case 0:
                value=dbftablefile->record().field(col).value().toString().trimmed();
                break;
            case 1:
                value=Utils::qStringTrimLeft(dbftablefile->record().field(col).value().toString());
                break;
            case 2:
                value=Utils::qStringTrimRight(dbftablefile->record().field(col).value().toString());
                break;
            default:
                value=dbftablefile->record().field(col).value().toString();
            }
        }
        else if(type==-1||type==4){
            value=dbftablefile->record().field(col).value().toString().trimmed();
        }
        //浮点2，数值5，货币型8
        else if(type==2||type==5||type==8){
            int declength=dbf->getFieldList().at(col).getDecLength();
            value=QString::number(dbftablefile->record().field(col).value().toDouble(), 'f', declength).trimmed();
        }
        else if(type==6){
            value=dbftablefile->record().field(col).value().toString().trimmed();
        }
        //
        else if(type==1||type==7){
            value=dbftablefile->record().field(col).value().toString().trimmed();
        }
        //bool型3
        else if(type==3){
            bool vale=dbftablefile->record().field(col).value().toBool();
            if(vale){
                value="True";
            }
            else{
                value="False";
            }
        }
        //其他类型
        else{
            value=dbftablefile->record().field(col).value().toString().trimmed();
        }
        return value;
    }
    return "";
}


QStringList Utils::getOriginalRowCsvValuesFromcsvFileContentQStringList(QList<QByteArray> *  csvFileContentQByteArrayList,CsvFileDefinition * csv,int dataCompressLevel,int row){
    //判断越界
    QStringList rowData;
    if(row>=csvFileContentQByteArrayList->count()){
        return rowData;
    }else{
        QTextCodec *codec=QTextCodec::codecForName(csv->getEcoding().toLocal8Bit());
        if(csv->getClearQuotes()&&csv->getSplit().length()==1){
            QRegExp rx("\\"+csv->getSplit()+"(?=([^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*$)");
            if(dataCompressLevel==0){
                rowData=codec->toUnicode(csvFileContentQByteArrayList->at(row)).split(rx);
            }
            else{
                rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(rx);
            }
        }
        else{
            if(dataCompressLevel==0){
                rowData=codec->toUnicode(csvFileContentQByteArrayList->at(row)).split(csv->getSplit());
            }
            else{
                rowData=codec->toUnicode(qUncompress(csvFileContentQByteArrayList->at(row))).split(csv->getSplit());
            }
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
    struct tm tma = {0,0,0,0,0,0,0,0,0};
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


void Utils::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Utils::getFileListFromDirSkipOkfile(QString dirpath,QStringList *filelist){
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
            Utils::getFileListFromDirSkipOkfile(path,filelist);
        }
        else{
            if(!fileInfo.filePath().endsWith(".ok",Qt::CaseInsensitive)){
                filelist->append(fileInfo.filePath());
            }
        }
        i++;
    } while(i < list.size());
    return;
}

fieldcheckitem Utils::parseStringtofieldcheckitem(QString text,int countInt,bool * okFlag){
    QStringList line2=text.split("|");
    fieldcheckitem ckitem;
    bool flagcon=true;
    if(line2.count()==2){
        //分离条件和必填字段列表
        QString condition=line2.at(0);
        QString req=line2.at(1);
        //使用分号分割多个条件分组
        if(condition.toUpper()=="ALL"){
            QList<CheckCondition> list;
            CheckCondition ck;
            //魔法数值-代表本行记录全部检查不分业务
            ck.index=ALL_CHECK_INDEX;
            ck.value="";
            list.append(ck);
            ckitem.addCheckConditions(list);
        }
        else{
            QStringList conditionListOverList=condition.split(";");
            for (QString conditionliststr:conditionListOverList){
                QStringList conditionlist=conditionliststr.split("&");
                if(conditionlist.count()>0){
                    QList<CheckCondition> list;
                    for(int ccc=0;ccc<conditionlist.count();ccc++){
                        QStringList conditionItemList=conditionlist.at(ccc).split("=");
                        if(conditionItemList.count()==2){
                            //index
                            int fieldindex=conditionItemList.at(0).toInt(&flagcon,10);
                            if(flagcon&&fieldindex>0&&countInt>0&&fieldindex<=countInt){
                                CheckCondition ck;
                                ck.index=fieldindex-1;
                                ck.value=conditionItemList.at(1);
                                list.append(ck);
                            }
                            //支持csv自动字段数
                            else if (flagcon&&fieldindex>0&&countInt==-1){
                                CheckCondition ck;
                                ck.index=fieldindex-1;
                                ck.value=conditionItemList.at(1);
                                list.append(ck);
                            }
                            else{
                                //检查条件不准有错，否则就放弃
                                flagcon=false;
                                break;
                            }
                        }
                    }
                    //添加本分组条件--分组内是和的关系，分组之间是或的关系
                    if(flagcon){
                        ckitem.addCheckConditions(list);
                    }
                }
            }
        }
        //如果条件没问题,则开始判断字段列表
        if (flagcon){
            QStringList reqlist=req.split(",");
            if(reqlist.count()<1){
                flagcon=false;
            }
            else{
                QList<int > fieldListforckitem;
                for(int xxx=0;xxx<reqlist.count();xxx++){
                    bool flag=true;
                    int fieldindex=reqlist.at(xxx).toInt(&flag,10);
                    if(flag&&fieldindex>0&&fieldindex<=countInt){
                        fieldListforckitem.append(fieldindex-1);
                    }
                    else{
                        //如果存在多余字段,忽略不加载
                        if(fieldindex>countInt){
                            continue;
                        }
                        //其他错误的不加载
                        else{
                            flagcon=false;
                            break;
                        }

                    }
                }
                //字段检查也符合预期--插入本必填
                if(flagcon){
                    ckitem.setRequiredFields(fieldListforckitem);
                }
            }
        }
        //条件异常
        else{
            flagcon=false;
        }
    }
    if(flagcon){
        * okFlag=true;
    }
    else{
        * okFlag=false;
    }
    return ckitem;
}

QList<QStringList> Utils::parseCheckItemListtoChineseList(OFDFileDefinition &ofd){
    QList<QStringList> data;
    for (fieldcheckitem item: ofd.getFieldcheckList()) {
        QString condListQString;
        QString fieldListQString;
        bool thisconflag=true;
        foreach (auto cclist,item.getCheckConditionsListOverList()){
            //本分组内条件清单--和关系--必须全部满足
            QString cond="";
            thisconflag=true;
            /////////////////////////////条件检查//////////////////
            if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                cond="本文件中所有记录";
            }
            else{
                foreach(auto cc,cclist){
                    if(ofd.getFieldCount()>=cc.index+1){
                        if(cond.isEmpty()){
                            cond.append(ofd.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                        }
                        else{
                            cond.append(",并且").append(ofd.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                        }
                        continue;
                    }
                    //一个条件不满足就不检查了
                    else{
                        thisconflag=false;
                        break;
                    }
                }
                //如果有一个条件异常，则也跳出本条件组-继续下一个
                if(!thisconflag){
                    continue;
                }
            }
            ////////////////////条件构造结束开始拼接条件////////////////
            if(thisconflag){
                if(condListQString.isEmpty()){
                    condListQString=cond;
                }
                else{
                    condListQString.append(";").append(cond);
                }
            }
        }
        //构造条件和字段要求清单
        if(thisconflag){
            foreach(int i,item.getRequiredFields()){
                if(i<ofd.getFieldCount()){
                    if(fieldListQString.isEmpty()){
                        fieldListQString=ofd.getFieldList().at(i).getFieldDescribe();
                    }
                    else{
                        fieldListQString.append(",").append(ofd.getFieldList().at(i).getFieldDescribe());
                    }
                }
                else{
                    thisconflag=false;
                }
            }
            if(thisconflag){
                QStringList line;
                line.append(condListQString);
                line.append(fieldListQString);
                data.append(line);
            }
        }
    }
    return data;
}

QList<QStringList> Utils::parseCheckItemListtoChineseList(CsvFileDefinition &csv){
    QList<QStringList> data;
    foreach(fieldcheckitem item,csv.getFieldcheckList()) {
        QString condListQString;
        QString fieldListQString;
        bool thisconflag=true;
        foreach (auto cclist,item.getCheckConditionsListOverList()){
            //本分组内条件清单--和关系--必须全部满足
            QString cond="";
            thisconflag=true;
            /////////////////////////////条件检查//////////////////
            if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                cond="本文件中所有记录";
            }
            else{
                foreach(auto cc,cclist){
                    if(csv.getFieldCount()>=cc.index+1){
                        if(cond.isEmpty()){
                            cond.append(csv.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                        }
                        else{
                            cond.append(",并且").append(csv.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                        }
                        continue;
                    }
                    //一个条件不满足就不检查了
                    else{
                        thisconflag=false;
                        break;
                    }
                }
                //如果有一个条件异常，则也跳出本条件组-继续下一个
                if(!thisconflag){
                    continue;
                }
            }
            ////////////////////条件构造结束开始拼接条件////////////////
            if(thisconflag){
                if(condListQString.isEmpty()){
                    condListQString=cond;
                }
                else{
                    condListQString.append(";").append(cond);
                }
            }
        }
        //构造条件和字段要求清单
        if(thisconflag){
            foreach(int i,item.getRequiredFields()){
                if(i<csv.getFieldCount()){
                    if(fieldListQString.isEmpty()){
                        fieldListQString=csv.getFieldList().at(i).getFieldDescribe();
                    }
                    else{
                        fieldListQString.append(",").append(csv.getFieldList().at(i).getFieldDescribe());
                    }
                }
                else{
                    thisconflag=false;
                }
            }
            if(thisconflag){
                QStringList line;
                line.append(condListQString);
                line.append(fieldListQString);
                data.append(line);
            }
        }
    }
    return data;
}

QList<QStringList> Utils::parseCheckItemListtoChineseList(FIXEDFileDefinition &fixed){
    QList<QStringList> data;
    for (fieldcheckitem item: fixed.getFieldcheckList()) {
        QString condListQString;
        QString fieldListQString;
        bool thisconflag=true;
        foreach (auto cclist,item.getCheckConditionsListOverList()){
            //本分组内条件清单--和关系--必须全部满足
            QString cond="";
            thisconflag=true;
            /////////////////////////////条件检查//////////////////
            if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                cond="本文件中所有记录";
            }
            else{
                foreach(auto cc,cclist){
                    if(fixed.getFieldList().count()>=cc.index+1){
                        if(cond.isEmpty()){
                            cond.append(fixed.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                        }
                        else{
                            cond.append(",并且").append(fixed.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                        }
                        continue;
                    }
                    //一个条件不满足就不检查了
                    else{
                        thisconflag=false;
                        break;
                    }
                }
                //如果有一个条件异常，则也跳出本条件组-继续下一个
                if(!thisconflag){
                    continue;
                }
            }
            ////////////////////条件构造结束开始拼接条件////////////////
            if(thisconflag){
                if(condListQString.isEmpty()){
                    condListQString=cond;
                }
                else{
                    condListQString.append(";").append(cond);
                }
            }
        }
        //构造条件和字段要求清单
        if(thisconflag){
            foreach(int i,item.getRequiredFields()){
                if(i<fixed.getFieldCountMax()){
                    if(fieldListQString.isEmpty()){
                        fieldListQString=fixed.getFieldList().at(i).getFieldDescribe();
                    }
                    else{
                        fieldListQString.append(",").append(fixed.getFieldList().at(i).getFieldDescribe());
                    }
                }
                else{
                    thisconflag=false;
                }
            }
            if(thisconflag){
                QStringList line;
                line.append(condListQString);
                line.append(fieldListQString);
                data.append(line);
            }
        }
    }
    return data;
}

double Utils::CVCcal (QList<QStringList > list){
    int n = list.count ();
    double sigma = 0, pSigma = 0;
    for (int i = 0; i < n; ++i) {
        double v = list.at (i).count();
        sigma += v;        // sum
        pSigma += v*v;     // 平方和
    }
    sigma /= n;
    return qSqrt((pSigma/n - sigma*sigma))/sigma;
}
