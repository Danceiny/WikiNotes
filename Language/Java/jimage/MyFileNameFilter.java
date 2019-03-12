package myimagej;
import java.io.File;
import java.io.FilenameFilter;
public class MyFileNameFilter implements FilenameFilter {
private String[] condition;
public MyFileNameFilter()
{
this.condition=new
String[]{".jpg",".JPG",".BMP",".bmp",".png",".PNG",".TIF","
.tif"};
}
public boolean accept(File dir, String name) {
for(String s:condition)
{
if(name.endsWith(s))return true;
}
return false;
}