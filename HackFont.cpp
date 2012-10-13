#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
typedef unsigned char byte;

const char REVERSE[] =
{
	0x00, 0x08, 0x04, 0x0c,
	0x02, 0x0a, 0x06, 0x0e,
	0x01, 0x09, 0x05, 0x0d,
	0x03, 0x0b, 0x07, 0x0f
};

byte reverseByte(byte c)
{
	return (*(REVERSE + (c & (byte)0x0f)) << 4) ^ (*(REVERSE + (c >> 4)) & (byte)0x0f);
}

void reverseArrayInBit(char *in, char *out, int length)
{
	for (int i=0; i<length; i++)
		out[i] = reverseByte(in[length-1-i]);
}

void reverseBlockInRow(char *p, int length)
{
	char * q = new char[length];
	for (int i=0; i < row_count; i++)
	{
		int indent = byte_in_row * i;
		reverseArrayInBit(p + indent, q + indent, byte_in_row);
	}
	
	memcpy(p, q, length);

	delete[] q;
}

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
//		memset(q, 0, length);

		fout.write(p, length);
		fout.flush();
	}
	delete[] p;

	fin.close();
	fout.close();

	return 0;
}
