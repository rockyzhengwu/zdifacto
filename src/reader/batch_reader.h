#ifndef ZDIFACTO_BATCH_READER_H_
#define ZDIFACTO_BATCH_READER_H_

#include <string>
#include <vector>
#include "zdifacto/base.h"
#include "dmlc/data.h"
#include "reader.h"

namespace zdifacto {

class BatchReader : public Reader {
 public:
  /**
   * \brief create a batch iterator
   *
   * @param uri filename
   * @param format the data format, support libsvm, rec, ...
   * @param part_index the i-th part to read
   * @param num_parts partition the file into serveral parts
   * @param batch_size the batch size.
   * @param shuffle_size if nonzero, then the batch is randomly picked from a buffer with
   * shuffle_buf_size examples
   * @param neg_sampling the probability to pickup a negative sample (label <= 0)
   */
  BatchReader(const std::string& uri,
            const std::string& format,
            unsigned part_index,
            unsigned num_parts,
            unsigned batch_size,
            unsigned shuffle_buf_size = 0,
            float neg_sampling = 1.0);

  virtual ~BatchReader() {
    delete reader_;
    delete buf_reader_;
  }

  bool Next() override;

  const dmlc::RowBlock<feaid_t>& Value() const override {
    return out_blk_;
  }

 private:
  
  void Push(size_t pos, size_t len);

  unsigned batch_size_, shuf_buf_;

  Reader *reader_;
  BatchReader* buf_reader_;

  float neg_sampling_;
  size_t start_, end_;
  dmlc::RowBlock<feaid_t> in_blk_, out_blk_;
  dmlc::data::RowBlockContainer<feaid_t> batch_;

  // random pertubation
  std::vector<unsigned> rdp_;
  unsigned int seed_;
};
}

#endif