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
#include "dictionary.h"

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
