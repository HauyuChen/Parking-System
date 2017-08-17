package ui;

import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;



	/**
	 * @author benson
	 *
	 */
	public class OpenUI  {
		JFrame frame;
		JPanel framePanel;
		JLabel backGround;
		ImageIcon image;
		JLabel welcome;
		JLabel nameLab;
		JLabel keyLab;
		JTextField nameField;
		JPasswordField keyField;
		JButton login;
		JButton exit;
		
		
		public OpenUI(){
			init();
		}

		/* (non-Javadoc)
		 * @see com.benson.ui.UI#init()
		 */
		
		public void init() {
			// TODO Auto-generated method stub
			initCompenont();
			initButton();
			initLabel();
			initPanel();
			initFrame();
		}

		/* (non-Javadoc)
		 * @see com.benson.ui.UI#initFrame()
		 */
		
		public void initFrame() {
			// TODO Auto-generated method stub
			frame.setBounds(250, 100, 700, 600);
			
			frame.getLayeredPane().add(backGround, new Integer(Integer.MIN_VALUE));	
//			backGround.setBounds(0, 0, 700, 600);
			
			JPanel temp=(JPanel) frame.getContentPane();
			temp.setLayout(new GridLayout(2,1));
			temp.add(welcome);
			temp.add(framePanel);
			temp.setOpaque(false);
		
			frame.setVisible(true);
		
			frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			
		}

		/* (non-Javadoc)
		 * @see com.benson.ui.UI#initButton()
		 */
		
		public void initButton() {
			// TODO Auto-generated method stub
			exit.addActionListener(new ActionListener(){

				@Override
				public void actionPerformed(ActionEvent arg0) {
					// TODO Auto-generated method stub
//					BaseMachine.getBasemachine().close();
					System.exit(0);
					
				}
				
				
			});
			
			login.addActionListener(new ActionListener(){

				@Override
				public void actionPerformed(ActionEvent e) {
					// TODO Auto-generated method stub
					// input the right userName and password
						if(nameField.getText().equals("boss")){
							System.out.println(nameField.getText());
							System.out.println(new String(keyField.getPassword()));
							if(new String(keyField.getPassword()).equals("yes")){
//								new SecondUI();
								frame.setVisible(false);
								frame.removeAll();
							}
						}
				}
				
			});
		}

		/* (non-Javadoc)
		 * @see com.benson.ui.UI#initLabel()
		 */
		
		public void initLabel() {
			// TODO Auto-generated method stub
			
			welcome.setFont(new Font("宋体",Font.BOLD,45));
//			welcome.setOpaque(false); // it doesn't work
			
			image=new ImageIcon(image.getImage().getScaledInstance(700, 600, Image.SCALE_DEFAULT)) ;
			backGround.setIcon(image);
			backGround.setBounds(0, 0, 700, 600);
			
		}

		/* (non-Javadoc)
		 * @see com.benson.ui.UI#initPanel()
		 */
		
		public void initPanel() {
			// TODO Auto-generated method stub
//			framePanel.setLayout(new GridLayout(1,2));
			framePanel.add(nameLab);
			framePanel.add(nameField);
			
			framePanel.add(keyLab);
			framePanel.add(keyField);
			
			
			framePanel.add(login);
			framePanel.add(exit);
			
			
			
			framePanel.setOpaque(false);
			
		}

		/* (non-Javadoc)
		 * @see com.benson.ui.UI#iniCompenont()
		 */
		
		public void initCompenont() {
			// TODO Auto-generated method stub
			frame=new JFrame("control System");
			framePanel=new JPanel();
			backGround=new JLabel();
			image=new ImageIcon("src/openGround.jpeg");
		
			welcome=new JLabel("       欢迎使用本系统");
			
			nameLab=new JLabel("用户名：");
			keyLab=new JLabel("密 码：");
			
			
			nameField=new JTextField(10);
			keyField=new JPasswordField(10);
			
			login=new JButton("登陆");
			exit=new JButton("退出");
			
			
		}

		/**
		 * @param args
		 */
//		public static void main(String[] args) {
//			// TODO Auto-generated method stub
//			new OpenUI();
//		}


	

}
