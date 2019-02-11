#include "header.h"

using namespace openrasp;

/*
 * Class:     com_baidu_openrasp_plugin_v8_V8
 * Method:    Initialize
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_baidu_openrasp_plugin_v8_V8_Initialize(JNIEnv *env, jclass cls)
{
  if (!isInitialized)
  {
    this_class = ThisClass(env, cls);
    ctx_class = ContextClass(env);
    Platform::Initialize();
    v8::V8::Initialize();
    isInitialized = true;
  }
  return isInitialized;
}

/*
 * Class:     com_baidu_openrasp_plugin_v8_V8
 * Method:    Dispose
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_baidu_openrasp_plugin_v8_V8_Dispose(JNIEnv *env, jclass cls)
{
  if (isInitialized)
  {
    delete snapshot;
    Platform::Shutdown();
    v8::V8::Dispose();
    isInitialized = false;
  }
  return !isInitialized;
}

/*
 * Class:     com_baidu_openrasp_plugin_v8_V8
 * Method:    CreateSnapshot
 * Signature: (Ljava/lang/String;[Ljava/lang/Object;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_baidu_openrasp_plugin_v8_V8_CreateSnapshot(JNIEnv *env, jclass cls, jstring jconfig, jobjectArray jplugins)
{
  const char *raw_config = env->GetStringUTFChars(jconfig, 0);
  std::string config(raw_config);
  env->ReleaseStringUTFChars(jconfig, raw_config);

  std::vector<PluginFile> plugin_list;
  const size_t plugin_len = env->GetArrayLength(jplugins);
  for (int i = 0; i < plugin_len; i++)
  {
    jobjectArray plugin = (jobjectArray)env->GetObjectArrayElement(jplugins, i);
    jstring name = (jstring)env->GetObjectArrayElement(plugin, 0);
    jstring source = (jstring)env->GetObjectArrayElement(plugin, 1);
    const char *raw_name = env->GetStringUTFChars(name, 0);
    const char *raw_source = env->GetStringUTFChars(source, 0);
    plugin_list.emplace_back(raw_name, raw_source);
    env->ReleaseStringUTFChars(name, raw_name);
    env->ReleaseStringUTFChars(source, raw_source);
  }
  auto duration = std::chrono::system_clock::now().time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  Isolate::Data data;
  Isolate::Data::Custom custom_data;
  custom_data.env = env;
  data.custom_data = &custom_data;
  Snapshot *blob = new Snapshot(config, plugin_list, millis, &data);
  if (!blob->IsOk())
  {
    delete blob;
  }
  else
  {
    std::lock_guard<std::mutex> lock(mtx);
    delete snapshot;
    snapshot = blob;
  }
  return true;
}

/*
 * Class:     com_baidu_openrasp_plugin_v8_V8
 * Method:    Check
 * Signature: (Ljava/lang/String;[BI[BI)Z
 */
JNIEXPORT jboolean JNICALL Java_com_baidu_openrasp_plugin_v8_V8_Check__Ljava_lang_String_2_3BI_3BI(JNIEnv *env, jclass cls, jstring jtype, jbyteArray jparams, jint jparams_size, jbyteArray jcontext, jint jcontext_size)
{
  Isolate *isolate = GetIsolate();
  if (!isolate)
  {
    return false;
  }
  isolate->GetData()->custom_data->env = env;
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);
  v8::Local<v8::String> type;
  v8::Local<v8::Object> request_params;
  v8::Local<v8::Object> request_context;

  {
    const jchar *raw = env->GetStringCritical(jtype, nullptr);
    const size_t len = env->GetStringLength(jtype);
    type = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
    env->ReleaseStringCritical(jtype, raw);
  }

  {
    char *raw = static_cast<char *>(env->GetPrimitiveArrayCritical(jparams, nullptr));
    v8::Local<v8::String> json_string = v8::String::NewFromUtf8(isolate, raw, v8::NewStringType::kNormal, jparams_size).ToLocalChecked();
    request_params = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked().As<v8::Object>();
    env->ReleasePrimitiveArrayCritical(jparams, raw, JNI_ABORT);
  }

  {
    char *raw = static_cast<char *>(env->GetPrimitiveArrayCritical(jcontext, nullptr));
    v8::Local<v8::String> json_string = v8::String::NewFromUtf8(isolate, raw, v8::NewStringType::kNormal, jcontext_size).ToLocalChecked();
    request_context = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked().As<v8::Object>();
    env->ReleasePrimitiveArrayCritical(jcontext, raw, JNI_ABORT);
  }

  isolate->GetData()->request_context.Reset(isolate, request_context);

  return isolate->Check(type, request_params);
}

/*
 * Class:     com_baidu_openrasp_plugin_v8_V8
 * Method:    Check
 * Signature: (Ljava/lang/String;[BILcom/baidu/openrasp/plugin/v8/Context;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_baidu_openrasp_plugin_v8_V8_Check__Ljava_lang_String_2_3BILcom_baidu_openrasp_plugin_v8_Context_2Z(JNIEnv *env, jclass cls, jstring jtype, jbyteArray jparams, jint jparams_size, jobject jcontext, jboolean jnew_request)
{
  Isolate *isolate = GetIsolate();
  if (!isolate)
  {
    return false;
  }
  auto data = isolate->GetData();
  data->custom_data->env = env;
  data->custom_data->context = jcontext;
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);
  v8::Local<v8::String> type;
  v8::Local<v8::Object> request_params;

  {
    const jchar *raw = env->GetStringCritical(jtype, nullptr);
    const size_t len = env->GetStringLength(jtype);
    type = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
    env->ReleaseStringCritical(jtype, raw);
  }

  {
    char *raw = static_cast<char *>(env->GetPrimitiveArrayCritical(jparams, nullptr));
    v8::Local<v8::String> json_string = v8::String::NewFromUtf8(isolate, raw, v8::NewStringType::kNormal, jparams_size).ToLocalChecked();
    request_params = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked().As<v8::Object>();
    env->ReleasePrimitiveArrayCritical(jparams, raw, JNI_ABORT);
  }

  if (jnew_request)
  {
    auto request_context = data->request_context_templ.Get(isolate)->NewInstance();
    isolate->GetData()->request_context.Reset(isolate, request_context);
  }

  return isolate->Check(type, request_params);
}