#include "physica/wolfram_physics.h"
#include <gtest/gtest.h>

namespace cnr
{
namespace test
{
class RuleTest : public ::testing::Test
{
 public:
   RuleTest() {}

   ~RuleTest() {}

 protected:
   void SetUp() override {}

   void TearDown() override {}
};

TEST_F(RuleTest, CanonicalConstruct)
{
   cnr::Rule rule_1 = {{{1, 1}, {2, 4, 5}, {7, 5}}, {{3, 8}, {2, 7}, {5, 4, 1}, {4, 6}, {5, 1, 7}}};

   cnr::Rule rule_2 = {{{7, 3}, {4, 4}, {8, 5, 3}}, {{3, 4, 7}, {5, 6}, {8, 7}, {3, 5, 4}, {1, 2}}};

   cnr::Rule rule_3 = {{{2, 2}, {3, 5, 6}, {8, 6}}, {{4, 9}, {3, 8}, {6, 5, 2}, {5, 7}, {6, 2, 8}}};

   EXPECT_EQ(rule_1, rule_2);
   EXPECT_EQ(rule_1, rule_3);
   EXPECT_EQ(rule_2, rule_3);
}
} // namespace test
} // namespace cnr