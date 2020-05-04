package il.co.ilrd.http_message;

public enum HttpMethod {
	POST("POST"),
	GET("GET"),
	PUT("PUT"),
	PATCH("PATCH"),
	DELETE("DELETE")
	;
	
	private String method;

	HttpMethod(String method) {
	    this.method = method;
	}

	public String getMethodAsString() {
	    return method;
	}
}