#ifndef ZDIFACTO_TRACKER_ASYNC_LOCAL_TRACKER_H_
#define ZDIFACTO_TRACKER_ASYNC_LOCAL_TRACKER_H_
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace zdifacto {

template<typename JobArgs, typename JobRets = std::string>
class AsyncLocalTracker {
 public:
  AsyncLocalTracker() {
    thread_ = new std::thread(&AsyncLocalTracker::RunExecutor, this);
  }
  ~AsyncLocalTracker() {
    Wait();
    done_ = true;
    run_cond_.notify_one();
    thread_->join();
    delete thread_;
  }


  void Issue(const std::vector<JobArgs>& jobs) {
    CHECK(executor_) << "set executor first";
    {
      std::lock_guard<std::mutex> lk(mu_);
      for (const auto& w : jobs) pending_.push(w);
    }
    run_cond_.notify_all();
  }

  
  void Wait(size_t num_remains = 0) {
    std::unique_lock<std::mutex> lk(mu_);
    fin_cond_.wait(lk, [this, num_remains] {
        return pending_.size() + running_.size() <= num_remains;
      });
  }
  /**
   * \brief clear all jobs that have not been assigned yet
   */
  void Clear() {
    std::lock_guard<std::mutex> lk(mu_);
    while (pending_.size()) pending_.pop();
  }
  /**
   * \brief return the number of unfinished job
   */
  int NumRemains() {
    std::lock_guard<std::mutex> lk(mu_);
    return pending_.size() + running_.size();
  }

  typedef std::function<void()> Callback;

  typedef std::function<void(
      const JobArgs args, const Callback& on_complete, JobRets* rets)> Executor;

  void SetExecutor(const Executor& executor) {
    executor_ = executor;
  }

 
  typedef std::function<void(const JobRets& rets)> Monitor;

  void SetMonitor(const Monitor& monitor) {
    monitor_ = monitor;
  }

 private:
  void RunExecutor() {
    while (true) {
      // get a job from the queue
      std::unique_lock<std::mutex> lk(mu_);
      run_cond_.wait(lk, [this] { return (done_ || pending_.size() > 0); });
      if (done_) break;
      auto it = running_.insert(std::make_pair(
          cur_id_++, std::make_pair(std::move(pending_.front()), JobRets())));
      pending_.pop();
      lk.unlock();

      // run the job
      CHECK(executor_);
      int id = it.first->first;
      auto on_complete = [this, id]() { Remove(id); };
      executor_(it.first->second.first, on_complete, &(it.first->second.second));
    }
  }

  inline void Remove(int id) {
    {
      std::lock_guard<std::mutex> lk(mu_);
      auto it = running_.find(id);
      CHECK(it != running_.end());
      if (monitor_) monitor_(it->second.second);
      running_.erase(it);
    }
    fin_cond_.notify_one();
  }

  bool done_ = false;
  int cur_id_ = 0;
  std::mutex mu_;
  std::condition_variable run_cond_, fin_cond_;
  std::thread* thread_;
  Executor executor_;
  Monitor monitor_;
  std::queue<JobArgs> pending_;
  std::unordered_map<int, std::pair<JobArgs, JobRets>> running_;
};
}  
#endif 