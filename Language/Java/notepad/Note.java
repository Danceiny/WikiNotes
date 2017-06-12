package notepad;
import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

public class Note extends JFrame implements ActionListener {
	private static final long serialVersionUID = 7453067955905534656L;
	/**
	* @param args
	*/
	// 声明记事本中需要的变量
	// change_flag 标记当前的文档是否被修改过
	private boolean change_flag = false;
	// current_path 标记当前正在处理的文件的路径
	private String current_path = null;
	// 以下是界面元素
	private JTextArea ta;
	private JMenuBar menubar;
	private JMenu file;
	private JMenu edit;
	private JMenu help;
	private JMenuItem fileopen;
	private JMenuItem filesave;
	private JMenuItem filesave2;
	private JMenuItem fileexit;
	private JMenuItem copy;
	private JMenuItem copyall;
	private JMenuItem paste;
	private JMenuItem cut;
	private JMenuItem copyright;
	private JPopupMenu pmenu;
	private JMenuItem pcopy;
	private JMenuItem ppaste;
	private JMenuItem pcut;
	private JMenuItem pcopyall;

	public static void main(String[] args) {
	// 主函数入口
	Note note = new Note();
	note.init();
	note.setVisible(true);
	}
	public void init() {
	this.setTitle("简易记事本");
	this.setSize(600, 600);
	this.setLocation(200, 200);
	ta = new JTextArea();
	// ta.setCursor(new Cursor(Cursor.TEXT_CURSOR));
	ta.setFont(new Font("winter", Font.BOLD, 14));
	ta.setSelectionColor(new Color(255, 255, 0));
	ta.setSize(600, 600);
	// 添加一个文档监听器
	ta.getDocument().addDocumentListener(new DocumentListener() {
	public void changedUpdate(DocumentEvent e) {
	change_flag = true;
	}
	public void insertUpdate(DocumentEvent e) {
	change_flag = true;
	}
	public void removeUpdate(DocumentEvent e) {
	change_flag = true;
	}
	});
	// 初始化菜单系列
	menubar = new JMenuBar();
	file = new JMenu("文件");
	edit = new JMenu("编辑");
	help = new JMenu("帮助");
	fileopen = new JMenuItem("打开..");
	fileopen.setMnemonic('O');
	fileopen.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O,InputEvent.CTRL_MASK));
	filesave = new JMenuItem("保存..");
	filesave.setMnemonic('S');
	filesave.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,InputEvent.CTRL_MASK));
	filesave2 = new JMenuItem("另存为");
	filesave2.setMnemonic('K');
	fileexit = new JMenuItem("退出");
	fileexit.setMnemonic('E');
	fileopen.addActionListener(this);
	filesave.addActionListener(this);
	filesave2.addActionListener(this);
	fileexit.addActionListener(this);
	file.add(fileopen);
	file.add(filesave);
	file.add(filesave2);
	file.add(fileexit);
	copyall = new JMenuItem("全选");
	copyall.setMnemonic('A');
	copyall.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A,InputEvent.CTRL_MASK));
	copy = new JMenuItem("复制");
	copy.setMnemonic('C');
	copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C,InputEvent.CTRL_MASK));
	paste = new JMenuItem("粘贴");
	paste.setMnemonic('V');
	paste.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V,InputEvent.CTRL_MASK));
	cut = new JMenuItem("剪切");
	cut.setMnemonic('T');
	cut.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_T,InputEvent.CTRL_MASK));
	copyall.addActionListener(this);
	copy.addActionListener(this);
	cut.addActionListener(this);
	paste.addActionListener(this);
	edit.add(copyall);
	edit.add(copy);
	edit.add(paste);
	edit.add(cut);
	copyright = new JMenuItem("版权");
	copyright.setMnemonic('H');
	copyright.addActionListener(this);
	help.add(copyright);
	menubar.add(file);
	menubar.add(edit);
	menubar.add(help);
	// 初始化一个下拉菜单系列
	pmenu = new JPopupMenu();
	pcopy = new JMenuItem("复制");
	pcopyall = new JMenuItem("全选");
	ppaste = new JMenuItem("粘贴");
	pcut = new JMenuItem("剪切");
	pcopyall.addActionListener(this);
	pcopy.addActionListener(this);
	pcut.addActionListener(this);
	ppaste.addActionListener(this);
	pmenu.add(pcopy);
	pmenu.add(pcopyall);
	pmenu.add(pcut);
	pmenu.add(ppaste);
	this.setJMenuBar(menubar);
	this.getContentPane().add(new JScrollPane(ta));
	// 添加一个窗口监听器
	this.addWindowListener(new WindowAdapter() {
	public void windowClosing(WindowEvent arg0) {
	// 调用一个退出函数
	exit();
	}
	});
	// 添加一个鼠标监听器，右击会弹出一个下拉菜单
	ta.addMouseListener(new MouseAdapter() {
	public void mouseClicked(MouseEvent e) {
	int button_type = e.getButton();
	int x = e.getX();
	int y = e.getY();
	if (button_type == MouseEvent.BUTTON3) {
	if (ta.getSelectedText()==null) {
	pcopy.setEnabled(false);
	pcut.setEnabled(false);
	ppaste.setEnabled(true);
	pcopyall.setEnabled(true);
	// ppaste.setEnabled(false);
	} else if
	(ta.getSelectedText().equals(ta.getText())) {
	pcopy.setEnabled(true);
	pcut.setEnabled(true);
	ppaste.setEnabled(true);
	pcopyall.setEnabled(false);
	} else {
	pcopy.setEnabled(true);
	pcut.setEnabled(true);
	ppaste.setEnabled(true);
	pcopyall.setEnabled(true);
	}
	//在文本区域内弹出菜单
	pmenu.show(ta, x, y);
	}
	}
	});
	}
	public void actionPerformed(ActionEvent e) {
	// 获取触发事件的组件值
	String cmd = e.getActionCommand();
	if (cmd.equals("打开..")) {
	ta.setText("");
	// 创建一个文件选择器，这是 swing 提供的一个文件对话框，大家可以和 FileDialog 做一下比较
	JFileChooser filechooser = new JFileChooser();
	filechooser.setCurrentDirectory(new File("d:/"));
	// 文件选择器的参数大家可以查询一下 API 文档
	if (filechooser.showOpenDialog(this) ==
	JFileChooser.APPROVE_OPTION) {
	File openfile = filechooser.getSelectedFile();
	current_path = openfile.getPath();
	BufferedReader reader;
	try {
	reader = new BufferedReader(new
	FileReader(openfile));
	String r = reader.readLine();
	while (r != null) {
	ta.append(r + "\n");
	r = reader.readLine();
	}
	reader.close();
	} catch (FileNotFoundException e1) {
	e1.printStackTrace();
	} catch (IOException ee) {
	ee.printStackTrace();
	}
	}
	}
	// 注意，打开文件和保存文件事实上是一个文件读写的过程
	if (cmd.equals("保存..")) {
	if (current_path != null) {
	try {
	BufferedWriter writer = new BufferedWriter(new
	FileWriter(
	current_path));
	writer = new BufferedWriter(new FileWriter(current_path));
	String data = ta.getText();
	writer.write(data);
	writer.close();
	} catch (IOException e1) {
	e1.printStackTrace();
	}
	} else {
	JFileChooser filechooser = new JFileChooser();
	filechooser.setCurrentDirectory(new File("d:/"));
	if (filechooser.showSaveDialog(this) ==
	JFileChooser.APPROVE_OPTION) {
	File savefile = filechooser.getSelectedFile();
	String savetarget = savefile.getPath() +
	".txt";
	BufferedWriter writer;
	try {
	writer = new BufferedWriter(new
	FileWriter(savetarget));
	String data = ta.getText();
	writer.write(data);
	writer.close();
	} catch (FileNotFoundException e1) {
	e1.printStackTrace();
	} catch (IOException ee) {
	ee.printStackTrace();
	}
	current_path = savetarget;
	}
	}
	}
	// 注意一下‘另存为’和‘保存’的区别
	if (cmd.equals("另存为")) {
	JFileChooser filechooser = new JFileChooser();
	if (filechooser.showSaveDialog(this) ==
	JFileChooser.APPROVE_OPTION) {
	File savefile = filechooser.getSelectedFile();
	String savetarget = savefile.getPath() + ".txt";
	BufferedWriter writer;
	try {
	writer = new BufferedWriter(new
	FileWriter(savetarget));
	String data = ta.getText();
	writer.write(data);
	writer.close();
	} catch (FileNotFoundException e1) {
	e1.printStackTrace();
	} catch (IOException ee) {
	ee.printStackTrace();
	}
	}
	}
	if (cmd.equals("版权")) {
	JOptionPane.showMessageDialog(this, "版权归 Winter 所有!", "版权声明",
	JOptionPane.INFORMATION_MESSAGE);
	}
	if (cmd.equals("剪切")) {
	ta.cut();
	}
	if (cmd.equals("复制")) {
	ta.copy();
	}
	if (cmd.equals("粘贴")) {
	ta.paste();
	}
	if (cmd.equals("全选")) {
	ta.selectAll();
	}
	// 退出
	if (cmd.equals("退出")) {
	exit();
	}
	}
	public void exit() {
	if (change_flag == false) {
	System.exit(1);
	} else {
	int sign = JOptionPane.showConfirmDialog(this, "当前文件还没有保存\n 是否退出？",
	"提示", JOptionPane.YES_NO_CANCEL_OPTION);
	if (sign == JOptionPane.YES_OPTION) {
	BufferedWriter writer;
	try {
	writer = new BufferedWriter(new FileWriter("d:/新建文档.txt"));
	String data = ta.getText();
	writer.write(data);
	writer.close();
	} catch (FileNotFoundException e1) {
	e1.printStackTrace();
	} catch (IOException ee) {
	ee.printStackTrace();
	}
	}
	if (sign == JOptionPane.NO_OPTION) {
	System.exit(1);
	}
	if (sign == JOptionPane.CANCEL_OPTION)
	;
	}
	}
}