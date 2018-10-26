#ifndef CSVFAULTCAUSE_H
#define CSVFAULTCAUSE_H
#include <QString>

class CsvFaultCause
{
public:
    CsvFaultCause();

    int getConfigIndex() const;
    void setConfigIndex(int value);

    QString getCause() const;
    void setCause(const QString &value);

private:
    int configIndex;
    QString cause;
};

#endif // CSVFAULTCAUSE_H
