#include "date_literal.h"
#include <gtest/gtest.h>

namespace date_literal {
namespace {

TEST(DateLiteralTest, MonthValue) {
  EXPECT_EQ(Month(1), Jan);
  EXPECT_EQ(Month(2), Feb);
  EXPECT_EQ(Month(3), Mar);
  EXPECT_EQ(Month(4), Apr);
  EXPECT_EQ(Month(5), May);
  EXPECT_EQ(Month(6), Jun);
  EXPECT_EQ(Month(7), Jul);
  EXPECT_EQ(Month(8), Aug);
  EXPECT_EQ(Month(9), Sep);
  EXPECT_EQ(Month(10), Oct);
  EXPECT_EQ(Month(11), Nov);
  EXPECT_EQ(Month(12), Dec);
}

TEST(DateLiteralTest, MonthWrapping) {
  EXPECT_EQ(Month(13), Jan);
  EXPECT_EQ(Month(25), Jan);
  EXPECT_EQ(Month(-1), Nov);
  EXPECT_EQ(Month(0), Dec);
}

TEST(DateLiteralTest, MonthValues) {
  EXPECT_EQ(Jan(), 1);
  // Implicit conversion to int.
  EXPECT_EQ(static_cast<int>(Jan), 1);
}

TEST(DateLiteralTest, MonthOperators) {
  Month m = Month(1);
  EXPECT_EQ(m++, Month(1));
  EXPECT_EQ(m, Month(2));
  EXPECT_EQ(m--, Month(2));
  EXPECT_EQ(m, Month(1));
  EXPECT_EQ(m += 2, Month(3));
  EXPECT_EQ(m -= 2, Month(1));
}

TEST(DateLiteralTest, MonthComparisons) {
  EXPECT_EQ(Jan, January);
  EXPECT_NE(Jan, Feb);
  EXPECT_LE(Jan, Feb);
}

TEST(DateLiteralTest, LeapYear) {
  EXPECT_TRUE(IsLeapYear(2024));
  EXPECT_FALSE(IsLeapYear(2025));
  EXPECT_FALSE(IsLeapYear(2026));
  EXPECT_FALSE(IsLeapYear(2027));
  EXPECT_TRUE(IsLeapYear(2000));
  EXPECT_FALSE(IsLeapYear(2100));
}

TEST(DateLiteralTest, DateValidity) {
  Date good(2025, Jan, 15);
  EXPECT_TRUE(good.IsValid());
  Date bad(2025, Jan, 32);
  EXPECT_FALSE(bad.IsValid());
  EXPECT_EQ(bad, Date::Invalid());
}

TEST(DateLiteralTest, DateConstruction) {
  EXPECT_EQ(Date(2025, Jan, 1), Jan / 1 / 2025);
  EXPECT_EQ(Date(2025, Jan, 1), 2025 / Jan / 1);
  EXPECT_EQ(Date(2025, Jan, 1), 1 / Jan / 2025);
  EXPECT_EQ(Date(2025, Jan, 1), Jan - 1 - 2025);
  EXPECT_EQ(Date(2025, Jan, 1), 2025 - Jan - 1);
  EXPECT_EQ(Date(2025, Jan, 1), 1 - Jan - 2025);
}

TEST(DateLiteralTest, DateOperators) {
  Date d = 2025 / Jan / 1;
  EXPECT_EQ(d++, 2025 / Jan / 1);
  EXPECT_EQ(d, 2025 / Jan / 2);
  Date l = 2025 / Jan / 31;
  EXPECT_EQ(++l, 2025 / Feb / 1);
  Date p = 2025 / Jan / 1;
  EXPECT_EQ(--p, 2024 / Dec / 31);
}

TEST(DateLiteralTest, DateLoop) {
  int count = 0;
  for (auto today = 2025 / Jan / 31; today < 2025 / Feb / 2; ++today) {
    ++count;
    EXPECT_TRUE(today.IsValid());
    EXPECT_GE(today, 2025 / Jan / 31);
    EXPECT_LE(today, 2025 / Feb / 1);
  }
  EXPECT_EQ(count, 2);
}

} // namespace

} // namespace date_literal
