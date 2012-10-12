//============================================================================
// Name        : BitmapFont.cpp
// Author      : aGuegu
// Version     :
// Copyright   : @aguegu.net
// Description : BitmapFont
//============================================================================

#include <iostream>
#include <iconv.h>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

const char c_on[] =
{ 0xe2, 0x96, 0xa0, 0x20 };
const char c_off[] =
{ 0xe2, 0x96, 0xa1, 0x20 };

int convertCode(char *inbuf, unsigned long inlen, char *outbuf,
		unsigned long outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open("UTF-8", "GB2312");
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == (size_t) (-1))
		return -1;
	iconv_close(cd);
	return 0;
}

char *byteString(unsigned char c)
{
	char *tmp = new char[5];
	sprintf(tmp, "0x%02x", c);
	return tmp;
}

char *byteStringPure(unsigned char c)
{
	char *tmp = new char[3];
	sprintf(tmp, "%02x", c);
	return tmp;
}

void printString(ofstream & fout, unsigned char *p, int length)
{
	for (int i = 0; i < length; i++)
	{
		fout << byteString(p[i]);
		fout << ", ";

		if (i % 8 == 7)
			fout << endl;
	}
}

void printPattern(ofstream & fout, unsigned char *p, int length,
		int byte_in_row)
{
	for (int i = 0; i < length;)
	{
		fout << "//  ";

		for (int k = byte_in_row; k > 0; k--)
		{
			unsigned char temp = p[i];
			for (int j = 0; j < 8; j++)
			{
				fout.write(temp >= 0x80 ? c_on : c_off, 4);
				temp <<= 1;
			}
			i++;
		}
		fout << endl;
	}
}

int main(int argc, char* argv[])
{
	int length = atoi(argv[3]);
	int byte_in_row = atoi(argv[4]);
	int byte_in_code = atoi(argv[5]);
	int code_offset;
	sscanf(argv[6], "0x%02X", &code_offset);

	//cout << "BitmapFont" << endl; // prints !!!Hello World!!!
	cout << argv[1] << ", ";
	cout << argv[2] << ", ";
	cout << length << ", ";
	cout << byteString(byte_in_row) << ", ";
	cout << byteString(byte_in_code) << ", ";
	cout << byteString(code_offset) << endl;

	char s_in[8];
	char s_out[8];
	unsigned char *p = new unsigned char[length];

	ifstream fin(argv[1], ios::binary);
	ofstream fout(argv[2]);

	fout << hex;

	while (fin.read((char *) p, length))
	{
		long pos = fin.tellg();
		long index = pos / length - 1;

		fout << "// 0x" << fin.tellg();
		fout << ", 0x" << index << ", ";

		memset(s_in, 0x00, 8);
		if (byte_in_code == 1)
		{
			s_in[0] = index + code_offset;
			fout << "(GB2312)" << byteString(s_in[0]) << ", ";
		}
		else
		{
			s_in[0] = code_offset + (index / 94);
			s_in[1] = 0xA1 + (index % 94);
			fout << "(GB2312)" << byteString(s_in[0]);
			fout << byteStringPure(s_in[1]) << ", ";
		}

		convertCode(s_in, 8, s_out, 8);
		fout << "(UTF-8)" << byteString(s_out[0]) << byteStringPure(s_out[1])
				<< byteStringPure(s_out[2]);
		fout << ", \" " << s_out << " \"";

		fout << endl;

		printString(fout, p, length);
		fout << endl;

		printPattern(fout, p, length, byte_in_row);
		fout << endl;

		fout.flush();
	}

	fin.close();
	fout.close();

	return 0;
}
