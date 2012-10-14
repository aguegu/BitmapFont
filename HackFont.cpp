#include <iostream>
#include <fstream>
#include <cstring>
#include "Block.h"
#include <cstdlib>

using namespace std;
typedef unsigned char byte;

void shiftBlock(Block & block, int shift)
{

	if (bitRead(shift, 0))
		block.reverseInRow();
	if (bitRead(shift, 1))	
		block.reverseInCol();
	if (bitRead(shift, 2))
		block.reverseInDiag();
	if (bitRead(shift, 3))
		block.rotate(Block::R90);
	if (bitRead(shift, 4))
		block.rotate(Block::R180);
	if (bitRead(shift, 5))
		block.rotate(Block::R270);
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

	while(step--)
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

	int shift = atoi(argv[3]);
	int move = atoi(argv[4]);
	int step = atoi(argv[5]);
	int opposite = atoi(argv[6]);

	ifstream fin(argv[1], ios::binary);
	ofstream fout(argv[2], ios::binary);

	char * p = new char[length];
	while(fin.read(p, length))
	{
		Block block(p, length);
		
		shiftBlock(block, shift);
		moveBlock(block, move, step);

		if (bitRead(opposite, 0))
			block.opposite();
		if (bitRead(opposite, 1))
			block.reverse();

		fout.write(p, length);
		fout.flush();
	}
	delete[] p;

	fin.close();
	fout.close();

	return 0;
}
