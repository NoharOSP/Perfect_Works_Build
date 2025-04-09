#pragma once
#include <iostream>

class isoManager
{
public:
#define L2_RAW (1024*2)
#define L2_Q   (26*2*2)
#define L2_P   (43*2*2)
#define RS_L12_BITS 8
public:
	isoManager();
	virtual ~isoManager();
	int encode_L2_Q(unsigned char inout[4 + L2_RAW + 4 + 8 + L2_P + L2_Q]);
	int encode_L2_P(unsigned char inout[4 + L2_RAW + 4 + 8 + L2_P]);
	unsigned long build_edc(unsigned char inout[], int from, int upto);
	int Append(unsigned char* buffer, unsigned long size);
	int Write(unsigned char* buffer, unsigned long sector, unsigned long size);
	int CreateIso(char* filename);
	int AppendRawSectors(unsigned char* buffer, unsigned long size);
	int WriteRawSectors(unsigned char* buffer, unsigned long sector, unsigned long size);
	int ReadRawSectors(unsigned char* buffer, unsigned long sector, unsigned long size);
	int OpenIso(char* filename);
	int Read(unsigned char* buffer, unsigned long sector, unsigned long size);
private:
	unsigned long SizeToSectors(unsigned long size, int secsize);
	unsigned long m_NumOfSectors;
	char m_filename[100];
protected:
	int ToMSF(unsigned long Sector, unsigned char* res);
};

