import java.util.*;
public class DotComBust{
	private GameHelper helper = new GameHelper();
	private ArrayList<DotCom> dotComsList = new ArrayList<DotCom>();
	private int numOfGuesses = 0;

	private void setUpGame(){
		//first make some dot coms and give them locations
		DotCom one = new DotCom();
		one.setName("Pets.com");
		DotCom two = new DotCom();
		one.setName("eToys.com");
		DotCom three = new DotCom();
		three.setName("Go2.com");
		dotComsList.add(one);
		dotComsList.add(two);
		dotComsList.add(three);

		System.out.println("Your goal is to sink three dot coms.");
		System.out.println("Pets.com, eToys.com, Go2.com");
		System.out.println("Try to sink them all in the fewest number of guesses");
		for(DotCom dotComToSet:dotComsList){
			ArrayList<String> newLocation = helper.placeDotCom(3);
			dotComToSet.setLocationCells(newLocation);
		}
	}
	private void startPlaying(){
		while(!dotComsList.isEmpty()){
			String userGuess = helper.getUserInput("Entern a number: ");
			checkUserGuess(userGuess);

		}
		finishGame();
	}
	private void checkUserGuess(String userGuess){
		numOfGuesses++;
		String result = "miss";
		for(DotCom dotComToTest:dotComsList){
			result = dotComToTest.checkYourself(userGuess);
			if(result.equals("hit")){
				 break;
			}
			if(result.equals("kill")){
				dotComsList.remove(dotComToTest);
				break;
			}
		}
		System.out.println(result);


	}
	private void finishGame(){
		System.out.println("All dot coms are dead.");
		if(numOfGuesses>=18)
		{
			System.out.println("It took you long enough "+numOfGuesses+" guesses");
			System.out.println("Fish are dancing with your options.");
		}
		else
		{
			System.out.println("It only took you "+numOfGuesses+" guesses");
			System.out.println("You got out before your options sank.");
		}
	}
	public static void main(String[] args){
		DotComBust game = new DotComBust();
		game.setUpGame();
		game.startPlaying();

	}
}
