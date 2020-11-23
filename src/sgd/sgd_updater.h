#ifndef ZDIFACTO_SGD_SGD_UPDATER_H_
#define ZDIFACTO_SGD_SGD_UPDATER_H_
#include <vector>
#include <mutex>
#include <limits>

#include "zdifacto/updater.h"
#include "sgd_utils.h"

#include "dmlc/io.h"
#include "sgd_param.h"

namespace zdifacto {

struct SGDEntry {
 public:
  SGDEntry() { }
  ~SGDEntry() { delete [] V; }
  real_t fea_cnt = 0;

  real_t w = 0, sqrt_g = 0, z = 0;

  real_t *V = nullptr;
  bool Empty() const { return (w == 0 && fea_cnt == 0); }
};

class SGDUpdater : public Updater {
 public:
  SGDUpdater() {}
  virtual ~SGDUpdater() {}

  KWArgs Init(const KWArgs& kwargs) override;

  void Load(dmlc::Stream* fi, bool has_aux) override ;
  
  void Save(bool save_aux, dmlc::Stream *fo) const override ;
  
  void Get(const SArray<feaid_t>& fea_ids,
           int value_type,
           SArray<real_t>* weights,
           SArray<int>* val_lens) override;

  void Update(const SArray<feaid_t>& fea_ids,
              int value_type,
              const SArray<real_t>& values,
              const SArray<int>& val_lens) override;

  void Evaluate(sgd::Progress* prog) const;

  const SGDUpdaterParam& param() const { return param_; }

 private:
  /** \brief update w by FTRL */
  void UpdateW(real_t gw, SGDEntry* e);

  /** \brief update V by adagrad */
  void UpdateV(real_t const* gV, SGDEntry* e);

  /** \brief init V */
  void InitV(SGDEntry* e);

  SGDUpdaterParam param_;
  std::unordered_map<feaid_t, SGDEntry> model_;
  mutable std::mutex mu_;
  bool has_aux_ = true;
};

}  
#endif  // DIFACTO_SGD_SGD_UPDATER_H_
