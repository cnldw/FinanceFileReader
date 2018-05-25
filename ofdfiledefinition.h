#ifndef OFDFILEDEFINITION_H
#define OFDFILEDEFINITION_H

#include <QList>
#include <QString>
#include <fielddefinition.h>

class OFDFileDefinition
{

private:

    //配置是否可用
    bool useAble;
    //配置不可用原因
    QString message;
    //字段总和
    int fieldCount;
    //每行长度
    int rowLength;
    //字段信息
    QList <FieldDefinition> fieldList;

public:
    OFDFileDefinition();
    ~OFDFileDefinition();
    bool getuseAble()const;
    QString getMessage()const;
    int getfieldCount()const;
    int getrowLength()const;
    QList <FieldDefinition>  getfieldList()const;
    void setfieldCount(int count);
    void setrowLength(int length);
    void setuseAble(bool useAble);
    void setMessage(QString message);
    void setfieldList(QList <FieldDefinition> list);

};

#endif // OFDFILEDEFINITION_H
