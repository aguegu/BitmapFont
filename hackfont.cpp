// HackFont does not support non-square font right now.
// supports HZK16, HZK32, HZK24, HZK32, HZK40, HZK48
//
// command format:
// ./HackFont [source_font] [destination_font] [argv_shift] [argv_movedirection] [argv_step]
//
// argv_shift - bit7: byte_reverse, bit6: opposite, bit5: 270, bit4: 180, bit3: 90, 
// 				bit2: reverse_diagonal, bit1: reverse_column, bit0: reverse_row
//
// argv_movedirection - 0x01: Column +, right
// 						0x02: Column -, left
// 						0x04: Row +, down
// 						0x08: Row -, up
//
// argv_step: how may step you want to move in the [argv_movedirection]

#include <iostream>
#include <fstream>
#include <cstring>
#include "block.h"
#include <cstdlib>

using namespace std;
typedef unsigned char byte;

void shiftBlock(Block & block, int shift)
{
	if (bitRead(shift, 0))
		block.slipInRow();
	if (bitRead(shift, 1))	
		block.slipInCol();
	if (bitRead(shift, 2))
		block.slipInDiag();
	if (bitRead(shift, 3))
		block.rotate(Block::R90);
	if (bitRead(shift, 4))
		block.rotate(Block::R180);
	if (bitRead(shift, 5))
		block.rotate(Block::R270);
	if (bitRead(shift, 6))
		block.opposite();
	if (bitRead(shift, 7))
		block.reverse();
}

void moveBlock(Block & block, int direction, int step)
{
	switch (direction & 0x0f)
	{
		case 0x01:
			block.setMoveDirection(Block::BIT_IN_COL_POSI);
			break;
		case 0x02:
			block.setMoveDirection(Block::BIT_IN_COL_NEGA);
			break;
		case 0x04:
			block.setMoveDirection(Block::BIT_IN_ROW_POSI);
			break;
		case 0x08:
			block.setMoveDirection(Block::BIT_IN_ROW_NEGA);
			break;
		default:
			step = 0;
			break;
	}

	while (step--)
	{
		block.move(direction & 0x10);
	}
}

int main(int argc, char* argv[])
{
	int row_count;
	sscanf(argv[1], "%*[^/]/%*3s%d", &row_count);

	int byte_in_row = row_count / 8;
	int length = byte_in_row * row_count;

	int shift, move, step; 
	sscanf(argv[3], "0x%02x", &shift);   
	sscanf(argv[4], "0x%02x", &move);
	sscanf(argv[5], "0x%02x", &step);

	ifstream fin(argv[1], ios::binary);
	ofstream fout(argv[2], ios::binary);

	char * p = new char[length];
	while (fin.read(p, length))
	{
		Block block(p, length, byte_in_row);
		
		shiftBlock(block, shift);
		moveBlock(block, move, step);

		fout.write(p, length);
		fout.flush();
	}
	delete[] p;

	fin.close();
	fout.close();

	exit(0) ;
}
