/**
 *  Copyright (c) 2015 by Contributors
 */
#ifndef ZDIFACTO_SGD_SGD_LEARNER_H_
#define ZDIFACTO_SGD_SGD_LEARNER_H_
#include <string>
#include <vector>
#include "zdifacto/learner.h"
#include "sgd_param.h"
#include "zdifacto/loss.h"
#include "zdifacto/store.h"

#include "sgd_utils.h"
#include "sgd_updater.h"

namespace zdifacto {

class SGDLearner : public Learner {
 public:
  SGDLearner(){
    loss_ = nullptr;
    store_ = nullptr;
  }

  virtual ~SGDLearner() {
    delete loss_;
    delete store_;
  }

  KWArgs Init(const KWArgs& kwargs) override;

  void AddEpochEndCallback(const std::function<void(
      int epoch, const sgd::Progress& train, const sgd::Progress& val)>& callback) {
    epoch_end_callback_.push_back(callback);
  }

  SGDUpdater* GetUpdater() {
    return CHECK_NOTNULL(std::static_pointer_cast<SGDUpdater>(
        CHECK_NOTNULL(store_)->updater()).get());
  }

 protected:
  void RunScheduler() override;

  void Process(const std::string& args, std::string* rets) {
    using sgd::Job;
    sgd::Progress prog;
    Job job; job.ParseFromString(args);

    if (job.type == Job::kTraining ||
        job.type == Job::kValidation) {
      IterateData(job, &prog);
    } else if (job.type == Job::kEvaluation) {
      GetUpdater()->Evaluate(&prog);
    }
    prog.SerializeToString(rets);
  }

 private:
  void RunEpoch(int epoch, int job_type, sgd::Progress* prog);

  void IterateData(const sgd::Job& job, sgd::Progress* prog);

  real_t EvaluatePenalty(const SArray<real_t>& weight,
                         const SArray<int>& w_pos,
                         const SArray<int>& V_pos);

  void GetPos(const SArray<int>& len, SArray<int>* w_pos, SArray<int>* V_pos);

  /** \brief the model store*/
  Store* store_;
  /** \brief the loss*/
  Loss* loss_;
  /** \brief parameters */
  SGDLearnerParam param_;
  // ProgressPrinter pprinter_;
  int blk_nthreads_ = DEFAULT_NTHREADS;

  std::vector<std::function<void(int epoch, const sgd::Progress& train,
                                 const sgd::Progress& val)>> epoch_end_callback_;
};

}  
#endif  