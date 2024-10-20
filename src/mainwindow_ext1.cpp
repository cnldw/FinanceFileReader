/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
/***
mainwindow.cpp文件中的行数太多了,部分方法实现迁移到该文件中
***/
#include "src/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/dialogshowtablecompareview.h"
#include "src/utils.h"
#include "src/dialogmytip.h"
#include "src/dialogshowimportexcelerror.h"
#include "src/dialogshareqrcode.h"
#include "src/msgtoast.h"
#include "src/dialogmodifyrow.h"
#include "src/dialogmodifycell.h"
#include "src/fieldcheckitem.h"


void MainWindow::init_Others(){
    tips.append("导出数据到Excel,可以使用excel进行强大的筛选、统计、分析...");
    tips.append("导出数据到Csv,可以方便的进行数据交换,导入到别的系统...");
    tips.append("导出数据到Html,可以在浏览器中浏览数据,或者发送给其他人便捷浏览数据...");
    tips.append("输入你要查找的字段中文名或者英文名,点击列跳转可以快速跳到你要查找的字段,避免横着拖动滚动条寻找...");
    tips.append("输入你要直达的行号,点击行跳转,直接跳到你要查找的行...");
    tips.append("双击任意单元格,可以将本行数据以列模式显示,自带字典翻译,任意搜索...");
    tips.append("向上向下搜索,快速找到你要的数据...");
    tips.append("外部编辑文件后,使用重新读取文件功能,或者按F5键,极速重新加载数据...");
    tips.append("右键将任意行加入比对器,找到需要比对的行,再加入比对器,就能打开比对器以比对模式查看各行数据的差异了...");
    tips.append("比对器支持任意多行同时比对哟...");
    tips.append("右键菜单有不少小功能哟...");
    tips.append("数据修改后点击保存,程序会自动创建备份,避免找不回原文件...");
    tips.append("数据修改后点击另存保存,可以覆盖保存或者保存文件到其他位置...");
    tips.append("你可以在"+Utils::getConfigPath()+"目录下进行配置文件修改,可以让工具支持新增的各种OFD/类OFD文件...");
    tips.append("拖放文件到程序主窗口,即可打开文件,又快又省心...");
    tips.append("程序里有一个彩蛋哟,快去找一下...");
    tips.append("小心使用编辑功能,避免造成不可挽回的事故...");
    tips.append("源文件某行数据在解析器的第几行?,试试源文件行跳转功能,一键直达...");
    tips.append("选中某一列的多行数据(按住Crtl后单击需要选择的单元格),或者单击列标题选择本列单元格数据后,可以使用批量编辑功能...");
    tips.append("按Ctrl+G切换视图模式,可以隐藏或者显示文件头信息...");
    tips.append("尝试使用本工具解析各种固定分隔符文件吧，比excel更好的数据展示方式,且支持导出excel...");
    tips.append("创建一个本软件的桌面图标,直接拖放文件到图标上即可打开文件,只需一步即可直达");
    tips.append("选择一个区间的数据，右键复制,可以便捷粘贴到excel中");
    tips.append("对于全是数值的列，选择同一列多行数据时，状态栏会自动对选择的数据进行求和，求平均数");
    tips.append("尝试使用本工具解析各种固定字符/字节长度文件吧，高效解析,且支持导出excel...");
    tips.append("尝试进行设置本程序,可以实现默认拖进文件时打开一个新窗口解析,默认使用精简模式打开程序,你的爱好你做主...");
    tips.append("基金、银行理财领域的OFD/类OFD文件,本程序都支持解析哟...");
    tips.append("需要打开超大文件?,建议在设置里设置开启压缩模式和分页支持,支持的文件大小立即提升到GB级别...");
    tips.append("本程序是业余无偿开发的,如果程序帮助到了你,你可以选择小额捐赠给予支持,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("小额捐赠给予支持,FFReader将获得更好的发展,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("FFReader是一个免费的工具软件,如果程序帮助到了你,你可以选择小额捐赠给予支持,捐赠信息在菜单[帮助-关于程序下]...");
    tips.append("可以使用本程序新建OFD/类OFD文件,以及初始化自己的新建模板...");
    tips.append("导出功能可以分页导出或者导出全部数据,自由选择导出范围,是否使用UTF-8编码...");
    tips.append("在程序主窗口或者查看行详细信息界面,使用Ctrl+Alt+R(command+option+R)可以进行快速截图保存...");
    tips.append("固定分隔符文件可以在任意单元格右键对此列手工设置数据格式,调整为数值或者自定义小数长度哟,方便进行数值统计...");
    tips.append("在设置-首选项里设置默认精简视图,即可默认获得最大化的数据展示空间...");
    tips.append("本程序支持解析证券各类DBF文件,直接解析或者配置解析均可,使用配置获得更好的解析体验...");
    tips.append("需要编辑手工创造OFD/类OFD数据文件？尝试新建文件后导出到Excel编辑，编辑后可以立即导入本程序生成OFD/类OFD数据文件...");
    tips.append("字段必填检查工具可以方便的检查是否有异常的字段值缺失,尝试配置几个检测规则然后使用吧...");
    tips.append("不可见字符检查工具可以方便的检索文件中是否有不可见的零宽字符...");
    tips.append("担心文件存在重复冲突数据？尝试使用主键冲突检查工具检查你的文件吧...");
    tips.append("如果你的文件包含生僻字且显示异常,留意检查是否已安装相关字体补充文件...");
    tips.append("不喜欢默认字体?,你可以在设置-首选项中任意修改你喜欢的字体...");
    tips.append("你可以在设置-首选项中设定字体大小，找到适合你的字体大小吧...");
    tips.append("ZIP格式的压缩包可以尝试直接使用工具自动化解压并解析，快捷方便...");

#ifdef Q_OS_WIN32
    tips.append("同时拖放两个文件到程序主窗口,将使用文件比对插件自动比对两个文件的差异...");
    tips.append("如果你要查看接口文件的原始数据,不妨在附加工具菜单下点击\"在文本编辑器中打开当前文件\"...");
    tips.append("想要比对同一个文件不同版本的差异?试试导出到csv固定分隔符文件后同时把两个文件拖向FFReader,它会问你是否要比对差异...");
    tips.append("禁用压缩功能,可以极大的提高文件加载速度,如果你的电脑内存足够,建议禁用压缩功能(打开2GB的文件大概需要2.5GB内存)...");
    tips.append("如果没有特殊情况,请使用64位FFReader,32位版本的FFReader难以打开超大的文件...");
#endif
    specialCharacter.insert(0X00,"NUL (null):空字符");
    specialCharacter.insert(0X01,"SOH (start of headling):标题开始");
    specialCharacter.insert(0X02,"STX (start of text):正文开始");
    specialCharacter.insert(0X03,"ETX (end of text):正文结束");
    specialCharacter.insert(0X04,"EOT (end of transmission):传输结束");
    specialCharacter.insert(0X05,"ENQ (enquiry):请求");
    specialCharacter.insert(0X06,"ACK (acknowledge):收到通知");
    specialCharacter.insert(0X07,"BEL (bell):响铃");
    specialCharacter.insert(0X08,"BS (backspace):退格");
    specialCharacter.insert(0X09,"HT (horizontal tab):水平制表符");
    specialCharacter.insert(0X0A,"LF (NL line feed new line):换行键");
    specialCharacter.insert(0X0B,"VT (vertical tab):垂直制表符");
    specialCharacter.insert(0X0C,"FF (NP form feed new page):换页键");
    specialCharacter.insert(0X0D,"CR (carriage return):回车键");
    specialCharacter.insert(0X0E,"SO (shift out):不用切换");
    specialCharacter.insert(0X0F,"SI (shift in):启用切换");
    specialCharacter.insert(0X10,"DLE (data link escape):数据链路转义");
    specialCharacter.insert(0X11,"DC1 (device control 1):设备控制1");
    specialCharacter.insert(0X12,"DC2 (device control 2):设备控制2");
    specialCharacter.insert(0X13,"DC3 (device control 3):设备控制3");
    specialCharacter.insert(0X14,"DC4 (device control 4):设备控制4");
    specialCharacter.insert(0X15,"NAK (negative acknowledge):拒绝接收");
    specialCharacter.insert(0X16,"SYN (synchronous idle):同步空闲");
    specialCharacter.insert(0X17,"ETB (end of trans block):传输块结束");
    specialCharacter.insert(0X18,"CAN (cancel):取消");
    specialCharacter.insert(0X19,"EM (end of medium):介质中断");
    specialCharacter.insert(0X1A,"SUB (substitute):替补");
    specialCharacter.insert(0X1B,"ESC (escape):溢出");
    specialCharacter.insert(0X1C,"FS (file separator):文件分割符");
    specialCharacter.insert(0X1D,"GS (group separator):分组符");
    specialCharacter.insert(0X1E,"RS (record separator):记录分离符");
    specialCharacter.insert(0X1F,"US (unit separator):单元分隔符");
    specialCharacter.insert(0X20,"SP (space):空格");
    specialCharacter.insert(0X7F,"DEL (delete):删除");

    //其他不可见或者神奇字符
    specialCharacter.insert(0X2000,"EN QUAD:en四元");
    specialCharacter.insert(0X2001,"EM QUAD");
    specialCharacter.insert(0X2002,"EN SPACE");
    specialCharacter.insert(0X2003,"EM SPACE:Em空间");
    specialCharacter.insert(0X2004,"THREE-PER-EM SPACE:每em三个空格");
    specialCharacter.insert(0X2025,"FOUR-PER-EM SPACE:每em四个空格");
    specialCharacter.insert(0X2026,"SIX-PER-EM SPACE:六个/em空间");
    specialCharacter.insert(0X2027,"FIGURE SPACE:数字空间");
    specialCharacter.insert(0X2028,"PUNCTUATION SPACE:标点符号空间");
    specialCharacter.insert(0X2029,"THIN SPACE:薄空间");
    specialCharacter.insert(0X202A,"HAIR SPACE:发际空间");
    specialCharacter.insert(0X200B,"ZERO WIDTH SPACE:零宽空格");
    specialCharacter.insert(0X200C,"ZERO WIDTH NON-JOINER:零宽不连字");
    specialCharacter.insert(0X200D,"ZERO WIDTH JOINER:零宽连字");
    specialCharacter.insert(0X200E,"LEFT-TO-RIGHT MARK:左至右符号");
    specialCharacter.insert(0X200F,"RIGHT-TO-LEFT MARK:右至左符号");
    specialCharacter.insert(0X2011,"NON-BREAKING HYPHEN:不间断连字符");
    specialCharacter.insert(0X2028,"LINE SEPARATOR:线路分离器");
    specialCharacter.insert(0X2029,"PARAGRAPH SEPARATOR:段落分隔符");
    specialCharacter.insert(0X202A,"LEFT-TO-RIGHT EMBEDDING:从左到右嵌入");
    specialCharacter.insert(0X202B,"RIGHT-TO-LEFT EMBEDDING:从右到左嵌入");
    specialCharacter.insert(0X202C,"POP DIRECTIONAL FORMATTING:弹出方向格式");
    specialCharacter.insert(0X202D,"LEFT-TO-RIGHT OVERRIDE:左右超控");
    specialCharacter.insert(0X202E,"RIGHT-TO-LEFT OVERRIDE:从右向左超控");
    specialCharacter.insert(0X202F,"NARROW NO-BREAK SPACE:窄的不间断空间");
    specialCharacter.insert(0X205F,"MEDIUM MATHEMATICAL SPACE:中等数学空间");
    specialCharacter.insert(0X2060,"WORD JOINER:字连接符");
    specialCharacter.insert(0X2061,"FUNCTION APPLICATION:功能应用");
    specialCharacter.insert(0X2062,"INVISIBLE TIMES:不可见时间");
    specialCharacter.insert(0X2063,"INVISIBLE SEPARATOR:不可见分隔符");
    specialCharacter.insert(0X2064,"INVISIBLE PLUS:不可见加号");
    specialCharacter.insert(0X2066,"LEFT-TO-RIGHT ISOLATE:从左到右隔离");
    specialCharacter.insert(0X2067,"RIGHT-TO-LEFT ISOLATE:从右到左隔离");
    specialCharacter.insert(0X2068,"FIRST STRONG ISOLATE:第一强分离物");
    specialCharacter.insert(0X2069,"POP DIRECTIONAL ISOLATE:流行方向隔离");
    specialCharacter.insert(0X206A,"INHIBIT SYMMETRIC SWAPPING:禁止对称交换");
    specialCharacter.insert(0X206B,"ACTIVATE SYMMETRIC SWAPPING:激活对称交换");
    specialCharacter.insert(0X206C,"INHIBIT ARABIC FORM SHAPING:抑制阿拉伯形式的形成");
    specialCharacter.insert(0X206D,"ACTIVATE ARABIC FORM SHAPING:激活阿拉伯语形式变形");
    specialCharacter.insert(0X206E,"NATIONAL DIGIT SHAPES:国家数字形状");
    specialCharacter.insert(0X206F,"NOMINAL DIGIT SHAPES:名义数字形状");

    /*52个PUA汉字*/
    puaOrDeletedGBKCharacter.insert(0xE82F,"PUA码字符,建议使用正式码【㳠,Unicode码3CE0,GB18030码FE6A】");
    puaOrDeletedGBKCharacter.insert(0xE81A,"PUA码字符,建议使用正式码【㑳,Unicode码3473,GB18030码FE55】");
    puaOrDeletedGBKCharacter.insert(0xE81B,"PUA码字符,建议使用正式码【㑇,Unicode码3447,GB18030码FE56】");
    puaOrDeletedGBKCharacter.insert(0xE81F,"PUA码字符,建议使用正式码【㖞,Unicode码359E,GB18030码FE5A】");
    puaOrDeletedGBKCharacter.insert(0xE821,"PUA码字符,建议使用正式码【㘎,Unicode码360E,GB18030码FE5C】");
    puaOrDeletedGBKCharacter.insert(0xE820,"PUA码字符,建议使用正式码【㘚,Unicode码361A,GB18030码FE5B】");
    puaOrDeletedGBKCharacter.insert(0xE824,"PUA码字符,建议使用正式码【㥮,Unicode码396E,GB18030码FE5F】");
    puaOrDeletedGBKCharacter.insert(0xE825,"PUA码字符,建议使用正式码【㤘,Unicode码3918,GB18030码FE60】");
    puaOrDeletedGBKCharacter.insert(0xE827,"PUA码字符,建议使用正式码【㧏,Unicode码39CF,GB18030码FE62】");
    puaOrDeletedGBKCharacter.insert(0xE828,"PUA码字符,建议使用正式码【㧟,Unicode码39DF,GB18030码FE63】");
    puaOrDeletedGBKCharacter.insert(0xE829,"PUA码字符,建议使用正式码【㩳,Unicode码3A73,GB18030码FE64】");
    puaOrDeletedGBKCharacter.insert(0xE82A,"PUA码字符,建议使用正式码【㧐,Unicode码39D0,GB18030码FE65】");
    puaOrDeletedGBKCharacter.insert(0xE82D,"PUA码字符,建议使用正式码【㭎,Unicode码3B4E,GB18030码FE68】");
    puaOrDeletedGBKCharacter.insert(0xE82E,"PUA码字符,建议使用正式码【㱮,Unicode码3C6E,GB18030码FE69】");
    puaOrDeletedGBKCharacter.insert(0xE834,"PUA码字符,建议使用正式码【䁖,Unicode码4056,GB18030码FE6F】");
    puaOrDeletedGBKCharacter.insert(0xE835,"PUA码字符,建议使用正式码【䅟,Unicode码415F,GB18030码FE70】");
    puaOrDeletedGBKCharacter.insert(0xE837,"PUA码字符,建议使用正式码【䌷,Unicode码4337,GB18030码FE72】");
    puaOrDeletedGBKCharacter.insert(0xE83C,"PUA码字符,建议使用正式码【䎱,Unicode码43B1,GB18030码FE77】");
    puaOrDeletedGBKCharacter.insert(0xE83D,"PUA码字符,建议使用正式码【䎬,Unicode码43AC,GB18030码FE78】");
    puaOrDeletedGBKCharacter.insert(0xE83F,"PUA码字符,建议使用正式码【䏝,Unicode码43DD,GB18030码FE7A】");
    puaOrDeletedGBKCharacter.insert(0xE840,"PUA码字符,建议使用正式码【䓖,Unicode码44D6,GB18030码FE7B】");
    puaOrDeletedGBKCharacter.insert(0xE841,"PUA码字符,建议使用正式码【䙡,Unicode码4661,GB18030码FE7C】");
    puaOrDeletedGBKCharacter.insert(0xE842,"PUA码字符,建议使用正式码【䙌,Unicode码464C,GB18030码FE7D】");
    puaOrDeletedGBKCharacter.insert(0xE844,"PUA码字符,建议使用正式码【䜣,Unicode码4723,GB18030码FE80】");
    puaOrDeletedGBKCharacter.insert(0xE845,"PUA码字符,建议使用正式码【䜩,Unicode码4729,GB18030码FE81】");
    puaOrDeletedGBKCharacter.insert(0xE846,"PUA码字符,建议使用正式码【䝼,Unicode码477C,GB18030码FE82】");
    puaOrDeletedGBKCharacter.insert(0xE847,"PUA码字符,建议使用正式码【䞍,Unicode码478D,GB18030码FE83】");
    puaOrDeletedGBKCharacter.insert(0xE849,"PUA码字符,建议使用正式码【䥇,Unicode码4947,GB18030码FE85】");
    puaOrDeletedGBKCharacter.insert(0xE84C,"PUA码字符,建议使用正式码【䦂,Unicode码4982,GB18030码FE88】");
    puaOrDeletedGBKCharacter.insert(0xE84A,"PUA码字符,建议使用正式码【䥺,Unicode码497A,GB18030码FE86】");
    puaOrDeletedGBKCharacter.insert(0xE84B,"PUA码字符,建议使用正式码【䥽,Unicode码497D,GB18030码FE87】");
    puaOrDeletedGBKCharacter.insert(0xE84D,"PUA码字符,建议使用正式码【䦃,Unicode码4983,GB18030码FE89】");
    puaOrDeletedGBKCharacter.insert(0xE84E,"PUA码字符,建议使用正式码【䦅,Unicode码4985,GB18030码FE8A】");
    puaOrDeletedGBKCharacter.insert(0xE84F,"PUA码字符,建议使用正式码【䦆,Unicode码4986,GB18030码FE8B】");
    puaOrDeletedGBKCharacter.insert(0xE853,"PUA码字符,建议使用正式码【䦶,Unicode码49B3,GB18030码8234A038】");
    puaOrDeletedGBKCharacter.insert(0xE850,"PUA码字符,建议使用正式码【䦟,Unicode码499F,GB18030码FE8C】");
    puaOrDeletedGBKCharacter.insert(0xE852,"PUA码字符,建议使用正式码【䦷,Unicode码49B7,GB18030码FE8E】");
    puaOrDeletedGBKCharacter.insert(0xE851,"PUA码字符,建议使用正式码【䦛,Unicode码499B,GB18030码FE8D】");
    puaOrDeletedGBKCharacter.insert(0xE85A,"PUA码字符,建议使用正式码【䱷,Unicode码4C77,GB18030码FE96】");
    puaOrDeletedGBKCharacter.insert(0xE857,"PUA码字符,建议使用正式码【䲟,Unicode码4C9F,GB18030码FE93】");
    puaOrDeletedGBKCharacter.insert(0xE858,"PUA码字符,建议使用正式码【䲠,Unicode码4CA0,GB18030码FE94】");
    puaOrDeletedGBKCharacter.insert(0xE859,"PUA码字符,建议使用正式码【䲡,Unicode码4CA1,GB18030码FE95】");
    puaOrDeletedGBKCharacter.insert(0xE856,"PUA码字符,建议使用正式码【䲣,Unicode码4CA3,GB18030码FE92】");
    puaOrDeletedGBKCharacter.insert(0xE85B,"PUA码字符,建议使用正式码【䲢,Unicode码4CA2,GB18030码FE97】");
    puaOrDeletedGBKCharacter.insert(0xE85C,"PUA码字符,建议使用正式码【䴓,Unicode码4D13,GB18030码FE98】");
    puaOrDeletedGBKCharacter.insert(0xE85D,"PUA码字符,建议使用正式码【䴔,Unicode码4D14,GB18030码FE99】");
    puaOrDeletedGBKCharacter.insert(0xE85E,"PUA码字符,建议使用正式码【䴕,Unicode码4D15,GB18030码FE9A】");
    puaOrDeletedGBKCharacter.insert(0xE85F,"PUA码字符,建议使用正式码【䴖,Unicode码4D16,GB18030码FE9B】");
    puaOrDeletedGBKCharacter.insert(0xE860,"PUA码字符,建议使用正式码【䴗,Unicode码4D17,GB18030码FE9C】");
    puaOrDeletedGBKCharacter.insert(0xE861,"PUA码字符,建议使用正式码【䴘,Unicode码4D18,GB18030码FE9D】");
    puaOrDeletedGBKCharacter.insert(0xE862,"PUA码字符,建议使用正式码【䴙,Unicode码4D19,GB18030码FE9E】");
    puaOrDeletedGBKCharacter.insert(0xE863,"PUA码字符,建议使用正式码【䶮,Unicode码4DAE,GB18030码FE9F】");
    //另外一批PUA双码字,这一批不怎么常用
    puaOrDeletedGBKCharacter.insert(0xE26E,"PUA码字符,建议使用正式码【𫢸,Unicode码2B8B8,GB18030码9839C534】");
    puaOrDeletedGBKCharacter.insert(0xE278,"PUA码字符,建议使用正式码【𪟝,Unicode码2A7DD,GB18030码98368F39】");
    puaOrDeletedGBKCharacter.insert(0xE284,"PUA码字符,建议使用正式码【𫭟,Unicode码2BB5F,GB18030码99308B33】");
    puaOrDeletedGBKCharacter.insert(0xE295,"PUA码字符,建议使用正式码【鿍,Unicode码9FCD,GB18030码82359332】");
    puaOrDeletedGBKCharacter.insert(0xE297,"PUA码字符,建议使用正式码【𫭼,Unicode码2BB7C,GB18030码99308E32】");
    puaOrDeletedGBKCharacter.insert(0xE2A5,"PUA码字符,建议使用正式码【𫮃,Unicode码2BB83,GB18030码99308E39】");
    puaOrDeletedGBKCharacter.insert(0xE2BA,"PUA码字符,建议使用正式码【𪤗,Unicode码2A917,GB18030码9836AF33】");
    puaOrDeletedGBKCharacter.insert(0xE2C8,"PUA码字符,建议使用正式码【𫰛,Unicode码2BC1B,GB18030码99309E31】");
    /*10个竖排标点和8个汉字构件*/
    puaOrDeletedGBKCharacter.insert(0xE7C7,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE78D,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE78E,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE78F,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE790,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE791,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE792,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE793,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE794,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE795,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE796,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE81E,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE826,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE82B,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE82C,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE832,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE843,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE854,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE864,"PUA码字符");

    /*6个汉字构件*/
    puaOrDeletedGBKCharacter.insert(0xE816,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE817,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE818,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE831,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE83B,"PUA码字符");
    puaOrDeletedGBKCharacter.insert(0xE855,"PUA码字符");
}

void MainWindow::getConfigFileChange(bool flag,QStringList modifyConfigList){
    configFileChange=flag;
    modifyConfigQStringList.clear();
    modifyConfigQStringList=modifyConfigList;
}

void MainWindow::getSignalsClosePrimaryCheck(){
    qDebug()<<"接收到信号,关闭主键冲突弹窗";
    dialogDialogShowPrimaryCheck->hide();
    dialogDialogShowPrimaryCheck->close();
    dialogDialogShowPrimaryCheck=nullptr;
}

/**
 * @brief MainWindow::columnJump 列左右跳转功能，供点击列左右跳转时使用
 * @param type
 */
void MainWindow::columnJump(int type){
    QString text=ui->lineTextText_2->text();
    int col=tableColSearch;
    if(updateOrBlcokCheck()){
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    //数据类型插入点
    if(currentOpenFileType==openFileType::OFDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD/类OFD文件没有数据记录...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                    ptr_table->setFocus();
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(ofd.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||ofd.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        ptr_table->setFocus();
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                    ptr_table->setFocus();
                }
                col-=1;
                for(;col>=0;col--){
                    if(ofd.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||ofd.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        ptr_table->setFocus();
                        return;
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::CSVFile){
        if(commonContentQByteArrayList.size()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                }
                col-=1;
                for(;col>=0;col--){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        return;
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::FIXEDFile){
        if(commonContentQByteArrayList.size()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(fixed.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||fixed.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                }
                col-=1;
                for(;col>=0;col--){
                    if(fixed.getFieldList().at(col).getFieldDescribe().contains(text,Qt::CaseInsensitive)||fixed.getFieldList().at(col).getFieldName().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        return;
                    }
                }
            }
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==0){
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的DBF文件没有数据记录...");
                return;
            }
        }
        else{
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的DBF文件在当前模式下没有数据记录...");
                return;
            }
        }
        dataBlocked=true;
        dataBlockedMessage="正在进行列搜索,请稍后再操作...";
        if(!text.isEmpty()&&ptr_table->columnCount()>0&&ptr_table->rowCount()>0){
            //向右搜索
            if(type==1){
                if(col>=(ptr_table->columnCount()-1)){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在最后一列了,试试向左搜索...");
                }
                col+=1;
                for(;col<ptr_table->columnCount();col++){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到最后一列还没找到
                    else if(col>=(ptr_table->columnCount()-1)){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往右没有你要搜索的列了...");
                        return;
                    }
                }
            }
            //向左搜索
            else if(type==0){
                if(col<=0){
                    dataBlocked=false;
                    ptr_table->setFocus();
                    statusBar_disPlayMessage("已经在第一列了,试试向右搜索...");
                }
                col-=1;
                for(;col>=0;col--){
                    if(ptr_table->horizontalHeaderItem(col)->text().contains(text,Qt::CaseInsensitive)){
                        ptr_table->setCurrentCell(tableRowCurrent,col);
                        ptr_table->setFocus();
                        dataBlocked=false;
                        on_tableWidget_itemSelectionChanged();
                        return;
                    }
                    //搜到第一列还没找到
                    else if(col<=0){
                        dataBlocked=false;
                        ptr_table->setFocus();
                        statusBar_disPlayMessage("再往左没有你要搜索的列了...");
                        return;
                    }
                }
            }
        }
    }
    dataBlocked=false;
}

void MainWindow::setStandFontSize(){
#ifdef Q_OS_MAC
    if(standFontSize==1){
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(14));
    }
    else if(standFontSize==2){
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(15));
    }
    else{
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(13));
    }
