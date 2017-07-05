http://poi.apache.org/
创建Excel文档：
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFCell;
import java.io.FileOutputStream;
public class CreateXL {
　/** Excel 文件要存放的位置，假定在D盘JTest目录下*/
　public static String outputFile="D:/JTest/ gongye.xls";
　public static void main(String argv[]){
　try{
// 创建新的Excel 工作簿
HSSFWorkbook workbook = new HSSFWorkbook();
// 在Excel工作簿中建一工作表，其名为缺省值
// 如要新建一名为"效益指标"的工作表，其语句为：
// HSSFSheet sheet = workbook.createSheet("效益指标");
HSSFSheet sheet = workbook.createSheet();
// 在索引0的位置创建行（最顶端的行）
HSSFRow row = sheet.createRow((short)0);
//在索引0的位置创建单元格（左上端）
HSSFCell cell = row.createCell((short) 0);
// 定义单元格为字符串类型
cell.setCellType(HSSFCell.CELL_TYPE_STRING);
// 在单元格中输入一些内容
cell.setCellValue("增加值");
// 新建一输出文件流
FileOutputStream fOut = new FileOutputStream(outputFile);
// 把相应的Excel 工作簿存盘
workbook.write(fOut);
fOut.flush();
// 操作结束，关闭文件
fOut.close();
System.out.println("文件生成...");
　}catch(Exception e) {
System.out.println("已运行 xlCreate() : " + e );
　}
}
}
读取Excel：
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFCell;
import java.io.FileInputStream;
public class ReadXL {
　/** Excel文件的存放位置。注意是正斜线*/
　public static String fileToBeRead="D:/JTest/ gongye.xls";
　public static void main(String argv[]){ 
　try{
// 创建对Excel工作簿文件的引用
HSSFWorkbook workbook = new HSSFWorkbook(new FileInputStream(fileToBeRead));
// 创建对工作表的引用。
// 本例是按名引用（让我们假定那张表有着缺省名"Sheet1"）
HSSFSheet sheet = workbook.getSheet("Sheet1");
// 也可用getSheetAt(int index)按索引引用，
// 在Excel文档中，第一张工作表的缺省索引是0，
// 其语句为：HSSFSheet sheet = workbook.getSheetAt(0);
// 读取左上端单元
HSSFRow row = sheet.getRow(0);
HSSFCell cell = row.getCell((short)0);
// 输出单元内容，cell.getStringCellValue()就是取所在单元的值
System.out.println("左上端单元是： " + cell.getStringCellValue()); 
　}catch(Exception e) {
System.out.println("已运行xlRead() : " + e );
　}
}
}