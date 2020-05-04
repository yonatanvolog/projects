package il.co.ilrd.http_message;

import java.net.MalformedURLException;

public class ParserTest {
	public static void main(String[] args) throws MalformedURLException {
		String postman1 = 
		"GET /watch?v=t5n07Ybz7yI HTTP/1.1\r\n" + 
		"Host: www.youtube.com\n" + 
		"Cookie: YSC=bl1u8g2j1tM; GPS=1; VISITOR_INFO1_LIVE=AHYba5u8wP4\r\n" + 
		"\r\n" + 
		"";
		
		System.out.println("\nmypostman");
		String mypostman = 
		"GET www.youtube.com/watch?v=t5n07Ybz7yI HTTP/1.1\r\n" +
		//"Host: kukamoka\r\n" + 
		"\r\n";

		HttpParser parser12 = new HttpParser(mypostman);
		
		System.out.println("isRequest***" + parser12.isRequest());
		System.out.println("isReply***" + parser12.isReply());
		
		System.out.println("method***" + parser12.getStartLine().getHttpMethod());
		System.out.println("url***" + parser12.getStartLine().getURL());
		System.out.println("status***" + parser12.getStartLine().getStatus());
		System.out.println("version***" + parser12.getStartLine().getHttpVersion());
		System.out.println("header all***" + parser12.getHeader().getAllHeaders());
		System.out.println("body***" + parser12.getBody().getBody());

		
		System.out.println("\nmyrequest");
		String request = 
		"GET http://hello.htm HTTP/1.1\r\n" + 
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n" + 
		"Host: www.tutorialspoint.com\r\n" + 
		"Accept-Language: en-us\r\n" + 
		"Accept-Encoding: gzip, deflate\r\n" + 
		"Connection: Keep-Alive\r\n" +
		"\r\n" + //new line
		"some body\r\n" + 
		"ma body\r\n";
		;
		HttpParser parser1 = new HttpParser(request);
		
		System.out.println("isRequest***" + parser1.isRequest());
		System.out.println("isReply***" + parser1.isReply());
		
		System.out.println("method***" + parser1.getStartLine().getHttpMethod());
		System.out.println("url***" + parser1.getStartLine().getURL());
		System.out.println("status***" + parser1.getStartLine().getStatus());
		System.out.println("version***" + parser1.getStartLine().getHttpVersion());

		System.out.println("header User-Agent***" + parser1.getHeader().getHeader("User-Agent"));
		System.out.println("header Host***" + parser1.getHeader().getHeader("Host"));
		System.out.println("header Accept-Language***" + parser1.getHeader().getHeader("Accept-Language"));
		System.out.println("header Accept-Encoding***" + parser1.getHeader().getHeader("Accept-Encoding"));
		System.out.println("header Connection***" + parser1.getHeader().getHeader("Connection"));

		System.out.println("body***" + parser1.getBody().getBody());
		
		
		System.out.println("\nresponse");
		String response = 
		"HTTP/1.1 200 okay \r\n" + // \n -- \r -- \n\r
		"date: Thu, 30 Apr 2020 09:26:02 GMT\r\n" + 
		"expires: -1\r\n" + 
		"\r\n" + //new line
		"some body\r\n" + 
		"ma body\r\n";
		
		HttpParser parser = new HttpParser(response);
		
		System.out.println("isRequest***" + parser.isRequest());
		System.out.println("isReply***" + parser.isReply());
		
		System.out.println("method***" + parser.getStartLine().getHttpMethod());
		System.out.println("url***" + parser.getStartLine().getURL());
		System.out.println("status***" + parser.getStartLine().getStatus());
		System.out.println("version***" + parser.getStartLine().getHttpVersion());

		System.out.println("header date***" + parser.getHeader().getHeader("date"));
		System.out.println("header expires***" + parser.getHeader().getHeader("expires"));

		System.out.println("body***" + parser.getBody().getBody());

//		String startLine = parser.getStartLineSubstring(message);
//		String headers = parser.getHeadersSubstring(message);
//		String body = parser.getBodySubstring(message);
		
//		System.out.println("S:" + startLine);
//		System.out.println("H:" + headers);
//		System.out.println("B:" + body);
	}
}