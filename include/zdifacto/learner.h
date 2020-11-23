#ifndef ZDIFACTO_LEARNER_H_
#define ZDIFACTO_LEARNER_H_
#include <string.h>
#include <string>
#include <functional>
#include <vector>

#include "dmlc/io.h"
#include "zdifacto/base.h"
#include "zdifacto/tracker.h"

namespace zdifacto {

class Learner {
 public:
  
  static Learner* Create(const std::string& type);
  Learner() { }
  virtual ~Learner() { }

  virtual KWArgs Init(const KWArgs& kwargs);
   
  

  void Run() {
    if (!IsDistributed() || !strcmp(getenv("DMLC_ROLE"), "scheduler")) {
      RunScheduler();
    } else {
      tracker_->Wait();
    }
  }
  
  void Stop() {
    tracker_->Stop();
  }

 protected:

  virtual void RunScheduler() = 0;

  virtual void Process(const std::string& args, std::string* rets) = 0;

  Tracker* tracker_;
};

}  // namespace difacto
#endif  // DIFACTO_LEARNER_H_