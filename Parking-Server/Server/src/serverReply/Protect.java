package serverReply;

import java.io.DataOutputStream;

public class Protect extends WorkMethods{
	// in order to improve the security 
	// be used by ensure public security
	private static Protect protect=new Protect();
	private Protect(){}
	@Override
	protected void working(String s, DataOutputStream out, byte[] data) {
		// TODO Auto-generated method stub
		
	}
	public static WorkMethods getProtect(){
		return protect;
	}
	
}
