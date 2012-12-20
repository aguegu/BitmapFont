#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Block.h"

using namespace std;

int main(int argc, char* argv[])
{
	int row_count; 
	sscanf(argv[1], "%*[^/]/%*3s%d", &row_count);

	int byte_in_row = row_count / 8;
	int length = byte_in_row * row_count;

	ifstream fFont(argv[1], ios::binary);
	ifstream fChar(argv[2], ios::binary);

	char *p = new char[2];
	char *pattern = new char[length];

	while(fChar.read(p, 2))
	{
		unsigned char codeH = p[0];
		unsigned char codeL = p[1];

		unsigned int index = (codeH - 0xa1) * 94 + codeL - 0xa1;

		printf("// (GB2312)0x%02x%02x\n", codeH, codeL);

		fFont.seekg(length * index);

		fFont.read(pattern, length);

		Block block(pattern, length);

		cout << block.getVarString();
		cout << endl;
		cout << block.getPatternString();

		cout << endl;
	}

	fChar.close();
	fFont.close();
	return 0;
}
