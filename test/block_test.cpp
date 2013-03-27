#include "gtest/gtest.h"
#include "../block.h"

TEST(Block, testIsSquare)
{
	char p[32];
	Block block(p, 32, 2);
	EXPECT_TRUE(block.isSquare());
}
