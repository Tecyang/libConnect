#Makefile
#Created on: 2017-9-15
#Author: tecyang



CC :=   gcc
RM := rm -rf  


all : libconnect_mysql

libconnect_mysql : tec_pcap tec_mysql #tec_mysql.c
	$(CC) -g -o libconnect_mysql \
	tec_mysql.c \
	main.c \
	tec_pcap.c \
	-I /usr/include/mysql \
	-Ldir_path \
	-lmysqlclient \
	-lpcap

tec_pcap : tec_mysql 
	$(CC) -g -c  tec_pcap.c -lpcap  -I /usr/include/mysql -Ldir_path -lmysqlclient

tec_mysql:
	$(CC)  -g -c tec_mysql.c  -I /usr/include/mysql -Ldir_path -lmysqlclient

clean :
	$(RM) libconnect_mysql \
	$(RM) *.so \
	$(RM) *.o
	
	