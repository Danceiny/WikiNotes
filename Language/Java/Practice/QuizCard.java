import javax.swing.*;
public class QuizCard{
	private String question;
	private String answer;
	QuizCard(String q,String a){
		question = q;
		answer = a;
	}

	public String getQuestion(){
		return question;
	}
	public String getAnswer(){
		return answer;
	}
	
}