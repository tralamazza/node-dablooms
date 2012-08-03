#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "node_dablooms.h"

using namespace v8;

extern "C" void init(Handle<Object> target) {
  NodeCountingBloom::Init(target);
  NodeScalingBloom::Init(target);
}

NODE_MODULE(dablooms, init)
