#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	int row_count; 
	sscanf(argv[1], "%*[^/]/%*3s%d", &row_count);

	cout << row_count;

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

		printf("\n%02x%02x", codeH, codeL);
		unsigned int index = (codeH - 0xa1) * 94 + codeL - 0xa1;
		printf(" | %u\n", index);	

		fFont.seekg(length * index);

		fFont.read(pattern, length);

		for (int i=0; i<length; i++)
		{
			printf("%02x, ", (unsigned char)pattern[i]);
			if (i % 8 == 7) 
				cout << endl;
		}
		cout << endl;
	}

	fChar.close();
	fFont.close();
	return 0;
}
