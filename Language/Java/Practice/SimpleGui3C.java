import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class SimpleGui3C implements ActionListener{
	JFrame frame;
	public static void main(String[] args){
		SimpleGui3C gui = new SimpleGui3C();
		gui.go();
	
	}
	public void go(){
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		JButton button = new JButton("Change colors");
		button.addActionListener(this);

		MyDrawPanel drawPanel = new MyDrawPanel();

		frame.getContentPane().add(BorderLayout.SOUTH, button);
		frame.getContentPane().add(BorderLayout.CENTER, drawPanel);
		frame.setSize(300,300);
		frame.setVisible(true);
	}
	public void actionPerformed(ActionEvent event){
		frame.repaint();
	}
}
