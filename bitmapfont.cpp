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

void printFont(char *p, long pos, int length, bool is_dword, int byte_in_row, int val_in_row, 
		bool slip_horizontal, bool slip_vertical, bool slip_inbyte, bool show_pattern);

int convertCode(const char * tocode, const char * fromcode, char *inbuff, size_t inlen, char *outbuff, size_t outlen)
{
	iconv_t cd = iconv_open(tocode, fromcode);

	memset(outbuff, 0, outlen);

	if (cd == 0 || iconv(cd, &inbuff, &inlen, &outbuff, &outlen) == (size_t) (-1))
		return -1;

	iconv_close(cd);
	return 0;
}

std::string getHeader(long pos, int length, bool is_dword)
{
	std::ostringstream s;
	s << std::hex;
	s << "// 0x" << pos - length;

	long index = pos / length - 1;
	s << ", 0x" << index << ", ";

	if (is_dword) {
		char s_in[2], s_out[4];
		s_in[0] = 0xA1 + (index / 94);
		s_in[1] = 0xA1 + (index % 94);
		
		convertCode("utf-8", "GB2312", s_in, sizeof(s_in), s_out, sizeof(s_out));
		s << "(GB2312)" << Block::byteString(s_in[0]) << Block::byteStringPure(s_in[1]) << ", ";
		s << "(UTF-8)" << Block::byteString(s_out[0]) << Block::byteStringPure(s_out[1])
			<< Block::byteStringPure(s_out[2]) << ", ";
		s << "\" " << s_out << " \"";
	} else 	{
		s << "(UTF-8)" << Block::byteString(index) << ", ";
		s << "\" " << (char)index << " \"";
	}

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
	int var_in_row = 8;
	bool slip_horizontal = false, slip_vertical = false, slip_inbyte = false;

	while ((opt = getopt(argc, argv, ":f:c:n:pbhv")) != -1) {
		switch (opt) {
			case 'f':
				sscanf(optarg, "%*[^/]/%3s%d", code_sys, &row_count);
				file_font = optarg;
				break;
			case 'c':
				byte_in_row = atoi(optarg);
				if (byte_in_row == 0) byte_in_row = 1;
				break;
			case 'p':
				show_pattern = true;
				break;
			case 'n':
				var_in_row = atoi(optarg);
				if (var_in_row == 0) var_in_row = 8;
				break;
			case 'b':
				slip_inbyte = true;
				break;
			case 'h':
				slip_horizontal = true;
				break;
			case 'v':
				slip_vertical = true;
				break;
			case ':':
				fprintf(stderr, "option needs a value.\n");
				exit(EXIT_FAILURE);
				break;
			case '?':
				fprintf(stderr, "unknown option: %c\n", optopt);
				exit(EXIT_FAILURE);
				break;
			default:
				break;
		}
	}

	std::string str;
	while (optind < argc) {
		str.append(argv[optind]);
		optind++;
	}

	int length = byte_in_row * row_count;
	bool is_dword = strcmp(code_sys, "ASC");

	std::ifstream fin(file_font, std::ios::binary);

	char *p = new char[length];

	if (str.empty()) {
		while (fin.read(p, length)) {
			printFont(p, fin.tellg(), length, is_dword, byte_in_row, var_in_row, 
					slip_horizontal, slip_vertical, slip_inbyte, show_pattern);
		} 
	} else {
		char *source = new char [str.length() + 1];
		std::strcpy(source, str.c_str());
		char *dest = new char[str.length() + 1];

		convertCode("GB2312", "utf-8", source, sizeof(source), dest, sizeof(dest));

		unsigned int i = 0;
		while (i < strlen(dest)) {

			unsigned char c = (unsigned char) dest[i];

			if (c < 0x7f && !is_dword) {
				fin.seekg(length * dest[i]);
				fin.read(p, length); 
				printFont(p, fin.tellg(), length, is_dword, byte_in_row, var_in_row, 
					slip_horizontal, slip_vertical, slip_inbyte, show_pattern);
			} else if (c >= 0xa1 && is_dword ) {
				fin.seekg(((c - 0xa1) * 94 + (unsigned char)dest[i+1] - 0xa1) * length);	
				fin.read(p, length); 
				printFont(p, fin.tellg(), length, is_dword, byte_in_row, var_in_row, 
					slip_horizontal, slip_vertical, slip_inbyte, show_pattern);
				i++;
			} else {
				fprintf(stderr, "// No pattern for %c \n", dest[i]);
			}
			i++;
		}

		delete[] source;	
		delete[] dest;
	}

	fin.close();

	delete[] p;

	exit(EXIT_SUCCESS);
}

void printFont(char *p, long pos, int length, bool is_dword, int byte_in_row, int var_in_row, 
		bool slip_horizontal, bool slip_vertical, bool slip_inbyte, bool show_pattern)
{
	std::cout << getHeader(pos, length, is_dword) << std::endl;

	Block block(p, length, byte_in_row);

	if (slip_horizontal) block.slipInRow();
	if (slip_vertical) block.slipInCol();
	if (slip_inbyte) block.slipInByte();

	std::cout << block.getVarString(var_in_row) << std::endl;

	if (show_pattern)
		std::cout << block.getPatternString() << std::endl;
}
