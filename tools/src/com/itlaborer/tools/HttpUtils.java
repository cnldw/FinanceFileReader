package com.itlaborer.tools;

import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.LinkedHashMap;

import net.dongliu.requests.RawResponse;
import net.dongliu.requests.Requests;

public class HttpUtils {
	// HTTP GET 忽略证书安全
	public static RawResponse httpGet(String url, HashMap<String, String> parameter,
			LinkedHashMap<String, String> header, LinkedHashMap<String, String> cookies, Charset requestCharset)
			throws Exception {
		RawResponse resp = Requests.get(url).verify(false).headers(header).cookies(cookies).params(parameter)
				.requestCharset(requestCharset).send();
		return resp;
	}

	// HTTP POST 忽略证书安全
	public static RawResponse httpPost(String url, HashMap<String, String> parameter,
			LinkedHashMap<String, String> header, LinkedHashMap<String, String> cookies, Charset requestCharset)
			throws Exception {
		RawResponse resp = Requests.post(url).verify(false).headers(header).cookies(cookies).params(parameter)
				.requestCharset(requestCharset).send();
		return resp;
	}

	// HTTP HEAD 忽略证书安全
	public static RawResponse httpHead(String url, HashMap<String, String> parameter,
			LinkedHashMap<String, String> header, LinkedHashMap<String, String> cookies, Charset requestCharset)
			throws Exception {
		RawResponse resp = Requests.head(url).verify(false).headers(header).cookies(cookies).params(parameter)
				.requestCharset(requestCharset).send();
		return resp;
	}

	// HTTP PUT 忽略证书安全
	public static RawResponse httpPut(String url, HashMap<String, String> parameter,
			LinkedHashMap<String, String> header, LinkedHashMap<String, String> cookies, Charset requestCharset)
			throws Exception {
		RawResponse resp = Requests.put(url).verify(false).headers(header).cookies(cookies).params(parameter)
				.requestCharset(requestCharset).send();
		return resp;
	}

	// HTTP PATCH 忽略证书安全
	public static RawResponse httpPatch(String url, HashMap<String, String> parameter,
			LinkedHashMap<String, String> header, LinkedHashMap<String, String> cookies, Charset requestCharset)
			throws Exception {
		RawResponse resp = Requests.patch(url).verify(false).headers(header).cookies(cookies).params(parameter)
				.requestCharset(requestCharset).send();
		return resp;
	}

	// HTTP DELETE 忽略证书安全
	public static RawResponse httpDelete(String url, HashMap<String, String> parameter,
			LinkedHashMap<String, String> header, LinkedHashMap<String, String> cookies, Charset requestCharset)
			throws Exception {
		RawResponse resp = Requests.delete(url).verify(false).headers(header).cookies(cookies).params(parameter)
				.requestCharset(requestCharset).send();
		return resp;
	}
}
