# Copyright (c) 2024 dingodb.com, Inc. All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


file(GLOB DINGOFS_MDSV2_COORDINATOR_SRCS
    "*.cc"
)

add_library(mdsv2_coordinator
    ${DINGOFS_MDSV2_COORDINATOR_SRCS}
)

target_link_libraries(mdsv2_coordinator
    PROTO_OBJS
    dingo::sdk
    protobuf::libprotobuf
)