#include "vfs.h"
#include "filesystem.h"
#include "pipe.h"
#include "string.h"
#include "iso.h"
#include "Kernel/timer.h"
#include "Lib/debug.h"
#include "Process/dispatcher.h"
#include "Process/process.h"
#include "Process/scheduler.h"
#include "Drivers/driver.h"

namespace VFS
{
	DENTRY *root_dentry = 0;

	inline bool IsValidInode(FILE *file)
	{
		return file && file->dentry && file->dentry->inode;
	}

	DENTRY *GetExistingChild(DENTRY *parent, const char *name)
	{
		for (int i = 0; i < parent->children.Count(); i++)
		{
			DENTRY *child = parent->children[i];

			if (strcmp(child->name, name) == 0)
				return child;
		}

		return 0;
	}

	INODE *AllocInode(DENTRY *dentry)
	{
		if (dentry->inode)
			return dentry->inode;

		dentry->inode = new INODE;
		return dentry->inode;
	}

	DENTRY *AllocDentry(DENTRY *parent, const char *name)
	{
		if (parent && name)
		{
			DENTRY *dentry = GetExistingChild(parent, name);

			if (dentry)
				return dentry;
		}

		DENTRY *dentry = new DENTRY;
		dentry->name = strdup(name);
		return dentry;
	}

	void AddDentry(DENTRY *parent, DENTRY *child)
	{
		if (GetExistingChild(parent, child->name))
		{
			debug_print("dentry already added %s\n", child->name);
			return;
		}

		child->parent = parent;
		child->owner = parent->owner;
		parent->children.Add(child);
	}

	int GetChildren(DENTRY *parent)
	{
		return parent->owner->GetChildren(parent, 0);
	}

	DENTRY *GetChild(DENTRY *parent, const char *name)
	{
		DENTRY *dentry = GetExistingChild(parent, name);

		if (dentry)
			return dentry;

		parent->owner->GetChildren(parent, name);
		return GetExistingChild(parent, name);
	}

	DENTRY *GetDentry(Path path)
	{
		DENTRY *current = root_dentry;

		if (!root_dentry)
			return 0;

		for (int i = 0; i < path.count; i++)
		{
			const char *part = path.parts[i];
			current = GetChild(current, part);

			if (!current)
				break;
		}

		return current;
	}

	int Mount(BlockDriver *driver, FileSystem *fs, const char *mount_point)
	{
		Path path = Path(mount_point);
		DENTRY *dentry = AllocDentry(0, path.Filename());
		fs->Mount(driver, dentry);

		if (!dentry->inode)
			return -1;

		DENTRY *parent = 0;

		if (path.count > 0)
			parent = GetDentry(path.Parent());

		if (parent)
			AddDentry(parent, dentry);
		else
			root_dentry = dentry;

		dentry->type = INODE_TYPE_DIRECTORY;
		dentry->owner = fs;

		return 0;
	}

	int DuplicateFile(Filetable &filetable, int oldfd)
	{
		FILE *file = filetable.Get(oldfd);

		if (!file)
			return -1;

		return filetable.Add(file);
	}

	int DuplicateFile(Filetable &filetable, int oldfd, int newfd)
	{
		FILE *file = filetable.Get(oldfd);

		if (!file)
			return -1;

		//Close previous file
		if (filetable.Get(newfd))
			Close(filetable, newfd);

		return filetable.Set(newfd, file);
	}

	int Open(Filetable &filetable, const char *filename)
	{
		Path path(filename);
		DENTRY *dentry = GetDentry(path);

		if (!dentry)
			return -1;
		
		FILE *file = new FILE(dentry);
		int fd = filetable.Add(file);

		if (fd < 0)
			return fd;
		
		dentry->owner->Open(file);
		return fd;
	}

	int Close(Filetable &filetable, int fd)
	{
		int ret = 0;
		FILE *file = filetable.Get(fd);

		if (!file)
			return -1;

		//if ((ret = file->node->io->Close(file)))
		//	return ret;

		debug_print("Close file %d\n", fd);

		filetable.Remove(fd);
		delete file;
		return ret;
	}

	size_t Read(Filetable &filetable, int fd, char *dst, size_t size)
	{
		FILE *file = filetable.Get(fd);

		if (!IsValidInode(file))
			return 0;

		FileIO *owner = file->dentry->owner;
		int read = owner->Read(file, dst, size);

		if (read < 0)
			return read;

		file->pos += read;
		return read;
	}

	size_t Write(Filetable &filetable, int fd, const char *buf, size_t size)
	{
		FILE *file = filetable.Get(fd);

		if (!IsValidInode(file))
			return 0;

		FileIO *owner = file->dentry->owner;
		int written = owner->Write(file, buf, size);

		if (written < 0)
			return written;

		file->pos += written;
		return written;
	}

	off_t Seek(Filetable &filetable, int fd, off_t offset, int whence)
	{
		FILE *file = filetable.Get(fd);

		if (!IsValidInode(file))
			return -1;

		INODE *inode = file->dentry->inode;

		switch (whence)
		{
		case SEEK_SET:
			file->pos = offset;
			return 0;

		case SEEK_CUR:
			file->pos += offset;
			return 0;

		case SEEK_END:
			file->pos = inode->size + offset;
			return 0;

		default:
			return -1;
		}
	}

	int CreatePipes(Filetable &filetable, int pipefd[2], int flags)
	{
		Pipe *pipe = new Pipe();

		char namebuf[32];
		itoa(Timer::Ticks(), namebuf, 10);

		// TODO
		DENTRY *pipes_dentry = AllocDentry(root_dentry, "pipes");
		INODE *pipes_inode = AllocInode(pipes_dentry);
		AddDentry(root_dentry, pipes_dentry);

		DENTRY *dentry = AllocDentry(pipes_dentry, namebuf);
		INODE *inode = AllocInode(dentry);
		dentry->type = INODE_TYPE_FIFO;
		inode->type = INODE_TYPE_FIFO;
		AddDentry(pipes_dentry, dentry);

		FILE *read = new FILE(dentry);
		FILE *write = new FILE(dentry);

		pipefd[0] = filetable.Add(read);
		pipefd[1] = filetable.Add(write);

		if (pipefd[0] == -1 || pipefd[1] == -1)
			return -1;

		return 0;
	}

	uint32 ReadFile(const char *filename, char *&buffer)
	{
		Filetable filetable;
		int fd = Open(filetable, filename);
		FILE *file = filetable.Get(fd);

		if (!file)
			return 0;

		DENTRY *dentry = file->dentry;
		int size = dentry->inode->size;
		buffer = new char[size];
		int length = dentry->owner->Read(file, buffer, size);

		return length;
	}

	STATUS Init()
	{
		root_dentry = 0;
		return STATUS_SUCCESS;
	}
} // namespace VFS
