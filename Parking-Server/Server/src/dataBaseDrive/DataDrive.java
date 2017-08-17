package dataBaseDrive;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class DataDrive {
	//this class represent the data manage functionality of DBMS
	private  Connection con=DataBase.getConnection();
	private  Statement  stm=null;
//	private  ResultSet  res=null;
	private  static DataDrive data=null;
	
	
	public static DataDrive getData() {
		if(data==null){
			data=new DataDrive();
		}
		return data;
	}
	
	
	
	private  DataDrive(){
		try {
			stm=con.createStatement();
			System.out.println("DataBase connected successful");
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			System.err.println(e);
		}		
	}
	
	public boolean insert(String address,String type,String vaules){
		//used by Wechat-side program
			
			try {
//				System.out.println("INSERT\nINTO "
//						+address+'('+type+')'+"\nVALUES("+vaules+')');
				if(	stm.executeUpdate("INSERT\nINTO "
							+address+'('+type+')'+"\nVALUES("+vaules+')') !=0){
					return true;
				}
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return false;
	}
	
	public ResultSet search(String data ,String address, String condition){
		//used by sever-side program.
		
		try {
			return stm.executeQuery("SELECT "
					+data+"\nFROM "+address+"\nWHERE "+condition);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	public ResultSet search(String data,String address,String group_by,String condition){
		try {
			return stm.executeQuery("SELECT "
						+data+"\nFROM "+address+"\nWHERE "+condition);
		} catch (SQLException e) {
				// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	public boolean update(String address , String values,String condition){
		//used to modify the data
		try {
//			System.out.println("UPDATE " +address
//					+"\nSET "+values + "\nWHERE "+condition);
			if(stm.executeUpdate("UPDATE " +address
						+"\nSET "+values + "\nWHERE "+condition)!=0 ){
				return true;
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
	
	public boolean update(String address , String values){
		//used to modify the data
		try {
			if(stm.executeUpdate("UPDATE " +address
						+"\nSET "+values )!=0 ){
				return true;
			}
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
	
	public boolean delete(String address , String values,String condition){
		try {
//			System.err.println("DELETE " +values
//					+"\nFROM "+address+"\nWHERE "+condition);
			if(stm.executeUpdate("DELETE " +values
					+"\nFROM "+address+"\nWHERE "+condition)!=0 ){
				return true;
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}



	public ResultSet search(String data, String address) {
		// TODO Auto-generated method stub
	
			try {
				return stm.executeQuery("SELECT "
				+data+"\nFROM "+address);
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		
		return null;
	}
	
}