#endif

#ifdef Q_OS_LINUX
    if(standFontSize==1){
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(14));
    }
    else if(standFontSize==2){
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(15));
    }
    else{
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(13));
    }
#endif

#ifdef Q_OS_WIN32
    if(standFontSize==1){
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(13));
    }
    else if(standFontSize==2){
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(14));
    }
    else{
        fontSizeStand=QString("font-size:%1px;").arg(QString::number(12));
    }
#endif

}

/**
 * @brief MainWindow::pageJump 跳页函数
 * @param page
 * @param scrollIndex 滚动位置,-1滚动到页尾部,-2滚动到页首
 */

void MainWindow::pageJump(int page,int scrollIndex){
    if(page>pageCount){
        statusBar_disPlayMessage("页面计算错误,请检查");
        return;
    }
    else{
        currentPage=page;
    }
    //按钮禁用逻辑
    if(page==1){
        ui->pushButtonPageFirst->setEnabled(false);
        ui->pushButtonPagePrevious->setEnabled(false);
        ui->pushButtonPageNext->setEnabled(true);
        ui->pushButtonPageLast->setEnabled(true);
    }
    else if (page==pageCount){
        ui->pushButtonPageFirst->setEnabled(true);
        ui->pushButtonPagePrevious->setEnabled(true);
        ui->pushButtonPageNext->setEnabled(false);
        ui->pushButtonPageLast->setEnabled(false);
    }
    else{
        ui->pushButtonPageFirst->setEnabled(true);
        ui->pushButtonPagePrevious->setEnabled(true);
        ui->pushButtonPageNext->setEnabled(true);
        ui->pushButtonPageLast->setEnabled(true);
    }
    //计算本页行数
    int currentPageRowSize=0;
    if(currentPage<pageCount){
        currentPageRowSize=pageRowSize;
    }
    else{
        //数据类型插入点
        if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile){
            currentPageRowSize=commonContentQByteArrayList.count()-(pageRowSize*(pageCount-1));
        }
        else if(currentOpenFileType==openFileType::DBFFile){
            currentPageRowSize=dbfRowMap.count()-(pageRowSize*(pageCount-1));
        }
    }
    //清空表格内容和加载记录
    clear_Table_Contents();
    rowHasloaded.clear();
    //重设表格行数
    ptr_table->setRowCount(currentPageRowSize);
    //显示行号
    ui->pageText->setText(QString::number(page));
    //显示描述
    statusBar_disPlayMessage(QString("第%1页,本页记录数%2行").arg(QString::number(page),QString::number(currentPageRowSize)));
    //加载本页数据
    //数据类型插入点
    if(currentOpenFileType==openFileType::OFDFile){
        display_OFDTable();
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        display_CSVTable();
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        display_FIXEDTable();
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        display_DBFTable();
    }
    //跳转位置
    if(scrollIndex==-1){
        ptr_table->scrollToBottom();
        //强制触发下刷新，避免显示数据不完整
        acceptVScrollValueChanged(0);
        tableRowCurrent=ptr_table->rowCount()-1;
        tableColCurrent=ptr_table->columnCount()-1;
    }
    else if(scrollIndex==-2){
        ptr_table->scrollToTop();
        acceptVScrollValueChanged(0);
        tableRowCurrent=0;
        tableColCurrent=0;
    }
    else if(scrollIndex<currentPageRowSize&&scrollIndex>=0){
        if(scrollIndex==currentPageRowSize-1){
            ptr_table->scrollToBottom();
        }
        else if(scrollIndex==0){
            ptr_table->scrollToTop();
        }
        ptr_table->setCurrentCell(scrollIndex,0);
        acceptVScrollValueChanged(0);
        tableRowCurrent=scrollIndex;
        tableColCurrent=0;
    }
    else if(scrollIndex>currentPageRowSize){
        ptr_table->scrollToBottom();
        ptr_table->setCurrentCell(currentPageRowSize-1,0);
        acceptVScrollValueChanged(0);
        tableRowCurrent=currentPageRowSize-1;
        tableColCurrent=0;
    }
}

bool MainWindow::dbfColIsNumber(int col){
    bool s=false;
    if(currentOpenFileType==openFileType::DBFFile){
        if(col<dbf.getFieldCount()){
            if(dbfIsNumberFieldType.contains(dbf.getFieldList().at(col).getFieldType())){
                s=true;
            }
        }
    }
    return s;
}

void MainWindow::reCalculateFileCountLine(){
    if(currentOpenFileType==openFileType::OFDFile&&commonHeaderQStringList.count()>0){
        commonHeaderQStringList.replace(commonHeaderQStringList.count()-1,QString("%1").arg(commonContentQByteArrayList.count(), ofd.getRowCountLength(), 10, QLatin1Char('0')));
    }
    else if(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0){
        if(fixed.getRowcountcheckrow()>0&&fixed.getRowcountcheckrow()<=commonHeaderQStringList.count()){
            QString lineCount=commonHeaderQStringList.at(fixed.getRowcountcheckrow()-1);
            bool okFlag=true;
            lineCount.toDouble(&okFlag);
            if(okFlag){
                int lineLength=lineCount.length();
                if(lineCount.startsWith("0")&&lineLength>=8){
                    commonHeaderQStringList.replace(fixed.getRowcountcheckrow()-1,QString("%1").arg(commonContentQByteArrayList.count(),lineLength,10,QLatin1Char('0')));
                }
                else{
                    commonHeaderQStringList.replace(fixed.getRowcountcheckrow()-1,QString("%1").arg(QString::number(commonContentQByteArrayList.count())));
                }
            }
        }
    }
}

