//============================================================================
// Name        : BitmapFont.cpp
// Description : Print out info header, hex output, pattern view for each to 
//               character in a bitmap font file. hope to help programming 
//               character display stored in arrays, especially for micro 
//               controller system.
// Version     : 1.0
// Copyright   : Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 
// 	             http://creativecommons.org/licenses/by-nc/3.0/
// Author      : Weihong Guan (@aGuegu)
// Email       : weihong.guan@gmail.com
// Blog        : http://aguegu.net
// Host        : https://github.com/aguegu/BitmapFont
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
	char **pin = &inbuf;
	char **pout = &outbuf;
	iconv_t cd = iconv_open("UTF-8", "GB2312");

	memset(outbuf, 0, outlen);

	if (cd == 0 || iconv(cd, pin, &inlen, pout, &outlen) == (size_t) (-1))
		return -1;

	iconv_close(cd);
	return 0;
}

string byteStringPure(unsigned char c)
{
	char tmp[3];
	sprintf(tmp, "%02x", c);
	string ref(tmp);
	return ref;
}

string byteString(unsigned char c)
{
	string tmp("0x");
	tmp += byteStringPure(c);
	return tmp;	
}

void printHeader(long pos, int length, bool is_dword)
{
	char s_in[8] =
	{ 0 };
	char s_out[8] =
	{ 0 };

	long index = pos / length - 1;
	cout << "// 0x" << pos;
	cout << ", 0x" << index << ", ";

	if (is_dword)
	{
		s_in[0] = 0xA1 + (index / 94);
		s_in[1] = 0xA1 + (index % 94);
		convertCode(s_in, 8, s_out, 8);
		cout << "(GB2312)" << byteString(s_in[0]);
		cout << byteStringPure(s_in[1]) << ", ";
		cout << "(UTF-8)" << byteString(s_out[0]) << byteStringPure(s_out[1])
			<< byteStringPure(s_out[2]) << ", ";
	}
	else
	{
		s_in[0] = index;
		convertCode(s_in, 8, s_out, 8);
		cout << "(GB2312)" << byteString(s_in[0]) << ", ";
		cout << "(UTF-8)" << byteString(s_in[0]) << ", ";
	}

	cout << "\" " << s_out << " \"";
}

void printVar(char *p, int length)
{
	for (int i = 0; i < length; i++)
	{
		cout << byteString(p[i]);
		cout << ", ";

		if (i % 8 == 7)
			cout << endl;
	}
}

void printPattern(char *p, int length, int byte_in_row)
{
	for (int i = 0; i < length;)
	{
		cout << "//  ";

		for (int k = byte_in_row; k--;)
		{
			unsigned char temp = p[i];
			for (int j = 8; j--;)
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
	int byte_in_row = atoi(argv[2]);

	char code_sys[4];
	int row_count, length;
	bool is_dword;

	sscanf(argv[1], "%*[^/]/%3s%d", code_sys, &row_count);

	length = byte_in_row * row_count;

	is_dword = strcmp(code_sys, "ASC");

	ifstream fin(argv[1], ios::binary);

	cout << hex;

	char *p = new char[length];
	while (fin.read(p, length))
	{
		long pos = fin.tellg();

		printHeader(pos, length, is_dword);
		cout << endl;

		printVar(p, length);
		cout << endl;

		printPattern(p, length, byte_in_row);
		cout << endl;

		cout.flush();
	}

	fin.close();
	
	delete[] p;

	return 0;
}
