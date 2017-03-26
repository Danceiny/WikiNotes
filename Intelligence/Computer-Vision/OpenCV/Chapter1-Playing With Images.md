---
date: 2016-05-18 09:43
status: public
title: 'Chapter1-Playing With Images'
---

# Cover
* Installing the OpenCV Library
* Creating an OpenCV project with MS Visual C++
* Creating an OpenCV project with Qt
* Loading, displaying, and saving images
* Creating a GUI application using Qt
* Exploring the cv::Mat data structure
* Defining regions of interest

# IplImage
```C++:n
IplImage* iplImage = cvLoadImage("..");
cv::Mat image4(iplImage,false);//false:the default value,the data will not be copied.

//need to be careful to not create dangling(摇晃地悬挂着) pointers. So it's safer to encapsulate(封装) the IplImage pointer into the reference-counting pointer class provided by OpenCV2:
cv::Ptr<IplImage> iplImage = cvLoadImage("..");
```

# Qt Image
Displaying an image directly on the GUI is relatively easy with Qt. All you need to do is add
a label object to your window. You then assign an image to this label in order to display this
image. Remember, you have access to the label instance via the corresponding pointer
attribute of the ui pointer (ui->label in our example). But this image must be of type
QImage, the Qt data structure to handle images. The conversion is relatively straightforward
except that the order of the three color channels needs to be inverted (from BGR in our
cv::Mat to RGB in QImage). We can use the cv::cvtColor function for this. The Process
button of our simple GUI application can then be changed to:
```C++:n
void MainWindow::on_pushButton_2_clicked()
{
	cv::flip(image,image,1);//process the image 
	//change  color channel ordering
	cv::cvtColor(image,image,CV_BGR2RGB);
	//qt image
	QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
	//display on label 
	ui->label->setPixmap(QPixmap::fromImage(img));
	//resize the label to fit the image
	ui->label->resize(ui->label->pixmap()->size());
}
```

![](~/01-02-22.jpg)
