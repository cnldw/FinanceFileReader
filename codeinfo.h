#ifndef CODEINFO_H
#define CODEINFO_H

#include<QString>

class CodeInfo
{
public:
    CodeInfo();
    QString getCode()const;
    QString getVersion()const;
    QString getName()const;
    void setCode(QString name);
    void setVersion(QString version);
    void setName(QString name);

private:
    QString code;
    QString version;
    QString name;
};

#endif // CODEINFO_H
