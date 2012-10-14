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

void Block::reverseInRow()
{
	byte * cache = new byte[_byte_in_row];
	byte * p = _p;
	for (int i = 0; i < _row_count; i++, p += _byte_in_row)
	{
		reverseArrayInBit(cache, p, _byte_in_row);
		memcpy(p, cache, _byte_in_row);
	}
	delete[] cache;
}

void Block::reverseInCol()
{
	byte * cache = new byte[_byte_in_row];
	byte * p = _p;
	byte * q = _p + _length - _byte_in_row;

	for (int i = 0; i < _row_count; i++, i++)
	{
		memcpy(cache, p, _byte_in_row);
		memcpy(p, q, _byte_in_row);
		memcpy(q, cache, _byte_in_row);
		p += _byte_in_row;
		q -= _byte_in_row;
	}
	delete[] cache;
}

void Block::reverseInDiag()
{
	byte * cache = new byte[_length];
	memset(cache, 0x00, _length);

	for (int i=0; i < _length; i++)
	{
		int r = i / _byte_in_row;

		for (int j=0; j<8; j++)
		{
			int c = (i % _byte_in_row) * 8 + 7-j;
			int index = c * _byte_in_row + r / 8;
			if (_p[i] & _BV(j))
			    cache[index] |= _BV(7-r%8);
		}
	}
	memcpy(_p, cache, _length);

	delete[] cache;
}

void Block::setArray(char *p)
{
	_p = (byte *)p;
}

