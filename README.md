# FFReader

## 基金数据文件阅读器  
本程序是一个用于解析中国登记结算系统（中登TA）和基金销售机构之间传递的《开放式基金交换协议》文件，以及中国登记算结算系统（中登TA）和基金管理人之间传递的《中国结算开放式基金新版系统管理人数据接口规范(TXT)》文件，以及自建TA和基金销售机构之间的文件（各种OFD）的工具  

本程序具有以下特点  
* 超强的兼容能力,支持常见版本的各种数据文件  
* 跨平台完美运行,支持在Windows、Linux、macOS三种操作系统  
* 超强的数据解析能力,100万笔的交易文件,15s可以解析展示出来  
* 支持各种格式的数据导出，导出到excel、html、csv方便进行数据交换  
* 支持数据编辑保存  
* 支持跨行数据内容比对  
* 支持列模式数据查看,搜索  
* 支持数据合法校验  

## 程序编译指南

* 1：本程序使用Qt/C++开发,推荐使用Qt5.10或者以上版本  
* 2：本程序推荐使用GCC编译（win下可使用 MinGW-W64）,macOS下使用clang编译器  

## 程序界面截图
![截图](https://gitee.com/zzunet/HuangYingPicture/blob/master/screenshots/FFReader/001.png)  
![截图](https://gitee.com/zzunet/HuangYingPicture/blob/master/screenshots/FFReader/002.png)  
![截图](https://gitee.com/zzunet/HuangYingPicture/blob/master/screenshots/FFReader/003.png)  
![截图](https://gitee.com/zzunet/HuangYingPicture/blob/master/screenshots/FFReader/004.png)  
![截图](https://gitee.com/zzunet/HuangYingPicture/blob/master/screenshots/FFReader/005.png)  