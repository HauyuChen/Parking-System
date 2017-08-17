package serverReply;

import java.io.DataOutputStream;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
public class Zigbee extends WorkMethods {
	private static Zigbee zigbee=new Zigbee();
	private DataOutputStream out=null;
	public Warning warn=new Warning();
	
	class Warning extends Thread {
		public void run() {
			//make sure  the client is forgetting to close protection
			while(true){
				
				ResultSet set=dbms.search("telephone","Warning","makesure<25");
//				System.err.println("开始");
				try {
					if(set.next()){
//						System.err.println("开始");
//						while(set.next()){
						
						{
							zigbee.feedback("SENDMSG"+set.getString(1));
							dbms.update("warning", "makesure=25","telephone=\'"+set.getString(1)+'\'');
							System.err.println(set.getString(1));
						
						}while(set.next());
					}
					else {
//						System.err.println("结束");
						Warning.sleep(3000);
					}    
				} catch (SQLException | InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
		}	
	}
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
	private Zigbee(){}
	@Override
	protected void working(String s, DataOutputStream out, byte[] data) {
	//update the data in dataBase and administrator's program
		int c=0;
		if(s.startsWith("5226")){
			this.out=out;
			warn.start();
			return;
		}
		((APP) APP.getAPP()).feedback(s);
		((WeChat) WeChat.getWechat()).feedback(s);
		if(s.charAt(0)!='A'&&s.charAt(0)!='B'){
			return;
		}
		for(int i=1;i<s.length();i++){
			String condition=null;
			if(i<10){
				condition="00"+i;
			}
			else {
				condition="0"+i;
			}	
			if(s.charAt(i)=='0'){
				c++;
			}
			dbms.update("carState", "车位状态=\'"+s.charAt(i)
					+"\'","车位号=\'"+s.charAt(0)+condition+"\'");
		}
		
		if(c<10){
			this.feedback("P"+s.charAt(0)+'0'+c);
		}
		else this.feedback("P"+s.charAt(0)+c);
		
		
	}

	public static WorkMethods getZigbee() {
		// TODO Auto-generated method stub
		
		return zigbee;
	}
	
}
