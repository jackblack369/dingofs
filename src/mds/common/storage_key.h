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
 * Created Date: Thu Jul 22 10:45:43 CST 2021
 * Author: wuhanqing
 */

#ifndef DINGOFS_SRC_MDS_COMMON_STORAGE_KEY_H_
#define DINGOFS_SRC_MDS_COMMON_STORAGE_KEY_H_

#include <cstdint>

namespace dingofs {
namespace mds {

// All prefix keys here are used to concat with corresponding type of key
// The concated-key is used as the actual key stored in K/V storage(e.g., etcd)
// And all prefix keys must be unique
// NOTE: all prefix keys are start with `fs_`, this's to distingush it from the
//       previous volume prefix key

const char FS_NAME_KEY_PREFIX[] = "fs_01";
const char FS_NAME_KEY_END[] = "fs_02";
const char FS_ID_KEY_PREFIX[] = "fs_02";
const char CHUNKID_NAME_KEY_PREFIX[] = "fs_03";
const char CHUNKID_NAME_KEY_END[] = "fs_04";
const char BLOCKGROUP_KEY_PREFIX[] = "fs_04";
const char BLOCKGROUP_KEY_END[] = "fs_05";
constexpr uint32_t COMMON_PREFIX_LENGTH = 5;

const char POOLKEYPREFIX[] = "fs_1001";
const char POOLKEYEND[] = "fs_1002";
const char ZONEKEYPREFIX[] = "fs_1002";
const char ZONEKEYEND[] = "fs_1003";
const char SERVERKEYPREFIX[] = "fs_1003";
const char SERVERKEYEND[] = "fs_1004";
const char METASERVERKEYPREFIX[] = "fs_1004";
const char METASERVERKEYEND[] = "fs_1005";
const char CLUSTERINFOKEY[] = "fs_1006";
const char COPYSETKEYPREFIX[] = "fs_1007";
const char COPYSETKEYEND[] = "fs_1008";
const char PARTITIONKEYPREFIX[] = "fs_1008";
const char PARTITIONKEYEND[] = "fs_1009";
const char MEMCACHECLUSTERKEYPREFIX[] = "fs_1009";
const char MEMCACHECLUSTERKEYEND[] = "fs_1010";
const char FS2MEMCACHECLUSTERKEYPREFIX[] = "fs_1010";
const char FS2MEMCACHECLUSTERKEYEND[] = "fs_1011";
constexpr uint32_t TOPOLOGY_PREFIX_LENGTH = 7;

const char DLOCK_KEY_PREFIX[] = "dlock_01";
constexpr uint32_t DLOCK_PREFIX_LENGTH = 8;

// member id
const char CACHE_GROUP_MEMBER_ID_KEY_PREFIX[] = "fs_10001";
// group id
const char CACHE_GROUP_GROUP_ID_KEY_PREFIX[] = "fs_10002";
// group name
const char CACHE_GROUP_GROUP_NAME_KEY_PREFIX[] = "fs_10003";
const char CACHE_GROUP_GROUP_NAME_KEY_END[] = "fs_10004";
// group member
const char CACHE_GROUP_GROUP_MEMBER_KEY_PREFIX[] = "fs_10004";
const char CACHE_GROUP_GROUP_MEMBER_KEY_END[] = "fs_10005";
constexpr uint32_t CACHE_GROUP_PREFIX_LENGTH = 8;

}  // namespace mds
}  // namespace dingofs

#endif  // DINGOFS_SRC_MDS_COMMON_STORAGE_KEY_H_
