#include "header.h"

using namespace openrasp;

ThisClass this_class;
ContextClass ctx_class;
bool isInitialized = false;
Snapshot *snapshot = nullptr;
std::mutex mtx;

void openrasp::plugin_info(Isolate *isolate, const char *message, size_t length)
{
  auto data = isolate->GetData();
  auto custom_data = isolate->GetData()->custom_data;
  if (!custom_data || !custom_data->env)
  {
    return;
  }
  auto env = custom_data->env;
  auto msg = env->NewStringUTF(message);
  env->CallStaticVoidMethod(this_class.cls, this_class.plugin_log, msg);
}
void openrasp::alarm_info(Isolate *isolate, v8::Local<v8::String> type, v8::Local<v8::Object> params, v8::Local<v8::Object> result)
{
  auto custom_data = isolate->GetData()->custom_data;
  if (!custom_data || !custom_data->env)
  {
    return;
  }
  auto env = custom_data->env;
  v8::Local<v8::Value> val;
  if (v8::JSON::Stringify(isolate->GetCurrentContext(), result).ToLocal(&val))
  {
    v8::String::Value value(val);
    auto msg = env->NewString(*value, value.length());
    env->CallStaticVoidMethod(this_class.cls, this_class.alarm_log, msg);
  }
}

Isolate *GetIsolate()
{
  static thread_local IsolatePtr isolate_ptr;
  Isolate *isolate = isolate_ptr.get();
  if (snapshot)
  {
    if (!isolate || isolate->IsExpired(snapshot->timestamp))
    {
      std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
      if (lock)
      {
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        isolate = Isolate::New(snapshot, millis);
        isolate->GetData()->custom_data = new openrasp::Isolate::Data::Custom();
        isolate_ptr.reset(isolate);
      }
    }
  }
  return isolate;
}