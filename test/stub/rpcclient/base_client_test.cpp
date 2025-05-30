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
 * Created Date: Thur Jun 15 2021
 * Author: lixiaocui
 */

#include "stub/rpcclient/base_client.h"

#include <brpc/server.h>
#include <google/protobuf/util/message_differencer.h>
#include <gtest/gtest.h>

#include "dingofs/mds.pb.h"
#include "dingofs/metaserver.pb.h"
#include "stub/rpcclient/mock_mds_service.h"
#include "stub/rpcclient/mock_metaserver_service.h"
#include "stub/rpcclient/mock_topology_service.h"

namespace dingofs {
namespace stub {
namespace rpcclient {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::SetArgPointee;

using pb::mds::FsStatus;
using pb::mds::GetFsInfoRequest;
using pb::mds::GetFsInfoResponse;
using pb::mds::MountFsRequest;
using pb::mds::MountFsResponse;
using pb::mds::Mountpoint;
using pb::mds::UmountFsRequest;
using pb::mds::UmountFsResponse;

template <typename RpcRequestType, typename RpcResponseType,
          bool RpcFailed = false>
void RpcService(google::protobuf::RpcController* cntl_base,
                const RpcRequestType* request, RpcResponseType* response,
                google::protobuf::Closure* done) {
  if (RpcFailed) {
    brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
    cntl->SetFailed(112, "Not connected to");
  }
  done->Run();
}

class BaseClientTest : public testing::Test {
 protected:
  void SetUp() override {
    ASSERT_EQ(0, server_.AddService(&mockMetaServerService_,
                                    brpc::SERVER_DOESNT_OWN_SERVICE));
    ASSERT_EQ(0, server_.AddService(&mockMdsService_,
                                    brpc::SERVER_DOESNT_OWN_SERVICE));
    ASSERT_EQ(0, server_.AddService(&mockTopologyService_,
                                    brpc::SERVER_DOESNT_OWN_SERVICE));
    ASSERT_EQ(0, server_.Start(addr_.c_str(), nullptr));
  }

  void TearDown() override {
    server_.Stop(0);
    server_.Join();
  }

  MockMetaServerService mockMetaServerService_;
  MockMdsService mockMdsService_;
  MockTopologyService mockTopologyService_;
  MDSBaseClient mdsbasecli_;
  // TODO(lixiaocui): add base client for dingo block storage
  // SpaceBaseClient spacebasecli_;

