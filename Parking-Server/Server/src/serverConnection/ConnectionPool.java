package serverConnection;

import java.net.Socket;
import java.util.ArrayList;

public class ConnectionPool {
	private ArrayList<WorkProcess> workPool=null;
	private ArrayList<WorkProcess> freePool=null;
	private static ConnectionPool pool=null;
	private boolean freeEmpty;
	
	
	private ConnectionPool(){
		workPool=new ArrayList<>();
		freePool=new ArrayList<>();
		freeEmpty=true;
	}

	public static ConnectionPool getPool() {
		
		if(pool==null){
			return pool=new ConnectionPool();
		}
		else {
			return pool;
		}
	}
	
	public void exchange(WorkProcess work,boolean workDone){
		//be used by WorkProcess.workMethods.workDone and WorkProcees.set
		synchronized (work){
			if(workDone){			
				workPool.remove(work);
				freePool.add(work);
				freeEmpty=false;
				System.out.println(work.getName()+" workDone");
			}
			else{
				freePool.remove(work);
				workPool.add(work);
				if(freePool.isEmpty()){
					freeEmpty=true;
				}
				System.out.println(work.getName()+" workStart");
			}
		}
//		System.out.println("workPool size? "+workPool.size()
//				+" freePool size? "+freePool.size());
		
	}
	

	public boolean getfreeEmpty() {
		return freeEmpty;
	}

	public void create(Socket client){
		//create Process  ,used by ServerConnection(totalMemory<maxMemory&freeEmpty=true)
		
		WorkProcess work=new WorkProcess(client);
		work.start();
		
	}

	public void add(Socket client){
		//set one of processes on the freePool
		WorkProcess work=freePool.get(freePool.size()-1);
		work.set(client);
		
		synchronized (work){						
			work.notify();
		}
		
		
	}
}