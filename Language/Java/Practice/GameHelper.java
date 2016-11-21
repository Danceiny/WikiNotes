import java.io.*;
import java.util.*;

public class GameHelper{
	private static final String alphabet = "abcdefg";
	private int gridLength = 7;
	private int gridSize = 49;	
	private int[] grid = new int[gridSize];
	private int comCount = 0;
	public String getUserInput(String prompt){
		String inputLine = null;
		System.out.print(prompt + " ");
		try{
			BufferedReader is = new BufferedReader(
				new InputStreamReader(System.in));
			inputLine = is.readLine();
			if(inputLine.length()==0)return null;
		}
		catch(IOException e){
			System.out.println("IOException: "+e);
		}
		return inputLine.toLowerCase();
	}
	public ArrayList<String> placeDotCom(int comSize){
		ArrayList<String> alphaCells = new ArrayList<String>();
		String[] alphacoords = new String[comSize];
		String temp = null;	
		int [] coords = new int[comSize];
		int attempts = 0;//current test str
		boolean success = false;
		int location = 0;//current start point	

		comCount++;
		int incr = 1;//horizon increasement
		if(comCount%2==1){
			incr = gridLength;//vertical increasement
		}
		while(!success && attempts++ < 200){
			location = (int)(Math.random()*gridSize);	
			int x = 0;
			success = true;//assume success
			while(success && x<comSize){
				if(grid[location] == 0){
					coords[x++] = location;//store location
					location += incr;//try next location
					if(location>=gridSize){
						success = false;
					}
					if(x>0 && (location%gridLength==0)){
						success = false;
					}

				}
				else{
					success = false;
				}
			}
		}//end of while loop

		int x = 0;
		int row = 0;
		int column = 0;

		while(x<comSize)
		{
			grid[coords[x]] = 1;//used 
			row = (int)(coords[x]/gridLength);
			column = (int)(coords[x]%gridLength);
			temp = String.valueOf(alphabet.charAt(column));
			alphaCells.add(temp.concat(Integer.toString(row)));
			x++;
			System.out.println("coords "+x+" = "+alphaCells.get(x-1));//real location of DotCom
		}
		return alphaCells;

	}
}
