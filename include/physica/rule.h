#pragma once

#include <utility>
#include <vector>

namespace cnr
{
// Abstract relations are defined as an ordered set of distinct nodes on the hypergraph
using AbstractRelation = std::vector<size_t>;

class Rule
{
 public:
   Rule();
   ~Rule();
   Rule(const Rule& rhs);
   Rule& operator=(const Rule& rhs);

   Rule(std::vector<AbstractRelation> pattern, std::vector<AbstractRelation> replace);

 private:
   std::vector<AbstractRelation> pattern_;
   std::vector<AbstractRelation> replace_;
};
} // namespace cnr
