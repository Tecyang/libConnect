#Makefile
#Created on: 2017-9-12
#Author: tecyang


CC :=   gcc  
RM := rm -rf  
 
all : libconnect

clean :
	$(RM) libconnect libconnect.so
libconnect : libconnect.c
	$(CC) libconnect.c -fPIC -shared -o libconnect.so -I /usr/lib/jvm/java-8-openjdk-amd64/include -I /usr/lib/jvm/java-8-openjdk-amd64/include/linux 
install : 
	cp libconnect.so ../libconnect_java/lib/libconnect.so
