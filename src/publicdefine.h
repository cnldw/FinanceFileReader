/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/
#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H
#include <QString>

#define VERSION_V "V1.9.27"
#define AUTHOR_PHONE "MTUyMzg4NzIxMDE="
#define AUTHOR_NAME "NzkzNTU0MjYyQHFxLmNvbSjliJjlvrfkvY0p"
#define AUTHOR_EMAIL "NzkzNTU0MjYyQHFxLmNvbQ=="
#define AUTHOR_INFO "5L2c6ICFOuWImOW+t+S9jSjlubvoiJ7lpYflvbEpCgoyMDE15bm05q+V5Lia5LqO6YOR5bee5aSn5a2mCg=="
#define WEBSITE_INFO "https://www.ffreader.cn/?from=ffreader"
#define MANUAL_INFO "https://www.ffreader.cn/manual?from=ffreader"
//二维码分享功能开关--严格的生产环境限制数据导出时,需要使用限制分享的版本
#define ENABLE_QRCODE true
#define ALL_CHECK_INDEX 999999

//适用于qss的字体大小设置
//macOS和Linux强制设置13px
#define FONTSIZE13 "font-size:13px;"
//放大镜使用的64px
#define FONTSIZE64 "font-size:64px;"

//适用于macOS的字体设置
#define UIFontsMacOS "font-family:\"PingFang SC,Simsun,SimSun-ExtB,TH-Sung-TP2,Alibaba PuHuiTi 3.0,Alibaba PuHuiTi 3.0 55 Regular L3,Alibaba PuHuiTi 3 55 Regular L3,Alibaba PuHuiTi 3.0 55 Regular,Alibaba PuHuiTi 3 55 Regular,sans-serif\";"
//适用于Linux的字体设置
#define UIFontsLinux "font-family:\"WenQuanYi Zen Hei,Simsun,SimSun-ExtB,TH-Sung-TP2,Alibaba PuHuiTi 3.0,Alibaba PuHuiTi 3.0 55 Regular L3,Alibaba PuHuiTi 3 55 Regular L3,Alibaba PuHuiTi 3.0 55 Regular,Alibaba PuHuiTi 3 55 Regular,sans-serif\";"
//适用于Windows的字体设置
#define UIFontsWindows "font-family:\"Microsoft YaHei,Simsun,SimSun-ExtB,Alibaba PuHuiTi 3.0,Alibaba PuHuiTi 3.0 55 Regular L3,Alibaba PuHuiTi 3 55 Regular L3,Alibaba PuHuiTi 3.0 55 Regular,Alibaba PuHuiTi 3 55 Regular,TH-Sung-TP2,sans-serif\";"

#endif // PUBLICDEFINE_H
