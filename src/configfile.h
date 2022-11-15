#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>

struct matchIndex{
  int configFileIndex=0;
  int configSegmentIndex=0;
};
template <typename T>
class ConfigFile {
private:
    QString fileName;
    QString author;
    QList<T> ConfigSegmentList;
public:
    const QString &getFileName() const
    {
        return fileName;
    }

    void setFileName(const QString &newFileName)
    {
        fileName = newFileName;
    }

    const QList<T> getConfigSegmentList() const
    {
        return ConfigSegmentList;
    }

    void setConfigSegmentList(const QList<T> &newConfigSegment)
    {
        ConfigSegmentList = newConfigSegment;
    }

    const QString getAuthor() const
    {
        return author;
    }

    void setAuthor(const QString &newAuthor)
    {
        author = newAuthor;
    }
};


#endif // CONFIGFILE_H
