package serverReply;

import java.io.DataOutputStream;
import java.io.IOException;
import java.sql.SQLException;



public class WeChat extends WorkMethods {
	
	private static WorkMethods wechat;
	private DataOutputStream out;
	public void feedback(String s){
		try {
			if(out==null){
				return;
			}
			out.write(s.getBytes());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	private WeChat(){}
	
	public static WorkMethods getWechat() {
	
		if(wechat==null){
			wechat=new WeChat();
		}
		return wechat;
	}

	@Override
	protected void working(String s, DataOutputStream out, byte[] data) {
		

		try {

			if(s.startsWith("search")){
				CarFunctionality.s.searchFree(dbms, s, out);
			}
			else if(s.startsWith("insert")){
				CarFunctionality.s.userAdd(dbms, s, out);
			}
			else if(s.startsWith("update")){
				CarFunctionality.s.informationUp(dbms, s, out);
			}
			else if(s.startsWith("use")){
				CarFunctionality.s.positionUsed(dbms, s, out);
			}
			else if(s.startsWith("free")){
				CarFunctionality.s.positionFree(dbms, s, out);
			}
			else if(s.startsWith("Li")){
				 ((Zigbee) Zigbee.getZigbee()).feedback(s);
			}
			else if(s.startsWith("look")){
//				System.err.println(s);
				CarFunctionality.s.userCheck(dbms, s, out);
			}
			else if(s.startsWith("admin")){
				if(s.equals("Admin\nxxx")){
					this.out=out;
					out.write("已进入超级管理员模式".getBytes());
				}
				else{
					out.write("密码错误".getBytes());
				}
			}
			else if(s.equals("unadmin")){
				this.out=null;
			}
		} catch (SQLException | IOException e) {
			
			e.printStackTrace();
		}
		
	}

	

}
