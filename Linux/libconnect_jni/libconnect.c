/*
 * libconnect.c
 *
 *  Created on: 2017-9-12
 *      Author: root
 */
#include <jni.h>
#include "com_libconnect_jnitest_libconnect.h"
#include "string.h"


JNIEXPORT jstring JNICALL Java_com_libconnect_jnitest_libconnect_libconnect_1test
  (JNIEnv * env, jobject obj, jstring str)
{

	return str;
}
