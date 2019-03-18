import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
public class TwoButtons{
	JFrame frame;
	JLabel label;

	public static void main(String[] args){
		TwoButtons gui = new TwoButtons();
		gui.go();
	}
	public void go(){
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		JButton labelButton = new JButton("Change label");
		labelButton.addActionListener(new LabelListener());

		JButton colorButton = new JButton("Change circle");
		colorButton.addActionListener(new ColorListener());

		label = new JLabel("I'm a label");
		MyDrawPanel drawPanel = new MyDrawPanel();

		frame.getContentPane().add(BorderLayout.SOUTH,colorButton);
		frame.getContentPane().add(BorderLayout.CENTER,drawPanel);
		frame.getContentPane().add(BorderLayout.EAST,labelButton);
		frame.getContentPane().add(BorderLayout.WEST,label);

		frame.setSize(500,500);
		frame.setVisible(true);
	}
	class LabelListener implements ActionListener{
		public void actionPerformed(ActionEvent event){
			label.setText("Ouch");
		}
	}
	class ColorListener implements ActionListener{
		public void actionPerformed(ActionEvent event){
			frame.repaint();
		}
	}
}