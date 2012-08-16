#define BUILDING_NODE_EXTENSION
#include "node_dablooms.h"
#include <cstring>

using namespace v8;

Handle<Value> Version(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(String::New(dablooms_version()));
}

void NodeDabloomsInit(Handle<Object> target) {
  target->SetAccessor(String::NewSymbol("version"), Version);
  NodeCountingBloom::Init(target);
  NodeScalingBloom::Init(target);
}

/* NodeCountingBloom */
void NodeCountingBloom::Init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> tpl = FunctionTemplate::New(NewInstance);
  tpl->SetClassName(String::NewSymbol("CountingBloom"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "add", Add);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove", Remove);
  NODE_SET_PROTOTYPE_METHOD(tpl, "check", Check);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("num_bytes"), GetNumBytes);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("capacity"), GetCapacity);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("error_rate"), GetErrorRate);
  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("CountingBloom"), constructor);
}

NodeCountingBloom::NodeCountingBloom(unsigned int capacity, double error_rate, const char *filename) : node::ObjectWrap() {
  _bloom = new_counting_bloom(capacity, error_rate, filename);
}

NodeCountingBloom::~NodeCountingBloom() {
  free_counting_bloom(_bloom);
  _bloom = NULL;
}

Handle<Value> NodeCountingBloom::NewInstance(const Arguments& args) {
  HandleScope scope;
  NodeCountingBloom* obj = new NodeCountingBloom(args[0]->Uint32Value(), args[1]->NumberValue(), *String::AsciiValue(args[2]));
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> NodeCountingBloom::Add(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(args.This());
  const char* str = *String::AsciiValue(args[0]);
  int ret = counting_bloom_add(obj->_bloom, str, strlen(str));
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeCountingBloom::Remove(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(args.This());
  const char* str = *String::AsciiValue(args[0]);
  int ret = counting_bloom_remove(obj->_bloom, str, strlen(str));
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeCountingBloom::Check(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(args.This());
  const char* str = *String::AsciiValue(args[0]);
  int ret = counting_bloom_check(obj->_bloom, str, strlen(str));
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeCountingBloom::GetNumBytes(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(info.This());
  return scope.Close(Uint32::New(obj->_bloom->num_bytes));
}

Handle<Value> NodeCountingBloom::GetCapacity(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(info.This());
  return scope.Close(Uint32::New(obj->_bloom->capacity));
}

Handle<Value> NodeCountingBloom::GetErrorRate(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(info.This());
  return scope.Close(Number::New(obj->_bloom->error_rate));
}

/* NodeScalingBloom */
void NodeScalingBloom::Init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> tpl = FunctionTemplate::New(NewInstance);
  tpl->SetClassName(String::NewSymbol("ScalingBloom"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "add", Add);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove", Remove);
  NODE_SET_PROTOTYPE_METHOD(tpl, "check", Check);
  NODE_SET_PROTOTYPE_METHOD(tpl, "flush", Flush);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("num_bytes"), GetNumBytes);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("capacity"), GetCapacity);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("error_rate"), GetErrorRate);
  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("num_blooms"), GetNumBlooms);
  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("ScalingBloom"), constructor);
}

NodeScalingBloom::NodeScalingBloom(unsigned int capacity, double error_rate, const char* filename, bool from_file) : node::ObjectWrap() {
  _bloom = from_file ?
    new_scaling_bloom_from_file(capacity, error_rate, filename) :
    new_scaling_bloom(capacity, error_rate, filename);
}

NodeScalingBloom::~NodeScalingBloom() {
  free_scaling_bloom(_bloom);
  _bloom = NULL;
}

Handle<Value> NodeScalingBloom::NewInstance(const Arguments& args) {
  HandleScope scope;
  NodeScalingBloom* obj;
  if (args.Length() > 3 && args[3]->IsBoolean())
    obj = new NodeScalingBloom(args[0]->Uint32Value(), args[1]->NumberValue(), *String::AsciiValue(args[2]), args[3]->BooleanValue());
  else
    obj = new NodeScalingBloom(args[0]->Uint32Value(), args[1]->NumberValue(), *String::AsciiValue(args[2]), false);
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> NodeScalingBloom::Add(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));
  if (!args[1]->IsUint32())
    return ThrowException(Exception::TypeError(String::New("Expect integer as second param")));

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(args.This());
  const char* str = *String::AsciiValue(args[0]);
  int ret = scaling_bloom_add(obj->_bloom, str, strlen(str), args[1]->Uint32Value());
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeScalingBloom::Remove(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));
  if (!args[1]->IsUint32())
    return ThrowException(Exception::TypeError(String::New("Expect integer as second param")));

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(args.This()); 
  const char* str = *String::AsciiValue(args[0]);
  int ret = scaling_bloom_remove(obj->_bloom, str, strlen(str), args[1]->Uint32Value());
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeScalingBloom::Check(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect integer as second param")));

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(args.This());
  const char* str = *String::AsciiValue(args[0]);
  int ret = scaling_bloom_check(obj->_bloom, str, strlen(str));
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeScalingBloom::Flush(const Arguments& args) {
  HandleScope scope;

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(args.This());
  int ret = scaling_bloom_flush(obj->_bloom);
  return scope.Close(Integer::New(ret));
}

Handle<Value> NodeScalingBloom::GetNumBytes(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(info.This());
  return scope.Close(Uint32::New(obj->_bloom->num_bytes));
}

Handle<Value> NodeScalingBloom::GetCapacity(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(info.This());
  return scope.Close(Uint32::New(obj->_bloom->capacity));
}

Handle<Value> NodeScalingBloom::GetErrorRate(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(info.This());
  return scope.Close(Number::New(obj->_bloom->error_rate));
}

Handle<Value> NodeScalingBloom::GetNumBlooms(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(info.This());
  return scope.Close(Uint32::New(obj->_bloom->num_blooms));
}
