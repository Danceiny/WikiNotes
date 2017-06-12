package myimagej;
import ij.ImagePlus;
import ij.gui.ImageCanvas;
import ij.io.FileSaver;
import ij.process.ImageConverter;
import java.awt.BorderLayout;
import java.awt.FileDialog;
import java.awt.Frame;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.ScrollPane;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
public class MyImageJ implements ActionListener {
Frame frame;
ImagePlus imp;
ScrollPane sp;
MenuBar menuBar;
Menu file, process,image;
MenuItem processInvert;
MenuItem processSharpen;
MenuItem processFindEdges;
 面向对象的程序设计案例 v1.0（内部资料） HUST
44
MenuItem processDilate;
MenuItem processErode;
MenuItem convertToGray_8;
MenuItem makeBinary;
MenuItem fileSave;
MenuItem fileSaveAsJpg;
MenuItem fileOpen;
MenuItem fileQuit;
ImageCanvas ic;
FileDialog fd;
My_Filter_Listener_Plugin mf;
public static void main(String args[])
{
new MyImageJ().createGUI();
}
public void createGUI()
{//初始化图像界面
frame=new Frame("My ImageJ");
makeMenu();
frame.setMenuBar(menuBar);
frame.setLocation(200, 200);
frame.setSize(500, 400);
frame.addWindowListener(new WindowAdapter()
{
public void windowClosing(WindowEvent e)
{
System.exit(0);
}
});
frame.setVisible(true);
}
private void makeMenu()
{//初始化菜单栏
menuBar = new MenuBar();
file = new Menu("File");
fileSave = new MenuItem("Save");
fileSaveAsJpg = new MenuItem("Save as jpg");
fileOpen = new MenuItem("Open");
fileQuit = new MenuItem("Quit");
 面向对象的程序设计案例 v1.0（内部资料） HUST
45
process = new Menu("Process");
processInvert = new MenuItem("invert");
processSharpen= new MenuItem("sharpen");
processFindEdges= new MenuItem("find edges");
processDilate= new MenuItem("dilate");
processErode=new MenuItem("erode");
process.add(processInvert);
process.add(processSharpen);
process.add(processFindEdges);
process.add(processDilate);
process.add(processErode);
image =new Menu("image");
convertToGray_8= new MenuItem("convert to gray_8");
makeBinary=new MenuItem("make binary");
image.add(convertToGray_8);
image.add(makeBinary);
menuBar.add(file);
menuBar.add(process);
menuBar.add(image);
file.add(fileOpen);
file.add(fileSave);
file.add(fileSaveAsJpg);
file.addSeparator();
file.add(fileQuit);
fileOpen.addActionListener(this);
fileSave.addActionListener(this);
fileSaveAsJpg.addActionListener(this);
fileQuit.addActionListener(this);
convertToGray_8.addActionListener(this);
}
public void actionPerformed(ActionEvent e)
{//由菜单项触发的各种事件的处理逻辑如下
Object cmd = e.getSource();
if (cmd.equals(fileOpen)) {//打开文件
this.openFile();
 面向对象的程序设计案例 v1.0（内部资料） HUST
46
} else if (cmd.equals(fileSave)) {
this.saveFile();
} else if (cmd.equals(fileSaveAsJpg)) {
this.saveFileAsJpg();
} else if (cmd.equals(fileQuit)) {
System.exit(0);
} else if (cmd.equals(convertToGray_8)) {//灰度化处理
ImageConverter imco=new ImageConverter(imp);
imco.convertToGray8();
}
}
private void openFile()
{//打开文件的处理逻辑
//弹出一个文件对话框
fd=new FileDialog(frame,"打开文件",FileDialog.LOAD);
fd.setVisible(true);
MyFileNameFilter filter= new MyFileNameFilter();
fd.setFilenameFilter(filter);
//通过ImagePlus实例和ImageCanvas实例加载并显示图片
imp = new ImagePlus(fd.getDirectory()+fd.getFile());
ic = new ImageCanvas(imp);
sp= new ScrollPane();
sp.add(ic);
sp.setSize(500, 500);
mf=new My_Filter_Listener_Plugin(imp,ic);
processInvert.addActionListener(mf);
processSharpen.addActionListener(mf);
processFindEdges.addActionListener(mf);
processDilate.addActionListener(mf);
processErode.addActionListener(mf);
makeBinary.addActionListener(mf);
frame.add(BorderLayout.CENTER,sp);
frame.pack();
}
private void saveFile()
{
 面向对象的程序设计案例 v1.0（内部资料） HUST
47
new FileSaver(imp).saveAsBmp();
}
private void saveFileAsJpg()
{//以jpg格式进行存储
new FileSaver(imp).saveAsJpeg();
}