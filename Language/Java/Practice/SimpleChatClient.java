import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class SimpleChatClient{
	JTextArea incoming;
	JTextField outGoing;
	BufferedReader reader;
	PrintWriter writer;
	Socket sock;
	Private String userName="";
	public static void main(String[] args){
		SimpleChatClient client = new SimpleChatClient();
		client.setClientUserName(args[1]);
		client.go();
	}
	public void go(){
		JFrame frame = new JFrame("Ludicrously Simple Chat Client");
		JPanel mainPanel = new JPanel();
		incoming = new JTextArea(15,20);
		incoming.setLineWrap(true);
		incoming.setWrapStyleWord(true);
		//incoming.setEditable(false);
		//incoming.setText("^\n^\n^\n^\n^\n^\n^\n^\n^\n^\n^\n^\n^\n^\n^\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		JScrollPane qScroller = new JScrollPane(incoming);
		qScroller.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		qScroller.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

		outGoing = new JTextField(20);
		JButton sendButton = new JButton("Send");
		sendButton.addActionListener(new SendButtonListener());
		mainPanel.add(qScroller);
		mainPanel.add(outGoing);
		mainPanel.add(sendButton);
		setUpNetworking();

		Thread readerThread = new Thread(new IncomingReader());
		readerThread.start();

		frame.getContentPane().add(BorderLayout.CENTER,mainPanel);
		frame.setSize(400,500);
		frame.setVisible(true);
	}
	private void setUpNetworking(){
		try{
			sock = new Socket("127.0.0.1",5000);
			InputStreamReader streamReader = new InputStreamReader(sock.getInputStream());
			reader= new BufferedReader(streamReader);
			writer = new PrintWriter(sock.getOutputStream());
			System.out.println("networking established");
		}
		catch(IOException e){
			e.printStackTrace();
		}
	}
	public class SendButtonListener implements ActionListener{
		public void actionPerformed(ActionEvent ae){
			try{
				writer.println(outGoing.getText());
				writer.flush();
			}
			catch(Exception e){
				e.printStackTrace();
			}
			outGoing.setText("");
			outGoing.requestFocus();
		}
	}
	public class IncomingReader implements Runnable{
		public void run(){
			String message;
			try{
				while((message=reader.readLine())!=null){
					incoming.append(message+"\n");
					System.out.println("read "+message);
				}

			}
			catch(Exception e){
				e.printStackTrace();
			}
		}
	}

	public String getClientUserName(){
		return userName;
	}
	public String setClientUserName(String name){
		userName = name;
	}
}