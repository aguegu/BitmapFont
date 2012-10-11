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

using namespace std;

void printString(ofstream & fout, unsigned char *p, int length)
{
	char tmp[5];

	for (int i = 0; i < length; i++)
	{
		sprintf(tmp, "0x%02x", p[i]);
		fout << tmp;
		fout << ", ";

		if (i % 8 == 7)
			fout << endl;
	}
	fout << endl;
}

void printPattern(ofstream & fout, unsigned char *p, int length,
		int byte_in_row)
{
	char c_on[] =
	{ 0xe2, 0x97, 0xbc };
	char c_off[] =
	{ 0xe2, 0x97, 0xbb };

	for (int i = 0; i < length;)
	{
		fout << "//  ";

		for (int k = byte_in_row; k > 0; k--)
		{
			unsigned char temp = p[i];
			for (int j = 0; j < 8; j++)
			{
				fout.write(temp >= 0x80 ? c_on : c_off, 3);
				temp <<= 1;
			}
			i++;
		}
		fout << endl;
	}

	fout << endl;
}

int main(int argc, char* argv[])
{
	int length = atoi(argv[3]);
	int byte_in_row = atoi(argv[4]);
	int byte_in_code = atoi(argv[5]);
	int code_offset;
	sscanf(argv[6], "0x%02x", &code_offset);

	//cout << "BitmapFont" << endl; // prints !!!Hello World!!!
	cout << argv[1] << endl;
	cout << argv[2] << endl;
	cout << length << endl;
	cout << byte_in_row << endl;
	cout << byte_in_code << endl;
	cout << code_offset << endl;

	unsigned char *p = new unsigned char[length];

	ifstream fin(argv[1], ios::binary);
	ofstream fout(argv[2]);

	fout << hex;

	while (fin.read((char *) p, length))
	{
		long pos = fin.tellg();
        long index = pos / length - 1;

		fout << "// 0x" << fin.tellg();
		fout << ", 0x" << index;
		fout << ", 0x" << index + code_offset;
		fout << ", \" " << (char)(index + code_offset) << " \"";
		fout << endl;

		printString(fout, p, length);
		fout << endl;
		printPattern(fout, p, length, byte_in_row);

		fout.flush();
	}

	fin.close();
	fout.close();

	return 0;
}
