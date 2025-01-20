#pragma once

#include "identifiable_base.h"

#include <memory>
#include <vector>

namespace cnr
{
class Hyperedge;

class Node : public IdentifiableBase
{
   friend class Hyperedge;
   friend class Hypergraph;

 public:
   Node();
   ~Node();
   Node(const Node& rhs) = delete;
   Node operator=(const Node& rhs) = delete;

   const std::vector<std::weak_ptr<Hyperedge>> getIncidentEdges();

   bool isAdjacentTo(const std::weak_ptr<Node>& vertex);
   bool isIncidentTo(const std::weak_ptr<Hyperedge>& edge);

   bool operator==(const Node& rhs) const;
   bool operator!=(const Node& rhs) const;

 protected:
   void addIncidentEdge(const std::weak_ptr<Hyperedge>& incidentEdge);
   void removeIncidentEdge(const std::weak_ptr<Hyperedge>& incidentEdge);

 private:
   std::vector<std::shared_ptr<Hyperedge>> incidentEdges_;
};
} // namespace cnr
