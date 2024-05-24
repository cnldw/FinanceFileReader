/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H
#include <QString>

#define VERSION_V "V1.10.4"
#define AUTHOR_PHONE "MTUyMzg4NzIxMDE="
#define AUTHOR_NAME "NzkzNTU0MjYyQHFxLmNvbSjliJjlvrfkvY0p"
#define AUTHOR_EMAIL "NzkzNTU0MjYyQHFxLmNvbQ=="
#define AUTHOR_INFO "5L2c6ICFOuWImOW+t+S9jSjlubvoiJ7lpYflvbEpCgoyMDE15bm05q+V5Lia5LqO6YOR5bee5aSn5a2mCg=="
#define WEBSITE_INFO "https://www.ffreader.cn/?from=ffreader"
#define MANUAL_INFO "https://www.ffreader.cn/manual?from=ffreader"
//二维码分享功能开关--严格的生产环境限制数据导出时,需要使用限制分享的版本
#define ENABLE_QRCODE true
#define ALL_CHECK_INDEX 999999

//生僻字补充第一个字体
extern QString firstRareCharactersFontGlobal;
//全局UI默认字体
extern QString firstUIFontGlobal;
//全局字号信息
extern QString  fontSizeStand;

//适用于macOS的字体设置
#define UIFontsMacOS "PingFang SC,Simsun,SimSun-ExtB,TH-Sung-TP0,TH-Sung-TP2,Alibaba PuHuiTi 3.0,Alibaba PuHuiTi 3.0 55 Regular L3,Alibaba PuHuiTi 3 55 Regular L3,Alibaba PuHuiTi 3.0 55 Regular,Alibaba PuHuiTi 3 55 Regular,sans-serif"
//适用于Linux的字体设置
#define UIFontsLinux "WenQuanYi Zen Hei,Simsun,SimSun-ExtB,TH-Sung-TP0,TH-Sung-TP2,Alibaba PuHuiTi 3.0,Alibaba PuHuiTi 3.0 55 Regular L3,Alibaba PuHuiTi 3 55 Regular L3,Alibaba PuHuiTi 3.0 55 Regular,Alibaba PuHuiTi 3 55 Regular,sans-serif"
//适用于Windows的字体设置
#define UIFontsWindows "Microsoft YaHei,Simsun,SimSun-ExtB,Alibaba PuHuiTi 3.0,Alibaba PuHuiTi 3.0 55 Regular L3,Alibaba PuHuiTi 3 55 Regular L3,Alibaba PuHuiTi 3.0 55 Regular,Alibaba PuHuiTi 3 55 Regular,TH-Sung-TP0,TH-Sung-TP2,sans-serif"

#endif // PUBLICDEFINE_H
