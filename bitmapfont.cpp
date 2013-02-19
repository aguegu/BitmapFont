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
#include <unistd.h>
#include <sstream>
#include "block.h"

int convertCode(char *inbuff, unsigned long inlen, char *outbuff,
		unsigned long outlen)
{
	iconv_t cd = iconv_open("UTF-8", "GB2312");

	memset(outbuff, 0, outlen);

	if (cd == 0 || iconv(cd, &inbuff, &inlen, &outbuff, &outlen) == (size_t) (-1))
		return -1;

	iconv_close(cd);
	return 0;
}

std::string getHeader(long pos, int length, bool is_dword)
{
	char s_in[2] = { 0 };
	char s_out[4] = { 0 };

	long index = pos / length - 1;
	std::ostringstream s;
	s << std::hex;
	s << "// 0x" << pos - length;
	s << ", 0x" << index << ", ";

	if (is_dword)
	{
		s_in[0] = 0xA1 + (index / 94);
		s_in[1] = 0xA1 + (index % 94);
		convertCode(s_in, sizeof(s_in), s_out, sizeof(s_out));
		s << "(GB2312)" << Block::byteString(s_in[0]);
		s << Block::byteStringPure(s_in[1]) << ", ";
		s << "(UTF-8)" << Block::byteString(s_out[0]) << Block::byteStringPure(s_out[1])
			<< Block::byteStringPure(s_out[2]) << ", ";
	}
	else
	{
		s_in[0] = index;
		convertCode(s_in, sizeof(s_in), s_out, sizeof(s_out));
		s << "(GB2312)" << Block::byteString(s_in[0]) << ", ";
		s << "(UTF-8)" << Block::byteString(s_in[0]) << ", ";
	}

	s << "\" " << s_out << " \"";

	return (s.str());
}

int main(int argc, char ** argv)
{
	int opt;
	int byte_in_row = 1;
	int row_count = 1;
	char code_sys[4] = "\0";
	char *file_font = NULL;
	bool show_pattern = false;

	while ((opt = getopt(argc, argv, ":f:c:p")) != -1) {
		switch (opt) {
			case 'f':
				sscanf(optarg, "%*[^/]/%3s%d", code_sys, &row_count);
				file_font = optarg;
				break;
			case 'c':
				byte_in_row = atoi(optarg);
				break;
			case '?':
				fprintf(stderr, "unknown option: %c\n", optopt);
				exit(EXIT_FAILURE);
				break;
			case 'p':
				show_pattern = true;
			default:
				break;
		}
	}

	int length = byte_in_row * row_count;
	bool is_dword = strcmp(code_sys, "ASC");

	std::ifstream fin(file_font, std::ios::binary);

	char *p = new char[length];
	while (fin.read(p, length))
	{
		long pos = fin.tellg();

		std::cout << getHeader(pos, length, is_dword) << std::endl;

		Block block(p, length, byte_in_row);

		std::cout << block.getVarString() << std::endl;

		if (show_pattern)
			std::cout << block.getPatternString() << std::endl;

		std::cout.flush();
	}

	fin.close();

	delete[] p;

	exit(EXIT_SUCCESS);
}
