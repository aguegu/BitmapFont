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

	char tmp[5];
	char c_on[] =
	{ 0xe2, 0x97, 0xbc };
	char c_off[] =
	{ 0xe2, 0x97, 0xbb };
	while (fin.read((char *) p, length))
	{
		//fout.write(p, length);
		for (int i = 0; i < length; i++)
		{
			sprintf(tmp, "0x%02x", p[i]);
			fout << tmp;
			fout << ", ";
		}

		fout.write(c_on, 3);
		fout.write(c_off, 3);
		fout << endl;

		fout.flush();
	}

	fin.close();
	fout.close();

	return 0;
}
