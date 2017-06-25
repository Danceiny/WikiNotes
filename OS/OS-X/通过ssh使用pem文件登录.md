http://johnny-lee.iteye.com/blog/2232582

ssh -i key.pem root@192.168.1.1  
sudo chmod 600 key.pemd

ssh-add -k key.pem   
执行完上面的命令后下次就只需要像普通ssh那样访问就可以了，如下： 
Java代码  收藏代码
ssh root@192.168.1.1  