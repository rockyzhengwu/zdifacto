#include "zdifacto/store.h"
#include "store/store_local.h"

namespace zdifacto {

Store* Store::Create() {
  if (IsDistributed()) {
    LOG(FATAL) << "not implemented";
    return nullptr;
  } else {
    return new StoreLocal();
  }
}

}  