/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dictionary.h"

Dictionary::Dictionary()
{

}

QString Dictionary::getDictionary(QString name,QString key) const
{
    return hash.value(name).value(key);
}

void Dictionary::addDictionary(QString line)
{
    if(!line.contains("=")){
        return;
    }else{
        int index=line.indexOf('=');
        QString name=line.left(index);
        if(line.length()>index){
            QString dic=line.mid(index+1);
            //处理引号
            //引号开始的去掉引号
            if(dic.startsWith("\"")){
                dic=dic.replace(0,1,"");
            }
            //引号结束的去掉引号
            if(dic.endsWith("\"")){
                dic=dic.replace(dic.length()-1,1,"");
            }
            if(dic.contains(":")){
                QHash<QString,QString>dictionaryItem;
                QList <QString>list=dic.split("|");
                if(list.count()>0){
                    for(int i=0;i<list.count();i++){
                        QString item=list.at(i);
                        if(item.contains(":")){
                            int sp=item.indexOf(':');
                            dictionaryItem.insert(item.left(sp),item.mid(sp+1));
                        }
                    }
                }
                if(dictionaryItem.count()>0){
                    this->hash.insert(name,dictionaryItem);
                }
            }
        }
    }
}

void Dictionary::clearDictionary(){
    this->hash.clear();
}
