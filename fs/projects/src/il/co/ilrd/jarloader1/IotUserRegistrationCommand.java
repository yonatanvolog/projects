//package il.co.ilrd.jarloader1;
//
//import il.co.ilrd.gatewayserver.CMDFactory;
//import il.co.ilrd.gatewayserver.GatewayServer.DatabaseManagementInterface;
//import il.co.ilrd.gatewayserver.GatewayServer.FactoryCommand;
//import il.co.ilrd.gatewayserver.GatewayServer.FactoryCommandModifier;
//
//import java.sql.SQLException;
//
//import org.json.JSONException;
//import org.json.JSONObject;
//
//public class IotUserRegistrationCommand implements FactoryCommandModifier{
//	private final static String COMMAND_TYPE = "commandType";
//	private final static String SUCCESS_MESSAGE = "IOT user registered";
//	private final static String ERROR_MESSAGE = "error";
//	private final static String SYNTAX_ERROR = "sql syntax error";
//	private final static String JSON_ERROR = "invalid json";
//	private static final Integer version = 1;
//
//	@Override
//	public void addToFactory() {
//		CMDFactory<FactoryCommand, String, Object> cmdFactory = CMDFactory.getInstance();
//		cmdFactory.add("IOT_USER_REGISTRATION", (IotUserRegistration) -> new IotUserRegistration());
//	}
//	
//	@Override
//	public Integer getVersion() {
//		return version;
//	}
//	
//	public class IotUserRegistration implements FactoryCommand {
//		@Override
//		public String run(Object data, DatabaseManagementInterface databaseManagement) {
//			String response = null;
//			
//			JSONObject dataAsJson = (JSONObject) data;
//			try {
//				databaseManagement.createRow(dataAsJson.getString("sqlCommand"));
//				response = createJsonResponse(COMMAND_TYPE, SUCCESS_MESSAGE);
//			} catch (JSONException e1) {
//				e1.printStackTrace();
//				response = createJsonResponse(ERROR_MESSAGE, JSON_ERROR);
//			} catch (SQLException e2) {
//				e2.printStackTrace();
//				response = createJsonResponse(ERROR_MESSAGE, SYNTAX_ERROR);
//			}
//			
//			return response;
//		}
//		
//		private String createJsonResponse(String message1 ,String message2) {
//			return "{" + "\"" + message1 + "\"" +":" + "\"" + message2 + "\"" + "}";
//		}
//	}
//}