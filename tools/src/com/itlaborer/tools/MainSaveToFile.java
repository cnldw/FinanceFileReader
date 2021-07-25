package com.itlaborer.tools;

import java.io.File;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.apache.commons.lang.StringUtils;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class MainSaveToFile {
	public static void main(String[] args) {
		// 目标URL-从这里探测页数--第一页
		String url = "http://www.csisc.cn/zbscbzw/jijin/index_xsr_list.shtml";
		// pagebaseurl
		String pageurlbase = "http://www.csisc.cn/zbscbzw/jijin/";
		// 网站地址
		String websiturl = "http://www.csisc.cn";

		// 记录总页数
		int count = 1;

		// 头伪装
		LinkedHashMap<String, String> header = new LinkedHashMap<String, String>();
		header.put("Accept",
				"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
		header.put("Accept-Encoding", "gzip, deflate");
		header.put("Accept-Language", "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6");
		header.put("Cache-Control", "max-age=0");
		header.put("Connection", "keep-alive");
		header.put("DNT", "1");
		header.put("Host", "www.csisc.cn");
		header.put("Referer", "http://www.csisc.cn");
		header.put("Upgrade-Insecure-Requests", "1");
		header.put("User-Agent",
				"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.164 Safari/537.36 Edg/91.0.864.71");
		boolean getPageOk = false;
		// 探测总页数
		String reString;
		try {
			reString = HttpUtils.httpGet(url, new HashMap<String, String>(), new LinkedHashMap<String, String>(),
					header, Charset.forName("UTF-8")).readToText();
			if (StringUtils.contains(reString, "page_div")) {
				Document document = Jsoup.parse(reString);
				// 获取分页组件
				Elements elements = document.getElementsByTag("script");
				for (Iterator<Element> iterator = elements.iterator(); iterator.hasNext();) {
					Element element = (Element) iterator.next();
					// 找到包含总页数的script标签
					if (StringUtils.contains(element.html(), "page_div")) {
						List<String> ls = new ArrayList<String>();
						Pattern pattern = Pattern.compile("(?<=page_div',).*?(?=[,~])");
						Matcher matcher = pattern.matcher(element.html());
						while (matcher.find()) {
							ls.add(matcher.group());
						}
						System.out.println(ls);
						if (ls.size() > 0) {
							try {
								count = Integer.parseInt(ls.get(0));
								System.out.println("**************共计有" + count + "页数据**************");
								getPageOk = true;
								break;
							} catch (Exception e) {
								continue;
							}
						} else {
							System.out.println("找不到总页数,可能是网站改版了,程序退出");
						}

					}
				}
			} else {
				System.out.println("找不到总页数,可能是网站改版了,程序退出");
			}
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("抓取异常,可能是网站改版了,程序退出");
			return;
		}
		// 获取分页OK，开始获取数据
		if (getPageOk) {
			// 存储公司列表
			ArrayList<String> companyList = new ArrayList<>();
			for (int i = 1; i <= count; i++) {
				String pageurl = "";
				if (i == 1) {
					pageurl = url;
				} else {
					pageurl = pageurlbase + "index_xsr_list_" + i + ".shtml";
				}
				System.out.println("开始抓取第" + i + "页公司列表,地址为:" + pageurl);
				try {
					reString = HttpUtils.httpGet(pageurl, new HashMap<String, String>(), header,
							new LinkedHashMap<String, String>(), Charset.forName("UTF-8")).readToText();
					if (StringUtils.contains(reString, "page_div")) {
						Document document = Jsoup.parse(reString);
						// 获取table
						Elements elements = document.getElementsByTag("table");
						for (Iterator<Element> iterator = elements.iterator(); iterator.hasNext();) {
							// 获取tr
							Element element = (Element) iterator.next();
							Elements herflist = element.getElementsByTag("a");
							for (Iterator<Element> iterator2 = herflist.iterator(); iterator2.hasNext();) {
								// 获取该公司的链接
								Element elementherf = (Element) iterator2.next();
								String comurl = elementherf.attr("href");
								String comname = elementherf.attr("title");
								// 加个正则匹配，正儿八经的公司地址里有一串md5
								Pattern pattern = Pattern.compile("([a-f\\d]{32}|[A-F\\d]{32})");
								Matcher matcher = pattern.matcher(comurl);
								while (matcher.find()) {
									System.out.println("正则匹配成功" + comurl + ":" + comname);
									// 存起来
									if (!companyList.contains(comurl)) {
										companyList.add(comurl);
										break;
									}
								}
							}
						}
					} else {
						System.out.println("找不到总页数,可能是网站改版了,程序退出");
					}
				} catch (Exception e) {
					e.printStackTrace();
					System.out.println("抓取异常,可能是网站改版了,程序退出");
					return;
				}
				// 休眠，避免请求过于频繁
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			// 遍历每个公司获取最终的数据
			if (companyList.size() > 0) {
				// 开始抓取数据
				StringBuilder sbCsv = new StringBuilder();
				StringBuilder sbAgency = new StringBuilder();
				// 本次抓取的所有编码信息
				ArrayList<String> codelistallthistime = new ArrayList<>();
				// 开始按公司抓取信息
				for (int i = 0; i < companyList.size(); i++) {
					try {
						String companyurl = companyList.get(i);
						System.out.println("*************开始抓取" + websiturl + companyurl + "数据*****************");
						// 获取页面数据
						String response = HttpUtils.httpGet(websiturl + companyurl, new HashMap<String, String>(),
								header, new LinkedHashMap<String, String>(), Charset.forName("UTF-8")).readToText();
						Document document = Jsoup.parse(response);
						// 这里是公司信息
						Elements elements = document.getElementsByClass("con_nr").get(0).getElementsByTag("p");
						String companyname = "";
						ArrayList<String> codelist = new ArrayList<>();
						ArrayList<String> codelistwithtype = new ArrayList<>();
						if (elements.size() > 0) {
							for (Element element : elements) {
								String[] array = element.text().split("：");
								// 添加公司名称
								if (StringUtils.contains(element.text(), "机构名称") && array.length == 2) {
									companyname = array[1];
									System.out.println(array[1]);
								}
								// 添加各种编码信息
								else if ((!StringUtils.contains(element.text(), "参与方编码"))
										&& (!StringUtils.contains(element.text(), "编码类型"))
										&& (!StringUtils.contains(element.text(), "机构类型"))
										&& (!StringUtils.contains(element.text(), "机构名称"))
										&& (!StringUtils.contains(element.text(), "带*数据"))
										&& (!StringUtils.contains(element.text(), "其他机构"))
										&& (!StringUtils.contains(element.text(), "后两位数据无含义")) && array.length == 2) {
									// 正则匹配,2-9位的数字或者字母认为是有效的编码
									Pattern pattern = Pattern.compile("([a-f\\d]{2,9}|[A-F\\d]{2,9})");
									Matcher matcher = pattern.matcher(array[1]);
									while (matcher.find()) {
										System.out.println("正则匹配成功" + array[1]);
										if (!codelist.contains(array[1])) {
											codelist.add(array[1]);
											codelistallthistime.add(array[1]);
										}
										codelistwithtype.add(array[1] + "|" + array[0]);
										System.out.println(array[1] + "|" + array[0]);
										break;
									}
								}
							}
						}
						if (codelist.size() > 0) {
							// 配置文件，不区分编码类别
							for (int b = 0; b < codelist.size(); b++) {
								String code = codelist.get(b);
								// agency组装
								String agenvy = "[" + code + "]\r\nAGENCYNO=\"" + code + "\"\r\nNAME=\"" + companyname
										+ "\"";
								sbAgency.append(agenvy).append("\r\n");
							}
							// csv区分编码类别
							for (int b = 0; b < codelistwithtype.size(); b++) {
								String codewithtype = codelistwithtype.get(b);
								// csv组装
								String csv = companyname + "|" + codewithtype;
								sbCsv.append(csv).append("\r\n");
							}
						}
					} catch (Exception e) {
						e.printStackTrace();
					}
					// 休眠，避免请求过于频繁
					try {
						Thread.sleep(600);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
				// 结束保存到文件-如果本次抓到了数据
				if (codelistallthistime.size() > 0) {
					// 资本市场标准王改版后，数据不全，添加历史base数据，你自己的编码也可以维护到base数据
					if (FileUtils.fileIsExists("./basedata.txt")) {
						ArrayList<String> listbasedata = FileUtils.readFileToArrayList("./basedata.txt", "UTF-8");
						if (!listbasedata.equals(null) && listbasedata.size() > 0) {
							System.out.println("正在分析添加额外的数据");
							for (int a = 0; a < listbasedata.size(); a++) {
								String[] line = listbasedata.get(a).split("=");
								if (line.length == 2) {
									String code = line[0];
									if (!codelistallthistime.contains(code)) {
										System.out.println("正在分析添加额外的数据" + code);
										// agency组装
										String agenvy = "[" + code + "]\r\nAGENCYNO=\"" + code + "\"\r\nNAME=\""
												+ line[1] + "\"";
										sbAgency.append(agenvy).append("\r\n");
										// csv组装
										String csv = line[1] + "|" + code+"|";
										sbCsv.append(csv).append("\r\n");
									}
								}
							}
						}
					}
					FileUtils.saveToFile(new File("./OFD_CodeInfo.csv"), sbCsv.toString());
					FileUtils.saveToFile(new File("./OFD_CodeInfo.ini"), sbAgency.toString());
					System.out.println("*抓取结束文件已保存");
				}
			}
		}
	}
}
