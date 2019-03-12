import java.net.*;
import java.io.*;

public class DailyAdviceClient{
	public void go(){
		try{
			String serverIp = "127.0.0.1";
			//String serverIp = "192.168.222.1";
			Socket s = new Socket(serverIp,4242);
			InputStreamReader streamReader = new InputStreamReader(s.getInputStream());
			BufferedReader reader = new BufferedReader(streamReader);

			String advice = reader.readLine();
			System.out.println("Today you should: "+advice);

			reader.close();
		}
		catch(IOException e){
			e.printStackTrace();
		}	
	}
	public static void main(String[] args){
		DailyAdviceClient client = new DailyAdviceClient();
		client.go();
	}
}
