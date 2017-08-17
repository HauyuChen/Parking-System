package ui;

import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import dataBaseDrive.DataDrive;

public class SecondUI {
	JFrame frame;
	JScrollPane upPanel;
	JScrollPane downPanel;
	
	JTextArea  erro;
	JTextArea  change;
	
//	JScrollPane erroScroll;
//	JScrollPane changeScroll;
	
	JButton closeOrOpen;
	
	public SecondUI(){
		init();
		
	}
	
	void init() {
		// TODO Auto-generated method stub
		initCompenont();
		initButton();
		initLabel();
		initPanel();
		initFrame();
	}

	private void initFrame() {
		// TODO Auto-generated method stub
		frame=new JFrame("car security system");
		frame.setBounds(250, 100, 700, 600);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new GridLayout(3,1));
		frame.add(upPanel);
		frame.add(closeOrOpen);
		frame.add(downPanel);
		
		
		frame.setVisible(true);
//		frame.pack();
	}

	private void initPanel() {
		// TODO Auto-generated method stub
		upPanel=new JScrollPane(change);
//		upPanel.add(change);
		
		downPanel=new JScrollPane(erro);
//		downPanel.add(erro);
		
	}

	private void initLabel() {
		// TODO Auto-generated method stub
		
	}

	private void initButton() {
		// TODO Auto-generated method stub
		closeOrOpen=new JButton("启动服务器");
		closeOrOpen.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				// TODO Auto-generated method stub
				if(closeOrOpen.getText().equals("启动服务器")){
					if(DataDrive.getData()!=null)
						closeOrOpen.setText("服务器正常运行中");
				}
				
			}
			
		});
		
	}

	private void initCompenont() {
		// TODO Auto-generated method stub
		change=new JTextArea();
		change.setBounds(0, 0, 700,200);
		change.setWrapStyleWord(true);
		change.setLineWrap(true);

		erro=new JTextArea();
		erro.setBounds(0, 0, 700, 200);
		erro.setWrapStyleWord(true);
		erro.setLineWrap(true);
		erro.setForeground(Color.red);
		PrintStream out=new PrintStream( new OutputStream(){
			byte[] buffer=new byte[1024];
			String content="";
			int num=0,i=0;
				@Override
				public void write(int arg0) throws IOException {
						// TODO Auto-generated method stub
						if(num==1024){
							num=0;
							i++;
							if(i==2){
								content="";
							}
						}
						if((char)arg0!='\n'){
							 buffer[num]=(byte)arg0;
							 num++;
						 }
						 else{
							buffer[num++]=(byte)'\n';
							content+=new String(buffer,0,num);
							change.setText(content);
						 }
					}
					
				});
		System.setOut(out);
		
		PrintStream err=new PrintStream( new OutputStream(){
			byte[] buffer=new byte[1024];
			String content="";
			int num=0,i=0;
				@Override
				public void write(int arg0) throws IOException {
						// TODO Auto-generated method stub
						if(num==1024){
							num=0;
							i++;
							if(i==2){
								content="";
							}
						} 
						if((char)arg0!='\n'){
							 buffer[num]=(byte)arg0;
							 num++;
						 }
						 else{
							buffer[num++]=(byte)'\n';
							content+=new String(buffer,0,num);
							erro.setText(content);
						 }
					}
					
				});
		System.setErr(err);
	}
	
//	public static void main(String[] args){
//		new SecondUI();
////		System.out.print("sdsadsafd");
//		
//	}

}


