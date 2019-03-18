---
date: 2016-01-13 23:11
status: public
title: IIS-port80
---

## 启动IIS：    
     net start iisadmin    （IIS的整个服务）
     net start w3svc       （WWW网页WEB服务） 
## 停止IIS：     
    net stop iisadmin /y     （会自动停止www、ftp和smtp服务）            
    如果用的IIS自带FTP还可以执行 net start MSFtpsvc 命令来启动FTP 
    停止IIS命令（APMServ 80端口被占用,导致Apache启动失败的原因）   一些朋友使用的是APMServ,可是启动的时候,总会提示80端口被占用,Apache启动失败.找了很多,什么迅雷占用80端口,检查了,没有这种效果.想了很久,都不知道该怎么办,下一步就要检查下了,大部分人都是因为机器上IIS启动了.所以只需要把IIS整个服务关。  
   
## 通过命名行使用iisreset命令加参数对IIS进行控制： 
    iisreset [computername]    
    /RESTART 停止然后重新启动所有 Internet 服务
    /START  启动所有 Internet 服务。
    /STOP   停止所有 Internet 服务。      
    /REBOOT  重新启动计算机。    
    /REBOOTonERROR   如果在启动、停止或重启 Internet 服务时发生错误，则重新启动计算机。
     /NOFORCE  假如尝试停止失败时，不要强制终止 Internet 服务。 
    /TIMEOUT:val  指定超时值(以秒为单位)来等待Internet 服务的成功终止。假如 REBOOTonERROR 参数已被指定，超时后可以重新启动计算机。默认值为 20 秒重新开始，60 秒停止，0 秒重新启动 。
    /STATUS    显示所有 Internet 服务的状态。          /                                    