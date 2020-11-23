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
  // Reader* batch_reader  = new BatchReader(
  //   "/home/zhengwu/workspace/private-github/zdifacto/tests/cpp/data",
  //   "libsvm",
  //   0,
  //   1,
  //   10,
  //   10
  // );
  // std::cout << "batch reader \n";
  // feaid_t b = 0;
  // while(batch_reader->Next()){
  //   std::cout << "batch : " << b << "\n";
  //   std::cout << "hello \n";
  //   dmlc::RowBlock<feaid_t> batch = batch_reader->Value();
  //   std::cout << batch.size << "\n";
  //   ++b ;
  // }

  Reader* reader = new Reader("/home/zhengwu/workspace/private-github/zdifacto/data/gisette_scale.t",
                        "libsvm",
                        0,
                        1,
                        256*1024*1024);
  while (reader->Next()){
    dmlc::RowBlock<feaid_t> data=reader->Value();
    std::cout << "data size " << data.size << "\n";
    // for(size_t i=0; i<data.size; ++i){
    //   dmlc::Row<feaid_t, real_t> row = data[i];
    //   std::cout << *row.label << " " << row.length << "\n";
    //   for(size_t s=0; s < row.length; ++s){
    //     std::cout << row.get_index(s) << ":" << row.get_value(s) <<  " ";
    //   }
    //   std::cout << "\n";
    // }
    std::cout << data.size << "\n";
  }
}