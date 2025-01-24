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

   cnr::Rule rule_3 = {{{1, 2, 3}, {4, 4}, {5, 3}}, {{3, 2, 4}, {3, 4, 5}, {1, 5}, {2, 8}, {6, 7}}};

   cnr::Rule canonical = {{{1, 2, 3}, {4, 4}, {5, 3}},
                          {{3, 2, 4}, {3, 4, 5}, {1, 5}, {2, 6}, {7, 8}}};

   EXPECT_EQ(rule_1, rule_2);
   EXPECT_EQ(rule_1, rule_3);
   EXPECT_EQ(rule_2, rule_3);

   EXPECT_EQ(rule_1, canonical);
   EXPECT_EQ(rule_2, canonical);
   EXPECT_EQ(rule_3, canonical);
}
} // namespace test
} // namespace cnr