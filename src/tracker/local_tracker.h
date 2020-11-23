/**
 * Copyright (c) 2015 by Contributors
 */
#ifndef ZDIFACTO_TRACKER_LOCAL_TRACKER_H_
#define ZDIFACTO_TRACKER_LOCAL_TRACKER_H_
#include <vector>
#include <utility>
#include <string>
#include "zdifacto/tracker.h"
#include "async_local_tracker.h"

namespace zdifacto {
/**
 * \brief an implementation of the tracker which only runs within a local
 * process
 */
class LocalTracker : public Tracker {
 public:
  typedef std::pair<int, std::string> Job;

  LocalTracker() {
    tracker_ = new AsyncLocalTracker<Job, Job>();
  }
  virtual ~LocalTracker() { delete tracker_; }

  KWArgs Init(const KWArgs& kwargs) override { return kwargs; }


  void Issue(const std::vector<Job>& jobs) override {
    if (!tracker_) tracker_ = new AsyncLocalTracker<Job, Job>();
    tracker_->Issue(jobs);
  }

  int NumRemains() override {
    return CHECK_NOTNULL(tracker_)->NumRemains();
  }

  void Clear() override {
    CHECK_NOTNULL(tracker_)->Clear();
  }

  void Stop() override {
    if (tracker_) {
      delete tracker_;
      tracker_ = nullptr;
    }
  }

  void Wait() override {
    CHECK_NOTNULL(tracker_)->Wait();
  }

  void SetMonitor(const Monitor& monitor) override {
    CHECK_NOTNULL(tracker_)->SetMonitor(
        [monitor](const Job& rets) {
          if (monitor) monitor(rets.first, rets.second);
        });
  }

  void SetExecutor(const Executor& executor) override {
    CHECK_NOTNULL(executor);
    CHECK_NOTNULL(tracker_)->SetExecutor(
        [executor](const Job& args,
                   const std::function<void()>& on_complete,
                   Job* rets) {
          rets->first = args.first;
          executor(args.second, &(rets->second));
          on_complete();
        });
  }

 private:
  AsyncLocalTracker<Job, Job>* tracker_ = nullptr;
};

}  
#endif  