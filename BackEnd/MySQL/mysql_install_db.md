`$ sudo mysql_install_db`

## output
```
sudo: unable to resolve host c4e9436b8530e4d739970e94943b18d9f-node2
WARNING: The host 'c4e9436b8530e4d739970e94943b18d9f-node2' could not be looked up with resolveip.
This probably means that your libc libraries are not 100 % compatible
with this binary MySQL version. The MySQL daemon, mysqld, should work
normally with the exception that host name resolving will not work.
This means that you should use IP addresses instead of hostnames
when specifying MySQL privileges !
Installing MySQL system tables...
170810 13:12:13 [Warning] Using unique option prefix key_buffer instead of key_buffer_size is deprecated and will be removed in a future release. Please use the full name instead.
170810 13:12:13 [Note] Ignoring --secure-file-priv value as server is running with --bootstrap.
170810 13:12:13 [Note] /usr/sbin/mysqld (mysqld 5.5.57-0ubuntu0.14.04.1) starting as process 4928 ...
OK
Filling help tables...
170810 13:12:13 [Warning] Using unique option prefix key_buffer instead of key_buffer_size is deprecated and will be removed in a future release. Please use the full name instead.
170810 13:12:13 [Note] Ignoring --secure-file-priv value as server is running with --bootstrap.
170810 13:12:13 [Note] /usr/sbin/mysqld (mysqld 5.5.57-0ubuntu0.14.04.1) starting as process 4934 ...
OK

To start mysqld at boot time you have to copy
support-files/mysql.server to the right place for your system

PLEASE REMEMBER TO SET A PASSWORD FOR THE MySQL root USER !
To do so, start the server, then issue the following commands:

/usr/bin/mysqladmin -u root password 'new-password'
/usr/bin/mysqladmin -u root -h c4e9436b8530e4d739970e94943b18d9f-node2 password 'new-password'

Alternatively you can run:
/usr/bin/mysql_secure_installation

which will also give you the option of removing the test
databases and anonymous user created by default.  This is
strongly recommended for production servers.

See the manual for more instructions.

You can start the MySQL daemon with:
cd /usr ; /usr/bin/mysqld_safe &

You can test the MySQL daemon with mysql-test-run.pl
cd /usr/mysql-test ; perl mysql-test-run.pl

Please report any problems at http://bugs.mysql.com/

```