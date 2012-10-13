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

void printHeader(long pos, int length, int byte_in_code, int code_offset)
{
	char s_in[8] =
	{ 0 };
	char s_out[8] =
	{ 0 };

	long index = pos / length - 1;
	cout << "// 0x" << pos;
	cout << ", 0x" << index << ", ";

	if (byte_in_code == 1)
	{
		s_in[0] = index + code_offset;
		convertCode(s_in, 8, s_out, 8);
		cout << "(GB2312)" << byteString(s_in[0]) << ", ";
		cout << "(UTF-8)" << byteString(s_in[0]) << ", ";
	}
	else
	{
		s_in[0] = code_offset + (index / 94);
		s_in[1] = 0xA1 + (index % 94);
		convertCode(s_in, 8, s_out, 8);
		cout << "(GB2312)" << byteString(s_in[0]);
		cout << byteStringPure(s_in[1]) << ", ";
		cout << "(UTF-8)" << byteString(s_out[0]) << byteStringPure(s_out[1])
				<< byteStringPure(s_out[2]) << ", ";
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
	int length = atoi(argv[2]);
	int byte_in_row = atoi(argv[3]);
	int byte_in_code = atoi(argv[4]);
	int code_offset;
	sscanf(argv[5], "0x%02X", &code_offset);

	char *p = new char[length];

	ifstream fin(argv[1], ios::binary);

	cout << hex;

	while (fin.read(p, length))
	{
		long pos = fin.tellg();

		printHeader(pos, length, byte_in_code, code_offset);
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
