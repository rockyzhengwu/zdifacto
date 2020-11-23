/**
 * Copyright (c) 2015 by Contributors
 */
#include "zdifacto/tracker.h"
#include "local_tracker.h"
namespace zdifacto {

Tracker* Tracker::Create() {
  if (IsDistributed()) {
    LOG(FATAL) << "not implemented";
    return nullptr;
  } else {
    return new LocalTracker();
  }
}

}  
