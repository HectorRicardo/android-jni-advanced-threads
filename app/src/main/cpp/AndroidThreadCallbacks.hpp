//
// Created by hecto on 1/4/2022.
//

#ifndef MYAPP_ANDROID_THREAD_CALLBACKS_HPP
#define MYAPP_ANDROID_THREAD_CALLBACKS_HPP

#include <jni.h>
#include "common/ThreadCallbacks.hpp"

class AndroidThreadCallbacks : public ThreadCallbacks {
 public:
  AndroidThreadCallbacks(JNIEnv *jniEnv, jobject threadCallbacks);
  void onThreadStarted() const override;
 private:
  JNIEnv *jniEnv;
  jobject threadCallbacks;
  jmethodID onThreadStartedMethodID;
};

#endif // MYAPP_ANDROID_THREAD_CALLBACKS_HPP
