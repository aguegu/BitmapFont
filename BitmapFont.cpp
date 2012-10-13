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

void printHeader(long index, int byte_in_code, int code_offset)
{
	char s_in[8] =
	{ 0 };
	char s_out[8];

	if (byte_in_code == 1)
	{
		s_in[0] = index + code_offset;
		cout << "(GB2312)" << byteString(s_in[0]) << ", ";
		cout << "(UTF-8)" << byteString(s_in[0]) << ", ";
	}
	else
	{
		s_in[0] = code_offset + (index / 94);
		s_in[1] = 0xA1 + (index % 94);
		cout << "(GB2312)" << byteString(s_in[0]);
		cout << byteStringPure(s_in[1]) << ", ";
		cout << "(UTF-8)" << byteString(s_out[0]) << byteStringPure(s_out[1])
				<< byteStringPure(s_out[2]);
	}

	convertCode(s_in, 8, s_out, 8);

	cout << ", \" " << s_out << " \"";
}

void printVar(unsigned char *p, int length)
{
	for (int i = 0; i < length; i++)
	{
		cout << byteString(p[i]);
		cout << ", ";

		if (i % 8 == 7)
			cout << endl;
	}
}

void printPattern(unsigned char *p, int length, int byte_in_row)
{
	for (int i = 0; i < length;)
	{
		cout << "//  ";

		for (int k = byte_in_row; k > 0; k--)
		{
			unsigned char temp = p[i];
			for (int j = 0; j < 8; j++)
			{
				cout.write(temp >= 0x80 ? c_on : c_off, 4);
				temp <<= 1;
			}
			i++;
		}
		cout << endl;
	}
}

int main(int argc, char* argv[])
{
	int length = atoi(argv[2]);
	int byte_in_row = atoi(argv[3]);
	int byte_in_code = atoi(argv[4]);
	int code_offset;
	sscanf(argv[5], "0x%02X", &code_offset);

	//cout << "BitmapFont" << endl; // prints !!!Hello World!!!
	//	cout << argv[1] << ", ";
	//	cout << argv[2] << ", ";
	//	cout << length << ", ";
	//	cout << byteString(byte_in_row) << ", ";
	//	cout << byteString(byte_in_code) << ", ";
	//	cout << byteString(code_offset) << endl;

	unsigned char *p = new unsigned char[length];

	ifstream fin(argv[1], ios::binary);

	cout << hex;

	while (fin.read((char *) p, length))
	{
		long pos = fin.tellg();
		long index = pos / length - 1;

		cout << "// 0x" << pos;
		cout << ", 0x" << index << ", ";

		printHeader(index, byte_in_code, code_offset);
		cout << endl;

		printVar(p, length);
		cout << endl;

		printPattern(p, length, byte_in_row);
		cout << endl;

		cout.flush();
	}

	fin.close();

	return 0;
}
