#pragma once
#include <iostream>
#include <filesystem>

class archiveIndex
{
public:
#define	TYPE_ZEROS	2
#define	TYPE_OTHER	1
#define	TYPE_FILE	0
private:
	int nFileToSkip;
	int DetectType(unsigned long sector, unsigned long size);
	unsigned long SizeToSectors(unsigned long size, int secsize);
	typedef struct entry {
		unsigned long	sector;
		unsigned long	size;
		int				type;
		int				change;
		char			path[300];
	};

public:
	int SetFileToSkip(int num);
	int UpdateIndex();
	int setNewPath(int nfile, char* path);
	char* getNewPath(int nfile, char* path);
	int getChangedFlag(int nfile);
	int WriteIndex(unsigned char* buffer, unsigned long* size);
	int getType(int nfile);
	unsigned long getSize(int nfile);
	unsigned long getSector(int nfile);
	int getIndexLength();
	int ReadIndex(unsigned char* index, long size);
	archiveIndex(void);
	~archiveIndex(void);

protected:
	entry* m_List;
	int m_ListLength;
};

