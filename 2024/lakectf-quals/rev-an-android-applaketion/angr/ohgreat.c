#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <jni.h>

jint JNI_OnLoad(JavaVM* vm, void* reserved);

static char flag[128];

static jclass env_FindClass(JNIEnv *env, const char *name) {
    return 0;
}

static jint env_RegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods, jint nMethods) {
    assert(nMethods == 1);
    jboolean (*fn)(JNIEnv *, jobject, jstring) = methods[0].fnPtr;
    //printf("RegisterNatives %s%s = %p\n", methods[0].name, methods[0].signature, methods[0].fnPtr);
    jboolean res = fn(env, 0, 0);
    //printf("returned %d\n", res);
    if (!res)
        exit(1);
    return 0;
}

static const char *env_GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy) {
    return flag;
}

static struct JNINativeInterface native_interface = {
    .FindClass = env_FindClass,
    .RegisterNatives = env_RegisterNatives,
    .GetStringUTFChars = env_GetStringUTFChars,
};

static JNIEnv env = &native_interface;

static jint vm_GetEnv(JavaVM *vm, void **res, jint version) {
    *res = (void *)&env;
    return 0;
}

static struct JNIInvokeInterface invoke_interface = {
    .GetEnv = vm_GetEnv,
};

static JavaVM vm = &invoke_interface;

int main(int argc, char **argv) {
    //setvbuf(stdout, 0, _IONBF, 0);
    //setvbuf(stderr, 0, _IONBF, 0);
    fgets(flag, sizeof(flag), stdin);
    JNI_OnLoad(&vm, NULL);
    printf("OK\n");
    return 0;
}
