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

file(GLOB UTILS_TEST_SRCS
    "*.cpp"
)

add_executable(test_uitls ${UTILS_TEST_SRCS})
target_link_libraries(test_uitls
    dingofs_utils
    dingofs_auth
    block_accesser
    protobuf::libprotobuf
    ${TEST_DEPS}
)
set_target_properties(test_uitls PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_EXECUTABLE_OUTPUT_PATH}
)