#include "common_utility.h"
#include "hyperedge.h"
#include "node.h"
#include "pattern_match.h"

#include <functional>
#include <map>
#include <set>

namespace graphica
{
namespace core
{
namespace functions
{
struct CompareOrder;
using ElementToNodeMap = std::map<size_t, std::shared_ptr<elements::Node>>;
using ElementSet = std::set<size_t>;
using NodeSet = std::set<std::shared_ptr<elements::Node>, CompareOrder>;
using NodeVector = std::vector<std::shared_ptr<elements::Node>>;

struct CompareOrder 
{
   CompareOrder(const NodeVector& edgeNodes)
   {
      for (size_t i = 0; i < edgeNodes.size(); ++i) 
      {
         if (orderMap.find(edgeNodes[i]) == orderMap.end()) 
         {
            orderMap[edgeNodes[i]] = i;
         }
      }
   }

   std::unordered_map<std::shared_ptr<elements::Node>, size_t> orderMap;

   bool operator()(const std::shared_ptr<elements::Node>& lhs, const std::shared_ptr<elements::Node>& rhs) const {
      return orderMap.at(lhs) < orderMap.at(rhs);
   }
};

static NodeSet GetUniqueNodesInEdge(const NodeVector& edgeNodes)
{
   NodeSet uniqueNodesInEdge(*std::make_shared<CompareOrder>(edgeNodes).get());

   for (const auto& node : edgeNodes)
   {
      uniqueNodesInEdge.insert(node);
   }

   return uniqueNodesInEdge;
}

// This function maps the unique elements in the pattern to the unique elements in the edge { size_t, std::shared_ptr<Node> }
static ElementToNodeMap MapUniqueElementsToUniqueNodes(ElementSet uniquePatternElements, NodeSet uniqueEdgeNodes)
{
   ElementToNodeMap map;

   if (uniquePatternElements.size() != uniqueEdgeNodes.size())
   {
      return map; // at this point in the algorithm the sizes of these containers must match
   }

   // Now that we have verified that the unique sets are of the same size we declare iters at the first elements of each
   auto uniquePatternElementsIter = uniquePatternElements.begin();
   auto uniqueEdgeNodesIter = uniqueEdgeNodes.begin();

   // Local lambda function for determining if we should continue iterating through the two sets
   std::function<bool()> shouldContinueLoop = [&uniquePatternElementsIter, &uniqueEdgeNodesIter, &uniquePatternElements, &uniqueEdgeNodes]()
   {
      return uniquePatternElementsIter != uniquePatternElements.end() && uniqueEdgeNodesIter != uniqueEdgeNodes.end();
   };

   // Local lambda function for incremeanting the iterators operating each unique set
   std::function<void()> incremeantIterators = [&uniquePatternElementsIter, &uniqueEdgeNodesIter]()
   {
      ++uniquePatternElementsIter;
      ++uniqueEdgeNodesIter;
   };

   // The actual loop that maps each unique pattern element to a unique node
   for (; shouldContinueLoop(); incremeantIterators())
   {
      map[*uniquePatternElementsIter] = *uniqueEdgeNodesIter;
   }

   return map;
}

static bool EdgeMatchesPattern(const elements::AbstractRelation pattern, const NodeVector& edgeNodes, const ElementToNodeMap& map)
{
   if (pattern.size() != edgeNodes.size())
   {
      return false; // at this point in the algorithm, the sizes of these containers must match
   }

   for (size_t i = 0; i < pattern.size() && i < edgeNodes.size(); ++i)
   {
      if (map.at(pattern.at(i)) == edgeNodes.at(i))
      {
         continue;
      }
      else
      {
         return false;
      }
   }

   return true;
}

std::weak_ptr<elements::Hyperedge> PatternMatch(const elements::AbstractRelation pattern, 
                                                const std::vector<std::weak_ptr<elements::Hyperedge>>& edges)
{
   ElementSet uniqueElementsInPattern(pattern.begin(), pattern.end());

   for (const auto& weakEdge : edges)
   {
      auto edge = weakEdge.lock();

      if (!edge)
      {
         continue; // node must be valid to be a match, skipping...
      }

      const NodeVector& edgeNodes = utility::MakeSharedPtrVector(edge->getNodes());

      if (edgeNodes.size() == 0 || edgeNodes.size() != pattern.size())
      {
         continue; // number of nodes in edge must equal number of elements in pattern to be a match, skipping...
      }

      const NodeSet uniqueNodesInEdge = GetUniqueNodesInEdge(edgeNodes);

      if (uniqueElementsInPattern.size() != uniqueNodesInEdge.size())
      {
         continue; // number of unique nodes in edge must equal number of unique elements in pattern to be a match, skipping...
      }

      const ElementToNodeMap map = MapUniqueElementsToUniqueNodes(uniqueElementsInPattern, uniqueNodesInEdge);

      if (!map.empty() && EdgeMatchesPattern(pattern, edgeNodes, map))
      {
         return edge; // all matching criteria have been met, return this edge
      }
   }

   return std::weak_ptr<elements::Hyperedge>(); // none of the edges in the set match the pattern, return nullptr
}
}
}
}
