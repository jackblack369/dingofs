/*
 * Copyright (c) 2021 NetEase Inc.
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

/**
 * Project: Dingofs
 * Created Date: 2022-03-02
 * Author: Jingli Chen (Wine93)
 */

#include <gtest/gtest.h>

#include "common/process.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);

  ::dingofs::common::Process::InitSetProcTitle(argc, argv);

  return RUN_ALL_TESTS();
}
