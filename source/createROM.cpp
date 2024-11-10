#include "pch.h"
#include "createROM.h"

createROM::createROM(HWND hWnd, int num, std::string oldPath, std::string newPath, std::string listFile) {
	log = fopen("log.txt", "wt");
	cdnum = num;
	discfile = oldPath;
	destfile = newPath;
	window = hWnd;
	commandList cmd;
	process(cmd, listFile);
}

int createROM::process(commandList cmd, std::string listFile) {
	FILE_ENTRY fe;
	switch (cmd.setList(listFile, cdnum)) {
	case 1:
		glb_VideoFiles = 18;
		glb_FirstNonVideoSector = 108561;
		glb_IndexSector2 = 108607;
		break;
	case 2:
		//glb_VideoFiles = 14;
		glb_VideoFiles = 13;
		glb_FirstNonVideoSector = 172873;
		glb_IndexSector2 = 172919;
		break;
	case 0:
		MessageBox(window, L"Error opening file.", L"Error", MB_ICONERROR);
		return 0;
	case -1:
		MessageBox(window, L"CD type unknown.", L"Error", MB_ICONERROR);
		return 0;
	}
	char* src;
	char* dst;
	src = &discfile[0];
	dst = &destfile[0];
	isoManager s;
	isoManager d;
	if (!s.OpenIso(src)) {
		MessageBox(window, L"Error opening file.", L"Error", MB_ICONERROR);
		return 0;
	}
	d.CreateIso(dst);

	// Reading index
	archiveIndex index;
	archiveIndex index2;
	index.SetFileToSkip(glb_VideoFiles);
	index2.SetFileToSkip(glb_VideoFiles);
	unsigned char indice[32768];
	memset(indice, 0, 32768);
	s.Read(indice, 24, 16);
	unsigned long indexsize = FindEndOfIndex(indice, 32768);
	if (!index.ReadIndex(indice, indexsize)) {
		MessageBox(window, L"Error reading file.", L"Error", MB_ICONERROR);
		return 0;
	}
	if (!index2.ReadIndex(indice, indexsize)) {
		MessageBox(window, L"Error reading file.", L"Error", MB_ICONERROR);
		return 0;
	}
	// Updating index
	int num = 0;
	char path[100];

	while (cmd.getNextFile(&fe) != NULL) {
		fprintf(log, "Replacing %s files in position %d\n", fe.path, fe.num);
		if (!index2.setNewPath(fe.num, fe.path)) {
			std::cout << "Error opening file\r\n" << std::endl;
			return 0;
		}
	}

	std::cout << "Finished\n" << std::endl;

	std::cout << "Copying movies\n" << std::endl;
	int i = 0;
	unsigned char tbuffer[32 * 2352];
	for (i = 0; i < glb_FirstNonVideoSector - 32; i += 32) {
		s.ReadRawSectors(tbuffer, i, 32);
		d.AppendRawSectors(tbuffer, 32);
	}
	s.ReadRawSectors(tbuffer, i, glb_FirstNonVideoSector - i);
	d.AppendRawSectors(tbuffer, glb_FirstNonVideoSector - i);
	std::cout << "Finished\n" << std::endl;

	indexsize = index.getIndexLength();
	for (i = glb_VideoFiles; (unsigned)i < indexsize; i++) {
		std::cout << "Writing file" << std::endl;

		if (index2.getType(i) == TYPE_FILE) {
			if (!index2.getChangedFlag(i)) {
				int length = index.getSize(i);
				int len_sec = SizeToSectors(length, 2048);
				int cc;
				for (cc = 0; cc < len_sec - 32; cc += 32) {
					s.Read(tbuffer, index.getSector(i) + cc, 32);
					d.Append(tbuffer, 32);
				}
				s.Read(tbuffer, index.getSector(i) + cc, len_sec - cc);
				d.Append(tbuffer, len_sec - cc);
				fprintf(log, "Write unmodified file %d\n", i);
			}
			else {
				FILE* newfile;
				index2.getNewPath(i, path);
				fprintf(log, "Replacing file %d with %s\n", i, path);
				newfile = fopen(path, "rb");
				if (newfile == NULL) {
					std::cout << "Error opening file" << std::endl;
					return 0;
				}
				fseek(newfile, 0, SEEK_END);
				int length = ftell(newfile);
				int len_sec = SizeToSectors(length, 2048);
				fseek(newfile, 0, SEEK_SET);

				int cc;
				for (cc = 0; cc < len_sec - 32; cc += 32) {
					fread(tbuffer, 2048 * 32, 1, newfile);
					d.Append(tbuffer, 32);
				}
				fread(tbuffer, length - (cc * 2048), 1, newfile);
				d.Append(tbuffer, (len_sec - cc));
				fclose(newfile);
			}

		}
		std::cout << "Finished\n" << std::endl;
	}

	std::cout << "Writing index\n" << std::endl;

	memset(indice, 0, 32768);
	index2.WriteIndex(indice, &indexsize);
	d.Write(indice, 24, 16);
	memset(indice, 0xFF, 4);
	index2.WriteIndex(indice + 4, &indexsize);
	d.Write(indice, glb_IndexSector2, 16);
	std::cout << "Finished\n" << std::endl;


	std::cout << "Operation successful" << std::endl;
	fclose(log);
	return 0;
}

unsigned long createROM::FindEndOfIndex(unsigned char* index, unsigned long maxsize) {
	unsigned long i = 0;
	while (i < maxsize) {
		if ((index[i] == 0xFF) && (index[i + 1] == 0xFF) && (index[i + 2] == 0xFF))
			return i + 7;
		i += 7;
	}
	return 0;
}

unsigned long createROM::SizeToSectors(unsigned long size, int secsize)
{
	unsigned long temp;
	temp = size / secsize;
	if (size % secsize != 0) temp++;
	return temp;
}