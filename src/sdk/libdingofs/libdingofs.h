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
 * Created Date: 2023-07-12
 * Author: Jingli Chen (Wine93)
 */

#ifndef DINGOFS_SDK_LIBDINGOFS_LIBDINGOFS_H_
#define DINGOFS_SDK_LIBDINGOFS_LIBDINGOFS_H_

#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __cplusplus

#include <string>
#include <sstream>
#include <memory>

#include "src/sdk/libdingofs/config.h"
#include "src/client/vfs.h"

using ::dingofs::sdk::config::Configure;
using ::dingofs::client::vfs::VFS;

typedef struct {
    std::shared_ptr<Configure> cfg;
    std::shared_ptr<VFS> vfs;
} dingofs_mount_t;

#endif  // __cplusplus

typedef struct {
    uint64_t fd;
    uint64_t length;
} file_t;

typedef struct {
    char name[256];  // TODO(Wine93): smaller buffer
    struct stat stat;  // sizeof(stat) = 144
} dirent_t;

#ifdef __cplusplus
extern "C" {
#endif

uintptr_t dingofs_new();

void dingofs_delete(uintptr_t instance_ptr);

// NOTE: instance_ptr is the pointer of dingofs_mount_t instance.
void dingofs_conf_set(uintptr_t instance_ptr,
                      const char* key,
                      const char* value);

int dingofs_mount(uintptr_t instance_ptr,
                  const char* fsname,
                  const char* mountpoint);

int dingofs_umonut(uintptr_t instance_ptr,
                   const char* fsname,
                   const char* mountpoint);

// directory
int dingofs_mkdir(uintptr_t instance_ptr, const char* path, uint16_t mode);

int dingofs_mkdirs(uintptr_t instance_ptr, const char* path, uint16_t mode);

int dingofs_rmdir(uintptr_t instance_ptr, const char* path);

int dingofs_opendir(uintptr_t instance_ptr,
                    const char* path,
                    uint64_t* fd);

ssize_t dingofs_readdir(uintptr_t instance_ptr,
                        uint64_t fd,
                        dirent_t dirents[],
                        size_t count);

int dingofs_closedir(uintptr_t instance_ptr, uint64_t fd);

// file
int dingofs_create(uintptr_t instance_ptr,
                   const char* path,
                   uint16_t mode,
                   file_t* file);

int dingofs_open(uintptr_t instance_ptr,
                 const char* path,
                 uint32_t flags,
                 file_t* file);

int dingofs_lseek(uintptr_t instance_ptr,
                  int fd,
                  uint64_t offset,
                  int whence);

ssize_t dingofs_read(uintptr_t instance_ptr,
                     int fd,
                     char* buffer,
                     size_t count);

ssize_t dingofs_write(uintptr_t instance_ptr,
                      int fd,
                      char* buffer,
                      size_t count);

int dingofs_fsync(uintptr_t instance_ptr, int fd);

int dingofs_close(uintptr_t instance_ptr, int fd);

int dingofs_unlink(uintptr_t instance_ptr, const char* path);

// others
int dingofs_statfs(uintptr_t instance_ptr, struct statvfs* statvfs);

int dingofs_lstat(uintptr_t instance_ptr, const char* path, struct stat* stat);

int dingofs_fstat(uintptr_t instance_ptr, int fd, struct stat* stat);

int dingofs_setattr(uintptr_t instance_ptr,
                    const char* path,
                    struct stat* stat,
                    int to_set);

int dingofs_chmod(uintptr_t instance_ptr, const char* path, uint16_t mode);

int dingofs_chown(uintptr_t instance_ptr,
                  const char* path,
                  uint32_t uid,
                  uint32_t gid);

int dingofs_remove(uintptr_t instance_ptr, const char* path);

int dingofs_removeall(uintptr_t instance_ptr, const char* path);

int dingofs_rename(uintptr_t instance_ptr,
                   const char* oldpath,
                   const char* newpath);
#ifdef __cplusplus
}
#endif

#endif  // CURVEFS_SDK_LIBCURVEFS_LIBCURVEFS_H_
