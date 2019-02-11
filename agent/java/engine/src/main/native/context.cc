/*
 * Copyright 2017-2019 Baidu Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "header.h"

namespace openrasp
{
enum FieldIndex
{
  kUrl = 0,
  kHeader,
  kParameter,
  kPath,
  kQuerystring,
  kMethod,
  kProtocol,
  kRemoteAddr,
  kAppBasePath,
  kBody,
  kServer,
  kJsonBody,
  kEndForCount
};
inline Isolate *GetIsolate(const v8::PropertyCallbackInfo<v8::Value> &info)
{
  return reinterpret_cast<Isolate *>(info.GetIsolate());
}
static void url_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kUrl);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jurl = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getUrl);
  const jchar *raw = custom_data->env->GetStringCritical(jurl, nullptr);
  const size_t len = custom_data->env->GetStringLength(jurl);
  auto url = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jurl, raw);

  returnValue.Set(url);
  self->SetInternalField(kUrl, url);
}
static void method_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kMethod);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jmethod = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getMethod);
  const jchar *raw = custom_data->env->GetStringCritical(jmethod, nullptr);
  const size_t len = custom_data->env->GetStringLength(jmethod);
  auto method = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jmethod, raw);

  returnValue.Set(method);
  self->SetInternalField(kMethod, method);
}
static void querystring_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kQuerystring);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jquerystring = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getQuerystring);
  const jchar *raw = custom_data->env->GetStringCritical(jquerystring, nullptr);
  const size_t len = custom_data->env->GetStringLength(jquerystring);
  auto querystring = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jquerystring, raw);

  returnValue.Set(querystring);
  self->SetInternalField(kQuerystring, querystring);
}
static void appBasePath_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kAppBasePath);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jappBasePath = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getAppBasePath);
  const jchar *raw = custom_data->env->GetStringCritical(jappBasePath, nullptr);
  const size_t len = custom_data->env->GetStringLength(jappBasePath);
  auto appBasePath = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jappBasePath, raw);

  returnValue.Set(appBasePath);
  self->SetInternalField(kAppBasePath, appBasePath);
}
static void protocol_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kProtocol);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jprotocol = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getProtocol);
  const jchar *raw = custom_data->env->GetStringCritical(jprotocol, nullptr);
  const size_t len = custom_data->env->GetStringLength(jprotocol);
  auto protocol = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jprotocol, raw);

  returnValue.Set(protocol);
  self->SetInternalField(kProtocol, protocol);
}
static void remoteAddr_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kRemoteAddr);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jremoteAddr = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getRemoteAddr);
  const jchar *raw = custom_data->env->GetStringCritical(jremoteAddr, nullptr);
  const size_t len = custom_data->env->GetStringLength(jremoteAddr);
  auto remoteAddr = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jremoteAddr, raw);

  returnValue.Set(remoteAddr);
  self->SetInternalField(kRemoteAddr, remoteAddr);
}
static void path_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kPath);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jpath = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getPath);
  const jchar *raw = custom_data->env->GetStringCritical(jpath, nullptr);
  const size_t len = custom_data->env->GetStringLength(jpath);
  auto path = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jpath, raw);

  returnValue.Set(path);
  self->SetInternalField(kPath, path);
}
static void parameter_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kParameter);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jintArray jsize = custom_data->env->NewIntArray(1);
  jbyteArray jparameter = (jbyteArray)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getParameter, jsize);
  if (!jparameter)
  {
    returnValue.Set(v8::Object::New(isolate));
    return;
  }
  jint jparameter_size;
  custom_data->env->GetIntArrayRegion(jsize, 0, 1, &jparameter_size);
  char *raw = static_cast<char *>(custom_data->env->GetPrimitiveArrayCritical(jparameter, nullptr));
  auto json_string = v8::String::NewFromUtf8(isolate, raw, v8::NewStringType::kNormal, jparameter_size).ToLocalChecked();
  auto parameter = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked();
  custom_data->env->ReleasePrimitiveArrayCritical(jparameter, raw, JNI_ABORT);

  returnValue.Set(parameter);
  self->SetInternalField(kParameter, parameter);
}
static void header_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kHeader);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jintArray jsize = custom_data->env->NewIntArray(1);
  jbyteArray jheader = (jbyteArray)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getHeader, jsize);
  if (!jheader)
  {
    returnValue.Set(v8::Object::New(isolate));
    return;
  }
  jint jheader_size;
  custom_data->env->GetIntArrayRegion(jsize, 0, 1, &jheader_size);
  char *raw = static_cast<char *>(custom_data->env->GetPrimitiveArrayCritical(jheader, nullptr));
  auto json_string = v8::String::NewFromUtf8(isolate, raw, v8::NewStringType::kNormal, jheader_size).ToLocalChecked();
  auto header = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked();
  custom_data->env->ReleasePrimitiveArrayCritical(jheader, raw, JNI_ABORT);

  returnValue.Set(header);
  self->SetInternalField(kHeader, header);
}
static void body_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kBody);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jintArray jsize = custom_data->env->NewIntArray(1);
  jbyteArray jbody = (jbyteArray)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getBody, jsize);
  if (!jbody)
  {
    returnValue.Set(v8::ArrayBuffer::New(isolate, 0));
    return;
  }
  jint jbody_size;
  custom_data->env->GetIntArrayRegion(jsize, 0, 1, &jbody_size);
  char *raw = static_cast<char *>(custom_data->env->GetPrimitiveArrayCritical(jbody, nullptr));
  auto body = v8::ArrayBuffer::New(isolate, raw, jbody_size);
  custom_data->env->ReleasePrimitiveArrayCritical(jbody, raw, JNI_ABORT);

  returnValue.Set(body);
  self->SetInternalField(kBody, body);
}
static void server_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kServer);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jintArray jsize = custom_data->env->NewIntArray(1);
  jbyteArray jserver = (jbyteArray)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getServer, jsize);
  if (!jserver)
  {
    returnValue.Set(v8::Object::New(isolate));
    return;
  }
  jint jserver_size;
  custom_data->env->GetIntArrayRegion(jsize, 0, 1, &jserver_size);
  char *raw = static_cast<char *>(custom_data->env->GetPrimitiveArrayCritical(jserver, nullptr));
  auto json_string = v8::String::NewFromUtf8(isolate, raw, v8::NewStringType::kNormal, jserver_size).ToLocalChecked();
  auto server = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked();
  custom_data->env->ReleasePrimitiveArrayCritical(jserver, raw, JNI_ABORT);

  returnValue.Set(server);
  self->SetInternalField(kServer, server);
}

static void json_body_getter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
  auto self = info.Holder();
  auto returnValue = info.GetReturnValue();
  auto cache = self->GetInternalField(kJsonBody);
  if (!cache->IsUndefined())
  {
    returnValue.Set(cache);
    return;
  }

  auto isolate = GetIsolate(info);
  auto custom_data = isolate->GetData()->custom_data;
  jstring jjson = (jstring)custom_data->env->CallObjectMethod(custom_data->context, ctx_class.getJson);
  if (!jjson)
  {
    returnValue.Set(v8::Object::New(isolate));
    return;
  }
  const jchar *raw = custom_data->env->GetStringCritical(jjson, nullptr);
  const size_t len = custom_data->env->GetStringLength(jjson);
  auto json_string = v8::String::NewFromTwoByte(isolate, raw, v8::NewStringType::kNormal, len).ToLocalChecked();
  auto json = v8::JSON::Parse(isolate->GetCurrentContext(), json_string).ToLocalChecked();
  custom_data->env->ReleaseStringCritical(jjson, raw);

  returnValue.Set(json);
  self->SetInternalField(kJsonBody, json);
}

v8::Local<v8::ObjectTemplate> NewRequestContextTemplate(v8::Isolate *isolate)
{
  auto obj_templ = v8::ObjectTemplate::New(isolate);
  obj_templ->SetAccessor(NewV8String(isolate, "url"), url_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "header"), header_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "parameter"), parameter_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "path"), path_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "querystring"), querystring_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "method"), method_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "protocol"), protocol_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "remoteAddr"), remoteAddr_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "appBasePath"), appBasePath_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "body"), body_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "server"), server_getter);
  obj_templ->SetAccessor(NewV8String(isolate, "json"), json_body_getter);
  obj_templ->SetInternalFieldCount(kEndForCount);
  return obj_templ;
}
} // namespace openrasp