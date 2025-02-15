# FFReader

## 金融文件阅读器
本程序是一个用于解析中国登记结算系统（中登TA）和基金销售机构之间传递的《开放式基金交换协议》文件，以及中国登记算结算系统（中登TA）和基金管理人之间传递的《中国结算开放式基金新版系统管理人数据接口规范(TXT)》文件，以及自建TA和基金销售机构之间的文件（各种OFD文件）的工具 

图示为FFReader解析展示固定分隔符文件的例子,FFReader支持自定义的灵活解析规则,可方便拓展各种接口文件的解析
![avatar](https://cdn.ffreader.cn/attachment/20190406/7fd759c48df04b2198ccfff0a14c273b.png)

### 前往下载执行程序

下载链接: <a href="https://www.ffreader.cn/download" target="_blank">FFReader可执行程序下载链接</a>

### 本程序具有以下特点  
* 超强的兼容能力,支持常见版本的各种数据文件(01,02,03,04,05,06,07等等等等文件，通过配置文件配置，极速支持新的OFD文件)  
* 跨平台完美运行,支持在Windows、Linux、macOS三种操作系统  
* 超强的数据解析能力,100万笔的交易文件,15s可以解析展示出来  
* 支持各种格式的数据导出，导出到excel、html、csv方便进行数据交换  
* 支持数据编辑保存  
* 支持跨行数据内容比对  
* 支持列模式数据查看,搜索  
* 支持数据合法校验  
* 支持对OFD数据进行单元格编辑，行删除，行增加，文件合并 

### 本程序使用场景
* 针对国内各公募基金公司的IT运维人员，可以方便的排查开放式基金直销和TA交互的数据的正确性，进行数据预览，开发临时修改删除数据
* 开放式基金销售系统，开放式基金登记过户系统研发人员，用于预览导出的数据
* 公募私募基金公司运营人员，用于分析业务数据，导出excel做统计等

### 程序编译指南

* 本程序使用Qt/C++开发,推荐使用Qt5.10或者以上版本，推荐使用Qt Creator进行开发  
* 本程序推荐Linux下使用GCC编译，win可使用MinGW-W64编译,macOS下使用clang编译器  
* 建议使用C++11规范进行项目开发