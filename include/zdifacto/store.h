#ifndef ZDIFACTO_STORE_H_
#define ZDIFACTO_STORE_H_
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "dmlc/io.h"
#include "dmlc/parameter.h"

#include "zdifacto/base.h"
#include "zdifacto/sarray.h"
#include "zdifacto/updater.h"

namespace zdifacto {

class Store {
 public:
 
  static Store* Create();
  Store() { }
  virtual ~Store() { }

  static const int kFeaCount = 1;
  static const int kWeight = 2;
  static const int kGradient = 3;
 
  virtual KWArgs Init(const KWArgs& kwargs) = 0;

  virtual int Push(const SArray<feaid_t>& fea_ids,
                   int val_type,
                   const SArray<real_t>& vals,
                   const SArray<int>& lens,
                   const std::function<void()>& on_complete = nullptr) = 0;
  
  virtual int Pull(const SArray<feaid_t>& fea_ids,
                   int val_type,
                   SArray<real_t>* vals,
                   SArray<int>* lens,
                   const std::function<void()>& on_complete = nullptr) = 0;

  
  virtual void Wait(int time) = 0;

  
  virtual int NumWorkers() = 0;
  
  virtual int NumServers() = 0;
  
  virtual int Rank() = 0;

  void SetUpdater(const std::shared_ptr<Updater>& updater) {
    updater_ = updater;
  }
  std::shared_ptr<Updater> updater() { return updater_; }

 protected:
  std::shared_ptr<Updater> updater_;
};

} 

#endif  // DIFACTO_STORE_H_