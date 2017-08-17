package dataBaseDrive;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

class DataBase {
	private String userName;
	private String password;
	private String uRL;
	private static Connection connection;	
	
	
	DataBase() throws SQLException, ClassNotFoundException{
		// point to a database
		new Drivers(this);
		System.out.println("DataBaseSystem URL ready");
		
		System.out.println("DataBaseSystem connecting");
		connection=DriverManager.getConnection(uRL,userName,password);
		System.out.println("DataBaseSystem connection");
	}	
	
	void setDataBase(String uN ,String pw ,String uRL) {
		//in order to change the address of dataBase System
		this.uRL = uRL;
		this.password=pw;
		this.userName=uN;
	}	 
	 
	static Connection getConnection() {
		// only return one connection to calculated 
		if(DataBase.connection==null){
			try {
				new DataBase();
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				System.err.println(e);
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				System.err.println(e);
			}			
		}
		return DataBase.connection;
	}
}
