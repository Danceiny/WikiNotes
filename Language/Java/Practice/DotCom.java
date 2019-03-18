import java.util.ArrayList;
public class DotCom{
	private ArrayList<String> locationCells;
	//private int numOfHits;//deleted
	private String name;
	
	public void setLocationCells(ArrayList<String> loc){
		locationCells = loc;
	}
	public void setName(String n){
		name = n;
	}
		
	public String checkYourself(String userInput){
		String result = "miss";
		int index = locationCells.indexOf(userInput);
		if(index >= 0){
			locationCells.remove(index);//remove the one which hit.
			if(locationCells.isEmpty()){
				result = "kill";
			}
			else{
				result = "hit";
			}
		}
		return result;
	}
}
