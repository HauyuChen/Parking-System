package dataBaseDrive;


import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

class Drivers {
	//used to load the driver to connected dataBase.
	
	private Properties store = new Properties();
	private InputStream read ;
	
	
	Drivers(DataBase system) throws ClassNotFoundException{
		System.out.println("DataBaseSystem URL loading ");
		try{			
			read=getClass().getResourceAsStream("store.properties");			
			store.load(read);	
		}catch(IOException e){
			//System.exit(199);
			e.printStackTrace();
		}
		String url=store.getProperty("URL");
		system.setDataBase(store.getProperty("userName"),
						   store.getProperty("password"),
						   url) ;
		

	
	
		
	}
}
