/*
 *  Copyright (c) 2021 NetEase Inc.
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
 * Created Date: Mon Sept 5 2021
 * Author: lixiaocui
 */

#ifndef DINGOFS_TEST_CLIENT_RPCCLIENT_MOCK_METACACHE_H_
#define DINGOFS_TEST_CLIENT_RPCCLIENT_MOCK_METACACHE_H_

#include <gmock/gmock.h>

#include <string>
#include <vector>

#include "stub/common/common.h"
#include "stub/common/metacache_struct.h"
#include "stub/rpcclient/metacache.h"

namespace dingofs {
namespace stub {
namespace rpcclient {

using common::CopysetInfo;
using common::MetaserverID;
using common::PartitionID;
using pb::mds::topology::PartitionTxId;

class MockMetaCache : public MetaCache {
 public:
  MockMetaCache() : MetaCache() {}
  ~MockMetaCache() = default;
  MOCK_METHOD5(GetTarget,
               bool(uint32_t fsID, uint64_t inodeID, CopysetTarget* target,
                    uint64_t* applyIndex, bool refresh));

  MOCK_METHOD3(SelectTarget, bool(uint32_t fsID, CopysetTarget* target,
                                  uint64_t* applyIndex));

  MOCK_METHOD1(GetAllTxIds, void(std::vector<PartitionTxId>* txIds));

  MOCK_METHOD2(SetTxId, void(uint32_t partitionId, uint64_t txId));

  MOCK_METHOD2(UpdateApplyIndex,
               void(const CopysetGroupID& groupID, uint64_t applyIndex));

  MOCK_METHOD1(GetApplyIndex, uint64_t(const CopysetGroupID& groupID));

  MOCK_METHOD1(IsLeaderMayChange, bool(const CopysetGroupID& groupID));

  MOCK_METHOD2(UpdateCopysetInfo,
               void(const CopysetGroupID& groupID,
                    const CopysetInfo<MetaserverID>& csinfo));
  MOCK_METHOD1(MarkPartitionUnavailable, bool(PartitionID pid));

  MOCK_METHOD3(GetTargetLeader,
               bool(CopysetTarget* target, uint64_t* applyindex, bool refresh));

  MOCK_METHOD3(GetPartitionIdByInodeId,
               bool(uint32_t fsID, uint64_t inodeID, PartitionID* pid));
};

}  // namespace rpcclient
}  // namespace stub
}  // namespace dingofs
#endif  // DINGOFS_TEST_CLIENT_RPCCLIENT_MOCK_METACACHE_H_
