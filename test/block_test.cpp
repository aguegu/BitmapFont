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
	EXPECT_EQ(0x12, Block::flipByte(0x48));
}

TEST(Block, testByteStringPure)
{
	EXPECT_STREQ("00", Block::byteStringPure(0x00).c_str());
	EXPECT_STREQ("ff", Block::byteStringPure(0xff).c_str());
	EXPECT_STREQ("e7", Block::byteStringPure(0xe7).c_str());
	EXPECT_STREQ("93", Block::byteStringPure(0x93).c_str());
}

TEST(Block, testByteString)
{
	EXPECT_STREQ("0x00", Block::byteString(0x00).c_str());
	EXPECT_STREQ("0xff", Block::byteString(0xff).c_str());
	EXPECT_STREQ("0xe7", Block::byteString(0xe7).c_str());
	EXPECT_STREQ("0x93", Block::byteString(0x93).c_str());
}
