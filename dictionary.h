#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QMap>
#include <QString>

class Dictionary
{
public:
    Dictionary();
    QString getDictionary(QString name,QString key) const;
    void addDictionary(QString line);

private:
    QMap <QString,QMap<QString,QString>> map;
};

#endif // DICTIONARY_H
