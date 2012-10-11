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

	char *p = new char[length];

	ifstream fin;
	fin.open(argv[1], ios::binary);
	fin.read(p, length);
	fin.close();

	ofstream fout;
	fout.open(argv[2]);
	fout.write(p, length);
	fout.flush();
	fout.close();

	return 0;
}
