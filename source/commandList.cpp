#include "pch.h"
#include "commandList.h"
#include <sstream>
//#include <comdef.h>

commandList::commandList()
{
	list = NULL;
	m_inDir = 0;
	m_nCdType = -1;
	m_bFix = 0;
}

commandList::~commandList()
{
	if (list != NULL) {
		fclose(list);
	}
}

int commandList::setList(std::string filename, int cdnum)
{
	char* charfile;
	charfile = &filename[0];
	list = fopen(charfile, "rt");
	if (list == NULL) {
		return 0;
	}
	if (cdnum == 1) {
		m_nCdType = 1;
	}
	else if (cdnum == 2) {
		m_nCdType = 2;
		m_bFix = 1;
	}
	return m_nCdType;
}

FILE_ENTRY* commandList::getNextFile(FILE_ENTRY* file)
{
	int num = 0;
	char path[100];
	char search[100];
	wchar_t wsearch[100];

	if (feof(list) && !m_inDir) {
		return NULL;
	}

	if (!m_inDir) {
		fscanf(list, "%d,%s", &num, path);
		if (num == -1) {
			strcpy(curpath, path);
			strcpy(search, path);
			strcat(search, "\\*.*");
			mbstowcs(wsearch, search, 100);
			hFind = ::FindFirstFile(wsearch, &fd);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					if (fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
						strcpy(file->path, path);
						strcat(file->path, "\\");
						WCHAR* name = fd.cFileName;
						char* cname = (char*)name;
						strcat(file->path, cname);
						file->num = atoi(cname);
						if ((m_nCdType == 2) && (m_bFix == 1)) {
							(file->num) -= 5;
						}
						if (FindNextFile(hFind, &fd)) {
							m_inDir = TRUE;
						}
						else {
							m_inDir = FALSE;
							FindClose(hFind);
						}
						return file;
					}
				} while (FindNextFile(hFind, &fd));
			}

		}
		else {
			file->num = num;
			if ((m_nCdType == 2) && (m_bFix == 1)) {
				(file->num) -= 5;
			}
			strcpy(file->path, path);
			return file;
		}

	}
	else {
		strcpy(file->path, curpath);
		strcat(file->path, "\\");
		WCHAR* name = fd.cFileName;
		char* cname = (char*)name;
		strcat(file->path, cname);
		file->num = atoi(cname);
		if ((m_nCdType == 2) && (m_bFix == 1)) {
			(file->num) -= 5;
		}
		if (::FindNextFile(hFind, &fd)) {
			m_inDir = TRUE;
		}
		else {
			m_inDir = FALSE;
			::FindClose(hFind);
		}
	}
	return file;

}