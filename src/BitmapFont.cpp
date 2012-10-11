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

using namespace std;

int main(int argc, char* argv[])
{
	char s[24];

	//cout << "BitmapFont" << endl; // prints !!!Hello World!!!
	cout << argv[1] << endl;
	cout << argv[2] << endl;

	ifstream fin;
	fin.open(argv[1], ios::binary);
	fin.read(s, 24);
	fin.close();

	ofstream fout;
	fout.open(argv[2]);
	fout.write(s, 24);
	fout.flush();
	fout.close();
	return 0;
}
