package il.co.ilrd.httpiotserver;

public enum JsonResponses {
	
	CREATE_COMPANY_DATABASE("{\"message\":"),
	CREATE_TABLE("{\"message\":"),
	DELETE_TABLE("{\"message\":"),
	CREATE_IOT_EVENT("{\"message\":"),
	CREATE_ROW("{\"message\":"),
	READ_ROW("{\"rowValues\":"),
	READ_FIELD_BY_NAME("{\"fieldValue\":"),
	READ_FIELD_BY_INDEX("{\"fieldValue\":"),
	UPDATE_FIELD_BY_NAME("{\"message\":"),
	UPDATE_FIELD_BY_INDEX("{\"message\":"),
	DELETE_ROW("{\"message\":"),
	ERROR_MESSAGE("{\"errorMessage\":"),
	ACK_MESSAGE("{\"message\":"),
	
	OPTION(""),
	TRACE("{\"errorMessage\":{\"not implemented\"}"),
	PATCH("{\"errorMessage\":{\"not implemented\"}"),
	HEAD("{\"errorMessage\":{\"not implemented\"}"),
	;
	
    private String response;

    JsonResponses(String response) {
        this.response = response;
    }

    public String getResponse() {
        return response;
    }
}
