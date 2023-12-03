/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QHash>
#include <QString>

class Dictionary
{
public:
    Dictionary();
    QString getDictionary(QString name,QString key) const;
    /**
     * @brief addDictionary 增加字典项的方法，一般直接增加一行数据，形如dict=key:value|key:value
     * @param line
     */
    void addDictionary(QString line);
    void clearDictionary();
private:
    QHash <QString,QHash<QString,QString>> hash;
};

#endif // DICTIONARY_H
