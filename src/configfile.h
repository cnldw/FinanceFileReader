/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>

struct matchIndex{
  int configFileIndex=0;
  int configSegmentIndex=0;
};

//dbf专用的匹配信息
struct dbfMatchInfo{
    QString ini;
    int configFileIndex;
    QString matchFileType;
    QString matchfileDescribe;
    float matchDegree;
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
