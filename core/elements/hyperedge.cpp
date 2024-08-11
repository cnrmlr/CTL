#include "common_utility.h"
#include "hyperedge.h"
#include "node.h"
#include "identifiable_base.h"

namespace graphica
{
namespace core
{
namespace elements
{
Hyperedge::Hyperedge()
   : IdentifiableBase()
   , nodes_(0)
{
}

Hyperedge::Hyperedge(const std::vector<std::shared_ptr<Node>>& nodes)
   : IdentifiableBase()
{
   nodes_.reserve(nodes.size());

   for (auto& node : nodes)
   {
      nodes_.push_back(node);
   }
}

Hyperedge::~Hyperedge()
{
}

const std::vector<std::weak_ptr<Node>> Hyperedge::getNodes() const
{
   return utility::MakeWeakPtrVector(nodes_);
}

bool Hyperedge::isAdjacentTo(const std::weak_ptr<Hyperedge>& edge)
{
   for (auto& node : nodes_)
   {
      if (edge.lock() && edge.lock()->isIncidentTo(node))
      {
         return true;
      }
   }

   return false;
}

bool Hyperedge::isIncidentTo(const std::weak_ptr<Node>& vertex)
{
   return findNode(vertex) != nodes_.end();
}

const std::vector<std::shared_ptr<Node>>::iterator Hyperedge::findNode(std::weak_ptr<Node> node)
{
   return utility::FindWithWeakPtr(nodes_, node);
}

void Hyperedge::removeNode(std::weak_ptr<Node> node)
{
   auto nodeIter = findNode(node);
   
   if (nodeIter != nodes_.end())
   {
      nodes_.erase(findNode(node));
   }
}

void Hyperedge::removeFromNodeIncidenceLists()
{
   for (auto& node : nodes_)
   {
      node->removeIncidentEdge(shared_from_this());
   }
}

bool Hyperedge::operator==(const Hyperedge& rhs) const
{
   return IdentifiableBase::operator==(rhs);
}

bool Hyperedge::operator!=(const Hyperedge& rhs) const
{
   return IdentifiableBase::operator!=(rhs);
}
}
}
}