#Makefile
#Created on: 2017-9-15
#Author: tecyang



CC :=   gcc
RM := rm -rf  


all : libconnect_maria

libconnect_maria : tec_pcap tec_maria #tec_mysql.c
	$(CC) -g -o libconnect_mariadb \
	tec_maria.c \
	main.c \
	tec_pcap.c \
	-I /usr/include/mariadb \
	-Ldir_path \
	-lmariadb \
	-lpcap

tec_pcap : tec_maria 
	$(CC) -g -c  tec_pcap.c -lpcap  -I /usr/include/mariadb -Ldir_path -lmariadb

tec_maria:
	$(CC)  -g -c tec_maria.c  -I /usr/include/mariadb -Ldir_path -lmariadb



clean :
	$(RM) libconnect_mariadb \
	$(RM) *.so \
	$(RM) *.o
	
	