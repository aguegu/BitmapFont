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

	byte *p = _p;
	for (int r = 0; r < _row_count; r++)
	{
		for (int i = 0; i < _byte_in_row; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				int c = i * 8 + 7 - j;
				int index = c * _byte_in_row + (r >> 3);
				if (*p & _BV(j))
					cache[index] |= _BV(7 - (r & 0x07));
			}
			p++;
		}
	}
	memcpy(_p, cache, _length);

	delete[] cache;
}

void Block::setArray(char *p)
{
	_p = (byte *) p;
}

void Block::moveBitInColNega(bool recycle)
{
	byte *p = _p;
	for (byte r = _row_count; r--;)
	{
		bool b0 = bitRead(*p, 7);
		for (byte i = _byte_in_row - 1; i--;)
		{
			bool b = bitRead(*(p + 1), 7);
			*p <<= 1;
			bitWrite(*p, 0, b);
			p++;
		}

		*p <<= 1;
		if (recycle)
			bitWrite(*p, 0, b0);

		p++;
	}
}

void Block::moveBitInColPosi(bool recycle)
{
	byte *p = _p + _byte_in_row - 1;
	for (byte r = _row_count; r--;)
	{
		bool b0 = bitRead(*p, 0);
		for (byte i = _byte_in_row - 1; i--;)
		{
			bool b = bitRead(*(p - 1), 0);
			*p >>= 1;
			bitWrite(*p, 7, b);
			p--;
		}

		*p >>= 1;
		if (recycle)
			bitWrite(*p, 7, b0);

		p += _byte_in_row + _byte_in_row - 1;
	}
}

void Block::moveBitInRowNega(bool recycle)
{
	byte *cache = new byte[_byte_in_row];
	memcpy(cache, _p, _byte_in_row);

	memcpy(_p, _p + _byte_in_row, _length - _byte_in_row);

	recycle ?
			memcpy(_p + _length - _byte_in_row, cache, _byte_in_row) :
			memset(_p + _length - _byte_in_row, 0x00, _byte_in_row);

	delete[] cache;
}

void Block::moveBitInRowPosi(bool recycle)
{
	byte *cache = new byte[_byte_in_row];
	memcpy(cache, _p + _length - _byte_in_row, _byte_in_row);

	memmove(_p + _byte_in_row, _p, _length - _byte_in_row);

	recycle ? memcpy(_p, cache, _byte_in_row) : memset(_p, 0x00, _byte_in_row);

	delete[] cache;
}

void Block::setMoveDirection(Direction d)
{
	switch (d)
	{
	case BIT_IN_COL_NEGA:
		_move = &Block::moveBitInColNega;
		break;
	case BIT_IN_COL_POSI:
		_move = &Block::moveBitInColPosi;
		break;
	case BIT_IN_ROW_NEGA:
		_move = &Block::moveBitInRowNega;
		break;
	case BIT_IN_ROW_POSI:
		_move = &Block::moveBitInRowPosi;
		break;
	}
}

void Block::move(bool recycle)
{
	(this->*_move)(recycle);
}

void Block::rotate(Rotation r)
{
	switch (r)
	{
	case R90:
		this->reverseInDiag();
		this->reverseInCol();
		break;
	case R180:
		this->reverseInCol();
		this->reverseInRow();
		break;
	case R270:
		this->reverseInDiag();
		this->reverseInRow();
		break;
	default:
		break;
	}
}
