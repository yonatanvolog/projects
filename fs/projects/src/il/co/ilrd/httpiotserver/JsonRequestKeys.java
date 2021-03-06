package il.co.ilrd.httpiotserver;

public enum JsonRequestKeys {
	
	PK_NAME("pkName"),
	PK_VALUE("pkValue"),
	COLUMN_NAME("columnName"),
	COLUMN_INDEX("columnIndex"),
	SQL_COMMAND("sqlCommand"),
	RAW_DATA("rawData"),
	PRIMARY_KEY_NAME("primaryKeyName"),
	PRIMARY_KEY_VALUE("primaryKeyValue"),
	NEW_VALUE("newValue")
	;
	
    private String request;

    JsonRequestKeys(String response) {
        this.request = response;
    }

    public String asString() {
        return request;
    }
}
