import java.awt.*;
import javax.swing.*;

class MyDrawPanel extends JPanel{
	public void paintComponent(Graphics g){//called by system,not by yourself
	//1
//		g.setColor(Color.orange);
//		g.fillRect(20,50,100,100);
	//2	
//		Image image = new ImageIcon("1.jpg").getImage();
//		g.drawImage(image,4,4,this);
	//3
//		g.fillRect(0,0,this.getWidth(),this.getHeight());
//		int red = (int)(Math.random()*255);
//		int green = (int)(Math.random()*255);
//		int blue = (int)(Math.random()*255);
	//4
//		Color randomColor = new Color(red,green,blue);
//		g.setColor(randomColor);
//		g.fillOval(70,70,100,100);//tuoyuan
	//5
		Graphics2D g2d = (Graphics2D)g;
		int red = (int)(Math.random()*255);
		int green = (int)(Math.random()*255);
		int blue = (int)(Math.random()*255);
		Color startColor = new Color(red,green,blue);

		red = (int)(Math.random()*255);
		green = (int)(Math.random()*255);
		blue = (int)(Math.random()*255);
		Color endColor = new Color(red,green,blue);

		GradientPaint gradient = new GradientPaint(70,70,startColor,150,150,endColor);
		g2d.setPaint(gradient);
		g2d.fillOval(70,70,100,100);
		
	}
}