void MainWindow::invisibleCharactersCheckUp(int rangeType){
    if(updateOrBlcokCheck()){
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    //数据类型加入点
    if((currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile)&&commonContentQByteArrayList.count()<1){
        statusBar_disPlayMessage("打开的文件没有数据记录...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfDisplayType==0){
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的文件没有数据记录...");
                return;
            }
        }
        else{
            if(dbfRowMap.count()<1){
                statusBar_disPlayMessage("打开的文件在当前模式下没有数据记录...");
                return;
            }
        }
    }
    if(tableRowCurrent==0&&tableColCurrent==0){
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        statusBar_disPlayMessage("已经搜索到第一个单元格了,请向下搜索...");
        return;
    }
    dataBlocked=true;
    int oldCurrentPage=currentPage;
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->actioninvisibleCharactersCheck1->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    int beginCol=tableColCurrent-1;
    int beginRow=tableRowCurrent;
    int colCount=ptr_table->columnCount();
    int rowRealInContent=0;
    while(currentPage>0){
        for(int row=beginRow;row>=0;row--){
            rowRealInContent=(currentPage-1)*pageRowSize+row;
            QStringList rowdata;
            if(currentOpenFileType==openFileType::OFDFile){
                rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            if(rowdata.count()==0){
                beginCol=colCount-1;
                continue;
            }
            for(int col=rowdata.count()-1>=beginCol?beginCol:rowdata.count()-1;col>=0;col--){
                if(rangeType==0){
                    if(invisibleCharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck1->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("第%1行\"%2\"有不可见字符,右键使用\"十六进制字符编码透视(乱码分析)\"查看详情...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
                else{
                    if(invisibleCharactersCheck(rowdata.at(col))||invisibleASCIICharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck1->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("第%1行\"%2\"有不可见字符(含空格和制表符等ASCII码),右键使用\"十六进制字符编码透视(乱码分析)\"查看详情...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
            }
            beginCol=colCount-1;
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                qApp->processEvents();
                if(abortAndExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        if(currentPage>1){
            currentPage--;
            pageJump(currentPage,-1);
            beginCol=ptr_table->columnCount()-1;
            beginRow=ptr_table->rowCount()-1;
        }
        else{
            dataBlocked=false;
            ui->actioninvisibleCharactersCheck1->setEnabled(true);
            if(oldCurrentPage==currentPage){
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            else{
                ptr_table->scrollToTop();
                tableRowCurrent=0;
                tableColCurrent=0;
                ptr_table->setCurrentCell(0,0);
            }
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            if(rangeType==0){
                statusBar_disPlayMessage("再往上没有不可见字符了,请向下搜索...");
            }
            else{
                statusBar_disPlayMessage("再往上没有不可见字符(含空格和制表符等ASCII码)了,请向下搜索...");
            }
            break;
        }
    }
}

void MainWindow::invisibleCharactersCheckDown(int rangeType){
    if(updateOrBlcokCheck()){
        return;
    }
    if(currentOpenFileType==openFileType::NotFileOrErr){return;}
    if(currentOpenFileType==openFileType::OFDIndex){statusBar_disPlayMessage("不支持索引文件使用此功能...");return;}
    int colCount=ptr_table->columnCount();
    int tableColCount=ptr_table->columnCount();
    //数据类型加入点
    if((currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile)&&commonContentQByteArrayList.count()<1){
        statusBar_disPlayMessage("打开的文件没有数据记录...");
        return;
    }
    if(currentOpenFileType==openFileType::DBFFile){
        if(dbfRowMap.count()<1){
            statusBar_disPlayMessage("打开的文件没有数据记录...");
            return;
        }
    }
    if(tableRowCurrent==ptr_table->rowCount()-1&&tableColCurrent==colCount-1){
        ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
        ptr_table->setFocus();
        statusBar_disPlayMessage("已经搜索到最后一个单元格了,请向上搜索...");
        return;
    }
    dataBlocked=true;
    int oldCurrentPage=currentPage;
    dataBlockedMessage="正在搜索,请稍后再操作...";
    ui->actioninvisibleCharactersCheck2->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    int currentPageRowCount=ptr_table->rowCount();
    int beginCol=tableColCurrent+1;
    int beginRow=tableRowCurrent;
    int rowRealInContent=0;
    while(currentPage<=pageCount){
        for(int row=beginRow;row<currentPageRowCount;row++){
            rowRealInContent=(currentPage-1)*pageRowSize+row;
            QStringList rowdata;
            if(currentOpenFileType==openFileType::OFDFile){
                rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::CSVFile){
                rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::FIXEDFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            else if(currentOpenFileType==openFileType::DBFFile){
                rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            }
            if(rowdata.count()==tableColCount){
                colCount=tableColCount;
            }
            else if(rowdata.count()==0){
                beginCol=0;
                continue;
            }
            else if(rowdata.count()<colCount){
                colCount=rowdata.count();
            }
            for(int col=beginCol;col<colCount;col++){
                if(rangeType==0){
                    if(invisibleCharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck2->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("在第%1行\"%2\"搜索到不可见字符,在单元格右键使用\"十六进制字符编码透视(乱码分析)\"进行详细分析...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
                else{
                    if(invisibleCharactersCheck(rowdata.at(col))||invisibleASCIICharactersCheck(rowdata.at(col))){
                        ptr_table->setCurrentCell(row,col);
                        ptr_table->setFocus();
                        ui->actioninvisibleCharactersCheck2->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        dataBlocked=false;
                        reCalculateTableBeginAndEnd();
                        on_tableWidget_itemSelectionChanged();
                        QString message=QString("在第%1行\"%2\"搜索到不可见字符(含空格和制表符等ASCII码),在单元格右键使用\"十六进制字符编码透视(乱码分析)\"进行详细分析...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(col)->text());
                        statusBar_disPlayMessage(message);
                        return;
                    }
                }
            }
            beginCol=0;
            if(row%100==0){
                statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                qApp->processEvents();
                if(abortAndExit){
                    QApplication::restoreOverrideCursor();
                    qDebug()<<"强制立即终止任务";
                    return;
                }
            }
        }
        if(currentPage<pageCount){
            currentPage++;
            pageJump(currentPage,-2);
            beginCol=0;
            beginRow=0;
            currentPageRowCount=ptr_table->rowCount();
        }
        else{
            dataBlocked=false;
            ui->actioninvisibleCharactersCheck2->setEnabled(true);
            if(oldCurrentPage==currentPage){
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            else{
                ptr_table->scrollToBottom();
                tableRowCurrent=currentPageRowCount-1;
                tableColCurrent=colCount-1;
                ptr_table->setCurrentCell(tableRowCurrent,tableColCurrent);
            }
            ptr_table->setFocus();
            QApplication::restoreOverrideCursor();
            if(rangeType==0){
                statusBar_disPlayMessage("再往下没有不可见字符了,请向上搜索...");
            }
            else{
                statusBar_disPlayMessage("再往下没有不可见字符(含空格和制表符等ASCII码)了,请向上搜索...");
            }
            break;
        }
    }
}

void MainWindow::openPlugin(){
#ifdef Q_OS_WIN32
    QObject *object = QObject::sender();
    qDebug()<<"准备打开插件"<<object->objectName();
    QString pluginName=object->objectName().split("|").at(0);
    int pluginIndex=object->objectName().split("|").at(1).toInt();
    FormWebTools *web=new FormWebTools(pluginName,QApplication::applicationDirPath()+"/plugin-web/"+pluginpath.value(pluginIndex));
    web->setAttribute(Qt::WA_DeleteOnClose);
    web->show();
    web->raise();
    web->activateWindow();
#endif
}

void MainWindow::loadConfig(bool isReload){
    configLoadCompleted=false;
    //清理之前加载的数据
    commonDictionary.clear();
    commonFieldTips.clear();

    ofdDictionary.clearDictionary();

    loadedOfdCodeInfo.clear();
    loadedOfdIndexFileInfo.clear();
    likeOFDIndexFilename.clear();
    likeOFDDataFilename.clear();
    ofdConfigList.clear();
    ofdQuickMatchIndex.clear();

    csvConfigList.clear();
    fixedConfigList.clear();
    dbfConfigList.clear();

    //加载基金销售商和TA代码信息
    Utils::load_OFDCodeInfo(loadedOfdCodeInfo);
    qApp->processEvents();
    //加载OFD字典
    Utils::load_OFDDictionary(ofdDictionary);
    qApp->processEvents();
    //加载OFD tips字典
    Utils::load_OFDTipDictionary(commonFieldTips);
    qApp->processEvents();
    //加载OFD文件类别信息
    Utils::load_OFDIndexFileDefinition(loadedOfdIndexFileInfo);
    qApp->processEvents();
    //加载类OFD体系的文件的文件名前缀信息
    Utils::load_OFDLikeFileDefinition(likeOFDIndexFilename,likeOFDDataFilename);
    qApp->processEvents();
    //加载各类OFD文件的定义
    Utils::load_OFDDefinition(ofdConfigList,ofdQuickMatchIndex);
    qApp->processEvents();
    //加载各类CSV文件的定义
    Utils::load_CSVDefinition(csvConfigList,commonDictionary,commonFieldTips);
    qApp->processEvents();
    //加载各类定长文件的定义
    Utils::load_FIXEDDefinition(fixedConfigList,commonDictionary,commonFieldTips);
    qApp->processEvents();
    //加载DBF配置
    Utils::load_DBFDefinition(dbfConfigList,commonDictionary,commonFieldTips);
    qApp->processEvents();
    //配置加载完毕
    configLoadCompleted=true;
    if(isReload){
        if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::OFDIndex||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
            qDebug()<<modifyConfigQStringList;
            //如果修改了当前正在打开的文件所在的ini
            if(modifyConfigQStringList.contains(ui->lineEditUseIni->text())){
                statusBar_disPlayMessage("配置文件发生了更新,F5刷新加载当前打开的文件,以使用最新的配置解析...");
                Toast::showMsg(QString("配置文件发生了更新,F5刷新加载当前打开的文件,以使用最新的配置解析..."), ToastTime::Time::ToastTime_long,ToastType::Type::ToastType_info,this);
            }
        }
    }
}

void MainWindow::getActionFromPrimaryCheckTools(int type,int row1,int row2){
    if(needReCheckPrimary){
        statusBar_disPlayMessage("数据可能有变化,需要重新执行主键冲突检查才能使用此功能...");
    }
    else{
        //跳到某行
        if(type==1){
            int  lineNumber=row1;
            if(lineNumber<1){
                statusBar_disPlayMessage(tr("行号不能小于1"));
            }
            else{
                //数据类型加入点
                int targetIndex=0;
                if(lineNumber>commonContentQByteArrayList.count()){
                    statusBar_disPlayMessage(tr("无此行,行号超出了数据最大行数值"));
                    return;
                }
                else{
                    targetIndex=lineNumber-1;
                }
                //判断是否在当前页，是否需要跳行
                int targetPage=(targetIndex/pageRowSize)+1;
                int targetTableIndex=(targetIndex%pageRowSize);
                if(targetPage==currentPage){
                    ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
                    ptr_table->setFocus();
                }
                else{
                    pageJump(targetPage,targetTableIndex);
                    ptr_table->setCurrentCell(targetTableIndex,tableColCurrent);
                    ptr_table->setFocus();
                }
            }
        }
        //调出数据比对
        else if(type==2){
            if(row1>commonContentQByteArrayList.count()||row2>commonContentQByteArrayList.count()){
                statusBar_disPlayMessage(tr("异常,行号超出了数据最大行数值,放弃比对"));
                return;
            }
            else{
                //打开比对器
                //设置表格列标题
                QStringList title;
                QStringList fieldType;
                title.append("数据行号");
                QMap<int,QStringList> primaryCompareData;
                //OFD
                if(currentOpenFileType==openFileType::OFDFile){
                    for(int i=0;i<ofd.getFieldList().count();i++){
                        //仅获取列的中文备注当作列标题
                        title.append(ofd.getFieldList().at(i).getFieldDescribe());
                        fieldType.append(ofd.getFieldList().at(i).getFieldType());
                    }
                    primaryCompareData.insert(row1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row1-1));
                    primaryCompareData.insert(row2,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row2-1));
                }
                //CSV
                else if(currentOpenFileType==openFileType::CSVFile){
                    for(int i=0;i<csv.getFieldList().count();i++){
                        //仅获取列的中文备注当作列标题
                        title.append(csv.getFieldList().at(i).getFieldDescribe());
                        if(CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                            fieldType.append("N");
                        }
                        else{
                            fieldType.append("S");
                        }
                    }
                    primaryCompareData.insert(row1,Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row1-1));
                    primaryCompareData.insert(row2,Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row2-1));
                }
                //FIXED
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    for(int i=0;i<fixed.getFieldList().count();i++){
                        //仅获取列的中文备注当作列标题
                        title.append(fixed.getFieldList().at(i).getFieldDescribe());
                        fieldType.append(fixed.getFieldList().at(i).getFieldType());
                    }
                    primaryCompareData.insert(row1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row1-1));
                    primaryCompareData.insert(row2,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row2-1));
                }
                //打开窗口
                if(title.count()>0&&primaryCompareData.count()>1){
                    DialogShowTableCompareView * dialog = new DialogShowTableCompareView(title,fieldType,&primaryCompareData,this);
                    dialog->setWindowTitle(QString("行比对器视图"));
                    dialog->setModal(false);
                    dialog->setAttribute(Qt::WA_DeleteOnClose);
                    dialog->show();
                    dialog->raise();
                    dialog->activateWindow();
                }
                else{
                    statusBar_disPlayMessage(tr("没有配置适用于该文件的字段必填检查规则,请配置后重试..."));
                }
            }
        }
    }
}

void MainWindow::getPrimaryExportConfig(QMap <QString,int> config){
    QString openpath="./";
    if(!currentOpenFilePath.isEmpty()){
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
        }
    }
    QString filename="";
    if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
        QString filenameall=currentOpenFilePath.right(currentOpenFilePath.length()-currentOpenFilePath.lastIndexOf("/"));
        int index=filenameall.lastIndexOf(".");
        if(index==-1){
            filename=filenameall;
        }else{
            filename=filenameall.left(index);
        }
    }
    else{
        int index=currentOpenFilePath.lastIndexOf(".");
        if(index==-1){
            filename=currentOpenFilePath;
        }else{
            filename=currentOpenFilePath.left(index);
        }
    }
    filename=filename.append("_主键冲突检查报告");
    //文件过滤器,用于追踪选择的保存文件类别
    QString selectedFilter=Q_NULLPTR;
    //弹出保存框
    //数据类型插入点，csv文件不支持导出csv文件
    QString fileTypeSelect="";
    if(config.value("exptype")==0){
        fileTypeSelect="HTML文件(*.html)";
    }
    if(config.value("exptype")==1){
        fileTypeSelect="TXT文件(*.txt)";
    }
    //弹窗选择文件保存路径
    QString fileNameSave = QFileDialog::getSaveFileName(this,("选择数据导出保存路径"),openpath+filename,fileTypeSelect,&selectedFilter);
    if(!fileNameSave.isEmpty()){
        //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
        if(selectedFilter=="TXT文件(*.txt)"&&(!fileNameSave.endsWith(".txt"))){
            fileNameSave.append(".xlsx");
        }
        else if(selectedFilter=="HTML文件(*.html)"&&(!fileNameSave.endsWith(".html"))){
            fileNameSave.append(".html");
        }
        primaryCheck(1,fileNameSave,config.value("exptype"),config.value("openatexported"));
    }
    else{
        statusBar_disPlayMessage("取消导出...");
    }
}

void MainWindow::getFieldExportConfig(QMap <QString,int> config){
    if(currentOpenFileType==openFileType::OFDFile||currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile){
        dataBlocked=true;
        isExportFile=true;
        dataBlockedMessage="正在进行字段必填检查报告导出,请稍后再操作...";
        QString openpath="./";
        if(!currentOpenFilePath.isEmpty()){
            if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
                openpath=currentOpenFilePath.left(currentOpenFilePath.lastIndexOf("/")+1);
            }
        }
        QString filename="";
        if(currentOpenFilePath.contains("/",Qt::CaseSensitive)){
            QString filenameall=currentOpenFilePath.right(currentOpenFilePath.length()-currentOpenFilePath.lastIndexOf("/"));
            int index=filenameall.lastIndexOf(".");
            if(index==-1){
                filename=filenameall;
            }else{
                filename=filenameall.left(index);
            }
        }
        else{
            int index=currentOpenFilePath.lastIndexOf(".");
            if(index==-1){
                filename=currentOpenFilePath;
            }else{
                filename=currentOpenFilePath.left(index);
            }
        }
        filename=filename.append("_字段必填检查报告");
        //文件过滤器,用于追踪选择的保存文件类别
        QString selectedFilter=Q_NULLPTR;
        //弹出保存框
        //数据类型插入点，csv文件不支持导出csv文件
        QString fileTypeSelect="";
        if(config.value("exptype")==0){
            fileTypeSelect="HTML文件(*.html)";
        }
        if(config.value("exptype")==1){
            fileTypeSelect="TXT文件(*.txt)";
        }
        //弹窗选择文件保存路径
        QString fileNameSave = QFileDialog::getSaveFileName(this,("选择数据导出保存路径"),openpath+filename,fileTypeSelect,&selectedFilter);
        if(!fileNameSave.isEmpty()){
            //在某些系统下（linux系统）选择要保存的文件类型时,系统并不会自动补全文件后缀名,需要咱们自己补全文件后缀
            if(selectedFilter=="TXT文件(*.txt)"&&(!fileNameSave.endsWith(".txt"))){
                fileNameSave.append(".xlsx");
            }
            else if(selectedFilter=="HTML文件(*.html)"&&(!fileNameSave.endsWith(".html"))){
                fileNameSave.append(".html");
            }
            //开始写入数据
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QFile data(fileNameSave);
            if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
                QTextCodec *codec=QTextCodec::codecForName("UTF-8");
                QString sb;
                QString title="";
                if(filename.contains("/")){
                    title=filename.mid(filename.lastIndexOf("/")+1);
                    title=title.left(title.length()-0);
                }else{
                    title=title.left(title.length()-0);
                }
                if(config.value("exptype")==0){
                    //html文件头
                    sb.append("<!DOCTYPE html>\r\n<html>\r\n<!--Design By Liudewei("+QByteArray::fromBase64(AUTHOR_EMAIL)+")-->\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n<title>"+title+"</title>\r\n</head>\r\n<body>\r\n");
                    //内联的表格样式,内容太多,base64存储
                    sb.append(QByteArray::fromBase64("PHN0eWxlIHR5cGU9InRleHQvY3NzIj4KLnRhYmxlCnsKcGFkZGluZzogMDsKbWFyZ2luOiAwOwp9CnRoIHsKZm9udDogYm9sZCAxMnB4ICJUcmVidWNoZXQgTVMiLCBWZXJkYW5hLCBBcmlhbCwgSGVsdmV0aWNhLCBzYW5zLXNlcmlmOwpjb2xvcjogIzRmNmI3MjsKYm9yZGVyLXJpZ2h0OiAxcHggc29saWQgI0MxREFENzsKYm9yZGVyLWJvdHRvbTogMXB4IHNvbGlkICNDMURBRDc7CmJvcmRlci10b3A6IDFweCBzb2xpZCAjQzFEQUQ3OwpsZXR0ZXItc3BhY2luZzogMnB4Owp0ZXh0LXRyYW5zZm9ybTogdXBwZXJjYXNlOwp0ZXh0LWFsaWduOiBsZWZ0OwpwYWRkaW5nOiA2cHggNnB4IDZweCAxMnB4OwpiYWNrZ3JvdW5kOiAjQ0FFOEVBIG5vLXJlcGVhdDsKd29yZC1icmVhazoga2VlcC1hbGw7CndoaXRlLXNwYWNlOm5vd3JhcDsKfQp0ciB7CndvcmQtYnJlYWs6IGtlZXAtYWxsOwp3aGl0ZS1zcGFjZTpub3dyYXA7Cn0KdGQgewpib3JkZXItcmlnaHQ6IDFweCBzb2xpZCAjQzFEQUQ3Owpib3JkZXItYm90dG9tOiAxcHggc29saWQgI0MxREFENzsKZm9udC1zaXplOjE0cHg7CnBhZGRpbmc6IDJweCA2cHggMnB4IDZweDsKY29sb3I6ICM0ZjZiNzI7Cn0KLm51bWJlcnsKdGV4dC1hbGlnbjogcmlnaHQ7Cn0KPC9zdHlsZT4K"));
                    //标题表头
                    sb.append("<table>\r\n<tr><th>结果编号</th><th>数据行号</th><th>命中列号</th><th>校验结果</th></tr>\r\n");

                }
                else{
                    sb.append("结果编号\t数据行号\t命中列号\t校验结果\r\n");
                }
                data.write(codec->fromUnicode(sb));
                data.flush();
                sb.clear();
                ////开始获取数据
                int rowCount=commonContentQByteArrayList.count();
                int checkCount=0;
                if(rowCount>0){
                    for (int row=0;row<rowCount;row++){
                        QStringList rowdata;
                        if(currentOpenFileType==openFileType::OFDFile){
                            rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                        }
                        else if(currentOpenFileType==openFileType::CSVFile){
                            rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row);
                        }
                        QMap<int,QString> checkresult;
                        checkRowFieldResult (rowdata,checkresult,1);
                        if(!checkresult.empty()){
                            foreach(auto item,checkresult.toStdMap()){
                                checkCount++;
                                if(config.value("exptype")==0){
                                    sb.append("<tr><td class=\"number\">").append(QString::number(checkCount)).append("</td>");
                                    sb.append("<td class=\"number\">").append(QString::number(row+1)).append("</td>");
                                    sb.append("<td class=\"number\">").append(QString::number(item.first)).append("</td>");
                                    sb.append("<td>").append(item.second).append("</td></tr>\r\n");
                                }
                                else{
                                    sb.append(QString::number(checkCount));
                                    sb.append("\t").append(QString::number(row+1));
                                    sb.append("\t").append(QString::number(item.first));
                                    sb.append("\t").append(item.second).append("\r\n");
                                }
                            }
                        }
                        if((row%1000==0)||(row==rowCount-1)){
                            data.write(codec->fromUnicode(sb));
                            data.flush();
                            sb.clear();
                            statusBar_disPlayMessage(QString("字段必填检查报告导出中,请勿进行其他操作,已检查%1行").arg(QString::number(row+1)));
                            qApp->processEvents();
                            if(abortAndExit){
                                QApplication::restoreOverrideCursor();
                                qDebug()<<"强制立即终止任务";
                                return;
                            }
                        }
                    }
                }
                if(config.value("exptype")==0){
                    sb.append("</table>\r\n</body>\r\n</html>");
                }
                data.write(codec->fromUnicode(sb));
                data.flush();
                data.close();
                statusBar_disPlayMessage(tr("报告成功导出到%1").arg(fileNameSave));
                if(config.value("openatexported")==1){
                    bool is_open = QDesktopServices::openUrl(QUrl::fromLocalFile(fileNameSave));
                    if(!is_open)
                    {
                        statusBar_disPlayMessage("文件打开失败,请尝试手工打开~~~");
                        return;
                    }
                }
            }else{
                statusBar_disPlayMessage(tr("报告导出失败,请重试"));
            }
            dataBlocked=false;
            isExportFile=false;
            QApplication::restoreOverrideCursor();
        }
        else{
            dataBlocked=false;
            isExportFile=false;
            statusBar_disPlayMessage("取消导出...");
        }
    }
}

void MainWindow::checkRowFieldSearch (int direction){
    if(updateOrBlcokCheck()){
        return;
    }
    //数据类型校验
    if(currentOpenFileType==openFileType::NotFileOrErr){
        return;
    }
    else if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("不支持索引文件使用此功能...");
        return;
    }
    else if(currentOpenFileType==openFileType::OFDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的OFD文件没有数据记录...");
            return;
        }
        if(ofd.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::CSVFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的CSV文件没有数据记录...");
            return;
        }
        if(csv.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        if(commonContentQByteArrayList.count()<1){
            statusBar_disPlayMessage("打开的字段定长文件没有数据记录...");
            return;
        }
        if(fixed.getFieldcheckList().count()<1){
            statusBar_disPlayMessage("没有适用于本文件的必填校验规则...");
            return;
        }
    }
    else if(currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("不支持DBF文件使用此功能...");
        return;
    }
    if (direction!=0&&direction!=1){
        return;
    }
    if(!checkFieldFlag){
        statusBar_disPlayMessage("必填检测功能已被禁用,请开启...");
        return;
    }
    //准备工作
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();
    dataBlockedMessage="正在搜索字段必填缺失,请稍后再操作...";
    int rowRealInContent=0;
    //焦点行从焦点单元格上一个开始搜索,非焦点行从最后一个
    int beginCol=tableColCurrent;
    int beginRow=tableRowCurrent;
    /////////////////////////////////////////////////
    if (direction==0){
        if(currentPage==1&&beginRow==0&&beginCol==0){
            statusBar_disPlayMessage("已经搜索到第一个单元格了,请向下搜索...");
            QApplication::restoreOverrideCursor();
            dataBlocked=false;
            return;
        }
        else {
            ui->actionprefieldcheck->setEnabled(false);
            ui->actionnextfieldcheck->setEnabled(false);
            //初始起点
            beginCol=tableColCurrent-1;
            //上一行
            if(beginCol<0&&beginRow>0){
                beginRow=beginRow-1;
                beginCol=ptr_table->columnCount()-1;
            }
            while(currentPage>0){
                QStringList rowdata;
                for(int row=beginRow;row>=0;row--){
                    QMap<int,QString> checkresult;
                    rowRealInContent=(currentPage-1)*pageRowSize+row;
                    if(currentOpenFileType==openFileType::OFDFile){
                        rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::CSVFile){
                        rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::FIXEDFile){
                        rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
                    }
                    if(row%100==0){
                        statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortAndExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                    checkRowFieldResult (rowdata,checkresult);
                    if(checkresult.empty()){
                        //本行没有，准备搜下一行
                        beginCol=ptr_table->columnCount()-1;
                        continue;
                    }
                    else{
                        for (int colindex=beginCol;colindex>=0;colindex--) {
                            if(checkresult.contains(colindex)){
                                ptr_table->setCurrentCell(row,colindex);
                                ptr_table->setFocus();
                                ui->actionprefieldcheck->setEnabled(true);
                                ui->actionnextfieldcheck->setEnabled(true);
                                QApplication::restoreOverrideCursor();
                                dataBlocked=false;
                                reCalculateTableBeginAndEnd();
                                on_tableWidget_itemSelectionChanged();
                                QString message=QString("在第%1行\"%2\"查找到必填字段缺失...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(colindex)->text());
                                statusBar_disPlayMessage(message);
                                return;
                            }
                        }
                    }
                }
                if(currentPage>1){
                    currentPage--;
                    pageJump(currentPage,-1);
                    qApp->processEvents();
                    //重新设置从右下角开始继续搜索
                    beginCol=ptr_table->columnCount()-1;
                    beginRow=ptr_table->rowCount()-1;
                    tableColCurrent=ptr_table->columnCount()-1;
                    tableRowCurrent=ptr_table->rowCount()-1;
                }
                else{
                    statusBar_disPlayMessage("再往上没有命中必填缺失的单元格了...");
                    dataBlocked=false;
                    ui->actionprefieldcheck->setEnabled(true);
                    ui->actionnextfieldcheck->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    break;
                }
            }
        }
    }
    else if (direction==1){
        if(currentPage==pageCount&&beginRow==ptr_table->rowCount()-1&&beginCol==ptr_table->columnCount()-1){
            statusBar_disPlayMessage("已经搜索到最后一个单元格了,请向上搜索...");
            QApplication::restoreOverrideCursor();
            dataBlocked=false;
            return;
        }
        else{
            ui->actionprefieldcheck->setEnabled(false);
            ui->actionnextfieldcheck->setEnabled(false);
            //初始起点
            beginCol=tableColCurrent+1;
            //最后一列
            if(beginCol>ptr_table->columnCount()-1&&beginRow<ptr_table->rowCount()-1){
                beginRow=beginRow+1;
                beginCol=0;
            }
            while(currentPage<=pageCount){
                QStringList rowdata;
                for(int row=beginRow;row<ptr_table->rowCount();row++){
                    QMap<int,QString> checkresult;
                    rowRealInContent=(currentPage-1)*pageRowSize+row;
                    if(currentOpenFileType==openFileType::OFDFile){
                        rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::CSVFile){
                        rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                    }
                    else if(currentOpenFileType==openFileType::FIXEDFile){
                        rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
                    }
                    if(row%500==0){
                        statusBar_disPlayMessage(QString("正在搜索第%1行,请耐心等待...").arg(QString::number(rowRealInContent+1)));
                        qApp->processEvents();
                        //强制终止事件-立即return退出
                        if(abortAndExit){
                            QApplication::restoreOverrideCursor();
                            qDebug()<<"强制立即终止任务";
                            return;
                        }
                    }
                    checkRowFieldResult (rowdata,checkresult);
                    if(checkresult.empty()){
                        beginCol=0;
                        continue;
                    }
                    else{
                        for (int colindex=beginCol;colindex<ptr_table->columnCount();colindex++) {
                            if(checkresult.contains(colindex)){
                                ptr_table->setCurrentCell(row,colindex);
                                ptr_table->setFocus();
                                ui->actionprefieldcheck->setEnabled(true);
                                ui->actionnextfieldcheck->setEnabled(true);
                                QApplication::restoreOverrideCursor();
                                dataBlocked=false;
                                reCalculateTableBeginAndEnd();
                                on_tableWidget_itemSelectionChanged();
                                QString message=QString("在第%1行\"%2\"查找到必填字段缺失...").arg(QString::number(rowRealInContent+1),ptr_table->horizontalHeaderItem(colindex)->text());
                                statusBar_disPlayMessage(message);
                                return;
                            }
                        }
                    }
                }
                if(currentPage<pageCount){
                    currentPage++;
                    pageJump(currentPage,-2);
                    qApp->processEvents();
                    beginCol=0;
                    beginRow=0;
                    tableColCurrent=0;
                    tableRowCurrent=0;
                }
                else{
                    statusBar_disPlayMessage("再往下没有命中必填缺失的单元格了...");
                    dataBlocked=false;
                    ui->actionprefieldcheck->setEnabled(true);
                    ui->actionnextfieldcheck->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    break;
                }
            }
        }
    }
}

void MainWindow::update_import_excel_status(){
    statusBar_disPlayMessage(QString("已分析数据%1行,请耐心等待...").arg(QString::number(importExcelRow+1)));
}

void MainWindow::update_zip_extract_status(){
    statusBar_disPlayMessage(QString("正在解压文件,共计%1个,已完成解压%2个,请耐心等待...").arg(QString::number(zipFileInfoList.count()),QString::number(zipFileCurrentUnzipIndex)));
}

bool MainWindow::extractFileFromZipFile(){
    QFile file(zipfilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        zipExtractMessage=QString("无法打开zip文件[%1],请重试...").arg(zipfilePath);
        return false;
    }
    QZipReader reader(&file);
    if (!reader.isReadable()) {
        zipExtractMessage=QString("无法打开zip文件[%1],请重试...").arg(zipfilePath);
        return false;
    }
    if(Utils::isFileExist(zipOutPutFileDir)){
        QFile file(zipOutPutFileDir);
        if (file.exists()) {
            if (!file.remove()) {
                zipExtractMessage=QString("临时目录[%1]创建失败,请重试...").arg(zipOutPutFileDir);
                return false;
            }
        }
    }
    if(!Utils::isDirExist(zipOutPutFileDir)){
        QDir dir;
        if (!dir.mkpath(zipOutPutFileDir)) {
            zipExtractMessage=QString("临时目录[%1]创建失败,请重试...").arg(zipOutPutFileDir);
            return false;
        }
    }
    zipFileCurrentUnzipIndex=0;
    const auto fileList = reader.fileInfoList();
    bool extractOk=true;
    QSet<QString> zipFileNameFromZip;
    for (auto it = fileList.begin(); it != fileList.end(); ++it){
        zipFileNameFromZip.insert(it->filePath);
    }
    QTextCodec *systemCodec=QTextCodec::codecForName("System");
    QTextCodec *zipCodec=QTextCodec::codecForName("UTF-8");
    for (int i=0;i<zipFileInfoList.count();i++){
        //这个是用户在弹窗界面选择的编码获取的
        QString currenctFilePath=zipFileInfoList.at(i).filePath;
        //这个有可能是乱码的-使用系统默认编码获取的
        QString currenctFilePathSystemCodec=zipFileNameSystemCodecList.at(i);
        uint currentFileSize=zipFileInfoList.at(i).size;
        if ((currenctFilePath==zipTargetFileInZip||zipExtractAll)&&(zipFileNameFromZip.contains(currenctFilePathSystemCodec)||zipFileNameFromZip.contains(currenctFilePath))){
            //有可能因为各种原因导致获取不到数据，这里做各种尝试
            //走系统默认编码方式路径尝试读取
            QByteArray fileData = reader.fileData(currenctFilePathSystemCodec);
            if(currentFileSize>0&&fileData.size()==0){
                //走选择得编码方式路径尝试读取
                fileData = reader.fileData(currenctFilePath);
                if (fileData.size()==0){
                    //走固定的UTF-8编码方式路径尝试读取
                    QTextCodec::setCodecForLocale(zipCodec);
                    fileData = reader.fileData(currenctFilePath);
                    QTextCodec::setCodecForLocale(systemCodec);
                    if(fileData.size()==0){
                        zipExtractMessage=QString("从压缩包中提取[%1]失败,可能是不支持的压缩算法(非Deflate算法)/文件不完整/加密压缩,请重试或者使用系统解压缩工具解压...").arg(currenctFilePath);
                        extractOk=false;
                        break;
                    }
                }
            }
            QString targetFilePath=zipOutPutFileDir+currenctFilePath;
            QString targetFileDir=targetFilePath.left(targetFilePath.lastIndexOf("/")+1);
            if(Utils::isFileExist(targetFileDir)){
                QFile file(targetFileDir);
                if (file.exists()) {
                    if (!file.remove()) {
                        zipExtractMessage=QString("临时目录[%1]创建失败,请重试或检查是否有对该路径的读写权限...").arg(targetFileDir);
                        extractOk=false;
                        break;
                    }
                }
            }
            if(!Utils::isDirExist(targetFileDir)){
                QDir dir;
                if (!dir.mkpath(targetFileDir)) {
                    zipExtractMessage=QString("临时目录[%1]创建失败,请重试或检查是否有对该路径的读写权限...").arg(targetFileDir);
                    extractOk=false;
                    break;
                }
            }
            QFile outFile(targetFilePath);
            if (!outFile.open(QIODevice::WriteOnly)) {
                zipExtractMessage=QString("从压缩包中提取文件[%1]后写入失败,请重试或者检查是否有对路径[%2]的读写权限...").arg(currenctFilePath,targetFileDir);
                extractOk=false;
                break;
            }
            int returnCode=outFile.write(fileData);
            outFile.close();
            if(returnCode==-1){
                zipExtractMessage=QString("从压缩包中提取文件[%1]后写入失败,请重试或者检查是否有对路径[%2]的读写权限...").arg(currenctFilePath,targetFileDir);
                extractOk=false;
                break;
            }
            zipFileCurrentUnzipIndex++;
            emit signals_update_zip_extract_status();
            //恢复文件的修改时间-该时间从压缩包中提取
            Utils::UpdateFileTime(targetFilePath,zipFileListMdTime.value(currenctFilePath));
        }
        else{
            continue;
        }
    }
    reader.close();
    file.close();
    if(extractOk){
        zipExtractMessage="";
        return true;
    }
    else{
        return false;
    }
}

void MainWindow::extractZipFinished(){
    dataBlocked=false;
    if(watcherZipExtractStatus->result()){
        fileFromZip=true;
        initFile(zipOutPutFileDir+zipTargetFileInZip,false,false,true);
    }
    else{
        zipfilePath="";
        zipTargetFileInZip="";
        zipExtractAll=false;
        zipOutPutFileDir="";
        statusBar_disPlayMessage(zipExtractMessage);
    }
}

void MainWindow::save2Xlsx(QString filename,int pageNum){
    //计算导出范围
    //默认从0行开始
    int rowBegin=0;
    //默认为当前打开的文件的最大行记录
    int rowEnd=0;
    switch (currentOpenFileType) {
    case 1:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 2:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 3:
        rowEnd=commonContentQByteArrayList.count();
        break;
    case 4:
        rowEnd=dbfRowMap.count();
        break;
    default:
        break;
    }
    //分页导出,需要按当前页计算
    if(pageNum>0){
        rowBegin=(pageNum-1)*pageRowSize;
        rowEnd=qMin(pageNum*pageRowSize,rowEnd);
    }
    //禁止导出过大的excel文件-目前不会发生这个情况-目前单页最大100万行=最大允许导出的excel行
    if((rowEnd-rowBegin)>maxExcelRow){
        statusBar_disPlayMessage("当前数据记录大于"+QString::number(maxExcelRow)+"行,请降低分页条数到1000000万行或者以下按分页导出Excel...");
        dataBlocked=false;
        isExportFile=false;
        return;
    }
    xlsxSaveName=filename;
    //鼠标响应进入等待
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //标题的样式
    QXlsx::Format formatTitle;
    formatTitle.setFont(QFont("SimSun"));
    formatTitle.setFontBold(true);
    formatTitle.setFontColor(QColor(Qt::black));
    formatTitle.setPatternBackgroundColor(QColor(0,176,80));
    formatTitle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatTitle.setBorderStyle(QXlsx::Format::BorderThin);
    //用来记录列最宽的list
    int columnCount=ptr_table->columnCount();
    QList<int> colWidthArray;
    //初始化赋值
    for(int i=0;i<columnCount;i++){
        colWidthArray.append(0);
    }
    //标题和是否是数值列,数值列需要设置列格式
    QHash<int,QXlsx::Format> numberFormatQhash;
    //标准文本样式
    QXlsx::Format formatBody;
    formatBody.setFont(QFont("SimSun"));
    formatBody.setNumberFormat("@");
    int excelRowNumber=2;
    //导出OFD文件的内容
    if(currentOpenFileType==openFileType::OFDFile){
        //插入标题和数值列的专用格式
        for(int i=0;i<columnCount;i++){
            //插入标题
            xlsx->write(1,i+1,ofd.getFieldList().at(i).getFieldDescribe(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(ofd.getFieldList().at(i).getFieldDescribe().toLocal8Bit().length()+5);
            //插入列格式
            if(ofd.getFieldList().at(i).getFieldType()=="N"){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=ofd.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int exprow=0;
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,row);
                for(int col=0;col<columnCount;col++){
                    QString value=rowdata.at(col);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=value.length()/3;
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(ofd.getFieldList().at(col).getFieldType()=="N"&&numberFormatQhash.contains(col)){
                        //对于数值型的数据如果接口文档里给的确实是数值型,则填充到excel时也用数值型
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortAndExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    //导出CSV文件的内容
    if(currentOpenFileType==openFileType::CSVFile){
        //标题内容
        for(int i=0;i<columnCount;i++){
            xlsx->write(1,i+1,ptr_table->horizontalHeaderItem(i)->text(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(ptr_table->horizontalHeaderItem(i)->text().toLocal8Bit().length()+5);
            //插入列格式
            if(CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=CsvFieldIsNumberOrNot.value(i).getDecimalLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int exprow=0;
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                //csv文件按行获取数据
                QStringList rowdata=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,row);
                //csv文件的数据可能缺失列，因为读取的时候没有做强制判断，这里判断补充下,如果数据长度不够，补空数据
                while(rowdata.count()<columnCount){
                    rowdata.append("");
                }
                for(int col=0;col<columnCount;col++){
                    QString value=rowdata.at(col);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符和小数点后的数据，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=value.length()/3;
                            if(CsvFieldIsNumberOrNot.contains(col)&&CsvFieldIsNumberOrNot.value(col).getIsNumber()){
                                addLen=addLen+CsvFieldIsNumberOrNot.value(col).getDecimalLength()+3;
                            }
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(CsvFieldIsNumberOrNot.contains(col)&&numberFormatQhash.contains(col)){
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortAndExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    //fixed文件
    if(currentOpenFileType==openFileType::FIXEDFile){
        //插入标题和数值列的专用格式
        for(int i=0;i<columnCount;i++){
            //插入标题
            xlsx->write(1,i+1,fixed.getFieldList().at(i).getFieldDescribe(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(fixed.getFieldList().at(i).getFieldDescribe().toLocal8Bit().length()+5);
            //插入列格式
            if(fixed.getFieldList().at(i).getFieldType()=="N"){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=fixed.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        int exprow=0;
        //文本内容
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,row);
                for(int col=0;col<columnCount;col++){
                    QString value=(col<rowdata.count()?rowdata.at(col):nullptr);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=value.length()/3;
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(fixed.getFieldList().at(col).getFieldType()=="N"&&numberFormatQhash.contains(col)){
                        //对于数值型的数据如果接口文档里给的确实是数值型,则填充到excel时也用数值型
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortAndExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    if(currentOpenFileType==openFileType::DBFFile){
        //插入标题和数值列的专用格式
        for(int i=0;i<columnCount;i++){
            //插入标题
            xlsx->write(1,i+1,dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe(),formatTitle);
            //记录每列的宽度
            colWidthArray[i]=(dbf.getFieldList().at(i).getFieldDescribe().isEmpty()?dbf.getFieldList().at(i).getFieldName():dbf.getFieldList().at(i).getFieldName()+"-"+dbf.getFieldList().at(i).getFieldDescribe()).toLocal8Bit().length()+5;
            //插入列格式
            if(dbfColIsNumber(i)){
                QXlsx::Format formatNumber;
                formatNumber.setFont(QFont("SimSun"));
                //构造数值长度
                QString zj="#,##0";
                QString x="";
                int lengthx=dbf.getFieldList().at(i).getDecLength();
                int ix=0;
                while(ix<lengthx){
                    x.append("0");
                    ix++;
                }
                if(x.length()>0){
                    formatNumber.setNumberFormat(zj+"."+x);
                }
                else{
                    formatNumber.setNumberFormat(zj);
                }
                numberFormatQhash.insert(i,formatNumber);
            }
        }
        //文本内容
        int exprow=0;
        if(rowEnd>0){
            for (int row=rowBegin;row<rowEnd;row++){
                //数据写入--按行读取
                QStringList rowdata=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,row,&dbfRowMap,false,dbfTrimType);
                for(int col=0;col<columnCount;col++){
                    QString value=rowdata.at(col);
                    //空的单元格直接忽略
                    if(value.isEmpty()){
                        continue;
                    }
                    //决定是否更新列宽,如果本列数据比以往的都长那就得更新列宽度
                    int widthNew=value.toLocal8Bit().length()+5;
                    if(widthNew>colWidthArray[col]){
                        //重新校准数值型数据的宽度，数值型excel显示时会插入千位符，需要提高宽度
                        if(numberFormatQhash.contains(col)){
                            int addLen=(value.indexOf(".")+1)/3;
                            colWidthArray[col]=widthNew+addLen;
                        }
                        else{
                            colWidthArray[col]=widthNew;
                        }
                    }
                    //判断数据类型
                    if(dbfColIsNumber(col)&&numberFormatQhash.contains(col)){
                        bool okNb=false;
                        double number=value.toDouble(&okNb);
                        //数字超长
                        if(okNb&&value.length()<=16){
                            xlsx->write(excelRowNumber,col+1,number,numberFormatQhash.value(col));
                        }
                        else{
                            xlsx->write(excelRowNumber,col+1,value,formatBody);
                        }
                    }else{
                        xlsx->write(excelRowNumber,col+1,value,formatBody);
                    }
                }
                exprow++;
                //每100行读取下事件循环
                //excel导出较慢
                if(exprow%100==0){
                    statusBar_disPlayMessage(QString("正在生成xlsx文件数据记录,已生成%1行").arg(QString::number(exprow)));
                    qApp->processEvents();
                    //强制终止事件-立即return退出
                    if(abortAndExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                excelRowNumber++;
            }
        }
    }
    //根据每列最大的值设置本列的宽度
    for(int i=0;i<columnCount;i++){
        xlsx->setColumnWidth(i+1,i+1,static_cast<double>(colWidthArray[i]));
    }
    statusBar_disPlayMessage(QString("数据生成完毕,正在写入xlsx文件,请耐心等待..."));
    //恢复鼠标响应
    QApplication::restoreOverrideCursor();
    //保存文件////写入文件耗时操作放到新的线程取完成
    auto future = QtConcurrent::run(this, &MainWindow::save2XlxsFile);
    watcherXlsxSaveStatus->setFuture(future);
}

bool MainWindow::save2XlxsFile(){
    //专门为xlsx文件写入更新提示
    dataBlockedMessage="正在进行数据导出写入到xlsx文件,请稍后再操作...";
    //保存文件
    bool flag=xlsx->saveAs(xlsxSaveName);
    //释放内存空间
    delete xlsx;
    //重建变量
    xlsx=new QXlsx::Document();
    //返回结束
    return flag;
}

void MainWindow::save2XlsxFinished(){
    dataBlocked=false;
    if(watcherXlsxSaveStatus->result()){
        statusBar_disPlayMessage(tr("数据成功导出到:%1").arg(xlsxSaveName));
        //如果需要打开文件
        if(openXlsx){
            statusBar_disPlayMessage(tr("使用系统默认程序打开文件:%1").arg(xlsxSaveName));
            bool is_open = QDesktopServices::openUrl(QUrl::fromLocalFile(xlsxSaveName));
            if(!is_open)
            {
                statusBar_disPlayMessage("文件打开失败,请尝试手工打开~~~");
                return;
            }
        }
    }
    else{
        statusBar_disPlayMessage(tr("数据导出写入到[%1]失败,请重试").arg(xlsxSaveName));
    }
}

void MainWindow::checkRowFieldResult (QStringList & rowdata,QMap<int,QString> & checkresult,int splitFlag){
    switch (currentOpenFileType){
    case openFileType::OFDFile:
    {
        foreach (fieldcheckitem item, ofd.getFieldcheckList()) {
            //条件分组--或者关系
            foreach (auto cclist,item.getCheckConditionsListOverList()){
                //本分组内条件清单--和关系--必须全部满足
                QString cond="";
                bool needshi=true;
                bool thisconflag=true;
                /////////////////////////////条件检查//////////////////
                if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                    cond.append("本文件中");
                    needshi=false;
                }
                else{
                    needshi=true;
                    foreach(auto cc,cclist){
                        if(rowdata.count()>=cc.index+1&&rowdata.at(cc.index)==cc.value){
                            if(cond.isEmpty()){
                                cond.append(ofd.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            else{
                                cond.append(",并且").append(ofd.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            continue;
                        }
                        //一个条件不满足就不检查了
                        else{
                            thisconflag=false;
                            break;
                        }
                    }
                }
                /////////////////////////////条件检查//////////////////
                //命中本规则,开始构建规则检查结果
                if(thisconflag){
                    foreach(auto xxx,item.getRequiredFields()){
                        if(rowdata.at(xxx).isEmpty()){
                            if(checkresult.contains(xxx)){
                                if(needshi){
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append("时,").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append(",").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                            else{
                                if(needshi){
                                    checkresult.insert(xxx,QString(cond).append("时,").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(cond).append(",").append(ofd.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
        break;
    case openFileType::FIXEDFile:
    {
        QList<fieldcheckitem> fixedFileDefinition=fixed.getFieldcheckList();
        for (auto it = fixedFileDefinition.begin(); it != fixedFileDefinition.end(); ++it) {
            fieldcheckitem item=*it;
            //条件分组--或者关系
            foreach (auto cclist,item.getCheckConditionsListOverList()){
                //本分组内条件清单--和关系--必须全部满足
                QString cond="";
                bool needshi=true;
                bool thisconflag=true;
                if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                    cond.append("本文件中");
                    needshi=false;
                }
                else{
                    needshi=true;
                    foreach(auto cc,cclist){
                        if(rowdata.count()>=cc.index+1&&rowdata.at(cc.index)==cc.value){
                            if(cond.isEmpty()){
                                cond.append(fixed.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            else{
                                cond.append(",并且").append(fixed.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            continue;
                        }
                        //一个条件不满足就不检查了
                        else{
                            thisconflag=false;
                            break;
                        }
                    }
                }
                //命中本规则,开始构建规则检查结果
                if(thisconflag){
                    foreach(auto xxx,item.getRequiredFields()){
                        if(rowdata.at(xxx).isEmpty()){
                            if(checkresult.contains(xxx)){
                                if(needshi){
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append("时,").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append(",").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                            else{
                                if(needshi){
                                    checkresult.insert(xxx,QString(cond).append("时,").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(cond).append(",").append(fixed.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
        break;
    case openFileType::CSVFile:
    {
        QList<fieldcheckitem> csvFileDefinition=csv.getFieldcheckList();
        for (auto it = csvFileDefinition.begin(); it != csvFileDefinition.end(); ++it) {
            fieldcheckitem item=*it;
            //条件分组--或者关系
            foreach (auto cclist,item.getCheckConditionsListOverList()){
                //本分组内条件清单--和关系--必须全部满足
                QString cond="";
                bool needshi=true;
                bool thisconflag=true;
                if(cclist.count()==1&&cclist.at(0).index==ALL_CHECK_INDEX){
                    cond.append("本文件中");
                    needshi=false;
                }
                else{
                    needshi=true;
                    foreach(auto cc,cclist){
                        if(rowdata.count()>=cc.index+1&&rowdata.at(cc.index)==cc.value){
                            if(cond.isEmpty()){
                                cond.append(csv.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            else{
                                cond.append(",并且").append(csv.getFieldList().at(cc.index).getFieldDescribe()).append("=").append(cc.value);
                            }
                            continue;
                        }
                        //一个条件不满足就不检查了
                        else{
                            thisconflag=false;
                            break;
                        }
                    }
                }
                //命中本规则,开始构建规则检查结果
                if(thisconflag){
                    foreach(auto xxx,item.getRequiredFields()){
                        if(rowdata.at(xxx).isEmpty()){
                            if(checkresult.contains(xxx)){
                                if(needshi){
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append("时,").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(checkresult.value(xxx)).append(splitFlag==0?"\r\n":";").append(cond).append(",").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                            else{
                                if(needshi){
                                    checkresult.insert(xxx,QString(cond).append("时,").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                                else{
                                    checkresult.insert(xxx,QString(cond).append(",").append(csv.getFieldList().at(xxx).getFieldDescribe()).append("是必填字段"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::getNewHeaderAndFooter(QStringList header,QStringList footer){
    if(currentOpenFileType==openFileType::OFDFile){
        if(header.count()!=commonHeaderQStringList.count()){
            statusBar_disPlayMessage("文件头读取异常,跳过更新文件头文件尾");
            return;
        }
        else{
            bool modify=false;
            if(header.count()>0){
                for (int i=0;i<header.count();i++){
                    if(0!=header.at(i).compare(commonHeaderQStringList.at(i),Qt::CaseSensitive)){
                        commonHeaderQStringList.replace(i,header.at(i));
                        modify=true;
                    }
                }
            }
            if(footer.count()>0){
                if(0!=footer.at(0).compare(ofdFooterQString,Qt::CaseSensitive)){
                    ofdFooterQString=footer.at(0);
                    modify=true;
                }
            }
            if(modify){
                statusBar_disPlayMessage("已修改文件头文件尾,请保存文件!");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                fileChanged=true;
            }
            else{
                statusBar_disPlayMessage("文件头文件尾未修改...");
            }
        }
    }
}

bool MainWindow::importFromExcel(){
    dataBlocked=true;
    importExcelErrorDetail.clear();
    fileContentQByteArrayListFromExcel.clear();
    dataBlockedMessage="正在装载Excel文件,请耐心等待...";
    if (Utils::isFileExist(fromExcelImportFilePath)){
        QXlsx::Document xlsxread(fromExcelImportFilePath);
        QTextCodec *codec=QTextCodec::codecForName("GB18030");
        QList<fieldType> fieldTypeList;
        int colCount=0;
        int rowLength=0;
        QList <int> fixedFileFieldCountList;
        if(currentOpenFileType==openFileType::OFDFile){
            colCount=ofd.getFieldCount();
            codec=codecOFD;
            rowLength=ofd.getRowLength();
            for(int i=0;i<ofd.getFieldList().count();i++){
                fieldType item;
                item.fieldType=ofd.getFieldList().at(i).getFieldType();
                item.fieldLength=ofd.getFieldList().at(i).getLength();
                item.fieldDecLength=ofd.getFieldList().at(i).getDecLength();
                item.fieldName=ofd.getFieldList().at(i).getFieldName();
                item.fieldDescribe=ofd.getFieldList().at(i).getFieldDescribe();
                item.fieldBeginIndex=ofd.getFieldList().at(i).getRowBeginIndex();
                fieldTypeList.append(item);
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            colCount=fixed.getFieldCountMax();
            codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            fixedFileFieldCountList=fixed.getRowLengthHash().values();
            for(int i=0;i<fixed.getFieldList().count();i++){
                fieldType item;
                item.fieldType=fixed.getFieldList().at(i).getFieldType();
                item.fieldLength=fixed.getFieldList().at(i).getLength();
                rowLength+=item.fieldLength;
                item.fieldDecLength=fixed.getFieldList().at(i).getDecLength();
                item.fieldName=fixed.getFieldList().at(i).getFieldName();
                item.fieldDescribe=fixed.getFieldList().at(i).getFieldDescribe();
                item.fieldBeginIndex=fixed.getFieldList().at(i).getRowBeginIndex();
                fieldTypeList.append(item);
            }
        }
        if(xlsxread.load()){
            for(int col=1;col<=colCount;col++){
                if(currentOpenFileType==openFileType::OFDFile){
                    if(xlsxread.read(1,col).toString().replace("（","(").replace("）",")")!=ofd.getFieldList().at(col-1).getFieldDescribe().replace("（","(").replace("）",")")){
                        importExcelErrorDetail.append("选择的文件第"+Utils::CovertInt2ExcelCol(col-1)+"列不是["+ofd.getFieldList().at(col-1).getFieldDescribe()+"],请确认你导入的文件第一行标题和当前打开的文件列名一致,你可以使用数据导出功能导出一份Excel文件后编辑导入！");
                        return false;
                    }
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    if(xlsxread.read(1,col).toString().replace("（","(").replace("）",")")!=fixed.getFieldList().at(col-1).getFieldDescribe().replace("（","(").replace("）",")")){
                        importExcelErrorDetail.append("选择的文件第"+Utils::CovertInt2ExcelCol(col-1)+"列不是["+fixed.getFieldList().at(col-1).getFieldDescribe()+"],请确认你导入的文件第一行标题和当前打开的文件列名一致,你可以使用数据导出功能导出一份Excel文件后编辑导入！");
                        return false;
                    }
                }
            }
            QByteArray byteArrayRowT;
            for(int charIndex=0;charIndex<rowLength;charIndex++){
                byteArrayRowT.append(QChar(' ').toLatin1());
            }
            importExcelRow=0;
            int row=2;
            bool allColIsEmpty=true;
            while (true){
                QStringList rowdata;
                allColIsEmpty=true;
                for(int col=1;col<=colCount;col++){
                    std::string filedDecLengthString=QString("%.%1f").arg(fieldTypeList.at(col-1).fieldDecLength).toStdString();
                    QVariant colvalQVariant=xlsxread.read(row,col);
                    QString colval="";
                    if(colvalQVariant.type() == QVariant::Double){
                        if(fieldTypeList.at(col-1).fieldType=="N"){
                            colval=QString::asprintf(filedDecLengthString.c_str(),colvalQVariant.toDouble());
                        }
                        else{
                            colval=QString::asprintf("%.0f",colvalQVariant.toDouble());
                        }
                    }
                    else if(colvalQVariant.type() == QVariant::Int){
                        colval=QString::asprintf("%d",colvalQVariant.toInt());
                    }
                    else if(colvalQVariant.type() == QVariant::UInt){
                        colval=QString::asprintf("%d",colvalQVariant.toUInt());
                    }
                    else if(colvalQVariant.type() == QVariant::LongLong){
                        colval=QString::asprintf("%lld",colvalQVariant.toLongLong());
                    }
                    else if(colvalQVariant.type() == QVariant::ULongLong){
                        colval=QString::asprintf("%llu",colvalQVariant.toULongLong());
                    }
                    else{
                        if(fieldTypeList.at(col-1).fieldType=="N"){
                            bool ok=false;
                            colvalQVariant.toDouble(&ok);
                            if(ok){
                                colval=QString::asprintf(filedDecLengthString.c_str(),colvalQVariant.toDouble());
                            }
                            else{
                                colval=colvalQVariant.toString();
                            }
                        }else{
                            colval=colvalQVariant.toString();
                        }
                    }
                    if(!colval.isEmpty()){
                        allColIsEmpty=false;
                    }
                    int textLength=codec->fromUnicode(colval).length();
                    if(fieldTypeList.at(col-1).fieldType=="C"||fieldTypeList.at(col-1).fieldType=="TEXT"){
                        if(textLength>fieldTypeList.at(col-1).fieldLength){
                            importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]长度不应当大于[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength)));
                        }
                    }
                    else if(fieldTypeList.at(col-1).fieldType=="A"){
                        if(textLength>fieldTypeList.at(col-1).fieldLength){
                            importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]长度不应当大于[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength)));
                        }
                    }
                    else if(fieldTypeList.at(col-1).fieldType=="N"){
                        if(!colval.isEmpty()){
                            bool ok=false;
                            colval.toDouble(&ok);
                            if(!ok){
                                importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]应当是一个数值或者为空,但是现在看起来并不是").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe));
                            }
                            else{
                                if(!colval.contains("."))
                                {
                                    if(textLength>(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)){
                                        importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]整数部分长度不能超过[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)));
                                    }
                                }
                                else{
                                    int index=colval.indexOf(".");
                                    QString intS=colval.mid(0,index);
                                    if(intS.length()>(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)){
                                        importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]的整数部分长度不能超过[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength-fieldTypeList.at(col-1).fieldDecLength)));
                                    }
                                    else {
                                        QString intD=colval.mid(index+1,-1);
                                        if(intD.length()>fieldTypeList.at(col-1).fieldDecLength){
                                            colval=colval.mid(0,index+fieldTypeList.at(col-1).fieldDecLength+1);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else{
                        if(textLength>fieldTypeList.at(col-1).fieldLength){
                            importExcelErrorDetail.append(QString("Excel第[%1]行,字段[%2]的长度不应当大于[%3]").arg(QString::number(row),fieldTypeList.at(col-1).fieldDescribe,QString::number(fieldTypeList.at(col-1).fieldLength)));
                        }
                    }
                    rowdata.append(colval);
                }
                if(allColIsEmpty){
                    break;
                }
                else if (importExcelErrorDetail.count()==0){
                    QByteArray byteArrayRow=byteArrayRowT;
                    if(rowdata.count()>0){
                        //多种长度情境下自适应行长度
                        if(currentOpenFileType==openFileType::FIXEDFile&&fixed.getRowLengthHash().count()>1){
                            int rowCountNew=rowdata.count();
                            for(int iii=rowdata.count()-1;iii>0;iii--){if(rowdata.at(iii).isEmpty()){rowCountNew--;}else{break;}}
                            if (rowCountNew<colCount&&!fixedFileFieldCountList.contains(rowCountNew)){while(true){rowCountNew++;if(fixedFileFieldCountList.contains(rowCountNew)){break;}}}
                            if(rowCountNew<colCount){rowdata=rowdata.mid(0,rowCountNew);byteArrayRow=byteArrayRow.mid(0,fixed.getFieldList().at(rowdata.count()).getRowBeginIndex());}
                        }
                        for(int editCol=0;editCol<rowdata.count()&&editCol<colCount;editCol++){
                            QString valueNew=rowdata.at(editCol);
                            bool flag=Utils::updateOFDOrFixedFieldValueFromRow(fieldTypeList.at(editCol).fieldType,fieldTypeList.at(editCol).fieldLength,fieldTypeList.at(editCol).fieldDecLength,fieldTypeList.at(editCol).fieldBeginIndex,0,codec,valueNew,byteArrayRow);
                            if(!flag){
                                importExcelErrorDetail.append(QString("Excel第[%1]行数据导入时生成行数据异常,请重试...").arg(QString::number(row)));
                                break;
                            }
                        }
                    }
                    if(dataCompressLevel==0){
                        fileContentQByteArrayListFromExcel.append(byteArrayRow);
                    }
                    else{
                        fileContentQByteArrayListFromExcel.append(qCompress(byteArrayRow,dataCompressLevel));
                    }
                    if (importExcelRow>0&&importExcelRow%100==0){
                        emit signals_update_import_excel_status();
                    }
                }
                row++;
                importExcelRow++;
                if(row>1000000){
                    fileContentQByteArrayListFromExcel.clear();
                    importExcelErrorDetail.clear();
                    importExcelErrorDetail.append(QString("最多可以导入1,000,000行的Excel,看起来目前Excel数据太多了..."));
                    return false;
                }
            }
        }
        else{
            importExcelErrorDetail.append("加载Excel失败,请尝试重试或者确认文件是可读取的...");
        }
    }
    else{
        importExcelErrorDetail.append("加载Excel失败,请尝试重试或者确认文件是可读取的...");
    }
    if(importExcelErrorDetail.count()>0){
        return false;
    }
    else{
        return true;
    }
}

void MainWindow::importFromXlsxFinished(){
    //显示错误
    if (importExcelErrorDetail.count()>0){
        if(importExcelErrorDetail.count()==1&&!watcherXlsxImportStatus->result()){
            statusBar_disPlayMessage("导入失败,请查看具体失败原因...");
            DialogMyTip dialog2(importExcelErrorDetail.at(0),this,true);
            dialog2.setWindowTitle("导入失败警告！");
            dialog2.setModal(true);
            dialog2.exec();
        }
        //多条-展示为表格
        else if(!watcherXlsxImportStatus->result()){
            statusBar_disPlayMessage("导入失败,请查看具体失败原因...");
            DialogShowImportExcelError dialog2(&importExcelErrorDetail,this);
            dialog2.setWindowTitle("Excel导入失败详情");
            dialog2.setModal(true);
            dialog2.exec();
        }
        dataBlocked=false;
    }
    //导入数据
    else{
        dataBlocked=false;
        int rowCount=fileContentQByteArrayListFromExcel.count();
        if(rowCount==0){
            statusBar_disPlayMessage("导入的Excel有效数据行数为0...");
        }
        else{
            commonContentQByteArrayList=fileContentQByteArrayListFromExcel;
            fileContentQByteArrayListFromExcel.clear();
            reCalculateFileCountLine();
            int rowCount=commonContentQByteArrayList.count();
            clear_Table_Contents();
            ptr_table->setRowCount(rowCount);
            //添加完数据没有引发分页-原来也就1页
            if(rowCount<=pageRowSize){
                //关闭分页组件
                ui->framePage->setVisible(false);
                pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
                currentPage=1;
                fileChanged=true;
                rowHasloaded.clear();
                compareData.clear();
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable();
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable();
                }
            }
            //添加完数据数据大于1页，或者原本数据就大于1页-需要判断跳转的位置
            else{
                //展示分页组件
                ui->framePage->setVisible(true);
                statusBar_display_rowsCount(rowCount);
                //统计新的页数
                pageCount=(rowCount + pageRowSize - 1) / pageRowSize;
                fileChanged=true;
                rowHasloaded.clear();
                compareData.clear();
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable();
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable();
                }
            }
            acceptVScrollValueChanged(0);
            statusBar_display_rowsCount(rowCount);
            tableRowCurrent=0;
            tableColCurrent=0;
            this->setWindowTitle(appName+"-"+currentFileName+"-导入数据待保存");
            if(dialogDialogShowPrimaryCheck!=nullptr){
                needReCheckPrimary=true;
                statusBar_disPlayMessage(QString("导入完毕,从Excel导入数据%1行,请重新进行主键冲突检查...").arg(QString::number(rowCount)));
            }
            else{
                statusBar_disPlayMessage(QString("导入完毕,从Excel导入数据%1行").arg(QString::number(rowCount)));
            }
        }
    }
}

void MainWindow::showQrcode(){
    //获取数据
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    QString text="";
    int rangeCount=itemsRange.count();
    if(rangeCount>0){
        int topRow=itemsRange.at(0).topRow();
        int bottomRow=itemsRange.at(0).bottomRow();
        int leftColumn=itemsRange.at(0).leftColumn();
        int rigthColumn=itemsRange.at(0).rightColumn();
        //单个-只取值
        if(topRow==bottomRow&&leftColumn==rigthColumn){
            if(ptr_table->item(topRow,leftColumn)!=nullptr){
                text= ptr_table->item(topRow,leftColumn)->text();
            }
        }
        //多个
        else{
            //先判断选择的单元格数据
            if((bottomRow-topRow+1)*(rigthColumn-leftColumn+1)>120){
                statusBar_disPlayMessage("数据太多了,生成二维码的数据限制最多120个单元格且不大于2300字节");
                return;
            }else{
                //判断是跨行单列还是跨列单行
                //同一行的一些列--行列交换展示
                if((bottomRow==topRow)&&(rigthColumn!=leftColumn)){
                    for(int i=leftColumn;i<=rigthColumn;i++){
                        if(ptr_table->item(tableRowCurrent,i)==nullptr){
                            text.append(ptr_table->horizontalHeaderItem(i)->text());
                        }
                        else{
                            text.append(ptr_table->horizontalHeaderItem(i)->text()).append(" ").append(ptr_table->item(topRow,i)->text());
                        }
                        if(i<rigthColumn){
                            text.append("\r\n");
                        }
                    }
                }
                //同一列的多行-不要标题直接一行有一行
                else if((bottomRow!=topRow)&&(rigthColumn==leftColumn)){
                    int rowRealInContent=0;
                    if(currentOpenFileType==openFileType::OFDFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QString value=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,leftColumn);
                            if(ofd.getFieldList().at(leftColumn).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                    else if(currentOpenFileType==openFileType::CSVFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QStringList line=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                            QString value=line.count()>leftColumn?line.at(leftColumn):"";
                            if(CsvFieldIsNumberOrNot.contains(leftColumn)&&CsvFieldIsNumberOrNot.value(leftColumn).getIsNumber()){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                    else if(currentOpenFileType==openFileType::FIXEDFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QString value=Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,leftColumn);
                            if(fixed.getFieldList().at(leftColumn).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                    else if(currentOpenFileType==openFileType::DBFFile){
                        for(int row=topRow;row<=bottomRow;row++){
                            rowRealInContent=(currentPage-1)*pageRowSize+row;
                            QString value=Utils::getFormatValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,leftColumn,&dbfRowMap,dbfTrimType);
                            if(dbfColIsNumber(leftColumn)){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            if(row<bottomRow&&(!value.isEmpty())){
                                text.append("\r\n");
                            }
                        }
                    }
                }
                //多行多列
                else{
                    int rowRealInContent=0;
                    //标题行
                    for(int i=leftColumn;i<=rigthColumn;i++){
                        if(i<rigthColumn){
                            text.append(ptr_table->horizontalHeaderItem(i)->text()).append(" ");
                        }
                        else{
                            text.append(ptr_table->horizontalHeaderItem(i)->text()).append("\r\n");
                        }
                    }
                    //数据
                    for(int row=topRow;row<=bottomRow;row++){
                        rowRealInContent=(currentPage-1)*pageRowSize+row;
                        QStringList line;
                        if(currentOpenFileType==openFileType::OFDFile){
                            line=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::CSVFile){
                            line=Utils::getRowCsvValuesFromcsvFileContentQStringList(&commonContentQByteArrayList,&csv,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            line=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
                        }
                        else if(currentOpenFileType==openFileType::DBFFile){
                            line=Utils::getFormatRowValuesFromdbfTableFile(&dbftablefile,&dbf,rowRealInContent,&dbfRowMap,false,dbfTrimType);
                        }
                        for(int i=leftColumn;i<=rigthColumn&&i<line.count();i++){
                            QString value=line.at(i);
                            if (value.isEmpty()){
                                text.append("");
                            }
                            else if(currentOpenFileType==openFileType::OFDFile&&ofd.getFieldList().at(i).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else if(currentOpenFileType==openFileType::CSVFile&&CsvFieldIsNumberOrNot.contains(i)&&CsvFieldIsNumberOrNot.value(i).getIsNumber()){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }else if(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldList().at(i).getFieldType()=="N"){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else if(currentOpenFileType==openFileType::DBFFile&&dbfColIsNumber(i)){
                                text.append(Utils::CovertDoubleQStringWithThousandSplit(value));
                            }
                            else{
                                text.append(value);
                            }
                            //分割
                            if(i<rigthColumn&&(i<line.count()-1)){
                                text.append(" ");
                            }
                            //换行
                            else if((i==rigthColumn&&row<bottomRow)||((i==line.count()-1)&&row<bottomRow)){
                                text.append("\r\n");
                            }
                        }
                    }
                }
            }
        }
    }
    QString trimd=QString(text).replace("\r\n","").replace(" ","");
    if(!trimd.isEmpty()){
        if(text.toUtf8().count()>2300){
            statusBar_disPlayMessage(QString("数据太多了,生成二维码的数据限制最多120个单元格且不大于2300字节,当前%1字节").arg(QString::number(text.toUtf8().count())));
        }
        else{
            //打开窗口
            DialogShareQrCode * dialog = new DialogShareQrCode(text,this);
            dialog->setWindowTitle(QString("二维码分享数据-请扫一扫获得数据"));
            dialog->setModal(false);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
            dialog->raise();
            dialog->activateWindow();
        }
    }
    else{
        statusBar_disPlayMessage("没有有效数据可供生成二维码,数据为空或者全是空格!!!");
    }
}

QString MainWindow::HexStringToBinaryString(QString HexString){
    QString string="";
    if(HexString.length()>0){
        for(int i=0;i<HexString.length();i++){
            QChar ch=HexString.at(i);
            if(ch=='0'){
                string.append("0000");
            }
            else if(ch=='1'){
                string.append("0001");
            }
            else if(ch=='2'){
                string.append("0010");
            }
            else if(ch=='3'){
                string.append("0011");
            }
            else if(ch=='4'){
                string.append("0100");
            }
            else if(ch=='5'){
                string.append("0101");
            }
            else if(ch=='6'){
                string.append("0110");
            }
            else if(ch=='7'){
                string.append("0111");
            }
            else if(ch=='8'){
                string.append("1000");
            }
            else if(ch=='9'){
                string.append("1001");
            }
            else if(ch=='A'){
                string.append("1010");
            }
            else if(ch=='B'){
                string.append("1011");
            }
            else if(ch=='C'){
                string.append("1100");
            }
            else if(ch=='D'){
                string.append("1101");
            }
            else if(ch=='E'){
                string.append("1110");
            }
            else if(ch=='F'){
                string.append("1111");
            }
            else{
                return "ERROR";
            }
        }
    }
    return string;
}
void MainWindow::showModifyCell(){
    if(currentOpenFileType==openFileType::OFDFile){
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        int editRow=tableRowCurrent;
        int editCol=tableColCurrent;
        QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
        int fieldLength=ofd.getFieldList().at(editCol).getLength();
        int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
        QString fieldValues=Utils::getFormatValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent,editCol);
        bool modifyFlag=false;
        QString valueNew="";
        DialogModifyCell  dialog(fieldType,fieldLength,fieldDecLength,fieldValues,this);
        dialog.setWindowTitle(QString("编辑第%1行第%2列-%3").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1),ofd.getFieldList().at(tableColCurrent).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        valueNew=dialog.getValueNew();
        if(modifyFlag){
            if(valueNew==fieldValues){statusBar_disPlayMessage("单元格值没有变化,无需保存...");}
            else{
                int updateBegin=ofd.getFieldList().at(tableColCurrent).getRowBeginIndex();
                QByteArray rowByteArray;
                if(dataCompressLevel==0){rowByteArray=commonContentQByteArrayList.at(rowRealInContent);}
                else{rowByteArray=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                bool result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,0,codecOFD,valueNew,rowByteArray);
                if(result){
                    if(dataCompressLevel==0){
                        commonContentQByteArrayList.replace(rowRealInContent,rowByteArray);
                    }
                    else{
                        commonContentQByteArrayList.replace(rowRealInContent,qCompress(rowByteArray,dataCompressLevel));
                    }
                    rowHasloaded.remove(editRow);
                    display_OFDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                    }
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&ofd.getPrimaryKeyFieldList().contains(editCol)){
                        needReCheckPrimary=true;
                        statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");
                    }
                }
                else{statusBar_disPlayMessage("编辑失败,请重试...");}
            }
        }
        else{
            statusBar_disPlayMessage("取消编辑...");
        }
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        QTextCodec *codec=(QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()));
        int rowRealInContent=(currentPage-1)*pageRowSize+tableRowCurrent;
        int editRow=tableRowCurrent;
        int editCol=tableColCurrent;
        QString fieldType=fixed.getFieldList().at(editCol).getFieldType();
        int fieldLength=fixed.getFieldList().at(editCol).getLength();
        int fieldDecLength=fixed.getFieldList().at(editCol).getDecLength();
        QString fieldValues=Utils::getFormatValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent,editCol);
        bool modifyFlag=false;
        QString valueNew="";
        DialogModifyCell  dialog(fieldType,fieldLength,fieldDecLength,fieldValues,this);
        dialog.setWindowTitle(QString("编辑第%1行第%2列-%3").arg(QString::number(rowRealInContent+1),QString::number(tableColCurrent+1),fixed.getFieldList().at(tableColCurrent).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        valueNew=dialog.getValueNew();
        if(modifyFlag){
            if(valueNew==fieldValues){statusBar_disPlayMessage("单元格值没有变化,无需保存...");}
            else{
                int updateBegin=fixed.getFieldList().at(tableColCurrent).getRowBeginIndex();
                QByteArray rowByteArray;
                if(dataCompressLevel==0){rowByteArray=commonContentQByteArrayList.at(rowRealInContent);}
                else{rowByteArray=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                if(rowByteArray.length()<updateBegin+fieldLength){
                    while (true){
                        rowByteArray.append(' ');
                        if(fixed.getRowLengthHash().contains(rowByteArray.length())&&rowByteArray.length()>=updateBegin+fieldLength){
                            break;
                        }
                    }
                }
                bool result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,fixed.getFieldlengthtype(),codec,valueNew,rowByteArray);
                if(result){
                    if(dataCompressLevel==0){commonContentQByteArrayList.replace(rowRealInContent,rowByteArray);}
                    else{commonContentQByteArrayList.replace(rowRealInContent,qCompress(rowByteArray,dataCompressLevel));}
                    rowHasloaded.remove(editRow);
                    display_FIXEDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                    }
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&fixed.getPrimaryKeyFieldList().contains(editCol)){
                        needReCheckPrimary=true;
                        statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");
                    }
                }
                else{statusBar_disPlayMessage("编辑失败,请重试...");}
            }
        }
        else{statusBar_disPlayMessage("取消编辑...");}
    }
}

void MainWindow::showModifyCellBatch(){
    if(currentOpenFileType==openFileType::OFDFile){
        int editCol=tableColCurrent;
        QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
        int fieldLength=ofd.getFieldList().at(editCol).getLength();
        int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
        bool modifyFlag=false;
        DialogModifyCell dialog(fieldType,fieldLength,fieldDecLength,"",this);
        dialog.setWindowTitle(QString("批量编辑第%1列多个单元格-%2").arg(QString::number(editCol+1),ofd.getFieldList().at(editCol).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        QString valueNew=dialog.getValueNew();
        if(modifyFlag){
            QByteArray valueByteArray=codecOFD->fromUnicode(valueNew);
            bool flag=Utils::updateOFDOrFixedFieldValue(fieldType,fieldLength,fieldDecLength,0,codecOFD,valueNew,valueByteArray);
            if(flag){
                int rowRealInContent=0;
                QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
                if(itemsRange.count()>=1){
                    int updateBegin=ofd.getFieldList().at(editCol).getRowBeginIndex();
                    int updateEnd=updateBegin+fieldLength;
                    int updatedRow=0;
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            rowRealInContent=(currentPage-1)*pageRowSize+editRow;
                            QByteArray valueNewArrayRow;
                            if(dataCompressLevel==0){valueNewArrayRow=commonContentQByteArrayList.at(rowRealInContent);}
                            else{valueNewArrayRow=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                            int index=0;
                            for(int i=updateBegin;i<updateEnd&&i-updateBegin<valueByteArray.length();i++){
                                valueNewArrayRow[i]=valueByteArray.at(index);
                                index++;
                            }
                            if(dataCompressLevel==0){commonContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);}
                            else{commonContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));}
                            if(compareData.contains(editRow+1)){
                                compareData.remove(editRow+1);
                                compareData.insert(editRow+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,editRow));
                            }
                            updatedRow++;
                            if((updatedRow%1000==0)){
                                statusBar_disPlayMessage(QString("批量更新中,请勿进行其他操作,已更新%1行").arg(QString::number(updatedRow)));
                                qApp->processEvents();
                                if(abortAndExit){
                                    QApplication::restoreOverrideCursor();
                                    qDebug()<<"强制立即终止任务";
                                    return;
                                }
                            }
                        }
                    }
                    clear_Table_Contents();
                    rowHasloaded.clear();
                    display_OFDTable(true);
                    for(int rangeIndex2=0;rangeIndex2<itemsRange.count();rangeIndex2++){ptr_table->setRangeSelected(itemsRange.at(rangeIndex2),true);}
                    QApplication::restoreOverrideCursor();
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&ofd.getPrimaryKeyFieldList().contains(editCol)){needReCheckPrimary=true;statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");}
                }
                ptr_table->resizeColumnsToContents();
            }
            else{statusBar_disPlayMessage("编辑失败,请重试...");}
        }
        else{statusBar_disPlayMessage("取消编辑...");}
    }
    else if(currentOpenFileType==openFileType::FIXEDFile){
        QTextCodec *codec=(QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit()));
        int editCol=tableColCurrent;
        QString fieldType=fixed.getFieldList().at(editCol).getFieldType();
        int fieldLength=fixed.getFieldList().at(editCol).getLength();
        int fieldDecLength=fixed.getFieldList().at(editCol).getDecLength();
        bool modifyFlag=false;
        DialogModifyCell dialog(fieldType,fieldLength,fieldDecLength,"",this);
        dialog.setWindowTitle(QString("批量编辑第%1列多个单元格-%2").arg(QString::number(editCol+1),fixed.getFieldList().at(editCol).getFieldDescribe()));
        dialog.setModal(true);
        dialog.exec();
        modifyFlag=dialog.getModifyFlag();
        QString valueNew=dialog.getValueNew();
        if(modifyFlag){
            QByteArray valueByteArray=codec->fromUnicode(valueNew);
            bool flag=Utils::updateOFDOrFixedFieldValue(fieldType,fieldLength,fieldDecLength,fixed.getFieldlengthtype(),codec,valueNew,valueByteArray);
            if(flag){
                int rowRealInContent=0;
                QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
                if(itemsRange.count()>=1){
                    int updateBegin=fixed.getFieldList().at(editCol).getRowBeginIndex();
                    int updateEnd=updateBegin+fieldLength;
                    int updatedRow=0;
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                    for(int rangeIndex=0;rangeIndex<itemsRange.count();rangeIndex++){
                        for(int editRow=itemsRange.at(rangeIndex).topRow();editRow<=itemsRange.at(rangeIndex).bottomRow();editRow++){
                            rowRealInContent=(currentPage-1)*pageRowSize+editRow;
                            QByteArray valueNewArrayRow;
                            if(dataCompressLevel==0){valueNewArrayRow=commonContentQByteArrayList.at(rowRealInContent);}
                            else{valueNewArrayRow=qUncompress(commonContentQByteArrayList.at(rowRealInContent));}
                            int index=0;
                            if(valueNewArrayRow.length()<updateEnd){
                                while (true){
                                    valueNewArrayRow.append(' ');
                                    if(fixed.getRowLengthHash().contains(valueNewArrayRow.length())&&valueNewArrayRow.length()>=updateEnd){
                                        break;
                                    }
                                }
                            }
                            for(int i=updateBegin;i<updateEnd&&i-updateBegin<valueByteArray.length();i++){
                                valueNewArrayRow[i]=valueByteArray.at(index);
                                index++;
                            }
                            if(dataCompressLevel==0){commonContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);}
                            else{commonContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));}
                            if(compareData.contains(editRow+1)){
                                compareData.remove(editRow+1);
                                compareData.insert(editRow+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,editRow));
                            }
                            updatedRow++;
                            if((updatedRow%1000==0)){
                                statusBar_disPlayMessage(QString("批量更新中,请勿进行其他操作,已更新%1行").arg(QString::number(updatedRow)));
                                qApp->processEvents();
                                if(abortAndExit){
                                    QApplication::restoreOverrideCursor();
                                    qDebug()<<"强制立即终止任务";
                                    return;
                                }
                            }
                        }
                    }
                    clear_Table_Contents();
                    rowHasloaded.clear();
                    display_FIXEDTable(true);
                    for(int rangeIndex2=0;rangeIndex2<itemsRange.count();rangeIndex2++){ptr_table->setRangeSelected(itemsRange.at(rangeIndex2),true);}
                    QApplication::restoreOverrideCursor();
                    statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                    this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                    fileChanged=true;
                    if(dialogDialogShowPrimaryCheck!=nullptr&&fixed.getPrimaryKeyFieldList().contains(editCol)){needReCheckPrimary=true;statusBar_disPlayMessage("主键列数据被修改了,请重新进行主键冲突检查...");}
                }
                ptr_table->resizeColumnsToContents();
            }
            else{statusBar_disPlayMessage("编辑失败,请重试...");}
        }
        else{statusBar_disPlayMessage("取消编辑...");}
    }
}

void MainWindow::showMoaifyRow(){
    QList<QTableWidgetSelectionRange> itemsRange=ptr_table->selectedRanges();
    if(itemsRange.count()>0){
        int editRow=itemsRange.at(0).topRow();
        int rowRealInContent=(currentPage-1)*pageRowSize+editRow;
        QStringList rowDataOld;
        QList<fieldType> fieldTypeList;
        QTextCodec *codec=QTextCodec::codecForName("GB18030");;
        if(currentOpenFileType==openFileType::OFDFile){
            codec=codecOFD;
            rowDataOld=Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent);
            for(int i=0;i<ofd.getFieldList().count();i++){
                fieldType item;
                item.fieldType=ofd.getFieldList().at(i).getFieldType();
                item.fieldLength=ofd.getFieldList().at(i).getLength();
                item.fieldDecLength=ofd.getFieldList().at(i).getDecLength();
                item.fieldName=ofd.getFieldList().at(i).getFieldName();
                item.fieldDescribe=ofd.getFieldList().at(i).getFieldDescribe();
                fieldTypeList.append(item);
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            rowDataOld=Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent);
            for(int i=0;i<fixed.getFieldList().count();i++){
                fieldType item;
                item.fieldType=fixed.getFieldList().at(i).getFieldType();
                item.fieldLength=fixed.getFieldList().at(i).getLength();
                item.fieldDecLength=fixed.getFieldList().at(i).getDecLength();
                item.fieldName=fixed.getFieldList().at(i).getFieldName();
                item.fieldDescribe=fixed.getFieldList().at(i).getFieldDescribe();
                fieldTypeList.append(item);
                if(rowDataOld.count()-1<i){
                    rowDataOld.append(QString(""));
                }
            }
        }
        DialogModifyRow * dialog = new DialogModifyRow(fieldTypeList,codec,rowDataOld,this);
        dialog->setWindowTitle(QString("编辑第%1行数据").arg(QString::number(rowRealInContent+1)));
        dialog->setModal(true);
        dialog->exec();
        ///////////////////////////////开始进行数据编辑更新//////////////////////////////////
        if(dialog->getChange()&&dialog->getSaveFlag()){
            QStringList rowDataNew=dialog->getRowDataNew();
            delete dialog;
            dialog=nullptr;
            if(rowDataNew.count()>0){
                QByteArray valueNewArrayRow;
                if(dataCompressLevel==0){
                    valueNewArrayRow=commonContentQByteArrayList.at(rowRealInContent);
                }
                else{
                    valueNewArrayRow=qUncompress(commonContentQByteArrayList.at(rowRealInContent));
                }
                for(int editCol=0;editCol<rowDataNew.count()&&editCol<ptr_table->columnCount();editCol++){
                    QString fieldValues=rowDataOld.count()>editCol?rowDataOld.at(editCol):"";
                    QString valueNew=rowDataNew.count()>editCol?rowDataNew.at(editCol):"";
                    bool result=false;
                    if(fieldValues!=valueNew){
                        if(currentOpenFileType==openFileType::OFDFile){
                            QString fieldType=ofd.getFieldList().at(editCol).getFieldType();
                            int fieldLength=ofd.getFieldList().at(editCol).getLength();
                            int fieldDecLength=ofd.getFieldList().at(editCol).getDecLength();
                            int updateBegin=ofd.getFieldList().at(editCol).getRowBeginIndex();
                            result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,0,codec,valueNew,valueNewArrayRow);
                        }
                        else if(currentOpenFileType==openFileType::FIXEDFile){
                            QString fieldType=fixed.getFieldList().at(editCol).getFieldType();
                            int fieldLength=fixed.getFieldList().at(editCol).getLength();
                            int fieldDecLength=fixed.getFieldList().at(editCol).getDecLength();
                            int updateBegin=fixed.getFieldList().at(editCol).getRowBeginIndex();
                            if(valueNewArrayRow.length()-1<updateBegin){
                                while (true){
                                    valueNewArrayRow.append(' ');
                                    if(fixed.getRowLengthHash().contains(valueNewArrayRow.length())&&valueNewArrayRow.length()>=updateBegin+fieldLength){
                                        break;
                                    }
                                }
                            }
                            result=Utils::updateOFDOrFixedFieldValueFromRow(fieldType,fieldLength,fieldDecLength,updateBegin,0,codec,valueNew,valueNewArrayRow);
                        }
                        if(!result){
                            statusBar_disPlayMessage("编辑失败,请重试...");
                            return;
                        }
                    }
                }
                if(dataCompressLevel==0){
                    commonContentQByteArrayList.replace(rowRealInContent,valueNewArrayRow);
                }
                else{
                    commonContentQByteArrayList.replace(rowRealInContent,qCompress(valueNewArrayRow,dataCompressLevel));
                }
                rowHasloaded.remove(editRow);
                if(currentOpenFileType==openFileType::OFDFile){
                    display_OFDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromofdFileContentQByteArrayList(&commonContentQByteArrayList,&ofd,dataCompressLevel,rowRealInContent));
                    }
                }
                else if(currentOpenFileType==openFileType::FIXEDFile){
                    display_FIXEDTable(true);
                    if(compareData.contains(rowRealInContent+1)){
                        compareData.remove(rowRealInContent+1);
                        compareData.insert(rowRealInContent+1,Utils::getFormatRowValuesFromfixedFileContentQStringList(&commonContentQByteArrayList,&fixed,dataCompressLevel,rowRealInContent));
                    }
                }
                statusBar_disPlayMessage("单元格数据修改成功,请记得保存文件哟...");
                this->setWindowTitle(appName+"-"+currentFileName+"-修改待保存");
                fileChanged=true;
                if(dialogDialogShowPrimaryCheck!=nullptr){
                    needReCheckPrimary=true;
                    statusBar_disPlayMessage("检测到数据行更新,请重新进行主键冲突检查...");
                }
            }
        }
        else{
            if(!dialog->getSaveFlag()){
                statusBar_disPlayMessage("放弃编辑...");
            }
            if(dialog->getSaveFlag()&&!dialog->getChange()){
                statusBar_disPlayMessage("数据未修改,无需更新记录...");
            }
            delete dialog;
            dialog=nullptr;
        }
    }
}

bool MainWindow::compareOFDData(const OFDFileDefinition &ofd1, const OFDFileDefinition &ofd2){
    if (ofd1.getConfigSegment() < ofd2.getConfigSegment())
    {
        return true;
    }
    else{
        return false;
    }
}

bool MainWindow::compareDBFData(const dbfMatchInfo &dbf1, const dbfMatchInfo &dbf2){
    if (dbf1.matchDegree > dbf2.matchDegree)
    {
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief MainWindow::on_actionSave_triggered 文件保存功能槽函数
 */
void MainWindow::on_actionSave_triggered()
{
    if(updateOrBlcokCheck()){
        return;
    }
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
    }
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        if(fileChanged){
            //先备份原文件
            if(Utils::isFileExist(currentOpenFilePath)){
                QFile oldfile(currentOpenFilePath);
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyyMMddhhmmss");
                bool r=oldfile.rename(oldfile.fileName()+"."+current_date+".BAK");
                if(!r){
                    statusBar_disPlayMessage("备份原文件失败,请重试或检查文件是否被其他程序占用");
                    return;
                }
            }
            //原文件备份完毕后开始保存新的文件,文件名和之前保持一致
            saveOFDOrFixedFile(currentOpenFilePath);
            if(fileFromZip){
                qDebug()<<"保存的文件来自zip";
                qDebug()<<zipfilePath;
                qDebug()<<zipTargetFileInZip;
                DialogMyTip  dialog2(QString("当前保存的文件来自压缩包,如果你需要将修改后的文件更新到压缩包,请自行使用压缩软件更新\r\n压缩包路径:%1\r\n文件在压缩包中的路径:%2\r\n当前文件保存路径:%3").arg(zipfilePath,zipTargetFileInZip,currentOpenFilePath),this,true);
                dialog2.setWindowTitle("温馨提示");
                dialog2.setModal(true);
                dialog2.exec();
            }
        }
        else{
            statusBar_disPlayMessage("文件没有被修改,无需保存");
        }
    }
    else if(currentOpenFileType==openFileType::CSVFile||currentOpenFileType==openFileType::FIXEDFile||currentOpenFileType==openFileType::DBFFile){
        statusBar_disPlayMessage("暂不支持保存此类文件");
    }
}

void MainWindow::saveOFDOrFixedFile(QString filepath)
{
    //检测阻断的任务
    if(dataBlocked){
        statusBar_disPlayMessage(dataBlockedMessage);
        return;
    }
    if(Utils::isFileExist(filepath)){
        QFile oldfile(filepath);
        bool r=oldfile.remove();
        if(!r){statusBar_disPlayMessage("覆盖文件失败,请重试或检查文件是否被其他程序占用");return;}
    }
    if(!lastRowHasNewLine){
        DialogMyTip  dialog2("当前文件最后一行没有换行符,是否在保存时在最后一行添加换行符？\r\n备注:如果你不确定,请选择\"否\"或关闭该弹窗,将按文件当前特性不添加换行符\r\n",this);
        dialog2.setWindowTitle("温馨提示");
        dialog2.setModal(true);
        dialog2.exec();
        if(dialog2.getBoolFlag()){
            lastRowHasNewLine=true;
        }
    }
    dataBlocked=true;
    dataBlockedMessage="正在保存写入文件,请稍后再操作...";
    QFile newfile(filepath);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (newfile.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QString newLine="\r\n";
        if (currentFileNewLineType==newLineType::LF){
            newLine="\n";
        }
        else if (currentFileNewLineType==newLineType::CR){
            newLine="\r";
        }
        if(currentOpenFileType==openFileType::OFDFile){
            QString sb;
            for(int i=0;i<commonHeaderQStringList.length();i++){
                sb.append(commonHeaderQStringList.at(i)).append(newLine);
            }
            newfile.write(codecOFD->fromUnicode(sb));
            sb.clear();
            int row=0;
            while(row<commonContentQByteArrayList.length()){
                if(dataCompressLevel==0){
                    sb.append(codecOFD->toUnicode(commonContentQByteArrayList.at(row))).append(newLine);
                }
                else{
                    sb.append(codecOFD->toUnicode(qUncompress(commonContentQByteArrayList.at(row)))).append(newLine);
                }
                if((row%1000==0)||(row==commonContentQByteArrayList.count()-1)){
                    newfile.write(codecOFD->fromUnicode(sb));
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                    if(abortAndExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                row++;
            }
            if(lastRowHasNewLine){
                newfile.write(codecOFD->fromUnicode(ofdFooterQString+newLine));
            }
            else{
                newfile.write(codecOFD->fromUnicode(ofdFooterQString));
            }
        }
        else if(currentOpenFileType==openFileType::FIXEDFile){
            QTextCodec *codec=QTextCodec::codecForName(fixed.getEcoding().toLocal8Bit());
            QString sb;
            for(int i=0;i<commonHeaderQStringList.count();i++){
                sb.append(commonHeaderQStringList.at(i)).append(newLine);
            }
            newfile.write(codec->fromUnicode(sb));
            sb.clear();
            int row=0;
            while(row<commonContentQByteArrayList.length()){
                if(dataCompressLevel==0){
                    sb.append(codec->toUnicode(commonContentQByteArrayList.at(row))).append(newLine);
                }
                else{
                    sb.append(codec->toUnicode(qUncompress(commonContentQByteArrayList.at(row)))).append(newLine);
                }
                if((row%1000==0)||(row==commonContentQByteArrayList.count()-1)){
                    newfile.write(codec->fromUnicode(sb));
                    sb.clear();
                    statusBar_disPlayMessage(QString("文件保存中,请勿进行其他操作,已导出%1行").arg(QString::number(row)));
                    qApp->processEvents();
                    if(abortAndExit){
                        QApplication::restoreOverrideCursor();
                        qDebug()<<"强制立即终止任务";
                        return;
                    }
                }
                row++;
            }
            for(int i=0;i<commonFooterQStringList.count();i++){
                if(i<commonFooterQStringList.count()-1){
                    newfile.write(codec->fromUnicode(commonFooterQStringList.at(i)+newLine));
                }
                else{
                    if(lastRowHasNewLine){
                        newfile.write(codec->fromUnicode(commonFooterQStringList.at(i)+newLine));
                    }
                    else{
                        newfile.write(codec->fromUnicode(commonFooterQStringList.at(i)));
                    }
                }
            }
        }
        newfile.flush();
        newfile.close();
        fileChanged=false;
        statusBar_disPlayMessage(tr("文件保存完毕,保存在%1").arg(filepath));
        this->setWindowTitle(appName+"-"+currentFileName);
        currentOpenFilePath=filepath;
        ui->currentOpenFilePathLineText->setText(currentOpenFilePath);
    }else{
        statusBar_disPlayMessage(tr("数据保存失败,请重试"));
    }
    //恢复鼠标响应
    dataBlocked=false;
    QApplication::restoreOverrideCursor();
}

/**
 * @brief MainWindow::on_actionSaveAS_triggered 文件另存功能
 */
void MainWindow::on_actionSaveAS_triggered()
{
    if(updateOrBlcokCheck()){
        return;
    }
    if(currentOpenFilePath.isEmpty()){
        return;
    }
    if(currentOpenFileType==openFileType::OFDIndex){
        statusBar_disPlayMessage("索引文件不支持编辑保存");
        return;
    }
    if(currentOpenFileType==openFileType::OFDFile||(currentOpenFileType==openFileType::FIXEDFile&&fixed.getFieldlengthtype()==0)){
        if(currentOpenFileType==openFileType::OFDFile&&commonHeaderQStringList.size()<1){
            statusBar_disPlayMessage("未打开有效的OFD/类OFD文件,无法使用另存功能,请尝试打开一个有效的文件...");
            return;
        }
        QString selectedFilter=Q_NULLPTR;
        QString fileNameSave = QFileDialog::getSaveFileName(this,("另存为"),currentOpenFilePath,tr("所有文件(*);;文本文件(*.TXT)"),&selectedFilter);
        if(!fileNameSave.isEmpty()){
            if(selectedFilter=="文本文件(*.TXT)"&&(!fileNameSave.endsWith(".TXT"))){
                fileNameSave.append(".TXT");
            }
            saveOFDOrFixedFile(fileNameSave);
        }
    }
    else{
        statusBar_disPlayMessage("暂不支持另存此类文件");
    }
}

QString MainWindow::StringToHexStringWithEnCode(QString data,QTextCodec *codec,bool withSouceChar){
    QString sb;
    if(withSouceChar){
        for(int i=0;i<data.length();i++){
            //兼容4字节Unicode字符
            if(data.at(i).isHighSurrogate()&&i<data.count()-1&&data.at(i+1).isLowSurrogate())
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 99)
                sb.append("[").append(data.midRef(i,2)).append(":").append(codec->fromUnicode(data.midRef(i,2)).toHex().toUpper()).append("]");
#else
                sb.append("[").append(data.midRef(i,2)).append(":").append(codec->fromUnicode(data.mid(i,2)).toHex().toUpper()).append("]");
#endif
                i++;
                if(i<data.length()-1){
                    sb.append("-");
                }
            }
            else{
                sb.append("[").append(data.at(i)).append(":").append(codec->fromUnicode(data.at(i)).toHex().toUpper()).append("]");
                if(i<data.length()-1){
                    sb.append("-");
                }
            }
        }
    }
    else{
        sb=codec->fromUnicode(data).toHex().toUpper();
        int len = sb.length()/2;
        for(int i=1;i<len;i++)
        {
            sb.insert(2*i+i-1," ");
        }
    }
    return sb;
}
