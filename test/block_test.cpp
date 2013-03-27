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
}
