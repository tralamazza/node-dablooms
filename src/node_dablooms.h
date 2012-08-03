#ifndef NODE_DABLOOMS_H
#define NODE_DABLOOMS_H

#include <v8.h>
#include <node.h>
#include <dablooms.h>

using namespace v8;

class NodeCountingBloom : public node::ObjectWrap {
  public:
    static void Init(Handle<Object> target);

  private:
    NodeCountingBloom(uint32_t capacity, double error_rate, const char* filename, bool from_file);
    ~NodeCountingBloom();

    static Handle<Value> New(const Arguments& args);

    static Handle<Value> Add(const Arguments& args);
    static Handle<Value> Remove(const Arguments& args);
    static Handle<Value> Check(const Arguments& args);

    counting_bloom_t* _bloom;
};

class NodeScalingBloom : public node::ObjectWrap {
  public:
    static void Init(Handle<Object> target);

  private:
    NodeScalingBloom(unsigned int capacity, double error_rate, const char* filename, uint32_t id);
    NodeScalingBloom(unsigned int capacity, double error_rate, const char* filename);
    ~NodeScalingBloom();

    static Handle<Value> New(const Arguments& args);

    static Handle<Value> Add(const Arguments& args);
    static Handle<Value> Remove(const Arguments& args);
    static Handle<Value> Check(const Arguments& args);
    static Handle<Value> Flush(const Arguments& args);

    scaling_bloom_t* _bloom;
};

#endif
