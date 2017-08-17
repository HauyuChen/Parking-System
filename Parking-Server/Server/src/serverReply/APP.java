package serverReply;

import java.io.DataOutputStream;
import java.io.IOException;

public class APP extends WorkMethods {
	
	private static final  APP app=new APP();
	private DataOutputStream out=null ;
	
	public void feedback(String s){
		// to feedback the only one application user
		try {
			if(out==null){
				return;
			}
			out.write(s.getBytes());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	private APP(){}
	@Override
	protected void working(String s, DataOutputStream out, byte[] data) {
		// TODO Auto-generated method stub
		System.err.println(s);
		if(s.equals("5226")){
			this.out=out;
			return;
		}
		else if(s.equals("SELFCONTROL")){
				((Zigbee) Zigbee.getZigbee()).feedback(s);
		}
		else if(s.startsWith("MANUAL")){
			((Zigbee) Zigbee.getZigbee()).feedback(s);
		}
		else if(s.startsWith("LIGHT")){
			 ((Zigbee) Zigbee.getZigbee()).feedback(s);
		}
//		else if(s.startsWith("ask")){
//			 String[] values=s.split("\n");
//		}
//		
	}

	public static WorkMethods getAPP() {
		// TODO Auto-generated method stub
		return app;
	}

}
