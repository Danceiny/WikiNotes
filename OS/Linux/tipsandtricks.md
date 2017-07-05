## 忘记分配交换空间（swap space）
`dd if=/dev/zero of=/swapfile bs=1024 count=65536`会在硬盘上分配67MB的文件，然后初始化：
`mkswap /swapfile 65536`,再加到交换池（swap pool）：`swapon /swapfile`.


## 默认的文件权限
由掩码umask控制.
`umask 077`设置掩码为077，则对应的默认文件权限是700。

## 读取FAT32磁盘
`mount -tvfat /dev/hdb1 /mnt`

## 自动登录
```conf
# /etc/lightdm/lighttm.conf
autologin-user=root
```

## GCC
gcc不是编译器，而是driver。
-E 仅做预处理，不进行编译汇编和链接。
-S 只进行编译，不进行汇编和链接。