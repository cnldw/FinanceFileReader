#ifndef CSVFIELDDEFINITION_H
#define CSVFIELDDEFINITION_H
#include <QString>

class CsvFieldDefinition
{
public:
    CsvFieldDefinition();

    QString getFieldName() const;
    void setFieldName(const QString &value);

private:
    QString fieldName;
};

#endif // CSVFIELDDEFINITION_H
