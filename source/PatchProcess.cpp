#include "pch.h"
#include "PatchProcess.h"

PatchProcess::PatchProcess(HWND hWnd, std::string file1, std::string file2) {
    wind = hWnd;
    OpenFiles(file1, file2);
    ApplyPatch(ppf, bin, APPLY);
}

PatchProcess::~PatchProcess() {

}

void PatchProcess::ApplyPatch(int ppf, int bin, char mode) {
    unsigned char desc[51];
    unsigned char imagetype = 0;
    unsigned char undo = 0;
    unsigned char blockcheck = 0;
    unsigned char in;
    int idlen;
    __int64 offset;
    __int64 count;
    unsigned int seekpos;
    unsigned char anz = 0;
    _lseeki64(ppf, 6, SEEK_SET);
    _read(ppf, &desc, 50); 
    desc[50] = 0;
    idlen = ShowFileId(ppf);
    _lseeki64(ppf, 56, SEEK_SET);
    _read(ppf, &imagetype, 1);
    _lseeki64(ppf, 57, SEEK_SET);
    _read(ppf, &blockcheck, 1);
    _lseeki64(ppf, 58, SEEK_SET);
    _read(ppf, &undo, 1);
    if (mode == UNDO) {
        if (!undo) {
            return;
        }
    }
    if (blockcheck) {
        fflush(stdin);
        _lseeki64(ppf, 60, SEEK_SET);
        _read(ppf, &ppfblock, 1024);
        if (imagetype) {
            _lseeki64(bin, 0x80A0, SEEK_SET);
        }
        else {
            _lseeki64(bin, 0x9320, SEEK_SET);
        }
        _read(bin, &binblock, 1024);
        in = memcmp(ppfblock, binblock, 1024);
        if (in != 0) {
            fflush(stdout);
            in = getc(stdin);
            if (in != 'y' && in != 'Y') {
                return;
            }
        }
    }
    _lseeki64(ppf, 0, SEEK_END);
    count = _tell(ppf);
    _lseeki64(ppf, 0, SEEK_SET);
    if (blockcheck) {
        seekpos = 1084;
        count -= 1084;
    }
    else {
        seekpos = 60;
        count -= 60;
    }
    if (idlen) {
        count -= (idlen + 18 + 16 + 2);
    }
    _lseeki64(ppf, seekpos, SEEK_SET);
    do {
        fflush(stdout);
        _read(ppf, &offset, 8);
        _read(ppf, &anz, 1);

        if (mode == APPLY) {
            _read(ppf, &ppfmem, anz);
            if (undo) {
                _lseeki64(ppf, anz, SEEK_CUR);
            }
        }
        if (mode == UNDO) {
            _lseeki64(ppf, anz, SEEK_CUR);
            _read(ppf, &ppfmem, anz);
        }

        fflush(stdout);
        _lseeki64(bin, offset, SEEK_SET);
        _write(bin, &ppfmem, anz);
        count -= (anz + 9);
        if (undo) {
            count -= anz;
        }
    } 
    while (count != 0); 
    _close(bin);
    _close(ppf);
}

int PatchProcess::ShowFileId(int ppf) {
    char buffer2[3073];
    unsigned int idmagic;
    int lenidx = 2;
    int idlen = 0;
    int orglen = 0;
    _lseeki64(ppf, -(lenidx + 4), SEEK_END);
    _read(ppf, &idmagic, 4);
    if (idmagic != 'ZID.') {
        return(0);
    }
    else {
        _lseeki64(ppf, -lenidx, SEEK_END);
        _read(ppf, &idlen, lenidx);
        orglen = idlen;
        if (idlen > 3072) {
            idlen = 3072;
        }
        _lseeki64(ppf, -(lenidx + 16 + idlen), SEEK_END);
        _read(ppf, &buffer2, idlen);
        buffer2[idlen] = 0;
    }
    return(orglen);
}

int PatchProcess::OpenFiles(std::string file1, std::string file2) {
    char* char1 = const_cast<char*>(file1.c_str());
    char* char2 = const_cast<char*>(file2.c_str());
    bin = _open(char1, _O_BINARY | _O_RDWR);
    if (bin == -1) {
        MessageBox(wind, L"Could not find bin.", L"Error", MB_ICONERROR);
        return 1;
    }
    ppf = _open(char2, _O_RDONLY | _O_BINARY);
    if (ppf == -1) {
        MessageBox(wind, L"Could not find ppf.", L"Error", MB_ICONERROR);
        _close(bin);
        return 1;
    }
    return 0;
}