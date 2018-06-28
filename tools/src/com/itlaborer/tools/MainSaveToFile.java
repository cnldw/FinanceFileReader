package com.itlaborer.tools;

import java.io.File;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;

import org.apache.commons.lang.StringUtils;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class MainSaveToFile {
	public static void main(String[] args) {
		// 目标URL
		String url = "http://www.csisc.cn/search-webapp/foc/codeSearch_search.html";
		// 默认请求参数
		HashMap<String, String> parameters = new HashMap<>();
		parameters.put("manageCode", "");
		parameters.put("org.cyfFname", "");
		parameters.put("a", "");
		parameters.put("businessId", "");
		parameters.put("currPage", "");
		// 此参数代表当前页码
		parameters.put("pageData.pageNeeded", "1");
		parameters.put("pageData.pageSize", "10");
		parameters.put("pageData.maxPageSize", "100");
		parameters.put("pageData.count", "698");
		parameters.put("pageCount", "70");
		parameters.put("pageData.isNeedCount", "false");

		// 记录总页数
		int count = 1;
		/**
		 * 探测总页数--探测原理为在第一页找总页数标签，提取总页数信息
		 */
		String reString;
		try {
			reString = HttpUtils.httpPost(url, parameters, new LinkedHashMap<String, String>(),
					new LinkedHashMap<String, String>(), Charset.forName("UTF-8")).readToText();
			if (StringUtils.contains(reString, "共")) {
				Document document = Jsoup.parse(reString);
				Elements elements = document.getElementsContainingText("共");
				for (Iterator<Element> iterator = elements.iterator(); iterator.hasNext();) {
					Element element = (Element) iterator.next();
					if (StringUtils.contains(element.text(), "页")) {
						String countS = StringUtils
								.substringBeforeLast(StringUtils.substringAfterLast(element.text(), "共"), "页");
						try {
							count = Integer.parseInt(countS);
							System.out.println("**************共计有" + count + "页数据**************");
							break;
						} catch (Exception e) {
							continue;
						}
					}
				}
			} else {
				System.out.println("找不到总页数,程序退出");
			}
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("抓取异常,程序退出");
			return;
		}
		// 开始抓取数据
		StringBuilder sbCsv = new StringBuilder();
		StringBuilder sbAgency = new StringBuilder();
		// 开始按页递增抓取信息
		for (int i = 1; i <= count; i++) {
			try {
				System.out.println("*************开始抓取第" + i + "页数据*****************");
				parameters.put("pageData.pageNeeded", String.valueOf(i));
				// 获取页面数据
				String response = HttpUtils.httpPost(url, parameters, new LinkedHashMap<String, String>(),
						new LinkedHashMap<String, String>(), Charset.forName("UTF-8")).readToText();
				Document document = Jsoup.parse(response);
				// 这里是我们需要保存的数据的tbody
				Elements elements = document.getElementsByTag("tbody").get(1).children();
				// 按tbody的tr行记录进行数据抽取
				for (int i1 = 0; i1 < elements.size(); i1++) {
					Element elementCurrentRow = elements.get(i1);
					String companyName = null;
					if (StringUtils.contains(elementCurrentRow.html(), "rowspan")) {
						companyName = elementCurrentRow.getElementsByAttribute("align").text();
						// 如果一个行记录拥有algin属性，则这行是公司信息，此行下面有5行代码信息
						//////////////// 开始抓取具体的代码
						System.out.println("开始抓取公司" + companyName + "的数据");
						// 特殊信息，有的表格记录和公司名字在同一行
						if (elementCurrentRow.children().size() == 6
								&& StringUtils.isNotEmpty(elementCurrentRow.child(4).text())) {
							try {
								String code = elementCurrentRow.child(5).text();
								String name = elementCurrentRow.child(4).text();
								// agency组装
								String agenvy = "[" + code + "]\r\nAGENCYNO=" + code + "\r\nIVERSION=400\r\nNAME="
										+ companyName + "-" + code;
								sbAgency.append(agenvy).append("\r\n");
								// csv组装
								String csv = companyName + "|" + name + "|" + code;
								sbCsv.append(csv).append("\r\n");
								System.out.println(csv);
							} catch (Exception e) {
								System.out.println("*******skip***************");
							}
						}
						Element elementcode1 = elements.get(i1 + 1);
						Element elementcode2 = elements.get(i1 + 2);
						Element elementcode3 = elements.get(i1 + 3);
						Element elementcode4 = elements.get(i1 + 4);
						Element elementcode5 = elements.get(i1 + 5);
						if (StringUtils.isNotEmpty(elementcode1.text())) {
							try {
								// agency组装
								String agenvy = "[" + elementcode1.child(1).text() + "]\r\nAGENCYNO="
										+ elementcode1.child(1).text() + "\r\nIVERSION=400\r\nNAME=" + companyName + "-"
										+ elementcode1.child(0).text();
								sbAgency.append(agenvy).append("\r\n");
								// csv组装
								String csv = companyName + "|" + elementcode1.child(0).text() + "|"
										+ elementcode1.child(1).text();
								sbCsv.append(csv).append("\r\n");
								System.out.println(csv);
							} catch (Exception e) {
								System.out.println("*******skip***************");
							}
						}
						if (StringUtils.isNotEmpty(elementcode2.text())) {
							try {
								// agency组装
								String agenvy = "[" + elementcode2.child(1).text() + "]\r\nAGENCYNO="
										+ elementcode2.child(1).text() + "\r\nIVERSION=400\r\nNAME=" + companyName + "-"
										+ elementcode2.child(0).text();
								sbAgency.append(agenvy).append("\r\n");
								// csv组装
								String csv = companyName + "|" + elementcode2.child(0).text() + "|"
										+ elementcode2.child(1).text();
								sbCsv.append(csv).append("\r\n");
								System.out.println(csv);
							} catch (Exception e) {
								System.out.println("*******skip***************");
							}
						}
						if (StringUtils.isNotEmpty(elementcode3.text())) {
							try {
								// agency组装
								String agency = "[" + elementcode3.child(1).text() + "]\r\nAGENCYNO="
										+ elementcode3.child(1).text() + "\r\nIVERSION=400\r\nNAME=" + companyName + "-"
										+ elementcode3.child(0).text();
								sbAgency.append(agency).append("\r\n");
								// csv组装
								String csv = companyName + "|" + elementcode3.child(0).text() + "|"
										+ elementcode3.child(1).text();
								sbCsv.append(csv).append("\r\n");
								System.out.println(csv);
							} catch (Exception e) {
								System.out.println("*******skip***************");
							}
						}
						if (StringUtils.isNotEmpty(elementcode4.text())) {
							try {
								// agency组装
								String agency = "[" + elementcode4.child(1).text() + "]\r\nAGENCYNO="
										+ elementcode4.child(1).text() + "\r\nIVERSION=400\r\nNAME=" + companyName + "-"
										+ elementcode4.child(0).text();
								sbAgency.append(agency).append("\r\n");
								// csv组装
								String csv = companyName + "|" + elementcode4.child(0).text() + "|"
										+ elementcode4.child(1).text();
								sbCsv.append(csv).append("\r\n");
								System.out.println(csv);
							} catch (Exception e) {
								System.out.println("*******skip***************");
							}
						}
						if (StringUtils.isNotEmpty(elementcode5.text())) {
							try {
								// agency组装
								String agency = "[" + elementcode5.child(1).text() + "]\r\nAGENCYNO="
										+ elementcode5.child(1).text() + "\r\nIVERSION=400\r\nNAME=" + companyName + "-"
										+ elementcode5.child(0).text();
								sbAgency.append(agency).append("\r\n");
								// csv组装
								String csv = companyName + "|" + elementcode5.child(0).text() + "|"
										+ elementcode5.child(1).text();
								sbCsv.append(csv).append("\r\n");
								System.out.println(csv);
							} catch (Exception e) {
								System.out.println("*******skip***************");
							}
						}
					} else {
						continue;
					}
					// 这里是行记录
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			// 休眠，避免请求过于频繁
			try {
				Thread.sleep(300);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		// 结束保存到文件
		FileUtils.saveToFile(new File("./CodeInfo.csv"), sbCsv.toString());
		FileUtils.saveToFile(new File("./CodeInfo.ini"), sbAgency.toString());
		System.out.println("*抓取结束文件已保存");
	}
}
