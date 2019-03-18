import javax.sound.midi.*;
import java.io.*;
import javax.swing.*;
import java.awt.*;

public class MiniMusicPlayer2 implements ControllerEventListener{
	static JFrame f = new JFrame("My First Music Video");
	static MyDrawPanel m1;

	public static void main(String[] args){
		MiniMusicPlayer2 mini = new MiniMusicPlayer2();
		mini.go();
	}
	public void setUpGui(){
		m1 = new MyDrawPanel();
		f.setContentPane(m1);
		f.setBounds(30,30,300,300);
		f.setVisible(true);
	}
	public void go(){
		setUpGui();
		try{
			Sequencer sequencer = MidiSystem.getSequencer();
			sequencer.open();

			int[] eventsIWant = {127};
			sequencer.addControllerEventListener(this,eventsIWant);

			Sequence seq = new Sequence(Sequence.PPQ,4);
			Track track = seq.createTrack();
			int r = 0;
			for(int i=5;i<60;i+=4){
				r = (int)((Math.random()*50)+1);
				track.add(makeEvent(144,1,r,100,i));
				track.add(makeEvent(176,1,127,0,i));
				track.add(makeEvent(128,1,r,100,i+2));
			}
			sequencer.setSequence(seq);
			sequencer.setTempoInBPM(120);
			sequencer.start();
		}
		catch(Exception ex){ex.printStackTrace();}
	}
	public void controlChange(ShortMessage event){
		System.out.println("la");
	}
	public MidiEvent makeEvent(int comd,int chan,int one,int two,int tick){
		MidiEvent event = null;
		try{
			ShortMessage a = new ShortMessage();
			a.setMessage(comd,chan,one,two);
			event = new MidiEvent(a,tick);
		}
		catch(Exception a){

		}
		return event;
	}
}