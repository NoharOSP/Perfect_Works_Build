#include "pch.h"
#include "isoManager.h"
#include "crctable.out"
#include "crctables"

unsigned char from_BCD(unsigned char x) {
	return (((x & 15) + ((x & 240) >> 4) * 10));
}

unsigned char to_BCD(unsigned char x) {
	return (((x / 10) << 4) | (x % 10));
}
int is_valid_BCD(unsigned char x) {
	return (((x & 15) < 10) && ((x >> 4) < 10));
}

isoManager::isoManager()
{
}

isoManager::~isoManager()
{
}

int isoManager::Read(unsigned char* buffer, unsigned long sector, unsigned long size)
{
	FILE* file;
	file = fopen(m_filename, "rb");
	if (file == NULL) {
		return 0;
	}
	const int headersize = 24;
	const int eccsize = 280;
	fseek(file, sector * 2352, SEEK_SET);
	for (unsigned int i = 0; i < size; i++) {
		fseek(file, headersize, SEEK_CUR);
		fread(buffer + i * 2048, 2048, 1, file);
		fseek(file, eccsize, SEEK_CUR);
	}
	fclose(file);
	return 1;
}

int isoManager::OpenIso(char* filename)
{
	FILE* file;
	unsigned long size;

	file = fopen(filename, "rb");
	if (file == NULL) {
		return 0;
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fclose(file);
	m_NumOfSectors = SizeToSectors(size, 2352);
	strcpy(m_filename, filename);
	return 1;
}

int isoManager::ReadRawSectors(unsigned char* buffer, unsigned long sector, unsigned long size)
{
	FILE* file;
	if (sector > m_NumOfSectors) {
		return 0;
	}
	file = fopen(m_filename, "rb");
	if (file == NULL) {
		return 0;
	}
	fseek(file, sector * 2352, SEEK_SET);
	fread(buffer, 2352 * size, 1, file);
	fclose(file);
	return 1;
}

int isoManager::WriteRawSectors(unsigned char* buffer, unsigned long sector, unsigned long size)
{
	FILE* file;
	if (sector > m_NumOfSectors) {
		return 0;
	}
	file = fopen(m_filename, "rb+");
	if (file == NULL) {
		return 0;
	}
	fseek(file, sector * 2352, SEEK_SET);
	fwrite(buffer, 2352 * size, 1, file);
	fclose(file);
	return 1;
}

int isoManager::AppendRawSectors(unsigned char* buffer, unsigned long size)
{
	FILE* file;
	file = fopen(m_filename, "ab");
	if (file == NULL) {
		return 0;
	}
	fwrite(buffer, 2352 * size, 1, file);
	fclose(file);
	m_NumOfSectors += size;
	return 1;
}

int isoManager::CreateIso(char* filename)
{
	FILE* file;
	file = fopen(filename, "wb");
	if (file == NULL) {
		return 0;
	}
	fclose(file);
	strcpy(m_filename, filename);
	m_NumOfSectors = 0;
	return 1;
}

unsigned long isoManager::SizeToSectors(unsigned long size, int secsize)
{
	unsigned long temp;
	temp = size / secsize;
	if (size % secsize != 0) {
		temp++;
	}
	return temp;
}

int isoManager::Write(unsigned char* buffer, unsigned long sector, unsigned long size)
{
	const int headersize = 24;
	const int eccsize = 280;

	unsigned char* data;
	data = new unsigned char[size * 2352];
	memset(data, 0, 2352 * size);

	for (unsigned int i = 0; i < size; i++) {
		unsigned char sinc[12] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
		unsigned char local[3];
		unsigned char mode = 0x02;
		unsigned char subheader[4] = { 0x00, 0x00, 0x08, 0x00 };

		ToMSF(sector + i, local);

		memcpy(data + i * 2352, sinc, 12);
		memcpy(data + i * 2352 + 16, subheader, 4);
		memcpy(data + i * 2352 + 20, subheader, 4);

		memcpy(data + i * 2352 + headersize, buffer + i * 2048, 2048);

		unsigned long edc = build_edc(data + i * 2352, 16, 16 + 8 + 2048 - 1);
		memcpy(data + i * 2352 + 2072, &edc, 4);
		encode_L2_P(data + i * 2352 + 12);
		encode_L2_Q(data + i * 2352 + 12);
		memcpy(data + i * 2352 + 12, local, 3);
		data[i * 2352 + 15] = mode;
	}

	if (WriteRawSectors(data, sector, size) == 0) {
		return 0;
	}
	delete[] data;

	return 1;
}

int isoManager::ToMSF(unsigned long Sector, unsigned char* res)
{
	res[2] = (unsigned char)(Sector % 75);
	res[1] = (unsigned char)((Sector / 75) % 60) + 2;
	res[0] = (unsigned char)(Sector / (75 * 60));

	res[0] = to_BCD(res[0]);
	res[1] = to_BCD(res[1]);
	res[2] = to_BCD(res[2]);

	return 1;
}

int isoManager::Append(unsigned char* buffer, unsigned long size)
{
	const int headersize = 24;
	const int eccsize = 280;

	unsigned char* data;
	data = new unsigned char[size * 2352];
	memset(data, 0, 2352 * size);


	for (unsigned int i = 0; i < size; i++) {
		unsigned char sinc[12] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
		unsigned char local[3];
		unsigned char mode = 0x02;
		unsigned char subheader[4] = { 0x00, 0x00, 0x08, 0x00 };

		ToMSF(m_NumOfSectors + i, local);

		memcpy(data + i * 2352, sinc, 12);
		memcpy(data + i * 2352 + 16, subheader, 4);
		memcpy(data + i * 2352 + 20, subheader, 4);

		memcpy(data + i * 2352 + headersize, buffer + i * 2048, 2048);

		unsigned long edc = build_edc(data + i * 2352, 16, 16 + 8 + 2048 - 1);
		memcpy(data + i * 2352 + 2072, &edc, 4);
		encode_L2_P(data + i * 2352 + 12);
		encode_L2_Q(data + i * 2352 + 12);
		memcpy(data + i * 2352 + 12, local, 3);
		data[i * 2352 + 15] = mode;
	}

	if (AppendRawSectors(data, size) == 0) {
		return 0;
	}
	delete[] data;
	return 1;
}

unsigned long isoManager::build_edc(unsigned char inout[], int from, int upto)
{
	unsigned char* p = inout + from;
	unsigned long result = 0;

	for (; from <= upto; from++) {
		result = EDC_crctable[(result ^ *p++) & 0xffL] ^ (result >> 8);
	}

	return result;
}

int isoManager::encode_L2_P(unsigned char inout[])
{
	unsigned char* P;
	int i;
	int j;
	P = inout + 4 + L2_RAW + 4 + 8;
	memset(P, 0, L2_P);
	for (j = 0; j < 43; j++) {
		for (i = 0; i < 24; i++) {
			unsigned char data;

			data = inout[i * 2 * 43];
			if (data != 0) {
				unsigned int base = rs_l12_log[data];

				unsigned int sum = base + DP[0][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				P[0] ^= rs_l12_alog[sum];

				sum = base + DP[1][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				P[43 * 2] ^= rs_l12_alog[sum];
			}

			data = inout[i * 2 * 43 + 1];
			if (data != 0) {
				unsigned int base = rs_l12_log[data];

				unsigned int sum = base + DP[0][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				P[1] ^= rs_l12_alog[sum];

				sum = base + DP[1][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				P[43 * 2 + 1] ^= rs_l12_alog[sum];
			}
		}
		P += 2;
		inout += 2;
	}
	return 0;
}

int isoManager::encode_L2_Q(unsigned char inout[])
{
	unsigned char* Q;
	int i, j;

	Q = inout + 4 + L2_RAW + 4 + 8 + L2_P;
	memset(Q, 0, L2_Q);
	for (j = 0; j < 26; j++) {
		for (i = 0; i < 43; i++) {
			unsigned char data;

			data = inout[(j * 43 * 2 + i * 2 * 44) % (4 + L2_RAW + 4 + 8 + L2_P)];
			if (data != 0) {
				unsigned int base = rs_l12_log[data];

				unsigned int sum = base + DQ[0][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				Q[0] ^= rs_l12_alog[sum];

				sum = base + DQ[1][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				Q[26 * 2] ^= rs_l12_alog[sum];
			}

			data = inout[(j * 43 * 2 + i * 2 * 44 + 1) % (4 + L2_RAW + 4 + 8 + L2_P)];
			if (data != 0) {
				unsigned int base = rs_l12_log[data];

				unsigned int sum = base + DQ[0][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				Q[1] ^= rs_l12_alog[sum];

				sum = base + DQ[1][i];
				if (sum >= ((1 << RS_L12_BITS) - 1)) {
					sum -= (1 << RS_L12_BITS) - 1;
				}

				Q[26 * 2 + 1] ^= rs_l12_alog[sum];
			}
		}
		Q += 2;
	}
	return 0;
}
