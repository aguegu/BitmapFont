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
#include "block.h"

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

void printHeader(long pos, int length, bool is_dword)
{
	char s_in[8] =
	{ 0 };
	char s_out[8] =
	{ 0 };

	long index = pos / length - 1;
	std::cout << "// 0x" << pos - length;
	std::cout << ", 0x" << index << ", ";

	if (is_dword)
	{
		s_in[0] = 0xA1 + (index / 94);
		s_in[1] = 0xA1 + (index % 94);
		convertCode(s_in, 8, s_out, 8);
		std::cout << "(GB2312)" << Block::byteString(s_in[0]);
		std::cout << Block::byteStringPure(s_in[1]) << ", ";
		std::cout << "(UTF-8)" << Block::byteString(s_out[0]) << Block::byteStringPure(s_out[1])
			<< Block::byteStringPure(s_out[2]) << ", ";
	}
	else
	{
		s_in[0] = index;
		convertCode(s_in, 8, s_out, 8);
		std::cout << "(GB2312)" << Block::byteString(s_in[0]) << ", ";
		std::cout << "(UTF-8)" << Block::byteString(s_in[0]) << ", ";
	}

	std::cout << "\" " << s_out << " \"";
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

	std::ifstream fin(argv[1], std::ios::binary);

	std::cout << std::hex;

	char *p = new char[length];
	while (fin.read(p, length))
	{
		long pos = fin.tellg();

		printHeader(pos, length, is_dword);
		std::cout << std::endl;

		Block block(p, length, byte_in_row);

		std::cout << block.getVarString();
		std::cout << std::endl;

		std::cout << block.getPatternString();
		std::cout << std::endl;

		std::cout.flush();
	}

	fin.close();
	
	delete[] p;

	return 0;
}
