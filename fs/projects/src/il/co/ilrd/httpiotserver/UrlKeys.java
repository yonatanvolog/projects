package il.co.ilrd.httpiotserver;

public enum UrlKeys {
	DATABASE_NAME("databaseName"),
	TABLENAME("tablename"),
	METHOD("method")
	;
	
	private String keyAsString;

	UrlKeys(String keyAsString) {
	    this.keyAsString = keyAsString;
	}

	public String asString() {
	    return keyAsString;
	}
}