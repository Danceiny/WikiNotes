编写背景：

=========================

目前公司的福利饭补是是人力邮件发送的，每个人需要手动对应的发一遍相应的数据，没研究过outlook是否有读取excel群发邮件的功能，看着人力发送这么吃力，索性写了个工具协助读取excel群发邮件。

===============================
#!/usr/local/bin/python2.7

# encoding: gbk

import wx

import wx.grid

import os

import cPickle

import wx.lib.masked as masked

import subprocess

import time

import traceback 

from email.message import Message

from email.mime.multipart import MIMEMultipart

from email.mime.text import MIMEText

from email.mime.image import MIMEImage

from email.mime.base import MIMEBase

from email import encoders

import mimetypes

from email.mime.audio import MIMEAudio

import xlrd

import pykka

import mailok

import sys

reload(sys)

sys.setdefaultencoding('gbk')

class Example(wx.Frame):

def __init__(self,parent,title):

super(Example,self).__init__(parent,title=title,size=(500,480),style=wx.DEFAULT_FRAME_STYLE ^(wx.MAXIMIZE_BOX | wx.RESIZE_BORDER))

self.InitUI()

self.Centre()

self.Show()



def InitUI(self):



menuBar = wx.MenuBar()

filemenu = wx.Menu()

helpmenu = wx.Menu()

menuBar.Append(filemenu,"&文件")

fitem = filemenu.Append(1001,"&导入excel文件","导入excel文件") 

self.Bind(wx.EVT_MENU, self.OnIn, id=1001) 

fitem = filemenu.Append(1002,"&退出系统","退出系统")

self.Bind(wx.EVT_MENU, self.OnExit, id=1002)

menuBar.Append(helpmenu,"&帮助")

helpem = helpmenu.Append(1003,"&关于V1.0","关于")

self.Bind(wx.EVT_MENU, self.OnAbout, id=1003)

helpem = helpmenu.Append(1004,"&使用说明","使用说明")

self.Bind(wx.EVT_MENU, self.OnHelp, id=1004)

self.SetMenuBar(menuBar)

self.SetTitle("xxxx人力资源-饭补发放程序 V1.0")

self.SetBackgroundColour(wx.Colour(28,134,238)) 



panel=wx.Panel(self,-1)

text=basicLabel = wx.StaticText(panel, -1, "Excel文件路径",(50,115))

font = wx.Font(10, wx.SWISS, wx.NORMAL, wx.BOLD)

text.SetFont(font)

self.basicText1 = wx.TextCtrl(panel,-1,size=(200, 20),pos=(180,115))







self.Center()

self.Show(True)



self.button1 = wx.Button(panel, -1, "发送邮件", pos=(100, 260),size=(80,50))

self.button1.Bind(wx.EVT_BUTTON, self.OnOpen)

self.button3 = wx.Button(panel, -1, "取消发送", pos=(280, 260),size=(80,50))

self.button3.Bind(wx.EVT_BUTTON, self.OnClear)



def OnIn(self, event):

dlg1 = wx.FileDialog(self, "请选择要导入的excel文件...", os.getcwd(),style=wx.OPEN)

if dlg1.ShowModal() == wx.ID_OK:

file = dlg1.GetPath()

self.basicText1.AppendText(file)

#self.ReadFile()

#self.SetTitle(self.title + ' -- ' + self.filename)

dlg1.Destroy()



def OnExit(self, event): 

self.Close(True)

def OnAbout(self, event):

wx.MessageBox("Copyright(c)2014-2088 The xxx\nAll Rights Reserved\nVersion: 1.0\nSupport: wuxiaobing@xxxx.com", "关于",wx.ICON_INFORMATION)

def OnHelp(self, event):

wx.MessageBox("1、该工具适合公司饭补邮件群发。\n2、读取文件[excel]格式必须符合规定的模板。\n3、本版为试用，针对试用下版会改进。", "帮助",wx.ICON_QUESTION)



def OnOpen(self,event):

xmlfile=self.basicText1.GetValue()

book = xlrd.open_workbook(xmlfile)

sheet_name=book.sheet_names()



table = book.sheets()[0]



for i in range(2,table.nrows):

name=table.cell(i,1).value

mail=table.cell(i,2).value

wucan=table.cell(i,3).value

jiaban=table.cell(i,4).value

heji=table.cell(i,5).value

print mailok.sendTextEmail(mail, '2014年7月份餐补金额通知'+'\n',

name+' 您好 :'+'\n'+

' '+'\n'+

' '+'2014年7月餐补：'+str(wucan)+' 元 ,'+'加班餐补：'+str(jiaban)+' 元 ,'+'餐补（午餐+加班餐补）合计:'+str(heji)+'元 。'+'\n'+

' '+'\n'

' '+ '为了不耽误报销，请您提前准备好相应票据，并于2014年7月25日前将票据交到前台票据箱，谢谢!'



'''



xxx

北京xxx有限公司 行政部

地址:北京xxx

邮编：100102

网址：www.xxx.com

邮箱：xxx@xxx.com

手机：xxx

''')

def OnClear(self,event):

self.Close(True)



if __name__ == '__main__':

app = wx.App()

Example(None,title='Layout1')

app.MainLoop()