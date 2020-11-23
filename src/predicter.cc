#include "zdifacto/predicter.h"
#include "predicter/fm_predicter.h"
#include "reader/reader.h"
#include <iostream>

namespace zdifacto {

DMLC_REGISTER_PARAMETER(FMPredicterParam);

Predicter* Predicter::Create(const std::string& type ) {
  Predicter* predicter = nullptr;
  if (type == "fm") {
    predicter = new FMPredicter();
  } else {
    LOG(FATAL) << "unknown loss type";
  }
  return predicter;
}

KWArgs Predicter::Init(const KWArgs& kwargs){
  return kwargs;
}

}  // namespace difacto