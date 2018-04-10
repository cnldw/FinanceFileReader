#include "dictionary.h"
#include <QDebug>
Dictionary::Dictionary()
{

}

QString Dictionary::getDictionary(QString name,QString key) const
{
    return map.value(name).value(key);
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
            if(dic.contains(":")){
                QMap<QString,QString>dictionaryItem;
                QStringList list=dic.split("|");
                if(list.count()>0){
                    for(int i=0;i<list.count();i++){
                        QString item=((QString)list.at(i));
                        if(item.contains(":")){
                            int sp=item.indexOf(':');
                            dictionaryItem.insert(item.left(sp),item.mid(sp+1));
                        }
                    }
                }
                if(dictionaryItem.count()>0){
                    this->map.insert(name,dictionaryItem);
                }
            }
        }
    }
}
