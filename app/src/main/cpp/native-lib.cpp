#include <jni.h>
#include <thread>
#include <chrono>
#include "print.hpp"

class PlatformBridge {
 public:
  PlatformBridge(JNIEnv *jniEnv, jobject mainActivity)
      : jniEnv(jniEnv),
        mainActivity(mainActivity),
        methodToBeCalledFromJNI(
            jniEnv->GetMethodID(jniEnv->GetObjectClass(mainActivity),
                                "methodToBeCalledFromJNI",
                                "()V")) {}
  void callThreadStartedMethod() const {
    jniEnv->CallVoidMethod(mainActivity, methodToBeCalledFromJNI);
  }
 private:
  JNIEnv *jniEnv;
  jobject mainActivity;
  jmethodID methodToBeCalledFromJNI;
};

void threadBody(PlatformBridge& bridge) {
  for (int i = 0; i < 5; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    bridge.callThreadStartedMethod();
  }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_remember_MainActivity_startThread(
    JNIEnv *env,
    jobject mainActivity) {
  JavaVM *javaVM;
  env->GetJavaVM(&javaVM);

  jobject mainActivityGlobal = env->NewGlobalRef(mainActivity);

  std::thread thr([javaVM, mainActivityGlobal] {
    JNIEnv *env;
    javaVM->AttachCurrentThread(&env, nullptr);

    PlatformBridge bridge(env, mainActivityGlobal);
    threadBody(bridge);

    env->DeleteGlobalRef(mainActivityGlobal);
    javaVM->DetachCurrentThread();
  });
  thr.detach();
}
