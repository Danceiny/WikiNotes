挂载文件的时候要打开ACL功能：
mount -o acl /dev/sda5/..


getfacl filename

setfacl -m u:nash_su:rwx filename

setfacl -m g:training:rw filename
