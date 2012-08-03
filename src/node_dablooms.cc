#define BUILDING_NODE_EXTENSION

#include <v8.h>
#include <node.h>
#include <dablooms.h>
#include "node_dablooms.h"

using namespace v8;

/* NodeCountingBloom */
void NodeCountingBloom::Init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "add", Add);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove", Remove);
  NODE_SET_PROTOTYPE_METHOD(tpl, "check", Check);
  target->Set(String::NewSymbol("countingBloom"), tpl->GetFunction());
}

NodeCountingBloom::NodeCountingBloom(unsigned int capacity, double error_rate, const char *filename, bool from_file) : node::ObjectWrap() {
  _bloom = from_file ?
    new_counting_bloom_from_file(capacity, error_rate, filename) :
    new_counting_bloom(capacity, error_rate, filename);
}

NodeCountingBloom::~NodeCountingBloom() {
  free_counting_bloom(_bloom);
  _bloom = NULL;
}

Handle<Value> NodeCountingBloom::New(const Arguments& args) {
  NodeCountingBloom* obj = new NodeCountingBloom(args[0]->Uint32Value(), args[1]->NumberValue(), *String::AsciiValue(args[2]), args[3]->BooleanValue());
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
  int ret = counting_bloom_add(obj->_bloom, *String::AsciiValue(args[0]));
  return scope.Close(Int32::New(ret));
}

Handle<Value> NodeCountingBloom::Remove(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(args.This()); 
  int ret = counting_bloom_remove(obj->_bloom, *String::AsciiValue(args[0]));
  return scope.Close(Int32::New(ret));
}

Handle<Value> NodeCountingBloom::Check(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect string as first param")));

  NodeCountingBloom* obj = ObjectWrap::Unwrap<NodeCountingBloom>(args.This()); 
  int ret = counting_bloom_check(obj->_bloom, *String::AsciiValue(args[0]));
  return scope.Close(Int32::New(ret));
}

/* NodeScalingBloom */
void NodeScalingBloom::Init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "add", Add);
  NODE_SET_PROTOTYPE_METHOD(tpl, "remove", Remove);
  NODE_SET_PROTOTYPE_METHOD(tpl, "check", Check);
  NODE_SET_PROTOTYPE_METHOD(tpl, "flush", Flush);
  target->Set(String::NewSymbol("scalingBloom"), tpl->GetFunction());
}

NodeScalingBloom::NodeScalingBloom(unsigned int capacity, double error_rate, const char* filename, uint32_t id) : node::ObjectWrap() {
  _bloom = new_scaling_bloom(capacity, error_rate, filename, id);
}

NodeScalingBloom::NodeScalingBloom(unsigned int capacity, double error_rate, const char* filename) : node::ObjectWrap() {
  _bloom = new_scaling_bloom_from_file(capacity, error_rate, filename);
}

NodeScalingBloom::~NodeScalingBloom() {
  free_scaling_bloom(_bloom);
  _bloom = NULL;
}

Handle<Value> NodeScalingBloom::New(const Arguments& args) {
  NodeScalingBloom* obj;
  if (args.Length() > 3 && args[3]->IsUint32())
    obj = new NodeScalingBloom(args[0]->Uint32Value(), args[1]->NumberValue(), *String::AsciiValue(args[2]), args[3]->Uint32Value());
  else
    obj = new NodeScalingBloom(args[0]->Uint32Value(), args[1]->NumberValue(), *String::AsciiValue(args[2]));
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
  int ret = scaling_bloom_add(obj->_bloom, *String::AsciiValue(args[0]), args[1]->Uint32Value());
  return scope.Close(Int32::New(ret));
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
  int ret = scaling_bloom_remove(obj->_bloom, *String::AsciiValue(args[0]), args[1]->Uint32Value());
  return scope.Close(Int32::New(ret));
}

Handle<Value> NodeScalingBloom::Check(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1)
    return ThrowException(Exception::TypeError(String::New("Insufficient params")));
  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expect integer as second param")));

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(args.This()); 
  int ret = scaling_bloom_check(obj->_bloom, *String::AsciiValue(args[0]));
  return scope.Close(Int32::New(ret));
}

Handle<Value> NodeScalingBloom::Flush(const Arguments& args) {
  HandleScope scope;

  NodeScalingBloom* obj = ObjectWrap::Unwrap<NodeScalingBloom>(args.This()); 
  int ret = scaling_bloom_flush(obj->_bloom);
  return scope.Close(Int32::New(ret));
}
