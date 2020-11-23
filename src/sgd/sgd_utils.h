#ifndef ZDIFACTO_SGD_SGD_UTILS_H_
#define ZDIFACTO_SGD_SGD_UTILS_H_
#include <string>
#include <vector>
#include <sstream>

#include "dmlc/memory_io.h"
#include "zdifacto/base.h"

namespace zdifacto {
namespace sgd {

struct Job {
  static const int kLoadModel = 1;
  static const int kSaveModel = 2;
  static const int kTraining = 3;
  static const int kValidation = 4;
  static const int kEvaluation = 5;
  int type;
  int num_parts;
  int part_idx;
  int epoch;
  Job() { }

  void SerializeToString(std::string* str) const {
    *str = std::string(reinterpret_cast<char const*>(this), sizeof(Job));
  }

  void ParseFromString(const std::string& str) {
    CHECK_EQ(str.size(), sizeof(Job));
    memcpy(this, str.data(), sizeof(Job));
  }
};

struct Progress {
  real_t loss = 0;  //
  real_t penalty = 0;  //
  real_t auc = 0;   // auc
  real_t nnz_w = 0;  // |w|_0
  real_t nrows = 0;   // number of examples

  std::string TextString() {
    std::stringstream ss;
    ss << "loss = " << loss << ", AUC = " << auc / nrows;
    return ss.str();
  }

  void SerializeToString(std::string* str) const {
    *str = std::string(reinterpret_cast<char const*>(this), sizeof(Progress));
  }

  void ParseFrom(char const* data, size_t size) {
    if (size == 0) return;
    CHECK_EQ(size, sizeof(Progress));
    memcpy(this, data, sizeof(Progress));
  }

  void Merge(const std::string& str) {
    Progress other;
    other.ParseFrom(str.data(), str.size());
    Merge(other);
  }

  void Merge(const Progress& other) {
    size_t n = sizeof(Progress) / sizeof(real_t);
    auto a = reinterpret_cast<real_t*>(this);
    auto b = reinterpret_cast<real_t const*>(&other);
    for (size_t i = 0; i < n; ++i) a[i] += b[i];
  }
};

} 
}  
#endif 
