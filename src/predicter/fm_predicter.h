#ifndef ZDIFACTO_PREDICT_FM_PREDICTOR_H_
#define ZDIFACTO_PREDICT_FM_PREDICTOR_H_

#include "dmlc/data.h"
#include "zdifacto/loss.h"
#include "zdifacto/predicter.h"
#include "reader/reader.h"
#include "reader/batch_reader.h"
#include "zdifacto/updater.h"
#include "sgd/sgd_updater.h"

#include <memory>
#include <iostream>


namespace zdifacto {

  struct FMPredicterParam : public dmlc::Parameter<FMPredicterParam> {
  /**
   * \brief the embedding dimension
   */
  std::string model_in;
  std::string data_val;
  std::string data_out;

  DMLC_DECLARE_PARAMETER(FMPredicterParam) {
    DMLC_DECLARE_FIELD(model_in).set_default("fm.model");
    DMLC_DECLARE_FIELD(data_val);
    DMLC_DECLARE_FIELD(data_out).set_default("output.txt");
  }
};

class FMPredicter: public Predicter{
  public:
    FMPredicter(){}
    virtual ~FMPredicter(){}

    KWArgs Init(const KWArgs& kwargs) override;
    void GetPos(const SArray<int>& len, SArray<int>* w_pos, SArray<int>* V_pos) ;
    void PredictCli() override;
    
  private: 
    FMPredicterParam param_;
    std::shared_ptr<Updater> updater_;
    
};

}
#endif
