#include <iostream>

#include "zdifacto/base.h"
#include "reader/reader.h"
#include "reader/batch_reader.h"
#include "common/arg_parser.h"
#include "zdifacto/learner.h"
#include "sgd/sgd_learner.h"
#include "zdifacto/predicter.h"
#include "reader/reader.h"

namespace zdifacto {
struct DifactoParam : public dmlc::Parameter<DifactoParam> {
  
  std::string task;
  std::string learner;
  DMLC_DECLARE_PARAMETER(DifactoParam) {
    DMLC_DECLARE_FIELD(learner).set_default("sgd");
    DMLC_DECLARE_FIELD(task).set_default("train");
  }
};

void WarnUnknownKWArgs(const DifactoParam& param, const KWArgs& remain) {
  if (remain.empty()) return;
  LOG(WARNING) << "Unrecognized keyword argument for task = " << param.task;
  for (auto kw : remain) {
    LOG(WARNING) << " - " << kw.first << " = " << kw.second;
  }
}

DMLC_REGISTER_PARAMETER(DifactoParam);

}  

using namespace zdifacto;
int main(int argc, char *argv[]){
  if (argc < 2) {
    LOG(ERROR) << "usage: difacto key1=val1 key2=val2 ...";
    return 0;
  }
  ArgParser parser;
  for (int i = 1; i < argc; ++i) parser.AddArg(argv[i]);
  DifactoParam param;
  auto kwargs_remain = param.InitAllowUnknown(parser.GetKWArgs());
   if (param.task == "train") {
    Learner* learner = Learner::Create(param.learner);
    WarnUnknownKWArgs(param, learner->Init(kwargs_remain));
    learner->Run();
    delete learner;
  } else if(param.task=="predict"){
    Predicter* predicter = Predicter::Create("fm");
    WarnUnknownKWArgs(param, predicter->Init(kwargs_remain));
    predicter->Run();
  }
}