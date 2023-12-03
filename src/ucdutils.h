/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
// Module: libucd.h
// Creator: visualfc <visualfc@gmail.com>

#ifndef LIBUCD_H
#define LIBUCD_H

#include "src/libucd/src/libucd.h"
#include <QByteArray>
#include <QString>

class LibUcd
{
public:
    LibUcd()
    {
        ucd_init(&t);
    }
    ~LibUcd()
    {
        ucd_clear(&t);
    }
    /**
     * @brief CharsetDetect 字符集检测器，传入一段原始QByteArray，返回编码名字，建议直接从文件流读取文件前xx行数据的字节数组传入此函数进行检测
     * @param data
     * @return
     */
    QString CharsetDetect(const QByteArray &data)
    {
        int r = ucd_parse(&t,data.constData(),data.size());
        ucd_end(&t);
        char name[128] = {0};
        if (r == UCD_RESULT_OK) {
            ucd_results(&t,name,127);
        }
        ucd_reset(&t);
        return QString::fromLocal8Bit(name);
    }
protected:
    ucd_t t;
};


#endif // LIBUCD_H
