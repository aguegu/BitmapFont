#include "gtest/gtest.h"
#include "../block.h"

TEST(Block, testIsSquare)
{
	char p[32];
	Block block(p, 32, 2);
	EXPECT_TRUE(block.isSquare());

	Block block2(p, 16, 2);
	EXPECT_FALSE(block2.isSquare());
}

TEST(Block, testSlipByte)
{
	EXPECT_EQ(0x01, Block::flipByte(0x80));
	EXPECT_EQ(0x0f, Block::flipByte(0xf0));
	EXPECT_EQ(0xc0, Block::flipByte(0x03));
	
}
