#include "com_baidu_openrasp_plugin_v8_V8.h"
#include "openrasp_v8/openrasp_v8_bundle.h"
#include <memory>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

class ThisClass
{
public:
  ThisClass() = default;
  ThisClass(JNIEnv *env, jclass cls) : cls(cls)
  {
    plugin_log = env->GetStaticMethodID(cls, "PluginLog", "(Ljava/lang/String;)V");
    alarm_log = env->GetStaticMethodID(cls, "AlarmLog", "(Ljava/lang/String;)V");
  }
  jclass cls;
  jmethodID ttttt;
  jmethodID plugin_log;
  jmethodID alarm_log;
};

class ContextClass
{
public:
  ContextClass() = default;
  ContextClass(JNIEnv *env)
  {
    cls = env->FindClass("com/baidu/openrasp/plugin/v8/Context");
    getPath = env->GetMethodID(cls, "getPath", "()Ljava/lang/String;");
    getMethod = env->GetMethodID(cls, "getMethod", "()Ljava/lang/String;");
    getUrl = env->GetMethodID(cls, "getUrl", "()Ljava/lang/String;");
    getQuerystring = env->GetMethodID(cls, "getQuerystring", "()Ljava/lang/String;");
    getAppBasePath = env->GetMethodID(cls, "getAppBasePath", "()Ljava/lang/String;");
    getProtocol = env->GetMethodID(cls, "getProtocol", "()Ljava/lang/String;");
    getRemoteAddr = env->GetMethodID(cls, "getRemoteAddr", "()Ljava/lang/String;");
    getBody = env->GetMethodID(cls, "getBody", "([I)[B");
    getHeader = env->GetMethodID(cls, "getHeader", "([I)[B");
    getParameter = env->GetMethodID(cls, "getParameter", "([I)[B");
    getServer = env->GetMethodID(cls, "getServer", "([I)[B");
  }
  jclass cls;
  jmethodID getPath;
  jmethodID getMethod;
  jmethodID getUrl;
  jmethodID getQuerystring;
  jmethodID getAppBasePath;
  jmethodID getProtocol;
  jmethodID getRemoteAddr;
  jmethodID getBody;
  jmethodID getHeader;
  jmethodID getParameter;
  jmethodID getServer;
};

class openrasp::Isolate::Data::Custom
{
public:
  JNIEnv *env = nullptr;
  jobject context = nullptr;
};

class IsolateDeleter
{
public:
  void operator()(openrasp::Isolate *isolate)
  {
    delete isolate->GetData()->custom_data;
    isolate->Dispose();
  }
};
typedef std::unique_ptr<openrasp::Isolate, IsolateDeleter> IsolatePtr;


extern ThisClass this_class;
extern ContextClass ctx_class;
extern bool isInitialized;
extern openrasp::Snapshot *snapshot;
extern std::mutex mtx;


openrasp::Isolate *GetIsolate();