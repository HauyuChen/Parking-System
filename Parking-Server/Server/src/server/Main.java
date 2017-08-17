package server;

import java.io.IOException;
import serverConnection.ServerConnection;
import ui.SecondUI;




public class Main {
	
	public static void main(String[] args) throws IOException {
		// loading all the unit 


		ServerConnection server=new ServerConnection();
		new SecondUI();
		server.working();
		
	}

}
