/**
 * Copyright (c) 2015 by Contributors
 */

#ifndef ZDIFACTO_LOSS_H_
#define ZDIFACTO_LOSS_H_

#include <string>
#include <vector>
#include <cmath>
#include "dmlc/data.h"
#include "dmlc/omp.h"

#include "zdifacto/sarray.h"
#include "zdifacto/base.h"

namespace zdifacto {

class Loss {
 public:

  static Loss* Create(const std::string& type, int nthreads = DEFAULT_NTHREADS);

  Loss() : nthreads_(DEFAULT_NTHREADS) { }

  virtual ~Loss() { }

  virtual KWArgs Init(const KWArgs& kwargs) = 0;

  virtual void Predict(const dmlc::RowBlock<unsigned>& data,
                       const std::vector<SArray<char>>& param,
                       SArray<real_t>* pred) = 0;
  
  virtual real_t Evaluate(dmlc::real_t const* label,
                          const SArray<real_t>& pred) const {
    real_t objv = 0;
#pragma omp parallel for reduction(+:objv) num_threads(nthreads_)
    for (size_t i = 0; i < pred.size(); ++i) {
      real_t y = label[i] > 0 ? 1 : -1;
      objv += log(1 + exp(- y * pred[i]));
    }
    return objv;
  }


  virtual void CalcGrad(const dmlc::RowBlock<unsigned>& data,
                        const std::vector<SArray<char>>& param,
                        SArray<real_t>* grad) = 0;
 
  void set_nthreads(int nthreads) {
    CHECK_GT(nthreads, 1); CHECK_LT(nthreads, 50);
    nthreads_ = nthreads;
  }


  int nthreads_;
};
}  
#endif  
