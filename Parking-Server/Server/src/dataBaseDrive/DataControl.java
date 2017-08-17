package dataBaseDrive;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

public class DataControl {
	//this class represent the data manage functionality of DBMS
		private  Connection con=DataBase.getConnection();
		private  Statement  stm=null;
//		private  ResultSet  res=null;
		private  static DataControl data=null;
		
		public static DataControl getData() {
			if(data==null){
				data=new DataControl();
			}
			return data;
		}

		private  DataControl(){
			try {
				stm=con.createStatement();
//				System.out.println("DataBase connected successful");
				stm.close();
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				System.err.println(e);
			}
		}
		
}
