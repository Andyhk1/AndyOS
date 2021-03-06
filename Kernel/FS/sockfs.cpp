#include <FS/sockfs.h>
#include <FS/vfs.h>
#include <Net/socketmanager.h>
#include <debug.h>
#include <errno.h>
#include <libgen.h>
#include <string.h>

int SockFS::Mount(BlockDriver *driver)
{
    VFS::AllocInode(root_dentry, 1, S_IFDIR);
    return -1;
}

int SockFS::Open(FILE *file)
{
    return -1;
}

int SockFS::Close(FILE *file)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Close();
}

int SockFS::Read(FILE *file, void *buf, size_t size)
{
    return Recv(file, buf, size, 0);
}

int SockFS::Write(FILE *file, const void *buf, size_t size)
{
    return Send(file, buf, size, 0);
}

int SockFS::Seek(FILE *file, long offset, int origin)
{
    return -1;
}

int SockFS::GetChildren(DENTRY *parent, const char *find_name)
{
    return -1;
}

int SockFS::Create(int domain, int type, int protocol, DENTRY *&dentry)
{
    Socket *socket = SocketManager::CreateSocket();

    int ret = 0;

    if ((ret = socket->Init(domain, type, protocol))) {
        // TODO: SocketManager::RemoveSocket(socket);
        return ret;
    }

    dentry = CreateSocketDentry(socket->id);
    return 0;
}

int SockFS::Accept(FILE *file, sockaddr *addr, socklen_t addrlen, int flags, DENTRY *&dentry)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    int id = socket->Accept(addr, addrlen, flags);

    if (id < 0)
        return id;

    dentry = CreateSocketDentry(id);
    return 0;
}

int SockFS::Bind(FILE *file, const struct sockaddr *addr, socklen_t addrlen)
{
    Socket *socket = GetSocket(file);
    DENTRY *parent = 0;
    int ret = 0;

    if (!socket)
        return -1;

    sockaddr_un *unix_addr = (sockaddr_un *)addr;
    char *copy = 0;
    const char *filename = 0;
    const char *parentname = 0;

    if (addr->sa_family == AF_UNIX) {
        if (!unix_addr->sun_path || !unix_addr->sun_path[0])
            return -EINVAL;

        copy = strdup(unix_addr->sun_path);
        filename = basename(copy);
        parentname = dirname(copy);

        parent = VFS::GetDentry(parentname);

        if (!parent)
            return -ENOENT;
    }

    if ((ret = socket->Bind(addr, addrlen))) {
        if (copy)
            delete[] copy;

        return ret;
    }

    if (addr->sa_family == AF_UNIX) {
        if (parent) {
            DENTRY *dentry = VFS::AllocDentry(parent, filename);
            dentry->inode = file->dentry->inode; // TODO: Symlink?
            VFS::AddDentry(parent, dentry);
        }
    }

    if (copy)
        delete[] copy;

    return 0;
}

int SockFS::Connect(FILE *file, const sockaddr *addr, socklen_t addrlen)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Connect(addr, addrlen);
}

int SockFS::Listen(FILE *file, int backlog)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Listen(backlog);
}

int SockFS::Recv(FILE *file, void *buf, size_t len, int flags)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Recv(buf, len, flags);
}

int SockFS::Recvfrom(FILE *file, void *buf, size_t len, int flags, sockaddr *src_addr,
                     socklen_t addrlen)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Recvfrom(buf, len, flags, src_addr, addrlen);
}

int SockFS::Send(FILE *file, const void *buf, size_t len, int flags)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Send(buf, len, flags);
}

int SockFS::Sendto(FILE *file, const void *buf, size_t len, int flags,
                   const struct sockaddr *dest_addr, socklen_t addrlen)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Sendto(buf, len, flags, dest_addr, addrlen);
}

int SockFS::Shutdown(FILE *file, int how)
{
    Socket *socket = GetSocket(file);

    if (!socket)
        return -1;

    return socket->Shutdown(how);
}

Socket *SockFS::GetSocket(FILE *file)
{
    int ino = file->dentry->inode->ino;
    return SocketManager::GetSocket(ino);
}

DENTRY *SockFS::CreateSocketDentry(int socket_id)
{
    char namebuf[32];
    sprintf(namebuf, "socket:[%u]", socket_id);

    DENTRY *dentry = VFS::AllocDentry(root_dentry, namebuf);
    VFS::AllocInode(dentry, socket_id, S_IFSOCK);
    VFS::AddDentry(root_dentry, dentry);
    return dentry;
}
