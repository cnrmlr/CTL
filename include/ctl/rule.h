#pragma once
#include <algorithm>
#include <map>
#include <stdexcept>
#include <vector>

namespace cnr
{
using AbstractRelation     = std::vector<size_t>;
using AbstractRelationList = std::vector<AbstractRelation>;

struct Rule : public std::pair<AbstractRelationList, AbstractRelationList>
{
   using std::pair<AbstractRelationList, AbstractRelationList>::pair;
   Rule(const std::initializer_list<AbstractRelationList>& input);
   void Canonicalize();
};
} // namespace cnr
