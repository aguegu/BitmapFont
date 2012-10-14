#include "Block.h"

Block::Block(int length) :
		_length(length)
{
	_row_count = sqrt(_length * 8);
	_byte_in_row = _length / _row_count;
}

Block::~Block()
{
}

byte Block::reverseByte(byte c)
{
	return (*(REVERSE + (c & (byte) 0x0f)) << 4)
			^ (*(REVERSE + (c >> 4)) & (byte) 0x0f);
}

void Block::reverseArrayInBit(byte *destination, byte *source, int length)
{
	for (int i = 0; i < length; i++)
		destination[i] = reverseByte(source[length - 1 - i]);
}

void Block::reverseBlockInRow()
{
	byte * q = new byte[_byte_in_row];
	for (int i = 0; i < _row_count; i++)
	{
		int indent = _byte_in_row * i;
		reverseArrayInBit(q, _p + indent, _byte_in_row);
		memcpy(_p + indent, q, _byte_in_row);
	}
	delete[] q;
}

void Block::reverseBlockInCol()
{

}

void Block::setArray(char *p)
{
	_p = (byte *)p;
}

