#include "physica/wolfram_physics/rule.h"

#include <functional>
#include <vector>

namespace cnr
{
Rule::Rule(const std::initializer_list<AbstractRelationList>& input)
{
   if (input.size() != 2)
   {
      throw std::invalid_argument(
         "Initializer list for Rule must contain exactly two AbstractRelationLists");
   }

   this->first  = std::move(*input.begin());
   this->second = std::move(*(input.begin() + 1));

   Canonicalize();
}

void Rule::Canonicalize()
{
   std::map<size_t, size_t> canonicalValueMap;
   size_t canonicalValue = 1;

   auto Sort = [](AbstractRelationList& list) {
      std::sort(list.begin(), list.end(), [](const AbstractRelation& a, const AbstractRelation& b) {
         if (a.size() != b.size())
         {
            return a.size() > b.size();
         }

         return a < b;
      });
   };

   auto Map = [&](AbstractRelationList& list) {
      for (auto& relation : list)
      {
         for (auto& value : relation)
         {
            if (canonicalValueMap.find(value) == canonicalValueMap.end())
            {
               canonicalValueMap[value] = canonicalValue++;
            }

            value = canonicalValueMap[value];
         }
      }
   };

   Sort(this->first);
   Map(this->first);
   Sort(this->second);
   Map(this->second);
   Sort(this->second);
}
} // namespace cnr
