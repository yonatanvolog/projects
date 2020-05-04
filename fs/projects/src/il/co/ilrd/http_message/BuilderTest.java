package il.co.ilrd.http_message;

import java.net.MalformedURLException;
import java.util.HashMap;
import java.util.Map;

public class BuilderTest {
	public static void main(String[] args) throws MalformedURLException {
		String message1 = 
				HttpBuilder.createHttpRequestMessage(HttpMethod.GET, HttpVersion.HTTP_1_0,"/", null, null);
		System.out.println(message1);
		
		
		
		Map<String, String> header2 = new HashMap<>();
		header2.put("Host", "www.yoni-web.co.il");
		String message2 = 
				HttpBuilder.createHttpRequestMessage(HttpMethod.GET, HttpVersion.HTTP_1_0,"/", header2, null);
		System.out.println(message2);
		
		HttpParser parser2 = new HttpParser(message2);
		System.out.println("isRequest***" + parser2.isRequest());
		System.out.println("isReply***" + parser2.isReply());
		
		System.out.println("method***" + parser2.getStartLine().getHttpMethod());
		System.out.println("url***" + parser2.getStartLine().getURL());
		System.out.println("status***" + parser2.getStartLine().getStatus());
		System.out.println("version***" + parser2.getStartLine().getHttpVersion());
		System.out.println("header all***" + parser2.getHeader().getAllHeaders());
		System.out.println("body***" + parser2.getBody().getBody());
	
		
		
		header2.put("Content Lengh", "pretty long");
		String message3 = 
				HttpBuilder.createHttpRequestMessage(HttpMethod.GET, HttpVersion.HTTP_1_0,"/", header2, "chubby boi");
		System.out.println(message3);
		
		HttpParser parser3 = new HttpParser(message3);
		System.out.println("isRequest***" + parser3.isRequest());
		System.out.println("isReply***" + parser3.isReply());
		
		System.out.println("method***" + parser3.getStartLine().getHttpMethod());
		System.out.println("url***" + parser3.getStartLine().getURL());
		System.out.println("status***" + parser3.getStartLine().getStatus());
		System.out.println("version***" + parser3.getStartLine().getHttpVersion());
		System.out.println("header all***" + parser3.getHeader().getAllHeaders());
		System.out.println("body***" + parser3.getBody().getBody());
		
		System.out.println("test aaa");
		String message4 = 
				HttpBuilder.createHttpResponseMessage(HttpVersion.HTTP_1_1, HttpStatusCode.BAD_REQUEST, null, null);
		System.out.println(message4);
		HttpParser parser4 = new HttpParser(message4);
		System.out.println("isRequest***" + parser4.isRequest());
		System.out.println("isReply***" + parser4.isReply());
		
		System.out.println("method***" + parser4.getStartLine().getHttpMethod());
		System.out.println("url***" + parser4.getStartLine().getURL());
		System.out.println("status***" + parser4.getStartLine().getStatus());
		System.out.println("version***" + parser4.getStartLine().getHttpVersion());
		System.out.println("header(Some header)***" + parser4.getHeader().getHeader("Some header"));
		System.out.println("body***" + parser4.getBody().getBody() + "\n");
		
		Map<String, String> header3= new HashMap<>();
		header3.put("Host", "www.yoni-web.co.il/replies/replay1");
		String message5 = 
				HttpBuilder.createHttpResponseMessage(HttpVersion.HTTP_1_1, HttpStatusCode.BAD_REQUEST, header3, null);
		System.out.println(message5);
		
		parser2 = new HttpParser(message5);
		System.out.println("isRequest***" + parser2.isRequest());
		System.out.println("isReply***" + parser2.isReply());
		
		System.out.println("method***" + parser2.getStartLine().getHttpMethod());
		System.out.println("url***" + parser2.getStartLine().getURL());
		System.out.println("status***" + parser2.getStartLine().getStatus());
		System.out.println("version***" + parser2.getStartLine().getHttpVersion());
		System.out.println("header all***" + parser2.getHeader().getAllHeaders());
		System.out.println("body***" + parser2.getBody().getBody());
	
		
		
		header3.put("Content Lengh", "even longer");
		String message6 = 
				HttpBuilder.createHttpResponseMessage(HttpVersion.HTTP_1_1, HttpStatusCode.BAD_REQUEST, header3, "even chubbier");
		System.out.println(message6);
		
		parser3 = new HttpParser(message6);
		System.out.println("isRequest***" + parser3.isRequest());
		System.out.println("isReply***" + parser3.isReply());
		
		System.out.println("method***" + parser3.getStartLine().getHttpMethod());
		System.out.println("url***" + parser3.getStartLine().getURL());
		System.out.println("status***" + parser3.getStartLine().getStatus());
		System.out.println("version***" + parser3.getStartLine().getHttpVersion());
		System.out.println("header all***" + parser3.getHeader().getAllHeaders());
		System.out.println("body***" + parser3.getBody().getBody());
		
		
		String message9 = 
				HttpBuilder.createHttpRequestMessage(HttpMethod.GET, HttpVersion.HTTP_1_0,"http://www.google.com/", null, "body");
		System.out.println(message9);
		
		HttpParser parser9 = new HttpParser(message9);
		System.out.println("isRequest***" + parser9.isRequest());
		System.out.println("isReply***" + parser9.isReply());
		
		System.out.println("method***" + parser9.getStartLine().getHttpMethod());
		System.out.println("url***" + parser9.getStartLine().getURL());
		System.out.println("status***" + parser9.getStartLine().getStatus());
		System.out.println("version***" + parser9.getStartLine().getHttpVersion());
		System.out.println("header all***" + parser9.getHeader().getAllHeaders());
		System.out.println("body***" + parser9.getBody().getBody());
	}
}
