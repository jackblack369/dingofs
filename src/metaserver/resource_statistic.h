/*
 *  Copyright (c) 2022 NetEase Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Project: dingo
 * Date: Saturday Jun 11 14:28:18 CST 2022
 * Author: wuhanqing
 */

#ifndef DINGOFS_SRC_METASERVER_RESOURCE_STATISTIC_H_
#define DINGOFS_SRC_METASERVER_RESOURCE_STATISTIC_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace dingofs {
namespace metaserver {

struct StorageStatistics {
  uint64_t maxMemoryQuotaBytes;
  uint64_t maxDiskQuotaBytes;
  uint64_t memoryUsageBytes;
  uint64_t diskUsageBytes;
};

class ResourceCollector {
 public:
  ResourceCollector(uint64_t diskQuota, uint64_t memQuota,
                    std::string dataRoot);

  void SetDataRoot(const std::string& dataRoot);

  void SetDiskQuota(uint64_t diskQuotaBytes);

  void SetMemoryQuota(uint64_t memQuotaBytes);

  bool GetResourceStatistic(StorageStatistics* statistics);

 private:
  uint64_t diskQuotaBytes_;
  uint64_t memQuotaBytes_;
  std::string dataRoot_;
};

}  // namespace metaserver
}  // namespace dingofs

#endif  // DINGOFS_SRC_METASERVER_RESOURCE_STATISTIC_H_
