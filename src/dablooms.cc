#define BUILDING_NODE_EXTENSION
#include "node_dablooms.h"

using namespace v8;

extern "C" void init(Handle<Object> target) {
  NodeDabloomsInit(target);
}

NODE_MODULE(dablooms, init)
