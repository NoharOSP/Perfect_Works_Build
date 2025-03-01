#include "pch.h"
#include "archiveIndex.h"

archiveIndex::archiveIndex() {
}

archiveIndex::~archiveIndex()
{
	delete[] m_List;
}

int archiveIndex::ReadIndex(unsigned char* index, long size)
{
	int i = 0;
	m_ListLength = size / 7;
	m_List = new entry[size / 7];

	while (i < m_ListLength) {
		unsigned long tSector = 0;
		unsigned long tSize = 0;

		memcpy(&tSector, index + i * 7, 3);
		memcpy(&tSize, index + i * 7 + 3, 4);
		memset(&(m_List[i]), 0, sizeof(entry));
		m_List[i].sector = tSector;
		m_List[i].size = tSize;
		m_List[i].type = DetectType(tSector, tSize);
		m_List[i].change = 0;
		strcpy(m_List[i].path, "");
		i++;
	}

	if (m_List[i - 1].sector != 0xFFFFFF) {
		return 0;
	}

	return 1;
}


int archiveIndex::WriteIndex(unsigned char* buffer, unsigned long* size)
{
	int i = 0;
	unsigned long tSector;
	unsigned long tSize;

	*size = m_ListLength * 7;

	while (i < m_ListLength - 1) {
		tSector = m_List[i].sector;
		tSize = m_List[i].size;

		memcpy(buffer + i * 7, &tSector, 3);
		memcpy(buffer + i * 7 + 3, &tSize, 4);
		i++;
	}
	tSector = 0xFFFFFF;
	memcpy(buffer + i * 7, &tSector, 3);

	return 1;
}

int archiveIndex::DetectType(unsigned long sector, unsigned long size) {
	if ((sector != 0) && (size < 0xFF000000) && (size > 0)) {
		return TYPE_FILE;
	}
	else if ((sector == 0) && (size == 0)) {
		return TYPE_ZEROS;
	}
	else {
		return TYPE_OTHER;
	}
}

int archiveIndex::UpdateIndex()
{
	int i = 0;
	unsigned long pSector = m_List[nFileToSkip].sector;
	for (i = nFileToSkip; i < m_ListLength; i++) {
		switch (m_List[i].type)
		{
		case TYPE_FILE:
			m_List[i].sector = pSector;
			pSector += SizeToSectors(m_List[i].size, 2048);
			break;
		case TYPE_ZEROS:
			break;
		case TYPE_OTHER:
			m_List[i].sector = pSector;
			break;
		default:
			return 0;
		}
	}
	return 1;
}

int archiveIndex::getIndexLength() {
	return m_ListLength;
}

unsigned long archiveIndex::getSector(int nfile)
{
	if (nfile < m_ListLength) {
		return m_List[nfile].sector;
	}
	else {
		return -1;
	}
}

unsigned long archiveIndex::getSize(int nfile) {
	if (nfile < m_ListLength) {
		return m_List[nfile].size;
	}
	else {
		return -1;
	}
}

int archiveIndex::getType(int nfile) {
	if (nfile < m_ListLength) {
		return m_List[nfile].type;
	}
	else {
		return -1;
	}
}

int archiveIndex::getChangedFlag(int nfile) {
	if (nfile < m_ListLength) {
		return m_List[nfile].change;
	}
	else {
		return -1;
	}
}

char* archiveIndex::getNewPath(int nfile, char* path)
{
	if (nfile < m_ListLength) {
		strcpy(path, m_List[nfile].path);
		return path;
	}
	else {
		return NULL;
	}
}

int archiveIndex::setNewPath(int nfile, char* path)
{
	FILE* file;
	if (nfile >= m_ListLength) {
		return 0;
	}
	file = fopen(path, "rb");
	if (file != NULL) {
		unsigned long tSize = 0;
		strcpy(m_List[nfile].path, path);
		m_List[nfile].change = 1;
		fseek(file, 0, SEEK_END);
		tSize = ftell(file);
		m_List[nfile].size = tSize;
		fclose(file);
		UpdateIndex();
		return 1;
	}
	return 0;
}

unsigned long archiveIndex::SizeToSectors(unsigned long size, int secsize)
{
	unsigned long temp;
	temp = size / secsize;
	if (size % secsize != 0) {
		temp++;
	}
	return temp;
}

int archiveIndex::SetFileToSkip(int num)
{
	nFileToSkip = num;
	return num;
}
