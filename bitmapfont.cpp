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

void printFont(Block & block, int var_in_row, bool show_pattern, byte transform, byte direction, int step);

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
	bool show_pattern = false;
	int var_in_row = 8;

	byte transform = 0x00;
	byte move_direction = 0x00;
	int move_step = 0x00;
	std::string str;
	std::string s_in;
	std::string s_out;

	while ((opt = getopt(argc, argv, ":f:c:n:phvbdr:t:m:sa:o:")) != -1) {
		switch (opt) {
			case 'f':
				sscanf(optarg, "%*[^/]/%3s%d", code_sys, &row_count);
				s_in = optarg;
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
			case 'h':
				bitSet(transform, 0);
				break;
			case 'v':
				bitSet(transform, 1);
				break;
			case 's':
				bitSet(transform, 2);
				break;
			case 'b':
				bitSet(transform, 3);
				break;
			case 'r':
				transform |= (byte)(atoi(optarg) % 4) << 4;
				break;
			case 'd':
				bitSet(transform, 6);
				break;
			case 'm':
				move_direction = atoi(optarg);
				break;
			case 't':
				move_step = atoi(optarg);
				break;
			case 'a':
				str.append(optarg);
				break;
			case 'o':
				s_out.append(optarg);
				break;
			case ':':
				std::cerr << "option needs a value." << std::endl;
				exit(EXIT_FAILURE);
				break;
			case '?':
				std::cerr << "unknown option: " << (char)optopt << std::endl;
				exit(EXIT_FAILURE);
				break;
			default:
				break;
		}
	}

	int length = byte_in_row * row_count;
	bool is_dword = strcmp(code_sys, "ASC");

	std::ifstream fin(s_in.c_str(), std::ios::binary);
	std::ofstream fout;

	if (s_out.length())
		fout.open(s_out.c_str(), std::ios::binary);

	char *p = new char[length];
	Block block(p, length, byte_in_row);

	if (str.empty()) {
		while (fin.read(p, length)) {
			std::cout << getHeader(fin.tellg(), length, is_dword) << std::endl;
			printFont(block, var_in_row, show_pattern, transform, move_direction, move_step);
			if (s_out.length()) fout.write(p, length);
		} 
	} else {
		int len = str.length() + 1;
		char *source = new char[len];
		std::strcpy(source, str.c_str());
		char *dest = new char[len];
		convertCode("GB2312", "utf-8", source, len, dest, len);
		delete[] source;	

		unsigned int i = 0;
		while (i < strlen(dest)) {
			byte c = dest[i];

			if (c < 0x7f && !is_dword) {
				fin.seekg(length * dest[i]);
				fin.read(p, length); 
				std::cout << getHeader(fin.tellg(), length, is_dword) << std::endl;
				printFont(block, var_in_row, show_pattern, transform, move_direction, move_step);
				if (s_out.length()) fout.write(p, length);
			} else if (c >= 0xa1 && is_dword ) {
				fin.seekg(((c - 0xa1) * 94 + (byte)dest[i+1] - 0xa1) * length);	
				fin.read(p, length); 
				std::cout << getHeader(fin.tellg(), length, is_dword) << std::endl;
				printFont(block, var_in_row, show_pattern, transform, move_direction, move_step);
				if (s_out.length()) fout.write(p, length);
				i++;
			} else {
				std::cerr << "// No pattern for " << (char)dest[i] << std::endl;
			}
			i++;
		}

		delete[] dest;
	}

	fin.close();
	delete[] p;

	if (s_out.length())
		fout.close();

	exit(EXIT_SUCCESS);
}

void moveBlock(Block & block, int direction, int step)
{
	switch (direction & 0x0f)
	{
		case 0x01:
			block.setMoveDirection(Block::BIT_IN_COL_POSI);
			break;
		case 0x02:
			block.setMoveDirection(Block::BIT_IN_COL_NEGA);
			break;
		case 0x04:
			block.setMoveDirection(Block::BIT_IN_ROW_POSI);
			break;
		case 0x08:
			block.setMoveDirection(Block::BIT_IN_ROW_NEGA);
			break;
		default:
			step = 0;
			break;
	}

	while (step--)
	{
		block.move(direction & 0x10);
	}
}

void printFont(Block & block, int var_in_row, bool show_pattern, byte transform, byte direction, int step)
{
	switch ((transform & 0x30) >> 4) {
		case 3:
			block.rotate(Block::R270);
			break;
		case 2:
			block.rotate(Block::R180);
			break;
		case 1:
			block.rotate(Block::R90);
			break;
		case 0: 
		default:
			break;
	}

	moveBlock(block, direction, step);

	if (bitRead(transform, 0)) block.slipInRow();
	if (bitRead(transform, 1)) block.slipInCol();
	if (bitRead(transform, 2)) block.slipInByte();
	if (bitRead(transform, 3)) block.opposite();
	if (bitRead(transform, 6)) block.slipInDiag();

	std::cout << block.getVarString(var_in_row) << std::endl;

	if (show_pattern)
		std::cout << block.getPatternString() << std::endl;
}


