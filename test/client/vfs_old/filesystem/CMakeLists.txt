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

file(GLOB DINGOFS_CLIENT_FILESYSTEM_TEST_SRCS
    "*.cpp"
    "helper/*.cpp"
)

add_executable(test_dingofs_client_filesystem 
    ${DINGOFS_CLIENT_FILESYSTEM_TEST_SRCS} 
)
target_link_libraries(test_dingofs_client_filesystem
    fuse_client_lib
    ${TEST_DEPS}
)
set_target_properties(test_dingofs_client_filesystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_EXECUTABLE_OUTPUT_PATH}
)
