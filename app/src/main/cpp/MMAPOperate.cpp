//
// Created by Apple on 2019-12-05.
//
#include <jni.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <android/log.h>
#include "com_aisaka_mmapdemo_MMAPOperate.h"
#include<string.h>
#include <malloc.h>
#include <string>

using namespace std;

#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR,#TAG,__VA_ARGS__)

#define TAG "MMAPDEMO"

/*
 * Class:     com_aisaka_mmapdemo_MMAPOperate
 * Method:    exit
 * Signature: (J)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_aisaka_mmapdemo_MMAPOperate_exit
        (JNIEnv *env, jclass clazz, jlong handle) {

    char *handle_ptr = reinterpret_cast<char *>(handle);

    munmap(handle_ptr, DEFAULT_MMAP_SIZE);
    LOGE(TAG, "EXIT");

}

/*
 * Class:     com_aisaka_mmapdemo_MMAPOperate
 * Method:    wirteString
 * Signature: (JLjava/lang/String;)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_aisaka_mmapdemo_MMAPOperate_wirteString
        (JNIEnv *env, jclass clazz, jlong handle, jstring content) {


    LOGE(TAG, "write addr long %ld", handle);

    char *handle_ptr = reinterpret_cast<char *>(handle);

    LOGE(TAG, "write addr %p", handle_ptr);

    const char *byts = env->GetStringUTFChars(content, 0);


    string bytss(byts);
    int size = env->GetStringLength(content);

    LOGE(TAG, "write content %s", byts);


    LOGE(TAG, "write size %d", size);

    memcpy(handle_ptr, &size, 4);

    memcpy(handle_ptr + 4, bytss.c_str(), size);


    env->ReleaseStringUTFChars(content,byts);

    return JNI_TRUE;
}

/*
 * Class:     com_aisaka_mmapdemo_MMAPOperate
 * Method:    readString
 * Signature: (J)Ljava/lang/String;
 */
extern "C" JNIEXPORT jstring JNICALL Java_com_aisaka_mmapdemo_MMAPOperate_readString
        (JNIEnv *env, jclass clazz, jlong handle) {

    LOGE(TAG, "write addr long %d", handle);

    char *handle_ptr = reinterpret_cast<char *>(handle);

    LOGE(TAG, "handle addr %p", handle_ptr);

    int size;

    memcpy(&size, handle_ptr, 4);

    LOGE(TAG, "read size %d", size);

    char *str = (char *) malloc(size);

    memcpy(str, handle_ptr + 4, size);

    LOGE(TAG, "read content \t %s", str);

    const jstring content=env->NewStringUTF(str);

    free(str);

    return content;

}

/*
 * Class:     com_aisaka_mmapdemo_MMAPOperate
 * Method:    openFile
 * Signature: (Ljava/lang/String;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_com_aisaka_mmapdemo_MMAPOperate_openFile
        (JNIEnv *env, jclass clazz, jstring filepath) {

    const char *path = env->GetStringUTFChars(filepath, 0);

    LOGE(TAG, "GetStringUTFChars %s", path);

    int fd = open(path, O_RDWR | O_CREAT, S_IRWXU);

    LOGE(TAG, "open %d", fd);

    void *handle;
    handle = mmap(nullptr, DEFAULT_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    LOGE(TAG, "mmap %p", handle);

    env->ReleaseStringUTFChars(filepath, path);

    LOGE(TAG, "openFile");

    return (jlong) handle;
}