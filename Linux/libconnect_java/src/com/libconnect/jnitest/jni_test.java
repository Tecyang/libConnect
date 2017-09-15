package com.libconnect.jnitest;

//jni test class
//at src
//run sudo javac com/libconnect/jnitest/jni_test.java
//sudo javah -classpath ../bin/ -d ../bin/ -jni com.libconnect.jnitest.libconnect
//sudo sudo cp ../bin/com_libconnect_jnitest_libconnect.h ../../libconnect_jni/


public class jni_test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		 System.out.println( System.getProperty("java.library.path"));
		 System.loadLibrary("connect");
		 libconnect connect = new libconnect();
		 String result = connect.libconnect_test("test jni function!");
		 
		 System.out.println(result);
//		 
		
	}

}

class  libconnect
{
	public native String libconnect_test(String s); 

}