  std::string addr_ = "127.0.0.1:5600";
  brpc::Server server_;
};

TEST_F(BaseClientTest, test_MountFs) {
  std::string fsName = "test1";
  Mountpoint mp;
  mp.set_hostname("0.0.0.0");
  mp.set_port(9000);
  mp.set_path("/data");
  MountFsResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  MountFsResponse response;
  response.set_statuscode(pb::mds::FSStatusCode::OK);
  EXPECT_CALL(mockMdsService_, MountFs(_, _, _, _))
      .WillOnce(DoAll(SetArgPointee<2>(response),
                      Invoke(RpcService<MountFsRequest, MountFsResponse>)));

  mdsbasecli_.MountFs(fsName, mp, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_UmountFs) {
  std::string fsName = "test1";
  Mountpoint mp;
  mp.set_hostname("0.0.0.0");
  mp.set_port(9000);
  mp.set_path("/data");
  UmountFsResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  pb::mds::UmountFsResponse response;
  response.set_statuscode(pb::mds::FSStatusCode::OK);
  EXPECT_CALL(mockMdsService_, UmountFs(_, _, _, _))
      .WillOnce(DoAll(SetArgPointee<2>(response),
                      Invoke(RpcService<UmountFsRequest, UmountFsResponse>)));

  mdsbasecli_.UmountFs(fsName, mp, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_GetFsInfo_by_fsName) {
  std::string fsName = "test1";
  GetFsInfoResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  pb::mds::GetFsInfoResponse response;
  auto* fsinfo = new pb::mds::FsInfo();
  fsinfo->set_fsid(1);
  fsinfo->set_fsname(fsName);
  fsinfo->set_status(FsStatus::NEW);
  fsinfo->set_rootinodeid(1);
  fsinfo->set_capacity(10 * 1024 * 1024L);
  fsinfo->set_block_size(4 * 1024);
  fsinfo->set_chunk_size(16 * 1024);
  fsinfo->set_mountnum(1);
  fsinfo->set_owner("test");
  fsinfo->set_txsequence(0);
  fsinfo->set_txowner("owner");
  auto* storage_info = fsinfo->mutable_storage_info();
  storage_info->set_type(pb::common::StorageType::TYPE_S3);
  auto* s3_info = storage_info->mutable_s3_info();
  s3_info->set_ak("a");
  s3_info->set_sk("b");
  s3_info->set_endpoint("http://127.0.1:9000");
  s3_info->set_bucketname("test");

  response.set_allocated_fsinfo(fsinfo);
  response.set_statuscode(pb::mds::FSStatusCode::OK);
  EXPECT_CALL(mockMdsService_, GetFsInfo(_, _, _, _))
      .WillOnce(DoAll(SetArgPointee<2>(response),
                      Invoke(RpcService<GetFsInfoRequest, GetFsInfoResponse>)));

  mdsbasecli_.GetFsInfo(fsName, &resp, &cntl, &ch);
  LOG(INFO) << " .........." << cntl.ErrorText();
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();

  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_GetFsInfo_by_fsId) {
  uint32_t fsId = 1;
  GetFsInfoResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  pb::mds::GetFsInfoResponse response;
  auto* fsinfo = new pb::mds::FsInfo();
  fsinfo->set_fsid(1);
  fsinfo->set_fsname("test1");
  fsinfo->set_status(FsStatus::NEW);
  fsinfo->set_rootinodeid(1);
  fsinfo->set_capacity(10 * 1024 * 1024L);
  fsinfo->set_block_size(4 * 1024);
  fsinfo->set_chunk_size(16 * 1024);
  fsinfo->set_mountnum(1);
  fsinfo->set_owner("test");
  fsinfo->set_txsequence(0);
  fsinfo->set_txowner("owner");
  fsinfo->set_mountnum(1);

  auto* storage_info = fsinfo->mutable_storage_info();
  storage_info->set_type(pb::common::StorageType::TYPE_S3);
  auto* s3_info = storage_info->mutable_s3_info();
  s3_info->set_ak("a");
  s3_info->set_sk("b");
  s3_info->set_endpoint("http://127.0.1:9000");
  s3_info->set_bucketname("test");

  response.set_allocated_fsinfo(fsinfo);
  response.set_statuscode(pb::mds::FSStatusCode::OK);
  EXPECT_CALL(mockMdsService_, GetFsInfo(_, _, _, _))
      .WillOnce(DoAll(SetArgPointee<2>(response),
                      Invoke(RpcService<GetFsInfoRequest, GetFsInfoResponse>)));

  mdsbasecli_.GetFsInfo(fsId, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_CreatePartition) {
  uint32_t fsID = 1;
  uint32_t count = 2;
  pb::mds::topology::CreatePartitionResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  pb::common::PartitionInfo partitioninfo1;
  pb::common::PartitionInfo partitioninfo2;
  partitioninfo1.set_fsid(fsID);
  partitioninfo1.set_poolid(1);
  partitioninfo1.set_copysetid(2);
  partitioninfo1.set_partitionid(3);
  partitioninfo1.set_start(4);
  partitioninfo1.set_end(5);
  partitioninfo1.set_txid(6);
  partitioninfo1.set_status(pb::common::PartitionStatus::READWRITE);

  partitioninfo2.set_fsid(fsID);
  partitioninfo2.set_poolid(2);
  partitioninfo2.set_copysetid(3);
  partitioninfo2.set_partitionid(4);
  partitioninfo2.set_start(5);
  partitioninfo2.set_end(6);
  partitioninfo2.set_txid(7);
  partitioninfo2.set_status(pb::common::PartitionStatus::READWRITE);

  pb::mds::topology::CreatePartitionResponse response;
  response.add_partitioninfolist()->CopyFrom(partitioninfo1);
  response.add_partitioninfolist()->CopyFrom(partitioninfo2);

  response.set_statuscode(pb::mds::topology::TopoStatusCode::TOPO_OK);
  EXPECT_CALL(mockTopologyService_, CreatePartition(_, _, _, _))
      .WillOnce(DoAll(
          SetArgPointee<2>(response),
          Invoke(RpcService<pb::mds::topology::CreatePartitionRequest,
                            pb::mds::topology::CreatePartitionResponse>)));

  mdsbasecli_.CreatePartition(fsID, count, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_ListPartition) {
  uint32_t fsID = 1;
  pb::mds::topology::ListPartitionResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  pb::common::PartitionInfo partitioninfo1;
  pb::common::PartitionInfo partitioninfo2;
  partitioninfo1.set_fsid(fsID);
  partitioninfo1.set_poolid(1);
  partitioninfo1.set_copysetid(2);
  partitioninfo1.set_partitionid(3);
  partitioninfo1.set_start(4);
  partitioninfo1.set_end(5);
  partitioninfo1.set_txid(6);
  partitioninfo1.set_status(pb::common::PartitionStatus::READWRITE);

  partitioninfo2.set_fsid(fsID);
  partitioninfo2.set_poolid(2);
  partitioninfo2.set_copysetid(3);
  partitioninfo2.set_partitionid(4);
  partitioninfo2.set_start(5);
  partitioninfo2.set_end(6);
  partitioninfo2.set_txid(7);
  partitioninfo2.set_status(pb::common::PartitionStatus::READONLY);

  pb::mds::topology::ListPartitionResponse response;
  response.add_partitioninfolist()->CopyFrom(partitioninfo1);
  response.add_partitioninfolist()->CopyFrom(partitioninfo2);
  response.set_statuscode(pb::mds::topology::TopoStatusCode::TOPO_OK);

  EXPECT_CALL(mockTopologyService_, ListPartition(_, _, _, _))
      .WillOnce(
          DoAll(SetArgPointee<2>(response),
                Invoke(RpcService<pb::mds::topology::ListPartitionRequest,
                                  pb::mds::topology::ListPartitionResponse>)));

  mdsbasecli_.ListPartition(fsID, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_GetCopysetOfPartition) {
  std::vector<uint32_t> partitionIDList{1, 2};
  pb::mds::topology::GetCopysetOfPartitionResponse resp;
  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  pb::mds::topology::Copyset copyset1;
  pb::mds::topology::Copyset copyset2;
  copyset1.set_poolid(1);
  copyset1.set_copysetid(2);
  pb::common::Peer peer1;
  peer1.set_id(3);
  peer1.set_address("addr1");
  copyset1.add_peers()->CopyFrom(peer1);

  copyset2.set_poolid(2);
  copyset2.set_copysetid(3);
  pb::common::Peer peer2;
  peer2.set_id(4);
  peer2.set_address("addr2");
  copyset2.add_peers()->CopyFrom(peer2);

  pb::mds::topology::GetCopysetOfPartitionResponse response;
  auto* copysetMap = response.mutable_copysetmap();
  (*copysetMap)[1] = copyset1;
  (*copysetMap)[2] = copyset2;
  response.set_statuscode(pb::mds::topology::TopoStatusCode::TOPO_OK);
  EXPECT_CALL(mockTopologyService_, GetCopysetOfPartition(_, _, _, _))
      .WillOnce(DoAll(
          SetArgPointee<2>(response),
          Invoke(
              RpcService<pb::mds::topology::GetCopysetOfPartitionRequest,
                         pb::mds::topology::GetCopysetOfPartitionResponse>)));

  mdsbasecli_.GetCopysetOfPartitions(partitionIDList, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_RefreshSession) {
  // prepare in param
  pb::mds::topology::PartitionTxId tmp;
  tmp.set_partitionid(1);
  tmp.set_txid(2);
  std::vector<pb::mds::topology::PartitionTxId> txIds({tmp});
  std::string fsName = "fs1";
  Mountpoint mountpoint;
  mountpoint.set_hostname("127.0.0.1");
  mountpoint.set_port(9000);
  mountpoint.set_path("/mnt");

  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));
  pb::mds::RefreshSessionResponse resp;

  // prepare out param
  pb::mds::RefreshSessionRequest request;
  pb::mds::RefreshSessionResponse response;
  *request.mutable_txids() = {txIds.begin(), txIds.end()};
  request.set_fsname(fsName);
  *request.mutable_mountpoint() = mountpoint;
  response.set_statuscode(pb::mds::FSStatusCode::OK);
  *response.mutable_latesttxidlist() = {txIds.begin(), txIds.end()};

  EXPECT_CALL(mockMdsService_, RefreshSession(_, _, _, _))
      .WillOnce(DoAll(SetArgPointee<2>(response),
                      Invoke(RpcService<pb::mds::RefreshSessionRequest,
                                        pb::mds::RefreshSessionResponse>)));

  mdsbasecli_.RefreshSession(request, &resp, &cntl, &ch);
  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

TEST_F(BaseClientTest, test_AllocOrGetMemcacheCluster) {
  pb::mds::topology::AllocOrGetMemcacheClusterResponse response;
  pb::mds::topology::AllocOrGetMemcacheClusterResponse resp;
  response.set_statuscode(pb::mds::topology::TOPO_OK);
  EXPECT_CALL(mockTopologyService_, AllocOrGetMemcacheCluster(_, _, _, _))
      .WillOnce(
          DoAll(SetArgPointee<2>(response),
                Invoke(RpcService<
                       pb::mds::topology::AllocOrGetMemcacheClusterRequest,
                       pb::mds::topology::AllocOrGetMemcacheClusterResponse>)));

  brpc::Controller cntl;
  cntl.set_timeout_ms(1000);
  brpc::Channel ch;
  ASSERT_EQ(0, ch.Init(addr_.c_str(), nullptr));

  mdsbasecli_.AllocOrGetMemcacheCluster(1, &resp, &cntl, &ch);

  ASSERT_FALSE(cntl.Failed()) << cntl.ErrorText();
  ASSERT_TRUE(
      google::protobuf::util::MessageDifferencer::Equals(resp, response))
      << "resp:\n"
      << resp.ShortDebugString() << "response:\n"
      << response.ShortDebugString();
}

}  // namespace rpcclient
}  // namespace stub
}  // namespace dingofs
