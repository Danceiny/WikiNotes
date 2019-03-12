当前位置:首页 > arch安装软件提示包损坏
arch安装软件提示包损坏
错误：lib32-libjpeg6-turbo: signature from "Colin Keenan <colinnkeenan@gmail.com>" is unknown trust:: 文件 /var/cache/pacman/pkg/lib32-libj
正文
错误：lib32-libjpeg6-turbo: signature from "Colin Keenan <colinnkeenan@gmail.com>" is unknown trust
:: 文件 /var/cache/pacman/pkg/lib32-libjpeg6-turbo-1.4.2-2-x86_64.pkg.tar.xz 已损坏 (无效或已损坏的软件包 (PGP 签名)).
打算删除吗？ [Y/n] n
错误：无法提交处理 (无效或已损坏的软件包)
发生错误，没有软件包被更新。

解决方法:
到/var/cache/pacman/pkg直接`pacman -U *`安装就行
