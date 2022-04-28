#include <gtest/gtest.h>

#include <rna/basepair.h>
#include <rna/base.h>
#include <string>
#include <vector>

TEST(BasePair, Constructor) {
    BasePair bp(Base('A'), Base('U'), 0, 1);
    EXPECT_EQ(bp.getFirst(), Base::A);
    EXPECT_EQ(bp.getSecond(), Base::U);
    EXPECT_EQ(bp.getFirstIndex(), 0);
    EXPECT_EQ(bp.getSecondIndex(), 1);
}