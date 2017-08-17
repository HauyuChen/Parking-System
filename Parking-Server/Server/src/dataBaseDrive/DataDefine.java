package dataBaseDrive;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

public class DataDefine {
	//this class represent the data define functionality of DBMS
		private  Connection con=DataBase.getConnection();
		private  Statement  stm=null;
//		private  ResultSet  res=null;
		private  static DataDefine data=null;
		
		public static DataDefine getData() {
			if(data==null){
				data=new DataDefine();
			}
			return data;
		}

		private  DataDefine(){
			try {
				stm=con.createStatement();
				stm.close();
//				System.out.println("DataBase connected successful");
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				System.err.println(e);
			}		
		}
		
}
