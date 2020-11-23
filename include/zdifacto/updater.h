/*!
 * Copyright (c) 2015 by Contributors
 */

#ifndef ZDIFACTER_UPDATER_H_
#define ZDIFACTER_UPDATER_H_

#include "zdifacto/base.h"
#include "zdifacto/sarray.h"
#include "dmlc/io.h"


namespace zdifacto {

class Updater {
 public:
  Updater() { }

  virtual ~Updater() { }

  virtual KWArgs Init(const KWArgs& kwargs) = 0;

  virtual void Load(dmlc::Stream* fi, bool has_aux) = 0;

  virtual void Save(bool save_aux, dmlc::Stream *fo) const = 0;

  virtual void Get(const SArray<feaid_t>& fea_ids,
                   int data_type,
                   SArray<real_t>* data,
                   SArray<int>* data_offset) = 0;

  virtual void Update(const SArray<feaid_t>& fea_ids,
                      int data_type,
                      const SArray<real_t>& data,
                      const SArray<int>& data_offset) = 0;
};

}  
#endif  
