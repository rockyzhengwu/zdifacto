/**
 * Copyright (c) 2015 by Contributors
 */
#include "zdifacto/loss.h"
#include "loss/fm_loss.h"
#include "loss/logit_loss_delta.h"
#include "loss/logit_loss.h"

namespace zdifacto {

DMLC_REGISTER_PARAMETER(FMLossParam);
DMLC_REGISTER_PARAMETER(LogitLossDeltaParam);

Loss* Loss::Create(const std::string& type, int nthreads) {
  Loss* loss = nullptr;
  if (type == "fm") {
    loss = new FMLoss();
  } else if (type == "logit") {
    loss = new LogitLoss();
  } else if (type == "logit_delta") {
    loss = new LogitLossDelta();
  } else {
    LOG(FATAL) << "unknown loss type";
  }
  loss->set_nthreads(nthreads);
  return loss;
}

}  // namespace difacto
