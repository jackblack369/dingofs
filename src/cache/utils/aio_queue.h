/*
 * Copyright (c) 2025 dingodb.com, Inc. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Project: DingoFS
 * Created Date: 2025-03-30
 * Author: Jingli Chen (Wine93)
 */

#ifndef DINGOFS_SRC_CACHE_UTILS_AIO_QUEUE_H_
#define DINGOFS_SRC_CACHE_UTILS_AIO_QUEUE_H_

#include <bthread/condition_variable.h>
#include <bthread/execution_queue.h>
#include <bthread/mutex.h>
#include <glog/logging.h>
#include <google/protobuf/stubs/callback.h>
#include <sys/types.h>

#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>

#include "cache/utils/aio.h"
#include "cache/utils/helper.h"
#include "cache/utils/phase_timer.h"

namespace dingofs {
namespace cache {
namespace utils {

class ThrottleQueue {
 public:
  ThrottleQueue(uint32_t capacity) : capacity_(capacity) {}

  void PushOne() {
    std::unique_lock<bthread::Mutex> lk(mutex_);
    while (size_ == capacity_) {
      cv_.wait(lk);
    }
    size_++;
  }

  void PopOne() {
    std::unique_lock<bthread::Mutex> lk(mutex_);
    CHECK(size_ > 0);
    size_--;
    cv_.notify_one();
  }

 private:
  uint32_t size_{0};
  uint32_t capacity_;
  bthread::Mutex mutex_;
  bthread::ConditionVariable cv_;
};

class AioQueueImpl : public AioQueue {
  struct BthreadArg {
    BthreadArg(AioQueueImpl* queue, Aio* aio) : queue(queue), aio(aio) {}

    AioQueueImpl* queue;
    Aio* aio;
  };

 public:
  explicit AioQueueImpl(const std::shared_ptr<IoRing>& io_ring);

  Status Init(uint32_t iodepth) override;

  Status Shutdown() override;

  void Submit(Aio* aio) override;

 private:
  static void EnterPhase(Aio* aio, Phase phase);
  static void BatchEnterPhase(Aio* aios[], int n, Phase phase);

  void CheckIo(Aio* aio);
  static int PrepareIo(void* meta, bthread::TaskIterator<Aio*>& iter);
  void BatchSubmitIo(Aio* aios[], int n);
  void BackgroundWait();
  void RunClosureInBthread(Aio* aio);
  static void* RunClosure(void* arg);
  static Status GetStatus(Aio* aio);

 private:
  std::atomic<bool> running_;
  std::shared_ptr<IoRing> ioring_;
  bthread::ExecutionQueueId<Aio*> prep_io_queue_id_;
  std::thread bg_thread_;
  std::unique_ptr<ThrottleQueue> queued_;
  static constexpr uint32_t kSubmitBatchSize{16};
};

}  // namespace utils
}  // namespace cache
}  // namespace dingofs

#endif  // DINGOFS_SRC_CACHE_UTILS_AIO_QUEUE_H_
