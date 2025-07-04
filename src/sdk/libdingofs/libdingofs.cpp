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

#include "src/client/vfs_legacy/filesystem/error.h"
#include "src/client/vfs_legacy/filesystem/meta.h"
#include "src/sdk/libdingofs/libdingofs.h"

using ::dingofs::client::filesystem::DINGOFS_ERROR;
using ::dingofs::client::filesystem::SysErr;
using ::dingofs::client::vfs::File;
using ::dingofs::client::vfs::Dirent;

static dingofs_mount_t* get_instance(uintptr_t instance_ptr) {
    return reinterpret_cast<dingofs_mount_t*>(instance_ptr);
}

uintptr_t dingofs_new() {
    auto mount = new dingofs_mount_t();
    mount->cfg = Configure::Default();
    return reinterpret_cast<uintptr_t>(mount);
}

void dingofs_delete(uintptr_t instance_ptr) {
    auto mount = get_instance(instance_ptr);
    delete mount;
    mount = nullptr;
}

void dingofs_conf_set(uintptr_t instance_ptr,
                      const char* key,
                      const char* value) {
    auto mount = get_instance(instance_ptr);
    return mount->cfg->Set(key, value);
}

int dingofs_mount(uintptr_t instance_ptr,
                  const char* fsname,
                  const char* mountpoint) {
    auto mount = get_instance(instance_ptr);
    mount->vfs = std::make_shared<VFS>(mount->cfg);
    auto rc = mount->vfs->Mount(fsname, mountpoint);
    return SysErr(rc);
}

int dingofs_umonut(uintptr_t instance_ptr,
                   const char* fsname,
                   const char* mountpoint) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Umount(fsname, mountpoint);
    return SysErr(rc);
}

int dingofs_mkdir(uintptr_t instance_ptr, const char* path, uint16_t mode) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->MkDir(path, mode);
    return SysErr(rc);
}

int dingofs_mkdirs(uintptr_t instance_ptr, const char* path, uint16_t mode) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->MkDirs(path, mode);
    return SysErr(rc);
}

int dingofs_rmdir(uintptr_t instance_ptr, const char* path) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->RmDir(path);
    return SysErr(rc);
}

int dingofs_opendir(uintptr_t instance_ptr,
                    const char* path,
                    uint64_t* fd) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->OpenDir(path, fd);
    return SysErr(rc);
}

ssize_t dingofs_readdir(uintptr_t instance_ptr,
                        uint64_t fd,
                        dirent_t dirents[],
                        size_t count) {
    size_t nread = 0;
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->ReadDir(
        fd, reinterpret_cast<Dirent*>(dirents), count, &nread);
    if (rc == CURVEFS_ERROR::OK) {
        return nread;
    } else if (rc == CURVEFS_ERROR::END_OF_FILE) {
        return 0;
    }
    return SysErr(rc);
}

int dingofs_closedir(uintptr_t instance_ptr, uint64_t fd) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->CloseDir(fd);
    return SysErr(rc);
}

int dingofs_create(uintptr_t instance_ptr,
                   const char* path,
                   uint16_t mode,
                   file_t* file) {
    CURVEFS_ERROR rc;
    auto mount = get_instance(instance_ptr);
    rc = mount->vfs->Create(path, mode, reinterpret_cast<File*>(file));
    return SysErr(rc);
}

int dingofs_open(uintptr_t instance_ptr,
                 const char* path,
                 uint32_t flags,
                 file_t* file) {
    CURVEFS_ERROR rc;
    auto mount = get_instance(instance_ptr);
    rc = mount->vfs->Open(path, flags, reinterpret_cast<File*>(file));
    return SysErr(rc);
}

int dingofs_lseek(uintptr_t instance_ptr,
                  int fd,
                  uint64_t offset,
                  int whence) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->LSeek(fd, offset, whence);
    return SysErr(rc);
}

ssize_t dingofs_read(uintptr_t instance_ptr,
                     int fd,
                     char* buffer,
                     size_t count) {
    size_t nread = 0;
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Read(fd, buffer, count, &nread);
    if (rc != CURVEFS_ERROR::OK) {
        return SysErr(rc);
    }
    return nread;
}

ssize_t dingofs_write(uintptr_t instance_ptr,
                      int fd,
                      char* buffer,
                      size_t count) {
    size_t nwritten;
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Write(fd, buffer, count, &nwritten);
    if (rc != CURVEFS_ERROR::OK) {
        return SysErr(rc);
    }
    return nwritten;
}

int dingofs_fsync(uintptr_t instance_ptr, int fd) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->FSync(fd);
    return SysErr(rc);
}

int dingofs_close(uintptr_t instance_ptr, int fd) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Close(fd);
    return SysErr(rc);
}

int dingofs_unlink(uintptr_t instance_ptr, const char* path) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Unlink(path);
    return SysErr(rc);
}

int dingofs_statfs(uintptr_t instance_ptr,
                   struct statvfs* statvfs) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->StatFs(statvfs);
    return SysErr(rc);
}

int dingofs_lstat(uintptr_t instance_ptr, const char* path, struct stat* stat) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->LStat(path, stat);
    return SysErr(rc);
}

int dingofs_fstat(uintptr_t instance_ptr, int fd, struct stat* stat) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->FStat(fd, stat);
    return SysErr(rc);
}

int dingofs_setattr(uintptr_t instance_ptr,
                    const char* path,
                    struct stat* stat,
                    int to_set) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->SetAttr(path, stat, to_set);
    return SysErr(rc);
}

int dingofs_chmod(uintptr_t instance_ptr, const char* path, uint16_t mode) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Chmod(path, mode);
    return SysErr(rc);
}

int dingofs_chown(uintptr_t instance_ptr,
                  const char* path,
                  uint32_t uid,
                  uint32_t gid) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Chown(path, uid, gid);
    return SysErr(rc);
}

int dingofs_rename(uintptr_t instance_ptr,
                   const char* oldpath,
                   const char* newpath) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Rename(oldpath, newpath);
    return SysErr(rc);
}

int dingofs_remove(uintptr_t instance_ptr, const char* path) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->Remove(path);
    return SysErr(rc);
}

int dingofs_removeall(uintptr_t instance_ptr, const char* path) {
    auto mount = get_instance(instance_ptr);
    auto rc = mount->vfs->RemoveAll(path);
    return SysErr(rc);
}
