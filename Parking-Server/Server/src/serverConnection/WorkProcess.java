package serverConnection;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import serverReply.WorkMethods;

public class WorkProcess extends Thread{
	//this class is the set of client.
	private WorkMethods wM;
	private Socket client;
	private DataInputStream read;
	private DataOutputStream write;
	private byte[] data;
	
	
	WorkProcess(Socket client){
		data=new byte[1024];
		this.client=client;
		this.set(client);
		
	}
	
	
	public void set(Socket client
			       ){
		// to reuse the exit thread
		this.client=client;
		try {
			read=new DataInputStream(client.getInputStream());
			
			//read the beginning of the request to identify one special behavior 
			read.read(data,0,6);
			write=new DataOutputStream(client.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		}
		this.setWorkMethods();
		ConnectionPool.getPool().exchange(this, false);
	}
	
	
	void setWorkMethods(){
		//only this methods need to modify when program change
		String s=new String(data,0,6);
		wM=WorkMethods.setWorkMethods(s);
	}
	
	

	@Override
	public void run() {
		// TODO Auto-generated method stub
		while(true){
			
			if(client!=null){
				//wM is the object represent the behavior 
				wM.run(client,read,write,data);
				client=null;				
			}
			else{
				try {
					synchronized (this){						
						ConnectionPool.getPool().exchange(this, true);
						this.wait();
					}
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
			
			
	}
}
