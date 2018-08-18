global_defs {                       ##全局配置部分

\#   notification_email {             ##下面几行均为全局通知配置，可以实现出现问题后报警，但功能有限，因此注释掉，并采用Nagios监视lvs运行情况

\#       admin@toxingwang.com

\#   }

\#   notification_email_from master@toxingwang.com

\#   smtp_server smtp.exmail.qq.com

\#   smtp_connect_timeout 30

​	router_id LVS_DEVEL             ##设置lvs的id，在一个网络内应该是唯一的

}

vrrp_instance VI_1 {            ##设置vrrp组，唯一且同一LVS服务器组要相同

​	state MASTER             ##备份LVS服务器设置为BACKUP

​	interface eth0             # #设置对外服务的接口

​	virtual_router_id 51        ##设置虚拟路由标识

​	priority 100                   #设置优先级，数值越大，优先级越高，backup设置为99，这样就能实现当master宕机后自动将backup变为master，而当原master恢复正常时，则现在的master再次变为backup。

​	advert_int 1            ##设置同步时间间隔

​	authentication {         ##设置验证类型和密码，master和buckup一定要设置一样

​		auth_type PASS

​		auth_pass 1111

​	}

​	virtual_ipaddress {          ##设置VIP，可以多个，每个占一行

​		192.168.18.60

​	}

}

virtual_server 192.168.18.60 80 {

​	delay_loop 6            ##健康检查时间间隔，单位s

​	lb_algo wrr             ##负载均衡调度算法设置为加权轮叫

​	lb_kind DR                              ##负载均衡转发规则

​	nat_mask 255.255.255.0   ##网络掩码，DR模式要保障真实服务器和lvs在同一网段

​	persistence_timeout 50    ##会话保持时间，单位s

​	protocol TCP                           ##协议

​	real_server 192.168.18.61 80 {      ##真实服务器配置，80表示端口

​		weight 3                             ##权重

​		TCP_CHECK {                       ##服务器检测方式设置 keepalived的健康检查方式 有：HTTP_GET|SSL_GET|TCP_CHECK|SMTP_CHECK|MISC_CHECK

​			connect_timeout 5    ##连接超时时间

​			nb_get_retry 3	     ##失败重试次数

​			delay_before_retry 3 ##失败重试的间隔时间

​			connect_port 80      ##连接的后端端口

​		}

​	}

​	real_server 192.168.18.62 80 {

​		weight 3

​		TCP_CHECK {

​			connect_timeout 10

​			nb_get_retry 3

​			delay_before_retry 3

​			connect_port 80

​		}

​	}

}