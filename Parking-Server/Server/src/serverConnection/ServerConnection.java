package serverConnection;


import java.io.IOException;
import java.net.ServerSocket;




public class ServerConnection {	
	private ServerSocket socket;
	private long totalMemory;
	private final long maxMemory;
	private Runtime system; 
	
	private ConnectionPool pool;
	
	
	public ServerConnection() throws IOException {
		
		socket=new ServerSocket(5000);
		system=Runtime.getRuntime();
		totalMemory=system.totalMemory();
		maxMemory=(long)9*system.maxMemory()/10;
		pool=ConnectionPool.getPool();
	}
	
	public void working() throws IOException{
		
		
		while(true){
			if(totalMemory<maxMemory&&pool.getfreeEmpty()){				
			
			//totalMemory<maxMemory,no free process
//				System.out.println("pool Empty? "+pool.getfreeEmpty());
				pool.create(socket.accept());
				
			}
			else if(!pool.getfreeEmpty()){
				//no free memory , have free process
//				System.out.println("totalMemory<maxMemory? "+(totalMemory<maxMemory));
				pool.add(socket.accept());
			}
			
			
		}
		
		
		
	}
	


}
