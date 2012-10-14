#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <cstring>
#include <cmath>

typedef unsigned char byte;
using namespace std;

#define _BV(n) (0x01U << (n))
#define _LV(n) (~_BV(n))

const char REVERSE[] =
{
	0x00, 0x08, 0x04, 0x0c,
	0x02, 0x0a, 0x06, 0x0e,
	0x01, 0x09, 0x05, 0x0d,
	0x03, 0x0b, 0x07, 0x0f
};

class Block
{
	public:
		Block(int length);
		virtual	~Block();
		void setArray(char *p);
		void reverseInRow();
		void reverseInCol();
		void reverseInDiag();

	private:
		const int _length;
		int _row_count;
		int _byte_in_row;

		byte *_p;
		byte reverseByte(byte c);
		void reverseArrayInBit(byte *destination, byte *source, int length);

};

#endif
