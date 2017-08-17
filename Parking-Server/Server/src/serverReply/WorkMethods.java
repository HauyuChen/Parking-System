package serverReply;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

import dataBaseDrive.DataDrive;

public abstract class WorkMethods {
	//the super class of reply behavior 
	protected DataDrive dbms=DataDrive.getData();
	
	//reply to client
	protected abstract void working(String s,DataOutputStream out,byte[] data) ;
	
	
	
	private final void workDone(Socket client,DataInputStream read , DataOutputStream out ){
		//after working ,automatically running 
		try {
			read.close();
			out.close();
			client.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}



	public final void run(Socket client, DataInputStream read,
			DataOutputStream write, byte[] data)  {
		// TODO Auto-generated method stub
		String mes;
		while(true){
			mes=getRequest(read,data);
			if(mes!=null){
				working(mes,write,null);
			}
			else{
				break;
			}
		}
		workDone(client,read,write);
	}

	private String getRequest(DataInputStream read,byte[] data){
		//get the request from client.
		int num=-1;
		try {
			num = read.read(data,0,data.length);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(num<1){
			return null;
		}
		return new String(data,0,num);
	}

	public static WorkMethods setWorkMethods(String type){
	//contribute the reply behavior 
		if(type.equals("Zigbee")){
			((Zigbee)Zigbee.getZigbee()).warn.start();
			return Zigbee.getZigbee();

		}
		else if(type.equals("WeChat")){
			return WeChat.getWechat();

		}
//		else if(type.equals("000000")){
//			return Mountain.getMountain();
//		}
//		else if(type.equals("MVP666")){
//			return MVP666.getMVP666();
//		}
		else if(type.equals("APPLIC")){
			return APP.getAPP();
		}
		//no behavior to prevent the attack 
		else return null;
	}
} 
