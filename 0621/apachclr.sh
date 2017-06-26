#########################################################################
# File Name: apachclr.sh
# Author: C01
# mail: 1003192057@qq.com
# Created Time: 2017年06月22日 星期四 14时52分30秒
#########################################################################
#!/bin/bash
#删除前先保存cgi-bin和html目录下的文件
apt-get --purge remove apache2 -y
rm /etc/apache2 -rf
rm /usr/lib/cgi-bin -rf
rm /var/www/html -rf

apt-get autoremove -y
apt-get install apache2 -y

cd /etc/apache2/mods-enabled; ln -s ../mods-available/cgi.load
cd /etc/apache2/mods-enabled; ln -s ../mods-available/cgid.load
cd /etc/apache2/mods-enabled; ln -s ../mods-available/cgid.conf

/etc/init.d/apache2 restart
