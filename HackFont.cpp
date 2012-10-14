#include <iostream>
#include <fstream>
#include <cstring>
#include "Block.h"

using namespace std;
typedef unsigned char byte;

int main(int argc, char* argv[])
{
	int row_count;
	sscanf(argv[1], "%*[^/]/%*3s%d", &row_count);
	cout << row_count << endl;

	int byte_in_row = row_count / 8;
	cout << byte_in_row << endl;

	int length = byte_in_row * row_count;
	cout << length << endl;

	ifstream fin(argv[1], ios::binary);
	ofstream fout(argv[2], ios::binary);

	char * p = new char[length];
	while(fin.read(p, length))
	{
		Block block(length);
		block.setArray(p);
		block.reverseBlockInRow();

		fout.write(p, length);
		fout.flush();
	}
	delete[] p;

	fin.close();
	fout.close();

	return 0;
}
