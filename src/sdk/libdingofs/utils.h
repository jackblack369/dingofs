/*
 *  Copyright (c) 2023 NetEase Inc.
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
 * Project: DingoFS
 * Created Date: 2023-07-07
 * Author: Jingli Chen (Wine93)
 */

#ifndef DINGOFS_SDK_LIBDINGOFS_UTILS_H_
#define DINGOFS_SDK_LIBDINGOFS_UTILS_H_

#include <string>
#include <vector>

#include "absl/strings/string_view.h"

namespace dingofs {
namespace sdk {
namespace utils {

namespace strings {

std::string TrimSpace(const std::string& str);

bool HasPrefix(const std::string& str, const std::string& prefix);

std::vector<std::string> Split(const std::string& str, const std::string& sep);

std::string Join(const std::vector<std::string>& range,
                 absl::string_view delim);

std::string Join(const std::vector<std::string>& strs,
                 uint32_t start,
                 uint32_t end,
                 absl::string_view delim);

};  // namespace strings

namespace filepath {

std::string ParentDir(const std::string& path);

std::string Filename(const std::string& path);

std::vector<std::string> Split(const std::string& path);

};  // namespace filepath

}  // namespace utils
}  // namespace sdk
}  // namespace dingofs

#endif  // DINGOFS_SDK_LIBDINGOFS_UTILS_H_
