#include "physica/rule.h"

namespace cnr
{
Rule::Rule() {}

Rule::~Rule() {}

Rule::Rule(const Rule& rhs) : pattern_(rhs.pattern_), replace_(rhs.replace_) {}

Rule& Rule::operator=(const Rule& rhs)
{
   if (this != &rhs)
   {
      pattern_ = rhs.pattern_;
      replace_ = rhs.replace_;
   }

   return *this;
}

Rule::Rule(std::vector<AbstractRelation> pattern, std::vector<AbstractRelation> replace)
   : pattern_(pattern), replace_(replace)
{
}
} // namespace cnr
